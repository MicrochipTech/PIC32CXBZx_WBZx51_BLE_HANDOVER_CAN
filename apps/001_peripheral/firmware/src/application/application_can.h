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

/** \file application_can.h
  CAN application defines file
 */

#ifndef _APPLICATION_CAN_H    /* Guard against multiple inclusion */
#define _APPLICATION_CAN_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "application.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
#define CRCBASE    0xFFFF   /**< CRC Polynomial Init */
#define CRCUPPER   1    /**< Index of CRC Polynomial Upper Byte */

#define SPI_DEFAULT_BUFFER_LENGTH       96  /**< SPI buffer length */

#define CANFDSPI_MODULE_ID          DRV_HANDLE  /**< Device Handle - Handle to opened device driver */
    
// <editor-fold defaultstate="collapsed" desc="Section: Implementation">

// Device selection
//#define MCP2517FD
#define MCP2518FD   /**< MCP2518FD Device selection*/

// Maximum Size of TX/RX Object
#define MAX_MSG_SIZE 76 /**< Maximum Size of TX/RX Object */

// Maximum number of data bytes in message
#define MAX_DATA_BYTES          64  /**< Maximum number of data bytes in message */
#define MAX_RAM_SIZE            2048    /**< Maximum RAM size  */
// </editor-fold>
    
// <editor-fold defaultstate="collapsed" desc="Section: Object definitions">
//! CAN FIFO Channels

typedef enum {
    CAN_FIFO_CH0,   // CAN_TXQUEUE_CH0
    CAN_FIFO_CH1,
    CAN_FIFO_CH2,
    CAN_FIFO_CH3,
    CAN_FIFO_CH4,
    CAN_FIFO_CH5,
    CAN_FIFO_CH6,
    CAN_FIFO_CH7,
    CAN_FIFO_CH8,
    CAN_FIFO_CH9,
    CAN_FIFO_CH10,
    CAN_FIFO_CH11,
    CAN_FIFO_CH12,
    CAN_FIFO_CH13,
    CAN_FIFO_CH14,
    CAN_FIFO_CH15,
    CAN_FIFO_CH16,
    CAN_FIFO_CH17,
    CAN_FIFO_CH18,
    CAN_FIFO_CH19,
    CAN_FIFO_CH20,
    CAN_FIFO_CH21,
    CAN_FIFO_CH22,
    CAN_FIFO_CH23,
    CAN_FIFO_CH24,
    CAN_FIFO_CH25,
    CAN_FIFO_CH26,
    CAN_FIFO_CH27,
    CAN_FIFO_CH28,
    CAN_FIFO_CH29,
    CAN_FIFO_CH30,
    CAN_FIFO_CH31,
    CAN_FIFO_TOTAL_CHANNELS
} CAN_FIFO_CHANNEL;

// FIFO0 is a special FIFO, the TX Queue
#define CAN_TXQUEUE_CH0 CAN_FIFO_CH0    /**< FIFO0 is a special FIFO, the TX Queue */

//! CAN Filter Channels

typedef enum {
    CAN_FILTER0,
    CAN_FILTER1,
    CAN_FILTER2,
    CAN_FILTER3,
    CAN_FILTER4,
    CAN_FILTER5,
    CAN_FILTER6,
    CAN_FILTER7,
    CAN_FILTER8,
    CAN_FILTER9,
    CAN_FILTER10,
    CAN_FILTER11,
    CAN_FILTER12,
    CAN_FILTER13,
    CAN_FILTER14,
    CAN_FILTER15,
    CAN_FILTER16,
    CAN_FILTER17,
    CAN_FILTER18,
    CAN_FILTER19,
    CAN_FILTER20,
    CAN_FILTER21,
    CAN_FILTER22,
    CAN_FILTER23,
    CAN_FILTER24,
    CAN_FILTER25,
    CAN_FILTER26,
    CAN_FILTER27,
    CAN_FILTER28,
    CAN_FILTER29,
    CAN_FILTER30,
    CAN_FILTER31,
    CAN_FILTER_TOTAL,
} CAN_FILTER;


//! CAN Operation Modes

typedef enum {
    CAN_NORMAL_MODE = 0x00,
    CAN_SLEEP_MODE = 0x01,
    CAN_INTERNAL_LOOPBACK_MODE = 0x02,
    CAN_LISTEN_ONLY_MODE = 0x03,
    CAN_CONFIGURATION_MODE = 0x04,
    CAN_EXTERNAL_LOOPBACK_MODE = 0x05,
    CAN_CLASSIC_MODE = 0x06,
    CAN_RESTRICTED_MODE = 0x07,
    CAN_INVALID_MODE = 0xFF
} CAN_OPERATION_MODE;

//! Transmit Bandwidth Sharing

typedef enum {
    CAN_TXBWS_NO_DELAY,
    CAN_TXBWS_2,
    CAN_TXBWS_4,
    CAN_TXBWS_8,
    CAN_TXBWS_16,
    CAN_TXBWS_32,
    CAN_TXBWS_64,
    CAN_TXBWS_128,
    CAN_TXBWS_256,
    CAN_TXBWS_512,
    CAN_TXBWS_1024,
    CAN_TXBWS_2048,
    CAN_TXBWS_4096
} CAN_TX_BANDWITH_SHARING;

//! Wake-up Filter Time

typedef enum {
    CAN_WFT00,
    CAN_WFT01,
    CAN_WFT10,
    CAN_WFT11
} CAN_WAKEUP_FILTER_TIME;

//! Data Byte Filter Number

typedef enum {
    CAN_DNET_FILTER_DISABLE = 0,
    CAN_DNET_FILTER_SIZE_1_BIT,
    CAN_DNET_FILTER_SIZE_2_BIT,
    CAN_DNET_FILTER_SIZE_3_BIT,
    CAN_DNET_FILTER_SIZE_4_BIT,
    CAN_DNET_FILTER_SIZE_5_BIT,
    CAN_DNET_FILTER_SIZE_6_BIT,
    CAN_DNET_FILTER_SIZE_7_BIT,
    CAN_DNET_FILTER_SIZE_8_BIT,
    CAN_DNET_FILTER_SIZE_9_BIT,
    CAN_DNET_FILTER_SIZE_10_BIT,
    CAN_DNET_FILTER_SIZE_11_BIT,
    CAN_DNET_FILTER_SIZE_12_BIT,
    CAN_DNET_FILTER_SIZE_13_BIT,
    CAN_DNET_FILTER_SIZE_14_BIT,
    CAN_DNET_FILTER_SIZE_15_BIT,
    CAN_DNET_FILTER_SIZE_16_BIT,
    CAN_DNET_FILTER_SIZE_17_BIT,
    CAN_DNET_FILTER_SIZE_18_BIT
} CAN_DNET_FILTER_SIZE;

//! FIFO Payload Size

typedef enum {
    CAN_PLSIZE_8,
    CAN_PLSIZE_12,
    CAN_PLSIZE_16,
    CAN_PLSIZE_20,
    CAN_PLSIZE_24,
    CAN_PLSIZE_32,
    CAN_PLSIZE_48,
    CAN_PLSIZE_64
} CAN_FIFO_PLSIZE;

/**
CAN Configure
 */ 
typedef struct _CAN_CONFIG {
    uint32_t DNetFilterCount : 5;   /**< Device Net Filter Bit Number bits */
    uint32_t IsoCrcEnable : 1;  /**< Enable ISO CRC in CAN FD Frames bit */
    uint32_t ProtocolExpectionEventDisable : 1; /**< Protocol Exception Event Detection Disabled bit */
    uint32_t WakeUpFilterEnable : 1;    /**< Enable CAN Bus Line Wake-up Filter bit */
    uint32_t WakeUpFilterTime : 2;  /**< Selectable Wake-up Filter Time bits */
    uint32_t BitRateSwitchDisable : 1;  /**< Bit Rate Switching Disable bit */
    uint32_t RestrictReTxAttempts : 1;  /**< Restrict Retransmission Attempts bit */
    uint32_t EsiInGatewayMode : 1;  /**< Transmit ESI in Gateway Mode bit */
    uint32_t SystemErrorToListenOnly : 1;   /**< Transition to Listen Only Mode on System Error bit */
    uint32_t StoreInTEF : 1;    /**< Store in Transmit Event FIFO bit */
    uint32_t TXQEnable : 1; /**< Enable Transmit Queue bit */
    uint32_t TxBandWidthSharing : 4;    /**< Transmit Bandwidth Sharing bits */
} CAN_CONFIG;

/** 
CAN Transmit Channel Configure
 */
typedef struct _CAN_TX_FIFO_CONFIG {
    uint32_t RTREnable : 1; /**< Auto RTR Enable bit  */
    uint32_t TxPriority : 5;    /**< Message Transmit Priority bits */
    uint32_t TxAttempts : 2;    /**< Retransmission Attempts bits */
    uint32_t FifoSize : 5;  /**< FIFO Size bits */
    uint32_t PayLoadSize : 3;   /**< Payload Size bits */
} CAN_TX_FIFO_CONFIG;

/** 
CAN Transmit Queue Configure
 */
typedef struct _CAN_TX_QUEUE_CONFIG {
    uint32_t TxPriority : 5;    /**< Message Transmit Priority bits */
    uint32_t TxAttempts : 2;    /**< Retransmission Attemptx bits */
    uint32_t FifoSize : 5;      /**< FIFO Size bits */
    uint32_t PayLoadSize : 3;   /**< Payload Size bits  */
} CAN_TX_QUEUE_CONFIG;

/**
CAN Receive Channel Configure
 */ 
