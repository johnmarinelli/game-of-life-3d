#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "HandleManager.hpp"
#include "Components/Components.hpp"
#include "Entity.hpp"
#include "GameOfLifeSystem.hpp"
#include "DrawingSystem.hpp"
#include "ComponentFactory.hpp"
#include "Mesh.h"
#include "EntityFactory.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

const int ROWS = 5;
const int COLS = 5;
const int DEPTH = 5;

static const GLfloat cube_colors[] = {
 // front colors
 1.0, 0.0, 0.0,
 0.0, 1.0, 0.0,
 0.0, 0.0, 1.0,
 1.0, 1.0, 1.0,
 // back colors
 1.0, 0.0, 0.0,
 0.0, 1.0, 0.0,
 0.0, 0.0, 1.0,
 1.0, 1.0, 1.0,
 };

class GameOfLifeApp : public App {
public:
  void setup() override;
  void keyDown(cinder::app::KeyEvent event) override;
  void mouseDown( MouseEvent event ) override;
  void update() override;
  void draw() override;
  
  gl::GlslProgRef mGlsl;
  GLuint mCubeVertices, mCubeColors, mCubeNormals, mCubeIBO, mVao;
  
  struct {
    GLuint model_matrix;
    GLuint proj_matrix;
    GLuint view_matrix;
    GLuint mv_matrix;
  } mUniforms;
  
  john::HandleManager mHandleManager;
  john::DrawingSystem mDrawingSystem;
  john::GameOfLifeSystem mGameOfLifeSystem;
  
  john::EntityFactory mEntityFactory{mHandleManager};
  std::vector<std::shared_ptr<john::Entity>> mEntities;
  
  john::ComponentFactory<john::PositionComponent> mPositionComponentFactory{mHandleManager};

};

void GameOfLifeApp::setup()
{
  john::mesh::cube::calculateCubeNormals();
  
  glm::mat4 projMatrix = glm::perspective(50.f, getWindowAspectRatio(), 0.1f, 1000.f);
  glm::mat4 viewMatrix = glm::translate(glm::mat4{}, glm::vec3{0.f, 0.f, -100.f});
  
  mDrawingSystem.initialize(projMatrix, viewMatrix);

  auto size = Display::getMainDisplay()->getSize();
  
  auto xScale = (size.x / (float) COLS) * 0.001;
  auto yScale = (size.y / (float) ROWS) * 0.001;
  auto scale = (xScale + yScale) / 2.f;
  
  float marginWidth = 1.f * scale;
  
  for (auto i = 0; i < ROWS; i++) {
    for (auto j = 0; j < COLS; j++) {
      auto entityHdl = mEntityFactory.addEntity();
      auto entity = static_cast<john::Entity*>(mHandleManager.get(entityHdl));
      
      auto positionComponent = mPositionComponentFactory.create();
      positionComponent->rotationComponent.rotation = glm::mat4{};
      positionComponent->scaleComponent.scale = glm::scale(glm::mat4{}, glm::vec3{scale, scale, scale});
      positionComponent->translationComponent.translation = glm::translate(glm::mat4{}, glm::vec3{j + marginWidth, i + marginWidth, 1});//glm::mat4{};
      
      auto positionComponentHdl = mHandleManager.add(static_cast<void*>(positionComponent.get()), john::ComponentTypes::C_POSITION);
      entity->mComponents[john::ComponentTypes::C_POSITION] = positionComponentHdl;
      
      mDrawingSystem.registerEntity(entityHdl);
    }
  }
  
  mGlsl = gl::GlslProg::create(loadResource("gol.vs"), loadResource("gol.fs"));
  
  mUniforms.model_matrix = glGetUniformLocation(mGlsl->getHandle(), "model_matrix");
  mUniforms.mv_matrix = glGetUniformLocation(mGlsl->getHandle(), "mv_matrix");
  
  mDrawingSystem.mProjectionMatrixHandle = glGetUniformLocation(mGlsl->getHandle(), "proj_matrix");
  mDrawingSystem.mViewMatrixHandle = glGetUniformLocation(mGlsl->getHandle(), "view_matrix");
  mDrawingSystem.mModelMatrixHandle = glGetUniformLocation(mGlsl->getHandle(), "model_matrix");
  mDrawingSystem.mModelViewMatrixHandle = glGetUniformLocation(mGlsl->getHandle(), "mv_matrix");
  
  glGenVertexArrays(1, &mVao);
  glBindVertexArray(mVao);
  
  glGenBuffers(1, &mCubeVertices);
  glGenBuffers(1, &mCubeIBO);
  glGenBuffers(1, &mCubeNormals);
  glGenBuffers(1, &mCubeColors);

  glBindBuffer(GL_ARRAY_BUFFER, mCubeVertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(john::mesh::cube::vertices), john::mesh::cube::vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCubeIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(john::mesh::cube::indexOrder),
               john::mesh::cube::indexOrder, GL_STATIC_DRAW);
  
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  glBindBuffer(GL_ARRAY_BUFFER, mCubeNormals);
  glBufferData(GL_ARRAY_BUFFER, sizeof(john::mesh::cube::rawNormals), john::mesh::cube::rawNormals, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(1);
  
  glBindBuffer(GL_ARRAY_BUFFER, mCubeColors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(2);
}

void GameOfLifeApp::keyDown(cinder::app::KeyEvent event)
{
  auto input = event.getChar();
  
  switch (input) {
    default: break;
  }
}

void GameOfLifeApp::mouseDown( MouseEvent event )
{
}

void GameOfLifeApp::update()
{
}

void GameOfLifeApp::draw()
{
  glViewport(0, 0, getWindowWidth(), getWindowHeight());
  mGlsl->bind();
  auto elapsed = getElapsedSeconds();
  mDrawingSystem.perform(mHandleManager, elapsed);
}

CINDER_APP( GameOfLifeApp, RendererGl )
