// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

/** \file app.h
  Application defines file
 */

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "osal/osal_freertos_extend.h"


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#define MAX_TXQUEUE_ATTEMPTS        10            /**< Number of CAN TX Attempts */
#define DRV_CANFDSPI_INDEX_0        canSPIHandle  /**< CAN SPI Handle */
    
// Transmit Channels
#define APP_TX_FIFO CAN_FIFO_CH2  /**< CAN Transmit Channel */

// Receive Channels
#define APP_RX_FIFO CAN_FIFO_CH1  /**< CAN Receive Channel */


// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

/**
  Application states enumeration
  This enumeration defines the valid application states. These states
  determine the behavior of the application at various times.
 */
typedef enum
{
    APP_STATE_INIT=0, /**< Application's state machine's initial state. */
    APP_STATE_SERVICE_TASKS,  /**< Application's state machine's main/idle state. */
    /* TODO: Define states used by the application state machine. */

} APP_STATES;

/**
  Application message ID's
 */
typedef enum APP_MsgId_T
{

    APP_MSG_BLE_STACK_EVT,  /**< BLE Stack Event Message ID */
    APP_MSG_BLE_STACK_LOG,  /**< BLE Stock Log Message ID */


    APP_MSG_ZB_STACK_EVT,   /**< Zigbee Stack Event Message ID */
    APP_MSG_ZB_STACK_CB,    /**< Zigbee Stack Callback Message ID */
            
    APP_MSG_APP_TRIGGER,    /**< Trigger Application Message ID */
    APP_MSG_SW1_PRESSED,    /**< SW1 Pressed Message ID */
    APP_MSG_SW1_RELEASED,   /**< SW1 Released Message ID */
            
    APP_MSG_HCI_PACKET_RCVD,  /**< HCI Command/Event Packet Received Message ID */

    APP_MSG_CAN_RECV_CB,  /**< CAN Received Callback Message ID */
    APP_MSG_RSSI_CHANGED, /**< RSSI Changed Message ID */
            
    APP_MSG_STACK_END /**< Number of Message ID's */
} APP_MsgId_T;

/**
Application Message Structure
 */
typedef struct APP_Msg_T
{
    uint8_t msgId;  /**< Message ID */
    uint8_t msgData[256]; /**< Message Data Buffer */
} APP_Msg_T;

/**
RSSI Message data structure
 */
struct RSSI_MSG_STRUCT {
    uint16_t conn_handle; /**< 16-bit Connection Handle */
    int8_t rssi;  /**< RSSI */
}PACKED;
/**
RSSI Message data structure (see :: RSSI_MSG_STRUCT)
 */
typedef struct RSSI_MSG_STRUCT RSSI_MSG_STRUCT_T;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

/**
Application Data Structure.
This structure holds the application's data.  
\note Application strings and buffers are be defined outside this structure
 */
typedef struct
{
    APP_STATES state; /**< The application's current state */

    /* TODO: Define any additional data used by the application. */
    OSAL_QUEUE_HANDLE_TYPE appQueue;  /**< Application Message Queue Handle */

} APP_DATA;

/** 
Application Data (see ::APP_DATA)
*/
extern APP_DATA appData;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/
/**
  MPLAB Harmony application initialization routine.
  This function initializes the Harmony application. It places the application
  in its inital state and prepares it to run so that its APP_Tasks function can
  be called.
  Precondition: All other system initialization routines should be called before
  calling this routine (in "SYS_Initialize").
  \note This routine must be called from the SYS_Initialize function.
 */
void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

/**
  MPLAB Harmony Demo application tasks function
  This routine is the Harmony Demo application's tasks function. It defines
  the application's state machine and core logic.
  Precondition: The system and application initialization ("SYS_Initialize") 
  should be called before calling this.
  \note This routine must be called from SYS_Tasks() routine.
 */
void APP_Tasks( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_H */

/*******************************************************************************
 End of File
 */

