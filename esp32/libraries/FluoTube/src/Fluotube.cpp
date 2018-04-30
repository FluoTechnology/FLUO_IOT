#include "644VirtualCom/protocol.h"
#include "Fluotools.h"

#include "Fluotube.h"

// Arduino classic WRAP function 

void FluoTubeClass::pinMode(int pin, int val)
{
    VirtualpinMode(pin, val);
}

void FluoTubeClass::digitalWrite(int pin, int val)
{
    VirtualdigitalWrite(pin, val);
}

int FluoTubeClass::digitalRead(int pin)
{
    return VirtualdigitalRead(pin);
}

int FluoTubeClass::analogRead(int pin)
{
    return VirtualanalogRead(pin);
}

int FluoTubeClass::analogWrite(int pin, int val)
{
    return VirtualanalogWrite(pin, val);
}

//float pulseIn_set(int pin, int val, int trials); // no

//float pulseIn(int pin, int val); // no

int FluoTubeClass::servoRead(int pin)
{
    return VirtualservoRead(pin);
}

void FluoTubeClass::servoWriteMicroseconds(int pin, int uS)
{
    VirtualservoWriteMicroseconds(pin, uS);
}


void FluoTubeClass::servoWrite(int pin, int angle)
{
    VirtualservoWrite(pin, angle);
}

void FluoTubeClass::servoDetach(int pin)
{
    VirtualservoDetach(pin);
}

int FluoTubeClass::servoAttach(int pin, int min, int max)
{
    return VirtualservoAttach(pin, 544, 2400);
}

void FluoTubeClass::debug(String txt)
{
    VirtualserialSend(txt);
}

void FluoTubeClass::debugln(String txt)
{
    String tmp = txt + "\n\r";
    VirtualserialSend(tmp);
}

//Init
void FluoTubeClass::setup()
{
    HardwareInit(1);
    delay(1000); // wait AVR start
}

FluoTubeClass FluoTube;

