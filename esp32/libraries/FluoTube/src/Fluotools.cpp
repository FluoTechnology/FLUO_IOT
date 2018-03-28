#include <Arduino.h>
#include "Fluotools.h"
#include "FluoSD.h"
#include "644VirtualCom/protocol.h"

// struct GPIO
struct DigitalGpioMap_t DigitalGpioMap[14] = {};
struct AnalogGpioMap_t AnalogGpioMap[6] = {};


// ISR variables
volatile int StateAPRST;
volatile int StateTOUCHTEST;

//Millis - Time
unsigned long MillisStart;

int LinkStatus;

void LedBlinkTest(int, int);
void ledblink(int, int, int);

void TaskService();
void Task644Communication();
void Task644ReadInput();

// ISR Function
void ISR_AP_RST()
{
  StateAPRST = !StateAPRST;
  delay(10);

}
void ISR_TOUCH_TEST()
{
  StateTOUCHTEST = 1; 
}

void HardwareInit()
{
    pinMode(LED_STATUS, OUTPUT); 
    pinMode(LED_LINK, OUTPUT);
    pinMode(LED_CLOUD, OUTPUT);
    pinMode(LED_BLE, OUTPUT);
    pinMode(SPI_EN, OUTPUT);
    pinMode(SDCD, INPUT);
    pinMode(RESET_644P, OUTPUT);
    
    digitalWrite(LED_STATUS, 1); // force on  
    digitalWrite(LED_CLOUD, 0);
    digitalWrite(LED_LINK, 0);
    digitalWrite(LED_BLE, 0);
    digitalWrite(SPI_EN, 1); // 0 enable - 1 disable

    //init 644p Sync
    digitalWrite(RESET_644P, 0);
    delay(10);
    digitalWrite(RESET_644P, 1);
    delay(10);
    digitalWrite(RESET_644P, 0);

    pinMode(INTERRUPT_644P, OUTPUT);   //interrupt 644p
    
    pinMode(BUTTON_CHECK, OUTPUT);
    pinMode(BUTTON_APRST, INPUT_PULLUP);
  
    attachInterrupt(digitalPinToInterrupt(BUTTON_APRST), ISR_AP_RST, CHANGE);
    touchAttachInterrupt(BUTTON_CHECK, ISR_TOUCH_TEST, 40);
  
    Serial.begin(115200);
    SerialInternal.begin(57600);

    SDCardInit();

    MillisStart = millis();
}

