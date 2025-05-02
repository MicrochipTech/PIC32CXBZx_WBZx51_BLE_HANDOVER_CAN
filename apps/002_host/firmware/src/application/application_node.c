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

#include "application_node.h"
#include "application_can.h"
#include "application_handover.h"
#include <stdio.h>
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#define APPLICATION_NODE_PRINT(fmt, ...) SYS_CONSOLE_Print(SYS_CONSOLE_DEFAULT_INSTANCE, fmt, ##__VA_ARGS__)
// #define APPLICATION_NODE_PRINT(fmt, ...) 

typedef void(*HCI_PACKET_FUNC_T)(NODE_T *p_node, uintptr_t context);

static void _APPLICATION_NODE_HciCommandPacket(NODE_T *p_node, uintptr_t context);
static void _APPLICATION_NODE_HciEvtPacket(NODE_T *p_node, uintptr_t context);

static FUNC_PAIR_T _packet_funcs[] = {
    {HCI_COMMAND_PACKET_INDICATOR, _APPLICATION_NODE_HciCommandPacket},
    {HCI_ACL_DATA_PACKET_INDICATOR, NULL},
    {HCI_SYNCHRONOUS_DATA_PACKET_INDICATOR, NULL},
    {HCI_EVENT_PACKET_INDICATOR, _APPLICATION_NODE_HciEvtPacket},
    {HCI_ISO_DATA_PACKET_INDICATOR, NULL},
};
#define NUM_PACKET_FUNCS    ( sizeof(_packet_funcs) / sizeof(FUNC_PAIR_T) )


static void _APPLICATION_NODE_HciCommandHandover1(NODE_T *p_node, uintptr_t context);
static void _APPLICATION_NODE_HciCommandHandover2(NODE_T *p_node, uintptr_t context);
static void _APPLICATION_NODE_HciCommandHandover3(NODE_T *p_node, uintptr_t context);
static FUNC_PAIR_T _command_funcs[] = {
    { HCI_OPCODE(OGF_VENDOR_SPECIFIC_COMMANDS, OCF_VENDOR_SPECIFIC_HANDOVER_1), _APPLICATION_NODE_HciCommandHandover1 },
    { HCI_OPCODE(OGF_VENDOR_SPECIFIC_COMMANDS, OCF_VENDOR_SPECIFIC_HANDOVER_2), _APPLICATION_NODE_HciCommandHandover2 },
    { HCI_OPCODE(OGF_VENDOR_SPECIFIC_COMMANDS, OCF_VENDOR_SPECIFIC_HANDOVER_3), _APPLICATION_NODE_HciCommandHandover3 },
};
#define NUM_COMMAND_FUNCS (sizeof(_command_funcs) / sizeof(FUNC_PAIR_T))

static void _APPLICATION_NODE_HciConnectionComplete(NODE_T *p_node, uintptr_t context);
static void _APPLICATION_NODE_HciDisconnectionComplete(NODE_T *p_node, uintptr_t context);
static void _APPLICATION_NODE_HciCommandComplete(NODE_T *p_node, uintptr_t context);
static void _APPLICATION_NODE_HciLeMeta(NODE_T *p_node, uintptr_t context);
static FUNC_PAIR_T _event_funcs[] = {
    {HCI_CONNECTION_COMPLETE, _APPLICATION_NODE_HciConnectionComplete},
    {HCI_DISCONNECTION_COMPLETE, _APPLICATION_NODE_HciDisconnectionComplete},
    {HCI_COMMAND_COMPLETE, _APPLICATION_NODE_HciCommandComplete},
    {HCI_LE_META, _APPLICATION_NODE_HciLeMeta}
};
#define NUM_EVENT_FUNCS sizeof(_event_funcs)/sizeof(FUNC_PAIR_T)


static void _APPLICATION_NODE_HciCommandCompleteReset(NODE_T *p_node, uintptr_t context);
static void _APPLICATION_NODE_HciCommandCompleteReadRssi(NODE_T *p_node, uintptr_t context);
static void _APPLICATION_NODE_HciCommandCompleteLeSetAdvertisingEnable(NODE_T *p_node, uintptr_t context);
static FUNC_PAIR_T _command_complete_event_funcs[] = {
    {HCI_OPCODE(OGF_HOST_CONTROLLER_AND_BASEBAND_COMMANDS, OCF_HCI_RESET),_APPLICATION_NODE_HciCommandCompleteReset},
    {HCI_OPCODE(OGF_STATUS_PARAMETERS, OCF_HCI_READ_RSSI),_APPLICATION_NODE_HciCommandCompleteReadRssi},
    {HCI_OPCODE(OGF_STATUS_PARAMETERS, OCF_LE_SET_ADVERTISING_ENABLE),_APPLICATION_NODE_HciCommandCompleteLeSetAdvertisingEnable},
};
#define NUM_COMMAND_COMPLETE_EVENT_FUNCS sizeof(_command_complete_event_funcs)/sizeof(FUNC_PAIR_T)

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*
 * HCI Command Reception
 */
