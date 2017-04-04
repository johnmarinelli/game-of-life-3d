#include "GameOfLifeSystem.hpp"
#include "Handle.h"
#include "HandleManager.hpp"
#include "Entity.hpp"

#include <algorithm>

namespace john {

  void GameOfLifeSystem::getNeighbors(const john::Entity* entity, Array3d<john::Entity*, constants::NUM_NEIGHBORS>& neighbors) {
    auto subjComponents = entity->mComponents;
    auto subjGridPosHdl = subjComponents[john::ComponentTypes::C_GRIDPOSITION];
    auto subjGridPos = static_cast<john::GridPositionComponent*>(mHandleManagerRef.get(subjGridPosHdl));
    
    auto entityXyz = subjGridPos->xyz;
    
    int startX = entityXyz.x - 1,
    startY = entityXyz.y - 1,
    startZ = entityXyz.z - 1,
    endX   = entityXyz.x + 1,
    endY   = entityXyz.y + 1,
    endZ   = entityXyz.z + 1;
    
    auto neighborX = 0,
    neighborY = 0,
    neighborZ = 0;
    
    for (auto k = startZ; k <= endZ; k++) {
      
      for (auto j = startY; j <= endY; j++) {
        
        for (auto i = startX; i <= endX; i++) {
          john::Entity* neighbor = nullptr;
          
          if (i >= 0 && j >= 0 && k >= 0) {
            neighbor = mEntities.at(i,j,k);
          }
          neighbors.insert(neighborX, neighborY, neighborZ, neighbor);
          
          neighborX++;
        }
        neighborX = 0;
        neighborY++;
      }
      neighborY = 0;
      neighborZ++;
    }
    neighborZ = 0;
  }
  
  void GameOfLifeSystem::registerEntity(const john::Handle &handle)
  {
    auto entity = static_cast<john::Entity*>(mHandleManagerRef.get(handle));
    auto gridPos = static_cast<john::GridPositionComponent*>(mHandleManagerRef.get(entity->mComponents[john::ComponentTypes::C_GRIDPOSITION]));
    mEntities.insert(gridPos->xyz, entity);
  }
  
  void GameOfLifeSystem::perform(const john::HandleManager &handleManager, double elapsedSeconds)
  {
   Array3d<john::Entity*, constants::NUM_NEIGHBORS> neighborCube{3,3,3};
   std::vector<john::StateComponent*> aliveToDead;
   std::vector<john::StateComponent*> deadToAlive;
   
   for (auto k = 0; k < mEntities.mDepth; ++k) {
     for (auto i = 0; i < mEntities.mHeight; ++i) {
       for (auto j = 0; j < mEntities.mWidth; ++j) {
         neighborCube.clear();
         auto entity = mEntities.at(j, i, k);
         getNeighbors(entity, neighborCube);
         auto eles = neighborCube.mElements;
         
         int neighborCount = 0;
         
         // count neighbors
         for (auto nitr = 0; nitr < neighborCube.mSize; ++nitr) {
           auto neighbor = eles[nitr];
           
           if (nullptr != neighbor && neighbor != entity) {
             auto stateHdl = neighbor->mComponents[john::ComponentTypes::C_STATE];
             auto state = static_cast<john::StateComponent*>(mHandleManagerRef.get(stateHdl));
             if (state->on) {
               neighborCount++;
             }
           }
         }
         
         auto entityStateHdl = entity->mComponents[john::ComponentTypes::C_STATE];
         auto entityState = static_cast<john::StateComponent*>(mHandleManagerRef.get(entityStateHdl));
         
         if (entityState->on) {
           if (neighborCount < 2) aliveToDead.push_back(entityState);
           if (neighborCount > 3) aliveToDead.push_back(entityState);
         }
         else {
           if (neighborCount == 3) {
             //entityState->on = true;
             deadToAlive.push_back(entityState);
           }
         }
       }
     }
   }
   
   std::for_each(deadToAlive.begin(), deadToAlive.end(), [] (john::StateComponent* state) {
     state->on = true;
   });
   
   std::for_each(aliveToDead.begin(), aliveToDead.end(), [] (john::StateComponent* state) {
     state->on = false;
   });
     
  }

}
