//
//  Handle.h
//  GameOfLife
//
//  Created by John Marinelli on 1/16/17.
//
//  See http://gamesfromwithin.com/managing-data-relationships
//  For thorough explanation.


#ifndef Handle_h
#define Handle_h

typedef unsigned int uint32;

namespace john
{
  
  struct Handle
  {
   Handle() : mIndex(0), mCounter(0), mType(0)
   {}
   
   Handle(uint32 index, uint32 counter, uint32 type)
   : mIndex(index), mCounter(counter), mType(type)
   {}
   
   inline operator uint32() const;
   
   /*
    * Bitfield declarations:
    * Index: the index into the handle manager.  Determines # of handles.
    * 14 bits = >16k handles
    * Counter: number that goes up every time the index slot is reused
    * (to check handle invalidation)
    * Type: What kind of data is being pointed to.
    */
   uint32 mIndex : 12;
   uint32 mCounter : 15;
   uint32 mType : 5;
  };
  
  
  Handle::operator uint32() const
  {
    return mType << 27 | mCounter << 12 | mIndex;
  }

  
}
#endif 
