#include <Adafruit_MPU6050.h>
#include <bluefruit.h>
#include "BluefruitConfig.h"
#include "GestureManager.h"
#include "ButtonManager.h"

const int MOVE_SCALE = 50;
const int SENSITIVITY_POT = A5;
const int INDEX_BUTTON = 27;
const int MIDDLE_BUTTON = 30;
const int RING_BUTTON = 11;
const int PINKY_BUTTON = 7;
const int UTIL_BUTTON_1 = 15;
const int UTIL_BUTTON_2 = 16;

bool leftPressed = false;
bool middlePressed = false;
bool rightPressed = false;
bool frontPressed = false;
bool backPressed = false;

unsigned long prevTime = 0;
unsigned long lastAdvertiseStart = 0;

int moveSensitivity = 0;

BLEDis bledis;
BLEHidAdafruit blehid;
Adafruit_MPU6050 mpu;
GestureManager gMan (
  blehid,
  mpu,
  (int) INDEX_BUTTON,
  (int) MIDDLE_BUTTON,
  (int) RING_BUTTON,
  (int) PINKY_BUTTON
);
Button utilB1(UTIL_BUTTON_1);
Button utilB2(UTIL_BUTTON_2);

void startAdv(void)
{  
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_MOUSE);
    
    // Include BLE HID service
    Bluefruit.Advertising.addService(blehid);
  
    Bluefruit.Advertising.addName();
    Bluefruit.Advertising.restartOnDisconnect(AUTO_RESTART_ADVERTISING);
    Bluefruit.Advertising.setInterval(ADVERTISING_INTERVAL_MIN, ADVERTISING_INTERVAL_MAX);    // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(FAST_MODE_TIMEOUT);      // number of seconds in fast mode
    Bluefruit.Advertising.start(ADVERTISING_TIMEOUT);
}

void setup() 
{
    // setup input buttons/devices
    pinMode(SENSITIVITY_POT, INPUT);
    utilB1.setOnPress([]() { Serial.println("B1");});
    
    if (SERIAL_DEBUG) 
    {
        Serial.begin(SERIAL_BAUD);
        while ( !Serial ) delay(10);   // for nrf52840 with native usb
        
        Serial.println("Enter following characters");
        Serial.println("- 'WASD'  to move mouse (up, left, down, right)");
        Serial.println("- 'LRMBF' to press mouse button(s) (left, right, middle, backward, forward)");
        Serial.println("- 'X'     to release mouse button(s)");
    }
    if (PASSCODE_ENABLED)
    {
        Bluefruit.Security.setPIN(PASSCODE);
    }
    
    Bluefruit.setName(DEVICE_NAME);
    Bluefruit.begin(1,0);
    
//    Bluefruit.Periph.clearBonds();
    // HID Device can have a min connection interval of 9*1.25 = 11.25 ms
    Bluefruit.Periph.setConnInterval(CONN_INTERVAL_MIN, CONN_INTERVAL_MAX); // min = 9*1.25=11.25 ms, max = 16*1.25=20ms
    Bluefruit.setTxPower(4);   
    
    
    // Configure and Start Device Information Service
    bledis.setManufacturer(DEVICE_MANUFACTURER);
    bledis.setModel(DEVICE_MODEL);

//    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

    if (bledis.begin()) 
    {
        if (SERIAL_DEBUG)
        {
            Serial.println("Could not start BLE device information service");
        }
        while (1) delay(100);   //Necessary component, just accept futility if it fails
    }
    if (blehid.begin()) 
    {
        if (SERIAL_DEBUG)
        {
            Serial.println("Could not start BLE HID service");
        }
        while (1) delay(100);   //Necessary component, just accept futility if it fails
    }
    if (!mpu.begin()) 
    {
        if (SERIAL_DEBUG)
        {
            Serial.println("Could not find MPU6050 component");
        }
        // while (1) delay(100);   //Necessary component, just accept futility if it fails
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

    if (SERIAL_DEBUG)
    {
        Serial.println("All components started");
    }
   
    startAdv();
    if (SERIAL_DEBUG)
    {
        Serial.println("BLE Advertising started");
    }
    prevTime = millis();
}

void loop() 
{    
    moveSensitivity = map(analogRead(SENSITIVITY_POT), 1023, 0, 0, SENSITIVITY_MAX);
    gMan.setSensitivity(moveSensitivity);
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);

    double aX = a.acceleration.x;
    utilB1.check();
    utilB2.check();
    
    if (Serial.available())
    {
        char ch = (char) Serial.read();
        int xDiff = 0;
        int yDiff = 0;
        switch(ch)
        {
        // WASD to move the mouse
        case 'W':
            yDiff = -moveSensitivity;
            break;
  
        case 'A':
            xDiff = -moveSensitivity;
            break;
  
        case 'S':
            yDiff = moveSensitivity;
            break;
  
        case 'D':
            xDiff = moveSensitivity;
            break;
  
        // LRMBF for mouse button(s)
        case 'L':
            leftPressed = !leftPressed;
            break;
  
        case 'R':
            rightPressed = !rightPressed;
            break;
  
        case 'M':
            middlePressed = !middlePressed;
            break;
  
        case 'B':
            backPressed = !backPressed;
            break;
  
        case 'F':
            frontPressed = !frontPressed;
            break;
  
        case 'X':
            blehid.mouseButtonRelease();
            break;
  
       default: 
            break;
       }
       
        uint8_t buttonBin = leftPressed * MOUSE_BUTTON_LEFT + rightPressed * MOUSE_BUTTON_RIGHT + middlePressed * MOUSE_BUTTON_MIDDLE;
        blehid.mouseReport(buttonBin, xDiff, yDiff);
    }
    prevTime = millis();
}
