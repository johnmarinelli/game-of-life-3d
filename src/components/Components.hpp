#ifndef Components_hpp
#define Components_hpp

#include <stdio.h>
#include <cstdint>

namespace john {
  
enum ComponentTypes {
  C_BASE,
  C_MODELMAT
};

struct Component {
  Handle handle;
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

struct ModelMatrixComponent : public Component {
  RotationMatrixComponent rotationComponent;
  TranslationMatrixComponent translationComponent;
  ScaleMatrixComponent scaleComponent;
};
  
}

#endif
