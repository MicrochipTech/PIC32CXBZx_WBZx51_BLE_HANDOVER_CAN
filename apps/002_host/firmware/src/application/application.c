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

#include "application.h"
#include "application_can.h"
#include "application_cmd.h"
#include "application_console.h"
#include "application_dbg.h"
#include "application_node.h"
#include "application_timer.h"
#include "application_button.h"
#include "application_handover.h"

#include <stdio.h>

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#define APPLICATION_PRINT(fmt, ...) SYS_CONSOLE_Print(SYS_CONSOLE_DEFAULT_INSTANCE, fmt, ##__VA_ARGS__)
// #define APPLICATION_PRINT(fmt, ...) 

#define LED_PATTERN_SIZE    10
#define USER_LED_PATTERN_STATE_RESET_NODES                  (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define USER_LED_PATTERN_STATE_WAIT_4_RESET_COMPLETE        (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define USER_LED_PATTERN_STATE_INIT                         (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define USER_LED_PATTERN_STATE_ADVERTISING                  (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
#define USER_LED_PATTERN_STATE_ACTIVE_CONNECTED             (uint16_t)((0<<9)|(0<<8)|(0<<7)|(1<<6)|(0<<5)|(0<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
#define USER_LED_PATTERN_STATE_HANDOVER_IN_PROGRESS         (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(1<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
#define USER_LED_PATTERN_STATE_HANDOVER_ACTIVE              (uint16_t)((0<<9)|(1<<8)|(0<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
#define USER_LED_PATTERN_STATE_DISCONNECTION_IN_PROGRESS    (uint16_t)((1<<9)|(1<<8)|(0<<7)|(0<<6)|(1<<5)|(1<<4)|(0<<3)|(0<<2)|(1<<1)|(1<<0))
#define USER_LED_PATTERN_STATE_DISCONNECTED                 (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define USER_LED_PATTERN_STATE_ERROR                        (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define USER_LED_PATTERN_STATE_DEBUG                        (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
static uint16_t _led_pattern[] = {
    USER_LED_PATTERN_STATE_RESET_NODES,
    USER_LED_PATTERN_STATE_WAIT_4_RESET_COMPLETE,
    USER_LED_PATTERN_STATE_INIT,
    USER_LED_PATTERN_STATE_ADVERTISING,
    USER_LED_PATTERN_STATE_ACTIVE_CONNECTED,
    USER_LED_PATTERN_STATE_HANDOVER_IN_PROGRESS,
    USER_LED_PATTERN_STATE_HANDOVER_ACTIVE,
    USER_LED_PATTERN_STATE_DISCONNECTION_IN_PROGRESS,
    USER_LED_PATTERN_STATE_DISCONNECTED,
    USER_LED_PATTERN_STATE_ERROR,
    USER_LED_PATTERN_STATE_DEBUG,
};

static const struct {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
}_version = { .major = 0, .minor = 8, .patch = 0};


APPLICATION_DATA_T data;
const APPLICATION_DATA_T _init_data = {
    .handover_enabled = false,
};

/**
BLE Handover Demo application internal  structure
 */
typedef struct {
    uint8_t active_node;    /**< active node id */
    uint8_t state_index;    /**< BLE handover state machine index */
}APP_DATA_T;

static APP_DATA_T _data;

NODE_T nodes[] = {
    {
        .can_address = ID_NODE_1,
        .name = "NODE 1",
        .status.v = 0,
    },
    {
        .can_address = ID_NODE_2,
        .name = "NODE 2",
        .status.v = 0,
    },
    {
        .can_address = ID_NODE_3,
        .name = "NODE 3",
        .status.v = 0,
    },
    {
        .can_address = ID_NODE_4,
        .name = "NODE 4",
        .status.v = 0,
    },
};
NODE_T broadcast = {
        .can_address = ID_BROADCAST,
        .name = "BROADCAST",
        .status.v = 0,
};

#define _NUMBER_OF_NODES ( sizeof(nodes) / sizeof(NODE_T) )


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