typedef struct _CAN_RX_FIFO_CONFIG {
    uint32_t RxTimeStampEnable : 1; /**< Received Message Time Stamp Enable bit */
    uint32_t FifoSize : 5;  /**< FIFO Size bits */
    uint32_t PayLoadSize : 3;   /**< Payload Size bits */
} CAN_RX_FIFO_CONFIG;

/**
CAN Transmit Event FIFO Configure
 */ 
typedef struct _CAN_TEF_CONFIG {
    uint32_t TimeStampEnable : 1;   /**< Transmit Event FIFO Time Stamp Enable bit */
    uint32_t FifoSize : 5;  /**< FIFO Size bits */
} CAN_TEF_CONFIG;

/* CAN Message Objects */

/**
CAN Message Object ID
 */ 
typedef struct _CAN_MSGOBJ_ID {
    uint32_t SID : 11;  /**< Standard Identifier */
    uint32_t EID : 18;  /**< Extended Identifier */
    uint32_t SID11 : 1; /**< In FD mode the standard ID can be extended to 12 bit using r1 */
    uint32_t unimplemented1 : 2;    /**< rfu */
} CAN_MSGOBJ_ID;

//! CAN Data Length Code

typedef enum {
    CAN_DLC_0,
    CAN_DLC_1,
    CAN_DLC_2,
    CAN_DLC_3,
    CAN_DLC_4,
    CAN_DLC_5,
    CAN_DLC_6,
    CAN_DLC_7,
    CAN_DLC_8,
    CAN_DLC_12,
    CAN_DLC_16,
    CAN_DLC_20,
    CAN_DLC_24,
    CAN_DLC_32,
    CAN_DLC_48,
    CAN_DLC_64
} CAN_DLC;

/**
CAN TX Message Object Control
 */ 
typedef struct _CAN_TX_MSGOBJ_CTRL {
    uint32_t DLC : 4;   /**< Data Length Code */
    uint32_t IDE : 1;   /**< Identifier Extension Flag; distinguishes between base and extended format */
    uint32_t RTR : 1;   /**< Remote Transmission Request; not used in CAN FD */
    uint32_t BRS : 1;   /**< Bit Rate Switch; selects if data bit rate is switched */
    uint32_t FDF : 1;   /**< FD Frame; distinguishes between CAN and CAN FD formats */
    uint32_t ESI : 1;   /**< Error Status Indicator */
#ifdef MCP2517FD
    uint32_t SEQ : 7;
    uint32_t unimplemented1 : 16;
#else
    uint32_t SEQ : 23;  /**< Sequenc to keep track of transmitted messages in Transmit Event FIFO */
#endif
} CAN_TX_MSGOBJ_CTRL;

/** 
CAN RX Message Object Control
 */
typedef struct _CAN_RX_MSGOBJ_CTRL {
    uint32_t DLC : 4;   /**< Data Length Code */
    uint32_t IDE : 1;   /**< Identifier Extension Flag */
    uint32_t RTR : 1;   /**< Remote Transmission Request, not used in CAN FD */
    uint32_t BRS : 1;   /**< Bit Rate Switch; indicates if data bit rate was switched */
    uint32_t FDF : 1;   /**< FD Frame; distinguishes between CAN adn CAN FD formats */
    uint32_t ESI : 1;   /**< Error Status Indicator */
    uint32_t unimplemented1 : 2;    /**< rfu */
    uint32_t FilterHit : 5; /**< Filter Hit, number of filter that matched */
    uint32_t unimplemented2 : 16;   /**< rfu */
} CAN_RX_MSGOBJ_CTRL;

/**
CAN Message Time Stamp
 */ 
typedef uint32_t CAN_MSG_TIMESTAMP;

/**
CAN TX Message Object
 */ 
typedef union _CAN_TX_MSGOBJ {

    struct {
        CAN_MSGOBJ_ID id;   /**< CAN Message Object ID */
        CAN_TX_MSGOBJ_CTRL ctrl;    /**< CAN TX Message Object Control */
        CAN_MSG_TIMESTAMP timeStamp;    /**< CAN Message Time Stamp */
    } bF;   /**< CAN TX Message Object bitfield */
    uint32_t word[3];   /**< CAN TX Message Object 32-bit word array */
    uint8_t byte[12];   /**< CAN TX Message Object 8-bit word array */
} CAN_TX_MSGOBJ;

/**
CAN RX Message Object
 */ 
typedef union _CAN_RX_MSGOBJ {

    struct {
        CAN_MSGOBJ_ID id;   /**< CAN Message Object ID */
        CAN_RX_MSGOBJ_CTRL ctrl;    /**< CAN RX Message Object Control */
        CAN_MSG_TIMESTAMP timeStamp;    /**< CAN Message Time Stamp */
    } bF;   /**< CAN RX Message Object bitfield */
    uint32_t word[3];   /**< CAN RX Message Object 32-bit word array */
    uint8_t byte[12];   /**< CAN RX Message Object 8-bit byte array */
} CAN_RX_MSGOBJ;

/**
CAN TEF Message Object
 */ 
typedef union _CAN_TEF_MSGOBJ {

    struct {
        CAN_MSGOBJ_ID id;   /**< CAN Message Object ID */
        CAN_TX_MSGOBJ_CTRL ctrl;    /**< CAN TX Message Object Control */
        CAN_MSG_TIMESTAMP timeStamp;        /**< CAN Message Time Stamp */
    } bF;   /**< CAN TEF Message Object bitfield */
    uint32_t word[3];   /**< CAN TEF Message Object 32-bit word array */
    uint8_t byte[12];   /**< CAN TEF Message Object 8-bit byte array */
} CAN_TEF_MSGOBJ;

/** 
CAN Filter Object ID
 */
typedef struct _CAN_FILTEROBJ_ID {
    uint32_t SID : 11;  /**< Standard Identifier filter bits */
    uint32_t EID : 18;  /**< Extended Identifier filter bits */
    uint32_t SID11 : 1; /**< Standard Identiifer filter bit */
    uint32_t EXIDE : 1; /**< Extended Identifier Enable bit */
    uint32_t unimplemented1 : 1;    /**< rfu */
} CAN_FILTEROBJ_ID;

/**
CAN Mask Object ID
 */

typedef struct _CAN_MASKOBJ_ID {
    uint32_t MSID : 11; /**< Standard Identifier Mask bits */
    uint32_t MEID : 18; /**< Extended Identifier Mask bits */
    uint32_t MSID11 : 1;    /**< Standard Identifier Mask bit */
    uint32_t MIDE : 1;  /**< Identifier Receive mode bit */
    uint32_t unimplemented1 : 1;    /**< rfu */
} CAN_MASKOBJ_ID;

//! CAN RX FIFO Status

typedef enum {
    CAN_RX_FIFO_EMPTY = 0,
    CAN_RX_FIFO_STATUS_MASK = 0x0F,
    CAN_RX_FIFO_NOT_EMPTY = 0x01,
    CAN_RX_FIFO_HALF_FULL = 0x02,
    CAN_RX_FIFO_FULL = 0x04,
    CAN_RX_FIFO_OVERFLOW = 0x08
} CAN_RX_FIFO_STATUS;

//! CAN TX FIFO Status

typedef enum {
    CAN_TX_FIFO_FULL = 0,
    CAN_TX_FIFO_STATUS_MASK = 0x1F7,
    CAN_TX_FIFO_NOT_FULL = 0x01,
    CAN_TX_FIFO_HALF_FULL = 0x02,
    CAN_TX_FIFO_EMPTY = 0x04,
    CAN_TX_FIFO_ATTEMPTS_EXHAUSTED = 0x10,
    CAN_TX_FIFO_ERROR = 0x20,
    CAN_TX_FIFO_ARBITRATION_LOST = 0x40,
    CAN_TX_FIFO_ABORTED = 0x80,
    CAN_TX_FIFO_TRANSMITTING = 0x100
} CAN_TX_FIFO_STATUS;

//! CAN TEF FIFO Status

typedef enum {
    CAN_TEF_FIFO_EMPTY = 0,
    CAN_TEF_FIFO_STATUS_MASK = 0x0F,
    CAN_TEF_FIFO_NOT_EMPTY = 0x01,
    CAN_TEF_FIFO_HALF_FULL = 0x02,
    CAN_TEF_FIFO_FULL = 0x04,
    CAN_TEF_FIFO_OVERFLOW = 0x08
} CAN_TEF_FIFO_STATUS;

//! CAN Module Event (Interrupts)

typedef enum {
    CAN_NO_EVENT = 0,
    CAN_ALL_EVENTS = 0xFF1F,
    CAN_TX_EVENT = 0x0001,
    CAN_RX_EVENT = 0x0002,
    CAN_TIME_BASE_COUNTER_EVENT = 0x0004,
    CAN_OPERATION_MODE_CHANGE_EVENT = 0x0008,
    CAN_TEF_EVENT = 0x0010,

    CAN_RAM_ECC_EVENT = 0x0100,
    CAN_SPI_CRC_EVENT = 0x0200,
    CAN_TX_ATTEMPTS_EVENT = 0x0400,
    CAN_RX_OVERFLOW_EVENT = 0x0800,
    CAN_SYSTEM_ERROR_EVENT = 0x1000,
    CAN_BUS_ERROR_EVENT = 0x2000,
    CAN_BUS_WAKEUP_EVENT = 0x4000,
    CAN_RX_INVALID_MESSAGE_EVENT = 0x8000
} CAN_MODULE_EVENT;

//! CAN TX FIFO Event (Interrupts)

