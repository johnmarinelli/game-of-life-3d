#ifndef DrawingSystem_hpp
#define DrawingSystem_hpp

#include <stdio.h>
#include <vector>

namespace john {
  
  class Handle;
  class HandleManager;
  class Camera;
  
  class DrawingSystem {
  public:
    DrawingSystem();
    
    void initialize(const glm::mat4& projMatrix, john::Camera& camera);
    void registerEntity(const john::Handle& handle);
    void perform(const john::HandleManager& handleManager, double elapsedSeconds);
    
    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;
                    
    GLuint mProjectionMatrixHandle;
    GLuint mViewMatrixHandle;
    GLuint mModelMatrixHandle;
    GLuint mModelViewMatrixHandle;
    
    john::Camera* mCameraRef;
    
  private:
    std::vector<john::Handle> mHandles;
  };
  
}
#endif
