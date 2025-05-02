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

/** \file application.h
  Application defines file
 */


#ifndef _APPLICATION_H    /* Guard against multiple inclusion */
#define _APPLICATION_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "../app.h"
#include "ble_handover/ble_handover.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

#define STATUS_SUCCESS  0   /**< Application status SUCCESS */

        //Packet type
    #define HCI_PACKET_TYPE_COMMAND             0x01  /**< Packet type for HCI Command packets */
    // #define HCI_PACKET_TYPE_ACL_DATA            0x02
    // #define HCI_PACKET_TYPE_SYNC_DATA           0x03
    #define HCI_PACKET_TYPE_EVENT               0x04  /**< Packet type for HCI Event packets */
    
    #define HCI_OPCODE(ogf, ocf)                         (((ogf) << 10) + (ocf))  /**< Generate OP_CODE from OGF and OCF */

    #define HCI_LC                                              (0x01U)        /**< Link Control commands. */
    // #define HCI_LP                                              (0x02U)        /**< Link Policy commands. */
    #define HCI_HC                                              (0x03U)        /**< Host Controller and Baseband commands. */
    // #define HCI_IP                                              (0x04U)        /**< Informational Parameters commands. */
    #define HCI_SP                                              (0x05U)        /**< Status Parameters commands. */
    // #define HCI_TC                                              (0x06U)        /**< Testing Commands. */
    #define HCI_LE_CTRL                                         (0x08U)        /**< LE Controller commands. */
    #define MANUFACTURER_SPEC                                   (0x3FU)        /**< Vendor-Specific debug commands. */    

    
    #define HCI_BD_ADDRESS_LEN  (0x06U)        /**< Length of Bluetooth Device address. */
    #define HCI_RESET           0x03    /**< Reset command */
    #define HCI_READ_RSSI                                       (0x05U)        /**< Read RSSI command. */
    #define HCI_LE_SET_ADVERTISING_ENABLE                       (0x0AU)        /**< LE Set Advertising Enable command. */

    #define HCI_CONNECTION_COMPLETE                             0x03        /**< Connection Complete event. */
    #define HCI_DISCONNECTION_COMPLETE                          (0x05U)        /**< Disconnection Complete event. */
    #define HCI_COMMAND_COMPLETE                                (0x0EU)        /**< Command Complete event. */
    #define HCI_LE_META_EVENT                                   (0x3EU)        /**< LE Meta event. */

    #define HCI_DISCONNECT                                      (0x06U)        /**< Disconnect command. */

    #define HCI_LE_CONNECT_COMPLETE                             (0x01U)        /**< LE Connection Complete event. */

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /**
    Enumeration for Application Error Codes
     */
    typedef enum {
        ERROR_SUCCESS,  /**< Success, no failure */
        ERROR_TIMER_CREATE_FAILED,  /**< Timer creation failed */
        ERROR_TIMER_START_FAILED, /**< Timer start failed */
        ERROR_START_ADV_FAILED, /**< Start advertising failed */
        ERROR_UNDEFINED,  /**< Undefined error */
        ERROR_NOT_IMPLEMENTED, /**< Feature not implemented */
        ERROR_OVERFLOW, /**< Overflow error */
        ERROR_CONFIG_FAILURE, /**< Load configuration failed */
        NUM_ERROR_CODES /**< Number of error codes */
    }APPLICATION_ERROR_CODE_T;
    
    /**
    Application status data structure
     */
    typedef union {
        uint8_t all;  /**< status byte */
        struct {
            bool isConnected : 1; /**< Bit 0: Connected/Unconnected flag */
            uint8_t bit_1 : 1;  /**< Bit 1: rfu */
            uint8_t bit_2 : 1;  /**< Bit 2: rfu */
            uint8_t bit_3 : 1;  /**< Bit 3: rfu */
            uint8_t bit_4 : 1;  /**< Bit 4: rfu */
            uint8_t bit_5 : 1;  /**< Bit 5: rfu */
            uint8_t bit_6 : 1;  /**< Bit 6: rfu */
            uint8_t bit_7 : 1;  /**< Bit 7: rfu */
        };
    } APPLICATION_STATUS_T;
    /**
    Application data structure
     */
    typedef struct {
        APPLICATION_STATUS_T status;  /**< Application status */
        HANDOVER_ROLE_T handover_role;  /**< Handover Role (Passive/Active) */
        volatile int8_t prev_rssi;  /**< previous RSSI value */
    } APPLICATION_DATA_T;
    /**
    Application data (see ::APPLICATION_DATA_T)
     */
    extern APPLICATION_DATA_T data;


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    
    /**
    Application Initialization
     */
    extern void APPLICATION_Initialize(void);
    /**
    Application Main Tasks
    @param pMsg point to application message to process (see ::APP_Msg_T)
     */
    extern void APPLICATION_Tasks(APP_Msg_T *pMsg);
    /**
    Application Error Handler
    @param errorCode  Error code (see ::APPLICATION_ERROR_CODE_T)
     */
    extern void APPLICATION_ErrorHandler(APPLICATION_ERROR_CODE_T errorCode);



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_H */

/* *****************************************************************************
 End of File
 */