char* string2char(String command){
  if(command.length()!=0){
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}

bool hasExp(unsigned long &prevTime, unsigned long interval) 
{
  if (  millis() - prevTime > interval ) 
  {
    prevTime = millis();
    return true;
  }

  else     
    return false;
}

int CheckInterrupt(void)
{
  unsigned long firstTime;
  long millis_held;
  
  // check Test Touch Button
  if(StateTOUCHTEST)
  {

    delay(50);
    LedBlinkTest(5, 180); 

    StateTOUCHTEST = 0;

    Serial.println("[checkInterrupt] TEST LED BUTTON ");

    return 2; // touch Test pressed
  }

  // check AP-RST Button
  if (StateAPRST == 1)
  {
    firstTime = millis();

    while(StateAPRST == 1)
    {

      millis_held = (millis() - firstTime);

      if ( millis_held >= 5000 ) // condition 5 sec valid
      {
        
        //functions to call
        ledblink(10,180, LED_STATUS);

        StateAPRST = 0;

        Serial.println("[checkInterrupt] AP_RST BUTTON PRESSED 5 sec");
        return 1;
      }

    }

  } // if check AP RST Button

  return 0; // nothing to do

}

// Just a simple helper function to blink an led in various patterns
void ledblink(int times, int lengthms, int pinnum)
{
  for (int x=0; x<times;x++) 
  {
    digitalWrite(pinnum, HIGH);
    delay (lengthms);
    digitalWrite(pinnum, LOW);
    delay(lengthms);
  }
}

// Led blink for Testing Touch Buttton 
void LedBlinkTest(int times, int lengthms)
{
  for (int x=0; x<times;x++) 
  {
    digitalWrite(LED_CLOUD, HIGH);
    digitalWrite(LED_STATUS, HIGH);
    digitalWrite(LED_LINK, HIGH);
    digitalWrite(LED_BLE, HIGH);
    delay (lengthms);
    digitalWrite(LED_CLOUD, LOW);
    digitalWrite(LED_STATUS, LOW);
    digitalWrite(LED_LINK, LOW);
    digitalWrite(LED_BLE, LOW);
    delay(lengthms);
  }
}

unsigned long prevMillisTaskService;
unsigned long prevMillisTask644Communication;
unsigned long prevMillisTask644ReadInput;

byte TaskServiceExec;
byte Task644ReadInputExec;

byte Task644CommunicationExec = 1; // DISABLE !

void Scheduler()
{

  if ( hasExp(prevMillisTaskService, 2000) ) // 1 second interval
  { 
    if (!TaskServiceExec) 
      TaskService();
  }

  if ( hasExp(prevMillisTask644Communication, 2000) ) // 1 second interval
  { 
    if (!Task644CommunicationExec) 
      Task644Communication();
  }

  if ( hasExp(prevMillisTask644ReadInput, 1000) ) // 1 second interval
  { 
    if (!Task644ReadInputExec) 
      Task644ReadInput();
  }

  //... other task
}

void TaskService()
{ 

  static int TmpBlinkLink;

  digitalWrite(LED_STATUS, HIGH); // ESP32 Alive

  if (LinkStatus == LINK_AP)
  {
    digitalWrite(LED_CLOUD, LOW);
    digitalWrite(LED_LINK, (TmpBlinkLink) ? HIGH : LOW); // ESP32 Alive
    TmpBlinkLink = !TmpBlinkLink;
  }

  if (LinkStatus == LINK_STA) 
  {
    digitalWrite(LED_LINK, HIGH);
    digitalWrite(LED_CLOUD, LOW);
  }

  if (LinkStatus == LINK_WAN)
  {
    digitalWrite(LED_LINK, HIGH);
    digitalWrite(LED_CLOUD, HIGH);
  }

  if (LinkStatus == LINK_NO) 
  {
    digitalWrite(LED_LINK, LOW);
    digitalWrite(LED_CLOUD, LOW);
  }
}




// ONLY TEST
void Task644Communication()
{

    static byte TmpB;
    static byte oneshot = 1;

    int brightness = 15;
    int fadeAmount = 15;

 

    if (oneshot)
    {

        VirtualpinMode(13, OUTPUT);
        VirtualpinMode(10, OUTPUT);

        VirtualpinMode(8, INPUT);

        oneshot = 0; // like a setup()
    }
    
    VirtualdigitalWrite(13, (TmpB) ? HIGH : LOW);
    TmpB = !TmpB;

    Serial.println("Status DigitalPin: ");
    Serial.println( VirtualdigitalRead(8) );

    //fade
    while(1)
    {
        VirtualanalogWrite(10, brightness);

        // change the brightness for next time through the loop:
        brightness = brightness + fadeAmount;

        // reverse the direction of the fading at the ends of the fade:
        if (brightness >= 255) 
        {
            fadeAmount = -fadeAmount;
        }

        if(brightness == 0) // exit after 1 cycle
        {
            VirtualanalogWrite(10, 0);
            break;
        }

        // wait for 50 milliseconds to see the dimming effect
        delay(50);
    }

    //servo

    VirtualservoAttach(3);
    VirtualservoAttach(5);

    //VirtualservoWrite(3, 0);
    //VirtualservoWrite(5, 0);

    for (int pos = 0; pos <= 90; pos += 5) 
    { 
        VirtualservoWrite(3, pos);
        VirtualservoWrite(5, pos);

        delay(50);                       
    }

    VirtualservoWrite(3, 0);
    VirtualservoWrite(5, 0);

    Serial.println("Status Servo: ");

    Serial.println( VirtualservoRead(3) );
    Serial.println( VirtualservoRead(5) );

    VirtualservoDetach(3);
    VirtualservoDetach(5);

    //ADC
    float voltage;

    Serial.println("Status ADC: ");

    for(int i = 24; i < 30; i++)
    {
        int sensorValue = VirtualanalogRead(i);
        // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
        if (sensorValue > 0)
            voltage = sensorValue * 0.00488; //(5.0 / 1023.0)
        // print out the value you read:
        Serial.println(voltage);

        delay(20);
    }

}