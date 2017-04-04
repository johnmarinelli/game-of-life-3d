#include "Camera.hpp"

namespace john {
  Camera::Camera() :
  mViewMatrix(glm::mat4{0.f}),
  mEye(glm::vec3{0.f}),
  mRoll(0.f),
  mYaw(0.f),
  mPitch(0.f),
  mIsMousePressed(false)
  {}
  
  Camera::Camera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) :
  mViewMatrix(glm::lookAt(eye, center, up)),
  mEye(eye),
  mCenter(center),
  mUp(up),
  mRoll(0.f),
  mYaw(0.f),
  mPitch(0.f),
  mIsMousePressed(false)
  {}
  
  void Camera::UpdateView() {
    glm::mat4 matRoll = glm::mat4{1.f};
    glm::mat4 matPitch = glm::mat4{1.f};
    glm::mat4 matYaw = glm::mat4{1.f};
    
    // roll => rotate around z axis
    // pitch => rotate around x axis
    // yaw => rotate around y axis    
    matRoll = glm::rotate(matRoll, mRoll, glm::vec3{0.f, 0.f, 1.f});
    matPitch = glm::rotate(matPitch, mPitch, glm::vec3{1.f, 0.f, 0.f});
    matYaw = glm::rotate(matYaw, mYaw, glm::vec3{0.f, 1.f, 0.f});
    
    glm::mat4 rotate = matRoll * matPitch * matYaw;
    glm::mat4 translate = glm::mat4{1.f};
    
    translate = glm::translate(translate, mEye);
    mViewMatrix = rotate * translate;
  }
  
  void Camera::KeyPressed(const unsigned char key) {
    float dx = 0, // strafe x
      dz = 0; // walk on z
    
    switch (key) {
      case 'w' : {
        dz = 2;
        break;
      }
      case 's' : {
        dz = -2;
        break;
      }
      case 'a' : {
        dx = -2;
        break;
      }
      case 'd' : {
        dx = 2;
        break;
      }
      default: break;
    }
    
    // row major
    glm::vec3 forward{mViewMatrix[0][2], mViewMatrix[1][2], mViewMatrix[2][2]};
    glm::vec3 strafe{mViewMatrix[0][0], mViewMatrix[1][0], mViewMatrix[2][0]};
    
    const float speed = 0.12f;
    // forward vector is negative to look forward
    mEye += (dz * forward + dx * strafe) * speed;
    
    this->UpdateView();
  }
  
  void Camera::MouseMove(int x, int y, int w, int h, bool leftDown) {
    if (leftDown) {
      // delta
      glm::vec2 mouseDelta = glm::vec2{x,y} - mMousePosition;
      const float mouseXSensitivity = 0.525f;
      const float mouseYSensitivity = 0.525f;
      
      mYaw += mouseXSensitivity * mouseDelta.x;
      mPitch += mouseYSensitivity * mouseDelta.y;
      
      mMousePosition = glm::vec2{x, y};   
      this->UpdateView();
    }
  }
  
  void Camera::MousePress(bool down, int x, int y) {
    cinder::app::console() << "MousePress " << down << "\n";    
    
    if (down) {
      mIsMousePressed = true;
      mMousePosition = glm::vec2{x, y};
    }
    else {
      mIsMousePressed = false;
    }
  }

}