static void _APPLICATION_UserButton1Pressed(void) {
    if ( data.handover_enabled == true) {
        data.handover_enabled = false;
        SYS_CONSOLE_PRINT("BLE Handover DISABLED\r\n");
    } else {
        data.handover_enabled = true;
        SYS_CONSOLE_PRINT("BLE Handover ENABLED\r\n");
    }    
    // TODO: ADD IMPLEMENTATION
}

static void _APPLICATION_UserButton1Released(void) {
    // TODO: ADD IMPLEMENTATION
}

static void _APPLICATION_EVENT_CanDataOnRxFifo0Received(void) {
    /* 
     * Broadcast message are received on this fifo 
     */
    uint8_t *p_data = APPLICATION_CAN_GetData(0);
    if (p_data) {
        APPLICATION_NODE_Process(&broadcast, p_data);
    }
}



static void _APPLICATION_EVENT_CanDataOnRxFifo1Received(void) {
    /*
     * Node messages are received on this fifo
     */
    uint16_t node_id = APPLICATION_CAN_GetId(1);
    uint8_t *p_data = APPLICATION_CAN_GetData(1);
    
    for (int i=0; i< NUMBER_OF_NODES; i++) {
        if (node_id == nodes[i].can_address) {
            if (p_data) {
                APPLICATION_NODE_Process(&nodes[i], p_data);
            }
            break;
        }
    }
}

/*
 * State Machine
 */
typedef void (*APPLICATION_STATE_FUNC_T)(void);

static void _APPLICATION_STATE_ResetNodes(void);
static void _APPLICATION_STATE_Wait4NodesReady(void);
static void _APPLICATION_STATE_Init(void);
static void _APPLICATION_STATE_Advertising(void);
static void _APPLICATION_STATE_ActiveConnection(void);
static void _APPLICATION_STATE_HandoverInProgress(void);
static void _APPLICATION_STATE_HandoverActive(void);
static void _APPLICATION_STATE_DisconnectionInProgress(void);
static void _APPLICATION_STATE_Disconnected(void);
static void _APPLICATION_STATE_Error(void);
static void _APPLICATION_STATE_Debug(void);

static APPLICATION_STATE_FUNC_T stateTable[] = {
    _APPLICATION_STATE_ResetNodes,
    _APPLICATION_STATE_Wait4NodesReady,
    _APPLICATION_STATE_Init,
    _APPLICATION_STATE_Advertising,
    _APPLICATION_STATE_ActiveConnection,
    _APPLICATION_STATE_HandoverInProgress,
    _APPLICATION_STATE_HandoverActive,
    _APPLICATION_STATE_DisconnectionInProgress,
    _APPLICATION_STATE_Disconnected,
    _APPLICATION_STATE_Error,
    _APPLICATION_STATE_Debug,
};

typedef enum  {
    STATE_RESET_NODES = 0,
    STATE_WAIT_4_RESET_COMPLETE,
    STATE_INIT,
    STATE_ADVERTISING,
    STATE_ACTIVE_CONNECTED,
    STATE_HANDOVER_IN_PROGRESS,
    STATE_HANDOVER_ACTIVE,
    STATE_DISCONNECTION_IN_PROGRESS,
    STATE_DISCONNECTED,
    STATE_ERROR,
    STATE_DEBUG,
    NUMBER_STATES,
} APPLICATION_STATE_INDEX_T;

void _APPLICATION_STATE_ResetNodes(void) {
    for (int i=0; i<NUMBER_OF_NODES;i++) {
        nodes[i].status.v = 0;
        nodes[i].rssi = -128;
    }
    data.handover_enabled = false;
    // SYS_CONSOLE_PRINT("BLE Handover DISABLED\r\n");
    APPLICATION_NODE_HciReset(&broadcast);
    _data.state_index = STATE_WAIT_4_RESET_COMPLETE;
}

