#ifndef Components_hpp
#define Components_hpp

#include <stdio.h>

namespace john {
  
enum ComponentTypes {
  C_BASE,
  C_XYZ,
  C_VELOCITY,
  C_MODELMAT,
  C_NORMALS,
  C_VERTICES,
  C_INDICES,
  C_COLORS
};

struct Component {
  Handle handle;
};

struct XyzComponent : public Component {
  glm::vec3 xyz;
};
  
struct VelocityComponent : public Component {
  glm::vec3 velocity;
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
  
  glm::mat4 model;
};

template <int n>
struct NormalsComponent : public Component {
  GLfloat normals[n];
};

template <int n>
struct VerticesComponent : public Component {
  GLfloat vertices[n];
};
  
template <int n>
struct IndicesComponent : public Component {
  GLushort indices[n];
};
  
template <int n>
struct ColorsComponent : public Component {
  GLfloat colors[n];
};
  
struct GlVertexComponent : public Component {

};

}

#endif