typedef enum {
    CAN_TX_FIFO_NO_EVENT = 0,
    CAN_TX_FIFO_ALL_EVENTS = 0x17,
    CAN_TX_FIFO_NOT_FULL_EVENT = 0x01,
    CAN_TX_FIFO_HALF_FULL_EVENT = 0x02,
    CAN_TX_FIFO_EMPTY_EVENT = 0x04,
    CAN_TX_FIFO_ATTEMPTS_EXHAUSTED_EVENT = 0x10
} CAN_TX_FIFO_EVENT;

//! CAN RX FIFO Event (Interrupts)

typedef enum {
    CAN_RX_FIFO_NO_EVENT = 0,
    CAN_RX_FIFO_ALL_EVENTS = 0x0F,
    CAN_RX_FIFO_NOT_EMPTY_EVENT = 0x01,
    CAN_RX_FIFO_HALF_FULL_EVENT = 0x02,
    CAN_RX_FIFO_FULL_EVENT = 0x04,
    CAN_RX_FIFO_OVERFLOW_EVENT = 0x08
} CAN_RX_FIFO_EVENT;

//! CAN TEF FIFO Event (Interrupts)

typedef enum {
    CAN_TEF_FIFO_NO_EVENT = 0,
    CAN_TEF_FIFO_ALL_EVENTS = 0x0F,
    CAN_TEF_FIFO_NOT_EMPTY_EVENT = 0x01,
    CAN_TEF_FIFO_HALF_FULL_EVENT = 0x02,
    CAN_TEF_FIFO_FULL_EVENT = 0x04,
    CAN_TEF_FIFO_OVERFLOW_EVENT = 0x08
} CAN_TEF_FIFO_EVENT;

//! CAN Bit Time Setup: Arbitration/Data Bit Phase

typedef enum {
    CAN_500K_1M, // 0x00
    CAN_500K_2M, // 0x01
    CAN_500K_3M,
    CAN_500K_4M,
    CAN_500K_5M, // 0x04
    CAN_500K_6M7,
    CAN_500K_8M, // 0x06
    CAN_500K_10M,
    CAN_250K_500K, // 0x08
    CAN_250K_833K,
    CAN_250K_1M,
    CAN_250K_1M5,
    CAN_250K_2M,
    CAN_250K_3M,
    CAN_250K_4M,
    CAN_1000K_4M, // 0x0f
    CAN_1000K_8M,
    CAN_125K_500K // 0x11
} CAN_BITTIME_SETUP;

//! CAN Nominal Bit Time Setup

typedef enum {
    CAN_NBT_125K,
    CAN_NBT_250K,
    CAN_NBT_500K,
    CAN_NBT_1M
} CAN_NOMINAL_BITTIME_SETUP;

//! CAN Data Bit Time Setup

typedef enum {
    CAN_DBT_500K,
    CAN_DBT_833K,
    CAN_DBT_1M,
    CAN_DBT_1M5,
    CAN_DBT_2M,
    CAN_DBT_3M,
    CAN_DBT_4M,
    CAN_DBT_5M,
    CAN_DBT_6M7,
    CAN_DBT_8M,
    CAN_DBT_10M
} CAN_DATA_BITTIME_SETUP;

//! Secondary Sample Point Mode

typedef enum {
    CAN_SSP_MODE_OFF,
    CAN_SSP_MODE_MANUAL,
    CAN_SSP_MODE_AUTO
} CAN_SSP_MODE;

//! CAN Error State

typedef enum {
    CAN_ERROR_FREE_STATE = 0,
    CAN_ERROR_ALL = 0x3F,
    CAN_TX_RX_WARNING_STATE = 0x01,
    CAN_RX_WARNING_STATE = 0x02,
    CAN_TX_WARNING_STATE = 0x04,
    CAN_RX_BUS_PASSIVE_STATE = 0x08,
    CAN_TX_BUS_PASSIVE_STATE = 0x10,
    CAN_TX_BUS_OFF_STATE = 0x20
} CAN_ERROR_STATE;

//! CAN Time Stamp Mode Select

typedef enum {
    CAN_TS_SOF = 0x00,
    CAN_TS_EOF = 0x01,
    CAN_TS_RES = 0x02
} CAN_TS_MODE;

//! CAN ECC EVENT

typedef enum {
    CAN_ECC_NO_EVENT = 0x00,
    CAN_ECC_ALL_EVENTS = 0x06,
    CAN_ECC_SEC_EVENT = 0x02,
    CAN_ECC_DED_EVENT = 0x04
} CAN_ECC_EVENT;

//! CAN CRC EVENT

typedef enum {
    CAN_CRC_NO_EVENT = 0x00,
    CAN_CRC_ALL_EVENTS = 0x03,
    CAN_CRC_CRCERR_EVENT = 0x01,
    CAN_CRC_FORMERR_EVENT = 0x02
} CAN_CRC_EVENT;

//! GPIO Pin Position

typedef enum {
    GPIO_PIN_0,
    GPIO_PIN_1
} GPIO_PIN_POS;

//! GPIO Pin Modes

typedef enum {
    GPIO_MODE_INT,
    GPIO_MODE_GPIO
} GPIO_PIN_MODE;

//! GPIO Pin Directions

typedef enum {
    GPIO_OUTPUT,
    GPIO_INPUT
} GPIO_PIN_DIRECTION;

//! GPIO Open Drain Mode

typedef enum {
    GPIO_PUSH_PULL,
    GPIO_OPEN_DRAIN
} GPIO_OPEN_DRAIN_MODE;

//! GPIO Pin State

typedef enum {
    GPIO_LOW,
    GPIO_HIGH
} GPIO_PIN_STATE;

//! Clock Output Mode

typedef enum {
    GPIO_CLKO_CLOCK,
    GPIO_CLKO_SOF
} GPIO_CLKO_MODE;

/**
CAN Bus Diagnostic flags
 */ 
typedef struct _CAN_BUS_DIAG_FLAGS {
    uint32_t NBIT0_ERR : 1; /**< During the transmission of a message the device wanted to send a dominant level, but the monitored bus value was recessive. */
    uint32_t NBIT1_ERR : 1; /**< During the transmission of a message the device wanted to send a recessive level, but the monitored bus value was dominant. */
    uint32_t NACK_ERR : 1; /**< Transmitted message was not acknowledged */
    uint32_t NFORM_ERR : 1; /**< A fixed format part of a received frames has the wrong format */
    uint32_t NSTUFF_ERR : 1; /**< More than 5 equal bits in a sequence have occured in a part of a received message where this is not allowed. */
    uint32_t NCRC_ERR : 1; /**< The CRC check sum of a received message was incorrect */
    uint32_t unimplemented1 : 1; /**< rfu */
    uint32_t TXBO_ERR : 1; /**< Device went to bus-off (and auto-recovered) */
    uint32_t DBIT0_ERR : 1; /**< same as ::NBIT0_ERR */
    uint32_t DBIT1_ERR : 1; /**< same as ::NBIT1_ERR */
    uint32_t unimplemented2 : 1; /**< rfu */
    uint32_t DFORM_ERR : 1; /**< same as ::NFORM_ERR */
    uint32_t DSTUFF_ERR : 1; /**< same as ::NSTUFF_ERR */
    uint32_t DCRC_ERR : 1; /**< same as ::NCRC_ERR */
    uint32_t ESI : 1; /**< ESI flag of a received CAN FD message was set. */
    uint32_t DLC_MISMATCH : 1; /**< DLC Mismatch bit */
} CAN_BUS_DIAG_FLAGS;

/** 
CAN Bus Diagnostic Error Counts
*/ 
typedef struct _CAN_BUS_ERROR_COUNT {
    uint8_t NREC;   /**< Receive Error Counter bits */
    uint8_t NTEC;   /**< Transmit Error Counter bits */
    uint8_t DREC;   /**< Receive Error Counter bits */
    uint8_t DTEC;   /**< Transmit Error Counter bits */
} CAN_BUS_ERROR_COUNT;

/**
CAN BUS DIAGNOSTICS
 */ 
typedef union _CAN_BUS_DIAGNOSTIC {

    struct {
        CAN_BUS_ERROR_COUNT errorCount; /**< CAN Bus diagnostic error counts */
        uint16_t errorFreeMsgCount; /**< Error Free Mesage Counter bits */
        CAN_BUS_DIAG_FLAGS flag;    /**< CAN Bus diagnostic flags */
    } bF;   /**< CAN bus diagnostic bit field */
    uint32_t word[2];   /**< CAN bus diagnostic 32-bit word array */
    uint8_t byte[8];    /**< CAN bus diagnostic 8-bit byte array */
} CAN_BUS_DIAGNOSTIC;

//! TXREQ Channel Bits
// Multiple channels can be or'ed together

typedef enum {
    CAN_TXREQ_CH0 = 0x00000001,
    CAN_TXREQ_CH1 = 0x00000002,
    CAN_TXREQ_CH2 = 0x00000004,
    CAN_TXREQ_CH3 = 0x00000008,
    CAN_TXREQ_CH4 = 0x00000010,
    CAN_TXREQ_CH5 = 0x00000020,
    CAN_TXREQ_CH6 = 0x00000040,
    CAN_TXREQ_CH7 = 0x00000080,

    CAN_TXREQ_CH8 = 0x00000100,
    CAN_TXREQ_CH9 = 0x00000200,
    CAN_TXREQ_CH10 = 0x00000400,
    CAN_TXREQ_CH11 = 0x00000800,
    CAN_TXREQ_CH12 = 0x00001000,
    CAN_TXREQ_CH13 = 0x00002000,
    CAN_TXREQ_CH14 = 0x00004000,
    CAN_TXREQ_CH15 = 0x00008000,

    CAN_TXREQ_CH16 = 0x00010000,
    CAN_TXREQ_CH17 = 0x00020000,
    CAN_TXREQ_CH18 = 0x00040000,
    CAN_TXREQ_CH19 = 0x00080000,
    CAN_TXREQ_CH20 = 0x00100000,
    CAN_TXREQ_CH21 = 0x00200000,
    CAN_TXREQ_CH22 = 0x00400000,
    CAN_TXREQ_CH23 = 0x00800000,

    CAN_TXREQ_CH24 = 0x01000000,
    CAN_TXREQ_CH25 = 0x02000000,
    CAN_TXREQ_CH26 = 0x04000000,
    CAN_TXREQ_CH27 = 0x08000000,
    CAN_TXREQ_CH28 = 0x10000000,
    CAN_TXREQ_CH29 = 0x20000000,
    CAN_TXREQ_CH30 = 0x40000000,
    CAN_TXREQ_CH31 = 0x80000000
} CAN_TXREQ_CHANNEL;

