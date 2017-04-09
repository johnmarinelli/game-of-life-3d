#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>



namespace john {
  
  class Camera {
  public:
    glm::mat4 mViewMatrix;
    glm::mat4 mProjectionMatrix;

    glm::vec3 mEye;
    glm::vec3 mUp;
    glm::vec3 mCenter;
    glm::vec3 mOriginalEye;
    
    float mRoll;
    float mYaw;
    float mPitch;
    
    glm::vec2 mMousePosition{0.f, 0.f};
    
    Camera();
    Camera(const glm::mat4& projMatrix,
           const glm::vec3& eye,
           const glm::vec3& center,
           const glm::vec3& up);
    
    void UpdateView();
    void KeyPressed(const unsigned char key);
    void MouseMove(int x, int y, int w, int h, bool leftDown);
    void MousePress(bool down, int x, int y);
  };
  
}

#endif
