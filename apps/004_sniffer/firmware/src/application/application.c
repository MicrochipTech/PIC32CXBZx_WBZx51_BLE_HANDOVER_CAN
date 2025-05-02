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

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
uint8_t _can1MessageRam[CAN1_MESSAGE_RAM_CONFIG_SIZE] __attribute__((aligned (32)));
static uint32_t status = 0;
static uint32_t xferContext = 0;

typedef enum {
    CAN_STATE_RECEIVE,
    CAN_STATE_TRANSMIT,
    CAN_STATE_IDLE,
    CAN_STATE_XFER_SUCCESSFUL,
    CAN_STATE_XFER_ERROR,
    CAN_STATE_USER_INPUT,
    NUM_CAN_STATES
} CAN_STATES_T;
volatile static CAN_STATES_T state = CAN_STATE_USER_INPUT;

static struct {
    uint8_t rxFiFo0[CAN1_RX_FIFO0_SIZE];
    QueueHandle_t queue;
}_data;


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
/* Data length code to Message Length */
static uint8_t _APPLICATION_CAN_CanDlcToLengthGet(uint8_t dlc) {
    uint8_t msgLength[] = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};
    return msgLength[dlc];
}

static void _APPLICATION_CAN1_RxFifo0Callback(uint8_t numberOfMessage, uintptr_t contextHandle) {
    LED_On();
    xferContext = contextHandle;
    /*
     * Check CAN Status
     */
    status = CAN1_ErrorGet();
    if (((status & CAN_PSR_LEC_Msk) == CAN_ERROR_NONE) ||((status & CAN_PSR_LEC_Msk) == CAN_ERROR_LEC_NC)) {
        switch ((CAN_STATES_T)contextHandle)
        {
            case CAN_STATE_RECEIVE: {
                memset(_data.rxFiFo0, 0x00, (numberOfMessage * CAN1_RX_FIFO0_ELEMENT_SIZE));
                if (CAN1_MessageReceiveFifo(CAN_RX_FIFO_0, numberOfMessage, (CAN_RX_BUFFER *)_data.rxFiFo0) == true)
                {
                    APP_MSG_T msg;
                    msg.id = APP_MSG_CAN_RXFIFO0_EVENT;
                    memcpy(msg.data,_data.rxFiFo0,CAN1_RX_FIFO0_ELEMENT_SIZE);
                    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
                    xQueueSendFromISR(_data.queue,&msg,&xHigherPriorityTaskWoken);
                }
                else
                {
                    state = CAN_STATE_XFER_ERROR;
                }
                break;                
            }
            default: {
                break;
            }
        }
    } else {
        state = CAN_STATE_XFER_ERROR;
    }
    LED_Off();
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void APPLICATION_Initialize(void) {
    _data.queue = xQueueCreate(32, sizeof(APP_MSG_T));
    /* 
     * Set Message RAM Configuration 
     */
    CAN1_MessageRAMConfigSet(_can1MessageRam);  
    
    /*
     * Register Callback Functions
     */
    CAN1_RxFifoCallbackRegister(CAN_RX_FIFO_0, _APPLICATION_CAN1_RxFifo0Callback, 0);
    
}

void APPLICATION_Tasks(void) {
    APP_MSG_T msg;
    if ( xQueueReceive ( _data.queue, &msg, 0)) {
        
        CAN_RX_BUFFER *p_fifo = (CAN_RX_BUFFER *)msg.data;
        uint32_t id = p_fifo->id >> 18;
        uint8_t len = _APPLICATION_CAN_CanDlcToLengthGet(p_fifo->dlc);
        char buffer[256] = {0};
        int offset = 0;
        for ( int i=0;i<len;i++) {
            offset += sprintf(buffer+offset,"%02X ",p_fifo->data[i]);
        }
        SYS_CONSOLE_PRINT("[%04X] %s\r\n",id,buffer);
    }
}


/* *****************************************************************************
 End of File
 */
