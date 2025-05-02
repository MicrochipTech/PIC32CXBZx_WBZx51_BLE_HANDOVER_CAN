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
/** \file application_state.h
  STATE defines file
 */

#ifndef _APPLICATION_STATE_H    /* Guard against multiple inclusion */
#define _APPLICATION_STATE_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "app.h"
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
    Enumeration for application state machine states
     */
    typedef enum {  
        STATE_INIT, /**< Init State */
        STATE_IDLE, /**< Idle State */
        STATE_ADVERTISING,  /**< Advertising State */
        STATE_ACTIVE_CONNECTED, /**< Active Connected State */
        STATE_PASSIVE_CONNECTED,  /**< Passive Connected State */
        STATE_DISCONNECTED, /**< Disconnected State */
        STATE_ERROR,  /**< Error State */
        STATE_DEBUG,  /**< Debug State */
        NUM_APPLICATION_STATES  /**< Number of Application States */
    } APPLICATION_STATES_T;



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
    STATE Initialization
     */
    extern void APPLICATION_STATE_Initialize(void);
    /**
    STATE Main Task processing
    @param pMsg pointer to application message to process
     */
    extern void APPLICATION_STATE_Tasks(APP_Msg_T *pMsg);
    /**
    Returns current state
    @return current state (see ::APPLICATION_STATES_T)
     */
    extern APPLICATION_STATES_T APPLICATION_STATE_GetState(void);
    /**
    Switch Application to Error State
     */
    extern void APPLICATION_STATE_SetInErrorState(void);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_STATE_H */

/* *****************************************************************************
 End of File
 */