void _APPLICATION_NODE_HciCommandPacket(NODE_T *p_node, uintptr_t context){
    HCI_PACKET_T *p_pckt = (HCI_PACKET_T *)context;
    HCI_COMMAND_PACKET_T *p_cmd_pckt = (HCI_COMMAND_PACKET_T *)&p_pckt->command_packet;
    HCI_PACKET_FUNC_T func = APPLICATION_GetFunction(p_cmd_pckt->op_code, _command_funcs, NUM_COMMAND_FUNCS);
    if (func) {
        func(p_node,(uintptr_t)p_cmd_pckt);
    } else {
        // Invalid/Unsupported Opcode
        APPLICATION_NODE_PRINT("ERROR [%s] _APPLICATION_NODE_HciCommandPacket - Invalid/Unsupported OP Code %03X\r\n", p_node->name, p_cmd_pckt->op_code);
        APPLICATION_ErrorHandler(ERROR_INVALID_ID);
    }
}

void _APPLICATION_NODE_HciCommandHandover1(NODE_T *p_node, uintptr_t context) {
    GPIO_PA06_Set();
    APPLICATION_NODE_PRINT("[%s] _APPLICATION_NODE_HciCommandHandover1\r\n", p_node->name);
    HCI_COMMAND_PACKET_T *p_cmd_pckt = (HCI_COMMAND_PACKET_T *)context;
    (void)p_cmd_pckt;   // SKIP error: unused variable .... [-Werror]=unused-variable]
    
    p_node->status.handover_in_progress = true;
    GPIO_PA06_Clear();
#if true
    char buffer[80];
    sprintf(buffer, "\t- subevent_code: %02X\r\n", p_cmd_pckt->parameter[0]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- status: %02X\r\n", p_cmd_pckt->parameter[1]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- conn_handle: %02X%02X\r\n", p_cmd_pckt->parameter[3], p_cmd_pckt->parameter[2]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- role: %02X\r\n", p_cmd_pckt->parameter[4]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- peer_addr_type: %02X\r\n", p_cmd_pckt->parameter[5]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- peer_addr: %02X %02X %02X %02X %02X %02X\r\n", 
            p_cmd_pckt->parameter[6], p_cmd_pckt->parameter[7], p_cmd_pckt->parameter[8],
            p_cmd_pckt->parameter[9], p_cmd_pckt->parameter[10], p_cmd_pckt->parameter[11]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- connection_interval: %02X%02X\r\n", p_cmd_pckt->parameter[13], p_cmd_pckt->parameter[12]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- connection_latency: %02X%02X\r\n", p_cmd_pckt->parameter[15], p_cmd_pckt->parameter[14]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- supervision_timeout: %02X%02X\r\n", p_cmd_pckt->parameter[17], p_cmd_pckt->parameter[16]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- master_clock_accuracy: %02X\r\n", p_cmd_pckt->parameter[18]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- initA: %02X %02X %02X %02X %02X %02X\r\n", 
        p_cmd_pckt->parameter[19], p_cmd_pckt->parameter[20], p_cmd_pckt->parameter[21],
        p_cmd_pckt->parameter[22], p_cmd_pckt->parameter[23], p_cmd_pckt->parameter[24]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- advA: %02X %02X %02X %02X %02X %02X\r\n", 
        p_cmd_pckt->parameter[25], p_cmd_pckt->parameter[26], p_cmd_pckt->parameter[27],
        p_cmd_pckt->parameter[28], p_cmd_pckt->parameter[29], p_cmd_pckt->parameter[30]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- aa: %02X %02X %02X %02X\r\n", 
        p_cmd_pckt->parameter[31], p_cmd_pckt->parameter[32], p_cmd_pckt->parameter[33], p_cmd_pckt->parameter[34] );
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- crcInit: %02X %02X %02X\r\n", 
        p_cmd_pckt->parameter[35], p_cmd_pckt->parameter[36], p_cmd_pckt->parameter[37] );
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- winSize: %02X\r\n", p_cmd_pckt->parameter[38]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- winOffset: %02X%02X\r\n", p_cmd_pckt->parameter[40], p_cmd_pckt->parameter[39]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- interval: %02X%02X\r\n", p_cmd_pckt->parameter[42], p_cmd_pckt->parameter[41]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- latency: %02X%02X\r\n", p_cmd_pckt->parameter[44], p_cmd_pckt->parameter[43]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- timeout: %02X%02X\r\n", p_cmd_pckt->parameter[46], p_cmd_pckt->parameter[45]);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- chM: %02X %02X %02X %02X %02X\r\n", 
        p_cmd_pckt->parameter[47], p_cmd_pckt->parameter[48], p_cmd_pckt->parameter[49], p_cmd_pckt->parameter[50], p_cmd_pckt->parameter[51] );
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- hop: %02X\r\n", p_cmd_pckt->parameter[52] & 0x1F);
    SYS_CONSOLE_PRINT(buffer);
    sprintf(buffer, "\t- sca: %02X\r\n", (p_cmd_pckt->parameter[52] >> 5) & 0x03);    
    SYS_CONSOLE_PRINT(buffer);
