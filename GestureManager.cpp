#include "GestureManager.h"

GestureManager::GestureManager(BLEHidAdafruit BLEhid, Adafruit_MPU6050 mpu6050, int button1Pin, int button2Pin, int button3Pin, int enableScrollPin)
{
    this -> hid = BLEhid;
    this -> mpu = mpu6050;
    this -> sensitivity = SENSITIVITY_MAX/2;
    this -> b1 = button1Pin;
    this -> b2 = button2Pin;
    this -> b3 = button3Pin;
    this -> scroll = enableScrollPin;

    pinMode(this->b1, INPUT);
    pinMode(this->b2, INPUT);
    pinMode(this->b3, INPUT);
    pinMode(this->scroll, INPUT);
}


void GestureManager::update(void)
{
    
    boolean b1State = digitalRead(this->b1);
    boolean b2State = digitalRead(this->b2);
    boolean b3State = digitalRead(this->b3);
    boolean scrollState = digitalRead(this->scroll);
    uint8_t buttonReport = b1State * MOUSE_BUTTON_LEFT + b2State + MOUSE_BUTTON_RIGHT + b3State * MOUSE_BUTTON_MIDDLE;

    int xChange = 0;
    int yChange = 0;

    this -> hid.mouseReport(buttonReport, xChange, yChange);
    this -> prevB1 = b1State;
    this -> prevB2 = b2State;
    this -> prevB3 = b3State;
    this -> prevScroll = scrollState;
    this -> lastUpdateTime = millis();
}

void GestureManager::setSensitivity(int sensitivity)
{
    this -> sensitivity = constrain(sensitivity, 0, SENSITIVITY_MAX);
}
int GestureManager::getSensitivity(void)
{
    return this->sensitivity;
}
