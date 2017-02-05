#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "DrawingSystem.hpp"
#include "HandleManager.hpp"
#include "Entity.hpp"
namespace john {
  
  DrawingSystem::DrawingSystem()
  {
  }
  
  void DrawingSystem::initialize(const glm::mat4 &projMatrix, const glm::mat4 &viewMatrix)
  {
    mProjectionMatrix = projMatrix;
    mViewMatrix = viewMatrix;
  }
  
  void DrawingSystem::registerEntity(const john::Handle& handle)
  {
    mHandles.push_back(handle);
  }
  
  void DrawingSystem::perform(const john::HandleManager& handleManager, double elapsedSeconds)
  {    
    static const GLfloat one = 1.f;
    static const GLfloat bg[] = {0.f, 0.0f, 0.f, 1.f};
    
    glClearBufferfv(GL_COLOR, 0, bg);
    glClearBufferfv(GL_DEPTH, 0, &one);
    
    glUniformMatrix4fv(mViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(mViewMatrix));

    glUniformMatrix4fv(mProjectionMatrixHandle,
                       1,
                       GL_FALSE,
                       glm::value_ptr(mProjectionMatrix));
    
    auto seconds = elapsedSeconds * 0.01;
    float f = seconds * 0.03;
        
    for (const auto& handle : mHandles) {
      auto entityr = handleManager.get(handle);
      auto entity = static_cast<Entity*>(entityr);
      auto components = entity->mComponents;
      
      auto modelMatrixHandle = components[john::ComponentTypes::C_MODELMAT];      
      
      john::ModelMatrixComponent* modelMatrixComponent = static_cast<john::ModelMatrixComponent*>(handleManager.get(modelMatrixHandle));
      auto rotate = modelMatrixComponent->rotationComponent.rotation;
      auto scale = modelMatrixComponent->scaleComponent.scale;
      /*auto translate = glm::translate(glm::mat4{},
                                      glm::vec3{
                                        sinf(2.1f * f) * 0.5f,
                                        cosf(1.7f * f) * 0.5f,
                                        sinf(1.3f * f) * cosf(1.5f * f) * 2.f
                                      });*/
      auto translate = modelMatrixComponent->translationComponent.translation;
      
      auto modelMatrix = translate * rotate * scale * glm::mat4{};
      auto mvMatrix = mViewMatrix * modelMatrix;
      
      glUniformMatrix4fv(mModelMatrixHandle, 1, GL_FALSE, glm::value_ptr(modelMatrix));
      glUniformMatrix4fv(mModelViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(mvMatrix));
      
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    }
  }
  
}
