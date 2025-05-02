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
/** \file application_LED.h
  LED defines file
 */

#ifndef _APPLICATION_LED_H    /* Guard against multiple inclusion */
#define _APPLICATION_LED_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "application.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */



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
    LED Initialization
     */
    extern void APPLICATION_LED_Initialize(void);
    /**
    Switch RGB LED Off
     */
    extern void APPLICATION_RGB_LED_Off(void);
    /**
    Switch USER LED On
     */
    extern void APPLICATION_USER_LED_On(void);
    /** 
    Switch USER LED Off
     */
    extern void APPLICATION_USER_LED_Off(void);
    /**
    Toggle USER LED
     */
    extern void APPLICATION_USER_LED_Toggle(void);
    /**
    Switch RED RGB LED On
     */
    extern void APPLICATION_RGB_LED_RED_On(void);
    /**
    Switch RED RGB LED Off
     */
    extern void APPLICATION_RGB_LED_RED_Off(void);
    /**
    Toggle RED RGB LED
     */
    extern void APPLICATION_RGB_LED_RED_Toggle(void);
    /**
    Switch GREEN RGB LED On
     */
    extern void APPLICATION_RGB_LED_GREEN_On(void);
    /**
    Switch GREEN RGB LED Off
     */
    extern void APPLICATION_RGB_LED_GREEN_Off(void);
    /**
    Toggle GREEN RGB LED
     */
    extern void APPLICATION_RGB_LED_GREEN_Toggle(void);
    /**
    Switch BLUE RGB LED On
     */
    extern void APPLICATION_RGB_LED_BLUE_On(void);
    /**
    Switch BLUE RGB LED Off
     */
    extern void APPLICATION_RGB_LED_BLUE_Off(void);
    /**
    Toggle BLUE RGB LED
     */
    extern void APPLICATION_RGB_LED_BLUE_Toggle(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_LED_H */

/* *****************************************************************************
 End of File
 */
