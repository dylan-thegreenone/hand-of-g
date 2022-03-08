#include "ButtonManager.h"

Button::Button(int buttonPin, boolean pullup)
{
    this->pullup = pullup;
    this->pin = buttonPin;
    this->suspended = false;
    this->onPressSet = false;
    this->onReleaseSet = false;
    this->onChangeSet = false;
    this->onHoldSet = false;
    this->holdStart = 0;
    pinMode(this->pin, pullup ? INPUT_PULLUP : INPUT);
}
Button::Button(int buttonPin) : Button::Button(buttonPin, false){}
void Button::setOnPress(void (*callback)(void))
{
    this->onPress = callback;
    this->onPressSet = true;
}
void Button::setOnRelease(void (*callback)(void))
{
    this->onRelease = callback;
    this->onReleaseSet = true;
}
void Button::setOnChange(void (*callback)(void))
{
    this->onChange = callback;
    this->onChangeSet = true;
}
void Button::setOnHold(void (*callback)(void), unsigned long holdMillis)
{
    this->onHold = callback;
    this->holdDuration = holdMillis;
    this->onHoldSet = true;
}
void Button::unSetOnPress(void)
{
    this->onPressSet = false;
}
void Button::unSetOnRelease(void)
{
    this->onReleaseSet = false;
}
void Button::unSetOnChange(void)
{
    this->onChangeSet = false;
}
void Button::unSetOnHold(void)
{
    this->onHoldSet = false;
}
void Button::suspend(void)
{
    this->suspended = true;
}
void Button::resume(void)
{
    this->suspended = false;
}
void Button::check(void)
{
    boolean state = digitalRead(this->pin) ^ this->pullup;

    if (this->suspended) return;
    
    if (state == this->prevState)
    {
        if ( state && this->onHoldSet && millis() - this->holdStart >= this->holdDuration && !holdTriggered) 
        {
            this->holdTriggered = true;
            this->onHold();
        }
        return;
    }
    if (this->onChangeSet) this->onChange();

    if (state) //button pressed 
    {
        this->holdStart = millis();
        if (this->onPressSet) this->onPress();
    }
    else //button released
    {
        if (!this->holdTriggered && this->onReleaseSet) this->onRelease();
        
        this->holdTriggered = false;
    }
    this->prevState = state;
}
boolean Button::pressed(void)
{
    return this->prevState;
}
int Button::getPin(void)
{
    return this->pin;
}