void _APPLICATION_STATE_Wait4NodesReady(void) {
    for (int i=0; i<NUMBER_OF_NODES;i++) {
        if (nodes[i].status.reset_complete == false) {
            return;
        }
    }
    APPLICATION_PRINT("Reset Nodes Complete\r\n");
    _data.state_index = STATE_INIT;
}

void _APPLICATION_STATE_Init(void) {
    APPLICATION_PRINT("Advertising State\r\n");
    _data.state_index = STATE_ADVERTISING;
}

void _APPLICATION_STATE_Advertising(void) {
    // TODO: ADD IMPLEMENTATION    
    for (int i=0; i<NUMBER_OF_NODES;i++) {
        if (nodes[i].status.connected == true) {
            _data.active_node = i;
            _data.state_index = STATE_ACTIVE_CONNECTED;
            APPLICATION_PRINT("%s active connected\r\n",nodes[i].name);
            return;
        }
    }
}

void _APPLICATION_STATE_ActiveConnection(void){
    if (broadcast.status.handover_in_progress) {
        broadcast.status.handover_in_progress = false;
        _data.state_index = STATE_HANDOVER_IN_PROGRESS;
        APPLICATION_PRINT("Handover in Progress\r\n");

        APPLICATION_TIMER_EnableTimeout(400);

    }
}

void _APPLICATION_STATE_HandoverInProgress(void){

    if ( APPLICATION_TIMER_IsTimeout() ) {
        _data.state_index = STATE_DISCONNECTED;
        return;
    }
    // wait until all nodes are connected
    for (int i=0; i<NUMBER_OF_NODES; i++){
        if (nodes[i].status.connected == false) {
            return;
        }
    }
    
    // wait until all nodes provides at least one RSSI value
    for (int i=0; i<NUMBER_OF_NODES; i++){
        if (nodes[i].rssi == -128) {
            return;
        }
    }
    APPLICATION_TIMER_DisableTimeout();
    data.handover_enabled = true;
    _data.state_index = STATE_HANDOVER_ACTIVE;
    APPLICATION_PRINT("Handover Active\r\n");
}

void _APPLICATION_STATE_HandoverActive(void){
    int8_t rssi = -128;
    uint8_t prev_active_node =_data.active_node;
    
    for ( int i=0; i<NUMBER_OF_NODES; i++) {
        //check rssi
        if (nodes[i].rssi > rssi + 1) {
            if (data.handover_enabled == true) {
                _data.active_node = i;
            }
            rssi = nodes[i].rssi;
        }
        
        // check for disconnection of a single node
        if (nodes[i].status.connected == false) {
            _data.state_index = STATE_DISCONNECTION_IN_PROGRESS;
            APPLICATION_PRINT("Disconnection in Progress\r\n");
            return;
        }
    }
    if (prev_active_node != _data.active_node) {
        // Node switch
        if (data.handover_enabled == true) {
            APPLICATION_NODE_HciHandover3(&broadcast, nodes[_data.active_node].can_address);
        } else {
        }
        //APPLICATION_PRINT("Node Switch %s -> %s\r\n", nodes[prev_active_node].name, nodes[_data.active_node].name);
        
    }
}

static void _APPLICATION_STATE_DisconnectionInProgress(void) {
#if false
    APPLICATION_NODE_HciDisconnect(&broadcast, HCI_STATUS_REMOTE_USER_TERMINATED_CONNECTION);
#endif

    APPLICATION_PRINT("Disconnected\r\n");
    _data.state_index = STATE_DISCONNECTED;
}

static void _APPLICATION_STATE_Disconnected(void) {
#if true
     SYS_RESET_SoftwareReset();
#else
    for (int i=0; i<NUMBER_OF_NODES;i++) {
        if (nodes[i].status.connected == true) {
            return;
        }
    }
    _data.state_index = STATE_RESET_NODES;
#endif
}

static void _APPLICATION_STATE_Error(void) {
    // TODO: ADD IMPLEMENTATION    
}

static void _APPLICATION_STATE_Debug(void) {
    // TODO: ADD IMPLEMENTATION    
}

