#include "GestureManager.h"

GestureManager::GestureManager(BLEHidAdafruit BLEhid, Adafruit_MPU6050 mpu6050, int button1Pin, int button2Pin, int button3Pin, int enableScrollPin)
{
    this->hid = BLEhid;
    this->mpu = mpu6050;
    this->sensitivity = SENSITIVITY_MAX/2;
    this->b1 = new Button(button1Pin);
    this->b2 = new Button(button2Pin);
    this->b3 = new Button(button3Pin);
    this->b4 = new Button(enableScrollPin);
}
GestureManager::~GestureManager()
{
    delete this->b1;
    delete this->b2;
    delete this->b3;
    delete this->b4;
}

void GestureManager::refresh(void)
{
    this->b1->check();
    this->b2->check();
    this->b3->check();
    this->b4->check();
    boolean b1State = this->b1->pressed();
    boolean b2State = this->b2->pressed();
    boolean b3State = this->b3->pressed();
    boolean b4State = this->b4->pressed();
    uint8_t buttonReport = b1State * MOUSE_BUTTON_LEFT + b2State + MOUSE_BUTTON_RIGHT + b3State * MOUSE_BUTTON_MIDDLE;

    sensors_event_t a, g, t;
    this->mpu.getEvent(&a, &g, &t);
    double xAccel = a.acceleration.x;
    double yAccel = a.acceleration.y;
    double zAccel = a.acceleration.z;

    double xRot = g.gyro.x;
    double yRot = g.gyro.y;
    double zRot = g.gyro.z;

    double xAccelDiff = xAccel - lastXAccel;
    double yAccelDiff = yAccel - lastYAccel;
    double zAccelDiff = zAccel - lastZAccel;

    double xRotDiff = xRot - lastXRot;
    double yRotDiff = yRot - lastYRot;
    double zRotDiff = zRot - lastZRot;
    
    // Gyroscope movements (rotation)
    // +x -> tilt back
    // -x -> tilt forward
    // +y -> tilt right
    // -y -> tilt left
    // +z -> spin CCW
    // -z -> spin CW

    // Accelerometer movements (linear motion)
    // +x -> right
    // -x -> left
    // +y -> forward
    // -y -> backward
    // +z -> up
    // -z -> down

    int xChange = 0;
    int yChange = 0;

    // TODO: some sort of motion to mouse movement translation wizardry     

    this->hid.mouseReport(buttonReport, xChange, yChange);
    this->lastUpdateTime = millis();

    lastXAccel = xAccel;
    lastYAccel = yAccel;
    lastZAccel = zAccel;
    lastXRot = xRot;
    lastYRot = yRot;
    lastZRot = zRot;
}

void GestureManager::setSensitivity(int sensitivity)
{
    this->sensitivity = constrain(sensitivity, 0, SENSITIVITY_MAX);
}
int GestureManager::getSensitivity(void)
{
    return this->sensitivity;
}
