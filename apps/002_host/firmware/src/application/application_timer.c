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
#include "application.h"
#include "definitions.h"
#include "timers.h"
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static volatile uint32_t timer_counter;
static bool enable_timeout;
static volatile uint32_t timeout_counter;
static volatile bool is_timeout;
static TIMER_CALLBACK_FUNC_T callback;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

static void _APPLICATION_TIMER_Tc0Callback(TC_TIMER_STATUS status, uintptr_t context) {
    timer_counter++;
    if ( enable_timeout == true ) {
        if (timeout_counter > 0) {
            timeout_counter--;
            is_timeout = false;
        } else {
            is_timeout = true;
        }
    }
    if (callback != NULL) {
        callback(timer_counter);
    }
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void APPLICATION_TIMER_Initialize(void) {

    // Timer/Counter TC0
    TC0_TimerCallbackRegister(_APPLICATION_TIMER_Tc0Callback, 0);
    timer_counter = 0;
    APPLICATION_TIMER_DisableTimeout();
    callback = NULL;
    TC0_TimerStart();
}

void APPLICATION_TIMER_RegisterCallback(TIMER_CALLBACK_FUNC_T func) {
    callback = func;
}


void APPLICATION_TIMER_EnableTimeout(uint32_t counter) {
    is_timeout = false;
    timeout_counter = counter;
    enable_timeout = true;
}

void APPLICATION_TIMER_DisableTimeout(void) {
    enable_timeout = false;
    is_timeout = false;
    timeout_counter = 0;
}

bool APPLICATION_TIMER_IsTimeout(void) {
    return is_timeout;
}

/* *****************************************************************************
 End of File
 */
