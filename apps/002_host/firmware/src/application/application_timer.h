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

/** \file application_timer.h
    Timer include file
*/


#ifndef _APPLICATION_TIMER_H    /* Guard against multiple inclusion */
#define _APPLICATION_TIMER_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
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



    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************
    /**
    Timer callback function definition
     */
    typedef void (*TIMER_CALLBACK_FUNC_T)(uint32_t counter);


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    /**
    Timer Initializeation
     */
    extern void APPLICATION_TIMER_Initialize(void);
    extern void APPLICATION_TIMER_RegisterCallback(TIMER_CALLBACK_FUNC_T func);
    extern void APPLICATION_TIMER_EnableTimeout(uint32_t counter);
    extern void APPLICATION_TIMER_DisableTimeout(void);
    extern bool APPLICATION_TIMER_IsTimeout(void);

    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_TIMER_H */

/* *****************************************************************************
 End of File
 */
