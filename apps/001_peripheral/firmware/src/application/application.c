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

#include <stdio.h>
#include <stdlib.h>

#include "definitions.h"
#include "application_button.h"
#include "application_can.h"
#include "application_cmd.h"
#include "application_config.h"
#include "application_evt.h"
#include "application_led.h"
#include "application_state.h"
#include "application_tempsensor.h"
#include "application_timer.h"

#include "definitions.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#define APPLICATION_DEBUG(__PRINT__)  do { \
                                              SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,"APPLICATION[%d] ", __LINE__); \
                                              SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,__PRINT__); \
                                              SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,"\r\n"); \
                                          } while (0)

/**
Internal Application data structure
 */
typedef struct {
    uint8_t state;  /**< application state */
    APPLICATION_ERROR_CODE_T errorCode; /**< application error code */
}_APPLICATION_DATA_T;
/**
Private Application data
 */
static _APPLICATION_DATA_T _data;

/**
Initialization values of private application data _data
 */
static const _APPLICATION_DATA_T _initData = {
    .state = STATE_INIT,
    .errorCode = ERROR_SUCCESS
};
/**
Public Application data
 */
APPLICATION_DATA_T data;

/**
Initialization values of public application data_data
 */
static const APPLICATION_DATA_T initData = {
    .status = { 
        .isConnected = false,  
        .bit_1 = 0,
        .bit_2 = 0,
        .bit_3 = 0,
        .bit_4 = 0,
        .bit_5 = 0,
        .bit_6 = 0,
        .bit_7 = 0,
    },
    .handover_role = ACTIVE_NODE,
    .prev_rssi = -128,
};

/**
Application version
 */
static const struct {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
}_version = { .major = 0, .minor = 8, .patch = 0};


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
#define LED_PATTERN_SIZE                10

/*
 * STATE_INIT
 */
#define USER_LED_PATTERN_STATE_INIT     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0))
#define RED_LED_PATTERN_STATE_INIT      (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define GREEN_LED_PATTERN_STATE_INIT    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define BLUE_LED_PATTERN_STATE_INIT     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_RED_LED_PATTERN_STATE_INIT      (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_GREEN_LED_PATTERN_STATE_INIT    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_BLUE_LED_PATTERN_STATE_INIT     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
/*
 * STATE_IDLE
 */
#define USER_LED_PATTERN_STATE_IDLE     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0))
#define RED_LED_PATTERN_STATE_IDLE      (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define GREEN_LED_PATTERN_STATE_IDLE    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define BLUE_LED_PATTERN_STATE_IDLE     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_RED_LED_PATTERN_STATE_IDLE      (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_GREEN_LED_PATTERN_STATE_IDLE    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_BLUE_LED_PATTERN_STATE_IDLE     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
/*
 * STATE_ADVERTISING
 */
    #define USER_LED_PATTERN_STATE_ADVERTISING  (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
    #define RED_LED_PATTERN_STATE_ADVERTISING   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
    #define GREEN_LED_PATTERN_STATE_ADVERTISING (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
    #define BLUE_LED_PATTERN_STATE_ADVERTISING  (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
    #define EXT_RED_LED_PATTERN_STATE_ADVERTISING   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
    #define EXT_GREEN_LED_PATTERN_STATE_ADVERTISING (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
    #define EXT_BLUE_LED_PATTERN_STATE_ADVERTISING  (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
/*
 * STATE_ACTIVE_CONNECTED
 */
#define USER_LED_PATTERN_STATE_ACTIVE_CONNECTED     (uint16_t)((1<<9)|(1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0))
#define RED_LED_PATTERN_STATE_ACTIVE_CONNECTED      (uint16_t)((0<<9)|(0<<8)|(0<<7)|(1<<6)|(1<<5)|(0<<4)|(0<<3)|(0<<2)|(1<<1)|(1<<0))
#define GREEN_LED_PATTERN_STATE_ACTIVE_CONNECTED    (uint16_t)((1<<9)|(1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0))
#define BLUE_LED_PATTERN_STATE_ACTIVE_CONNECTED     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_RED_LED_PATTERN_STATE_ACTIVE_CONNECTED      (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_GREEN_LED_PATTERN_STATE_ACTIVE_CONNECTED    (uint16_t)((1<<9)|(1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0))
#define EXT_BLUE_LED_PATTERN_STATE_ACTIVE_CONNECTED     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
/* 
 * STATE_PASSIVE_CONNECTED
 */