/**
Oscillator Control
 */ 
typedef struct _CAN_OSC_CTRL {
    uint32_t PllEnable : 1; /**< PLL enable */
    uint32_t OscDisable : 1;    /**< Clock (Oscillator) Disable */
    uint32_t SclkDivide : 1;    /**< System Clock Divisor */
    uint32_t ClkOutDivide : 2;  /**< Clock Output Divisor */
#ifndef MCP2517FD
    uint32_t LowPowerModeEnable : 1;    /**< Low Power Mode(LPM) Enable */
#endif
} CAN_OSC_CTRL;

/**
Oscillator Status
 */ 
typedef struct _CAN_OSC_STATUS {
    uint32_t PllReady : 1;  /**< PLL Ready */
    uint32_t OscReady : 1;  /**< Clock Ready */
    uint32_t SclkReady : 1; /**< Synchronized SCLKDIV bit */
} CAN_OSC_STATUS;

//! ICODE

typedef enum {
    CAN_ICODE_FIFO_CH0,
    CAN_ICODE_FIFO_CH1,
    CAN_ICODE_FIFO_CH2,
    CAN_ICODE_FIFO_CH3,
    CAN_ICODE_FIFO_CH4,
    CAN_ICODE_FIFO_CH5,
    CAN_ICODE_FIFO_CH6,
    CAN_ICODE_FIFO_CH7,
    CAN_ICODE_FIFO_CH8,
    CAN_ICODE_FIFO_CH9,
    CAN_ICODE_FIFO_CH10,
    CAN_ICODE_FIFO_CH11,
    CAN_ICODE_FIFO_CH12,
    CAN_ICODE_FIFO_CH13,
    CAN_ICODE_FIFO_CH14,
    CAN_ICODE_FIFO_CH15,
    CAN_ICODE_FIFO_CH16,
    CAN_ICODE_FIFO_CH17,
    CAN_ICODE_FIFO_CH18,
    CAN_ICODE_FIFO_CH19,
    CAN_ICODE_FIFO_CH20,
    CAN_ICODE_FIFO_CH21,
    CAN_ICODE_FIFO_CH22,
    CAN_ICODE_FIFO_CH23,
    CAN_ICODE_FIFO_CH24,
    CAN_ICODE_FIFO_CH25,
    CAN_ICODE_FIFO_CH26,
    CAN_ICODE_FIFO_CH27,
    CAN_ICODE_FIFO_CH28,
    CAN_ICODE_FIFO_CH29,
    CAN_ICODE_FIFO_CH30,
    CAN_ICODE_FIFO_CH31,
    CAN_ICODE_TOTAL_CHANNELS,
    CAN_ICODE_NO_INT = 0x40,
    CAN_ICODE_CERRIF,
    CAN_ICODE_WAKIF,
    CAN_ICODE_RXOVIF,
    CAN_ICODE_ADDRERR_SERRIF,
    CAN_ICODE_MABOV_SERRIF,
    CAN_ICODE_TBCIF,
    CAN_ICODE_MODIF,
    CAN_ICODE_IVMIF,
    CAN_ICODE_TEFIF,
    CAN_ICODE_TXATIF,
    CAN_ICODE_RESERVED
} CAN_ICODE;

//! RXCODE

typedef enum {
    CAN_RXCODE_FIFO_CH1=1,
    CAN_RXCODE_FIFO_CH2,
    CAN_RXCODE_FIFO_CH3,
    CAN_RXCODE_FIFO_CH4,
    CAN_RXCODE_FIFO_CH5,
    CAN_RXCODE_FIFO_CH6,
    CAN_RXCODE_FIFO_CH7,
    CAN_RXCODE_FIFO_CH8,
    CAN_RXCODE_FIFO_CH9,
    CAN_RXCODE_FIFO_CH10,
    CAN_RXCODE_FIFO_CH11,
    CAN_RXCODE_FIFO_CH12,
    CAN_RXCODE_FIFO_CH13,
    CAN_RXCODE_FIFO_CH14,
    CAN_RXCODE_FIFO_CH15,
    CAN_RXCODE_FIFO_CH16,
    CAN_RXCODE_FIFO_CH17,
    CAN_RXCODE_FIFO_CH18,
    CAN_RXCODE_FIFO_CH19,
    CAN_RXCODE_FIFO_CH20,
    CAN_RXCODE_FIFO_CH21,
    CAN_RXCODE_FIFO_CH22,
    CAN_RXCODE_FIFO_CH23,
    CAN_RXCODE_FIFO_CH24,
    CAN_RXCODE_FIFO_CH25,
    CAN_RXCODE_FIFO_CH26,
    CAN_RXCODE_FIFO_CH27,
    CAN_RXCODE_FIFO_CH28,
    CAN_RXCODE_FIFO_CH29,
    CAN_RXCODE_FIFO_CH30,
    CAN_RXCODE_FIFO_CH31,
    CAN_RXCODE_TOTAL_CHANNELS,
    CAN_RXCODE_NO_INT = 0x40,
    CAN_RXCODE_RESERVED
} CAN_RXCODE;

//! TXCODE

typedef enum {
    CAN_TXCODE_FIFO_CH0,
    CAN_TXCODE_FIFO_CH1,
    CAN_TXCODE_FIFO_CH2,
    CAN_TXCODE_FIFO_CH3,
    CAN_TXCODE_FIFO_CH4,
    CAN_TXCODE_FIFO_CH5,
    CAN_TXCODE_FIFO_CH6,
    CAN_TXCODE_FIFO_CH7,
    CAN_TXCODE_FIFO_CH8,
    CAN_TXCODE_FIFO_CH9,
    CAN_TXCODE_FIFO_CH10,
    CAN_TXCODE_FIFO_CH11,
    CAN_TXCODE_FIFO_CH12,
    CAN_TXCODE_FIFO_CH13,
    CAN_TXCODE_FIFO_CH14,
    CAN_TXCODE_FIFO_CH15,
    CAN_TXCODE_FIFO_CH16,
    CAN_TXCODE_FIFO_CH17,
    CAN_TXCODE_FIFO_CH18,
    CAN_TXCODE_FIFO_CH19,
    CAN_TXCODE_FIFO_CH20,
    CAN_TXCODE_FIFO_CH21,
    CAN_TXCODE_FIFO_CH22,
    CAN_TXCODE_FIFO_CH23,
    CAN_TXCODE_FIFO_CH24,
    CAN_TXCODE_FIFO_CH25,
    CAN_TXCODE_FIFO_CH26,
    CAN_TXCODE_FIFO_CH27,
    CAN_TXCODE_FIFO_CH28,
    CAN_TXCODE_FIFO_CH29,
    CAN_TXCODE_FIFO_CH30,
    CAN_TXCODE_FIFO_CH31,
    CAN_TXCODE_TOTAL_CHANNELS,
    CAN_TXCODE_NO_INT = 0x40,
    CAN_TXCODE_RESERVED
} CAN_TXCODE;

//! System Clock Selection

typedef enum {
    CAN_SYSCLK_40M,
    CAN_SYSCLK_20M,
    CAN_SYSCLK_10M
} CAN_SYSCLK_SPEED;

//! CLKO Divide

typedef enum {
    OSC_CLKO_DIV1,
    OSC_CLKO_DIV2,
    OSC_CLKO_DIV4,
    OSC_CLKO_DIV10
} OSC_CLKO_DIVIDE;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="SPI Instruction Set">
#define cINSTRUCTION_RESET			0x00    /**< SPI Instruction 'Reset' */
#define cINSTRUCTION_READ			0x03    /**< SPI Instruction 'Read' */
#define cINSTRUCTION_READ_CRC       0x0B    /**< SPI Instruction 'Read with CRC' */
#define cINSTRUCTION_WRITE			0x02    /**< SPI Instruction 'Write' */
#define cINSTRUCTION_WRITE_CRC      0x0A    /**< SPI Instruction 'Write with CRC */
#define cINSTRUCTION_WRITE_SAFE     0x0C    /**< SPI Instruction 'Write Safe (Check CRC before Write)' */

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Register Addresses">

/* CAN FD Controller */
#define cREGADDR_CiCON  	0x000   /**< Register address for 'CiCON' */
#define cREGADDR_CiNBTCFG	0x004   /**< Register address for 'CiNBTCFG' */
#define cREGADDR_CiDBTCFG	0x008   /**< Register address for 'CiDBTCFG' */
#define cREGADDR_CiTDC  	0x00C   /**< Register address for 'CiTDC' */

#define cREGADDR_CiTBC      0x010   /**< Register address for 'CiTBC' */
#define cREGADDR_CiTSCON    0x014   /**< Register address for 'CiTSCON' */
#define cREGADDR_CiVEC      0x018   /**< Register address for 'CiVEC' */
#define cREGADDR_CiINT      0x01C   /**< Register address for 'CiINT' */
#define cREGADDR_CiINTFLAG      cREGADDR_CiINT   /**< Register address for 'CiINTFLAG' */
#define cREGADDR_CiINTENABLE    (cREGADDR_CiINT+2)   /**< Register address for 'CiINTENABLE' */

