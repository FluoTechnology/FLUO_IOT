#ifndef Fluotube_h
#define Fluotube_h

#include <Arduino.h>

class FluoTubeClass {
    public:
   
    // Arduino classic WRAP function 
    
    void digitalWrite(int pin, int val); //led blink

    int digitalRead(int pin); //button

    int analogRead(int pin); //potenziomerto

    int analogWrite(int pin, int val); // led fade

    //float pulseIn_set(int pin, int val, int trials); // no

    //float pulseIn(int pin, int val); // no

    void pinMode(int pin, int val); 

    int servoRead(int pin);

    void servoWriteMicroseconds(int pin, int uS);

    void servoWrite(int pin, int angle);

    void servoDetach(int pin);

    int servoAttach(int pin, int min=544, int max=2400);

    void debug(String txt);
    void debugln(String txt);

    //Init

    void setup();

};

extern FluoTubeClass FluoTube;


#endif