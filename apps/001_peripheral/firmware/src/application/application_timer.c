/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "application_timer.h"



/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

struct  {
    TimerHandle_t timer;
    TIMER_CALLBACK_FUNC_T func;
} _data;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
void _APPLICATION_TIMER_Callback(TimerHandle_t pxTimer) {
    _data.func();
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void APPLICATION_TIMER_Initialize(TIMER_CALLBACK_FUNC_T func) {
    _data.timer = xTimerCreate(
            "APP_TIMER",    // Just a name, not used by the kernel.
            100,            // The timer period in ticks.
            pdTRUE,         // The timer is auto-reloaded itself when it expire.
            0,              // Assign each timer a unique id equal to its array index.
            _APPLICATION_TIMER_Callback // Each timer calls the same callback when it expires
            );
    if (_data.timer == NULL) {
        APPLICATION_ErrorHandler(ERROR_TIMER_CREATE_FAILED);
    } else {
        // SUCCESS
    }
    _data.func = func;
    if (xTimerStart(_data.timer, 0) != pdPASS) {
        APPLICATION_ErrorHandler(ERROR_TIMER_START_FAILED);
    } else {
        // SUCCESS
    }
    
    // TODO: ADD IMPLEMENTATION
}


/* *****************************************************************************
 End of File
 */
