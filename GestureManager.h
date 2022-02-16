#ifndef _GESTUREMANAGER_H_
#define _GESTUREMANAGER_H_

#include <Arduino.h>
#include <bluefruit.h>
#include <math.h>
#include <Adafruit_MPU6050.h>

#define SENSITIVITY_MAX 100

typedef enum {
    THUMB,
    INDEX,
    MIDDLE,
    RING,
    PINKY
} Finger;

class GestureManager 
{
    private:
        unsigned long lastUpdateTime;
        int sensitivity;
        int b1, b2, b3, scroll;
        boolean prevB1 = false;  
        boolean prevB2  = false;
        boolean prevB3 = false;
        boolean prevScroll = false;

        double lastXAccel = 0.0;
        double lastYAccel = 0.0;
        double lastZAccel = 0.0;
        double lastXRot = 0.0;
        double lastYRot = 0.0;
        double lastZRot = 0.0;
        BLEHidAdafruit hid;
        Adafruit_MPU6050 mpu;
        
    public:
        GestureManager(BLEHidAdafruit BLEhid, Adafruit_MPU6050 mpu6050, int button1Pin, int button2Pin, int button3Pin, int enableScrollPin);
        void update(void);
        void setSensitivity(int sensitivity);
        int getSensitivity(void);
   
};

#endif
