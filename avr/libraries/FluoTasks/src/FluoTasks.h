
#ifndef FluoTasks_H
#define FluoTasks_H

// Library version
#define FluoTasks_VERSION 102FLUO
#include "Arduino.h"

// Constants
const uint8_t PAUSED = 0;
const uint8_t SCHEDULED = 1;                                   
const uint8_t SCHEDULED_IMMEDIATESTART = 5;                 
const uint8_t IMMEDIATESTART = SCHEDULED_IMMEDIATESTART;    
const uint8_t ONETIME = 2;

// pbRTOS class
class FluoTasks {
	public:

		FluoTasks();

        void begin(uint16_t resetTimeout = 0);
		uint8_t addTask(void (*)(void), unsigned long, uint8_t taskStatus = SCHEDULED);
		uint8_t removeTask(void (*)(void));

		uint8_t pauseTask(void (*)(void));
        uint8_t restartTask(void (*)(void));
		uint8_t modifyTask(void (*)(void), unsigned long, uint8_t oneTimeTask = NULL);
		uint8_t getTaskStatus(void (*)(void));
        uint32_t convertMs(uint32_t);
        void haltScheduler(void);
		void restartScheduler(void);
        void reset(void);

	private:

        void setWDT();
        uint8_t setTask(void (*)(void), uint8_t, unsigned long taskInterval = NULL);
};

#endif 
