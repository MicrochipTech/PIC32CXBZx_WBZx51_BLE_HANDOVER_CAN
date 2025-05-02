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

#include "application_can.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
uint8_t _can0MessageRam[CAN1_MESSAGE_RAM_CONFIG_SIZE] __attribute__((aligned (32)));

/* Standard identifier id[28:18] */
#define WRITE_ID(id) (id << 18)
#define READ_ID(id) (id >> 18)

// variable to save Tx/Rx transfer status and context
static uint32_t status = 0;
static uint32_t xferContext = 0;
// variable to save Tx/Rx message
// static uint8_t userInput = 0;
// variable to save application state
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
    uint8_t txFiFo[CAN1_TX_FIFO_BUFFER_SIZE];
    uint8_t rxFiFo0[CAN1_RX_FIFO0_SIZE];
    uint8_t rxFiFo1[CAN1_RX_FIFO1_SIZE];
    
    CAN_CALLBACK_FUNC_T data_on_rx_fifo_0;
    CAN_CALLBACK_FUNC_T data_on_rx_fifo_1;
    CAN_CALLBACK_FUNC_T tx_complete;
}_data;



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

static void _APPLICATION_CAN1_TxFifoCallback(uintptr_t contextHandle) {
    xferContext = contextHandle;
    /* Check CAN Status */
    status = CAN1_ErrorGet();

    if (((status & CAN_PSR_LEC_Msk) == CAN_ERROR_NONE) || ((status & CAN_PSR_LEC_Msk) == CAN_ERROR_LEC_NC)) {
        switch ((CAN_STATES_T)contextHandle)
        {
            case CAN_STATE_TRANSMIT:
            {
                if (_data.tx_complete) {
                    _data.tx_complete();
                }
                state = CAN_STATE_XFER_SUCCESSFUL;
                break;
            }
            default:
                break;
        }
    } else {
        state = CAN_STATE_XFER_ERROR;
    }
    
    // TODO: ADD IMPLEMENTATION
}

static void _APPLICATION_CAN0_RxFifo0Callback(uint8_t numberOfMessage, uintptr_t contextHandle) {
    xferContext = contextHandle;
    /* Check CAN Status */
    status = CAN1_ErrorGet();
    if (((status & CAN_PSR_LEC_Msk) == CAN_ERROR_NONE) || ((status & CAN_PSR_LEC_Msk) == CAN_ERROR_LEC_NC))
    {
        switch ((CAN_STATES_T)contextHandle)
        {
            case CAN_STATE_RECEIVE:
            {
                memset(_data.rxFiFo0, 0x00, (numberOfMessage * CAN1_RX_FIFO0_ELEMENT_SIZE));
                if (CAN1_MessageReceiveFifo(CAN_RX_FIFO_0, numberOfMessage, (CAN_RX_BUFFER *)_data.rxFiFo0) == true)
                {
                    if(_data.data_on_rx_fifo_0) {
                        _data.data_on_rx_fifo_0();
                    }
                }
                else
                {
                    state = CAN_STATE_XFER_ERROR;
                }
                break;
            }
            default:
                break;
        }
    }
    else
    {
        state = CAN_STATE_XFER_ERROR;
    }
    // TODO: ADD IMPLEMENTATION
}

static void _APPLICATION_CAN0_RxFifo1Callback(uint8_t numberOfMessage, uintptr_t contextHandle) {
    GPIO_PB05_Set();
    xferContext = contextHandle;
    /* Check CAN Status */
    status = CAN1_ErrorGet();
    if (((status & CAN_PSR_LEC_Msk) == CAN_ERROR_NONE) || ((status & CAN_PSR_LEC_Msk) == CAN_ERROR_LEC_NC))
    {
        switch ((CAN_STATES_T)contextHandle)
        {
            case CAN_STATE_RECEIVE:
            {
                memset(_data.rxFiFo1, 0x00, (numberOfMessage * CAN1_RX_FIFO1_ELEMENT_SIZE));
                if (CAN1_MessageReceiveFifo(CAN_RX_FIFO_1, numberOfMessage, (CAN_RX_BUFFER *)_data.rxFiFo1) == true)
                {
                    if (_data.data_on_rx_fifo_1) {
                        _data.data_on_rx_fifo_1();
                    }
                }
                else
                {
                    state = CAN_STATE_XFER_ERROR;
                }
                break;
            }
            default:
                break;
        }
    }
    else
    {
        state = CAN_STATE_XFER_ERROR;
    }
    GPIO_PB05_Clear();
    // TODO: ADD IMPLEMENTATION
}

