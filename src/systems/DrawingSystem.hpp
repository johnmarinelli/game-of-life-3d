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
    
    void initialize(john::Camera& camera);
    void registerEntity(const john::Handle& handle);
    void perform(const john::HandleManager& handleManager, double elapsedSeconds);
                    
    GLuint mProjectionMatrixHandle;
    GLuint mViewMatrixHandle;
    GLuint mModelMatrixHandle;
    GLuint mLightPositionWorldspaceHandle;
    
    GLfloat mLightPosition[3];
    
    john::Camera* mCameraRef;
    
  private:
    std::vector<john::Handle> mHandles;
  };
  
}
#endif
