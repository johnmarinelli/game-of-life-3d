#include "MovementSystem.hpp"
#include "HandleManager.hpp"
#include "Entity.hpp"

namespace john {
  
  MovementSystem::MovementSystem()
  {
  }
  
  void MovementSystem::registerEntity(const john::Handle& handle)
  {
    mHandles.push_back(handle);
  }
  
  void MovementSystem::update(const john::HandleManager& handleManager)
  {
    for (const auto& handle : mHandles) {
      auto entityr = handleManager.get(handle);
      auto entity = static_cast<Entity*>(entityr);
      auto components = entity->mComponents;
      
      auto xyzHdl = components[john::ComponentTypes::C_XYZ];
      auto velHdl = components[john::ComponentTypes::C_VELOCITY];
      
      john::XyzComponent* xyzc = static_cast<john::XyzComponent*>(handleManager.get(xyzHdl));
      john::VelocityComponent* velc = static_cast<john::VelocityComponent*>(handleManager.get(velHdl));
      
      xyzc->xyz += velc->velocity;
    }
  }

}
