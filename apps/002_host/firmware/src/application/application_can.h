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

/** \file application_can.h
    CAN include file
*/


#ifndef _APPLICATION_CAN_H    /* Guard against multiple inclusion */
#define _APPLICATION_CAN_H


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
    /**
    CAN events
    */
    typedef enum {
        CAN_DATA_ON_RX_FIFO_0,  /**< Data in RX FIFO 0 available event */
        CAN_DATA_ON_RX_FIFO_1,  /**< Data in RX FIFO 1 available event */
        CAN_TX_COMPLETE,    /**< CAN transmission complete event */
    } CAN_EVENT_T;
    /**
    CAN callback function definition
     */
    typedef void (*CAN_CALLBACK_FUNC_T)(void);



    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    /**
    CAN Initialization
     */
    extern void APPLICATION_CAN_Initialize(void);
    /**
    Register CAN Callback Function
    @param evt CAN event (see ::CAN_EVENT_T)
    @param func CAN callback function (see ::CAN_CALLBACK_FUNC_T)
     */
    extern void APPLICATION_CAN_RegisterCallback(CAN_EVENT_T evt, CAN_CALLBACK_FUNC_T func);
    
    /**
    CAN Main Tasks
     */
    extern void APPLICATION_CAN_Tasks(void);
    
    /**
    Send Data via CAN
    @param id CAN address
    @param length number of bytes to send via CAN
    @param p_data pointer to data to send via CAN
     */
    extern void APPLICATION_CAN_Send(uint16_t id, uint16_t length, uint8_t *p_data);
    
    /**
    Get CAN address/ID of received data
    @param fifoId ID of can rx fifo
    @return can address 
     */
    extern uint16_t APPLICATION_CAN_GetId(uint8_t fifoId);
    /**
    Get data length of received data
    @param fifoId ID of can rx fifo
    @return data length
     */
    extern uint8_t APPLICATION_CAN_GetDataLength(uint8_t fifoId);
    /**
    Get data pointer to received data
    @param fifoId ID of can rx fifo
    @return pointer to received data
     */
    extern uint8_t *APPLICATION_CAN_GetData(uint8_t fifoId);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_CAN_H */

/* *****************************************************************************
 End of File
 */
