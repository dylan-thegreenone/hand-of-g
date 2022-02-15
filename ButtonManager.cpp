#include "ButtonManager.h"

Button::Button(int buttonPin, boolean pullup)
{
    this->pullup = pullup;
    this->pin = buttonPin;
    this->suspended = false;
    pinMode(this->pin, pullup ? INPUT_PULLUP : INPUT);
}
void Button::setOnPress(void (*callback)(void))
{
    this->onPress = callback;
}
void Button::setOnRelease(void (*callback)(void))
{
    this->onRelease = callback;
}
void Button::setOnChange(void (*callback)(void))
{
    this->onChange = callback;
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

    if (state == prevState) return;
    
    this->onChange();

    if (state) this->onPress();
    else this->onRelease();
    
    prevState = state;
}
