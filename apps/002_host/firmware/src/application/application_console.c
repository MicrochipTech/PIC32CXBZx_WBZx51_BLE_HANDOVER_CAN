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

#include "application_console.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

// Console Commands
volatile struct {
    union {
        struct {
            uint32_t    bit_0 : 1,
                        bit_1 : 1,
                        bit_2 : 1,
                        bit_3 : 1,
                        bit_4 : 1,
                        rfu : 27;
        };
        uint32_t v;
    } bits;
}_console_commands_data;


void _APPLICATION_CONSOLE_Activate(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    if (argc == 2) {
        int id;
        sscanf(argv[1], "%d", &id);
        if ( 1 <= id && id <= NUMBER_OF_NODES ) {
            uint16_t address = nodes[id-1].can_address;
            APPLICATION_NODE_HciHandover3(&broadcast, address);
            return;
        }
    }
    SYS_CONSOLE_PRINT("Usage: ACT <node_id>\r\n");
}


void _APPLICATION_CONSOLE_EnableHandover(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    if (argc == 2) {
        int enable_disable;
        sscanf(argv[1], "%d", &enable_disable);
        if (enable_disable) {
            data.handover_enabled = true;
            SYS_CONSOLE_PRINT("BLE Handover ENABLED\r\n");
        } else {
            data.handover_enabled = false;
            SYS_CONSOLE_PRINT("BLE Handover DISABLED\r\n");
        }
        return;
    }
    SYS_CONSOLE_PRINT("Usage: EHO <1=enable/0=disable>\r\n");
    
}

void _APPLICATION_CONSOLE_Cmd1(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    SYS_CONSOLE_PRINT("_APPLICATION_CMD_Cmd1\r\n");
}

void _APPLICATION_CONSOLE_Tst(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    if (argc == 2) {
        int id;
        sscanf(argv[1], "%d", &id);
        if (id == 1) {
            _console_commands_data.bits.bit_0 = true;
            return;
        }
    }
    SYS_CONSOLE_PRINT("Usage:  TST <id>\r\n");
}

void _APPLICATION_CMD_TstCallback(void) {
    SYS_CONSOLE_PRINT("\r\n --TST 1\r\n");
    USER_LED_1_Toggle();
}

void _APPLICATION_CONSOLE_Rst(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char **argv) {
    if (argc == 2) {
        int id;
        sscanf(argv[1], "%d", &id);
        _console_commands_data.bits.v |= 1 << id;
        return;        
    }
    SYS_CONSOLE_PRINT("Usage: RST <node_id>\r\n");
}

static const SYS_CMD_DESCRIPTOR _cmdTable[] = {
    {"ACT", _APPLICATION_CONSOLE_Activate, ":Activate Node"},
    {"EHO", _APPLICATION_CONSOLE_EnableHandover, ":Enable/Disable Handover"},
    {"CMD1", _APPLICATION_CONSOLE_Cmd1, ": Command 1"},
    {"TST", _APPLICATION_CONSOLE_Tst, ": Test Command"},
    {"RST", _APPLICATION_CONSOLE_Rst, ": Reset Command"},
};
#define  NUM_COMMANDS sizeof(_cmdTable)/sizeof(*_cmdTable)




/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void APPLICATION_CONSOLE_Initialize (void) {
    SYS_CMD_ADDGRP(_cmdTable, NUM_COMMANDS,"Commands", ": SAME54 Commands");
    // TODO: Add Implementation
    
}
void APPLICATION_CONSOLE_Tasks(void) {
    if (_console_commands_data.bits.v != 0) {
        if (_console_commands_data.bits.bit_0) {
            // Reset Host Controller
            _console_commands_data.bits.bit_0 = 0;
            SYS_RESET_SoftwareReset();
        }
        if (_console_commands_data.bits.bit_1) {
            // Reset Node #2
            _console_commands_data.bits.bit_1 = 0;
            // APPLICATION_NODE_HciReset(&nodes[0]);
            APPLICATION_NODE_HciReset(&nodes[0]);
        }
        if (_console_commands_data.bits.bit_2) {
            // Reset Node #2
            _console_commands_data.bits.bit_2 = 0;
            APPLICATION_NODE_HciReset(&nodes[1]);
        }
        if (_console_commands_data.bits.bit_3) {
            // Reset Node #3
            _console_commands_data.bits.bit_3 = 0;
            APPLICATION_NODE_HciReset(&nodes[2]);
        }
        if (_console_commands_data.bits.bit_4) {
            // Reset Node #4
            _console_commands_data.bits.bit_4 = 0;
            APPLICATION_NODE_HciReset(&nodes[3]);
        }
    }
    // TODO: Add Implementation
}




/* *****************************************************************************
 End of File
 */
