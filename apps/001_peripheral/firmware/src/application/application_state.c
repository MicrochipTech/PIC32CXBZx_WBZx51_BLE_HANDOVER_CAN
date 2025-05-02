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

#include "application_state.h"
#include "application.h"
#include "application_cmd.h"
#include "ble_gap.h"
#include "mba_error_defs.h"
#include "host_hci_defs.h"
#include "application_config.h"
#include "application_common.h"
#include "ble_handover/ble_handover.h"

#include <stdio.h>

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#if false
#define APPLICATION_STATE_DEBUG(__PRINT__)
#else
#define APPLICATION_STATE_DEBUG(__PRINT__)  do { \
                                              /* SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,"APPLICATION_STATE[%d] ", __LINE__); */ \
                                              SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,__PRINT__); \
                                              SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,"\r\n"); \
                                          } while (0)
#endif

typedef void (*STATE_MACHINE_FUNC_T)(APP_Msg_T *);

static const char *_stateTable[]  __attribute__((used)) = {
    "STATE_INIT",
    "STATE_IDLE",
    "STATE_ADVERTISING",
    "STATE_ACTIVE_CONNECTED",
    "STATE_PASSIVE_CONNECTED",
    "STATE_DISCONNECTED",
    "STATE_ERROR",
    "STATE_DEBUG",
};

static struct {
    STATE_MACHINE_FUNC_T *pStateMachine;
    APPLICATION_STATES_T index;
    bool isStateSwitched;
}_data;


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
static void _APPLICATION_STATE_Init(APP_Msg_T *pMsg);
static void _APPLICATION_STATE_Idle(APP_Msg_T *pMsg);
static void _APPLICATION_STATE_Advertising(APP_Msg_T *pMsg);
static void _APPLICATION_STATE_ActiveConnected(APP_Msg_T *pMsg);
static void _APPLICATION_STATE_PassiveConnected(APP_Msg_T *pMsg);
static void _APPLICATION_STATE_Disconnected(APP_Msg_T *pMsg);
static void _APPLICATION_STATE_Error(APP_Msg_T *pMsg);
static void _APPLICATION_STATE_Debug(APP_Msg_T *pMsg);

static const STATE_MACHINE_FUNC_T _stateMachineTable[] = {
    _APPLICATION_STATE_Init,
    _APPLICATION_STATE_Idle,
    _APPLICATION_STATE_Advertising,
    _APPLICATION_STATE_ActiveConnected,
    _APPLICATION_STATE_PassiveConnected,
    _APPLICATION_STATE_Disconnected,
    _APPLICATION_STATE_Error,
    _APPLICATION_STATE_Debug
};



static void _APPLICATION_STATE_SendMsg(APP_MsgId_T id) {
    APP_Msg_T msg;
    msg.msgId = id;
    OSAL_QUEUE_Send(&appData.appQueue, &msg, 0);
}

static void _APPLICATION_STATE_Trigger(void) {
    _APPLICATION_STATE_SendMsg(APP_MSG_APP_TRIGGER);
}

static void _APPLICATION_STATE_Switch(APPLICATION_STATES_T state, bool trigger) {
    _data.index = state;
    _data.isStateSwitched = true;
    if (trigger == true) {
        _APPLICATION_STATE_Trigger();
    }
}

static volatile bool timer_elapsed;
static volatile uint16_t down_counter;
static void _APPLICATION_STATE_TimerFunc(TC_TIMER_STATUS status, uintptr_t context) 
{
    if (down_counter == 0) {
        TC2_TimerStop();
        GPIO_RB11_Clear();
        timer_elapsed = true;
        _APPLICATION_STATE_Trigger();
    } else {
        down_counter--;
        GPIO_RB11_Toggle();
    }
}


static void _APPLICATION_STATE_TimerStart(uint16_t time_in_ms){
    TC2_TimerCallbackRegister(_APPLICATION_STATE_TimerFunc, (uintptr_t)NULL);
    down_counter = time_in_ms;
    timer_elapsed = false;
    GPIO_RB11_Set();
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,"TC2_TimerStart()\r\n");
    TC2_TimerStart();
}

static bool _APPLICATION_STATE_IsTimerElapsed(void) { return timer_elapsed; }

static void _APPLICATION_STATE_Init(APP_Msg_T *pMsg) {
    uint16_t addr = APPLICATION_CONFIG_GetCanAddress();
    HCI_EVT_COMMAND_COMPLETE_T params = {
        .num_hci_command_packets = 1,
        .ocf = HCI_RESET,
        .ogf = HCI_HC,
        .hci_reset.status = 0x00,
    };
    
    APPLICATION_CMD_HciEventCommandComplete(addr, &params, sizeof(HCI_EVT_RESET_COMMAND_COMPLETE_PARAMS_T) );
    _APPLICATION_STATE_Switch( STATE_IDLE, true);
}

