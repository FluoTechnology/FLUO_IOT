//include required libraries
#include "FluoTasks.h"
#include <avr/wdt.h>
#include <avr/interrupt.h>

// Global settings
const uint8_t MAX_TASKS = 10; 
volatile unsigned long _ticksCounter = 0;
// Set your max interval here (max 2^32-1) - default 225000 ticks (1 hour)
#define MAX_TASK_INTERVAL 225000UL


// Tasks variables
struct tasks_core {
    void (*taskPointer)(void); 
    volatile unsigned long userTasksInterval;
    volatile unsigned long plannedTask;
    volatile uint8_t taskIsActive; 
};
tasks_core tasks[MAX_TASKS];

volatile uint8_t _numTasks; 
volatile uint8_t _initialized;
volatile uint16_t _wdtResetTimeout;
volatile uint8_t _taskIsRunning;
volatile uint16_t _maxTimeouts;

// Class constructor
FluoTasks::FluoTasks(void) {
	_initialized = 0;
}

// Class initialization
void FluoTasks::begin(uint16_t resetTimeout) {
	_wdtResetTimeout = resetTimeout;
    setWDT(); 
	_initialized = 1;
	_numTasks = 0;
	_taskIsRunning = 0;
}

// Add a task to the scheduler
uint8_t FluoTasks::addTask(void (*userTask)(void), unsigned long taskInterval, uint8_t taskStatus) {
	if ((_initialized == 0) || (_numTasks == MAX_TASKS)) { 
		return 1;
	}
	if ((taskInterval < 1) || (taskInterval > MAX_TASK_INTERVAL)) {
		taskInterval = 1; 
	}
    if (taskStatus > SCHEDULED_IMMEDIATESTART) {
        taskStatus = SCHEDULED;
    }
    // Add the task to the scheduler
    SREG &= ~(1<<SREG_I); 
	tasks[_numTasks].taskPointer = *userTask;
	tasks[_numTasks].taskIsActive = taskStatus & 0b00000011; 
	tasks[_numTasks].userTasksInterval = taskInterval;
	tasks[_numTasks].plannedTask = _ticksCounter + ((taskStatus & 0b00000100)? 0 : taskInterval);
	_numTasks++;
    // Restart the scheduler
    SREG |= (1<<SREG_I); 
    return 0;
}

// Pause a specific task
uint8_t FluoTasks::pauseTask(void (*userTask)(void)) {
    return (setTask(userTask, 0));
}

// Restart a specific task
uint8_t FluoTasks::restartTask(void (*userTask)(void)) {
    return (setTask(userTask, 1));
}

// Modify an existing task
uint8_t FluoTasks::modifyTask(void (*userTask)(void), unsigned long taskInterval, uint8_t oneTimeTask) {
	if ((oneTimeTask < SCHEDULED) && (oneTimeTask > ONETIME)) {
		oneTimeTask = NULL;
	}
	if ((taskInterval < 1) || (taskInterval > MAX_TASK_INTERVAL)) {
		taskInterval = 1; 
	}
	// Modify the task into the scheduler & Halt the scheduler
	SREG &= ~(1<<SREG_I); 
	uint8_t tempI = 0;
	uint8_t _done = 1;
	do {
		if (tasks[tempI].taskPointer == *userTask) { 
			tasks[tempI].userTasksInterval = taskInterval;
			if (oneTimeTask != NULL) {
				tasks[tempI].taskIsActive = oneTimeTask;
			}
			tasks[tempI].plannedTask = _ticksCounter + taskInterval;
			_done = 0;
			break;
		}
		tempI++;
    } while (tempI < _numTasks);
    // Restart the scheduler
    SREG |= (1<<SREG_I); 
	return _done;

}

// Manage the tasks' status
uint8_t FluoTasks::setTask(void (*userTask)(void), uint8_t tempStatus, unsigned long taskInterval) {
    if ((_initialized == 0) || (_numTasks == 0)) {
		return 1;
	}
    // Halt the scheduler
    SREG &= ~(1<<SREG_I); 
	uint8_t tempI = 0;
	do {
        if (tasks[tempI].taskPointer == *userTask) {
            tasks[tempI].taskIsActive = tempStatus;
            if (tempStatus == SCHEDULED) {
				if (taskInterval == NULL) {
					tasks[tempI].plannedTask = _ticksCounter + tasks[tempI].userTasksInterval;
				} else {
					tasks[tempI].plannedTask = _ticksCounter + taskInterval;
				}
			}
            break;
        } else {
            tempI++;
    }
	} while (tempI < _numTasks);
    // Restart the scheduler
    SREG |= (1<<SREG_I); 
    return 0;
}

