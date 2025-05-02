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
#include <stdio.h>

#include "application.h"
#include "app_ble.h"
#include "ble_gap.h"
#include "timers.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
typedef enum APPLICATION_State_T {
    STATE_IDLE,
    STATE_SCANNING,
    STATE_CONNECTED,
    NUM_APPLICATION_STATES
}APPLICATION_State_T;


typedef struct APPLICATION_Data_T {
    APPLICATION_State_T state;

    TimerHandle_t timer;
    volatile uint32_t tick_counter;

    BLE_GAP_Addr_T addr;
    uint16_t conn_handle;
    
}APPLICATION_Data_T;
static APPLICATION_Data_T _data;

bool enter_deep_sleep = false;
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
#define SWITCH2_Get() ((GPIOB_REGS->GPIO_PORT >> 9) & 0x1U)
static void _APPLICATION_Switch2Callback (uintptr_t context){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    APP_Msg_T msg;
    if (SWITCH2_Get()) {
        msg.msgId = APP_MSG_SWITCH2_PRESSED;
    } else {
        msg.msgId = APP_MSG_SWITCH2_RELEASED;
    }
    xQueueSendFromISR( appData.appQueue, &msg, &xHigherPriorityTaskWoken );
}

#define SWITCH3_Get() ((GPIOA_REGS->GPIO_PORT >> 4) & 0x1U)
static void _APPLICATION_Switch3Callback (uintptr_t context){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    APP_Msg_T msg;
    if (SWITCH3_Get()) {
        msg.msgId = APP_MSG_SWITCH3_RELEASED;
    } else {
        msg.msgId = APP_MSG_SWITCH3_PRESSED;
    }
    xQueueSendFromISR( appData.appQueue, &msg, &xHigherPriorityTaskWoken );
}

#define LED_PATTERN_SIZE                10
/*
 * USER LED
 */
#define USER_LED_PATTERN_IDLE           (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
#define USER_LED_PATTERN_SCANNING       (uint16_t)((0<<9)|(1<<8)|(0<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
#define USER_LED_PATTERN_CONNECTED      (uint16_t)((1<<9)|(1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0))
/*
 * RGB LED RED
 */
#define RGB_LED_RED_PATTERN_IDLE        (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define RGB_LED_RED_PATTERN_SCANNING    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define RGB_LED_RED_PATTERN_CONNECTED   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
/*
 * RGB LED GREEN
 */
#define RGB_LED_GREEN_PATTERN_IDLE      (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define RGB_LED_GREEN_PATTERN_SCANNING  (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define RGB_LED_GREEN_PATTERN_CONNECTED (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
/*
 * RGB LED BLUE
 */
#define RGB_LED_BLUE_PATTERN_IDLE       (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define RGB_LED_BLUE_PATTERN_SCANNING   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define RGB_LED_BLUE_PATTERN_CONNECTED  (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))

static uint16_t _led_pattern[NUM_APPLICATION_STATES][4] = {
    {
        USER_LED_PATTERN_IDLE, RGB_LED_RED_PATTERN_IDLE, RGB_LED_GREEN_PATTERN_IDLE, RGB_LED_BLUE_PATTERN_IDLE,
    },{
        USER_LED_PATTERN_SCANNING, RGB_LED_RED_PATTERN_SCANNING, RGB_LED_GREEN_PATTERN_SCANNING, RGB_LED_BLUE_PATTERN_SCANNING,
    },{
        USER_LED_PATTERN_CONNECTED, RGB_LED_RED_PATTERN_CONNECTED, RGB_LED_GREEN_PATTERN_CONNECTED, RGB_LED_BLUE_PATTERN_CONNECTED,
    },
};

static void _APPLICATION_TimerCallback(TimerHandle_t pxTimer) {
    
    uint16_t index = _data.tick_counter % LED_PATTERN_SIZE;
    uint16_t mask = (1<<index);
    
    (_led_pattern[_data.state][0] & mask) ? USER_LED_On() : USER_LED_Off();
    (_led_pattern[_data.state][1] & mask) ? RGB_LED_RED_On() : RGB_LED_RED_Off();
    (_led_pattern[_data.state][2] & mask) ? RGB_LED_GREEN_On() : RGB_LED_GREEN_Off();
    (_led_pattern[_data.state][3] & mask) ? RGB_LED_BLUE_On() : RGB_LED_BLUE_Off();
           
    _data.tick_counter++;
}

