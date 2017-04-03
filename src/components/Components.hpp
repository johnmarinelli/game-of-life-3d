#ifndef Components_hpp
#define Components_hpp

#include <stdio.h>
#include <cstdint>

namespace john {
  
enum ComponentTypes {
  C_BASE,
  C_POSITION,
  C_GRIDPOSITION,
  C_STATE
};

struct Component {
  Handle handle;
};
  
struct ModelMatrixComponent : public Component {
  glm::mat4 model;
};
  
struct RotationMatrixComponent : public Component {
  glm::mat4 rotation;
};

struct TranslationMatrixComponent : public Component {
  glm::mat4 translation;
};

struct ScaleMatrixComponent : public Component {
  glm::mat4 scale;
};

struct PositionComponent : public Component {
  RotationMatrixComponent rotationComponent;
  TranslationMatrixComponent translationComponent;
  ScaleMatrixComponent scaleComponent;
  ModelMatrixComponent modelComponent;
};
  
struct GridPositionComponent : public Component {
  glm::vec3 xyz;
};
  
struct StateComponent : public Component {
  bool on;
  StateComponent() : on(false) {}
};
  
}

#endif
