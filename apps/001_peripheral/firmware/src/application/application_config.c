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

#include "application_config.h"
#include "application_common.h"
#include "definitions.h"
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
static APPLICATION_Config_T _config;

volatile struct {
    union {
        struct {
            uint32_t    write_complete : 1,
                        rfu : 31;
        };
        uint32_t v;
    }bits;
} _data;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
PDS_DECLARE_FILE(PDS_APP_ITEM_ID_1, (uint16_t)sizeof(APPLICATION_Config_T),&_config,FILE_INTEGRITY_CONTROL_MARK);


static void _APPLICATION_CONFIG_PdsWriteComplete(PDS_MemId_t memoryId) {
    _data.bits.write_complete = true;
}

void _APPLICATION_CONFIG_SetCommand(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    if (argc == 3) {
        if ( (atoi(argv[1])) ==  1) {
            APPLICATION_COMMON_U16AsciiToHex((uint8_t *)argv[2],&_config.can_address);
            PDS_Store(PDS_APP_ITEM_ID_1);
            return;
        } else if ( (atoi(argv[1])) ==  2) {
            APPLICATION_COMMON_StringToHex((uint8_t *)argv[2], (uint8_t *)&_config.ble_address, 7);
            PDS_Store(PDS_APP_ITEM_ID_1);
            return;
        }
    }
    SYS_CONSOLE_PRINT(" ***  USAGE: S <id>,<addr> ***\r\n");
}

void _APPLICATION_CONFIG_GetCommand(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    if (argc == 2) {
        if ( (atoi(argv[1])) == 1) {
            SYS_CONSOLE_PRINT("%04X\r\n", _config.can_address);
            return;
        } else if ( (atoi(argv[1])) == 2 ) {
            SYS_CONSOLE_PRINT("%02X\r\n%02X %02X %02X %02X %02X %02X\r\n", 
            _config.ble_address.addrType,
            _config.ble_address.addr[0],
            _config.ble_address.addr[1],
            _config.ble_address.addr[2],
            _config.ble_address.addr[3],
            _config.ble_address.addr[4],
            _config.ble_address.addr[5]);
            return;
        }
    }
    SYS_CONSOLE_PRINT(" ***  USAGE: G <id> ***\r\n");
}

void _APPLICATION_CONFIG_ClearCommand(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    if (argc == 1) {
        _config.can_address = 0x0000;
        PDS_Store(PDS_APP_ITEM_ID_1);
        return;
    }
    SYS_CONSOLE_PRINT(" ***  USAGE: C ***\r\n");
}

void _APPLICATION_CONFIG_ResetCommand(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    APPLICATION_COMMON_Reset();
}

void _APPLICATION_CONFIG_PrintCommand(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    SYS_CMD_PRINT("can_address %04X\r\n", _config.can_address);
    SYS_CONSOLE_PRINT("ble_address %02X\r\n%02X %02X %02X %02X %02X %02X\r\n", 
        _config.ble_address.addrType,
        _config.ble_address.addr[0],
        _config.ble_address.addr[1],
        _config.ble_address.addr[2],
        _config.ble_address.addr[3],
        _config.ble_address.addr[4],
        _config.ble_address.addr[5]);
}

void _APPLICATION_CONFIG_DebugCommand(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    extern void APPLICATION_CAN_Debug(void);
    APPLICATION_CAN_Debug();
    
}

static const SYS_CMD_DESCRIPTOR _cmdTbl[]={
    {"S",_APPLICATION_CONFIG_SetCommand, ": Configuration SET Command"},
    {"G",_APPLICATION_CONFIG_GetCommand, ": Configuration GET Command"},
    {"C",_APPLICATION_CONFIG_ClearCommand, ": Configuration CLEAR Command"},
    {"R",_APPLICATION_CONFIG_ResetCommand, ": Reset Command"},
    {"P",_APPLICATION_CONFIG_PrintCommand, ": Print Command"},
    {"D",_APPLICATION_CONFIG_DebugCommand, ": Debug Command"},
};

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void APPLICATION_CONFIG_Initialize(void) {
    
    SYS_CMD_ADDGRP(_cmdTbl, sizeof(_cmdTbl)/sizeof(*_cmdTbl), "CFG", ": Configuration Commands");
    
    PDS_RegisterWriteCompleteCallback(_APPLICATION_CONFIG_PdsWriteComplete);
    if (PDS_IsAbleToRestore(PDS_APP_ITEM_ID_1) == false) {
        PDS_InitItems(PDS_APP_ITEM_ID_1,PDS_APP_ITEM_ID_1);
        PDS_Store(PDS_APP_ITEM_ID_1);
    } else {
        PDS_Restore(PDS_APP_ITEM_ID_1);
    }
    SYS_CMD_PRINT("\r\n>");
}

uint16_t APPLICATION_CONFIG_GetCanAddress(void) {
    return _config.can_address;
}

uint16_t APPLICATION_CONFIG_GetBleAddress(BLE_GAP_Addr_T *p_addr) {
    memcpy(p_addr, &_config.ble_address, sizeof(BLE_GAP_Addr_T));
    return MBA_RES_SUCCESS;
}

bool APPLICATION_CONFIG_ValidCheck(void) {
    bool ret_val = true;
    if (_config.can_address == 0x0000) { return false; }
    if (_config.ble_address.addr[0] == 0x00 &&
        _config.ble_address.addr[1] == 0x00 &&
        _config.ble_address.addr[2] == 0x00 &&
        _config.ble_address.addr[3] == 0x00 &&
        _config.ble_address.addr[4] == 0x00 &&
        _config.ble_address.addr[5] == 0x00 ) {
        return false;
    }
    return ret_val;
}

/* *****************************************************************************
 End of File
 */
