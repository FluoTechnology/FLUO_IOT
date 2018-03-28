#ifndef FluoTools_h
#define FluoTools_h

extern int LinkStatus;

enum{
    LINK_AP,
    LINK_STA,
    LINK_WAN,
    LINK_NO,
};

struct DigitalGpioMap_t {

    int   value;   // 0/1 or 0 to 255 or 0 to 180
    char  mode;   // O || I || P || S || A
    String  label; // OUTPUT or INPUT or ON/OFF ... it s a string 
    String  ReadyFunc;
};

struct AnalogGpioMap_t {

    int    value;      // decimal
    float  voltage;  // Volt 
    String  label;    // A0  ... it s a string 
    
};

extern struct DigitalGpioMap_t  DigitalGpioMap[14];
extern struct AnalogGpioMap_t   AnalogGpioMap[6];

extern void Task644ReadInput();

int CheckInterrupt(void);
void HardwareInit();

bool hasExp(unsigned long &prevTime, unsigned long interval);
char* string2char(String command);

void Scheduler();


#endif