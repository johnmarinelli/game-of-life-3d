//
//  HandleManager.hpp
//  GameOfLife
//
//  Created by John Marinelli on 1/16/17.
//
//  See http://gamesfromwithin.com/managing-data-relationships
//  For thorough explanation.

#ifndef HandleManager_hpp
#define HandleManager_hpp

#include <stdio.h>
#include "Handle.h"

namespace john {
  
/* example:
 std::shared_ptr<john::XyzComponent> xyzc = std::make_shared<john::XyzComponent>();
 xyzc->xyz = glm::vec3{1.f};
 auto hdl = mHandleManager.add(static_cast<void*>(xyzc.get()), john::ComponentTypes::C_XYZ);
 
 auto cmp = static_cast<john::XyzComponent*>(mHandleManager.get(hdl));
 xyzc->xyz = glm::vec3{2.f};
 
 std::shared_ptr<john::XyzComponent> xyzc2 = std::make_shared<john::XyzComponent>();
 xyzc2->xyz = glm::vec3{-2.f};
 mHandleManager.update(hdl, static_cast<void*>(xyzc2.get()));
 
 mHandleManager.remove(hdl);
 */
  
class HandleManager {
public:
  
  enum { MaxEntries = 4096 }; // 2 ^ 12
  
  HandleManager();
  
  void reset();
  Handle add(void* ptr, uint32 type);
  void update(Handle hdl, void* ptr);
  void remove(Handle hdl);
  
  void* get(Handle hdl) const;
  bool get(Handle hdl, void*& out) const;
  

  template<typename T> bool getAs(Handle hdl, T& out) const;
  
  int getCount() const;
  
private:
  HandleManager(const HandleManager&);
  HandleManager& operator=(const HandleManager&);
  
  struct HandleEntry {
    HandleEntry();
    explicit HandleEntry(uint32 nextFreeIndex);
    
    uint32 mNextFreeIndex : 12;
    uint32 mCounter : 15;
    uint32 mActive : 1;
    uint32 mEndOfList : 1;
    void* mEntry;
  };
  
  HandleEntry mEntries[MaxEntries];
  
  int mActiveEntryCount;
  uint32 mFirstFreeEntry;
};

template<typename T>
  inline bool HandleManager::getAs(Handle hdl, T& out) const
{
  void* outAsVoid;
  const bool rv = get(hdl, outAsVoid);
  out = static_cast<T>(outAsVoid);
  return rv;
}

}

#endif
