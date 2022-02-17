#include "GestureManager.h"

GestureManager::GestureManager(BLEHidAdafruit BLEhid, Adafruit_MPU6050 mpu6050, int button1Pin, int button2Pin, int button3Pin, int enableScrollPin)
{
    this->hid = BLEhid;
    this->mpu = mpu6050;
    this->sensitivity = SENSITIVITY_MAX/2;
    this->b1 = new Button(button1Pin);
    this->b2 = new Button(button2Pin);
    this->b3 = new Button(button3Pin);
    this->bScroll = new Button(enableScrollPin);
//    this->b1->setOnPress([]() { Serial.println("mouse1");});
//    this->b2->setOnPress([]() { Serial.println("mouse2");});
//    this->b3->setOnPress([]() { Serial.println("mouse3");});
//    this->bScroll->setOnPress([]() { Serial.println("mouseS");});

}
GestureManager::~GestureManager()
{
    delete this->b1;
    delete this->b2;
    delete this->b3;
    delete this->bScroll;
}

void GestureManager::refresh(void)
{
    this->b1->check();
    this->b2->check();
    this->b3->check();
    this->bScroll->check();
    boolean b1State = this->b1->pressed();
    boolean b2State = this->b2->pressed();
    boolean b3State = this->b3->pressed();
    boolean scrollState = this->bScroll->pressed();
    uint8_t buttonReport = b1State * MOUSE_BUTTON_LEFT + b2State + MOUSE_BUTTON_RIGHT + b3State * MOUSE_BUTTON_MIDDLE;

    int xChange = 0;
    int yChange = 0;

    this->hid.mouseReport(buttonReport, xChange, yChange);
    this->lastUpdateTime = millis();
}

void GestureManager::setSensitivity(int sensitivity)
{
    this->sensitivity = constrain(sensitivity, 0, SENSITIVITY_MAX);
}
int GestureManager::getSensitivity(void)
{
    return this->sensitivity;
}
