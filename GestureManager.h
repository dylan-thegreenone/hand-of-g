#ifndef _GESTUREMANAGER_H_
#define _GESTUREMANAGER_H_

#include <BLEHidAdafruit.h>

class GestureManager 
{
    private:
        unsigned long lastUpdateTime;
        boolean lastThumbState = false;  //No thumb switch yet, but hopefully will be added at some point
        boolean lastIndexState = false;
        boolean lastMiddleState = false;
        boolean lastRingState = false;
        boolean lastPinkyState = false;
        BLEHidAdafruit hidOutput;
    public:
        GestureManager(BLEHidAdafruit output);
        void update(
          boolean thumbState, boolean indexState, boolean middleState, boolean ringState, boolean pinkyState,
          
}

#endif