#define USER_LED_PATTERN_STATE_PASSIVE_CONNECTED    (uint16_t)((0<<9)|(1<<8)|(0<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
#define RED_LED_PATTERN_STATE_PASSIVE_CONNECTED     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define GREEN_LED_PATTERN_STATE_PASSIVE_CONNECTED   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define BLUE_LED_PATTERN_STATE_PASSIVE_CONNECTED    (uint16_t)((0<<9)|(1<<8)|(0<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
#define EXT_RED_LED_PATTERN_STATE_PASSIVE_CONNECTED     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_GREEN_LED_PATTERN_STATE_PASSIVE_CONNECTED   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_BLUE_LED_PATTERN_STATE_PASSIVE_CONNECTED    (uint16_t)((0<<9)|(1<<8)|(0<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
/*
 * STATE_DISCONNECTED
 */
#define USER_LED_PATTERN_STATE_DISCONNECTED     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define RED_LED_PATTERN_STATE_DISCONNECTED      (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define GREEN_LED_PATTERN_STATE_DISCONNECTED    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define BLUE_LED_PATTERN_STATE_DISCONNECTED     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_RED_LED_PATTERN_STATE_DISCONNECTED     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_GREEN_LED_PATTERN_STATE_DISCONNECTED   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_BLUE_LED_PATTERN_STATE_DISCONNECTED    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
/* 
 * STATE_ERROR
 */
#if false
#define USER_LED_PATTERN_STATE_ERROR    (uint16_t)((1<<9)|(0<<8)|(1<<7)|(0<<6)|(1<<5)|(0<<4)|(1<<3)|(0<<2)|(1<<1)|(0<<0))
#define RED_LED_PATTERN_STATE_ERROR     (uint16_t)((0<<9)|(1<<8)|(0<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
#define GREEN_LED_PATTERN_STATE_ERROR   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define BLUE_LED_PATTERN_STATE_ERROR    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_RED_LED_PATTERN_STATE_ERROR     (uint16_t)((0<<9)|(1<<8)|(0<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
#define EXT_GREEN_LED_PATTERN_STATE_ERROR   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_BLUE_LED_PATTERN_STATE_ERROR    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#else
    #define USER_LED_PATTERN_STATE_ERROR  (uint16_t)((1<<9)|(0<<8)|(1<<7)|(0<<6)|(1<<5)|(0<<4)|(1<<3)|(0<<2)|(1<<1)|(0<<0))
    #define RED_LED_PATTERN_STATE_ERROR   (uint16_t)((0<<9)|(1<<8)|(0<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(1<<2)|(0<<1)|(1<<0))
    #define GREEN_LED_PATTERN_STATE_ERROR (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
    #define BLUE_LED_PATTERN_STATE_ERROR  (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
    #define EXT_RED_LED_PATTERN_STATE_ERROR   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(1<<1)|(1<<0))
    #define EXT_GREEN_LED_PATTERN_STATE_ERROR (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(1<<4)|(1<<3)|(0<<2)|(0<<1)|(0<<0))
    #define EXT_BLUE_LED_PATTERN_STATE_ERROR  (uint16_t)((0<<9)|(0<<8)|(1<<7)|(1<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#endif
/*
 * STATE_DEBUG
 */
#define USER_LED_PATTERN_STATE_DEBUG    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define RED_LED_PATTERN_STATE_DEBUG     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define GREEN_LED_PATTERN_STATE_DEBUG   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define BLUE_LED_PATTERN_STATE_DEBUG    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_RED_LED_PATTERN_STATE_DEBUG     (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_GREEN_LED_PATTERN_STATE_DEBUG   (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
#define EXT_BLUE_LED_PATTERN_STATE_DEBUG    (uint16_t)((0<<9)|(0<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))