#define cREGADDR_CiRXIF     0x020   /**< Register address for 'CiRXIF' */
#define cREGADDR_CiTXIF     0x024   /**< Register address for 'CiTXIF' */
#define cREGADDR_CiRXOVIF   0x028   /**< Register address for 'CiRXOVIF' */
#define cREGADDR_CiTXATIF   0x02C   /**< Register address for 'CiTXATIF' */

#define cREGADDR_CiTXREQ    0x030   /**< Register address for 'CiTXREQ' */
#define cREGADDR_CiTREC     0x034   /**< Register address for 'CiTREC' */
#define cREGADDR_CiBDIAG0   0x038   /**< Register address for 'CiBDIAG0' */
#define cREGADDR_CiBDIAG1   0x03C   /**< Register address for 'CiBDIAG1' */

#define cREGADDR_CiTEFCON   0x040   /**< Register address for 'CiTEFCON' */
#define cREGADDR_CiTEFSTA   0x044   /**< Register address for 'CiTEFSTA' */
#define cREGADDR_CiTEFUA    0x048   /**< Register address for 'CiTEFUA' */
#define cREGADDR_CiFIFOBA   0x04C   /**< Register address for 'CiFIFOBA' */

#define cREGADDR_CiFIFOCON  0x050   /**< Register address for 'CiFIFOCON' */
#define cREGADDR_CiFIFOSTA  0x054   /**< Register address for 'CiFIFOSTA' */
#define cREGADDR_CiFIFOUA   0x058   /**< Register address for 'CiFIFOUA' */
#define CiFIFO_OFFSET       (3*4)   /**< Offset for 'CiFIFO' */

#define cREGADDR_CiTXQCON  0x050   /**< Register address for 'CiTXQCON' */
#define cREGADDR_CiTXQSTA  0x054   /**< Register address for 'CiTXQSTA' */
#define cREGADDR_CiTXQUA   0x058   /**< Register address for 'CiTXQUA' */

// The filters start right after the FIFO control/status registers
#define cREGADDR_CiFLTCON   (cREGADDR_CiFIFOCON+(CiFIFO_OFFSET*CAN_FIFO_TOTAL_CHANNELS))   /**< Register address for 'CiFLTCON' */
#define cREGADDR_CiFLTOBJ   (cREGADDR_CiFLTCON+CAN_FIFO_TOTAL_CHANNELS)   /**< Register address for 'CiFLTOBJ' */
#define cREGADDR_CiMASK     (cREGADDR_CiFLTOBJ+4)   /**< Register address for 'CiMASK' */

#define CiFILTER_OFFSET     (2*4)   /**< Offset for 'CiFILTER' */

/* MCP25xxFD Specific */
#define cREGADDR_OSC        0xE00   /**< Register address for 'OSC' */
#define cREGADDR_IOCON      0xE04   /**< Register address for 'IOCON' */
#define cREGADDR_CRC    	0xE08   /**< Register address for 'CRC' */
#define cREGADDR_ECCCON  	0xE0C   /**< Register address for 'ECCCON' */
#define cREGADDR_ECCSTA  	0xE10   /**< Register address for 'ECCSTA' */
#ifndef MCP2517FD
#define cREGADDR_DEVID  	0xE14   /**< Register address for 'DEVID' */
#endif

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="RAM addresses">
#if defined(MCP2517FD) || defined(MCP2518FD)
#define cRAM_SIZE       2048   /**< RAM size */
#endif

#define cRAMADDR_START  0x400   /**< RAM start address */
#define cRAMADDR_END    (cRAMADDR_START+cRAM_SIZE)  /**< RAM end address */
// </editor-fold>




    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************
    // <editor-fold defaultstate="collapsed" desc="Register Structures">
/**
General 32-bit Register
 */
typedef union _REG_t {
    uint8_t byte[4];    /**< General 32-bit Register 8-bit byte array */
    uint32_t word;  /**< General 32-bit Register 32-bit word */
} REG_t;

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="CAN FD Controller">
/**
CAN Control Register
 */
typedef union _REG_CiCON {

    struct {
        uint32_t DNetFilterCount : 5;   /**< Device Net Filter Bit Number */
        uint32_t IsoCrcEnable : 1;  /**< Enable ISO CRC in CAN FD Frames */
        uint32_t ProtocolExceptionEventDisable : 1; /**< Protocol Exception Event Detection Disabled */
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t WakeUpFilterEnable : 1;    /**< Enable CAN Bus Line Wake-up Filter */
        uint32_t WakeUpFilterTime : 2;  /**< Selectable Wake-up Filter Time */
        uint32_t unimplemented2 : 1;    /**< rfu */
        uint32_t BitRateSwitchDisable : 1;  /**< Bit Rate Switching Disable */
        uint32_t unimplemented3 : 3;    /**< rfu */
        uint32_t RestrictReTxAttempts : 1;  /**< Restrict Retransmission Attempts */
        uint32_t EsiInGatewayMode : 1;  /**< Transmit ESI in Gateway Mode */
        uint32_t SystemErrorToListenOnly : 1;   /**< Transition to Listen Only Mode on System Error */
        uint32_t StoreInTEF : 1;    /**< Store in Tranmsit Event FIFO */
        uint32_t TXQEnable : 1; /**< Enable Transmit Queue */
        uint32_t OpMode : 3;    /**< Operation Mode Status */
        uint32_t RequestOpMode : 3; /**< Request Operation Mode */
        uint32_t AbortAllTx : 1;    /**< Abort All Pending Transmissions */
        uint32_t TxBandWidthSharing : 4;    /**< Transmit Bandwith Sharing */
    } bF;   /**< CAN Control Register bit field */
    uint32_t word;  /**< CAN Control Register 32-bit word */
    uint8_t byte[4];    /**< CAN Control Register 8-bit byte array */
} REG_CiCON;

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Nominal Bit Time Configuration Register">
/**
Nominal Bit Time Configuration Register
 */
typedef union _REG_CiNBTCFG {

    struct {
        uint32_t SJW : 7;   /**< Synchronization Jump Width */
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t TSEG2 : 7; /**< Time Segment 2  */
        uint32_t unimplemented2 : 1;    /**< rfu */
        uint32_t TSEG1 : 8; /**< Time Segment 1  */
        uint32_t BRP : 8;   /**< Baud Rate Prescaler  */
    } bF;   /**< Nominal Bit Time Configuration Register bit field */
    uint32_t word;  /**< Nominal Bit Time Configuration Register 32-bit word */
    uint8_t byte[4];    /**< Nominal Bit Time Configuration Register 8-bit byte array */
} REG_CiNBTCFG;
    // </editor-fold>

// *****************************************************************************
/**
Data Bit Time Configuration Register
 */ 
typedef union _REG_CiDBTCFG {

    struct {
        uint32_t SJW : 4;   /**< Synchronization Jump Width */
        uint32_t unimplemented1 : 4;    /**< rfu */
        uint32_t TSEG2 : 4; /**< Time Segment 2  */
        uint32_t unimplemented2 : 4;    /**< rfu */
        uint32_t TSEG1 : 5; /**< Time Segment 1 */
        uint32_t unimplemented3 : 3;    /**< rfu */
        uint32_t BRP : 8;   /**< Baud Rate Prescaler */
    } bF;   /**< Data Bit Time Configuration Register bit field */
    uint32_t word;   /**< Data Bit Time Configuration Register 32-bit word */
    uint8_t byte[4];   /**< Data Bit Time Configuration Register 8-bit byte array */
} REG_CiDBTCFG;

// *****************************************************************************
/**
Transmitter Delay Compensation Register
 */ 
typedef union _REG_CiTDC {

    struct {
        uint32_t TDCValue : 6;    /**< Transmitter Delay Compensation Value */
        uint32_t unimplemented1 : 2;    /**< rfu */
        uint32_t TDCOffset : 7;    /**< Transmitter Delay Compensation Offset */
        uint32_t unimplemented2 : 1;    /**< rfu */
        uint32_t TDCMode : 2;    /**< Transmitter Delay Compensation Mode */
        uint32_t unimplemented3 : 6;    /**< rfu */
        uint32_t SID11Enable : 1;    /**< Enable 12-Bit SID in CAN FD Base Format Messages */
        uint32_t EdgeFilterEnable : 1;    /**< Enable Edge Filterin during BUS Integration state */
        uint32_t unimplemented4 : 6;    /**< rfu */
    } bF;   /**< Transmitter Delay Compensation Register bit field */
    uint32_t word;  /**< Transmitter Delay Compensation Register 32-bit word */
    uint8_t byte[4];    /**< Transmitter Delay Compensation Register 8-bit byte array */
} REG_CiTDC;

// *****************************************************************************
/**
Time Stamp Configuration Register
 */ 
typedef union _REG_CiTSCON {

    struct {
        uint32_t TBCPrescaler : 10; /**< Time Base Counter Prescaler */
        uint32_t unimplemented1 : 6;    /**< rfu */
        uint32_t TBCEnable : 1; /**< Time Base Counter Enable */
        uint32_t TimeStampEOF : 1;  /**< Time Stamp EOF */
        uint32_t unimplemented2 : 14;   /**< rfu */
    } bF;   /**< Time Stamp Configuration Register bit field*/
    uint32_t word;  /**< Time Stamp Configuration Register 32-bit word */
    uint8_t byte[4];    /**< Time Stamp Configuration Register 8-bit byte array */
} REG_CiTSCON;

// *****************************************************************************
/**
Interrupt Vector Register
 */
