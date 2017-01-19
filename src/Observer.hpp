#ifndef Observer_hpp
#define Observer_hpp

#include <stdio.h>

namespace john {
  
  template <typename T>
  class Observer {
  public:
    virtual void update(const T& newT) = 0;
  };
  
   /* example:
   template <typename T>
   class DivObserver : public Observer<T> {
   public:
     virtual void update(const T& newT) {
      std::cout << "DivObserver::update: " << newT << '\n';
    }
   
   };
   */
}

#endif