static void _APPLICATION_STATE_Idle(APP_Msg_T *pMsg) {
    // Configure Advertising
    BLE_GAP_Addr_T addr;
    APPLICATION_CONFIG_GetBleAddress(&addr);
    BLE_GAP_SetDeviceAddr(&addr);
    
    BLE_GAP_GetDeviceAddr(&addr);
    
    char buf[128] = {0};
    sprintf(buf,"\r\nDeviceAddress: %02X %02X %02X %02X %02X %02X\r\n", 
            addr.addr[0],addr.addr[1],addr.addr[2],addr.addr[3],addr.addr[4],addr.addr[5]);
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, buf);
    
            
            
    BLE_GAP_AdvParams_T advParam = {
        .intervalMin = 0x400,   /* Minimum advertising interval, see @ref BLE_GAP_ADV_INTERVAL. (Unit: 0.625 ms) */
        .intervalMax = 0x400,   /* Maximum advertising interval, see @ref BLE_GAP_ADV_INTERVAL. (Unit: 0.625 ms) */
        .type = BLE_GAP_ADV_TYPE_ADV_IND,           /* Advertising type, see @ref BLE_GAP_ADV_TYPE. */
        .peerAddr = {           /* Remote bluetooth address and address type, see @ref BLE_GAP_Addr_T.*/
            .addrType = BLE_GAP_ADDR_TYPE_PUBLIC,   /* See @ref BLE_GAP_ADDR_TYPE. */
            .addr={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},   /* Bluetooth address, LSB format. */        
        },
        .advChannelMap = BLE_GAP_ADV_CHANNEL_ALL,  /* Advertising Channel Map. See @ref BLE_GAP_ADV_CHANNEL. */
        .filterPolicy = BLE_GAP_ADV_FILTER_DEFAULT,   /* Advertising filter policy. See @ref BLE_GAP_ADV_FILTER_POLICY. */
    };
    BLE_GAP_SetAdvParams(&advParam);
    
    BLE_GAP_AdvDataParams_T advData = {
        .advLen = 0x14, /* Length of advertising data, see @ref BLE_GAP_ADV_DATA_LEN for the maximum length. */
        .advData={
            0x02, 0x01, 0x06,                               /* 0x01: Flags */
            0x0A, 0x09, 0x5F, 0x48, 0x61, 0x6E, 0x64, 0x6F, 0x76, 0x65, 0x72,   /* 0x09: Complete Local Name - "_Handover" */
            // 0x0A, 0x09, 0x5F, 0x47, 0x61, 0x6D, 0x65, 0x6F, 0x76, 0x65, 0x72,   /* 0x09: Complete Local Name - "_Gameover" */
            0x05, 0x16, 0xCD, 0x00, 0xFE, 0xDA,             /* 0x16: Service Data - 16-bit UUID */
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    /* Padding */
        },  /* Advertising data. */
        
    };
    BLE_GAP_SetAdvData(&advData);
    
    // Start Advertising
    uint16_t status = BLE_GAP_SetAdvEnable(true, 0);
    if (status != MBA_RES_SUCCESS ) {
        APPLICATION_ErrorHandler( ERROR_START_ADV_FAILED );
    } else {
        // SUCCESS -> Continue
    }
    // Switch to Advertising State
    _APPLICATION_STATE_Switch( STATE_ADVERTISING, true);
}

static void _APPLICATION_STATE_Advertising(APP_Msg_T *pMsg) {
    if (data.status.isConnected == true) {
        if (BLE_HANDOVER_GetRole()/*data.handover_role*/ == ACTIVE_NODE) {
            APPLICATION_CMD_Handover1();
            _APPLICATION_STATE_Switch( STATE_ACTIVE_CONNECTED, true );
        } else {
            _APPLICATION_STATE_Switch( STATE_PASSIVE_CONNECTED, true );
        }
    } else {
        // Do Nothing !!!
    }
}

static void _APPLICATION_STATE_ActiveConnected(APP_Msg_T *pMsg) {
    if (data.status.isConnected == false) {
        // TODO: FORWARD DISCONNECTION_COMPLETE EVENT TO HOST
        _APPLICATION_STATE_Switch( STATE_DISCONNECTED, true );
    } else {
        if (BLE_HANDOVER_GetRole()/*data.handover_role*/ == PASSIVE_NODE) {
            _APPLICATION_STATE_Switch( STATE_PASSIVE_CONNECTED, true );
        } else {
            // Do Nothing !!!
        }
    }
}

static void _APPLICATION_STATE_PassiveConnected(APP_Msg_T *pMsg) {
    if (data.status.isConnected == false) {
        // TODO: FORWARD DISCONNECTION_COMPLETE EVENT TO HOST
        _APPLICATION_STATE_Switch( STATE_DISCONNECTED, true );
    } else {
        if (BLE_HANDOVER_GetRole()/*data.handover_role*/ == ACTIVE_NODE) {
            _APPLICATION_STATE_Switch( STATE_ACTIVE_CONNECTED, true );
        } else {
            // Do Nothing !!!
        }
    }
}

static void _APPLICATION_STATE_Disconnected(APP_Msg_T *pMsg) {
#if true
    //--M43814-- 
    _APPLICATION_STATE_Switch( STATE_IDLE, true );
#else
    
    APPLICATION_COMMON_Reset();
#endif
}

static void _APPLICATION_STATE_Error(APP_Msg_T *pMsg) {
    //  TODO: Add Implementation
}

static void _APPLICATION_STATE_Debug(APP_Msg_T *pMsg) {
    //  TODO: Add Implementation
}

static void _APPLICATION_STATE_Print() {
    if ( _data.isStateSwitched) {
        _data.isStateSwitched = false;
        char buffer[128] = {0};
        sprintf(buffer, "%s",_stateTable[_data.index]);
        
        APPLICATION_STATE_DEBUG(buffer);
    }
}



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void APPLICATION_STATE_Initialize(void) {
    
    _data.pStateMachine = (STATE_MACHINE_FUNC_T *) _stateMachineTable;
    
    _APPLICATION_STATE_Switch(STATE_INIT, true);
    
    
}

void APPLICATION_STATE_Tasks(APP_Msg_T *pMsg) {
    if (_data.pStateMachine != NULL) {
        _APPLICATION_STATE_Print();
        STATE_MACHINE_FUNC_T func = _data.pStateMachine[_data.index];
        func(pMsg);
    }
}

APPLICATION_STATES_T APPLICATION_STATE_GetState(void) {
    return _data.index;
}

void APPLICATION_STATE_SetInErrorState(void) {
    _APPLICATION_STATE_Switch(STATE_ERROR, true);
}

/* *****************************************************************************
 End of File
 */
