#include "LEDManager.h"

LED::LED(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    this->state = 0;
    this->nextTriggerTime = 0;
    this->blinking = false;
}
boolean LED::isOn(void)
{
    return this->state;
}
void LED::on(void)
{
    digitalWrite(this->pin, HIGH);
    this->state = 1;
    this->blinking = false;
}
void LED::off(void)
{
    digitalWrite(this->pin, LOW);
    this->state = 0;
    this->blinking = false;
}
void LED::toggle(void)
{
    if (this->state)
    {
        this->state = 0;
        digitalWrite(this->pin,LOW);
    }
    else
    {
        this->state = 1;
        digitalWrite(this->pin,HIGH);
    }
}
void LED::blink(long onMillis, long offMillis, long stopAfter)
{
    this->blinking = true;
    this->onDur = onMillis;
    this->offDur = offMillis;
    this->nextTriggerTime = millis() + onMillis;
    this->blinkStop = millis() + stopAfter;
    this->mortalBlink = stopAfter > 0;
    this->on();
}
void LED::blink(long onMillis, long offMillis)
{
    this->blink(onMillis, offMillis, 0);
}
void LED::blink(long interval)
{
    this->blink(interval, interval);
}
void LED::flash(long durationOn)
{
    this->blink(durationOn, durationOn);
}
void LED::cancelBlink(void)
{
    this->blinking = false;
}
void LED::refresh(void)
{
    if (this->blinking)
    {
        if (this->mortalBlink && millis() >= this->blinkStop)
        {
            this->off();
        }
        else if (millis() >= this->nextTriggerTime)
        {     
            this->toggle();
            this->nextTriggerTime = millis() + (this->state ? this->onDur : this->offDur);
        }
    }
}
