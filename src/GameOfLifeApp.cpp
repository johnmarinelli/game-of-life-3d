#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "HandleManager.hpp"
#include "Components/Components.hpp"
#include "Entity.hpp"
#include "systems/GameOfLifeSystem.hpp"
#include "systems/DrawingSystem.hpp"
#include "factories/ComponentFactory.hpp"
#include "Mesh.h"
#include "factories/EntityFactory.hpp"
#include "Camera.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

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
  void mouseDrag(MouseEvent event) override;
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
  
  john::Camera mCamera;
  
  john::HandleManager mHandleManager;
  john::DrawingSystem mDrawingSystem;
  john::GameOfLifeSystem mGameOfLifeSystem{mHandleManager};
  
  john::EntityFactory mEntityFactory{mHandleManager};
  std::vector<std::shared_ptr<john::Entity>> mEntities;
  
  john::ComponentFactory<john::PositionComponent> mPositionComponentFactory{mHandleManager};
  john::ComponentFactory<john::GridPositionComponent> mGridPositionComponentFactory{mHandleManager};
  john::ComponentFactory<john::StateComponent> mStateComponentFactory{mHandleManager};

};

void GameOfLifeApp::setup()
{
  setFrameRate(20.0);
  john::mesh::cube::calculateCubeNormals();
  
  glm::mat4 projMatrix = glm::perspective(50.f, getWindowAspectRatio(), 0.1f, 1000.f);
  mCamera = john::Camera{glm::vec3{0,0,-50}, glm::vec3{0,0,0}, glm::vec3{0,1,0}};
  
  mDrawingSystem.initialize(projMatrix, mCamera);

  auto size = Display::getMainDisplay()->getSize();
  
  auto xScale = (size.x / (float) john::constants::COLS) * 0.001;
  auto yScale = (size.y / (float) john::constants::ROWS) * 0.001;
  auto scale = (xScale + yScale) / 2.f;
  
  float marginWidth = 1.f * scale;
  
  for (auto k = 0; k < john::constants::DEPTH; k++) {
    for (auto i = 0; i < john::constants::ROWS; i++) {
      for (auto j = 0; j < john::constants::COLS; j++) {
        auto entityHdl = mEntityFactory.addEntity();
        auto entity = static_cast<john::Entity*>(mHandleManager.get(entityHdl));
        
        auto rotation = glm::mat4{};
        auto scaleVec = glm::scale(glm::mat4{}, glm::vec3{scale, scale, scale});
        auto translation = glm::translate(glm::mat4{},
                                          glm::vec3{j + marginWidth, i + marginWidth, k + marginWidth});
        
        auto positionComponent = mPositionComponentFactory.create(rotation,
                                                                  translation,
                                                                  scaleVec, glm::mat4{1.f});
        
        auto positionComponentHdl = mHandleManager.add(static_cast<void*>(positionComponent.get()), john::ComponentTypes::C_POSITION);
        entity->mComponents[john::ComponentTypes::C_POSITION] = positionComponentHdl;
        
        // gol coordinate system
        auto gridPositionComponent = mGridPositionComponentFactory.create(glm::vec3{j,i,k});
        auto gridPositionComponentHdl = mHandleManager.add(static_cast<void*>(gridPositionComponent.get()), john::ComponentTypes::C_GRIDPOSITION);
        
        // gol state system
        auto stateComponent = mStateComponentFactory.create();
        
        if (i == 1 && j == 1) stateComponent->on = true;
        if (i == 2 && j == 2) stateComponent->on = true;
        if (i == 3 && j == 0) stateComponent->on = true;
        if (i == 3 && j == 1) stateComponent->on = true;
        if (i == 3 && j == 2) stateComponent->on = true;
        
        auto stateComponentHdl = mHandleManager.add(static_cast<void*>(stateComponent.get()), john::ComponentTypes::C_STATE);
        
        entity->mComponents[john::ComponentTypes::C_POSITION] = positionComponentHdl;
        entity->mComponents[john::ComponentTypes::C_GRIDPOSITION] = gridPositionComponentHdl;
        entity->mComponents[john::ComponentTypes::C_STATE] = stateComponentHdl;
        
        mDrawingSystem.registerEntity(entityHdl);
        mGameOfLifeSystem.registerEntity(entityHdl);
      }
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
  mCamera.KeyPressed(input);
  
  switch (input) {
    default: break;
  }
}

void GameOfLifeApp::mouseDrag(MouseEvent event)
{
  auto mouseLoc = event.getPos();
  mCamera.MouseMove(mouseLoc.x, mouseLoc.y, getWindowWidth(), getWindowHeight(), event.isLeft());
}

void GameOfLifeApp::update()
{
  mGameOfLifeSystem.perform(mHandleManager, 0.0);  
}

void GameOfLifeApp::draw()
{
  glViewport(0, 0, getWindowWidth(), getWindowHeight());
  mGlsl->bind();
  auto elapsed = getElapsedSeconds();
  mDrawingSystem.perform(mHandleManager, elapsed);
}

CINDER_APP( GameOfLifeApp, RendererGl )
