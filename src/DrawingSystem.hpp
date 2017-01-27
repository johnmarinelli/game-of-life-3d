#ifndef DrawingSystem_hpp
#define DrawingSystem_hpp

#include <stdio.h>
#include <vector>

namespace john {
  
  class Handle;
  class HandleManager;
  
  class DrawingSystem {
  public:
    DrawingSystem();
    void registerEntity(const john::Handle& handle);
    void update(const john::HandleManager& handleManager);
    
    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;
    GLuint mProjectionMatrixHandle;
    GLuint mViewMatrixHandle;
    
  private:
    std::vector<john::Handle> mHandles;
  };
  
}
#endif
