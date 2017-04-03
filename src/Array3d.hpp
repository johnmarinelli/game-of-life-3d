#ifndef Array3d_hpp
#define Array3d_hpp

#include <stdio.h>
#include <vector>

namespace john {

template <typename T, int sz>
class Array3d {
private:
  typedef T ElementsList[sz];
  
  /*
   * project 3d index to 1d
   */
  int projectedIndex(int x, int y, int z) const {
    return (y * mWidth + x) + (z * (mWidth * mHeight));
  }
  
public:
  
  ElementsList mElements;
  int mWidth, mHeight, mDepth;
  int mSize;
  
  Array3d(int w, int h, int d) :
    mWidth(w),
    mHeight(h),
    mDepth(d),
    mSize(0) {
    // zero-initialize array
    this->clear();
  }

  void insert(int x, int y, int z, const T& e) {
    auto idx = projectedIndex(x, y, z);
    
    if (idx >= 0 && idx < sz) {
      mElements[idx] = e;
      mSize++;
    }
    else {
      // TODO: error checking
    }
  }
  
  void insert(const glm::vec3& xyz, const T& e) {
    insert(xyz.x, xyz.y, xyz.z, e);
  }
  
  /*
   * if element exists at x,y,z then nullity it and return true
   * otherwise return false
   */
  bool removeAt(int x, int y, int z) {
    auto idx = projectedIndex(x, y, z);
    bool removed = false;
    
    if (idx > 0 && idx < sz) {
      auto ele = at(x, y, z);
      if (nullptr != ele) {
        mElements[idx] = nullptr;
      }
    }
    
    return removed;
  }
  
  void clear() {
    for (auto i = 0; i < mSize; ++i) {
      mElements[i] = nullptr;
    }
    
    mSize = 0;
  }
  
  T& at(int x, int y, int z) {
    auto idx = projectedIndex(x, y, z);
    
    // TODO
    // assert idx > -1 && idx < sz
    if (idx >= 0 && idx < sz) {
      return mElements[idx];
    }
  }

};
  
}

#endif