static uint16_t _led_pattern[NUM_APPLICATION_STATES][7] = {
    {   
        USER_LED_PATTERN_STATE_INIT, 
        RED_LED_PATTERN_STATE_INIT, GREEN_LED_PATTERN_STATE_INIT, BLUE_LED_PATTERN_STATE_INIT,
        EXT_RED_LED_PATTERN_STATE_INIT, EXT_GREEN_LED_PATTERN_STATE_INIT, EXT_BLUE_LED_PATTERN_STATE_INIT,
    },
    {
        USER_LED_PATTERN_STATE_IDLE, 
        RED_LED_PATTERN_STATE_IDLE, GREEN_LED_PATTERN_STATE_IDLE, BLUE_LED_PATTERN_STATE_IDLE,
        EXT_RED_LED_PATTERN_STATE_IDLE, EXT_GREEN_LED_PATTERN_STATE_IDLE, EXT_BLUE_LED_PATTERN_STATE_IDLE,
    },
    {
        USER_LED_PATTERN_STATE_ADVERTISING, 
        RED_LED_PATTERN_STATE_ADVERTISING, GREEN_LED_PATTERN_STATE_ADVERTISING, BLUE_LED_PATTERN_STATE_ADVERTISING,
        EXT_RED_LED_PATTERN_STATE_ADVERTISING, EXT_GREEN_LED_PATTERN_STATE_ADVERTISING, EXT_BLUE_LED_PATTERN_STATE_ADVERTISING,
    },
    {
        USER_LED_PATTERN_STATE_ACTIVE_CONNECTED, 
        RED_LED_PATTERN_STATE_ACTIVE_CONNECTED, GREEN_LED_PATTERN_STATE_ACTIVE_CONNECTED, BLUE_LED_PATTERN_STATE_ACTIVE_CONNECTED,
        EXT_RED_LED_PATTERN_STATE_ACTIVE_CONNECTED, EXT_GREEN_LED_PATTERN_STATE_ACTIVE_CONNECTED, EXT_BLUE_LED_PATTERN_STATE_ACTIVE_CONNECTED,
    },
    {
        USER_LED_PATTERN_STATE_PASSIVE_CONNECTED, 
        RED_LED_PATTERN_STATE_PASSIVE_CONNECTED, GREEN_LED_PATTERN_STATE_PASSIVE_CONNECTED, BLUE_LED_PATTERN_STATE_PASSIVE_CONNECTED,
        EXT_RED_LED_PATTERN_STATE_PASSIVE_CONNECTED, EXT_GREEN_LED_PATTERN_STATE_PASSIVE_CONNECTED, EXT_BLUE_LED_PATTERN_STATE_PASSIVE_CONNECTED,
    },
    {
        USER_LED_PATTERN_STATE_DISCONNECTED, 
        RED_LED_PATTERN_STATE_DISCONNECTED, GREEN_LED_PATTERN_STATE_DISCONNECTED, BLUE_LED_PATTERN_STATE_DISCONNECTED,
        EXT_RED_LED_PATTERN_STATE_DISCONNECTED, EXT_GREEN_LED_PATTERN_STATE_DISCONNECTED, EXT_BLUE_LED_PATTERN_STATE_DISCONNECTED,
    },
    {
        USER_LED_PATTERN_STATE_ERROR, 
        RED_LED_PATTERN_STATE_ERROR, GREEN_LED_PATTERN_STATE_ERROR, BLUE_LED_PATTERN_STATE_ERROR,
        EXT_RED_LED_PATTERN_STATE_ERROR, EXT_GREEN_LED_PATTERN_STATE_ERROR, EXT_BLUE_LED_PATTERN_STATE_ERROR,
    },
    {
        USER_LED_PATTERN_STATE_DEBUG, 
        RED_LED_PATTERN_STATE_DEBUG, GREEN_LED_PATTERN_STATE_DEBUG, BLUE_LED_PATTERN_STATE_DEBUG,
        EXT_RED_LED_PATTERN_STATE_DEBUG, EXT_GREEN_LED_PATTERN_STATE_DEBUG, EXT_BLUE_LED_PATTERN_STATE_DEBUG,
    },
};

