#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include "Handle.h"

namespace john {
  // uint32 tag for a Handle's type field.
  const uint32 ENTITY = 31;
  struct Entity {
    Handle mHandle;
    std::vector<Handle> mComponents;
  };
}

#endif