#endif
}

void _APPLICATION_NODE_HciCommandHandover2(NODE_T *p_node, uintptr_t context) {
    APPLICATION_NODE_PRINT("[%s] _APPLICATION_NODE_HciCommandHandover2\r\n", p_node->name);
    HCI_COMMAND_PACKET_T *p_cmd_pckt = (HCI_COMMAND_PACKET_T *)context;
    (void)p_cmd_pckt;   // SKIP error: unused variable .... [-Werror]=unused-variable]
}

void _APPLICATION_NODE_HciCommandHandover3(NODE_T *p_node, uintptr_t context) {
    APPLICATION_NODE_PRINT("[%s] _APPLICATION_NODE_HciCommandHandover3\r\n", p_node->name);
    HCI_COMMAND_PACKET_T *p_cmd_pckt = (HCI_COMMAND_PACKET_T *)context;
    (void)p_cmd_pckt;   // SKIP error: unused variable .... [-Werror]=unused-variable]
}



/*
 * HCI Event Reception
 */
void _APPLICATION_NODE_HciEvtPacket(NODE_T *p_node, uintptr_t context){
    HCI_PACKET_T *p_pckt = (HCI_PACKET_T *)context;
    HCI_EVENT_PACKET_T *p_evt_pckt = (HCI_EVENT_PACKET_T *)&p_pckt->event_packet;
    HCI_PACKET_FUNC_T func = APPLICATION_GetFunction(p_evt_pckt->event_code, _event_funcs, NUM_EVENT_FUNCS);
    if (func) {
        func(p_node,(uintptr_t)p_evt_pckt);
    } else {
        // Invalid/Unsupported Event Code
        APPLICATION_NODE_PRINT("ERROR [%s] _APPLICATION_NODE_HciEvtPacket - Invalid/Unsupported Event Code %02X\r\n", p_node->name, p_evt_pckt->event_code);
        APPLICATION_ErrorHandler(ERROR_INVALID_ID);
    }
}

void _APPLICATION_NODE_HciConnectionComplete(NODE_T *p_node, uintptr_t context){
    // --M43814--DBG-- APPLICATION_NODE_PRINT("[%s] _APPLICATION_NODE_HciConnectionComplete\r\n", p_node->name);
    HCI_PACKET_T *p_pckt = (HCI_PACKET_T *)context;
    HCI_EVENT_PACKET_T *p_evt_pckt = (HCI_EVENT_PACKET_T *)&p_pckt->event_packet;
    p_node->conn_handle = p_evt_pckt->connection_complete.conn_handle;
    p_node->status.connected = true;
}

void _APPLICATION_NODE_HciDisconnectionComplete(NODE_T *p_node, uintptr_t context){
    APPLICATION_NODE_PRINT("[%s] _APPLICATION_NODE_HciDisconnectionComplete\r\n", p_node->name);
    p_node->status.connected = false;
}

void _APPLICATION_NODE_HciCommandComplete(NODE_T *p_node, uintptr_t context){
    // --M43814--DBG-- APPLICATION_NODE_PRINT("[%s] _APPLICATION_NODE_HciCommandComplete\r\n", p_node->name);
    HCI_EVENT_PACKET_T *p_evt_pckt = (HCI_EVENT_PACKET_T *)context;
    uint16_t op_code = p_evt_pckt->command_complete.op_code;
    HCI_PACKET_FUNC_T func = APPLICATION_GetFunction(op_code, _command_complete_event_funcs, NUM_COMMAND_COMPLETE_EVENT_FUNCS);
    if (func) {
        func(p_node, context);
    } else {
        // Invalid/Unsupported OP Code
        APPLICATION_NODE_PRINT("ERROR [%s] _APPLICATION_NODE_HciCommandComplete - Invalid/Unsupported OP Code %03X\r\n", p_node->name, op_code);
        APPLICATION_ErrorHandler(ERROR_INVALID_ID);
    }
}

void _APPLICATION_NODE_HciCommandCompleteReset(NODE_T *p_node, uintptr_t context){
    // --M43814--DBG-- APPLICATION_NODE_PRINT("[%s] _APPLICATION_NODE_HciCommandCompleteReset\r\n", p_node->name);
    p_node->status.reset_complete = true;
}

