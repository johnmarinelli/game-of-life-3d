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
  ModelMatrixComponent() : model(glm::mat4{1.f}) {}
  ModelMatrixComponent(const glm::mat4& m) : model(m) {}
};
  
struct RotationMatrixComponent : public Component {
  glm::mat4 rotation;
  RotationMatrixComponent() : rotation(glm::mat4{0.f}) {}
  RotationMatrixComponent(const glm::mat4& m) : rotation(m) {}
};

struct TranslationMatrixComponent : public Component {
  glm::mat4 translation;
  TranslationMatrixComponent() : translation(glm::mat4{0.f}) {}
  TranslationMatrixComponent(const glm::mat4& m) : translation(m) {}
};

struct ScaleMatrixComponent : public Component {
  glm::mat4 scale;
  ScaleMatrixComponent() : scale(glm::mat4{1.f}) {}
  ScaleMatrixComponent(const glm::mat4& m) : scale(m) {}
};

struct PositionComponent : public Component {
  RotationMatrixComponent rotationComponent;
  TranslationMatrixComponent translationComponent;
  ScaleMatrixComponent scaleComponent;
  ModelMatrixComponent modelComponent;
  
  PositionComponent() {}
  
  PositionComponent(const glm::mat4& rot,
                    const glm::mat4& trans,
                    const glm::mat4& scale,
                    const glm::mat4& model) :
  rotationComponent(rot),
  translationComponent(trans),
  scaleComponent(scale),
  modelComponent(model)
  {}
};
  
struct GridPositionComponent : public Component {
  glm::vec3 xyz;
  GridPositionComponent() : xyz(glm::vec3{0.f}) {}
  GridPositionComponent(const glm::vec3& _xyz) : xyz(_xyz) {}
};
  
struct StateComponent : public Component {
  bool on;
  StateComponent() : on(false) {}
  StateComponent(bool _on) : on(_on) {}
};
  
}

#endif
