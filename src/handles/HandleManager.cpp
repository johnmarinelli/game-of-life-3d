#include "HandleManager.hpp"

namespace john {

HandleManager::HandleEntry::HandleEntry() :
  mNextFreeIndex(0),
  mCounter(1),
  mActive(0),
  mEndOfList(0),
  mEntry(nullptr)
{
}
  
HandleManager::HandleEntry::HandleEntry(uint32 nextFreeIndex) :
  mNextFreeIndex(nextFreeIndex),
  mCounter(1),
  mActive(0),
  mEndOfList(0),
  mEntry(nullptr)
{
}
  
HandleManager::HandleManager()
{
  reset();
}
  

void HandleManager::reset()
{
  mActiveEntryCount = 0;
  mFirstFreeEntry = 0;
  
  for (int i = 0; i < MaxEntries - 1; ++i) {
    mEntries[i] = HandleEntry(i+1);
  }
  
  mEntries[MaxEntries - 1] = HandleEntry{};
  mEntries[MaxEntries - 1].mEndOfList = true;
}
  
Handle HandleManager::add(void *ptr, uint32 type)
{
  if (mActiveEntryCount >= MaxEntries - 1) {
    std::cout << "HandleManager::add: could not add handle.  Increase size of MaxEntries.\n";
    return Handle{};
  }
  
  if (type > 31) {
    std::cout << "HandleManager::add: could not add handle.  Check type.  Received: " << type << '\n';
    return Handle{};
  }
  
  // get first index from member variable
  const uint32 newIndex = mFirstFreeEntry;
  /*
   Is this stuff necessary?
   POW2_ASSERT(newIndex < MaxEntries);
   POW2_ASSERT(m_entries[newIndex].m_active == false);
   POW2_ASSERT(!m_entries[newIndex].m_endOfList);
   */
  
  // update member variable
  mFirstFreeEntry = mEntries[newIndex].mNextFreeIndex;
  
  // Setting first 12 bits
  mEntries[newIndex].mNextFreeIndex = 0;
  
  // Setting next 15 bits
  mEntries[newIndex].mCounter = mEntries[newIndex].mCounter + 1;
  
  // Invalidate flag
  if (mEntries[newIndex].mCounter == 0) {
    mEntries[newIndex].mCounter = 1;
  }
  
  // Setting next bit
  mEntries[newIndex].mActive = true;
  
  // Setting data
  mEntries[newIndex].mEntry = ptr;
  
  // update member variable
  ++mActiveEntryCount;
  
  return Handle{newIndex, mEntries[newIndex].mCounter, type};
  
}
  
// TODO: assert sizeof(ptr) == sizeof(mEntries[i].mEntry)
void HandleManager::update(john::Handle hdl, void *ptr)
{
  const int i = hdl.mIndex;
  
  // If handle is in sync with my storage and my storage says handle is active,
  if (mEntries[i].mCounter == hdl.mCounter && mEntries[i].mActive) {
    // set data to ptr
    mEntries[i].mEntry = ptr;
  }
}
  
void HandleManager::remove(john::Handle hdl)
{
  const uint32 i = hdl.mIndex;
  
  // If handle is in sync with my storage and my storage says handle is active,
  if (mEntries[i].mCounter == hdl.mCounter && mEntries[i].mActive) {
    // Set next free index to class variable.
    mEntries[i].mNextFreeIndex = mFirstFreeEntry;
    
    // Set flag.
    mEntries[i].mActive = false;
    
    // Update class variable.
    mFirstFreeEntry = i;
    
    --mActiveEntryCount;
  }
}
  
void* HandleManager::get(john::Handle hdl) const
{
  void* p = nullptr;
  
  if (!get(hdl, p)) { return nullptr; }
  return p;
}
  
bool HandleManager::get(john::Handle hdl, void*& out) const
{
  const int i = hdl.mIndex;
  if (mEntries[i].mCounter != hdl.mCounter || !mEntries[i].mActive) {
    return false;
  }
  
  out = mEntries[i].mEntry;
  return true;
}
  
int HandleManager::getCount() const
{
   return mActiveEntryCount;
}

}
