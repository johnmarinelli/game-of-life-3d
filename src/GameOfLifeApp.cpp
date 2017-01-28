#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "HandleManager.hpp"
#include "Components/Components.hpp"
#include "Entity.hpp"
#include "MovementSystem.hpp"
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

// { :componentType => { :entitiesHavingThisComponent => [], :component handles => [] }
std::map<john::ComponentTypes,
  std::pair<std::vector<john::Handle>, std::vector<john::Handle>>> components;

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
  john::MovementSystem mMovementSystem;
  john::DrawingSystem mDrawingSystem;
  john::ComponentFactory<john::XyzComponent> mXyzComponentFactory{mHandleManager};
};

std::shared_ptr<john::XyzComponent> xyzc;
std::shared_ptr<john::VelocityComponent> velc;
std::shared_ptr<john::Entity> entity;
std::shared_ptr<john::Entity> entity2;

void GameOfLifeApp::setup()
{
  john::mesh::cube::calculateCubeNormals();
  
  glm::mat4 projMatrix = glm::perspective(50.f, getWindowAspectRatio(), 0.1f, 1000.f);
  mDrawingSystem.mProjectionMatrix = projMatrix;
  
  glm::mat4 viewMatrix = glm::translate(glm::mat4{}, glm::vec3{0.f, 0.f, -100.f});
  mDrawingSystem.mViewMatrix = viewMatrix;
  
  // Create a cartesian coordinate component.
  xyzc = std::make_shared<john::XyzComponent>();
  
  // Register it with the handle manager.
  // Receive a handle.
  auto xyzHdl = mHandleManager.add(static_cast<void*>(xyzc.get()), john::ComponentTypes::C_XYZ);
  
  // Set its properties.
  xyzc->xyz = glm::vec3{1.f};

  // Velocity component.
  velc = std::make_shared<john::VelocityComponent>();
  auto velocityHdl = mHandleManager.add(static_cast<void*>(velc.get()), john::ComponentTypes::C_VELOCITY);
  velc->velocity = glm::vec3{0.5f};
  
  // Store handle by type.
  components[john::ComponentTypes::C_XYZ].second.push_back(xyzHdl);
  components[john::ComponentTypes::C_VELOCITY].second.push_back(velocityHdl);

  // Create an entity.
  entity = std::make_shared<john::Entity>();
  entity->mComponents[john::ComponentTypes::C_XYZ] = xyzHdl;
  entity->mComponents[john::ComponentTypes::C_VELOCITY] = velocityHdl;
  auto entityHdl = mHandleManager.add(static_cast<void*>(entity.get()), john::ENTITY);
  entity->mHandle = entityHdl;
  
  auto entityr = mHandleManager.get(entityHdl);
  
  // Store an entity in the components 'cache'
  // NOTE: this isn't really a cache its just.....ugly
  components[john::ComponentTypes::C_XYZ].first.push_back(entityHdl);
  components[john::ComponentTypes::C_VELOCITY].first.push_back(entityHdl);

  // Entity 2.
  using namespace john;
  using namespace std;

  entity2 = make_shared<Entity>();
  auto entity2Hdl = mHandleManager.add(static_cast<void*>(entity2.get()), ENTITY);
  entity2->mHandle = entity2Hdl;
  shared_ptr<XyzComponent> xyzc2 = make_shared<XyzComponent>();
  auto xyz2Hdl = mHandleManager.add(static_cast<void*>(xyzc2.get()), ComponentTypes::C_XYZ);
  xyzc2->xyz = glm::vec3{1.f};
  auto velc2 = make_shared<VelocityComponent>();
  auto velc2Hdl = mHandleManager.add(static_cast<void*>(velc2.get()), ComponentTypes::C_VELOCITY);
  velc2->velocity = glm::vec3{-0.5f};
  
  components[ComponentTypes::C_XYZ].second.push_back(xyz2Hdl);
  components[ComponentTypes::C_VELOCITY].second.push_back(velc2Hdl);
  
  entity2->mComponents[john::ComponentTypes::C_XYZ] = xyz2Hdl;
  entity2->mComponents[john::ComponentTypes::C_VELOCITY] = velc2Hdl;
  components[john::ComponentTypes::C_XYZ].first.push_back(entity2Hdl);
  components[john::ComponentTypes::C_VELOCITY].first.push_back(entity2Hdl);
  
  // Register entities with a system.
  mMovementSystem.registerEntity(entityHdl);
  mMovementSystem.registerEntity(entity2Hdl);
  
  mGlsl = gl::GlslProg::create(loadResource("gol.vs"), loadResource("gol.fs"));
  
  mUniforms.model_matrix = glGetUniformLocation(mGlsl->getHandle(), "model_matrix");
  //mUniforms.proj_matrix = glGetUniformLocation(mGlsl->getHandle(), "proj_matrix");
  //mUniforms.view_matrix = glGetUniformLocation(mGlsl->getHandle(), "view_matrix");
  mUniforms.mv_matrix = glGetUniformLocation(mGlsl->getHandle(), "mv_matrix");
  
  mDrawingSystem.mProjectionMatrixHandle = glGetUniformLocation(mGlsl->getHandle(), "proj_matrix");
  mDrawingSystem.mViewMatrixHandle = glGetUniformLocation(mGlsl->getHandle(), "view_matrix");
  
  glGenVertexArrays(1, &mVao);
  glBindVertexArray(mVao);
  
  glGenBuffers(1, &mCubeVertices);
  glGenBuffers(1, &mCubeIBO);
  glGenBuffers(1, &mCubeNormals);
  glGenBuffers(1, &mCubeColors);

  glBindBuffer(GL_ARRAY_BUFFER, mCubeVertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mesh::cube::vertices), mesh::cube::vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCubeIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(mesh::cube::indexOrder),
               mesh::cube::indexOrder, GL_STATIC_DRAW);
  
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  glBindBuffer(GL_ARRAY_BUFFER, mCubeNormals);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mesh::cube::rawNormals), mesh::cube::rawNormals, GL_STATIC_DRAW);
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
  mMovementSystem.update(mHandleManager);
}