typedef union _REG_CiVEC {

    struct {
        uint32_t ICODE : 7;
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t FilterHit : 5;
        uint32_t unimplemented2 : 3;    /**< rfu */
        uint32_t TXCODE : 7;
        uint32_t unimplemented3 : 1;    /**< rfu */
        uint32_t RXCODE : 7;
        uint32_t unimplemented4 : 1;    /**< rfu */
    } bF;   /**< Interrupt Vector Register bit field */
    uint32_t word;  /**< Interrupt Vector Register 32-bit word */
    uint8_t byte[4];    /**< Interrupt Vector Register 8-bit byte array */
} REG_CiVEC;

// *****************************************************************************
/** 
Interrupt Flags
*/ 
typedef struct _CAN_INT_FLAGS {
    uint32_t TXIF : 1;  /**< Transmit FIFO Interrupt Flag bit */
    uint32_t RXIF : 1;  /**< Receive FIFO Interrupt Flag bit */
    uint32_t TBCIF : 1; /**< Time Base Counter Interrupt Flag bit */
    uint32_t MODIF : 1; /**< Mode Change Interrupt Flag bit */
    uint32_t TEFIF : 1; /**< Transmit Event FIFO Interrupt Flag bit */
    uint32_t unimplemented1 : 3;    /**< rfu */

    uint32_t ECCIF : 1; /**< ECC Error Interrupt Flag bit */
    uint32_t SPICRCIF : 1;  /**< SPI CRC Error Interrupt Flag bit */
    uint32_t TXATIF : 1;    /**< Transmit Attempt Interrupt Flag bit */
    uint32_t RXOVIF : 1;    /**< Receive FIFO Overflow Interrupt Flag bit */
    uint32_t SERRIF : 1;    /**< System Error Interrupt Flag bit */
    uint32_t CERRIF : 1;    /**< CAN Bus Error Interrupt Flag bit */
    uint32_t WAKIF : 1; /**< Bus Wake Up Interrupt Flag bit */
    uint32_t IVMIF : 1; /**< Invalid Message Interrupt Flag bit */
} CAN_INT_FLAGS;

// *****************************************************************************

/**
Interrupt Enables
 */
typedef struct _CAN_INT_ENABLES {
    uint32_t TXIE : 1;  /**< Transmit FIFO Interrupt Enable bit */
    uint32_t RXIE : 1;  /**< Receive FIFO Interrupt Enable bit */
    uint32_t TBCIE : 1; /**< Time Base Counter Interrupt Enable bit */
    uint32_t MODIE : 1; /**< Mode Change Interrupt Enable bit */
    uint32_t TEFIE : 1; /**< Transmit Event FIFO Interrupt Enable bit */
    uint32_t unimplemented2 : 3;    /**< rfu */

    uint32_t ECCIE : 1; /**< ECC Error Interrupt Enable bit */
    uint32_t SPICRCIE : 1;  /**< SPI CRC Error Interrupt Enable bit */
    uint32_t TXATIE : 1;    /**< Transmit Attempt Interrupt Enable bit */
    uint32_t RXOVIE : 1;    /**< Receive FIFO Overflow Interrupt Enable bit */
    uint32_t SERRIE : 1;    /**< System Error Interrupt Enable bit */
    uint32_t CERRIE : 1;    /**< CAN Bus Error Interrupt Enable bit */
    uint32_t WAKIE : 1;     /**< Bus Wake Up Interrupt Enable bit */
    uint32_t IVMIE : 1;     /**< Invalid Message Interrupt Enable bit */
} CAN_INT_ENABLES;

// *****************************************************************************
/**
Interrupt Register
 */
typedef union _REG_CiINT {

    struct {
        CAN_INT_FLAGS IF;   /**< Interrupt Flags (see ::CAN_INT_FLAGS) */
        CAN_INT_ENABLES IE; /**< Interrupt Enables (see ::CAN_INT_ENABLES) */
    } bF;   /**< Interrupt Register bit field */
    uint32_t word;  /**< Interrupt Register 32-bit word */
    uint8_t byte[4];    /**< Interrupt Register 8-bit byte array */
} REG_CiINT;

// *****************************************************************************
/**
Interrupt Flag Register
 */
typedef union _REG_CiINTFLAG {
    CAN_INT_FLAGS IF;   /**< Interrupt Flags (see ::CAN_INT_FLAGS) */
    uint16_t word;  /**< Interrupt Flag Register 16-bit word */
    uint8_t byte[2];    /**< Interrupt Flag Register 8-bit byte array */
} REG_CiINTFLAG;

// *****************************************************************************
/**
Interrupt Enable Register
 */
typedef union _REG_CiINTENABLE {
    CAN_INT_ENABLES IE; /**< Interrupt Enables (see ::CAN_INT_ENABLES) */
    uint16_t word;  /**< Interrupt Flag Register 16-bit word */
    uint8_t byte[2];    /**< Interrupt Flag Register 8-bit byte array */
} REG_CiINTENABLE;

// *****************************************************************************
/**
Transmit/Receive Error Count Register
 */
typedef union _REG_CiTREC {

    struct {
        uint32_t RxErrorCount : 8;  /**< Receive Error Counter */
        uint32_t TxErrorCount : 8;  /**< Transmit Error Counter */
        uint32_t ErrorStateWarning : 1; /**< Tranmsitter or Receiver is in Error Warning State */
        uint32_t RxErrorStateWarning : 1;   /**< Receiver in Error Warning State */
        uint32_t TxErrorStateWarning : 1;   /**< Transmitter in Error Warning State */
        uint32_t RxErrorStatePassive : 1;   /**< Receiver in Error Passive State */
        uint32_t TxErrorStatePassive : 1;   /**< Tranmsitter in Error Passive State */
        uint32_t TxErrorStateBusOff : 1;    /**< Transmitter in Bus Off State */
        uint32_t unimplemented1 : 10;   /**< rfu */
    } bF;   /**< Transmit/Receive Error Count Register bit field */
    uint32_t word;  /**< Transmit/Receive Error Count Register 32-bit word */
    uint8_t byte[4];    /**< Transmit/Receive Error Count Register 8-bit byte array */
} REG_CiTREC;

// *****************************************************************************
/**
Diagnostic Register 0
 */
typedef union _REG_CiBDIAG0 {

    struct {
        uint32_t NRxErrorCount : 8; /**< Nominal Bit Rate Receive Error Counter */
        uint32_t NTxErrorCount : 8; /**< Nominal Bit Rate Transmit Error Counter */
        uint32_t DRxErrorCount : 8; /**< Data Bit Rate Receive Error Counter */
        uint32_t DTxErrorCount : 8; /**< Data Bit Rate Transmit Error Counter */
    } bF;   /**< Diagnostic Register 0 bit field */
    uint32_t word;  /**< Diagnostic Register 0 32-bit word */
    uint8_t byte[4];    /**< Diagnostic Register 0 8-bit byte array */
} REG_CiBDIAG0;

// *****************************************************************************
/**
Diagnostic Register 1
 */ 
typedef union _REG_CiBDIAG1 {

    struct {
        uint32_t ErrorFreeMsgCount : 16;    /**< Error Free Messge Counter */

        uint32_t NBit0Error : 1;    /**< During the transmission of a message, the device wanted to send a dominant level, but the monitored bus value was recessive. */
        uint32_t NBit1Error : 1;    /**< During the transmission of a message, the device wanted to send a recessive level, but the monitored bus value was dominant. */
        uint32_t NAckError : 1; /**< Transmitted Message was not acknowledged */
        uint32_t NFormError : 1;    /**< A fixed formate part of a received frame has the wrong format. */
        uint32_t NStuffError : 1;   /**< More than 5 equal bits in a sequence have occured in a part of a received message where this is not allowed */
        uint32_t NCRCError : 1; /**< The CRC check sum of a received message was incorrect. The CRC of an incoming message does not match with the CRC calculation from the received data. */
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t TXBOError : 1; /**< Device went to bus-off (and auto-recovered) */
        uint32_t DBit0Error : 1;    /**< During the transmission of a message, the device wanted to send a dominant level, but the monitored bus value was recessive. */
        uint32_t DBit1Error : 1;    /**< During the transmission of a message, the device wanted to send a recessive level, but the monitored bus value was dominant. */
        uint32_t DAckError : 1; /**< rfu */
        uint32_t DFormError : 1;    /**< A fixed formate part of a received frame has the wrong format. */
        uint32_t DStuffError : 1;   /**< More than 5 equal bits in a sequence have occured in a part of a received message where this is not allowed */
        uint32_t DCRCError : 1; /**< The CRC check sum of a received message was incorrect. The CRC of an incoming message does not match with the CRC calculation from the received data. */
        uint32_t ESI : 1;   /**< ESI flag of a received CAN FD message was set. */
        uint32_t unimplemented2 : 1;    /**< rfu */
    } bF;   /**<  Diagnostic Register 1 bit field */
    uint32_t word;  /**< Diagnostic Register 1 32-bit word */
    uint8_t byte[4];    /**< Diagnostic Register 1 8-bit word array */
} REG_CiBDIAG1;

// *****************************************************************************
/**
Transmit Event FIFO Control Register
 */
