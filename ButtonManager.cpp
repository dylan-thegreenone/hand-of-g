#include "ButtonManager.h"

Button::Button(int buttonPin, boolean pullup)
{
    this->pullup = pullup;
    this->pin = buttonPin;
    this->suspended = false;
    this->onPressSet = false;
    this->onReleaseSet = false;
    this->onChangeSet = false;
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

    if (state == this->prevState) return;
    
    if (this->onChangeSet) this->onChange();

    if (state && onPressSet) this->onPress();
    else if (onReleaseSet) this->onRelease();
    
    this->prevState = state;
}
boolean Button::pressed()
{
    return this->prevState
}
