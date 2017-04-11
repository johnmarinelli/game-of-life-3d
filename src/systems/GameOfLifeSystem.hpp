#ifndef GameOfLifeSystem_hpp
#define GameOfLifeSystem_hpp

/*
 * Handles all Game of Life related logic -
 * checking neighborly existence and setting on/off state
 */

#include "Array3d.hpp"
#include "Constants.h"

namespace john {
  
  class Handle;
  class HandleManager;
  class Entity;
  
  /*
   * semantic representation of
   * 3d neighbors
   */
  enum {
    // First layer; "front" of cube
    X0Y0Z0, X1Y0Z0, X2Y0Z0,
    X0Y1Z0, X1Y1Z0, X2Y1Z0,
    X0Y2Z0, X1Y2Z0, X2Y2Z0,
    
    // Second layer; "middle" of cube
    // Origin is in this layer
    X0Y0Z1, X1Y0Z1, X2Y0Z1,
    X0Y1Z1, SELF,   X2Y1Z1,
    X0Y2Z1, X1Y2Z1, X2Y2Z1,
    
    // Third layer; "back" of cube
    X0Y0Z2, X1Y0Z2, X2Y0Z2,
    X0Y1Z2, X1Y1Z2, X2Y1Z2,
    X0Y2Z2, X1Y2Z2, X2Y2Z2
    
    
  } GOL_NEIGHBOR_POSITIONS;
  
  class GameOfLifeSystem {
    
  public:
    GameOfLifeSystem(john::HandleManager& hm) : mHandleManagerRef(hm) {}
    
    void getNeighbors(const john::Entity* entity, Array3d<john::Entity*, constants::NUM_NEIGHBORS>& neighbors);
    
    void registerEntity(const john::Handle& handle);
    void perform(const john::HandleManager& handleManager, double elapsedSeconds);
    
    john::HandleManager& mHandleManagerRef;
    john::Array3d<john::Entity*, constants::MAX_HANDLES> mEntities{constants::COLS, constants::ROWS, constants::DEPTH};
  };
  
}

#endif
