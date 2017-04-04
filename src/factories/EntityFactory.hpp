#ifndef EntityFactory_hpp
#define EntityFactory_hpp

#include <stdio.h>
#include <vector>

#include "Entity.hpp"
#include "HandleManager.hpp"

namespace john {
  
  class EntityFactory {
    public:
    
    EntityFactory(john::HandleManager& handleManagerRef) : mHandleManagerRef(&handleManagerRef) {}
    
    Handle addEntity()
    {
      auto entity = std::make_shared<john::Entity>();
      auto entityHdl = mHandleManagerRef->add(static_cast<void*>(entity.get()), john::ENTITY);
      entity->mHandle = entityHdl;
      
      mEntities.push_back(entity);
      return entityHdl;
    }
    
    bool deleteEntity(const john::Handle& handle)
    {
      // TODO
      return false;
    }
    
    Handle deleteEntity()
    {
      // TODO
    }
    
    std::vector<std::shared_ptr<john::Entity>> mEntities;
    john::HandleManager* mHandleManagerRef;
  };
  
}

#endif
