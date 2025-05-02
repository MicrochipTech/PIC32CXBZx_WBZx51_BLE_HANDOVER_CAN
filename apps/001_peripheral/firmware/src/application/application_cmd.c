/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "application_cmd.h"
#include "application_can.h"
#include "application_led.h"
#include "definitions.h"

#include "ble_handover/ble_handover.h"

#include "application_config.h"
#include "application_common.h"

#include <stdio.h>

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#define TOGGLE(n) { \
    for (int i=0; i< n;i++) { \
        GPIO_RB11_Toggle(); \
        for (int i=0; i<100;i++) { \
            __NOP(); \
        }\
        GPIO_RB11_Toggle();\
    } \
}



struct FUNC_PAIR {
    uint16_t id;
    void *func;
}; 
typedef struct FUNC_PAIR FUNC_PAIR_T;

typedef void (*HCI_PACKET_FUNC_T)(HCI_PACKET_T *pPacket);
typedef void (*HCI_CMD_PACKET_FUNC_T)(HCI_COMMAND_PACKET_T *pCmd);
typedef void (*HCI_EVT_PACKET_FUNC_T)(HCI_EVENT_PACKET_T *pEvt);

/**
BLE Handover data structure
 */
struct BLE_HANDOVER_ROLE_SWITCH_STRUCT {
    uint16_t active_can_address;    /**< Active CAN address */
}PACKED;
/** 
BLE Handover data structure (see ::BLE_HANDOVER_ROLE_SWITCH_STRUCT)
 */
typedef struct BLE_HANDOVER_ROLE_SWITCH_STRUCT BLE_HANDOVER_ROLE_SWITCH_STRUCT_T;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

static void _APPLICATION_CMD_HciReset(HCI_COMMAND_PACKET_T *pCmd);
static void _APPLICATION_CMD_HciSpReadRssi(HCI_COMMAND_PACKET_T *pCmd);
static void _APPLICATION_CMD_HciLeCtrlSetAdvertisingEnable(HCI_COMMAND_PACKET_T *pCmd);
static void _APPLICATION_CMD_HciVendorSpecificHandover1(HCI_COMMAND_PACKET_T *pCmd);
static void _APPLICATION_CMD_HciVendorSpecificHandover2(HCI_COMMAND_PACKET_T *pCmd);
static void _APPLICATION_CMD_HciVendorSpecificHandover3(HCI_COMMAND_PACKET_T *pCmd);

const FUNC_PAIR_T _hci_cmd_packets[] = {
    /* Link Control commands. (0x01) */
    // { HCI_OPCODE(HCI_LC,0), (void *)NULL },

    /* Link Policy commands. (0x02)*/
    // { HCI_OPCODE(HCI_LP,0), (void *)NULL },

    /* Host Controller and Baseband commands. (0x03)*/
    { HCI_OPCODE(HCI_HC,HCI_RESET), (void *)_APPLICATION_CMD_HciReset },

    /* Informational Parameters commands. (0x04) */
    // { HCI_OPCODE(HCI_IP,0), (void *)NULL },

    /* Status Parameters commands. (0x05)*/
    { HCI_OPCODE(HCI_SP,HCI_READ_RSSI), (void *)_APPLICATION_CMD_HciSpReadRssi },

    /* Testing Commands. (0x06)*/
    // { HCI_OPCODE(HCI_TC,0), (void *)NULL },

    /* LE Controller commands. (0x08)*/
    { HCI_OPCODE(HCI_LE_CTRL,HCI_LE_SET_ADVERTISING_ENABLE), (void *)_APPLICATION_CMD_HciLeCtrlSetAdvertisingEnable },

    /* Vendor-Specific debug commands. (0x3F)*/
    { HCI_OPCODE(MANUFACTURER_SPEC,HCI_VENDOR_SPECIFIC_HANDOVER_1), (void *)_APPLICATION_CMD_HciVendorSpecificHandover1 },
    { HCI_OPCODE(MANUFACTURER_SPEC,HCI_VENDOR_SPECIFIC_HANDOVER_2), (void *)_APPLICATION_CMD_HciVendorSpecificHandover2 },
    { HCI_OPCODE(MANUFACTURER_SPEC,HCI_VENDOR_SPECIFIC_HANDOVER_3), (void *)_APPLICATION_CMD_HciVendorSpecificHandover3 },
};
#define NUM_HCI_CMD_PACKET_FUNCS (sizeof(_hci_cmd_packets)/sizeof(FUNC_PAIR_T))


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void APPLICATION_CMD_Initialize(void) {
}