typedef union _REG_CiTEFCON {

    struct {
        uint32_t TEFNEIE : 1;   /**< Transmit Event FIFO Not Empty Interrupt Enable */
        uint32_t TEFHFIE : 1;   /**< Transmit Event FIFO Half Full Interrupt Enable */
        uint32_t TEFFULIE : 1;  /**< Transmit Event FIFO Full Interrupt Enable */
        uint32_t TEFOVIE : 1;   /**< Transmit Event FIFO Overflow Interrupt Enable */
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t TimeStampEnable : 1;   /**< Transmit Event FIFO Time Stamp Enable */
        uint32_t unimplemented2 : 2;    /**< rfu */
        uint32_t UINC : 1;  /**< Increment Tail */
        uint32_t unimplemented3 : 1;    /**< rfu */
        uint32_t FRESET : 1;    /**< FIFO Reset */
        uint32_t unimplemented4 : 13;    /**< rfu */
        uint32_t FifoSize : 5;  /**< FIFO Size */
        uint32_t unimplemented5 : 3;    /**< rfu */
    } bF;   /**< Transmit Event FIFO Control Register bit field */
    uint32_t word;  /**< Transmit Event FIFO Control Register 32-bit word */
    uint8_t byte[4];    /**< Transmit Event FIFO Control Register 8-bit byte array */
} REG_CiTEFCON;

// *****************************************************************************
/**
Transmit Event FIFO Status Register
 */
typedef union _REG_CiTEFSTA {

    struct {
        uint32_t TEFNotEmptyIF : 1; /**< Transmit Event FIFO Not Empty Interrupt Flag */
        uint32_t TEFHalfFullIF : 1; /**< Transmit Event FIFO Half Full Interrupt Flag */
        uint32_t TEFFullIF : 1; /**< Transmit Event FIFO Full Interrupt Flag */
        uint32_t TEFOVIF : 1;   /**< Transmit Event FIFO Overflow Interrupt Flag */
        uint32_t unimplemented1 : 28;   /**< rfu */
    } bF;   /**< Transmit Event FIFO Status Register bit field */
    uint32_t word;  /**< Transmit Event FIFO Status Register 32-bit word */
    uint8_t byte[4];    /**< Transmit Event FIFO Status Register 8-bit byte array */
} REG_CiTEFSTA;

// *****************************************************************************
/**
Transmit Queue Control Register
 */
typedef union _REG_CiTXQCON {

    struct {
        uint32_t TxNotFullIE : 1;   /**< Transmit Queue Not Full Interrupt Enable */
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t TxEmptyIE : 1; /**< Transmit Queue Empty Interrupt Enable */
        uint32_t unimplemented2 : 1;    /**< rfu */
        uint32_t TxAttemptIE : 1;   /**< Transmit Attempts Exhausted Interrupt Enable */
        uint32_t unimplemented3 : 2;    /**< rfu */
        uint32_t TxEnable : 1;  /**< TX Enable */
        uint32_t UINC : 1;  /**< Increment Head */
        uint32_t TxRequest : 1; /**< Message Send Request */
        uint32_t FRESET : 1;    /**< FIFO Reset */
        uint32_t unimplemented4 : 5;    /**< rfu */
        uint32_t TxPriority : 5;    /**< Message Transmit Priority */
        uint32_t TxAttempts : 2;    /**< Retransmission Attempts */
        uint32_t unimplemented5 : 1;    /**< rfu */
        uint32_t FifoSize : 5;  /**< FIFO Size */
        uint32_t PayLoadSize : 3;   /**< Payload Size */
    } txBF; /**< Transmit Queue Control Register bit field */
    uint32_t word;  /**< Transmit Queue Control Register 32-bit word */
    uint8_t byte[4];    /**< Transmit Queue Control Register 8-bit byte array */
} REG_CiTXQCON;

// *****************************************************************************
/**
Transmit Queue Status Register
 */
typedef union _REG_CiTXQSTA {

    struct {
        uint32_t TxNotFullIF : 1;   /**< Transmit Queue Not Full Interrupt Flag */
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t TxEmptyIF : 1; /**< Transmit Queue Empty Interrupt Flag */
        uint32_t unimplemented2 : 1;    /**< rfu */
        uint32_t TxAttemptIF : 1;   /**< Transmit Attempts Exhausting Interrupt Pending */
        uint32_t TxError : 1;   /**< Error Detected During Transmission */
        uint32_t TxLostArbitration : 1; /**< Message Lost Arbitration Status */
        uint32_t TxAborted : 1; /**< Message Aborted Status */
        uint32_t FifoIndex : 5; /**< Transmit Queue Message Index */
        uint32_t unimplemented3 : 19;   /**< rfu */
    } txBF; /**< Transmit Queue Status Register bit field */
    uint32_t word;  /**< Transmit Queue Status Register 32-bit word */
    uint8_t byte[4];    /**< Transmit Queue Status Register 8-bit byte array */
} REG_CiTXQSTA;

// *****************************************************************************
/**
FIFO Control Register
 */
typedef union _REG_CiFIFOCON {
    // Receive FIFO
    struct {
        uint32_t RxNotEmptyIE : 1;  /**< Receive FIFO Not Empty Interrupt Enable */
        uint32_t RxHalfFullIE : 1;  /**< Receive FIFO Half Full Interrupt Enable */
        uint32_t RxFullIE : 1;  /**< Receive FIFO Full Interrupt Enable */
        uint32_t RxOverFlowIE : 1;  /**< Overflow Interrupt Enable */
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t RxTimeStampEnable : 1; /**< Received Message Time Stamp Enable */
        uint32_t unimplemented2 : 1;    /**< rfu */
        uint32_t TxEnable : 1;  /**< TX/RX FIFO Selection */
        uint32_t UINC : 1;  /**< Increment Tail */
        uint32_t unimplemented3 : 1;    /**< rfu */
        uint32_t FRESET : 1;    /**< FIFO Reset */
        uint32_t unimplemented4 : 13;   /**< rfu */
        uint32_t FifoSize : 5;  /**< FIFO size */
        uint32_t PayLoadSize : 3;   /**< Payload size */
    } rxBF; /**< Receive FIFO Control Register bit field */

    // Transmit FIFO
    struct {
        uint32_t TxNotFullIE : 1;   /**< Transmit FIFO Not Full Interrupt Enable */
        uint32_t TxHalfFullIE : 1;  /**< Transmit FIFO Half Full Interrupt Enable */
        uint32_t TxEmptyIE : 1; /**< Transmit FIFO Empty Interrupt Enable */
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t TxAttemptIE : 1;   /**< Transmit Attempts Exhausted Interrupt Enable */
        uint32_t unimplemented2 : 1;    /**< rfu */
        uint32_t RTREnable : 1; /**< Auto RTR Enable */
        uint32_t TxEnable : 1;  /**< TX/RX FIFO Selection */
        uint32_t UINC : 1;  /**< Increment Head */
        uint32_t TxRequest : 1; /**< Message Send Request */
        uint32_t FRESET : 1;    /**< FIFO Reset */
        uint32_t unimplemented3 : 5;    /**< rfu */
        uint32_t TxPriority : 5;    /**< Message Transmit Priority */
        uint32_t TxAttempts : 2;    /**< Retransmission Attempts */
        uint32_t unimplemented4 : 1;    /**< rfu */
        uint32_t FifoSize : 5;  /**< FIFO size */
        uint32_t PayLoadSize : 3;   /**< Payload size */
    } txBF; /**< Transmit FIFO Control Register bit field */
    uint32_t word;  /**< FIFO Control Register 32-bit word */
    uint8_t byte[4];    /**< FIFO Control Register 8-bit byte array */
} REG_CiFIFOCON;

// *****************************************************************************
/**
FIFO Status Register
 */
typedef union _REG_CiFIFOSTA {
    // Receive FIFO
    struct {
        uint32_t RxNotEmptyIF : 1;  /**< Receive FIFO Not Empty Interrupt Flag */
        uint32_t RxHalfFullIF : 1;  /**< Receive FIFO Half Full Interrupt Flag */
        uint32_t RxFullIF : 1;  /**< Receive FIFO Full Interrupt Flag */
        uint32_t RxOverFlowIF : 1;  /**< Receive FIFO Overflow Interrupt Flag */
        uint32_t unimplemented1 : 4;    /**< rfu */
        uint32_t FifoIndex : 5; /**< FIFO Message Index */
        uint32_t unimplemented2 : 19;   /**< rfu */
    } rxBF; /**< Receive FIFO Status Register bit field */

    // Transmit FIFO
    struct {
        uint32_t TxNotFullIF : 1;   /**< Transmit FIFO Not Full Interrupt Flag */
        uint32_t TxHalfFullIF : 1;  /**< Transmit FIFO Half Empty Interrupt Flag */
        uint32_t TxEmptyIF : 1; /**< Transmit FIFO Empty Interrupt Flag */
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t TxAttemptIF : 1;   /**< Transmit Attempts Exhausted Interrupt Pending */
        uint32_t TxError : 1;   /**< Error Detected During Transmission */
        uint32_t TxLostArbitration : 1; /**< Message Lost Arbitration Status */
        uint32_t TxAborted : 1; /**< TX Aborted Status */
        uint32_t FifoIndex : 5; /**< FIFO Message Index */
        uint32_t unimplemented2 : 19;   /**< rfu */
    } txBF; /**< Transmit FIFO Status Register bit field */
    uint32_t word;  /**< FIFO Status Register 32-bit word */
    uint8_t byte[4];    /**< FIFO Status Register 8-bit byte array */
} REG_CiFIFOSTA;

// *****************************************************************************
/**
FIFO User Address Register
 */
typedef union _REG_CiFIFOUA {
    struct {
        uint32_t UserAddress : 12;  /**< FIFO User Address */
        uint32_t unimplemented1 : 20;   /**< rfu */
    } bF;   /**< FIFO User Address Register bit field */
    uint32_t word;  /**< FIFO User Address Register 32-bit word */
    uint8_t byte[4];    /**< FIFO User Address Register 8-bit byte array */
} REG_CiFIFOUA;

// *****************************************************************************
/**
Filter Control Register
 */