void GameOfLifeApp::draw()
{
  static const GLfloat one = 1.f;
  static const GLfloat bg[] = {0.f, 0.0f, 0.f, 1.f};
  
  glViewport(0, 0, getWindowWidth(), getWindowHeight());
  glClearBufferfv(GL_COLOR, 0, bg);
  glClearBufferfv(GL_DEPTH, 0, &one);
  
  mGlsl->bind();
  
  glUniformMatrix4fv(mDrawingSystem.mProjectionMatrixHandle,
                     1,
                     GL_FALSE,
                     glm::value_ptr(mDrawingSystem.mProjectionMatrix));
  
  auto seconds = getElapsedSeconds() * 0.01;
  float f = seconds * 0.03;
  
  glUniformMatrix4fv(mDrawingSystem.mViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(mDrawingSystem.mViewMatrix));
  
  auto translationMatrix = glm::translate(glm::mat4{},
                                          glm::vec3{sinf(2.1f * f) * 0.5f,
                                            cosf(1.7f * f) * 0.5f,
                                            sinf(1.3f * f) * cosf(1.5f * f) * 2.f});
  
  auto rotationMatrix = glm::rotate(glm::mat4{}, (float)seconds * 45.f, glm::vec3{0.f, 1.f, 0.f});
  
  auto scaleMatrix = glm::scale(glm::mat4(), glm::vec3(2.f));
  
  auto modelMatrix = translationMatrix * rotationMatrix * scaleMatrix * glm::mat4{};
  glUniformMatrix4fv(mUniforms.model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
  
  auto mvMatrix = mDrawingSystem.mViewMatrix * modelMatrix;
  glUniformMatrix4fv(mUniforms.mv_matrix, 1, GL_FALSE, glm::value_ptr(mvMatrix));
  
  //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
  
  for (int i = 0; i < 100; ++i) {
    auto modelMatrix2 = glm::translate(glm::mat4{}, glm::vec3{i * 0.1f, 0.f, 1.f}) *
    rotationMatrix *
    glm::scale(glm::mat4{}, glm::vec3{0.5f}) *
    glm::mat4{};
    
    glUniformMatrix4fv(mUniforms.model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix2));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
  }
}

CINDER_APP( GameOfLifeApp, RendererGl )
