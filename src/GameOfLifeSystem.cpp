#include "GameOfLifeSystem.hpp"
#include "Handle.h"
#include "HandleManager.hpp"

namespace john {
  void GameOfLifeSystem::registerEntity(const john::Handle &handle)
  {
    mHandles.push_back(handle);
  }
  
  void GameOfLifeSystem::perform(const john::HandleManager &handleManager, double elapsedSeconds)
  {
    // do gol logic here
  }
}