static void _APPLICATION_TriggerStateMachine(void) {
    APP_Msg_T msg;
    msg.msgId = APP_MSG_TRIGGER_APPLICATION;
    xQueueSend(appData.appQueue, &msg, 0);
}

static void _APPLICATION_SwitchState(APPLICATION_State_T state) {
    _data.state = state;
    _APPLICATION_TriggerStateMachine();
}

static void _APPLICATION_SwitchOff(void) {
    SERCOM0_USART_Write((uint8_t *)"SWITCH OFF\r\n", 12);
    while (SERCOM0_USART_TransmitComplete() == false);

    SERCOM0_USART_Disable();

    xTimerStop(_data.timer, 0);
    
    USER_LED_Off();
    
    EIC_InterruptDisable(EIC_PIN_1);
    
    EIC_InterruptDisable(EIC_PIN_2);

    // Disable JTAG since at least one of its pins is configured for Non-JTAG function
    CFG_REGS->CFG_CFGCON0CLR = CFG_CFGCON0_JTAGEN_Msk;

    // Revert PORTA Initialization
    GPIOA_REGS->GPIO_CNPUSET = 0x00U; // Pull-Up Disable

    // Revert PORTB Initialization
    GPIOB_REGS->GPIO_LAT = 0x00U; // Initial Latch Value
    GPIOB_REGS->GPIO_TRISSET = 0xa9U; // Direction Control
    GPIOB_REGS->GPIO_ANSELSET = 0xa9U; // Digital Mode Enable
    GPIOB_REGS->GPIO_CNPUCLR = 0x200U; // Pull-Up Enable

    // Revert PPS Input Remapping 
    PPS_REGS->PPS_EXTINT1R = 0U;
    PPS_REGS->PPS_EXTINT2R = 0U;

    // Revert PPS Output Remapping


    BT_SYS_EnterDeepSleepMode();
    enter_deep_sleep = true;
}

static void _APPLICATION_StateIdle(APP_Msg_T *pMsg){
    BLE_GAP_SetScanningEnable(true, BLE_GAP_SCAN_FD_ENABLE, BLE_GAP_SCAN_MODE_OBSERVER, 1000);
    SERCOM0_USART_Write((uint8_t *)"Scanning \r\n", 11);
    _APPLICATION_SwitchState(STATE_SCANNING);
}

static void _APPLICATION_InitiateConnection(void) {
    BLE_GAP_CreateConnParams_T createConnParam_t;
    createConnParam_t.scanInterval = 0x3C; // 37.5 ms 
    createConnParam_t.scanWindow = 0x1E; // 18.75 ms
    createConnParam_t.filterPolicy = BLE_GAP_SCAN_FP_ACCEPT_ALL;
    createConnParam_t.peerAddr.addrType = _data.addr.addrType;
    memcpy(createConnParam_t.peerAddr.addr, _data.addr.addr, GAP_MAX_BD_ADDRESS_LEN);
    createConnParam_t.connParams.intervalMin = 0x10;// 0x24; // 20ms
    createConnParam_t.connParams.intervalMax = 0x10;// 0x24; // 20ms
    createConnParam_t.connParams.latency = 0;
    createConnParam_t.connParams.supervisionTimeout = 0x1F4;// 0x48; // 720ms

    SERCOM0_USART_Write((uint8_t *)"Initiating Connection\r\n", 23);

    BLE_GAP_CreateConnection(&createConnParam_t);
}

static void _APPLICATION_StateScanning(APP_Msg_T *pMsg){
    if (pMsg->msgId==APP_MSG_BLE_NODE_FOUND) {
        SERCOM0_USART_Write((uint8_t *)"Found Peer Node\r\n", 17);
        // stop scanning
        BLE_GAP_SetScanningEnable(false, BLE_GAP_SCAN_FD_ENABLE, BLE_GAP_SCAN_MODE_OBSERVER, 1000);
        _APPLICATION_InitiateConnection();
        

    } else if (pMsg->msgId==APP_MSG_BLE_NODE_CONNECTED) {
        SERCOM0_USART_Write((uint8_t *)"Connected\r\n", 11);
        
        _APPLICATION_SwitchState(STATE_CONNECTED);
        
    } else if (pMsg->msgId==APP_MSG_BLE_SCAN_COMPLETED){
        _APPLICATION_SwitchState(STATE_IDLE);
    } else {
        // TODO: Add Implementation
    }
    
}

