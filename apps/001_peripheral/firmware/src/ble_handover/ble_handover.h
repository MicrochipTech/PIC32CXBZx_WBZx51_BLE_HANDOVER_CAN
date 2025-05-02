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

#ifndef _BLE_HANDOVER_H    /* Guard against multiple inclusion */
#define _BLE_HANDOVER_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "xc.h"
#include "bt_sys.h"
#include "ble_handover_hci.h"
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
    typedef enum {
        ACTIVE_NODE = 0,
        PASSIVE_NODE
    }HANDOVER_ROLE_T;
    
    typedef void (* RSSI_CALLBACK_FUNC_T)(uint16_t conn_handle, int rssi);
    
    typedef void (* GPIO_CALLBACK_FUNC_T)(void);
    
    typedef void (* TRACE_CALLBACK_FUNC_T)(uint8_t* pWrBuffer, const size_t size );
        
    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    extern HANDOVER_ROLE_T handover_role;
    extern uint8_t csa_type;
    
    extern void BLE_HANDOVER_Initialize(void);
    extern void BLE_HANDOVER_RegisterRssiCallback(RSSI_CALLBACK_FUNC_T func);
    
    extern void BLE_HANDOVER_SetRole(HANDOVER_ROLE_T role);
    extern HANDOVER_ROLE_T BLE_HANDOVER_GetRole(void);
    
    extern int BLE_HANDOVER_CmdHandover1(HCI_PACKET_T *p_packet);
    extern void BLE_HANDOVER_CmdHandover1PcktRcvd(HCI_COMMAND_PACKET_T *p_packet);
    
    extern int BLE_HANDOVER_CmdHandover2(HCI_PACKET_T *p_packet);
    extern void BLE_HANDOVER_CmdHandover2PcktRcvd(HCI_COMMAND_PACKET_T *p_packet);
    
    extern int BLE_HANDOVER_CmdHandover3(HCI_PACKET_T *p_packet);
    extern void BLE_HANDOVER_CmdHandover3PcktRcvd(HCI_COMMAND_PACKET_T *p_packet);
    
    extern int8_t BLE_HANDOVER_GetPower(void);
    extern uint8_t BLE_HANDOVER_GetChannel(void);
    
    extern void BLE_HANDOVER_RegisterGpioSetCallback(GPIO_CALLBACK_FUNC_T func);
    extern void BLE_HANDOVER_RegisterGpioClearCallback(GPIO_CALLBACK_FUNC_T func);
    extern void BLE_HANDOVER_RegisterGpioToggleCallback(GPIO_CALLBACK_FUNC_T func);

    extern void BLE_HANDOVER_RegisterGpio2SetCallback(GPIO_CALLBACK_FUNC_T func);
    extern void BLE_HANDOVER_RegisterGpio2ClearCallback(GPIO_CALLBACK_FUNC_T func);
    extern void BLE_HANDOVER_RegisterGpio2ToggleCallback(GPIO_CALLBACK_FUNC_T func);
    extern void BLE_HANDOVER_RegisterTraceCallback(TRACE_CALLBACK_FUNC_T func);
    
    extern void BLE_HANDOVER_Trace( const char *p_ptr, ... );

    extern GPIO_CALLBACK_FUNC_T gpio_toggle_func;
    extern GPIO_CALLBACK_FUNC_T gpio2_toggle_func;
#define TOGGLE_GPIO(n) { \
    for (int i=0; i< n;i++) { \
        if (gpio_toggle_func) {gpio_toggle_func();} \
        for (int i=0; i<100;i++) { \
            __NOP(); \
        }\
        if (gpio_toggle_func) {gpio_toggle_func();}\
    } \
}
#define TOGGLE_GPIO2(n) { \
    for (int i=0; i< n;i++) { \
        if (gpio2_toggle_func) {gpio2_toggle_func();} \
        for (int i=0; i<100;i++) { \
            __NOP(); \
        }\
        if (gpio2_toggle_func) {gpio2_toggle_func();}\
    } \
}
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _BLE_HANDOVER_H */

/* *****************************************************************************
 End of File
 */
