#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "DrawingSystem.hpp"
#include "HandleManager.hpp"
#include "Entity.hpp"
#include "Camera.hpp"

namespace john {
  
  DrawingSystem::DrawingSystem() :
    mCameraRef(nullptr),
    mLightPositionWorldspaceHandle(-1)
  {
    mLightPosition[0] = 0.f;
    mLightPosition[1] = 0.f;
    mLightPosition[2] = 100.f;
  }
  
  void DrawingSystem::initialize(john::Camera& camera)
  {
    mCameraRef = &camera;
  }
  
  void DrawingSystem::registerEntity(const john::Handle& handle)
  {
    mHandles.push_back(handle);
  }
  
  void DrawingSystem::perform(const john::HandleManager& handleManager, double elapsedSeconds)
  {
    auto projMatrix = mCameraRef->mProjectionMatrix;
    auto viewMatrix = mCameraRef->mViewMatrix;

    static const GLfloat one = 1.f;
    static const GLfloat bg[] = {0.f, 0.0f, 0.f, 1.f};
    
    glClearBufferfv(GL_COLOR, 0, bg);
    glClearBufferfv(GL_DEPTH, 0, &one);
    
    glUniformMatrix4fv(mViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glUniformMatrix4fv(mProjectionMatrixHandle,
                       1,
                       GL_FALSE,
                       glm::value_ptr(projMatrix));
 
   glUniform3fv(mLightPositionWorldspaceHandle, 1, mLightPosition);
   
    //auto seconds = elapsedSeconds * 0.01;
        
    for (const auto& handle : mHandles) {
      auto entityr = handleManager.get(handle);
      auto entity = static_cast<Entity*>(entityr);
      auto components = entity->mComponents;
      
      auto stateComponentHdl = entity->mComponents[john::ComponentTypes::C_STATE];
      auto stateComponent = static_cast<john::StateComponent*>(handleManager.get(stateComponentHdl));
      
      if (stateComponent->on) {        
        auto modelMatrixHandle = components[john::ComponentTypes::C_POSITION];
        
        john::PositionComponent* positionComponent = static_cast<john::PositionComponent*>(handleManager.get(modelMatrixHandle));
        auto rotate = positionComponent->rotationComponent.rotation;
        auto scale = positionComponent->scaleComponent.scale;
        auto translate = positionComponent->translationComponent.translation;
        auto model = positionComponent->modelComponent.model;
        
        auto modelMatrix = translate * rotate * scale * model;
        auto mvMatrix = viewMatrix * modelMatrix;
      
        glUniformMatrix4fv(mModelMatrixHandle, 1, GL_FALSE, glm::value_ptr(modelMatrix));
       
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
      }
    }
  }
  
}
