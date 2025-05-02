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
#include "definitions.h"
#include "ble_handover/ble_handover.h"



/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
static void _APPLICATION_BUTTON_ExtDoorHandleCallback(uintptr_t context) {
    bool state = (bool)((GPIOA_REGS->GPIO_PORT >> 7) & 0x01);
    if (state == false) {
        SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "DOOR HANDLE BUTTON PRESSED\r\n");
#if false
        EXT_RELAY_Clear();
#else
        if (data.status.isConnected == true) {
            if (BLE_HANDOVER_GetRole() == ACTIVE_NODE) {
                EXT_RELAY_Clear();
            } else {
                EXT_RELAY_Set();
            }
        } else {
            EXT_RELAY_Set();
        }
#endif        
    } else {
        SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "DOOR HANDLE BUTTON RELEASED\r\n");
        EXT_RELAY_Set();        
    }
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void APPLICATION_BUTTON_Initialize(void) {
    // --M43814--TBD-- User Button only available on Extension Boards 
    EIC_CallbackRegister(EIC_PIN_0, _APPLICATION_BUTTON_ExtDoorHandleCallback, 0);
}


/* *****************************************************************************
 End of File
 */