/* Host Controller and Baseband commands */
static void * _APPLICATION_CMD_HciGetFunction(uint16_t id, const FUNC_PAIR_T *pFuncTable,size_t size) {
    for (int i=0; i<size; i++) {
        if (id == pFuncTable[i].id) {
            return pFuncTable[i].func;
        }
    }
    return NULL;
}

void _APPLICATION_CMD_HciReset(HCI_COMMAND_PACKET_T *pCmd){
    APPLICATION_COMMON_Reset();
}

void _APPLICATION_CMD_HciSpReadRssi(HCI_COMMAND_PACKET_T *pCmd){
    // SYS_CONSOLE_PRINT("\r\n_APPLICATION_CMD_HciSpReadRssi\r\n");
    // TODO: ADD IMPLEMENTATION
    
    uint16_t addr = APPLICATION_CONFIG_GetCanAddress();
    HCI_EVT_READ_RSSI_COMMAND_COMPLETE_PARAMS_T param = {
        .status = STATUS_SUCCESS,
        .handle = 0xABCD,
        .rssi = 0x55,
    };
    APPLICATION_CMD_HciReadRssiCommandComplete(addr, &param);
}

void _APPLICATION_CMD_HciLeCtrlSetAdvertisingEnable(HCI_COMMAND_PACKET_T *pCmd){
    // SYS_CONSOLE_PRINT("\r\n_APPLICATION_CMD_HciLeCtrlSetAdvertisingEnable\r\n");

    uint16_t addr = APPLICATION_CONFIG_GetCanAddress();
    HCI_EVT_COMMAND_COMPLETE_T param = {
        .ocf = HCI_LE_SET_ADVERTISING_ENABLE,
        .ogf = HCI_LE_CTRL,
        .hci_set_advertising_enable.status = 0x00,
    };
    
    APPLICATION_CMD_HciEventCommandComplete(addr, &param, sizeof(HCI_EVT_SET_ADVERTISING_ENABLE_COMMAND_COMPLETE_PARAMS_T) );
    
}

void _APPLICATION_CMD_HciVendorSpecificHandover1(HCI_COMMAND_PACKET_T *pCmd){
    // SYS_CONSOLE_PRINT("\r\n_APPLICATION_CMD_HciVendorSpecificHandover1\r\n");
    TOGGLE(5);
    BLE_HANDOVER_CmdHandover1PcktRcvd(pCmd);
    TOGGLE(5);
}

void _APPLICATION_CMD_HciVendorSpecificHandover2(HCI_COMMAND_PACKET_T *pCmd){
    // SYS_CONSOLE_PRINT("\r\n_APPLICATION_CMD_HciVendorSpecificHandover2\r\n");
    BLE_HANDOVER_CmdHandover2PcktRcvd(pCmd);
    // --DBG-- TOGGLE(10);
    
    // GPIO_RB11_Clear();
    // data.handover_role = PASSIVE_NODE;
    // BLE_HANDOVER_SetRole(PASSIVE_NODE);    
}

void _APPLICATION_CMD_HciVendorSpecificHandover3(HCI_COMMAND_PACKET_T *pCmd){
    BLE_HANDOVER_ROLE_SWITCH_STRUCT_T *p_data = (BLE_HANDOVER_ROLE_SWITCH_STRUCT_T *)pCmd->parameter;
    uint16_t addr = APPLICATION_CONFIG_GetCanAddress();
    
    if (addr == p_data->active_can_address) { 
        // --DBG-- GPIO_RB11_Set();
        data.handover_role = ACTIVE_NODE;
        BLE_HANDOVER_SetRole(ACTIVE_NODE);
        // --M43814-- SYS_CONSOLE_PRINT("\r\nACTIVE\r\n");
    } else {
        // --DBG-- GPIO_RB11_Clear();
        data.handover_role = PASSIVE_NODE;
        BLE_HANDOVER_SetRole(PASSIVE_NODE);
        // --M43814-- SYS_CONSOLE_PRINT("\r\nPASSIVE\r\n");
    }
    BLE_HANDOVER_CmdHandover3PcktRcvd(pCmd);
    // --DBG-- TOGGLE(15);
}

