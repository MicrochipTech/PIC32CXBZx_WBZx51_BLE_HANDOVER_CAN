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
/** \file application_cmd.h
  CMD defines file
 */


#ifndef _APPLICATION_CMD_H    /* Guard against multiple inclusion */
#define _APPLICATION_CMD_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "application.h"
#include "definitions.h"
// #include "host_hci_main.h"

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
    CMD Initialization
     */
    extern void APPLICATION_CMD_Initialize(void);
    /**
    CMD Main Task
    @param pMsg pointer to application message to process
     */
    extern void APPLICATION_CMD_Tasks(APP_Msg_T *pMsg);
    /**
    Send Application command 'Handover1'
     */
    extern void APPLICATION_CMD_Handover1(void);
    /**
    Send Application command 'Handover2'
     */
    extern void APPLICATION_CMD_Handover2(void);
    /**
    Send Application command 'Handover3'
     */
    extern void APPLICATION_CMD_Handover3(void);
    
    /** 
    Process HCI Event 'Read RSSI Command Complete'
    @param addr CAN address
    @param p_param pointer to event parameters
     */
    extern void APPLICATION_CMD_HciReadRssiCommandComplete(uint16_t addr, HCI_EVT_READ_RSSI_COMMAND_COMPLETE_PARAMS_T *p_param);
    /**
    Process HCI Event 'Connection Complete'
    @param addr CAN address
    @param p_param pointer to event parameters
     */
    extern void APPLICATION_CMD_HciEventConnectionComplete(uint16_t addr, HCI_EVT_CONNECTION_COMPLETE_T *p_param);
    /**
    Process HCI Event 'Disconnection Complete'
    @param addr CAN address
    @param p_param pointer to event parameters
     */
    extern void APPLICATION_CMD_HciEventDisconnectionComplete(uint16_t addr, HCI_EVT_DISCONNECTION_COMPLETE_T *p_param);
    /**
    Process HCI Event 'Command Complete'
    @param addr CAN address
    @param p_param pointer to event parameters
    @param param_len parameter length in bytes
     */
    extern void APPLICATION_CMD_HciEventCommandComplete(uint16_t addr, HCI_EVT_COMMAND_COMPLETE_T *p_param, uint8_t param_len);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_CMD_H */

/* *****************************************************************************
 End of File
 */
