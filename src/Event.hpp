#ifndef Event_hpp
#define Event_hpp

#include <stdio.h>
#include <vector>

#include "Observer.hpp"

namespace john {
  
  class Event {
  public:
    Event();
    
    template <typename T>
    void attach(Observer<T>* obs);
    
    void notify();
    
  private:
    //std::vector<Observer<T>*> mObservers;
  };
}

#endif