// Remove a task from the scheduler
uint8_t FluoTasks::removeTask(void (*userTask)(void)) {
	if ((_initialized == 0) || (_numTasks == 0)) {
		return 1;
	}
    // Halt the scheduler
    SREG &= ~(1<<SREG_I); 
	uint8_t tempI = 0;
	do {
		if (tasks[tempI].taskPointer == *userTask) {
            if ((tempI + 1) == _numTasks) {
                _numTasks--;
            } else if (_numTasks > 1) {
                for (uint8_t tempJ = tempI; tempJ < _numTasks; tempJ++) {
                    tasks[tempJ].taskPointer = tasks[tempJ + 1].taskPointer;
                    tasks[tempJ].taskIsActive = tasks[tempJ + 1].taskIsActive;
                    tasks[tempJ].userTasksInterval = tasks[tempJ + 1].userTasksInterval;
                    tasks[tempJ].plannedTask = tasks[tempJ + 1].plannedTask;
                }
                _numTasks -= 1;
            } else {
                _numTasks = 0;
            }
			break;
		} else {
			tempI++;
		}
	} while (tempI < _numTasks);
    // Restart the scheduler
    SREG |= (1<<SREG_I); 
    return 0;
}

// Check if a task is running
uint8_t FluoTasks::getTaskStatus(void (*userTask)(void)) {
	if ((_initialized == 0) || (_numTasks == 0)) {
		return -1;
	}
    uint8_t tempJ = 255; 
    SREG &= ~(1<<SREG_I); 
	uint8_t tempI = 0;
    // Look for the task
	do {
		if (tasks[tempI].taskPointer == *userTask) {
            tempJ = tasks[tempI].taskIsActive;
            break;
        }
        tempI++;
    } while (tempI < _numTasks);
    // Restart the scheduler
    SREG |= (1<<SREG_I); 
    return tempJ; 
}

// Convert milliseconds in ticks
uint32_t FluoTasks::convertMs(uint32_t tempMs) {
    if (tempMs < 16) {
        return 1;
    }
	tempMs = tempMs >> 4;
    if (tempMs > MAX_TASK_INTERVAL) {
        return MAX_TASK_INTERVAL;
    } else {
        return tempMs;
    }
}

// Reset the MCU
void FluoTasks::reset(void) {
    wdt_disable();
    wdt_enable(WDTO_2S);
    while(1){}; 
}

ISR(WDT_vect, ISR_NOBLOCK) {
	_ticksCounter++; 
    if (_wdtResetTimeout ) {
        _WD_CONTROL_REG |= (1<<WDIE); 
        // Check if a task is already running
        if (_taskIsRunning) {
            // Check if the maximum # of timeouts has been reached
            _maxTimeouts--;
            if (_maxTimeouts == 0) {
                // Max numb. of timeouts reached - next time we need a chip reset!
                _WD_CONTROL_REG &= ~(1<<WDIE);
            }
            return;
        }
    }

    // SCHEDULER
	uint8_t tempI = 0; 
	void (*savedJobPointer)(void);
	while (tempI < _numTasks) {
		if (tasks[tempI].taskIsActive > 0 ) { //
            // Check if it's time to execute the task
            if ((long)(_ticksCounter - tasks[tempI].plannedTask) >=0) {
                // Prepare the counters to monitor if a task will freeze
				_maxTimeouts = _wdtResetTimeout;
				_taskIsRunning = 1;
				savedJobPointer = tasks[tempI].taskPointer; 
				savedJobPointer(); 
                // Reset the counters
				_taskIsRunning = 0;
				if (tasks[tempI].taskIsActive == ONETIME) { 
					// Re-determine the task's position in case it's changed
					tempI = 0;
					do {
						if (tasks[tempI].taskPointer == savedJobPointer) { //found the task
							break;
						} else {
							tempI++;
						}
					} while (tempI <= _numTasks);
					// Remove it from the scheduler
                    if (tempI == _numTasks) { 
                        _numTasks--;
                    } else if (_numTasks > 1) {
                        for (uint8_t tempJ = tempI; tempJ < _numTasks; tempJ++) {
                            tasks[tempJ].taskPointer = tasks[tempJ + 1].taskPointer;
                            tasks[tempJ].taskIsActive = tasks[tempJ + 1].taskIsActive;
                            tasks[tempJ].userTasksInterval = tasks[tempJ + 1].userTasksInterval;
                            tasks[tempJ].plannedTask = tasks[tempJ + 1].plannedTask;
                        }
                        _numTasks -= 1;
                    } else {
                        _numTasks = 0;
                    }
                } else {
                    // Let's schedule next start
                    tasks[tempI].plannedTask = _ticksCounter + tasks[tempI].userTasksInterval;
                }
			}
		}
	    tempI++;
	}
}

// Set the WatchDog Timer
void FluoTasks::setWDT() {
    MCUSR = 0; 
    wdt_disable();
    SREG &= ~(1<<SREG_I); 
    byte _tempI = (1<<WDIE);
    if (_wdtResetTimeout) {
        _tempI |= (1<<WDE);
    }
    _WD_CONTROL_REG = ((1<<_WD_CHANGE_BIT) | (1<<WDE));
    _WD_CONTROL_REG = _tempI;
    SREG |= (1<<SREG_I);
}

// Halt the scheduler
void FluoTasks::haltScheduler() {
    SREG &= ~(1<<SREG_I); 
    wdt_disable(); 
    SREG |= (1<<SREG_I);
}

// Restart the scheduler
void FluoTasks::restartScheduler() {
	if (_initialized) {
		setWDT();
	}
}