static volatile uint16_t timer_counter;
void _APPLICATION_TimerCallbackFunc(uint32_t counter) {
    
    // User LED Control
    if ( (counter % 100) == 0) {
        timer_counter++;
        uint16_t index = timer_counter % LED_PATTERN_SIZE;
        uint16_t mask = (1<<index);    
        (_led_pattern[_data.state_index] & mask) ? USER_LED_1_Clear() : USER_LED_1_Set();
    }
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void APPLICATION_Initialize(void) {
    data = _init_data;
    {
        _data.active_node = 0xFF;
        _data.state_index = STATE_RESET_NODES;
    }
    
    /*
     * Console
     */
    
    APPLICATION_CONSOLE_Initialize();
    
    /*
     * Button
     */
    APPLICATION_BUTTON_Initialize();
    APPLICATION_BUTTON_RegisterCallback(USR_BTN_1_PRESSED, _APPLICATION_UserButton1Pressed);
    APPLICATION_BUTTON_RegisterCallback(USR_BTN_1_RELEASED, _APPLICATION_UserButton1Released);

    
    /*
     * Timer
     */
    APPLICATION_TIMER_Initialize();
    timer_counter = 0;
    APPLICATION_TIMER_RegisterCallback(_APPLICATION_TimerCallbackFunc);
    
    
    /*
     * CAN
     */
    APPLICATION_CAN_Initialize();
    APPLICATION_CAN_RegisterCallback(CAN_DATA_ON_RX_FIFO_0, _APPLICATION_EVENT_CanDataOnRxFifo0Received);
    APPLICATION_CAN_RegisterCallback(CAN_DATA_ON_RX_FIFO_1, _APPLICATION_EVENT_CanDataOnRxFifo1Received);
    APPLICATION_CAN_RegisterCallback(CAN_TX_COMPLETE, NULL);
    
    /*
     * Node
     */
    for (int i=0; i<_NUMBER_OF_NODES; i++) {
        APPLICATION_NODE_Initialize(&nodes[i]);
    }
    
    /*
     * Version
     */
    SYS_CONSOLE_PRINT("\r\nSAME54 V%d.%d.%d\r\n",_version.major, _version.minor, _version.patch);
    
}

void APPLICATION_Tasks(void) {
    
    /*
     * Application State Machine
     */
    APPLICATION_STATE_FUNC_T func = stateTable[_data.state_index];
    func();
    
    APPLICATION_CONSOLE_Tasks();

}

void *APPLICATION_GetFunction(uint16_t id, const FUNC_PAIR_T *pFuncTable, size_t size) {
    for (int i=0; i<size; i++) {
        if (id == pFuncTable[i].id) {
            return pFuncTable[i].func;
        }
    }
    return NULL;
}

/**
    BLE handover demo application error dictionary
    Contains error code as key and corresponding error message
 */
typedef struct {
    uint8_t error_code;     /**< error code */
    char *msg;              /**< error message */
} APPLICATION_ERROR_PAIR_T;

APPLICATION_ERROR_PAIR_T error_table[] = {
    { ERROR_SUCCESS ,"Success" },
    { ERROR_CAN, "CAN Error" },
    { ERROR_NOT_IMPLEMENTED, "Feature Not Implemented" },
    { ERROR_INVALID_ID, "Invalid Index/Id"},
};

void APPLICATION_ErrorHandler(APPLICATION_ERROR_CODE_T errorCode) {
    for (int i=0; i< sizeof(error_table)/sizeof(APPLICATION_ERROR_PAIR_T); i++) {
        if (errorCode == error_table[i].error_code) {
            char buf[128]={0};
            size_t len = sprintf(buf, "\r\n[EEROR] %s",error_table[i].msg);
            APPLICATION_DBG_Write((uint8_t *)buf, len);
            break;
        }
    }
    // TODO _data.errorCode = errorCode;
    // TODO: ADD IMPLEMENTATION --> LED Signaling ...
    while (true) {
    }
}

void vApplicationDaemonTaskStartupHook( void ) {
}

/* *****************************************************************************
 End of File
 */
