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

/** \file application_config.h
  Application Configuration defines file
 */

#ifndef _APPLICATION_CONFIG_H    /* Guard against multiple inclusion */
#define _APPLICATION_CONFIG_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "definitions.h"
#include "ble_gap.h"


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
    PDS Application data structure
     */
    typedef enum APPLICATION_PdsApplicationItem_T {
        PDS_APP_ITEM_ID_1 = (PDS_MODULE_APP_OFFSET),
    }APPLICATION_PdsApplicationItem_T;
    
    #define APPLICATION_FILE_MAIN_ITEM_START    PDS_APP_ITEM_ID_1 /**< PDS Entry */

    /**
    Enumeration of CAN addresses
     */
    enum {
        CAN_ADDRESS_BROADCAST = 0x045A, /**< CAN address used for broadcasting */
        CAN_ADDRESS_NODE1 = 0x045B, /**< CAN address for Node 1 */
        CAN_ADDRESS_NODE2 = 0x045C, /**< CAN address for Node 2 */
        CAN_ADDRESS_NODE3 = 0x045D, /**< CAN address for Node 3 */
        CAN_ADDRESS_NODE4 = 0x045E, /**< CAN address for Node 4 */
    };


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************
    /**
    Application Configuration Data Structure
     */
    typedef struct APPLICATION_Config_T {
        uint16_t can_address; /**< CAN address */
        BLE_GAP_Addr_T ble_address; /**< BLE address */
    }APPLICATION_Config_T;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    /**
    Application Configuration Initialization
     */
    extern void APPLICATION_CONFIG_Initialize(void);
    /**
    Get configured CAN address 
    @return CAN address
     */
    extern uint16_t APPLICATION_CONFIG_GetCanAddress(void);
    /**
    Get configured BLE address 
    @param p_addr pointer to store configured BLE Address (see ::BLE_GAP_Addr_T)
    @return status, always MBA_RES_SUCCESS
     */
    extern uint16_t APPLICATION_CONFIG_GetBleAddress(BLE_GAP_Addr_T *p_addr);
    /**
    Check if configuration contains valid data
    @return TRUE if configuration is valid, otherwise false
     */
    extern bool APPLICATION_CONFIG_ValidCheck(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_CONFIG_H */

/* *****************************************************************************
 End of File
 */