void _APPLICATION_NODE_HciCommandCompleteReadRssi(NODE_T *p_node, uintptr_t context){
    // --M43814--DBG-- APPLICATION_NODE_PRINT("[%s] _APPLICATION_NODE_HciCommandCompleteReadRssi\r\n", p_node->name);
    HCI_EVENT_PACKET_T *p_evt_pckt = (HCI_EVENT_PACKET_T *)context;
    uint8_t status = p_evt_pckt->command_complete.hci_read_rssi_parameters.status;
    if (status == HCI_STATUS_SUCCESS) {
        p_node->rssi = p_evt_pckt->command_complete.hci_read_rssi_parameters.rssi;
    }
    
}

void _APPLICATION_NODE_HciCommandCompleteLeSetAdvertisingEnable(NODE_T *p_node, uintptr_t context){
    APPLICATION_NODE_PRINT("[%s] _APPLICATION_HciCommandCompleteLeSetAdvertisingEnable\r\n", p_node->name);
}

void _APPLICATION_NODE_HciLeMeta(NODE_T *p_node, uintptr_t context){
    SYS_CONSOLE_PRINT("[%s] _APPLICATION_NODE_HciLeMeta\r\n", p_node->name);
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void APPLICATION_NODE_Initialize(NODE_T *p_node) {
    // TODO: Add Implementation
}

void APPLICATION_NODE_Tasks(NODE_T *p_node) {
    // TODO: Add Implementation
}

void APPLICATION_NODE_Process(NODE_T *p_node, uint8_t *p_data) {
    HCI_PACKET_T *p_pckt = (HCI_PACKET_T *)p_data;
    uint8_t type = p_pckt->packet_type;
    HCI_PACKET_FUNC_T func = APPLICATION_GetFunction(type, _packet_funcs, NUM_PACKET_FUNCS);
    
    if (func) {
        func(p_node, (uintptr_t)p_data);
    } else {
        // Unsupported packet type
        APPLICATION_ErrorHandler(ERROR_INVALID_ID);
    }
}

/*
 * Link Control Commands
 */
void APPLICATION_NODE_HciDisconnect(NODE_T *p_node, uint8_t reason) {
    SYS_CONSOLE_PRINT("[%s] APPLICATION_NODE_HciDisconnect\r\n", p_node->name);
    HCI_PACKET_T packet = {
        .packet_type = HCI_COMMAND_PACKET_INDICATOR,
        .command_packet = {
            .ocf = OCF_HCI_DISCONNECT,
            .ogf = OGF_LINK_CONTROL_COMMANDS,
            .parameter_total_length = 3,
            .hci_disconnect_command_parameters = {
                .connection_handle = p_node->conn_handle,
                .reason = reason,
            },
        },
    };
    APPLICATION_CAN_Send(p_node->can_address, 7, (uint8_t *)&packet);
}
/*
 * Link Policy Commands
 */
/*
 * Controller & Baseband commands
 */
/*      Reset Command   */
void APPLICATION_NODE_HciReset(NODE_T *p_node) {
    // --M43814--DBG-- SYS_CONSOLE_PRINT("[%s] APPLICATION_NODE_HciReset\r\n", p_node->name);
    HCI_PACKET_T packet = {
        .packet_type = HCI_COMMAND_PACKET_INDICATOR,
        .command_packet = {
            .ocf = OCF_HCI_RESET,
            .ogf = OGF_HOST_CONTROLLER_AND_BASEBAND_COMMANDS,
            .parameter_total_length = 0,
        },
    };
    APPLICATION_CAN_Send(p_node->can_address, 4, (uint8_t *)&packet);
}


/*
 * Informational parameters
 */
/*
 * Status parameters
 */
/*
 * Testing commands
 */
/*
 * Events
 */
/*
 * LE Controller commands
 */
/*
 * Manufacturer specific commands
 */
void APPLICATION_NODE_HciHandover3(NODE_T *p_node, uint16_t address) {
    // --M43814--DBG-- 
    SYS_CONSOLE_PRINT("[%s] APPLICATION_NODE_HciHandover3 %04X\r\n", p_node->name, address);
    HCI_PACKET_T packet = {
        .packet_type = HCI_COMMAND_PACKET_INDICATOR,
        .command_packet = {
            .ocf = OCF_VENDOR_SPECIFIC_HANDOVER_3,
            .ogf = OGF_VENDOR_SPECIFIC_COMMANDS,
            .parameter_total_length = 2,
            .hci_vendor_specific_handover3_command_parameters.address = address,
        }
    };
    APPLICATION_CAN_Send(p_node->can_address, 6, (uint8_t *)&packet);
}


/* *****************************************************************************
 End of File
 */
