#ifndef Components_hpp
#define Components_hpp

#include <stdio.h>

namespace john {
  
enum ComponentTypes {
  C_BASE,
  C_XYZ,
  C_VELOCITY,
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

}

#endif
