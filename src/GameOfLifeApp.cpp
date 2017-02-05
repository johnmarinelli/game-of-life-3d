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
  void initEntity(const glm::mat4& translate = glm::mat4{});
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
  
  std::vector<std::shared_ptr<john::Entity>> mEntities;

  john::ComponentFactory<john::ModelMatrixComponent> mModelMatrixComponentFactory{mHandleManager};

};

//std::shared_ptr<john::Entity> entity;

void GameOfLifeApp::initEntity(const glm::mat4& translate)
{
  auto entity = std::make_shared<john::Entity>();
  auto entityHdl = mHandleManager.add(static_cast<void*>(entity.get()), john::ENTITY);
  entity->mHandle = entityHdl;
  
  auto modelMatrixComponent = mModelMatrixComponentFactory.create();
  modelMatrixComponent->rotationComponent.rotation = glm::mat4{};
  modelMatrixComponent->scaleComponent.scale = glm::mat4{};
  modelMatrixComponent->translationComponent.translation = translate;//glm::mat4{1.f};
  
  auto modelMatrixComponentHdl = mHandleManager.add(static_cast<void*>(modelMatrixComponent.get()), john::ComponentTypes::C_MODELMAT);
  modelMatrixComponent->handle = modelMatrixComponentHdl;
  
  entity->mComponents[john::ComponentTypes::C_MODELMAT] = modelMatrixComponentHdl;
  
  mDrawingSystem.registerEntity(entityHdl);
  mEntities.push_back(entity);
}

void GameOfLifeApp::setup()
{
  john::mesh::cube::calculateCubeNormals();
  
  glm::mat4 projMatrix = glm::perspective(50.f, getWindowAspectRatio(), 0.1f, 1000.f);
  glm::mat4 viewMatrix = glm::translate(glm::mat4{}, glm::vec3{0.f, 0.f, -100.f});
  
  mDrawingSystem.initialize(projMatrix, viewMatrix);
  
  initEntity(glm::mat4{1.f});
  initEntity(glm::translate(glm::mat4{}, glm::vec3{1.f, 2.f, 0.f}));

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