static void _APPLICATION_StateConnected(APP_Msg_T *pMsg){
    if (pMsg->msgId==APP_MSG_BLE_NODE_DISCONNECTED) {
        char buf[80] = {0};
        size_t len = sprintf(buf, "Disconnected Re-Connect %x\r\n", pMsg->msgData[0] );
        SERCOM0_USART_Write((uint8_t *)buf, len);

        _APPLICATION_InitiateConnection();
        _APPLICATION_SwitchState(STATE_SCANNING);
    } else if (pMsg->msgId==APP_MSG_SWITCH2_PRESSED){

        BLE_GAP_Disconnect(_data.conn_handle, GAP_STATUS_REMOTE_TML_CONN /*Remote User Terminated Connection.*/ );
        _APPLICATION_SwitchOff();
        
    } else {
        // TODO: Add Implementation
    }
    
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void APPLICATION_Initialize(void) {
    
    // Switch 2
    EIC_CallbackRegister(EIC_PIN_1, _APPLICATION_Switch2Callback, (uintptr_t)NULL);
    EIC_InterruptEnable(EIC_PIN_1);
    // Switch 3
    EIC_CallbackRegister(EIC_PIN_2, _APPLICATION_Switch3Callback, (uintptr_t)NULL);
    EIC_InterruptEnable(EIC_PIN_2);
    
    // Application Timer
    uint32_t timer_id;
    _data.timer = xTimerCreate("APPLICATION TIMER", 100, pdTRUE, &timer_id, _APPLICATION_TimerCallback);
    _data.tick_counter = 0;
    if ( _data.timer ) {
        if ( xTimerStart(_data.timer, 0) != pdPASS) {
            // The timer could not be set into the Active state.
        }
    } else {
        // The timer was not created.
    }

    SERCOM0_USART_Write((uint8_t *)"\r\n\r\n WBZ351_CENTRAL\r\n\r\n",23);
    while (SERCOM0_USART_TransmitComplete() == false);
    
    _APPLICATION_SwitchState(STATE_IDLE);
        
}

void APPLICATION_Tasks(APP_Msg_T *pMsg) {
    switch (_data.state) {
        case STATE_IDLE: { _APPLICATION_StateIdle(pMsg); break; }
        case STATE_SCANNING: { _APPLICATION_StateScanning(pMsg); break; }
        case STATE_CONNECTED: { _APPLICATION_StateConnected(pMsg); break; }
        default: { break; }
    }
    
}

// <editor-fold defaultstate="collapsed" desc="defines for terminal debugging purpose">
#define _BLE_GAP_EVT_CONNECTED "BLE_GAP_EVT_CONNECTED\r\n"
#define _BLE_GAP_EVT_CONNECTED_LEN sizeof(_BLE_GAP_EVT_CONNECTED)
#define _BLE_GAP_EVT_DISCONNECTED "BLE_GAP_EVT_DISCONNECTED\r\n"
#define _BLE_GAP_EVT_DISCONNECTED_LEN sizeof(_BLE_GAP_EVT_DISCONNECTED)
#define _BLE_GAP_EVT_CONN_PARAM_UPDATE "BLE_GAP_EVT_CONN_PARAM_UPDATE\r\n"
#define _BLE_GAP_EVT_CONN_PARAM_UPDATE_LEN sizeof(_BLE_GAP_EVT_CONN_PARAM_UPDATE)
#define _BLE_GAP_EVT_ENCRYPT_STATUS "BLE_GAP_EVT_ENCRYPT_STATUS\r\n"
#define _BLE_GAP_EVT_ENCRYPT_STATUS_LEN sizeof(_BLE_GAP_EVT_ENCRYPT_STATUS)
#define _BLE_GAP_EVT_ADV_REPORT "BLE_GAP_EVT_ADV_REPORT\r\n"
#define _BLE_GAP_EVT_ADV_REPORT_LEN sizeof(_BLE_GAP_EVT_ADV_REPORT)
#define _BLE_GAP_EVT_ENC_INFO_REQUEST "BLE_GAP_EVT_ENC_INFO_REQUEST\r\n"
#define _BLE_GAP_EVT_ENC_INFO_REQUEST_LEN sizeof(_BLE_GAP_EVT_ENC_INFO_REQUEST)
#define _BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST "BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST\r\n"
#define _BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST_LEN sizeof(_BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST)
#define _BLE_GAP_EVT_EXT_ADV_REPORT "BLE_GAP_EVT_EXT_ADV_REPORT\r\n"
#define _BLE_GAP_EVT_EXT_ADV_REPORT_LEN sizeof(_BLE_GAP_EVT_EXT_ADV_REPORT)
#define _BLE_GAP_EVT_ADV_TIMEOUT "BLE_GAP_EVT_ADV_TIMEOUT\r\n"
#define _BLE_GAP_EVT_ADV_TIMEOUT_LEN sizeof(_BLE_GAP_EVT_ADV_TIMEOUT)
#define _BLE_GAP_EVT_TX_BUF_AVAILABLE "BLE_GAP_EVT_TX_BUF_AVAILABLE\r\n"
#define _BLE_GAP_EVT_TX_BUF_AVAILABLE_LEN sizeof(_BLE_GAP_EVT_TX_BUF_AVAILABLE)
#define _BLE_GAP_EVT_DEVICE_NAME_CHANGED "BLE_GAP_EVT_DEVICE_NAME_CHANGED\r\n"
#define _BLE_GAP_EVT_DEVICE_NAME_CHANGED_LEN sizeof(_BLE_GAP_EVT_DEVICE_NAME_CHANGED)
#define _BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT "BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT\r\n"
#define _BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT_LEN sizeof(_BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT)
#define _BLE_GAP_EVT_PHY_UPDATE "BLE_GAP_EVT_PHY_UPDATE\r\n"
#define _BLE_GAP_EVT_PHY_UPDATE_LEN sizeof(_BLE_GAP_EVT_PHY_UPDATE)
#define _BLE_GAP_EVT_SCAN_REQ_RECEIVED "BLE_GAP_EVT_SCAN_REQ_RECEIVED\r\n"
#define _BLE_GAP_EVT_SCAN_REQ_RECEIVED_LEN sizeof(_BLE_GAP_EVT_SCAN_REQ_RECEIVED)
#define _BLE_GAP_EVT_DIRECT_ADV_REPORT "BLE_GAP_EVT_DIRECT_ADV_REPORT\r\n"
#define _BLE_GAP_EVT_DIRECT_ADV_REPORT_LEN sizeof(_BLE_GAP_EVT_DIRECT_ADV_REPORT)
#define _BLE_GAP_EVT_PERI_ADV_SYNC_EST "BLE_GAP_EVT_PERI_ADV_SYNC_EST\r\n"
#define _BLE_GAP_EVT_PERI_ADV_SYNC_EST_LEN sizeof(_BLE_GAP_EVT_PERI_ADV_SYNC_EST)
#define _BLE_GAP_EVT_PERI_ADV_REPORT "BLE_GAP_EVT_PERI_ADV_REPORT\r\n"
#define _BLE_GAP_EVT_PERI_ADV_REPORT_LEN sizeof(_BLE_GAP_EVT_PERI_ADV_REPORT)
#define _BLE_GAP_EVT_PERI_ADV_SYNC_LOST "BLE_GAP_EVT_PERI_ADV_SYNC_LOST\r\n"
#define _BLE_GAP_EVT_PERI_ADV_SYNC_LOST_LEN sizeof(_BLE_GAP_EVT_PERI_ADV_SYNC_LOST)
#define _BLE_GAP_EVT_ADV_SET_TERMINATED "BLE_GAP_EVT_ADV_SET_TERMINATED\r\n"
#define _BLE_GAP_EVT_ADV_SET_TERMINATED_LEN sizeof(_BLE_GAP_EVT_ADV_SET_TERMINATED)
#define _BLE_GAP_EVT_SCAN_TIMEOUT "BLE_GAP_EVT_SCAN_TIMEOUT\r\n"
#define _BLE_GAP_EVT_SCAN_TIMEOUT_LEN sizeof(_BLE_GAP_EVT_SCAN_TIMEOUT)
#define _BLE_GAP_EVT_TRANSMIT_POWER_REPORTING "BLE_GAP_EVT_TRANSMIT_POWER_REPORTING\r\n"
#define _BLE_GAP_EVT_TRANSMIT_POWER_REPORTING_LEN sizeof(_BLE_GAP_EVT_TRANSMIT_POWER_REPORTING)
#define _BLE_GAP_EVT_ADV_COMPL "BLE_GAP_EVT_ADV_COMPL\r\n"
#define _BLE_GAP_EVT_ADV_COMPL_LEN sizeof(_BLE_GAP_EVT_ADV_COMPL)
#define _BLE_GAP_EVT_PATH_LOSS_THRESHOLD "BLE_GAP_EVT_PATH_LOSS_THRESHOLD\r\n"
#define _BLE_GAP_EVT_PATH_LOSS_THRESHOLD_LEN sizeof(_BLE_GAP_EVT_PATH_LOSS_THRESHOLD)
#define _BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL "BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL\r\n"
#define _BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL_LEN sizeof(_BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL)
// </editor-fold>

void APPLICATION_BleGapEventHandler(BLE_GAP_Event_T *p_event){
    switch(p_event->eventId) {
        case BLE_GAP_EVT_CONNECTED:{
            SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_CONNECTED, _BLE_GAP_EVT_CONNECTED_LEN);
            _data.conn_handle = p_event->eventField.evtConnect.connHandle;
            {
                APP_Msg_T msg;
                msg.msgId = APP_MSG_BLE_NODE_CONNECTED;
                xQueueSend(appData.appQueue, &msg, 0);
            }
            break;
        }
        case BLE_GAP_EVT_DISCONNECTED:{
            SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_DISCONNECTED, _BLE_GAP_EVT_DISCONNECTED_LEN);
            {
                BLE_GAP_EvtDisconnect_T *p_evt = (BLE_GAP_EvtDisconnect_T *)p_event;
                APP_Msg_T msg;
                msg.msgId = APP_MSG_BLE_NODE_DISCONNECTED;
                msg.msgData[0] = p_evt->reason;
                xQueueSend(appData.appQueue, &msg, 0);
            }
            
            break;
        }
        case BLE_GAP_EVT_CONN_PARAM_UPDATE:{
            SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_CONN_PARAM_UPDATE, _BLE_GAP_EVT_CONN_PARAM_UPDATE_LEN);
            break;
        }
        // <editor-fold defaultstate="collapsed" desc="not used at the moment">
        case BLE_GAP_EVT_ENCRYPT_STATUS: {SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_ENCRYPT_STATUS, _BLE_GAP_EVT_ENCRYPT_STATUS_LEN); break;}
        // </editor-fold>
        case BLE_GAP_EVT_ADV_REPORT:{
            // SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_ADV_REPORT, _BLE_GAP_EVT_ADV_REPORT_LEN);
            // Filter Devices based of Address, for this example address checking only 2 bytes
            
            // _Handover 9C:95:6E:00:00:00
            
            if (
                    p_event->eventField.evtAdvReport.addr.addr[5] == 0x9c && 
                    p_event->eventField.evtAdvReport.addr.addr[4] == 0x95 &&
                    p_event->eventField.evtAdvReport.addr.addr[3] == 0x6E &&
                    p_event->eventField.evtAdvReport.addr.addr[2] == 0x00 &&
                    p_event->eventField.evtAdvReport.addr.addr[1] == 0x00 &&
                    p_event->eventField.evtAdvReport.addr.addr[0] == 0x00 
                ) {

                _data.addr = p_event->eventField.evtAdvReport.addr;
                {
                    APP_Msg_T msg;
                    msg.msgId = APP_MSG_BLE_NODE_FOUND;
                    xQueueSend(appData.appQueue, &msg, 0);
                }
            }
            break;
        }
        // <editor-fold defaultstate="collapsed" desc="not used at the moment">
        case BLE_GAP_EVT_ENC_INFO_REQUEST: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_ENC_INFO_REQUEST, _BLE_GAP_EVT_ENC_INFO_REQUEST_LEN); break; }
        case BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST, _BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST_LEN); break; }
        case BLE_GAP_EVT_EXT_ADV_REPORT: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_EXT_ADV_REPORT, _BLE_GAP_EVT_EXT_ADV_REPORT_LEN); break; }
        case BLE_GAP_EVT_ADV_TIMEOUT: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_ADV_TIMEOUT, _BLE_GAP_EVT_ADV_TIMEOUT_LEN); break; }
        case BLE_GAP_EVT_TX_BUF_AVAILABLE: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_TX_BUF_AVAILABLE, _BLE_GAP_EVT_TX_BUF_AVAILABLE_LEN); break; }
        case BLE_GAP_EVT_DEVICE_NAME_CHANGED: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_PHY_UPDATE, _BLE_GAP_EVT_PHY_UPDATE_LEN); break; }
        case BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_PHY_UPDATE, _BLE_GAP_EVT_PHY_UPDATE_LEN); break; }
        case BLE_GAP_EVT_PHY_UPDATE: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_PHY_UPDATE, _BLE_GAP_EVT_PHY_UPDATE_LEN); break; }
        case BLE_GAP_EVT_SCAN_REQ_RECEIVED: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_SCAN_REQ_RECEIVED, _BLE_GAP_EVT_SCAN_REQ_RECEIVED_LEN); break; }
        case BLE_GAP_EVT_DIRECT_ADV_REPORT: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_DIRECT_ADV_REPORT, _BLE_GAP_EVT_DIRECT_ADV_REPORT_LEN); break; }
        case BLE_GAP_EVT_PERI_ADV_SYNC_EST: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_PERI_ADV_SYNC_EST, _BLE_GAP_EVT_PERI_ADV_SYNC_EST_LEN); break; }
        case BLE_GAP_EVT_PERI_ADV_REPORT: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_PERI_ADV_REPORT, _BLE_GAP_EVT_PERI_ADV_REPORT_LEN); break; }
        case BLE_GAP_EVT_PERI_ADV_SYNC_LOST: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_PERI_ADV_SYNC_LOST, _BLE_GAP_EVT_PERI_ADV_SYNC_LOST_LEN); break; }
        case BLE_GAP_EVT_ADV_SET_TERMINATED: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_ADV_SET_TERMINATED, _BLE_GAP_EVT_ADV_SET_TERMINATED_LEN); break; }
        // </editor-fold>   
        case BLE_GAP_EVT_SCAN_TIMEOUT: {
            SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_SCAN_TIMEOUT, _BLE_GAP_EVT_SCAN_TIMEOUT_LEN);
			// SERCOM0_USART_Write((uint8_t *)"Scan Completed \r\n", 17);
            {
                APP_Msg_T msg;
                msg.msgId = APP_MSG_BLE_SCAN_COMPLETED;
                xQueueSend(appData.appQueue, &msg, 0);
            }
            break;
        }
        // <editor-fold defaultstate="collapsed" desc="not used at the moment">
        case BLE_GAP_EVT_TRANSMIT_POWER_REPORTING: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_TRANSMIT_POWER_REPORTING, _BLE_GAP_EVT_TRANSMIT_POWER_REPORTING_LEN); break; }
        case BLE_GAP_EVT_ADV_COMPL: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_ADV_COMPL, _BLE_GAP_EVT_ADV_COMPL_LEN); break; }
        case BLE_GAP_EVT_PATH_LOSS_THRESHOLD: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_PATH_LOSS_THRESHOLD, _BLE_GAP_EVT_PATH_LOSS_THRESHOLD_LEN); break; }
        case BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL: { SERCOM0_USART_Write((uint8_t *)_BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL, _BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL_LEN); break; }
        // </editor-fold>
        default: {
            break;
        }
    }
    // TODO: Add implementation
}

void vApplicationDaemonTaskStartupHook(void){
}

/* *****************************************************************************
 End of File
 */
