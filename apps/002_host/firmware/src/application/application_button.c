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

#include "application_button.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
static struct {
    BUTTON_CALLBACK_FUNC_T func_button_1_pressed;
    BUTTON_CALLBACK_FUNC_T func_button_1_released;
}_data;


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
static void _APPLICATION_USER_BUTTON_1_Callback(uintptr_t context) {
    if ( USER_BUTTON_1_Get() == 0 ) {
        if (_data.func_button_1_released) {
            _data.func_button_1_released();
        }
    } else {
        if (_data.func_button_1_pressed) {
            _data.func_button_1_pressed();
        }
    }
}



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void APPLICATION_BUTTON_Initialize(void) {
    _data.func_button_1_pressed = NULL;
    _data.func_button_1_released = NULL;
    
    EIC_CallbackRegister(EIC_PIN_15, _APPLICATION_USER_BUTTON_1_Callback, 0);
}

void APPLICATION_BUTTON_RegisterCallback(BUTTON_EVENT_T evt, BUTTON_CALLBACK_FUNC_T func) {
    if (evt == USR_BTN_1_PRESSED) {
        _data.func_button_1_pressed = func;
    } else if (evt == USR_BTN_1_RELEASED) {
        _data.func_button_1_released = func;
    } else {
        // Invalid Event
    }
}


/* *****************************************************************************
 End of File
 */
