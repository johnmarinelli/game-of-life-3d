#include "DrawingSystem.hpp"
#include "HandleManager.hpp"
#include "Entity.hpp"

namespace john {
  
  DrawingSystem::DrawingSystem()
  {
  }
  
  void DrawingSystem::registerEntity(const john::Handle& handle)
  {
    mHandles.push_back(handle);
  }
  
  void DrawingSystem::update(const john::HandleManager& handleManager)
  {
    for (const auto& handle : mHandles) {
      auto entityr = handleManager.get(handle);
      auto entity = static_cast<Entity*>(entityr);
      auto components = entity->mComponents;
      
      auto modelMatHdl = components[john::ComponentTypes::C_MODELMAT];
      
      john::ModelMatrixComponent* modelmat = static_cast<john::ModelMatrixComponent*>(handleManager.get(modelMatHdl));
    }
  }
  
}