void _APPLICATION_CMD_HciCommandPacketRcvd(HCI_PACKET_T *pPacket) {
    HCI_COMMAND_PACKET_T *pCmd = &pPacket->command_packet;
    
    // --M43814-- SYS_CONSOLE_PRINT("\r\nOGF: %X OCF: %X\r\n", pCmd->ogf, pCmd->ocf);
    
    HCI_CMD_PACKET_FUNC_T func = _APPLICATION_CMD_HciGetFunction(
            pCmd->op_code, _hci_cmd_packets, NUM_HCI_CMD_PACKET_FUNCS);
    if (func != NULL) {
        func(pCmd);
    }
}

static void _APPLICATION_CMD_HCI_LeConnectComplete(HCI_EVENT_PACKET_T *pEvt) {
    // SYS_CONSOLE_PRINT("\r\n_APPLICATION_CMD_HCI_LeConnectComplete\r\n");
    // TODO: Add implementation

}

static const FUNC_PAIR_T _hci_le_meta_evt_packets[] = {
    { HCI_LE_CONNECT_COMPLETE, _APPLICATION_CMD_HCI_LeConnectComplete },
};
#define NUM_HCI_LE_META_EVT_PACKET_FUNCS ( sizeof(_hci_le_meta_evt_packets)/sizeof(FUNC_PAIR_T) )


void _APPLICATION_CMD_HCI_LE_META_Evt(HCI_EVENT_PACKET_T *pEvt) {
    uint8_t subevent_code = pEvt->event_parameter[0];
    HCI_EVT_PACKET_FUNC_T func = _APPLICATION_CMD_HciGetFunction(
            subevent_code, _hci_le_meta_evt_packets, NUM_HCI_LE_META_EVT_PACKET_FUNCS);
    if (func != NULL) {
        func(pEvt);
    } else {
        SYS_CONSOLE_PRINT("\r\nNO LE Meta Event subevent_code: %02X\r\n", subevent_code);
    }
}

static const FUNC_PAIR_T _hci_event_packets[] = {
    {HCI_LE_META_EVENT, (void *)_APPLICATION_CMD_HCI_LE_META_Evt},
};
#define NUM_HCI_EVT_PACKET_FUNCS ( sizeof(_hci_event_packets)/sizeof(FUNC_PAIR_T) )

void _APPLICATION_CMD_HciEventPacketRcvd(HCI_PACKET_T *pPacket){
    HCI_EVENT_PACKET_T *pEvt = &pPacket->event_packet;
    uint8_t event_code = pEvt->event_code;
    HCI_EVT_PACKET_FUNC_T func = _APPLICATION_CMD_HciGetFunction(
            event_code, _hci_event_packets, NUM_HCI_EVT_PACKET_FUNCS);
    if (func != NULL) {
        func(pEvt);
    } else {
        SYS_CONSOLE_PRINT("\r\nNO event_code: %02X\r\n", event_code);
    }
}

HCI_PACKET_FUNC_T hci_packet_funcs[] = {
    NULL,   // Invalid
    _APPLICATION_CMD_HciCommandPacketRcvd,  // 0x01 - HCI Command packet
    NULL, // _APPLICATION_CMD_HciAclDataPacketRcvd,  // 0x02 - HCI ACL Data packet
    NULL, // _APPLICATION_CMD_HciSynchronousDataPacketRcvd,  // 0x03 - HCI Synchronous Data packet
    _APPLICATION_CMD_HciEventPacketRcvd,    // 0x04 - HCI Event packet
    NULL, // _APPLICATION_CMD_HciIsoDataPacketRcvd,  // 0x05 - HCI ISO Data packet
};

void APPLICATION_CMD_Tasks(APP_Msg_T *pMsg) {
    APP_MsgId_T id = pMsg->msgId;
    if (id == APP_MSG_HCI_PACKET_RCVD) {
        
        HCI_PACKET_T *pPacket = (HCI_PACKET_T *)pMsg->msgData;

        HCI_PACKET_FUNC_T func = hci_packet_funcs[pPacket->packet_type];
        if (func != NULL) {
            func(pPacket);
        }
#if false
        // --M43814-- 
        for(uint8_t i = 0; i<4; i++)
        {
            SYS_CONSOLE_PRINT("_0x%X",pMsg->msgData[i]);
        }
        SYS_CONSOLE_PRINT("\r\n\n");            
#endif
        APPLICATION_USER_LED_Off();
    }
}

