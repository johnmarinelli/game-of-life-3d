#ifndef MovementSystem_h
#define MovementSystem_h

#include <vector>

namespace john {
  
  class Handle;
  class HandleManager;
  
  class MovementSystem {
  public:
    MovementSystem();
    void registerEntity(const john::Handle& handle);
    void update(const john::HandleManager& handleManager);
    
  private:
    std::vector<john::Handle> mHandles;
  };

}

#endif