typedef union _REG_CiFLTCON_BYTE {

    struct {
        uint32_t BufferPointer : 5; /**< Pointer to FIFO when Filter hits */
        uint32_t unimplemented1 : 2;    /**< rfu */
        uint32_t Enable : 1;    /**< Enable Filter to Accept Messages */
    } bF;   /**< Filter Control Register bit field */
    uint8_t byte;   /**< Filter Control Register byte */
} REG_CiFLTCON_BYTE;

// *****************************************************************************
/**
Filter Object Register
 */
typedef union _REG_CiFLTOBJ {
    CAN_FILTEROBJ_ID bF;    /**< Filter Object Register bit field */
    uint32_t word;  /**< Filter Object Register 32-bit word */
    uint8_t byte[4];    /**< Filter Object Register 8-bit byte array */
} REG_CiFLTOBJ;

// *****************************************************************************
/**
Mask Object Register
 */
typedef union _REG_CiMASK {
    CAN_MASKOBJ_ID bF;  /**< Mask Object Register bit field */
    uint32_t word;  /**< Mask Object Register 32-bit word */
    uint8_t byte[4];    /**< Mask Object Register 8-bit byte array */
} REG_CiMASK;


// *****************************************************************************
// *****************************************************************************
/* MCP25xxFD Specific */

// *****************************************************************************
/**
Oscillator Control Register 
 */

typedef union _REG_OSC {

    struct {
        uint32_t PllEnable : 1; /**< PLL Enable */
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t OscDisable : 1;    /**< Clock (Oscillator) Disable */
#ifdef MCP2517FD
        uint32_t unimplemented2 : 1;    /**< rfu */
#else
        uint32_t LowPowerModeEnable : 1;    /**< Low Power Mode(LPM) Enable */
#endif
        uint32_t SCLKDIV : 1;   /**< System Clock Divisor */
        uint32_t CLKODIV : 2;   /**< Clock Output Divisor */
        uint32_t unimplemented3 : 1;    /**< rfu */
        uint32_t PllReady : 1;  /**< PLL Ready */
        uint32_t unimplemented4 : 1;    /**< rfu */
        uint32_t OscReady : 1;  /**< Clock Ready */
        uint32_t unimplemented5 : 1;    /**< rfu */
        uint32_t SclkReady : 1; /**< Synchronized SCLKDIV */
        uint32_t unimplemented6 : 19;   /**< rfu */
    } bF;   /**< Oscillator Control Register bit field */
    uint32_t word;  /**< Oscillator Control Register 32-bit word */
    uint8_t byte[4];    /**< Oscillator Control Register 8-bit byte array */
} REG_OSC;

// *****************************************************************************
/**
I/O Control Register
 */
typedef union _REG_IOCON {

    struct {
        uint32_t TRIS0 : 1; /**< GPIO0 Data Direction */
        uint32_t TRIS1 : 1; /**< GPIO1 Data Direction */
        uint32_t unimplemented1 : 2;    /**< rfu */
        uint32_t ClearAutoSleepOnMatch : 1; /**< rfu */
        uint32_t AutoSleepEnable : 1;   /**< rfu */
        uint32_t XcrSTBYEnable : 1; /**< Enable Transceiver Standby Pin Control */
        uint32_t unimplemented2 : 1;    /**< rfu */
        uint32_t LAT0 : 1;  /**< GPIO0 Latch */
        uint32_t LAT1 : 1;  /**< GPIO1 Latch */
        uint32_t unimplemented3 : 5;    /**< rfu */
        uint32_t HVDETSEL : 1;  /**< rfu */
        uint32_t GPIO0 : 1; /**< GPIO0 Status */
        uint32_t GPIO1 : 1; /**< GPIO1 Status */
        uint32_t unimplemented4 : 6;    /**< rfu */
        uint32_t PinMode0 : 1;  /**< GPIO Pin Mode */
        uint32_t PinMode1 : 1;  /**< GPIO Pin Mode */
        uint32_t unimplemented5 : 2;    /**< rfu */
        uint32_t TXCANOpenDrain : 1;    /**< TXCAN Open Drain Mode */
        uint32_t SOFOutputEnable : 1;   /**< Start-Of-Frame signal */
        uint32_t INTPinOpenDrain : 1;   /**< Interrupt Pins Open Drain Mode */
        uint32_t unimplemented6 : 1;    /**< rfu */
    } bF;   /**< I/O Control Register bit field */
    uint32_t word;  /**< I/O Control Register 32-bit word */
    uint8_t byte[4];    /**< I/O Control Register 8-bit byte array */
} REG_IOCON;

// *****************************************************************************
/**
CRC Register
 */
typedef union _REG_CRC {

    struct {
        uint32_t CRC : 16;  /**< Cycle Redundancy Check from last CRC mismatch */
        uint32_t CRCERRIF : 1;  /**< CRC Error Interrupt Flag */
        uint32_t FERRIF : 1;    /**< CRC Command Format Framte Error Interrupt Flag */
        uint32_t unimplemented1 : 6;    /**< rfu */
        uint32_t CRCERRIE : 1;  /**< CRC Error Interrupt Enable */
        uint32_t FERRIE : 1;    /**< CRC Command Format Frame Error Interrupt Enable */
        uint32_t unimplemented2 : 6;    /**< rfu */
    } bF;   /**< CRC Register bit field */
    uint32_t word;  /**< CRC Register 32-bit word */
    uint8_t byte[4];    /**< CRC Register 8-bit byte array */
} REG_CRC;

// *****************************************************************************
/**
ECC Control Register
 */
typedef union _REG_ECCCON {

    struct {
        uint32_t EccEn : 1; /**< ECC Enable */
        uint32_t SECIE : 1; /**< Single Error Detection Interrupt Enable Flag */
        uint32_t DEDIE : 1; /**< Double Error Detection Interrupt Enable Flag */
        uint32_t unimplemented1 : 5;    /**< rfu */
        uint32_t Parity : 7;    /**< Parity bits used during write to RAM when ECC is disabled */
        uint32_t unimplemented2 : 17;   /**< rfu */
    } bF;   /**< ECC Control Register bit field */
    uint32_t word;  /**< ECC Control Register 32-bit word */
    uint8_t byte[4];    /**< ECC Control Register 8-bit byte array */
} REG_ECCCON;

// *****************************************************************************
/**
ECC Status Register
 */
typedef union _REG_ECCSTA {

    struct {
        uint32_t unimplemented1 : 1;    /**< rfu */
        uint32_t SECIF : 1; /**< Single Detection Interrupt Flag */
        uint32_t DEDIF : 1; /**< Double Error Detection Interrupt Flag */
        uint32_t unimplemented2 : 13;   /**< rfu */
        uint32_t ErrorAddress : 12; /**< Address where last ECC error occured */
        uint32_t unimplemented3 : 4;    /**< rfu */
    } bF;   /**< ECC Status Register bit field */
    uint32_t word;  /**< ECC Status Register 32-bit word */
    uint8_t byte[4];    /**< ECC Status Register 8-bit byte array */
} REG_ECCSTA;

// *****************************************************************************
/**
DEVID Register
 */
#ifndef MCP2517FD
typedef union _REG_DEVID {

    struct {
        uint32_t REV : 4;   /**< Silicon revision */
        uint32_t DEV : 4;   /**< Device ID */
        uint32_t unimplemented : 24;    /**< rfu */
    } bF;   /**< DEVID Register bit field */
    uint32_t word;  /**< DEVID Register 32-bit word */
    uint8_t byte[4];    /**< DEVID Register 8-bit byte array */
} REG_DEVID;
#endif


// *****************************************************************************
// *****************************************************************************
/* Register Reset Values */

// *****************************************************************************
/* CAN FD Controller */

// Control Register Reset Values up to FIFOs
const static uint32_t canControlResetValues[] = {
    /* Address 0x000 to 0x00C */
    0x04980760, 0x003E0F0F, 0x000E0303, 0x00021000,
    /* Address 0x010 to 0x01C */
    0x00000000, 0x00000000, 0x40400040, 0x00000000,
    /* Address 0x020 to 0x02C */
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    /* Address 0x030 to 0x03C */
    0x00000000, 0x00200000, 0x00000000, 0x00000000,
    /* Address 0x040 to 0x04C */
    0x00000400, 0x00000000, 0x00000000, 0x00000000
};

// FIFO Register Reset Values
const static uint32_t canFifoResetValues[] = {
    0x00600400, 0x00000000, 0x00000000
};

// Filter Control Register Reset Values
const static uint32_t canFilterControlResetValue = 0x00000000;

// Filter and Mask Object Reset Values
const static uint32_t canFilterObjectResetValues[] = {
    0x00000000, 0x00000000
};

// *****************************************************************************
/* MCP25xxFD */

#if defined(MCP2517FD) || defined(MCP2518FD)
const static uint32_t mcp25xxfdControlResetValues[] = {
    0x00000460, 0x00000003, 0x00000000, 0x00000000, 0x00000000
};
#endif


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    /**
    Application CAN Initialization
     */
    extern void APPLICATION_CAN_Initialize(void);
    /**
    CAN Module Initialization
     */
    extern void APPLICATION_CAN_Init(void);
    /**
    Application CAN Main Tasks
    @param pMsg pointer to message to process
     */
    extern void APPLICATION_CAN_Tasks(APP_Msg_T *pMsg);
    /**
    Write data via CAN
    @param id CAN address
    @param pData pointer to data to write via CAN
    @param size number of bytes to write via CAN
     */
    extern void APPLICATION_CAN_Write(uint16_t id, uint8_t *pData, const size_t size);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_CAN_H */

/* *****************************************************************************
 End of File
 */
