#ifndef ComponentFactory_hpp
#define ComponentFactory_hpp

#include "HandleManager.hpp"
#include "components/Components.hpp"
#include <vector>
#include <stdio.h>

namespace john {

template <typename T>
class ComponentFactory {
public:
  ComponentFactory(john::HandleManager& hm) :
    mHandleManagerRef(&hm)
  {
    
  }
  
  std::vector<std::shared_ptr<T>> mComponents;
  
  template<typename... Args>
  std::shared_ptr<T> create(Args... args) {
    mComponents.push_back(std::make_shared<T>(args...));
    return mComponents.back();
  }
  
  john::HandleManager* mHandleManagerRef;
};
}

#endif
