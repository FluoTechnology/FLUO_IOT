#ifndef Fluoweb_h
#define Fluoweb_h

//#include "FluoSD.h"


//Extern
void WebSetup(void);
void WebLoop(void);


class WiFiCallbackClass {
   public:
   
   	void APmode();
   	void STAmode();
   	void APSTAmode();
   	void SCANmode();

};


#endif