uint32_t _counter = 0;

static void _APPLICATION_TimerCallback(void) {
    /*
     * LED control
     */
    APPLICATION_STATES_T state  = APPLICATION_STATE_GetState();
    uint16_t index = _counter % LED_PATTERN_SIZE;
    uint16_t mask = (1<<index);
    
    (_led_pattern[state][0] & mask) ? APPLICATION_USER_LED_On() : APPLICATION_USER_LED_Off();
    (_led_pattern[state][1] & mask) ? APPLICATION_RGB_LED_RED_On() : APPLICATION_RGB_LED_RED_Off();
    // (_led_pattern[state][2] & mask) ? APPLICATION_RGB_LED_GREEN_On() : APPLICATION_RGB_LED_GREEN_Off();
    // (_led_pattern[state][3] & mask) ? APPLICATION_RGB_LED_BLUE_On() : APPLICATION_RGB_LED_BLUE_Off();
    (_led_pattern[state][4] & mask) ? EXT_RGB_LED_RED_Set() : EXT_RGB_LED_RED_Clear();
    (_led_pattern[state][5] & mask) ? EXT_RGB_LED_GREEN_Set() : EXT_RGB_LED_GREEN_Clear();
    (_led_pattern[state][6] & mask) ? EXT_RGB_LED_BLUE_Set() : EXT_RGB_LED_BLUE_Clear();
    
    /*
     * Trigger application periodically every 2s to reset watchdog in main loop
     */
    uint16_t app_trigger = _counter % 20; // 20 * 100ms = 2s
    if (app_trigger == 0) {
        APP_Msg_T msg;
        msg.msgId = APP_MSG_APP_TRIGGER;
        OSAL_QUEUE_SendISR(&appData.appQueue, &msg);
    }
    
    /*
     * Lock Handling ( Automatic release of lock after 3s )
     */
    // --M43814--GW-- TODO
    
    
    _counter++;    
}

static void _APPLICATION_RssiChanged(uint16_t conn_handle, int rssi) {
    APP_Msg_T msg;
    msg.msgId = APP_MSG_RSSI_CHANGED;

    RSSI_MSG_STRUCT_T *p_rssi_msg= (RSSI_MSG_STRUCT_T *)msg.msgData;
    p_rssi_msg->conn_handle = conn_handle;
    p_rssi_msg->rssi = rssi;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(appData.appQueue, &msg, &xHigherPriorityTaskWoken);                
}


static void _APPLICATION_GpioRb2Set(void) {GPIO_RB2_Set();}
static void _APPLICATION_GpioRb2Clear(void) {GPIO_RB2_Clear();}
static void _APPLICATION_GpioRb2Toggle(void) {GPIO_RB2_Toggle();}

static void _APPLICATION_GpioRb11Set(void) {GPIO_RB11_Set();}
static void _APPLICATION_GpioRb11Clear(void) {GPIO_RB11_Clear();}
static void _APPLICATION_GpioRb11Toggle(void) {GPIO_RB11_Toggle();}

