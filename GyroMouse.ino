#include <Adafruit_MPU6050.h>
#include <bluefruit.h>
#include <LED.h>
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
const int STATUS_LED_RED = A3;
const int STATUS_LED_GREEN = A2;

bool leftPressed,  rightPressed, middlePressed, backPressed, frontPressed;

unsigned long prevTime = 0;
unsigned long lastAdvertiseStart = 0;

int moveSensitivity = 0;

LED greenLED(STATUS_LED_GREEN);
LED redLED(STATUS_LED_RED);

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

void disconnectAndStartAdv(void)
{  
    if (Bluefruit.connected() > 0)
    {
        Bluefruit.disconnect(0);
    }
    if (!Bluefruit.Advertising.isRunning() || millis() - lastAdvertiseStart > ADVERTISING_TIMEOUT * 1000)
    {
        Bluefruit.Advertising.start(ADVERTISING_TIMEOUT);
        lastAdvertiseStart = millis();
    }
}

// Must be run once before advertise is started
void setupAdv(void)
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
}

// if crucial component fails, prevent running further
// AKA feather freak out
void panic(const char* message)
{
    if (SERIAL_DEBUG)
    {
        Serial.println(message);
    }
    redLED.blink(0, 500);
    greenLED.blink(0, 250);
    while (1) delay(100);
}
void setup() 
{
    // setup input buttons/devices
    pinMode(SENSITIVITY_POT, INPUT);
    redLED.begin(2000);
    redLED.begin(2000);


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
    
    setupAdv();
        
    // Configure and Start Device Information Service
    bledis.setManufacturer(DEVICE_MANUFACTURER);
    bledis.setModel(DEVICE_MODEL);

//    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

    if (bledis.begin()) 
    {
        panic("Could not start BLE device information service");
    }
    if (blehid.begin()) 
    {
        panic("Could not start BLE HID service");
    }
    if (!mpu.begin()) 
    {
        panic("Could not find MPU6050 acceleromoter/gyroscope component");
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

    if (SERIAL_DEBUG)
    {
        Serial.println("All components started");
    }
     
    disconnectAndStartAdv();
    if (SERIAL_DEBUG)
    {
        Serial.println("BLE Advertising started");
    }

    utilB1.setOnPress(disconnectAndStartAdv);


    prevTime = millis();
}

void loop() 
{    
    moveSensitivity = map(analogRead(SENSITIVITY_POT), 1023, 0, 0, SENSITIVITY_MAX);
    gMan.setSensitivity(moveSensitivity);
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);

    // double aX = a.acceleration.x;
    utilB1.check();
    utilB2.check();
    gMan.refresh();
    
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
        // blehid.mouseReport(buttonBin, xDiff, yDiff);
    }
    prevTime = millis();
}
