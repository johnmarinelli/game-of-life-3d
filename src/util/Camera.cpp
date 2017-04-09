#include "Camera.hpp"
#include "Constants.h"

namespace john {
  
  glm::mat4 createViewMatrix(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
    // forward vector; direction you are looking at
    auto f = glm::normalize(center - eye);
    
    // right vector
    auto s = glm::normalize(glm::cross(f, up));
    
    // up vector
    auto u = glm::normalize(glm::cross(s, f));
    
    glm::mat4 result{1};
    
    // row major so we can multiply it later
    // [row][column]
    // first row - right vector
    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;
    
    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;
    
    result[0][2] = -f.x;
    result[1][2] = -f.y;
    result[2][2] = -f.z;
    
    result[3][0] = glm::dot(s, eye);
    result[3][1] = glm::dot(u, eye);
    result[3][2] = glm::dot(f, eye);
    
    return result;
  }
  
  Camera::Camera() :
  mEye(glm::vec3{0, 0, -100}),
  mOriginalEye(mEye),
  mRoll(0.f),
  mYaw(0.f),
  mPitch(0.f)
  {
    this->UpdateView();
  }
  
  Camera::Camera(const glm::mat4& projMatrix,
                 const glm::vec3& eye,
                 const glm::vec3& center,
                 const glm::vec3& up) :
  mProjectionMatrix(projMatrix),
  mEye(eye),
  mOriginalEye(mEye),
  mCenter(center),
  mUp(up),
  mRoll(0.f),
  mYaw(0.f),
  mPitch(0.f)
  {   
    this->UpdateView();
  }
  
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
      case 'r' : {
        mEye = mOriginalEye;
      }
      default: break;
    }
    
    // row major
    glm::vec3 forward{mViewMatrix[0][2], mViewMatrix[1][2], mViewMatrix[2][2]};
    glm::vec3 strafe{mViewMatrix[0][0], mViewMatrix[1][0], mViewMatrix[2][0]};
    
    mEye += (dz * forward + dx * strafe) * john::constants::CAMERA_SPEED;
    
    this->UpdateView();
  }
  
  void Camera::MouseMove(int x, int y, int w, int h, bool leftDown) {
    if (leftDown) {
      // delta
      glm::vec2 mouseDelta = glm::vec2{x,y} - mMousePosition;
      
      mYaw += john::constants::MOUSE_X_SENSITIVITY * mouseDelta.x;
      mPitch += john::constants::MOUSE_Y_SENSITIVITY * mouseDelta.y;
      
      mMousePosition = glm::vec2{x, y};   
      this->UpdateView();
    }
  }
  
}
