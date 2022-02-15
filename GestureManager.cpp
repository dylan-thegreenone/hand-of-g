#include <Arduino.h>
#include "GestureManager.h"

GestureManager::GestureManager(BLEHidAdafruit BLEhid)
{
    this -> hid = BLEhid;
}

void GestureManager::update(
    boolean thumbState, boolean indexState, boolean middleState, boolean ringState, boolean pinkyState
    )
    {
      
    }