static uint8_t _APPLICATION_CAN_CanLengthToDlcGet(uint8_t length) {
    // Data Length Code to Message Length
    uint8_t dlc = 0;

    if (length <= 8U)
    {
        dlc = length;
    }
    else if (length <= 12U)
    {
        dlc = 0x9U;
    }
    else if (length <= 16U)
    {
        dlc = 0xAU;
    }
    else if (length <= 20U)
    {
        dlc = 0xBU;
    }
    else if (length <= 24U)
    {
        dlc = 0xCU;
    }
    else if (length <= 32U)
    {
        dlc = 0xDU;
    }
    else if (length <= 48U)
    {
        dlc = 0xEU;
    }
    else
    {
        dlc = 0xFU;
    }
    return dlc;
}


/* Data length code to Message Length */
static uint8_t _APPLICATION_CAN_CanDlcToLengthGet(uint8_t dlc) {
    uint8_t msgLength[] = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};
    return msgLength[dlc];
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void APPLICATION_CAN_Initialize(void) {
    /* Set Message RAM Configuration */
    CAN1_MessageRAMConfigSet(_can0MessageRam);  
    
    /* Register Callback Functions */
    CAN1_TxFifoCallbackRegister(_APPLICATION_CAN1_TxFifoCallback, 0);
    CAN1_RxFifoCallbackRegister(CAN_RX_FIFO_0, _APPLICATION_CAN0_RxFifo0Callback, 0);
    CAN1_RxFifoCallbackRegister(CAN_RX_FIFO_1, _APPLICATION_CAN0_RxFifo1Callback, 0);

    // TODO: ADD IMPLEMENTATION
}

void APPLICATION_CAN_RegisterCallback(CAN_EVENT_T evt, CAN_CALLBACK_FUNC_T func) {
    if (evt == CAN_DATA_ON_RX_FIFO_0) {
        _data.data_on_rx_fifo_0 = func;
    } else if (evt == CAN_DATA_ON_RX_FIFO_1) {
        _data.data_on_rx_fifo_1 = func;
    } else if (evt == CAN_TX_COMPLETE) {
        _data.tx_complete = func;
    } else {
        // Invalid Event
    }
    
    
    
}

void APPLICATION_CAN_Tasks(void) {
    // TODO: ADD IMPLEMENTATION
}

void APPLICATION_CAN_Send(uint16_t id, uint16_t length, uint8_t *p_data) {
    // TODO: ADD IMPLEMENTATION
    while (CAN1_TxBufferIsBusy(0)){
        GPIO_PB06_Toggle();
    };
    
    if (length > CAN1_TX_FIFO_BUFFER_ELEMENT_SIZE) {
        APPLICATION_ErrorHandler(ERROR_CAN);
        return;
    }
    CAN_TX_BUFFER *txBuffer = NULL;
    
    memset(_data.txFiFo, 0x00, CAN1_TX_FIFO_BUFFER_ELEMENT_SIZE);
    txBuffer = (CAN_TX_BUFFER *)_data.txFiFo;
    txBuffer->id = WRITE_ID(id);    // Identifier
    txBuffer->dlc = _APPLICATION_CAN_CanLengthToDlcGet(length); // Data Length Code
    txBuffer->fdf = 1;  // FD Format
    txBuffer->brs = 0; // Bit Rate Switch
    
    memcpy(txBuffer->data, p_data, length);
    GPIO_PB04_Set();
    if (CAN1_MessageTransmitFifo(1,txBuffer) == false) {
        APPLICATION_ErrorHandler(ERROR_CAN);    // FAILED
    } else {
        // PASSED
    }
    GPIO_PB04_Clear();
}

CAN_RX_BUFFER *_APPLICATION_CAN_GetRxFifo(uint8_t fifoId) {
    switch (fifoId) {
        case 0: { return (CAN_RX_BUFFER *)_data.rxFiFo0; }
        case 1: { return (CAN_RX_BUFFER *)_data.rxFiFo1; }
        default: { return NULL; }
    }
}

uint16_t APPLICATION_CAN_GetId(uint8_t fifoId) {
    CAN_RX_BUFFER *pFifo = _APPLICATION_CAN_GetRxFifo(fifoId);
    if (pFifo == NULL) {
        return 0xFFFF;
    }
    return (uint16_t)(pFifo->id >> 18);
    
}
uint8_t APPLICATION_CAN_GetDataLength(uint8_t fifoId) {
    CAN_RX_BUFFER *pFifo = _APPLICATION_CAN_GetRxFifo(fifoId);
    if (pFifo == NULL) {
        return 0;
    }
    return _APPLICATION_CAN_CanDlcToLengthGet(pFifo->dlc);
}
    
uint8_t *APPLICATION_CAN_GetData(uint8_t fifoId) {
    CAN_RX_BUFFER *pFifo = _APPLICATION_CAN_GetRxFifo(fifoId);
    if (pFifo == NULL) {
        return NULL;
    }
    return pFifo->data;
}

/* *****************************************************************************
 End of File
 */
