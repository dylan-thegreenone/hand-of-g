// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Gesture manager class for motion based mouse glove
// Uses Adafruit MPU6050 sensor and nrf52 Bluefruit HID 
//
// @author Dylan Greenberg
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef _GESTUREMANAGER_H_
#define _GESTUREMANAGER_H_

#include <Arduino.h>
#include <bluefruit.h>
#include <math.h>
#include <Adafruit_MPU6050.h>
#include "ButtonManager.h"

#define SENSITIVITY_MAX 100

// fingers available (if more, I'm impressed)
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
        const double GYRO_THRESHOLD = 0.0; 
        const double ACCEL_THRESHOLD = 0.0;

        boolean started = false;

        unsigned long lastUpdateTime;
        int sensitivity;
        Button* b1;
        Button* b2;
        Button* b3;
        Button* b4;

        double lastXAccel = 0.0;
        double lastYAccel = 0.0;
        double lastZAccel = 0.0;
        double lastXRot = 0.0;
        double lastYRot = 0.0;
        double lastZRot = 0.0;

        double zeroedXRot = 0.0;
        double zeroedYRot = 0.0;
        double zeroedZRot = 0.0;

        BLEHidAdafruit* hid;
        Adafruit_MPU6050* mpu;
        
    public:
        static const int DEADZONE_VAL = 2;
    
        /*
        * constructor for gesture manager translating motion and finger movement to mouse input
        * @param BLEhid pointer to bluetooth HID service to send output to
        * @param mpu6050 pointer to adafruit combined accelerometer gyroscope to get movement data from
        * @param pin of button to be used as Mouse Button 1
        * @param pin of button to be used as Mouse Button 2
        * @param pin of button to be used as Mouse Button 3
        * @param pin of button to be used as the movement enable signal
        */
        GestureManager(BLEHidAdafruit* BLEhid, Adafruit_MPU6050* mpu6050, int button1Pin, int button2Pin, int button3Pin, int enablePin);

        /*
        * gesture manager deconstructor, releases button managers bound to the button pins
        */
        ~GestureManager();

        /*
        * zero gyroscope at current state (not fully working)
        * @return
        */
        void zeroGyro(void);

        /*
        * start the gesture management
        * must be called before input is given
        * @return
        */
        void startGestures(void);

        /*
        * refresh function, called in main loop for accurate state updates
        * @return
        */
        void refresh(void);

        /*
        * set sensitivity of mouse movement for translation
        * @param senstivity value to use for speed
        * @return
        */
        void setSensitivity(int sensitivity);

        /*
        * get current sensitivity of gestures
        * @return current sensitivity
        */
        int getSensitivity(void);
   
};

#endif
