#ifndef GameOfLifeSystem_hpp
#define GameOfLifeSystem_hpp

/*
 * Handles all Game of Life related logic - 
 * checking neighborly existence and setting on/off state
 */

namespace john {

  class Handle;
  class HandleManager;
  
class GameOfLifeSystem {
  
  void registerEntity(const john::Handle& handle);
  void perform(const john::HandleManager& handleManager, double elapsedSeconds);
  
  // spatial partition
  //Octree mEntities;
  
  
//private:
  std::vector<Handle> mHandles;
};
  
}

#endif
