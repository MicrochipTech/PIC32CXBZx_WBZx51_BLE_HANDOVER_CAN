/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/** \file application_button.h
    Button include file
*/

#ifndef _APPLICATION_BUTTON_H    /* Guard against multiple inclusion */
#define _APPLICATION_BUTTON_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "application.h"
#include "definitions.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
    /**
    Button Events
     */
    typedef enum {
        USR_BTN_1_PRESSED,  /**< USR_BTN_1 pressed event */
        USR_BTN_1_RELEASED, /**< USR_BTN_1 released event */
    } BUTTON_EVENT_T;
    /**
    Button callback function definition
     */
    typedef void (*BUTTON_CALLBACK_FUNC_T)(void);


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************



    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    /**
    Button Initialization
     */
    extern void APPLICATION_BUTTON_Initialize(void);
    /**
    Register Button Callback Function(see ::BUTTON_CALLBACK_FUNC_T)
    @param evt  button event (see ::BUTTON_EVENT_T)
    @param func button callback function (see ::BUTTON_CALLBACK_FUNC_T)
     */
    extern void APPLICATION_BUTTON_RegisterCallback(BUTTON_EVENT_T evt, BUTTON_CALLBACK_FUNC_T func);
    


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_BUTTON_H */

/* *****************************************************************************
 End of File
 */
