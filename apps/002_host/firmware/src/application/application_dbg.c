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

#include "application_dbg.h"
#include "application_cmd.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
    /**
    Debug Data structure
    */
    typedef struct {
        union {
            struct {
                uint32_t    command_0 : 1,  /**< Bit 00: [DBG] COMMAND 0 */
                            command_1 : 1,  /**< Bit 01: [EVT] COMMAND 1 */
                            command_2 : 1,  /**< Bit 02: [EVT] COMMAND 2 */
                            rfu : 29;   /**< RFU  */
            };
            uint32_t v; /**< command flags */
        }commands;  /**< command flags */
    }DBG_DATA_T;
    /**
    Debug data structure (see ::DBG_DATA_T)
     */
    static DBG_DATA_T _data;


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

static void _APPLICATION_DBG_ReadCallback(SERCOM_USART_EVENT event, uintptr_t context) {
    switch (event) {
        case SERCOM_USART_EVENT_READ_THRESHOLD_REACHED: 
        { 
            /* Threshold number of bytes are available in the receive ring buffer */
            uint8_t rcvd;
            SERCOM2_USART_Read(&rcvd, 1);
            switch (rcvd) {
                case '0': {_data.commands.command_0 = true; break;}
                case '1': {_data.commands.command_1 = true; break;}
                case '2': {_data.commands.command_2 = true; break;}
                default: break;
            }
            
            // SERCOM1_USART_Write(data,1);
            break; 
        }
        case SERCOM_USART_EVENT_READ_BUFFER_FULL: 
        { 
            /* Receive ring buffer is full. Application must read the data out to avoid missing data on the next RX interrupt. */
            break; 
        }
        case SERCOM_USART_EVENT_READ_ERROR: 
        { 
            /* USART error. Application must call the SERCOMx_USART_ErrorGet API to get the type of error and clear the error. */
            break; 
        }
        case SERCOM_USART_EVENT_WRITE_THRESHOLD_REACHED: 
        { 
            /* Threshold number of free space is available in the transmit ring buffer */
            break; 
        }
        case SERCOM_USART_EVENT_BREAK_SIGNAL_DETECTED: 
        { 
            /* Recevie break signal is detected */
            break; 
        }
        default: 
        { 
            break; 
        }
    }
}



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void APPLICATION_DBG_Initialize(void) {
    
    _data.commands.v = 0;
    
    SERCOM2_USART_ReadCallbackRegister(_APPLICATION_DBG_ReadCallback, 0);
    SERCOM2_USART_ReadNotificationEnable(true,true);
    SERCOM2_USART_ReadThresholdSet(1);
    
    char *usage = "\r\n"
                    "(0) Debug Command 0\r\n"
                    "(1) Debug Command 1\r\n"
                    "(2) Debug Command 2\r\n";
    
    APPLICATION_DBG_Write((uint8_t *)usage, strlen(usage));
}

void APPLICATION_DBG_Tasks(void) {
    if (_data.commands.command_0){
        _data.commands.command_0 = false;
        char *msg="Command 0\r\n";
        APPLICATION_DBG_Write((uint8_t *)msg, strlen(msg));
#if false
// --M43814--remove-- 
        APPLICATION_CMD_VendorSpecificResetNode(0x45B);
#endif
    }
    if (_data.commands.command_1){
        _data.commands.command_1 = false;
        char *msg="Command 1\r\n";
        APPLICATION_DBG_Write((uint8_t *)msg, strlen(msg));
#if false
// --M43814--remove-- 
        APPLICATION_CMD_Reset(0x45B);
#endif
    }
    if (_data.commands.command_2){
        _data.commands.command_2 = false;
        char *msg="Command 2\r\n";
        APPLICATION_DBG_Write((uint8_t *)msg, strlen(msg));
         __NVIC_SystemReset();
    }
    // TODO: ADD IMPLEMENTATION
}

size_t APPLICATION_DBG_Write(uint8_t* pWrBuffer, const size_t size ) {
    return SERCOM2_USART_Write(pWrBuffer, size);
}
/*
    char buf[32] = {0};
    size_t len = sprintf(buf, "\r\n%04lX\r\n",data.events.v);
    SERCOM1_USART_Write(buf, len);
    while ( !SERCOM1_USART_TransmitComplete() );
 */

/* *****************************************************************************
 End of File
 */