static void _APPLICATION_TraceFunc(uint8_t *pBuf, const size_t length) {
    SYS_DEBUG_PRINT(SYS_ERROR_INFO, " %.*s", length, pBuf);
}
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void APPLICATION_Initialize(void) {
    

    _data = _initData;
    GPIO_RB11_Clear();

    data = initData;

    SYS_DEBUG_PRINT(
            SYS_ERROR_INFO, 
            "\r\n-----\r\n-- WBZ451 v.%d.%d.%d\r\n-- %s %s\r\n-----\r\n", 
            _version.major, _version.minor, _version.patch, __DATE__, __TIME__);

    APPLICATION_CAN_Initialize();
    APPLICATION_EVT_Initialize();
    APPLICATION_STATE_Initialize();
    APPLICATION_TIMER_Initialize(_APPLICATION_TimerCallback);
    APPLICATION_BUTTON_Initialize();
    APPLICATION_TEMPSENSOR_Initialize();
    APPLICATION_LED_Initialize();
    APPLICATION_CMD_Initialize();
    APPLICATION_CONFIG_Initialize();
    
    if (APPLICATION_CONFIG_ValidCheck() == false ) {
        EXT_RELAY_Set();
        APPLICATION_ErrorHandler(ERROR_CONFIG_FAILURE);
    }
    
    BLE_HANDOVER_Initialize();
    BLE_HANDOVER_RegisterRssiCallback(_APPLICATION_RssiChanged);
    BLE_HANDOVER_RegisterGpioSetCallback(_APPLICATION_GpioRb2Set);
    BLE_HANDOVER_RegisterGpioClearCallback(_APPLICATION_GpioRb2Clear);
    BLE_HANDOVER_RegisterGpioToggleCallback(_APPLICATION_GpioRb2Toggle);

    BLE_HANDOVER_RegisterGpio2SetCallback(_APPLICATION_GpioRb11Set);
    BLE_HANDOVER_RegisterGpio2ClearCallback(_APPLICATION_GpioRb11Clear);
    BLE_HANDOVER_RegisterGpio2ToggleCallback(_APPLICATION_GpioRb11Toggle);
    
    BLE_HANDOVER_RegisterTraceCallback(_APPLICATION_TraceFunc);
    
    //WDT_Enable();
    
}


void APPLICATION_Tasks(APP_Msg_T *pMsg) {
    WDT_Clear();
    
    /*
     * Application CAN Handling
     */
    APPLICATION_CAN_Tasks(pMsg);
    
    /*
     * Application Event Handling
     */
    APPLICATION_EVT_Tasks(pMsg);
    
    /*
     * Application Command Handling
     */
    APPLICATION_CMD_Tasks(pMsg);
    
    /*
     * Application State Machine Handling
     */
    APPLICATION_STATE_Tasks(pMsg);
    
    
}

/**
Dictionary struct contiaining error_code as key and error_message as value
 */
struct APPLICATION_ERROR_PAIR {
    uint8_t error_code; /**< error code */
    char *msg;  /**< corresponding error message */
}; 
/**
Dictionary struct contiaining error_code as key and error_message as value (see ::APPLICATION_ERROR_PAIR)
 */
typedef struct APPLICATION_ERROR_PAIR APPLICATION_ERROR_PAIR_T;

APPLICATION_ERROR_PAIR_T error_table[] = {
    { ERROR_SUCCESS, "Success"},
    { ERROR_TIMER_CREATE_FAILED, "Timer Creation Failed"},
    { ERROR_TIMER_START_FAILED, "Timer Start Failed"},
    { ERROR_START_ADV_FAILED, "Start Advertising Failed"},
    { ERROR_UNDEFINED, "Undefined Error"},
    { ERROR_NOT_IMPLEMENTED, "Feature Not Implemented"},
    { ERROR_OVERFLOW, "Overflow Error"},
    { ERROR_CONFIG_FAILURE, "Configuration Failure"},
};

void APPLICATION_ErrorHandler(APPLICATION_ERROR_CODE_T errorCode) {
    for (int i=0; i<sizeof(error_table)/sizeof(APPLICATION_ERROR_PAIR_T);i++) {
        if (errorCode == error_table[i].error_code) {
            char buf[128] = {0};
            sprintf(buf,"\r\n[ERROR] %s\r\n", error_table[i].msg);
            SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, buf);
        }
    }
    _data.errorCode = errorCode;
    APPLICATION_STATE_SetInErrorState();
}

void vApplicationDaemonTaskStartupHook( void ) {
    
}

/* *****************************************************************************
 End of File
 */
