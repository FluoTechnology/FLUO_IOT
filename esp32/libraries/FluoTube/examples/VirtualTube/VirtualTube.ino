#include <Fluotube.h>

void setup(){
     
    FluoTube.setup();
  
    FluoTube.debugln("ESP32 start");
  
    FluoTube.pinMode(13, OUTPUT);
    FluoTube.pinMode(10, OUTPUT);
  
    FluoTube.pinMode(8, INPUT);
}

// the loop function runs over and over again forever
void loop() {
    
    FluoTube.digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    FluoTube.digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
  
    FluoTube.debug("Status DigitalPin: ");
    FluoTube.debugln( String(FluoTube.digitalRead(8)) );

    delay(1000);

    pwm_fade();

    delay(1000);

    servo();

    delay(1000);

    adc();

    delay(1000);

}


void pwm_fade()
{

    int brightness = 15;
    int fadeAmount = 15;
    
    while(1)
    {
        FluoTube.analogWrite(10, brightness);

        // change the brightness for next time through the loop:
        brightness = brightness + fadeAmount;

        // reverse the direction of the fading at the ends of the fade:
        if (brightness >= 255) 
        {
            fadeAmount = -fadeAmount;
        }

        if(brightness == 0) // exit after 1 cycle
        {
            FluoTube.analogWrite(10, 0);
            break;
        }

        // wait for 50 milliseconds to see the dimming effect
        delay(50);
    }
}

void servo()
{
    FluoTube.servoAttach(5);

    for (int pos = 0; pos <= 90; pos += 5) 
    { 

        FluoTube.servoWrite(5, pos);
        delay(50);                       
    }

    FluoTube.servoWrite(5, 0);

    FluoTube.debug("Status Servo: ");
    FluoTube.debugln( String(FluoTube.servoRead(5)) );

    FluoTube.servoDetach(5);

}

void adc()
{
    //ADC
    float voltage;

    FluoTube.debugln("Status ADC: ");

    for(int i = 24; i < 30; i++)
    {
        int sensorValue = FluoTube.analogRead(i);
        // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
        if (sensorValue > 0)
            voltage = sensorValue * 0.004888758553275; //(5.0 / 1023.0)
        // print out the value you read:
        FluoTube.debugln( String (i) + "  " + String(voltage) );

        delay(250);
    }
}


