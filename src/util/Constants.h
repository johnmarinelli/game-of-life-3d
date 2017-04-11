//
//  Constants.h
//  GameOfLife
//
//  Created by John Marinelli on 3/27/17.
//
//

#ifndef Constants_h
#define Constants_h

namespace john {
  namespace constants {
    
    const int ROWS = 10;
    const int COLS = 10;
    const int DEPTH = 10;
    const int NUM_NEIGHBORS = 27;
    const float CAMERA_SPEED = 0.12;
    const float MOUSE_X_SENSITIVITY = 0.0005f;
    const float MOUSE_Y_SENSITIVITY = 0.0005f;
    const float FPS = 20.f;
    enum { MAX_HANDLES = 4096 };
    
  }
}

#endif