void APPLICATION_CMD_HciReadRssiCommandComplete(uint16_t addr, HCI_EVT_READ_RSSI_COMMAND_COMPLETE_PARAMS_T *p_param) {
    
    HCI_EVT_COMMAND_COMPLETE_T param = {
        .ocf = HCI_READ_RSSI,
        .ogf = HCI_SP,
        .hci_read_rssi = *p_param,
    };
    APPLICATION_CMD_HciEventCommandComplete(addr, &param, sizeof(HCI_EVT_READ_RSSI_COMMAND_COMPLETE_PARAMS_T));
}


void APPLICATION_CMD_HciEventConnectionComplete(uint16_t addr, HCI_EVT_CONNECTION_COMPLETE_T *p_param) {
    HCI_PACKET_T packet = {
        .packet_type = HCI_PACKET_TYPE_EVENT,
        .event_packet = { 
            .event_code = HCI_CONNECTION_COMPLETE,
            .parameter_total_length = sizeof(HCI_EVT_CONNECTION_COMPLETE_T),
            .connection_complete_event_parameters = *p_param,
        },
    };

    size_t size = 3 + packet.event_packet.parameter_total_length;
    APPLICATION_CAN_Write(addr,(uint8_t *)&packet, size);    
}

void APPLICATION_CMD_HciEventDisconnectionComplete(uint16_t addr, HCI_EVT_DISCONNECTION_COMPLETE_T *p_param){

    HCI_PACKET_T packet = {
        .packet_type = HCI_PACKET_TYPE_EVENT,
        .event_packet = { 
            .event_code = HCI_DISCONNECTION_COMPLETE,
            .parameter_total_length = sizeof(HCI_EVT_DISCONNECTION_COMPLETE_T),
            .disconnection_complete_event_parameters = *p_param,
        },
    };

    size_t size = 3 + packet.event_packet.parameter_total_length;
    APPLICATION_CAN_Write(addr,(uint8_t *)&packet, size);    
}

void APPLICATION_CMD_HciEventCommandComplete(uint16_t addr, HCI_EVT_COMMAND_COMPLETE_T *p_param, uint8_t param_len) {
    HCI_PACKET_T packet = {
        .packet_type = HCI_PACKET_TYPE_EVENT,
        .event_packet = { 
            .event_code = HCI_COMMAND_COMPLETE,
            .parameter_total_length = 3 + param_len,
            .command_complete_event_parameters = *p_param,
        },
    };

    size_t size = 3 + packet.event_packet.parameter_total_length;
    APPLICATION_CAN_Write(addr,(uint8_t *)&packet, size);    
}

/*
 * BLE Handover Commands
 */

void APPLICATION_CMD_Handover1(void) {
    // SYS_CONSOLE_PRINT("\r\n--APPLICATION_CMD_Handover1--\r\n");
    HCI_PACKET_T packet;
    // --DBG-- GPIO_RB11_Set();
    data.handover_role = ACTIVE_NODE;
    BLE_HANDOVER_SetRole(ACTIVE_NODE);
    
    size_t size = BLE_HANDOVER_CmdHandover1(&packet);
    if (size > 0) {
        APPLICATION_CAN_Write(CAN_ADDRESS_BROADCAST, (uint8_t *)&packet, size);
    }
}


void APPLICATION_CMD_Handover2(void) {
    // SYS_CONSOLE_PRINT("\r\n--APPLICATION_CMD_Handover2--\r\n");
    HCI_PACKET_T packet;
    size_t size = BLE_HANDOVER_CmdHandover2(&packet);
    if (size > 0) {
        APPLICATION_CAN_Write(CAN_ADDRESS_BROADCAST, (uint8_t *)&packet, size);
    }
}


void APPLICATION_CMD_Handover3(void) {
    // SYS_CONSOLE_PRINT("\r\n--APPLICATION_CMD_Handover3--\r\n");
    HCI_PACKET_T packet;
    size_t size = BLE_HANDOVER_CmdHandover3(&packet);
    if (size > 0) {
        APPLICATION_CAN_Write(CAN_ADDRESS_BROADCAST, (uint8_t *)&packet, size);
    }
}

/* *****************************************************************************
 End of File
 */
