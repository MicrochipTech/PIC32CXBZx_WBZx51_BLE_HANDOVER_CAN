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
#include "application_config.h"
#include "application_led.h"
#include "definitions.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
DRV_HANDLE canSPIHandle = DRV_HANDLE_INVALID;

/**
CAN Message structure
 */
typedef struct CAN_MSG_t {
    union {
        CAN_TX_MSGOBJ txObj;    /**< CAN TX Object */
        CAN_RX_MSGOBJ rxObj;    /**< CAN RX Object */
    }msgObj;    /**< CAN Message object */
    uint8_t can_data[MAX_DATA_BYTES];   /**< CAN Message data buffer */
}CAN_MSG_t;

bool ramInitialized = false;

//! SPI Transmit buffer
uint8_t spiTransmitBuffer[SPI_DEFAULT_BUFFER_LENGTH];

//! SPI Receive buffer
uint8_t spiReceiveBuffer[SPI_DEFAULT_BUFFER_LENGTH];

//! Reverse order of bits in byte
const uint8_t BitReverseTable256[256] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

//! Look-up table for CRC calculation
const uint16_t crc16_table[256] = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
// <editor-fold defaultstate="collapsed" desc="static declarations">
int8_t _APPLICATION_CAN_TransferData(CANFDSPI_MODULE_ID index, void* txb, void* rxb, uint16_t txs);
int8_t _APPLICATION_CAN_Reset(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_ReadByte(CANFDSPI_MODULE_ID index, uint16_t address, uint8_t *rxd);
int8_t _APPLICATION_CAN_WriteByte(CANFDSPI_MODULE_ID index, uint16_t address, uint8_t txd);
int8_t _APPLICATION_CAN_ReadWord(CANFDSPI_MODULE_ID index, uint16_t address, uint32_t *rxd);
int8_t _APPLICATION_CAN_WriteWord(CANFDSPI_MODULE_ID index, uint16_t address,uint32_t txd);
int8_t _APPLICATION_CAN_ReadHalfWord(CANFDSPI_MODULE_ID index, uint16_t address, uint16_t *rxd);
int8_t _APPLICATION_CAN_WriteHalfWord(CANFDSPI_MODULE_ID index, uint16_t address,uint16_t txd);
int8_t _APPLICATION_CAN_WriteByteSafe(CANFDSPI_MODULE_ID index, uint16_t address,uint8_t txd);
int8_t _APPLICATION_CAN_WriteWordSafe(CANFDSPI_MODULE_ID index, uint16_t address,uint32_t txd);
int8_t _APPLICATION_CAN_ReadByteArray(CANFDSPI_MODULE_ID index, uint16_t address,uint8_t *rxd, uint16_t nBytes);
int8_t _APPLICATION_CAN_ReadByteArrayWithCRC(CANFDSPI_MODULE_ID index, uint16_t address,uint8_t *rxd, uint16_t nBytes, bool fromRam, bool* crcIsCorrect);
int8_t _APPLICATION_CAN_WriteByteArray(CANFDSPI_MODULE_ID index, uint16_t address,uint8_t *txd, uint16_t nBytes);
int8_t _APPLICATION_CAN_WriteByteArrayWithCRC(CANFDSPI_MODULE_ID index, uint16_t address,uint8_t *txd, uint16_t nBytes, bool fromRam);
int8_t _APPLICATION_CAN_ReadWordArray(CANFDSPI_MODULE_ID index, uint16_t address,uint32_t *rxd, uint16_t nWords);
int8_t _APPLICATION_CAN_WriteWordArray(CANFDSPI_MODULE_ID index, uint16_t address,uint32_t *txd, uint16_t nWords);
int8_t _APPLICATION_CAN_Configure(CANFDSPI_MODULE_ID index, CAN_CONFIG* config);
int8_t _APPLICATION_CAN_ConfigureObjectReset(CAN_CONFIG* config);
int8_t _APPLICATION_CAN_OperationModeSelect(CANFDSPI_MODULE_ID index,CAN_OPERATION_MODE opMode);
CAN_OPERATION_MODE _APPLICATION_CAN_OperationModeGet(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_LowPowerModeEnable(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_LowPowerModeDisable(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_TransmitChannelConfigure(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_CONFIG* config);
int8_t _APPLICATION_CAN_TransmitChannelConfigureObjectReset(CAN_TX_FIFO_CONFIG* config);
int8_t _APPLICATION_CAN_TransmitQueueConfigure(CANFDSPI_MODULE_ID index,CAN_TX_QUEUE_CONFIG* config);
int8_t _APPLICATION_CAN_TransmitQueueConfigureObjectReset(CAN_TX_QUEUE_CONFIG* config);
int8_t _APPLICATION_CAN_TransmitChannelLoad(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_TX_MSGOBJ* txObj,uint8_t *txd, uint32_t txdNumBytes, bool flush);
int8_t _APPLICATION_CAN_TransmitChannelFlush(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel);
int8_t _APPLICATION_CAN_TransmitChannelStatusGet(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_STATUS* status);
int8_t _APPLICATION_CAN_TransmitChannelReset(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel);
int8_t _APPLICATION_CAN_TransmitChannelUpdate(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, bool flush);
int8_t _APPLICATION_CAN_TransmitRequestSet(CANFDSPI_MODULE_ID index,CAN_TXREQ_CHANNEL txreq);
int8_t _APPLICATION_CAN_TransmitRequestGet(CANFDSPI_MODULE_ID index,uint32_t* txreq);
int8_t _APPLICATION_CAN_TransmitChannelAbort(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel);
int8_t _APPLICATION_CAN_TransmitAbortAll(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_TransmitBandWidthSharingSet(CANFDSPI_MODULE_ID index,CAN_TX_BANDWITH_SHARING txbws);
int8_t _APPLICATION_CAN_FilterObjectConfigure(CANFDSPI_MODULE_ID index,CAN_FILTER filter, CAN_FILTEROBJ_ID* id);
int8_t _APPLICATION_CAN_FilterMaskConfigure(CANFDSPI_MODULE_ID index,CAN_FILTER filter, CAN_MASKOBJ_ID* mask);
int8_t _APPLICATION_CAN_FilterToFifoLink(CANFDSPI_MODULE_ID index,CAN_FILTER filter, CAN_FIFO_CHANNEL channel, bool enable);
int8_t _APPLICATION_CAN_FilterEnable(CANFDSPI_MODULE_ID index, CAN_FILTER filter);
int8_t _APPLICATION_CAN_FilterDisable(CANFDSPI_MODULE_ID index, CAN_FILTER filter);
int8_t _APPLICATION_CAN_DeviceNetFilterCountSet(CANFDSPI_MODULE_ID index,CAN_DNET_FILTER_SIZE dnfc);
int8_t _APPLICATION_CAN_ReceiveChannelConfigure(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_CONFIG* config);
int8_t _APPLICATION_CAN_ReceiveChannelConfigureObjectReset(CAN_RX_FIFO_CONFIG* config);
int8_t _APPLICATION_CAN_ReceiveChannelStatusGet(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_STATUS* status);
int8_t _APPLICATION_CAN_ReceiveMessageGet(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_RX_MSGOBJ* rxObj,uint8_t *rxd, uint8_t nBytes);
int8_t _APPLICATION_CAN_ReceiveChannelReset(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel);
int8_t _APPLICATION_CAN_ReceiveChannelUpdate(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel);
int8_t _APPLICATION_CAN_TefStatusGet(CANFDSPI_MODULE_ID index,CAN_TEF_FIFO_STATUS* status);
int8_t _APPLICATION_CAN_TefMessageGet(CANFDSPI_MODULE_ID index,CAN_TEF_MSGOBJ* tefObj);
int8_t _APPLICATION_CAN_TefReset(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_TefUpdate(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_TefConfigure(CANFDSPI_MODULE_ID index, CAN_TEF_CONFIG* config);
int8_t _APPLICATION_CAN_TefConfigureObjectReset(CAN_TEF_CONFIG* config);
int8_t _APPLICATION_CAN_ModuleEventGet(CANFDSPI_MODULE_ID index,CAN_MODULE_EVENT* flags);
int8_t _APPLICATION_CAN_ModuleEventEnable(CANFDSPI_MODULE_ID index,CAN_MODULE_EVENT flags);
int8_t _APPLICATION_CAN_ModuleEventDisable(CANFDSPI_MODULE_ID index,CAN_MODULE_EVENT flags);
int8_t _APPLICATION_CAN_ModuleEventClear(CANFDSPI_MODULE_ID index,CAN_MODULE_EVENT flags);
int8_t _APPLICATION_CAN_ModuleEventRxCodeGet(CANFDSPI_MODULE_ID index,CAN_RXCODE* rxCode);
int8_t _APPLICATION_CAN_ModuleEventTxCodeGet(CANFDSPI_MODULE_ID index,CAN_TXCODE* txCode);
int8_t _APPLICATION_CAN_ModuleEventFilterHitGet(CANFDSPI_MODULE_ID index,CAN_FILTER* filterHit);
int8_t _APPLICATION_CAN_ModuleEventIcodeGet(CANFDSPI_MODULE_ID index,CAN_ICODE* icode);
int8_t _APPLICATION_CAN_TransmitChannelEventGet(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_EVENT* flags);
int8_t _APPLICATION_CAN_TransmitEventGet(CANFDSPI_MODULE_ID index, uint32_t* txif);
int8_t _APPLICATION_CAN_TransmitEventAttemptGet(CANFDSPI_MODULE_ID index,uint32_t* txatif);
int8_t _APPLICATION_CAN_TransmitChannelIndexGet(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, uint8_t* idx);
int8_t _APPLICATION_CAN_TransmitChannelEventEnable(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_EVENT flags);
int8_t _APPLICATION_CAN_TransmitChannelEventDisable(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_EVENT flags);
int8_t _APPLICATION_CAN_TransmitChannelEventAttemptClear(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel);
int8_t _APPLICATION_CAN_ReceiveChannelEventGet(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_EVENT* flags);
int8_t _APPLICATION_CAN_ReceiveEventGet(CANFDSPI_MODULE_ID index, uint32_t* rxif);
int8_t _APPLICATION_CAN_ReceiveEventOverflowGet(CANFDSPI_MODULE_ID index,uint32_t* rxovif);
int8_t _APPLICATION_CAN_ReceiveChannelIndexGet(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, uint8_t* idx);
int8_t _APPLICATION_CAN_ReceiveChannelEventEnable(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_EVENT flags);
int8_t _APPLICATION_CAN_ReceiveChannelEventDisable(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_EVENT flags);
int8_t _APPLICATION_CAN_ReceiveChannelEventOverflowClear(CANFDSPI_MODULE_ID index,CAN_FIFO_CHANNEL channel);
int8_t _APPLICATION_CAN_TefEventGet(CANFDSPI_MODULE_ID index,CAN_TEF_FIFO_EVENT* flags);
int8_t _APPLICATION_CAN_TefEventEnable(CANFDSPI_MODULE_ID index,CAN_TEF_FIFO_EVENT flags);
int8_t _APPLICATION_CAN_TefEventDisable(CANFDSPI_MODULE_ID index,CAN_TEF_FIFO_EVENT flags);
int8_t _APPLICATION_CAN_TefEventOverflowClear(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_ErrorCountTransmitGet(CANFDSPI_MODULE_ID index,uint8_t* tec);
int8_t _APPLICATION_CAN_ErrorCountReceiveGet(CANFDSPI_MODULE_ID index,uint8_t* rec);
int8_t _APPLICATION_CAN_ErrorStateGet(CANFDSPI_MODULE_ID index,CAN_ERROR_STATE* flags);
int8_t _APPLICATION_CAN_ErrorCountStateGet(CANFDSPI_MODULE_ID index,uint8_t* tec, uint8_t* rec, CAN_ERROR_STATE* flags);
int8_t _APPLICATION_CAN_BusDiagnosticsGet(CANFDSPI_MODULE_ID index,CAN_BUS_DIAGNOSTIC* bd);
int8_t _APPLICATION_CAN_BusDiagnosticsClear(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_EccEnable(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_EccDisable(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_EccEventGet(CANFDSPI_MODULE_ID index,CAN_ECC_EVENT* flags);
int8_t _APPLICATION_CAN_EccParitySet(CANFDSPI_MODULE_ID index,uint8_t parity);
int8_t _APPLICATION_CAN_EccParityGet(CANFDSPI_MODULE_ID index,uint8_t* parity);
int8_t _APPLICATION_CAN_EccErrorAddressGet(CANFDSPI_MODULE_ID index,uint16_t* a);
int8_t _APPLICATION_CAN_EccEventEnable(CANFDSPI_MODULE_ID index,CAN_ECC_EVENT flags);
int8_t _APPLICATION_CAN_EccEventDisable(CANFDSPI_MODULE_ID index,CAN_ECC_EVENT flags);
int8_t _APPLICATION_CAN_EccEventClear(CANFDSPI_MODULE_ID index,CAN_ECC_EVENT flags);
int8_t _APPLICATION_CAN_CrcEventEnable(CANFDSPI_MODULE_ID index,CAN_CRC_EVENT flags);
int8_t _APPLICATION_CAN_CrcEventDisable(CANFDSPI_MODULE_ID index,CAN_CRC_EVENT flags);
int8_t _APPLICATION_CAN_CrcEventClear(CANFDSPI_MODULE_ID index,CAN_CRC_EVENT flags);
int8_t _APPLICATION_CAN_CrcEventGet(CANFDSPI_MODULE_ID index, CAN_CRC_EVENT* flags);
int8_t _APPLICATION_CAN_CrcValueGet(CANFDSPI_MODULE_ID index, uint16_t* crc);
int8_t _APPLICATION_CAN_RamInit(CANFDSPI_MODULE_ID index, uint8_t d);
int8_t _APPLICATION_CAN_TimeStampEnable(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_TimeStampDisable(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_TimeStampGet(CANFDSPI_MODULE_ID index, uint32_t* ts);
int8_t _APPLICATION_CAN_TimeStampSet(CANFDSPI_MODULE_ID index, uint32_t ts);
int8_t _APPLICATION_CAN_TimeStampModeConfigure(CANFDSPI_MODULE_ID index,CAN_TS_MODE mode);
int8_t _APPLICATION_CAN_TimeStampPrescalerSet(CANFDSPI_MODULE_ID index,uint16_t ps);
int8_t _APPLICATION_CAN_OscillatorEnable(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_OscillatorControlSet(CANFDSPI_MODULE_ID index,CAN_OSC_CTRL ctrl);
int8_t _APPLICATION_CAN_OscillatorControlObjectReset(CAN_OSC_CTRL* ctrl);
int8_t _APPLICATION_CAN_OscillatorStatusGet(CANFDSPI_MODULE_ID index,CAN_OSC_STATUS* status);
int8_t _APPLICATION_CAN_BitTimeConfigure(CANFDSPI_MODULE_ID index,CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode,CAN_SYSCLK_SPEED clk);
int8_t _APPLICATION_CAN_BitTimeConfigureNominal40MHz(CANFDSPI_MODULE_ID index,CAN_BITTIME_SETUP bitTime);
int8_t _APPLICATION_CAN_BitTimeConfigureData40MHz(CANFDSPI_MODULE_ID index,CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode);
int8_t _APPLICATION_CAN_BitTimeConfigureNominal20MHz(CANFDSPI_MODULE_ID index,CAN_BITTIME_SETUP bitTime);
int8_t _APPLICATION_CAN_BitTimeConfigureData20MHz(CANFDSPI_MODULE_ID index,CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode);
int8_t _APPLICATION_CAN_BitTimeConfigureNominal10MHz(CANFDSPI_MODULE_ID index,CAN_BITTIME_SETUP bitTime);
int8_t _APPLICATION_CAN_BitTimeConfigureData10MHz(CANFDSPI_MODULE_ID index,CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode);
int8_t _APPLICATION_CAN_GpioModeConfigure(CANFDSPI_MODULE_ID index,GPIO_PIN_MODE gpio0, GPIO_PIN_MODE gpio1);
int8_t _APPLICATION_CAN_GpioDirectionConfigure(CANFDSPI_MODULE_ID index,GPIO_PIN_DIRECTION gpio0, GPIO_PIN_DIRECTION gpio1);
int8_t _APPLICATION_CAN_GpioStandbyControlEnable(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_GpioStandbyControlDisable(CANFDSPI_MODULE_ID index);
int8_t _APPLICATION_CAN_GpioInterruptPinsOpenDrainConfigure(CANFDSPI_MODULE_ID index,GPIO_OPEN_DRAIN_MODE mode);
int8_t _APPLICATION_CAN_GpioTransmitPinOpenDrainConfigure(CANFDSPI_MODULE_ID index, GPIO_OPEN_DRAIN_MODE mode);
int8_t _APPLICATION_CAN_GpioPinSet(CANFDSPI_MODULE_ID index,GPIO_PIN_POS pos, GPIO_PIN_STATE latch);
int8_t _APPLICATION_CAN_GpioPinRead(CANFDSPI_MODULE_ID index, GPIO_PIN_POS pos, GPIO_PIN_STATE* state);
int8_t _APPLICATION_CAN_GpioClockOutputConfigure(CANFDSPI_MODULE_ID index, GPIO_CLKO_MODE mode);
uint16_t _APPLICATION_CAN_CalculateCRC16(uint8_t* data, uint16_t size);
uint32_t _APPLICATION_CAN_DlcToDataBytes(CAN_DLC dlc);
int8_t _APPLICATION_CAN_FifoIndexGet(CANFDSPI_MODULE_ID index, CAN_FIFO_CHANNEL channel, uint8_t* mi);
CAN_DLC _APPLICATION_CAN_DataBytesToDlc(uint8_t n);
void _APPLICATION_CAN_ReceiveCallback(void);
void _APPLICATION_CAN_Init();
void _APPLICATION_CAN_TransmitMessageQueue(CAN_MSG_t *canMsg);

// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Section: Reset">
int8_t _APPLICATION_CAN_TransferData(CANFDSPI_MODULE_ID index, void* txb, void* rxb, uint16_t txs)
{
    bool isSuccess = DRV_SPI_WriteReadTransfer(index, txb, txs, rxb, txs);
    return !isSuccess;
}

int8_t _APPLICATION_CAN_Reset(CANFDSPI_MODULE_ID index)
{
    uint16_t spiTransferSize = 2;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) (cINSTRUCTION_RESET << 4);
    spiTransmitBuffer[1] = 0;

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: SPI Access Functions">

int8_t _APPLICATION_CAN_ReadByte(CANFDSPI_MODULE_ID index, uint16_t address, uint8_t *rxd)
{
    uint16_t spiTransferSize = 3;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
    spiTransmitBuffer[2] = 0;

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    // Update data
    *rxd = spiReceiveBuffer[2];

    return spiTransferError;
}

int8_t _APPLICATION_CAN_WriteByte(CANFDSPI_MODULE_ID index, uint16_t address, uint8_t txd)
{
    uint16_t spiTransferSize = 3;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
    spiTransmitBuffer[2] = txd;

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReadWord(CANFDSPI_MODULE_ID index, uint16_t address, uint32_t *rxd)
{
    uint8_t i;
    uint32_t x;
    uint16_t spiTransferSize = 6;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
    if (spiTransferError) {
        return spiTransferError;
    }

    // Update data
    *rxd = 0;
    for (i = 2; i < 6; i++) {
        x = (uint32_t) spiReceiveBuffer[i];
        *rxd += x << ((i - 2)*8);
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_WriteWord(CANFDSPI_MODULE_ID index, uint16_t address,
        uint32_t txd)
{
    uint8_t i;
    uint16_t spiTransferSize = 6;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);

    // Split word into 4 bytes and add them to buffer
    for (i = 0; i < 4; i++) {
        spiTransmitBuffer[i + 2] = (uint8_t) ((txd >> (i * 8)) & 0xFF);
    }

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReadHalfWord(CANFDSPI_MODULE_ID index, uint16_t address, uint16_t *rxd)
{
    uint8_t i;
    uint32_t x;
    uint16_t spiTransferSize = 4;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
    if (spiTransferError) {
        return spiTransferError;
    }

    // Update data
    *rxd = 0;
    for (i = 2; i < 4; i++) {
        x = (uint32_t) spiReceiveBuffer[i];
        *rxd += x << ((i - 2)*8);
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_WriteHalfWord(CANFDSPI_MODULE_ID index, uint16_t address,
        uint16_t txd)
{
    uint8_t i;
    uint16_t spiTransferSize = 4;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);

    // Split word into 2 bytes and add them to buffer
    for (i = 0; i < 2; i++) {
        spiTransmitBuffer[i + 2] = (uint8_t) ((txd >> (i * 8)) & 0xFF);
    }

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_WriteByteSafe(CANFDSPI_MODULE_ID index, uint16_t address,
        uint8_t txd)
{
    uint16_t crcResult = 0;
    uint16_t spiTransferSize = 5;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE_SAFE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
    spiTransmitBuffer[2] = txd;

    // Add CRC
    crcResult = _APPLICATION_CAN_CalculateCRC16(spiTransmitBuffer, 3);
    spiTransmitBuffer[3] = (crcResult >> 8) & 0xFF;
    spiTransmitBuffer[4] = crcResult & 0xFF;

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_WriteWordSafe(CANFDSPI_MODULE_ID index, uint16_t address,
        uint32_t txd)
{
    uint8_t i;
    uint16_t crcResult = 0;
    uint16_t spiTransferSize = 8;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE_SAFE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);

    // Split word into 4 bytes and add them to buffer
    for (i = 0; i < 4; i++) {
        spiTransmitBuffer[i + 2] = (uint8_t) ((txd >> (i * 8)) & 0xFF);
    }

    // Add CRC
    crcResult = _APPLICATION_CAN_CalculateCRC16(spiTransmitBuffer, 6);
    spiTransmitBuffer[6] = (crcResult >> 8) & 0xFF;
    spiTransmitBuffer[7] = crcResult & 0xFF;

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReadByteArray(CANFDSPI_MODULE_ID index, uint16_t address,
        uint8_t *rxd, uint16_t nBytes)
{
    uint16_t i;
    uint16_t spiTransferSize = nBytes + 2;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);

    // Clear data
    for (i = 2; i < spiTransferSize; i++) {
        spiTransmitBuffer[i] = 0;
    }

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    // Update data
    for (i = 0; i < nBytes; i++) {
        rxd[i] = spiReceiveBuffer[i + 2];
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReadByteArrayWithCRC(CANFDSPI_MODULE_ID index, uint16_t address,
        uint8_t *rxd, uint16_t nBytes, bool fromRam, bool* crcIsCorrect)
{
    uint8_t i;
    uint16_t crcFromSpiSlave = 0;
    uint16_t crcAtController = 0;
    uint16_t spiTransferSize = nBytes + 5; //first two bytes for sending command & address, third for size, last two bytes for CRC
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ_CRC << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
    if (fromRam) {
        spiTransmitBuffer[2] = nBytes >> 2;
    } else {
        spiTransmitBuffer[2] = nBytes;
    }

    // Clear data
    for (i = 3; i < spiTransferSize; i++) {
        spiTransmitBuffer[i] = 0;
    }

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
    if (spiTransferError) {
        return spiTransferError;
    }

    // Get CRC from controller
    crcFromSpiSlave = (uint16_t) (spiReceiveBuffer[spiTransferSize - 2] << 8) + (uint16_t) (spiReceiveBuffer[spiTransferSize - 1]);

    // Use the receive buffer to calculate CRC
    // First three bytes need to be command
    spiReceiveBuffer[0] = spiTransmitBuffer[0];
    spiReceiveBuffer[1] = spiTransmitBuffer[1];
    spiReceiveBuffer[2] = spiTransmitBuffer[2];
    crcAtController = _APPLICATION_CAN_CalculateCRC16(spiReceiveBuffer, nBytes + 3);

    // Compare CRC readings
    if (crcFromSpiSlave == crcAtController) {
        *crcIsCorrect = true;
    } else {
        *crcIsCorrect = false;
    }

    // Update data
    for (i = 0; i < nBytes; i++) {
        rxd[i] = spiReceiveBuffer[i + 3];
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_WriteByteArray(CANFDSPI_MODULE_ID index, uint16_t address,
        uint8_t *txd, uint16_t nBytes)
{
    uint16_t i;
    uint16_t spiTransferSize = nBytes + 2;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);

    // Add data
    for (i = 2; i < spiTransferSize; i++) {
        spiTransmitBuffer[i] = txd[i - 2];
    }

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_WriteByteArrayWithCRC(CANFDSPI_MODULE_ID index, uint16_t address,
        uint8_t *txd, uint16_t nBytes, bool fromRam)
{
    uint16_t i;
    uint16_t crcResult = 0;
    uint16_t spiTransferSize = nBytes + 5;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE_CRC << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
    if (fromRam) {
        spiTransmitBuffer[2] = nBytes >> 2;
    } else {
        spiTransmitBuffer[2] = nBytes;
    }

    // Add data
    for (i = 0; i < nBytes; i++) {
        spiTransmitBuffer[i + 3] = txd[i];
    }

    // Add CRC
    crcResult = _APPLICATION_CAN_CalculateCRC16(spiTransmitBuffer, spiTransferSize - 2);
    spiTransmitBuffer[spiTransferSize - 2] = (uint8_t) ((crcResult >> 8) & 0xFF);
    spiTransmitBuffer[spiTransferSize - 1] = (uint8_t) (crcResult & 0xFF);

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReadWordArray(CANFDSPI_MODULE_ID index, uint16_t address,
        uint32_t *rxd, uint16_t nWords)
{
    uint16_t i, j, n;
    REG_t w;
    uint16_t spiTransferSize = nWords * 4 + 2;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF);
    spiTransmitBuffer[1] = address & 0xFF;

    // Clear data
    for (i = 2; i < spiTransferSize; i++) {
        spiTransmitBuffer[i] = 0;
    }

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
    if (spiTransferError) {
        return spiTransferError;
    }

    // Convert Byte array to Word array
    n = 2;
    for (i = 0; i < nWords; i++) {
        w.word = 0;
        for (j = 0; j < 4; j++, n++) {
            w.byte[j] = spiReceiveBuffer[n];
        }
        rxd[i] = w.word;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_WriteWordArray(CANFDSPI_MODULE_ID index, uint16_t address,
        uint32_t *txd, uint16_t nWords)
{
    uint16_t i, j, n;
    REG_t w;
    uint16_t spiTransferSize = nWords * 4 + 2;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF);
    spiTransmitBuffer[1] = address & 0xFF;

    // Convert ByteArray to word array
    n = 2;
    for (i = 0; i < nWords; i++) {
        w.word = txd[i];
        for (j = 0; j < 4; j++, n++) {
            spiTransmitBuffer[n] = w.byte[j];
        }
    }

    spiTransferError = _APPLICATION_CAN_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: Configuration">
int8_t _APPLICATION_CAN_Configure(CANFDSPI_MODULE_ID index, CAN_CONFIG* config)
{
    REG_CiCON ciCon;
    int8_t spiTransferError = 0;

    ciCon.word = canControlResetValues[cREGADDR_CiCON / 4];

    ciCon.bF.DNetFilterCount = config->DNetFilterCount;
    ciCon.bF.IsoCrcEnable = config->IsoCrcEnable;
    ciCon.bF.ProtocolExceptionEventDisable = config->ProtocolExpectionEventDisable;
    ciCon.bF.WakeUpFilterEnable = config->WakeUpFilterEnable;
    ciCon.bF.WakeUpFilterTime = config->WakeUpFilterTime;
    ciCon.bF.BitRateSwitchDisable = config->BitRateSwitchDisable;
    ciCon.bF.RestrictReTxAttempts = config->RestrictReTxAttempts;
    ciCon.bF.EsiInGatewayMode = config->EsiInGatewayMode;
    ciCon.bF.SystemErrorToListenOnly = config->SystemErrorToListenOnly;
    ciCon.bF.StoreInTEF = config->StoreInTEF;
    ciCon.bF.TXQEnable = config->TXQEnable;
    ciCon.bF.TxBandWidthSharing = config->TxBandWidthSharing;

    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiCON, ciCon.word);
    if (spiTransferError) {
        return -1;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ConfigureObjectReset(CAN_CONFIG* config)
{
    REG_CiCON ciCon;
    ciCon.word = canControlResetValues[cREGADDR_CiCON / 4];

    config->DNetFilterCount = ciCon.bF.DNetFilterCount;
    config->IsoCrcEnable = ciCon.bF.IsoCrcEnable;
    config->ProtocolExpectionEventDisable = ciCon.bF.ProtocolExceptionEventDisable;
    config->WakeUpFilterEnable = ciCon.bF.WakeUpFilterEnable;
    config->WakeUpFilterTime = ciCon.bF.WakeUpFilterTime;
    config->BitRateSwitchDisable = ciCon.bF.BitRateSwitchDisable;
    config->RestrictReTxAttempts = ciCon.bF.RestrictReTxAttempts;
    config->EsiInGatewayMode = ciCon.bF.EsiInGatewayMode;
    config->SystemErrorToListenOnly = ciCon.bF.SystemErrorToListenOnly;
    config->StoreInTEF = ciCon.bF.StoreInTEF;
    config->TXQEnable = ciCon.bF.TXQEnable;
    config->TxBandWidthSharing = ciCon.bF.TxBandWidthSharing;

    return 0;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: Operating Mode">

int8_t _APPLICATION_CAN_OperationModeSelect(CANFDSPI_MODULE_ID index,
        CAN_OPERATION_MODE opMode)
{
    uint8_t d = 0;
    int8_t spiTransferError = 0;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_CiCON + 3, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d &= ~0x07;
    d |= opMode;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_CiCON + 3, d);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

CAN_OPERATION_MODE _APPLICATION_CAN_OperationModeGet(CANFDSPI_MODULE_ID index)
{
    uint8_t d = 0;
    CAN_OPERATION_MODE mode = CAN_INVALID_MODE;
    int8_t spiTransferError = 0;

    // Read Opmode
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_CiCON + 2, &d);
    if (spiTransferError) {
        return CAN_INVALID_MODE;
    }

    // Get Opmode bits
    d = (d >> 5) & 0x7;

    // Decode Opmode
    switch (d) {
        case CAN_NORMAL_MODE:
            mode = CAN_NORMAL_MODE;
            break;
        case CAN_SLEEP_MODE:
            mode = CAN_SLEEP_MODE;
            break;
        case CAN_INTERNAL_LOOPBACK_MODE:
            mode = CAN_INTERNAL_LOOPBACK_MODE;
            break;
        case CAN_EXTERNAL_LOOPBACK_MODE:
            mode = CAN_EXTERNAL_LOOPBACK_MODE;
            break;
        case CAN_LISTEN_ONLY_MODE:
            mode = CAN_LISTEN_ONLY_MODE;
            break;
        case CAN_CONFIGURATION_MODE:
            mode = CAN_CONFIGURATION_MODE;
            break;
        case CAN_CLASSIC_MODE:
            mode = CAN_CLASSIC_MODE;
            break;
        case CAN_RESTRICTED_MODE:
            mode = CAN_RESTRICTED_MODE;
            break;
        default:
            mode = CAN_INVALID_MODE;
            break;
    }

    return mode;
}

int8_t _APPLICATION_CAN_LowPowerModeEnable(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint8_t d = 0;

#ifdef MCP2517FD
    // LPM not implemented
    spiTransferError = -100;
#else
    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_OSC, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d |= 0x08;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_OSC, d);
    if (spiTransferError) {
        return -2;
    }
#endif

    return spiTransferError;
}

int8_t _APPLICATION_CAN_LowPowerModeDisable(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint8_t d = 0;

#ifdef MCP2517FD
    // LPM not implemented
    spiTransferError = -100;
#else
    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_OSC, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d &= ~0x08;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_OSC, d);
    if (spiTransferError) {
        return -2;
    }
#endif

    return spiTransferError;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: CAN Transmit">

int8_t _APPLICATION_CAN_TransmitChannelConfigure(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_CONFIG* config)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Setup FIFO
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canFifoResetValues[0];

    ciFifoCon.txBF.TxEnable = 1;
    ciFifoCon.txBF.FifoSize = config->FifoSize;
    ciFifoCon.txBF.PayLoadSize = config->PayLoadSize;
    ciFifoCon.txBF.TxAttempts = config->TxAttempts;
    ciFifoCon.txBF.TxPriority = config->TxPriority;
    ciFifoCon.txBF.RTREnable = config->RTREnable;

    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = _APPLICATION_CAN_WriteWord(index, a, ciFifoCon.word);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitChannelConfigureObjectReset(CAN_TX_FIFO_CONFIG* config)
{
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canFifoResetValues[0];

    config->RTREnable = ciFifoCon.txBF.RTREnable;
    config->TxPriority = ciFifoCon.txBF.TxPriority;
    config->TxAttempts = ciFifoCon.txBF.TxAttempts;
    config->FifoSize = ciFifoCon.txBF.FifoSize;
    config->PayLoadSize = ciFifoCon.txBF.PayLoadSize;

    return 0;
}

int8_t _APPLICATION_CAN_TransmitQueueConfigure(CANFDSPI_MODULE_ID index,
        CAN_TX_QUEUE_CONFIG* config)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Setup FIFO
    REG_CiTXQCON ciFifoCon;
    ciFifoCon.word = canFifoResetValues[0];

    ciFifoCon.txBF.TxEnable = 1;
    ciFifoCon.txBF.FifoSize = config->FifoSize;
    ciFifoCon.txBF.PayLoadSize = config->PayLoadSize;
    ciFifoCon.txBF.TxAttempts = config->TxAttempts;
    ciFifoCon.txBF.TxPriority = config->TxPriority;

    a = cREGADDR_CiTXQCON;
    spiTransferError = _APPLICATION_CAN_WriteWord(index, a, ciFifoCon.word);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitQueueConfigureObjectReset(CAN_TX_QUEUE_CONFIG* config)
{
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canFifoResetValues[0];

    config->TxPriority = ciFifoCon.txBF.TxPriority;
    config->TxAttempts = ciFifoCon.txBF.TxAttempts;
    config->FifoSize = ciFifoCon.txBF.FifoSize;
    config->PayLoadSize = ciFifoCon.txBF.PayLoadSize;

    return 0;
}

int8_t _APPLICATION_CAN_TransmitChannelLoad(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_TX_MSGOBJ* txObj,
        uint8_t *txd, uint32_t txdNumBytes, bool flush)
{
    uint16_t a;
    uint32_t fifoReg[3];
    uint32_t dataBytesInObject;
    REG_CiFIFOCON ciFifoCon;
    __attribute__((unused)) REG_CiFIFOSTA ciFifoSta;
    REG_CiFIFOUA ciFifoUa;
    int8_t spiTransferError = 0;

    // Get FIFO registers
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = _APPLICATION_CAN_ReadWordArray(index, a, fifoReg, 3);
    if (spiTransferError) {
        return -1;
    }

    // Check that it is a transmit buffer
    ciFifoCon.word = fifoReg[0];
    if (!ciFifoCon.txBF.TxEnable) {
        return -2;
    }

    // Check that DLC is big enough for data
    dataBytesInObject = _APPLICATION_CAN_DlcToDataBytes((CAN_DLC) txObj->bF.ctrl.DLC);
    if (dataBytesInObject < txdNumBytes) {
        return -3;
    }

    // Get status
    ciFifoSta.word = fifoReg[1];

    // Get address
    ciFifoUa.word = fifoReg[2];
#ifdef USERADDRESS_TIMES_FOUR
    a = 4 * ciFifoUa.bF.UserAddress;
#else
    a = ciFifoUa.bF.UserAddress;
#endif
    a += cRAMADDR_START;

    uint8_t txBuffer[MAX_MSG_SIZE];

    txBuffer[0] = txObj->byte[0]; //not using 'for' to reduce no of instructions
    txBuffer[1] = txObj->byte[1];
    txBuffer[2] = txObj->byte[2];
    txBuffer[3] = txObj->byte[3];

    txBuffer[4] = txObj->byte[4];
    txBuffer[5] = txObj->byte[5];
    txBuffer[6] = txObj->byte[6];
    txBuffer[7] = txObj->byte[7];

    uint8_t i;
    for (i = 0; i < txdNumBytes; i++) {
        txBuffer[i + 8] = txd[i];
    }

    // Make sure we write a multiple of 4 bytes to RAM
    uint16_t n = 0;
    uint8_t j = 0;

    if (txdNumBytes % 4) {
        // Need to add bytes
        n = 4 - (txdNumBytes % 4);
        i = txdNumBytes + 8;

        for (j = 0; j < n; j++) {
            txBuffer[i + 8 + j] = 0;
        }
    }

    spiTransferError = _APPLICATION_CAN_WriteByteArray(index, a, txBuffer, txdNumBytes + 8 + n);
    if (spiTransferError) {
        return -4;
    }

    // Set UINC and TXREQ
    spiTransferError = _APPLICATION_CAN_TransmitChannelUpdate(index, channel, flush);
    if (spiTransferError) {
        return -5;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitChannelFlush(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel)
{
    uint8_t d = 0;
    uint16_t a = 0;
    int8_t spiTransferError = 0;

    // Address of TXREQ
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
    a += 1;

    // Set TXREQ
    d = 0x02;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, d);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitChannelStatusGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_STATUS* status)
{
    uint16_t a = 0;
    uint32_t sta = 0;
    uint32_t fifoReg[2];
    REG_CiFIFOSTA ciFifoSta;
    REG_CiFIFOCON ciFifoCon;
    int8_t spiTransferError = 0;

    // Get FIFO registers
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = _APPLICATION_CAN_ReadWordArray(index, a, fifoReg, 2);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    ciFifoCon.word = fifoReg[0];
    ciFifoSta.word = fifoReg[1];

    // Update status
    sta = ciFifoSta.byte[0];

    if (ciFifoCon.txBF.TxRequest) {
        sta |= CAN_TX_FIFO_TRANSMITTING;
    }

    *status = (CAN_TX_FIFO_STATUS) (sta & CAN_TX_FIFO_STATUS_MASK);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitChannelReset(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel)
{
    return _APPLICATION_CAN_ReceiveChannelReset(index, channel);
}

int8_t _APPLICATION_CAN_TransmitChannelUpdate(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, bool flush)
{
    uint16_t a;
    REG_CiFIFOCON ciFifoCon;
    int8_t spiTransferError = 0;

    // Set UINC
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
    ciFifoCon.word = 0;
    ciFifoCon.txBF.UINC = 1;

    // Set TXREQ
    if (flush) {
        ciFifoCon.txBF.TxRequest = 1;
    }

    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciFifoCon.byte[1]);
    if (spiTransferError) {
        return -1;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitRequestSet(CANFDSPI_MODULE_ID index,
        CAN_TXREQ_CHANNEL txreq)
{
    int8_t spiTransferError = 0;

    // Write TXREQ register
    uint32_t w = txreq;

    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiTXREQ, w);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitRequestGet(CANFDSPI_MODULE_ID index,
        uint32_t* txreq)
{
    int8_t spiTransferError = 0;

    spiTransferError = _APPLICATION_CAN_ReadWord(index, cREGADDR_CiTXREQ, txreq);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitChannelAbort(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel)
{
    uint16_t a;
    uint8_t d;
    int8_t spiTransferError = 0;

    // Address
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
    a += 1; // byte address of TXREQ

    // Clear TXREQ
    d = 0x00;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, d);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitAbortAll(CANFDSPI_MODULE_ID index)
{
    uint8_t d;
    int8_t spiTransferError = 0;

    // Read CiCON byte 3
    spiTransferError = _APPLICATION_CAN_ReadByte(index, (cREGADDR_CiCON + 3), &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d |= 0x8;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, (cREGADDR_CiCON + 3), d);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitBandWidthSharingSet(CANFDSPI_MODULE_ID index,
        CAN_TX_BANDWITH_SHARING txbws)
{
    uint8_t d = 0;
    int8_t spiTransferError = 0;

    // Read CiCON byte 3
    spiTransferError = _APPLICATION_CAN_ReadByte(index, (cREGADDR_CiCON + 3), &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d &= 0x0f;
    d |= (txbws << 4);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, (cREGADDR_CiCON + 3), d);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: CAN Receive">

int8_t _APPLICATION_CAN_FilterObjectConfigure(CANFDSPI_MODULE_ID index,
        CAN_FILTER filter, CAN_FILTEROBJ_ID* id)
{
    uint16_t a;
    REG_CiFLTOBJ fObj;
    int8_t spiTransferError = 0;

    // Setup
    fObj.word = 0;
    fObj.bF = *id;
    a = cREGADDR_CiFLTOBJ + (filter * CiFILTER_OFFSET);

    spiTransferError = _APPLICATION_CAN_WriteWord(index, a, fObj.word);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_FilterMaskConfigure(CANFDSPI_MODULE_ID index,
        CAN_FILTER filter, CAN_MASKOBJ_ID* mask)
{
    uint16_t a;
    REG_CiMASK mObj;
    int8_t spiTransferError = 0;

    // Setup
    mObj.word = 0;
    mObj.bF = *mask;
    a = cREGADDR_CiMASK + (filter * CiFILTER_OFFSET);

    spiTransferError = _APPLICATION_CAN_WriteWord(index, a, mObj.word);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_FilterToFifoLink(CANFDSPI_MODULE_ID index,
        CAN_FILTER filter, CAN_FIFO_CHANNEL channel, bool enable)
{
    uint16_t a;
    REG_CiFLTCON_BYTE fCtrl;
    int8_t spiTransferError = 0;

    // Enable
    if (enable) {
        fCtrl.bF.Enable = 1;
    } else {
        fCtrl.bF.Enable = 0;
    }

    // Link
    fCtrl.bF.BufferPointer = channel;
    a = cREGADDR_CiFLTCON + filter;

    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, fCtrl.byte);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_FilterEnable(CANFDSPI_MODULE_ID index, CAN_FILTER filter)
{
    uint16_t a;
    REG_CiFLTCON_BYTE fCtrl;
    int8_t spiTransferError = 0;

    // Read
    a = cREGADDR_CiFLTCON + filter;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &fCtrl.byte);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    fCtrl.bF.Enable = 1;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, fCtrl.byte);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_FilterDisable(CANFDSPI_MODULE_ID index, CAN_FILTER filter)
{
    uint16_t a;
    REG_CiFLTCON_BYTE fCtrl;
    int8_t spiTransferError = 0;

    // Read
    a = cREGADDR_CiFLTCON + filter;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &fCtrl.byte);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    fCtrl.bF.Enable = 0;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, fCtrl.byte);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_DeviceNetFilterCountSet(CANFDSPI_MODULE_ID index,
        CAN_DNET_FILTER_SIZE dnfc)
{
    uint8_t d = 0;
    int8_t spiTransferError = 0;

    // Read CiCON byte 0
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_CiCON, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d &= 0x1f;
    d |= dnfc;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_CiCON, d);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReceiveChannelConfigure(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_CONFIG* config)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    if (channel == CAN_TXQUEUE_CH0) {
        return -100;
    }

    // Setup FIFO
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canFifoResetValues[0];

    ciFifoCon.rxBF.TxEnable = 0;
    ciFifoCon.rxBF.FifoSize = config->FifoSize;
    ciFifoCon.rxBF.PayLoadSize = config->PayLoadSize;
    ciFifoCon.rxBF.RxTimeStampEnable = config->RxTimeStampEnable;

    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = _APPLICATION_CAN_WriteWord(index, a, ciFifoCon.word);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReceiveChannelConfigureObjectReset(CAN_RX_FIFO_CONFIG* config)
{
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canFifoResetValues[0];

    config->FifoSize = ciFifoCon.rxBF.FifoSize;
    config->PayLoadSize = ciFifoCon.rxBF.PayLoadSize;
    config->RxTimeStampEnable = ciFifoCon.rxBF.RxTimeStampEnable;

    return 0;
}

int8_t _APPLICATION_CAN_ReceiveChannelStatusGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_STATUS* status)
{
    uint16_t a;
    REG_CiFIFOSTA ciFifoSta;
    int8_t spiTransferError = 0;

    // Read
    ciFifoSta.word = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *status = (CAN_RX_FIFO_STATUS) (ciFifoSta.byte[0] & 0x0F);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReceiveMessageGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_MSGOBJ* rxObj,
        uint8_t *rxd, uint8_t nBytes)
{
    uint8_t n = 0;
    uint8_t i = 0;
    uint16_t a;
    uint32_t fifoReg[3];
    REG_CiFIFOCON ciFifoCon;
    __attribute__((unused)) REG_CiFIFOSTA ciFifoSta;
    REG_CiFIFOUA ciFifoUa;
    int8_t spiTransferError = 0;

    // Get FIFO registers
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = _APPLICATION_CAN_ReadWordArray(index, a, fifoReg, 3);
    if (spiTransferError) {
        return -1;
    }

    // Check that it is a receive buffer
    ciFifoCon.word = fifoReg[0];
    if (ciFifoCon.txBF.TxEnable) {
        return -2;
    }

    // Get Status
    ciFifoSta.word = fifoReg[1];

    // Get address
    ciFifoUa.word = fifoReg[2];
#ifdef USERADDRESS_TIMES_FOUR
    a = 4 * ciFifoUa.bF.UserAddress;
#else
    a = ciFifoUa.bF.UserAddress;
#endif
    a += cRAMADDR_START;

    // Number of bytes to read
    n = nBytes + 8; // Add 8 header bytes

    if (ciFifoCon.rxBF.RxTimeStampEnable) {
        n += 4; // Add 4 time stamp bytes
    }

    // Make sure we read a multiple of 4 bytes from RAM
    if (n % 4) {
        n = n + 4 - (n % 4);
    }

    // Read rxObj using one access
    uint8_t ba[MAX_MSG_SIZE];

    if (n > MAX_MSG_SIZE) {
        n = MAX_MSG_SIZE;
    }

    spiTransferError = _APPLICATION_CAN_ReadByteArray(index, a, ba, n);
    if (spiTransferError) {
        return -3;
    }

    // Assign message header
    REG_t myReg;

    myReg.byte[0] = ba[0];
    myReg.byte[1] = ba[1];
    myReg.byte[2] = ba[2];
    myReg.byte[3] = ba[3];
    rxObj->word[0] = myReg.word;

    myReg.byte[0] = ba[4];
    myReg.byte[1] = ba[5];
    myReg.byte[2] = ba[6];
    myReg.byte[3] = ba[7];
    rxObj->word[1] = myReg.word;

    if (ciFifoCon.rxBF.RxTimeStampEnable) {
        myReg.byte[0] = ba[8];
        myReg.byte[1] = ba[9];
        myReg.byte[2] = ba[10];
        myReg.byte[3] = ba[11];
        rxObj->word[2] = myReg.word;

        // Assign message data
        for (i = 0; i < nBytes; i++) {
            rxd[i] = ba[i + 12];
        }
    } else {
        rxObj->word[2] = 0;

        // Assign message data
        for (i = 0; i < nBytes; i++) {
            rxd[i] = ba[i + 8];
        }
    }

    // UINC channel
    spiTransferError = _APPLICATION_CAN_ReceiveChannelUpdate(index, channel);
    if (spiTransferError) {
        return -4;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReceiveChannelReset(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel)
{
    uint16_t a = 0;
    REG_CiFIFOCON ciFifoCon;
    int8_t spiTransferError = 0;

    // Address and data
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
    ciFifoCon.word = 0;
    ciFifoCon.rxBF.FRESET = 1;

    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciFifoCon.byte[1]);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReceiveChannelUpdate(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel)
{
    uint16_t a = 0;
    REG_CiFIFOCON ciFifoCon;
    int8_t spiTransferError = 0;
    ciFifoCon.word = 0;

    // Set UINC
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
    ciFifoCon.rxBF.UINC = 1;

    // Write byte
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciFifoCon.byte[1]);

    return spiTransferError;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: Transmit Event FIFO">

int8_t _APPLICATION_CAN_TefStatusGet(CANFDSPI_MODULE_ID index,
        CAN_TEF_FIFO_STATUS* status)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    REG_CiTEFSTA ciTefSta;
    ciTefSta.word = 0;
    a = cREGADDR_CiTEFSTA;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciTefSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *status = (CAN_TEF_FIFO_STATUS) (ciTefSta.byte[0] & CAN_TEF_FIFO_STATUS_MASK);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TefMessageGet(CANFDSPI_MODULE_ID index,
        CAN_TEF_MSGOBJ* tefObj)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;
    uint32_t fifoReg[3];
    uint8_t n = 0;

    // Get FIFO registers
    a = cREGADDR_CiTEFCON;

    spiTransferError = _APPLICATION_CAN_ReadWordArray(index, a, fifoReg, 3);
    if (spiTransferError) {
        return -1;
    }

    // Get control
    REG_CiTEFCON ciTefCon;
    ciTefCon.word = fifoReg[0];

    // Get status
    __attribute__((unused)) REG_CiTEFSTA ciTefSta;
    ciTefSta.word = fifoReg[1];

    // Get address
    REG_CiFIFOUA ciTefUa;
    ciTefUa.word = fifoReg[2];
#ifdef USERADDRESS_TIMES_FOUR
    a = 4 * ciTefUa.bF.UserAddress;
#else
    a = ciTefUa.bF.UserAddress;
#endif
    a += cRAMADDR_START;

    // Number of bytes to read
    n = 8; // 8 header bytes

    if (ciTefCon.bF.TimeStampEnable) {
        n += 4; // Add 4 time stamp bytes
    }

    // Read rxObj using one access
    uint8_t ba[12];

    spiTransferError = _APPLICATION_CAN_ReadByteArray(index, a, ba, n);
    if (spiTransferError) {
        return -2;
    }

    // Assign message header
    REG_t myReg;

    myReg.byte[0] = ba[0];
    myReg.byte[1] = ba[1];
    myReg.byte[2] = ba[2];
    myReg.byte[3] = ba[3];
    tefObj->word[0] = myReg.word;

    myReg.byte[0] = ba[4];
    myReg.byte[1] = ba[5];
    myReg.byte[2] = ba[6];
    myReg.byte[3] = ba[7];
    tefObj->word[1] = myReg.word;

    if (ciTefCon.bF.TimeStampEnable) {
        myReg.byte[0] = ba[8];
        myReg.byte[1] = ba[9];
        myReg.byte[2] = ba[10];
        myReg.byte[3] = ba[11];
        tefObj->word[2] = myReg.word;
    } else {
        tefObj->word[2] = 0;
    }

    // Set UINC
    spiTransferError = _APPLICATION_CAN_TefUpdate(index);
    if (spiTransferError) {
        return -3;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TefReset(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Set FRESET
    a = cREGADDR_CiTEFCON + 1;
    REG_CiTEFCON ciTefCon;
    ciTefCon.word = 0;
    ciTefCon.bF.FRESET = 1;

    // Write byte
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciTefCon.byte[1]);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TefUpdate(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Set UINC
    a = cREGADDR_CiTEFCON + 1;
    REG_CiTEFCON ciTefCon;
    ciTefCon.word = 0;
    ciTefCon.bF.UINC = 1;

    // Write byte
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciTefCon.byte[1]);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TefConfigure(CANFDSPI_MODULE_ID index, CAN_TEF_CONFIG* config)
{
    int8_t spiTransferError = 0;

    // Setup FIFO
    REG_CiTEFCON ciTefCon;
    ciTefCon.word = canFifoResetValues[0];

    ciTefCon.bF.FifoSize = config->FifoSize;
    ciTefCon.bF.TimeStampEnable = config->TimeStampEnable;

    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiTEFCON, ciTefCon.word);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TefConfigureObjectReset(CAN_TEF_CONFIG* config)
{
    REG_CiTEFCON ciTefCon;
    ciTefCon.word = canFifoResetValues[0];

    config->FifoSize = ciTefCon.bF.FifoSize;
    config->TimeStampEnable = ciTefCon.bF.TimeStampEnable;

    return 0;
}


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: Module Events">

int8_t _APPLICATION_CAN_ModuleEventGet(CANFDSPI_MODULE_ID index,
        CAN_MODULE_EVENT* flags)
{
    int8_t spiTransferError = 0;

    // Read Interrupt flags
    REG_CiINTFLAG intFlags;
    intFlags.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadHalfWord(index, cREGADDR_CiINTFLAG, &intFlags.word);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *flags = (CAN_MODULE_EVENT) (intFlags.word & CAN_ALL_EVENTS);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ModuleEventEnable(CANFDSPI_MODULE_ID index,
        CAN_MODULE_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt Enables
    a = cREGADDR_CiINTENABLE;
    REG_CiINTENABLE intEnables;
    intEnables.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadHalfWord(index, a, &intEnables.word);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    intEnables.word |= (flags & CAN_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteHalfWord(index, a, intEnables.word);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ModuleEventDisable(CANFDSPI_MODULE_ID index,
        CAN_MODULE_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt Enables
    a = cREGADDR_CiINTENABLE;
    REG_CiINTENABLE intEnables;
    intEnables.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadHalfWord(index, a, &intEnables.word);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    intEnables.word &= ~(flags & CAN_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteHalfWord(index, a, intEnables.word);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ModuleEventClear(CANFDSPI_MODULE_ID index,
        CAN_MODULE_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt flags
    a = cREGADDR_CiINTFLAG;
    REG_CiINTFLAG intFlags;
    intFlags.word = 0;

    // Write 1 to all flags except the ones that we want to clear
    // Writing a 1 will not set the flag
    // Only writing a 0 will clear it
    // The flags are HS/C
    intFlags.word = CAN_ALL_EVENTS;
    intFlags.word &= ~flags;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteHalfWord(index, a, intFlags.word);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ModuleEventRxCodeGet(CANFDSPI_MODULE_ID index,
        CAN_RXCODE* rxCode)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;
    uint8_t rxCodeByte = 0;

    // Read
    a = cREGADDR_CiVEC + 3;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &rxCodeByte);
    if (spiTransferError) {
        return -1;
    }

    // Decode data
    // 0x40 = "no interrupt" (CAN_FIFO_CIVEC_NOINTERRUPT)
    if ((rxCodeByte < CAN_RXCODE_TOTAL_CHANNELS) || (rxCodeByte == CAN_RXCODE_NO_INT)) {
        *rxCode = (CAN_RXCODE) rxCodeByte;
    } else {
        *rxCode = CAN_RXCODE_RESERVED; // shouldn't get here
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ModuleEventTxCodeGet(CANFDSPI_MODULE_ID index,
        CAN_TXCODE* txCode)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;
    uint8_t txCodeByte = 0;

    // Read
    a = cREGADDR_CiVEC + 2;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &txCodeByte);
    if (spiTransferError) {
        return -1;
    }

    // Decode data
    // 0x40 = "no interrupt" (CAN_FIFO_CIVEC_NOINTERRUPT)
    if ((txCodeByte < CAN_TXCODE_TOTAL_CHANNELS) || (txCodeByte == CAN_TXCODE_NO_INT)) {
        *txCode = (CAN_TXCODE) txCodeByte;
    } else {
        *txCode = CAN_TXCODE_RESERVED; // shouldn't get here
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ModuleEventFilterHitGet(CANFDSPI_MODULE_ID index,
        CAN_FILTER* filterHit)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;
    uint8_t filterHitByte = 0;

    // Read
    a = cREGADDR_CiVEC + 1;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &filterHitByte);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *filterHit = (CAN_FILTER) filterHitByte;

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ModuleEventIcodeGet(CANFDSPI_MODULE_ID index,
        CAN_ICODE* icode)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;
    uint8_t icodeByte = 0;

    // Read
    a = cREGADDR_CiVEC;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &icodeByte);
    if (spiTransferError) {
        return -1;
    }

    // Decode
    if ((icodeByte < CAN_ICODE_RESERVED) && ((icodeByte < CAN_ICODE_TOTAL_CHANNELS) || (icodeByte >= CAN_ICODE_NO_INT))) {
        *icode = (CAN_ICODE) icodeByte;
    } else {
        *icode = CAN_ICODE_RESERVED; // shouldn't get here
    }

    return spiTransferError;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: Transmit FIFO Events">

int8_t _APPLICATION_CAN_TransmitChannelEventGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_EVENT* flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt flags
    REG_CiFIFOSTA ciFifoSta;
    ciFifoSta.word = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *flags = (CAN_TX_FIFO_EVENT) (ciFifoSta.byte[0] & CAN_TX_FIFO_ALL_EVENTS);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitEventGet(CANFDSPI_MODULE_ID index, uint32_t* txif)
{
    int8_t spiTransferError = 0;

    spiTransferError = _APPLICATION_CAN_ReadWord(index, cREGADDR_CiTXIF, txif);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitEventAttemptGet(CANFDSPI_MODULE_ID index,
        uint32_t* txatif)
{
    int8_t spiTransferError = 0;

    spiTransferError = _APPLICATION_CAN_ReadWord(index, cREGADDR_CiTXATIF, txatif);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitChannelIndexGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, uint8_t* idx)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read index
    REG_CiFIFOSTA ciFifoSta;
    ciFifoSta.word = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

    spiTransferError = _APPLICATION_CAN_ReadWord(index, a, &ciFifoSta.word);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *idx = ciFifoSta.txBF.FifoIndex;

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitChannelEventEnable(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt Enables
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoCon.byte[0] |= (flags & CAN_TX_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitChannelEventDisable(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt Enables
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoCon.byte[0] &= ~(flags & CAN_TX_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TransmitChannelEventAttemptClear(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt Enables
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);
    REG_CiFIFOSTA ciFifoSta;
    ciFifoSta.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoSta.byte[0] &= ~CAN_TX_FIFO_ATTEMPTS_EXHAUSTED_EVENT;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: Receive FIFO Events">

int8_t _APPLICATION_CAN_ReceiveChannelEventGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_EVENT* flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    if (channel == CAN_TXQUEUE_CH0) return -100;

    // Read Interrupt flags
    REG_CiFIFOSTA ciFifoSta;
    ciFifoSta.word = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *flags = (CAN_RX_FIFO_EVENT) (ciFifoSta.byte[0] & CAN_RX_FIFO_ALL_EVENTS);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReceiveEventGet(CANFDSPI_MODULE_ID index, uint32_t* rxif)
{
    int8_t spiTransferError = 0;

    spiTransferError = _APPLICATION_CAN_ReadWord(index, cREGADDR_CiRXIF, rxif);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReceiveEventOverflowGet(CANFDSPI_MODULE_ID index,
        uint32_t* rxovif)
{
    int8_t spiTransferError = 0;

    spiTransferError = _APPLICATION_CAN_ReadWord(index, cREGADDR_CiRXOVIF, rxovif);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReceiveChannelIndexGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, uint8_t* idx)
{
    return _APPLICATION_CAN_TransmitChannelIndexGet(index, channel, idx);
}

int8_t _APPLICATION_CAN_ReceiveChannelEventEnable(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    if (channel == CAN_TXQUEUE_CH0) return -100;

    // Read Interrupt Enables
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoCon.byte[0] |= (flags & CAN_RX_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReceiveChannelEventDisable(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    if (channel == CAN_TXQUEUE_CH0) return -100;

    // Read Interrupt Enables
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoCon.byte[0] &= ~(flags & CAN_RX_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ReceiveChannelEventOverflowClear(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    if (channel == CAN_TXQUEUE_CH0) return -100;

    // Read Interrupt Flags
    REG_CiFIFOSTA ciFifoSta;
    ciFifoSta.word = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoSta.byte[0] &= ~(CAN_RX_FIFO_OVERFLOW_EVENT);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Section: Transmit Event FIFO Events">

int8_t _APPLICATION_CAN_TefEventGet(CANFDSPI_MODULE_ID index,
        CAN_TEF_FIFO_EVENT* flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt flags
    REG_CiTEFSTA ciTefSta;
    ciTefSta.word = 0;
    a = cREGADDR_CiTEFSTA;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciTefSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *flags = (CAN_TEF_FIFO_EVENT) (ciTefSta.byte[0] & CAN_TEF_FIFO_ALL_EVENTS);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TefEventEnable(CANFDSPI_MODULE_ID index,
        CAN_TEF_FIFO_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt Enables
    a = cREGADDR_CiTEFCON;
    REG_CiTEFCON ciTefCon;
    ciTefCon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciTefCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciTefCon.byte[0] |= (flags & CAN_TEF_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciTefCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TefEventDisable(CANFDSPI_MODULE_ID index,
        CAN_TEF_FIFO_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt Enables
    a = cREGADDR_CiTEFCON;
    REG_CiTEFCON ciTefCon;
    ciTefCon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciTefCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciTefCon.byte[0] &= ~(flags & CAN_TEF_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciTefCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TefEventOverflowClear(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt Flags
    REG_CiTEFSTA ciTefSta;
    ciTefSta.word = 0;
    a = cREGADDR_CiTEFSTA;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &ciTefSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciTefSta.byte[0] &= ~(CAN_TEF_FIFO_OVERFLOW_EVENT);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, ciTefSta.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: Error Handling">

int8_t _APPLICATION_CAN_ErrorCountTransmitGet(CANFDSPI_MODULE_ID index,
        uint8_t* tec)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Error count
    a = cREGADDR_CiTREC + 1;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, tec);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ErrorCountReceiveGet(CANFDSPI_MODULE_ID index,
        uint8_t* rec)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Error count
    a = cREGADDR_CiTREC;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, rec);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ErrorStateGet(CANFDSPI_MODULE_ID index,
        CAN_ERROR_STATE* flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Error state
    a = cREGADDR_CiTREC + 2;
    uint8_t f = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &f);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *flags = (CAN_ERROR_STATE) (f & CAN_ERROR_ALL);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_ErrorCountStateGet(CANFDSPI_MODULE_ID index,
        uint8_t* tec, uint8_t* rec, CAN_ERROR_STATE* flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Error
    a = cREGADDR_CiTREC;
    REG_CiTREC ciTrec;
    ciTrec.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadWord(index, a, &ciTrec.word);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *tec = ciTrec.byte[1];
    *rec = ciTrec.byte[0];
    *flags = (CAN_ERROR_STATE) (ciTrec.byte[2] & CAN_ERROR_ALL);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_BusDiagnosticsGet(CANFDSPI_MODULE_ID index,
        CAN_BUS_DIAGNOSTIC* bd)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read diagnostic registers all in one shot
    a = cREGADDR_CiBDIAG0;
    uint32_t w[2];

    spiTransferError = _APPLICATION_CAN_ReadWordArray(index, a, w, 2);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    CAN_BUS_DIAGNOSTIC b;
    b.word[0] = w[0];
    b.word[1] = w[1] & 0x0000ffff;
    b.word[2] = (w[1] >> 16) & 0x0000ffff;
    *bd = b;

    return spiTransferError;
}

int8_t _APPLICATION_CAN_BusDiagnosticsClear(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint8_t a = 0;

    // Clear diagnostic registers all in one shot
    a = cREGADDR_CiBDIAG0;
    uint32_t w[2];
    w[0] = 0;
    w[1] = 0;

    spiTransferError = _APPLICATION_CAN_WriteWordArray(index, a, w, 2);

    return spiTransferError;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: ECC">

int8_t _APPLICATION_CAN_EccEnable(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint8_t d = 0;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_ECCCON, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d |= 0x01;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_ECCCON, d);
    if (spiTransferError) {
        return -2;
    }

    return 0;
}

int8_t _APPLICATION_CAN_EccDisable(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint8_t d = 0;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_ECCCON, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d &= ~0x01;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_ECCCON, d);
    if (spiTransferError) {
        return -2;
    }

    return 0;
}

int8_t _APPLICATION_CAN_EccEventGet(CANFDSPI_MODULE_ID index,
        CAN_ECC_EVENT* flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Interrupt flags
    uint8_t eccStatus = 0;
    a = cREGADDR_ECCSTA;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &eccStatus);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *flags = (CAN_ECC_EVENT) (eccStatus & CAN_ECC_ALL_EVENTS);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_EccParitySet(CANFDSPI_MODULE_ID index,
        uint8_t parity)
{
    int8_t spiTransferError = 0;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_ECCCON + 1, parity);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_EccParityGet(CANFDSPI_MODULE_ID index,
        uint8_t* parity)
{
    int8_t spiTransferError = 0;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_ECCCON + 1, parity);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_EccErrorAddressGet(CANFDSPI_MODULE_ID index,
        uint16_t* a)
{
    int8_t spiTransferError = 0;
    REG_ECCSTA reg;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadWord(index, cREGADDR_ECCSTA, &reg.word);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *a = reg.bF.ErrorAddress;

    return spiTransferError;
}

int8_t _APPLICATION_CAN_EccEventEnable(CANFDSPI_MODULE_ID index,
        CAN_ECC_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_ECCCON;
    uint8_t eccInterrupts = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &eccInterrupts);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    eccInterrupts |= (flags & CAN_ECC_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, eccInterrupts);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_EccEventDisable(CANFDSPI_MODULE_ID index,
        CAN_ECC_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_ECCCON;
    uint8_t eccInterrupts = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &eccInterrupts);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    eccInterrupts &= ~(flags & CAN_ECC_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, eccInterrupts);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_EccEventClear(CANFDSPI_MODULE_ID index,
        CAN_ECC_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_ECCSTA;
    uint8_t eccStat = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &eccStat);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    eccStat &= ~(flags & CAN_ECC_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, eccStat);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: CRC">

int8_t _APPLICATION_CAN_CrcEventEnable(CANFDSPI_MODULE_ID index,
        CAN_CRC_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read interrupt control bits of CRC Register
    a = cREGADDR_CRC + 3;
    uint8_t crc;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &crc);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    crc |= (flags & CAN_CRC_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, crc);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_CrcEventDisable(CANFDSPI_MODULE_ID index,
        CAN_CRC_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read interrupt control bits of CRC Register
    a = cREGADDR_CRC + 3;
    uint8_t crc;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &crc);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    crc &= ~(flags & CAN_CRC_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, crc);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_CrcEventClear(CANFDSPI_MODULE_ID index,
        CAN_CRC_EVENT flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read interrupt flags of CRC Register
    a = cREGADDR_CRC + 2;
    uint8_t crc;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &crc);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    crc &= ~(flags & CAN_CRC_ALL_EVENTS);

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, crc);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_CrcEventGet(CANFDSPI_MODULE_ID index, CAN_CRC_EVENT* flags)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read interrupt flags of CRC Register
    a = cREGADDR_CRC + 2;
    uint8_t crc;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &crc);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *flags = (CAN_CRC_EVENT) (crc & CAN_CRC_ALL_EVENTS);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_CrcValueGet(CANFDSPI_MODULE_ID index, uint16_t* crc)
{
    int8_t spiTransferError = 0;

    // Read CRC value from CRC Register
    spiTransferError = _APPLICATION_CAN_ReadHalfWord(index, cREGADDR_CRC, crc);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_RamInit(CANFDSPI_MODULE_ID index, uint8_t d)
{
    uint8_t txd[SPI_DEFAULT_BUFFER_LENGTH];
    uint32_t k;
    int8_t spiTransferError = 0;

    // Prepare data
    for (k = 0; k < SPI_DEFAULT_BUFFER_LENGTH; k++) {
        txd[k] = d;
    }

    uint16_t a = cRAMADDR_START;

    for (k = 0; k < (cRAM_SIZE / SPI_DEFAULT_BUFFER_LENGTH); k++) {
        spiTransferError = _APPLICATION_CAN_WriteByteArray(index, a, txd, SPI_DEFAULT_BUFFER_LENGTH);
        if (spiTransferError) {
            return -1;
        }
        a += SPI_DEFAULT_BUFFER_LENGTH;
    }

    return spiTransferError;
}


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: Time Stamp">

int8_t _APPLICATION_CAN_TimeStampEnable(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint8_t d = 0;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_CiTSCON + 2, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d |= 0x01;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_CiTSCON + 2, d);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TimeStampDisable(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint8_t d = 0;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_CiTSCON + 2, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d &= 0x06;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_CiTSCON + 2, d);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TimeStampGet(CANFDSPI_MODULE_ID index, uint32_t* ts)
{
    int8_t spiTransferError = 0;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadWord(index, cREGADDR_CiTBC, ts);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TimeStampSet(CANFDSPI_MODULE_ID index, uint32_t ts)
{
    int8_t spiTransferError = 0;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiTBC, ts);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TimeStampModeConfigure(CANFDSPI_MODULE_ID index,
        CAN_TS_MODE mode)
{
    int8_t spiTransferError = 0;
    uint8_t d = 0;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_CiTSCON + 2, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d &= 0x01;
    d |= mode << 1;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_CiTSCON + 2, d);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_TimeStampPrescalerSet(CANFDSPI_MODULE_ID index,
        uint16_t ps)
{
    int8_t spiTransferError = 0;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteHalfWord(index, cREGADDR_CiTSCON, ps);

    return spiTransferError;
}


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: Oscillator and Bit Time">

int8_t _APPLICATION_CAN_OscillatorEnable(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint8_t d = 0;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_OSC, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d &= ~0x4;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_OSC, d);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_OscillatorControlSet(CANFDSPI_MODULE_ID index,
        CAN_OSC_CTRL ctrl)
{
    int8_t spiTransferError = 0;

    REG_OSC osc;
    osc.word = 0;

    osc.bF.PllEnable = ctrl.PllEnable;
    osc.bF.OscDisable = ctrl.OscDisable;
    osc.bF.SCLKDIV = ctrl.SclkDivide;
    osc.bF.CLKODIV = ctrl.ClkOutDivide;
#ifndef MCP2517FD
    osc.bF.LowPowerModeEnable = ctrl.LowPowerModeEnable;
#endif

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, cREGADDR_OSC, osc.byte[0]);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_OscillatorControlObjectReset(CAN_OSC_CTRL* ctrl)
{
    REG_OSC osc;
    osc.word = mcp25xxfdControlResetValues[0];

    ctrl->PllEnable = osc.bF.PllEnable;
    ctrl->OscDisable = osc.bF.OscDisable;
    ctrl->SclkDivide = osc.bF.SCLKDIV;
    ctrl->ClkOutDivide = osc.bF.CLKODIV;

    return 0;
}

int8_t _APPLICATION_CAN_OscillatorStatusGet(CANFDSPI_MODULE_ID index,
        CAN_OSC_STATUS* status)
{
    int8_t spiTransferError = 0;

    REG_OSC osc;
    osc.word = 0;
    CAN_OSC_STATUS stat;

    // Read
    spiTransferError = _APPLICATION_CAN_ReadByte(index, cREGADDR_OSC + 1, &osc.byte[1]);
    if (spiTransferError) {
        return -1;
    }

    stat.PllReady = osc.bF.PllReady;
    stat.OscReady = osc.bF.OscReady;
    stat.SclkReady = osc.bF.SclkReady;

    *status = stat;

    return spiTransferError;
}

int8_t _APPLICATION_CAN_BitTimeConfigure(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode,
        CAN_SYSCLK_SPEED clk)
{
    int8_t spiTransferError = 0;

    // Decode clk
    switch (clk) {
        case CAN_SYSCLK_40M:
            spiTransferError = _APPLICATION_CAN_BitTimeConfigureNominal40MHz(index, bitTime);
            if (spiTransferError) return spiTransferError;

            spiTransferError = _APPLICATION_CAN_BitTimeConfigureData40MHz(index, bitTime, sspMode);
            break;
        case CAN_SYSCLK_20M:
            spiTransferError = _APPLICATION_CAN_BitTimeConfigureNominal20MHz(index, bitTime);
            if (spiTransferError) return spiTransferError;

            spiTransferError = _APPLICATION_CAN_BitTimeConfigureData20MHz(index, bitTime, sspMode);
            break;
        case CAN_SYSCLK_10M:
            spiTransferError = _APPLICATION_CAN_BitTimeConfigureNominal10MHz(index, bitTime);
            if (spiTransferError) return spiTransferError;

            spiTransferError = _APPLICATION_CAN_BitTimeConfigureData10MHz(index, bitTime, sspMode);
            break;
        default:
            spiTransferError = -1;
            break;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_BitTimeConfigureNominal40MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime)
{
    int8_t spiTransferError = 0;
    REG_CiNBTCFG ciNbtcfg;

    ciNbtcfg.word = canControlResetValues[cREGADDR_CiNBTCFG / 4];

    // Arbitration Bit rate
    switch (bitTime) {
            // All 500K
        case CAN_500K_1M:
        case CAN_500K_2M:
        case CAN_500K_3M:
        case CAN_500K_4M:
        case CAN_500K_5M:
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 62;
            ciNbtcfg.bF.TSEG2 = 15;
            ciNbtcfg.bF.SJW = 15;
            break;

            // All 250K
        case CAN_250K_500K:
        case CAN_250K_833K:
        case CAN_250K_1M:
        case CAN_250K_1M5:
        case CAN_250K_2M:
        case CAN_250K_3M:
        case CAN_250K_4M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 126;
            ciNbtcfg.bF.TSEG2 = 31;
            ciNbtcfg.bF.SJW = 31;
            break;

        case CAN_1000K_4M:
        case CAN_1000K_8M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 30;
            ciNbtcfg.bF.TSEG2 = 7;
            ciNbtcfg.bF.SJW = 7;
            break;

        case CAN_125K_500K:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 254;
            ciNbtcfg.bF.TSEG2 = 63;
            ciNbtcfg.bF.SJW = 63;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiNBTCFG, ciNbtcfg.word);

    return spiTransferError;
}

int8_t _APPLICATION_CAN_BitTimeConfigureData40MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode)
{
    int8_t spiTransferError = 0;
    REG_CiDBTCFG ciDbtcfg;
    REG_CiTDC ciTdc;
    //    sspMode;

    ciDbtcfg.word = canControlResetValues[cREGADDR_CiDBTCFG / 4];
    ciTdc.word = 0;

    // Configure Bit time and sample point
    ciTdc.bF.TDCMode = CAN_SSP_MODE_AUTO;
    uint32_t tdcValue = 0;

    // Data Bit rate and SSP
    switch (bitTime) {
        case CAN_500K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 30;
            ciDbtcfg.bF.TSEG2 = 7;
            ciDbtcfg.bF.SJW = 7;
            // SSP
            ciTdc.bF.TDCOffset = 31;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_2M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 14;
            ciDbtcfg.bF.TSEG2 = 3;
            ciDbtcfg.bF.SJW = 3;
            // SSP
            ciTdc.bF.TDCOffset = 15;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_3M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 8;
            ciDbtcfg.bF.TSEG2 = 2;
            ciDbtcfg.bF.SJW = 2;
            // SSP
            ciTdc.bF.TDCOffset = 9;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_4M:
        case CAN_1000K_4M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_5M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 4;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 5;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_6M7:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 3;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 4;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_8M:
        case CAN_1000K_8M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 2;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 3;
            ciTdc.bF.TDCValue = 1;
            break;
        case CAN_500K_10M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 1;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 2;
            ciTdc.bF.TDCValue = 0;
            break;

        case CAN_250K_500K:
        case CAN_125K_500K:
            ciDbtcfg.bF.BRP = 1;
            ciDbtcfg.bF.TSEG1 = 30;
            ciDbtcfg.bF.TSEG2 = 7;
            ciDbtcfg.bF.SJW = 7;
            // SSP
            ciTdc.bF.TDCOffset = 31;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_833K:
            ciDbtcfg.bF.BRP = 1;
            ciDbtcfg.bF.TSEG1 = 17;
            ciDbtcfg.bF.TSEG2 = 4;
            ciDbtcfg.bF.SJW = 4;
            // SSP
            ciTdc.bF.TDCOffset = 18;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 30;
            ciDbtcfg.bF.TSEG2 = 7;
            ciDbtcfg.bF.SJW = 7;
            // SSP
            ciTdc.bF.TDCOffset = 31;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_1M5:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 18;
            ciDbtcfg.bF.TSEG2 = 5;
            ciDbtcfg.bF.SJW = 5;
            // SSP
            ciTdc.bF.TDCOffset = 19;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_2M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 14;
            ciDbtcfg.bF.TSEG2 = 3;
            ciDbtcfg.bF.SJW = 3;
            // SSP
            ciTdc.bF.TDCOffset = 15;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_3M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 8;
            ciDbtcfg.bF.TSEG2 = 2;
            ciDbtcfg.bF.SJW = 2;
            // SSP
            ciTdc.bF.TDCOffset = 9;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_4M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiDBTCFG, ciDbtcfg.word);
    if (spiTransferError) {
        return -2;
    }

    // Write Transmitter Delay Compensation
#ifdef REV_A
    ciTdc.bF.TDCOffset = 0;
    ciTdc.bF.TDCValue = 0;
#endif

    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiTDC, ciTdc.word);
    if (spiTransferError) {
        return -3;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_BitTimeConfigureNominal20MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime)
{
    int8_t spiTransferError = 0;
    REG_CiNBTCFG ciNbtcfg;

    ciNbtcfg.word = canControlResetValues[cREGADDR_CiNBTCFG / 4];

    // Arbitration Bit rate
    switch (bitTime) {
            // All 500K
        case CAN_500K_1M:
        case CAN_500K_2M:
        case CAN_500K_4M:
        case CAN_500K_5M:
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 30;
            ciNbtcfg.bF.TSEG2 = 7;
            ciNbtcfg.bF.SJW = 7;
            break;

            // All 250K
        case CAN_250K_500K:
        case CAN_250K_833K:
        case CAN_250K_1M:
        case CAN_250K_1M5:
        case CAN_250K_2M:
        case CAN_250K_3M:
        case CAN_250K_4M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 62;
            ciNbtcfg.bF.TSEG2 = 15;
            ciNbtcfg.bF.SJW = 15;
            break;

        case CAN_1000K_4M:
        case CAN_1000K_8M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 14;
            ciNbtcfg.bF.TSEG2 = 3;
            ciNbtcfg.bF.SJW = 3;
            break;

        case CAN_125K_500K:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 126;
            ciNbtcfg.bF.TSEG2 = 31;
            ciNbtcfg.bF.SJW = 31;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiNBTCFG, ciNbtcfg.word);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_BitTimeConfigureData20MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode)
{
    int8_t spiTransferError = 0;
    REG_CiDBTCFG ciDbtcfg;
    REG_CiTDC ciTdc;
    //    sspMode;

    ciDbtcfg.word = canControlResetValues[cREGADDR_CiDBTCFG / 4];
    ciTdc.word = 0;

    // Configure Bit time and sample point
    ciTdc.bF.TDCMode = CAN_SSP_MODE_AUTO;
    uint32_t tdcValue = 0;

    // Data Bit rate and SSP
    switch (bitTime) {
        case CAN_500K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 14;
            ciDbtcfg.bF.TSEG2 = 3;
            ciDbtcfg.bF.SJW = 3;
            // SSP
            ciTdc.bF.TDCOffset = 15;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_2M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_4M:
        case CAN_1000K_4M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 2;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 3;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_5M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 1;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 2;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
        case CAN_1000K_8M:
            //qDebug("Data Bitrate not feasible with this clock!");
            return -1;
            break;

        case CAN_250K_500K:
        case CAN_125K_500K:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 30;
            ciDbtcfg.bF.TSEG2 = 7;
            ciDbtcfg.bF.SJW = 7;
            // SSP
            ciTdc.bF.TDCOffset = 31;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_833K:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 17;
            ciDbtcfg.bF.TSEG2 = 4;
            ciDbtcfg.bF.SJW = 4;
            // SSP
            ciTdc.bF.TDCOffset = 18;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 14;
            ciDbtcfg.bF.TSEG2 = 3;
            ciDbtcfg.bF.SJW = 3;
            // SSP
            ciTdc.bF.TDCOffset = 15;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_1M5:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 8;
            ciDbtcfg.bF.TSEG2 = 2;
            ciDbtcfg.bF.SJW = 2;
            // SSP
            ciTdc.bF.TDCOffset = 9;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_2M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_3M:
            //qDebug("Data Bitrate not feasible with this clock!");
            return -1;
            break;
        case CAN_250K_4M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 2;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 3;
            ciTdc.bF.TDCValue = tdcValue;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiDBTCFG, ciDbtcfg.word);
    if (spiTransferError) {
        return -2;
    }

    // Write Transmitter Delay Compensation
#ifdef REV_A
    ciTdc.bF.TDCOffset = 0;
    ciTdc.bF.TDCValue = 0;
#endif

    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiTDC, ciTdc.word);
    if (spiTransferError) {
        return -3;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_BitTimeConfigureNominal10MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime)
{
    int8_t spiTransferError = 0;
    REG_CiNBTCFG ciNbtcfg;

    ciNbtcfg.word = canControlResetValues[cREGADDR_CiNBTCFG / 4];

    // Arbitration Bit rate
    switch (bitTime) {
            // All 500K
        case CAN_500K_1M:
        case CAN_500K_2M:
        case CAN_500K_4M:
        case CAN_500K_5M:
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 14;
            ciNbtcfg.bF.TSEG2 = 3;
            ciNbtcfg.bF.SJW = 3;
            break;

            // All 250K
        case CAN_250K_500K:
        case CAN_250K_833K:
        case CAN_250K_1M:
        case CAN_250K_1M5:
        case CAN_250K_2M:
        case CAN_250K_3M:
        case CAN_250K_4M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 30;
            ciNbtcfg.bF.TSEG2 = 7;
            ciNbtcfg.bF.SJW = 7;
            break;

        case CAN_1000K_4M:
        case CAN_1000K_8M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 7;
            ciNbtcfg.bF.TSEG2 = 2;
            ciNbtcfg.bF.SJW = 2;
            break;

        case CAN_125K_500K:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 62;
            ciNbtcfg.bF.TSEG2 = 15;
            ciNbtcfg.bF.SJW = 15;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiNBTCFG, ciNbtcfg.word);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_BitTimeConfigureData10MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode)
{
    int8_t spiTransferError = 0;
    REG_CiDBTCFG ciDbtcfg;
    REG_CiTDC ciTdc;
    //    sspMode;

    ciDbtcfg.word = canControlResetValues[cREGADDR_CiDBTCFG / 4];
    ciTdc.word = 0;

    // Configure Bit time and sample point
    ciTdc.bF.TDCMode = CAN_SSP_MODE_AUTO;
    uint32_t tdcValue = 0;

    // Data Bit rate and SSP
    switch (bitTime) {
        case CAN_500K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_2M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 2;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 3;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_4M:
        case CAN_500K_5M:
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
        case CAN_1000K_4M:
        case CAN_1000K_8M:
            //qDebug("Data Bitrate not feasible with this clock!");
            return -1;
            break;

        case CAN_250K_500K:
        case CAN_125K_500K:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 14;
            ciDbtcfg.bF.TSEG2 = 3;
            ciDbtcfg.bF.SJW = 3;
            // SSP
            ciTdc.bF.TDCOffset = 15;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_833K:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 7;
            ciDbtcfg.bF.TSEG2 = 2;
            ciDbtcfg.bF.SJW = 2;
            // SSP
            ciTdc.bF.TDCOffset = 8;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_1M5:
            //qDebug("Data Bitrate not feasible with this clock!");
            return -1;
            break;
        case CAN_250K_2M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 2;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 3;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_3M:
        case CAN_250K_4M:
            //qDebug("Data Bitrate not feasible with this clock!");
            return -1;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiDBTCFG, ciDbtcfg.word);
    if (spiTransferError) {
        return -2;
    }

    // Write Transmitter Delay Compensation
#ifdef REV_A
    ciTdc.bF.TDCOffset = 0;
    ciTdc.bF.TDCValue = 0;
#endif

    spiTransferError = _APPLICATION_CAN_WriteWord(index, cREGADDR_CiTDC, ciTdc.word);
    if (spiTransferError) {
        return -3;
    }

    return spiTransferError;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Section: GPIO">

int8_t _APPLICATION_CAN_GpioModeConfigure(CANFDSPI_MODULE_ID index,
        GPIO_PIN_MODE gpio0, GPIO_PIN_MODE gpio1)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_IOCON + 3;
    REG_IOCON iocon;
    iocon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &iocon.byte[3]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    iocon.bF.PinMode0 = gpio0;
    iocon.bF.PinMode1 = gpio1;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, iocon.byte[3]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_GpioDirectionConfigure(CANFDSPI_MODULE_ID index,
        GPIO_PIN_DIRECTION gpio0, GPIO_PIN_DIRECTION gpio1)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_IOCON;
    REG_IOCON iocon;
    iocon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &iocon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    iocon.bF.TRIS0 = gpio0;
    iocon.bF.TRIS1 = gpio1;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, iocon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_GpioStandbyControlEnable(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_IOCON;
    REG_IOCON iocon;
    iocon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &iocon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    iocon.bF.XcrSTBYEnable = 1;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, iocon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_GpioStandbyControlDisable(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_IOCON;
    REG_IOCON iocon;
    iocon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &iocon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    iocon.bF.XcrSTBYEnable = 0;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, iocon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_GpioInterruptPinsOpenDrainConfigure(CANFDSPI_MODULE_ID index,
        GPIO_OPEN_DRAIN_MODE mode)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_IOCON + 3;
    REG_IOCON iocon;
    iocon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &iocon.byte[3]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    iocon.bF.INTPinOpenDrain = mode;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, iocon.byte[3]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_GpioTransmitPinOpenDrainConfigure(CANFDSPI_MODULE_ID index,
        GPIO_OPEN_DRAIN_MODE mode)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_IOCON + 3;
    REG_IOCON iocon;
    iocon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &iocon.byte[3]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    iocon.bF.TXCANOpenDrain = mode;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, iocon.byte[3]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_GpioPinSet(CANFDSPI_MODULE_ID index,
        GPIO_PIN_POS pos, GPIO_PIN_STATE latch)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_IOCON + 1;
    REG_IOCON iocon;
    iocon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &iocon.byte[1]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    switch (pos) {
        case GPIO_PIN_0:
            iocon.bF.LAT0 = latch;
            break;
        case GPIO_PIN_1:
            iocon.bF.LAT1 = latch;
            break;
        default:
            return -1;
            break;
    }

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, iocon.byte[1]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_GpioPinRead(CANFDSPI_MODULE_ID index,
        GPIO_PIN_POS pos, GPIO_PIN_STATE* state)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_IOCON + 2;
    REG_IOCON iocon;
    iocon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &iocon.byte[2]);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    switch (pos) {
        case GPIO_PIN_0:
            *state = (GPIO_PIN_STATE) iocon.bF.GPIO0;
            break;
        case GPIO_PIN_1:
            *state = (GPIO_PIN_STATE) iocon.bF.GPIO1;
            break;
        default:
            return -1;
            break;
    }

    return spiTransferError;
}

int8_t _APPLICATION_CAN_GpioClockOutputConfigure(CANFDSPI_MODULE_ID index,
        GPIO_CLKO_MODE mode)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read
    a = cREGADDR_IOCON + 3;
    REG_IOCON iocon;
    iocon.word = 0;

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &iocon.byte[3]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    iocon.bF.SOFOutputEnable = mode;

    // Write
    spiTransferError = _APPLICATION_CAN_WriteByte(index, a, iocon.byte[3]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Section: Miscellaneous">

uint32_t _APPLICATION_CAN_DlcToDataBytes(CAN_DLC dlc)
{
    uint32_t dataBytesInObject = 0;

    Nop();
    Nop();

    if (dlc < CAN_DLC_12) {
        dataBytesInObject = dlc;
    } else {
        switch (dlc) {
            case CAN_DLC_12:
                dataBytesInObject = 12;
                break;
            case CAN_DLC_16:
                dataBytesInObject = 16;
                break;
            case CAN_DLC_20:
                dataBytesInObject = 20;
                break;
            case CAN_DLC_24:
                dataBytesInObject = 24;
                break;
            case CAN_DLC_32:
                dataBytesInObject = 32;
                break;
            case CAN_DLC_48:
                dataBytesInObject = 48;
                break;
            case CAN_DLC_64:
                dataBytesInObject = 64;
                break;
            default:
                break;
        }
    }

    return dataBytesInObject;
}

int8_t _APPLICATION_CAN_FifoIndexGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, uint8_t* mi)
{
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Read Status register
    uint8_t b = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);
    a += 1; // byte[1]

    spiTransferError = _APPLICATION_CAN_ReadByte(index, a, &b);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *mi = b & 0x1f;

    return spiTransferError;
}

uint16_t _APPLICATION_CAN_CalculateCRC16(uint8_t* data, uint16_t size)
{
    uint16_t init = CRCBASE;
    uint8_t index;

    while (size-- != 0) {
        index = ((uint8_t*) & init)[CRCUPPER] ^ *data++;
        init = (init << 8) ^ crc16_table[index];
    }

    return init;
}

CAN_DLC _APPLICATION_CAN_DataBytesToDlc(uint8_t n)
{
    CAN_DLC dlc = CAN_DLC_0;

    if (n <= 4) {
        dlc = CAN_DLC_4;
    } else if (n <= 8) {
        dlc = CAN_DLC_8;
    } else if (n <= 12) {
        dlc = CAN_DLC_12;
    } else if (n <= 16) {
        dlc = CAN_DLC_16;
    } else if (n <= 20) {
        dlc = CAN_DLC_20;
    } else if (n <= 24) {
        dlc = CAN_DLC_24;
    } else if (n <= 32) {
        dlc = CAN_DLC_32;
    } else if (n <= 48) {
        dlc = CAN_DLC_48;
    } else if (n <= 64) {
        dlc = CAN_DLC_64;
    }

    return dlc;
}

// </editor-fold>



void _APPLICATION_CAN_ReceiveCallback(void)
{
    APP_Msg_T appCANMsg;
    appCANMsg.msgId = APP_MSG_CAN_RECV_CB;
    OSAL_QUEUE_SendISR(&appData.appQueue, &appCANMsg);
}


void _APPLICATION_CAN_Init()
{
    CAN_BITTIME_SETUP selectedBitTime = CAN_500K_2M;
    
    CAN_CONFIG config;
    CAN_TX_FIFO_CONFIG txConfig;
    CAN_RX_FIFO_CONFIG rxConfig;
    REG_CiFLTOBJ fObj;
    REG_CiMASK mObj;
    
    // Reset device
    _APPLICATION_CAN_Reset(DRV_CANFDSPI_INDEX_0);

    // Enable ECC and initialize RAM
    _APPLICATION_CAN_EccEnable(DRV_CANFDSPI_INDEX_0);

    if (!ramInitialized) {
        _APPLICATION_CAN_RamInit(DRV_CANFDSPI_INDEX_0, 0xFF);
        ramInitialized = true;
    }

    // Configure device
    _APPLICATION_CAN_ConfigureObjectReset(&config);
    config.IsoCrcEnable = 1;
    config.StoreInTEF = 0;

    _APPLICATION_CAN_Configure(DRV_CANFDSPI_INDEX_0, &config);
            
    // Setup TX FIFO
    _APPLICATION_CAN_TransmitChannelConfigureObjectReset(&txConfig);
    txConfig.FifoSize = 7;
    txConfig.PayLoadSize = CAN_PLSIZE_64;
    txConfig.TxPriority = 1;

    _APPLICATION_CAN_TransmitChannelConfigure(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, &txConfig);

    // Setup RX FIFO
    _APPLICATION_CAN_ReceiveChannelConfigureObjectReset(&rxConfig);
    rxConfig.FifoSize = 7;
    rxConfig.PayLoadSize = CAN_PLSIZE_64;

    _APPLICATION_CAN_ReceiveChannelConfigure(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, &rxConfig);

    /*
     * Setup RX Filter 0 (Receive Broadcast Messages)
     */
    {
    // Setup RX Filter
    fObj.word = 0;
    fObj.bF.SID = 0x45A;
    fObj.bF.EXIDE = 0;
    fObj.bF.EID = 0x00;

    _APPLICATION_CAN_FilterObjectConfigure(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, &fObj.bF);

    // Setup RX Mask
    mObj.word = 0;
    mObj.bF.MSID = 0x07FF;
    mObj.bF.MIDE = 1; // Only allow standard IDs
    mObj.bF.MEID = 0x0;
    _APPLICATION_CAN_FilterMaskConfigure(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, &mObj.bF);

    // Link FIFO and Filter
    _APPLICATION_CAN_FilterToFifoLink(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, APP_RX_FIFO, true);
    }
    
    /*
     * Setup RX Filter 1 (Receive Node Specific Messages)
     */
    {
        // Setup RX Filter
        fObj.word = 0;
        fObj.bF.SID = APPLICATION_CONFIG_GetCanAddress();
        fObj.bF.EXIDE = 0;
        fObj.bF.EID = 0x00;

        _APPLICATION_CAN_FilterObjectConfigure(DRV_CANFDSPI_INDEX_0, CAN_FILTER1, &fObj.bF);

        // Setup RX Mask
        mObj.word = 0;
        mObj.bF.MSID = 0x07FF;
        mObj.bF.MIDE = 1; // Only allow standard IDs
        mObj.bF.MEID = 0x0;
        _APPLICATION_CAN_FilterMaskConfigure(DRV_CANFDSPI_INDEX_0, CAN_FILTER1, &mObj.bF);

        // Link FIFO and Filter
        _APPLICATION_CAN_FilterToFifoLink(DRV_CANFDSPI_INDEX_0, CAN_FILTER1, APP_RX_FIFO, true);
    }
    // Setup Bit Time
    _APPLICATION_CAN_BitTimeConfigure(DRV_CANFDSPI_INDEX_0, selectedBitTime, CAN_SSP_MODE_AUTO, CAN_SYSCLK_40M);

    // Setup Transmit and Receive Interrupts
    _APPLICATION_CAN_GpioModeConfigure(DRV_CANFDSPI_INDEX_0, GPIO_MODE_INT, GPIO_MODE_INT);
    _APPLICATION_CAN_TransmitChannelEventEnable(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, CAN_TX_FIFO_NOT_FULL_EVENT);
    _APPLICATION_CAN_ReceiveChannelEventEnable(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, CAN_RX_FIFO_NOT_EMPTY_EVENT);
    _APPLICATION_CAN_ModuleEventEnable(DRV_CANFDSPI_INDEX_0, /*CAN_TX_EVENT |*/ CAN_RX_EVENT);

    // Select Normal Mode
    _APPLICATION_CAN_OperationModeSelect(DRV_CANFDSPI_INDEX_0, CAN_NORMAL_MODE);
    
    CAN_STDBY_Clear();
    EIC_CallbackRegister(EIC_PIN_3, (EIC_CALLBACK)_APPLICATION_CAN_ReceiveCallback, 0);
    EIC_InterruptEnable(EIC_PIN_3);
}

void _APPLICATION_CAN_TransmitMessageQueue(CAN_MSG_t *canMsg)
{

    uint8_t attempts = MAX_TXQUEUE_ATTEMPTS;
    CAN_TX_FIFO_EVENT txFlags;
    uint8_t tec;
    uint8_t rec;
    CAN_ERROR_STATE errorFlags;

    // Check if FIFO is not full
    do {
        _APPLICATION_CAN_TransmitChannelEventGet(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, &txFlags);
        if (attempts == 0)
        {
            _APPLICATION_CAN_ErrorCountStateGet(DRV_CANFDSPI_INDEX_0, &tec, &rec, &errorFlags);
            SYS_CONSOLE_PRINT("[CAN] Tx Failed: 0x%X\r\n",errorFlags);
            return;
        }
        attempts--;
    }
    while (!(txFlags & CAN_TX_FIFO_NOT_FULL_EVENT));

    // Load message and transmit
    uint8_t n = _APPLICATION_CAN_DlcToDataBytes(canMsg->msgObj.txObj.bF.ctrl.DLC);

    _APPLICATION_CAN_TransmitChannelLoad(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, &canMsg->msgObj.txObj, canMsg->can_data, n, true);
    // --M43814--TODO-- GREEN_LED_Clear();
#ifdef ENABLE_CONSOLE_PRINT
    SYS_CONSOLE_PRINT("New Message Received from BLE\r\nMessage ID: 0x%X, DLC: 0x%X\r\nMessage: ", canMsg->msgObj.txObj.bF.id.SID, canMsg->msgObj.txObj.bF.ctrl.DLC);
    for(uint8_t i = 0; i<n; i++)
    {
        SYS_CONSOLE_PRINT(" 0x%X",canMsg->can_data[i]);
    }
    SYS_CONSOLE_PRINT("\r\n[CAN] TX Done\r\n");
#endif
}

/* TODO:  Add any necessary local functions.
*/



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void APPLICATION_CAN_Initialize(void) {
    
}

void APPLICATION_CAN_Init(void) {
    DRV_SPI_TRANSFER_SETUP setup;
    canSPIHandle = DRV_SPI_Open(DRV_SPI_INDEX_0, DRV_IO_INTENT_READWRITE);
    setup.baudRateInHz = 10000000;
    setup.clockPhase = DRV_SPI_CLOCK_PHASE_VALID_LEADING_EDGE;
    setup.clockPolarity = DRV_SPI_CLOCK_POLARITY_IDLE_LOW;
    setup.dataBits = DRV_SPI_DATA_BITS_8;
    setup.chipSelect = GPIO_PIN_RB13;
    setup.csPolarity = DRV_SPI_CS_POLARITY_ACTIVE_LOW;
    DRV_SPI_TransferSetup ( canSPIHandle, &setup );    
    
    _APPLICATION_CAN_Init();
}

void APPLICATION_CAN_Tasks(APP_Msg_T *pMsg) {
    APP_MsgId_T id = pMsg->msgId;
    if ( id==APP_MSG_CAN_RECV_CB ) {
        CAN_MSG_t *canMsg = (CAN_MSG_t *)&pMsg->msgData;
        CAN_RX_FIFO_EVENT rxFlags;
        _APPLICATION_CAN_ReceiveChannelEventGet(
                DRV_CANFDSPI_INDEX_0, 
                APP_RX_FIFO, 
                &rxFlags);
        
        if (rxFlags & CAN_RX_FIFO_NOT_EMPTY_EVENT) {
            _APPLICATION_CAN_ReceiveMessageGet(
                    DRV_CANFDSPI_INDEX_0, 
                    APP_RX_FIFO, 
                    &canMsg->msgObj.rxObj, 
                    canMsg->can_data, 
                    MAX_DATA_BYTES);
            uint8_t len = _APPLICATION_CAN_DlcToDataBytes(canMsg->msgObj.rxObj.bF.ctrl.DLC);


            if ( (canMsg->msgObj.rxObj.bF.id.SID == 0x45A) ||
                 (canMsg->msgObj.rxObj.bF.id.SID == APPLICATION_CONFIG_GetCanAddress() )
                    ) {
                APPLICATION_USER_LED_On();

                APP_Msg_T msg;
                msg.msgId = APP_MSG_HCI_PACKET_RCVD;
                memset(msg.msgData, 0, sizeof(msg.msgData));
                memcpy(msg.msgData, canMsg->can_data, len);
                OSAL_QUEUE_Send(&appData.appQueue, &msg, 0);
            }
            
            _APPLICATION_CAN_ReceiveChannelEventGet(
                    DRV_CANFDSPI_INDEX_0, 
                    APP_RX_FIFO, 
                    &rxFlags);
            
            if (rxFlags & CAN_RX_FIFO_NOT_EMPTY_EVENT) {
                APP_Msg_T appCANMsg;
                appCANMsg.msgId = APP_MSG_CAN_RECV_CB;
                OSAL_QUEUE_Send(&appData.appQueue, &appCANMsg,0);
            }
        } else {
            // SYS_CONSOLE_PRINT("\r\nCAN_RX_FIFO_EVENT not handled %02X\r\n",rxFlags);
        }
    }
}

void APPLICATION_CAN_Write(uint16_t id, uint8_t *pData, const size_t size) {
    CAN_MSG_t canMsg = {
        .msgObj = {
            .txObj = {
                .bF = {
                    .id = {
                        /* SID[10:0]: Standard Identifier */
                        .SID = id,
                        /* EID[17:0]: Extended Identifier */
                        .EID = 0,
                        /* SID11: In FD mode the standard ID can be extended to 12 bit using r1 */
                        .SID11 = 0,
                        /* Unimplemented: Read as ?x? */
                        .unimplemented1 = 0,
                    },
                    .ctrl = {
                        /* DLC[3:0]: Data Length Code */
                        .DLC = _APPLICATION_CAN_DataBytesToDlc(size),
                        /* IDE: Identifier Extension Flag; distinguishes between base and extended format */
                        .IDE = 0,
                        /* RTR: Remote Transmission Request; not used in CAN FD */
                        .RTR = 0,
                        /* BRS: Bit Rate Switch; selects if data bit rate is switched */
                        .BRS = 0,
                        /* FDF: FD Frame; distinguishes between CAN and CAN FD formats */
                        .FDF = 1,
                        /* ESI: Error Status Indicator */
                        .ESI = 0,
                        /* SEQ[22:0]: Sequence to keep track of transmitted messages in Transmit Event FIFO */
                        .SEQ = 0,
                    },
                    .timeStamp = 0,
                },
            },
        },
        .can_data = {
            0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
            0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
            0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
            0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
        },
    };
    
    // canMsg.msgObj.txObj.bF.ctrl.DLC = _APPLICATION_CAN_DataBytesToDlc(size);
    memcpy(canMsg.can_data, pData, size);
    
    _APPLICATION_CAN_TransmitMessageQueue(&canMsg);
    
}

void APPLICATION_CAN_Debug(void) {
    uint32_t buf = 0;
    uint16_t address = cREGADDR_CiCON;
    for (int i=0; i<0x2F0;i=i+4) {
        _APPLICATION_CAN_ReadWord(DRV_CANFDSPI_INDEX_0, address+i, &buf);
    }
    for (int i=0x0E00;i<0x0E18;i=i+4) {
        _APPLICATION_CAN_ReadWord(DRV_CANFDSPI_INDEX_0, i, &buf);
    }
}

/* *****************************************************************************
 End of File
 */
