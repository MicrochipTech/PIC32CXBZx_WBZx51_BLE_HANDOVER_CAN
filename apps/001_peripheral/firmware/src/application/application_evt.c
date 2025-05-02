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

#include "app.h"
#include "application_evt.h"
#include "application_dbg.h"
#include "definitions.h"
#include "stack_mgr.h"
#include "ble_gap.h"
#include "application_tempsensor.h"
#include "application_led.h"
#include "application_cmd.h"
#include "application_config.h"
#include "host_hci_defs.h"
#include <stdio.h>

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#if true
#define APPLICATION_EVT_DEBUG(__PRINT__)
#else
#define APPLICATION_EVT_DEBUG(__PRINT__)  do { \
                                              SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,"APPLICATION_EVT[%d] ", __LINE__); \
                                              SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,__PRINT__); \
                                              SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,"\r\n"); \
                                          } while (0)
#endif
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void APPLICATION_EVT_Initialize(void) {
}

void APPLICATION_EVT_Tasks(APP_Msg_T *pMsg) {
    uint8_t id = pMsg->msgId;
    uint16_t addr = APPLICATION_CONFIG_GetCanAddress();
    
    switch (id) {
        case APP_MSG_BLE_STACK_EVT: { 
            STACK_Event_T *pStackEvt = (STACK_Event_T *)pMsg->msgData;
            switch (pStackEvt->groupId) {
                case STACK_GRP_BLE_GAP: { 
                    BLE_GAP_Event_T *pEvt = (BLE_GAP_Event_T *)pStackEvt->p_event;
                    switch (pEvt->eventId) {
                        case BLE_GAP_EVT_CONNECTED: { 
                            BLE_GAP_EvtConnect_T *pEvtConnected = (BLE_GAP_EvtConnect_T *)pStackEvt->p_event;
                            if (pEvtConnected->status == GAP_STATUS_SUCCESS) {
                                // SYS_CONSOLE_PRINT("BLE_GAP_EVT_CONNECTED: 0x%04X\r\n",pEvtConnected->connHandle);
                                data.status.isConnected = true;
                                
                                HCI_EVT_CONNECTION_COMPLETE_T param = {
                                    .status = STATUS_SUCCESS,
                                    .conn_handle = pEvtConnected->connHandle,
                                    .bd_addr = {0},
                                    .link_type = 0x00,
                                    .encryption_enabled = 0x00,
                                };
                                memcpy(param.bd_addr, pEvtConnected->remoteAddr.addr, sizeof(GAP_MAX_BD_ADDRESS_LEN));
                                APPLICATION_CMD_HciEventConnectionComplete(addr, &param);

                            } else {
                                // TODO: Add Error Handling
                                APPLICATION_EVT_DEBUG("TODO: Add Error Handling");
                            }
                            break; 
                        }
                        case BLE_GAP_EVT_DISCONNECTED: { 
                            
                            data.status.isConnected = false;
                            BLE_GAP_EvtDisconnect_T *pEvtDisconnected = (BLE_GAP_EvtDisconnect_T *)pStackEvt->p_event;
                            // SYS_CONSOLE_PRINT("BLE_GAP_EVT_DISCONNECTED\r\n");
                            
                            HCI_EVT_DISCONNECTION_COMPLETE_T param = {
                                .status = 0x00,
                                .conn_handle = pEvtDisconnected->connHandle,
                                .reason = pEvtDisconnected->reason,
                            };
                            APPLICATION_CMD_HciEventDisconnectionComplete(addr, &param);
                            break; 
                        }
                        case BLE_GAP_EVT_CONN_PARAM_UPDATE: { 
                            APPLICATION_EVT_DEBUG("BLE_GAP_EVT_CONN_PARAM_UPDATE"); 
                            break; 
                        }
                        case BLE_GAP_EVT_ENCRYPT_STATUS: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_ENCRYPT_STATUS"); break; }
                        case BLE_GAP_EVT_ADV_REPORT: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_ADV_REPORT"); break; }
                        case BLE_GAP_EVT_ENC_INFO_REQUEST: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_ENC_INFO_REQUEST"); break; }
                        case BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST"); break; }
                        case BLE_GAP_EVT_EXT_ADV_REPORT: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_EXT_ADV_REPORT"); break; }
                        case BLE_GAP_EVT_ADV_TIMEOUT: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_ADV_TIMEOUT"); break; }
                        case BLE_GAP_EVT_TX_BUF_AVAILABLE: { 
                            APPLICATION_EVT_DEBUG("BLE_GAP_EVT_TX_BUF_AVAILABLE"); 
                            break; 
                        }
                        case BLE_GAP_EVT_DEVICE_NAME_CHANGED: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_DEVICE_NAME_CHANGED"); break; }
                        case BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT"); break; }
                        case BLE_GAP_EVT_PHY_UPDATE: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_PHY_UPDATE"); break; }
                        case BLE_GAP_EVT_SCAN_REQ_RECEIVED: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_SCAN_REQ_RECEIVED"); break; }
                        case BLE_GAP_EVT_DIRECT_ADV_REPORT: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_DIRECT_ADV_REPORT"); break; }
                        case BLE_GAP_EVT_PERI_ADV_SYNC_EST: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_PERI_ADV_SYNC_EST"); break; }
                        case BLE_GAP_EVT_PERI_ADV_REPORT: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_PERI_ADV_REPORT"); break; }
                        case BLE_GAP_EVT_PERI_ADV_SYNC_LOST: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_PERI_ADV_SYNC_LOST"); break; }
                        case BLE_GAP_EVT_ADV_SET_TERMINATED: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_ADV_SET_TERMINATED"); break; }
                        case BLE_GAP_EVT_SCAN_TIMEOUT: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_SCAN_TIMEOUT"); break; }
                        case BLE_GAP_EVT_TRANSMIT_POWER_REPORTING: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_TRANSMIT_POWER_REPORTING"); break; }
                        case BLE_GAP_EVT_ADV_COMPL: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_ADV_COMPL"); break; }
                        case BLE_GAP_EVT_PATH_LOSS_THRESHOLD: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_PATH_LOSS_THRESHOLD"); break; }
                        case BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL: { APPLICATION_EVT_DEBUG("BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL"); break; }
                        default: { break; }
                    }
                    
                    break; 
                }
                case STACK_GRP_BLE_L2CAP: { 
                    APPLICATION_EVT_DEBUG("STACK_GRP_BLE_L2CAP");
                    break; 
                }
                case STACK_GRP_BLE_SMP: { 
                    APPLICATION_EVT_DEBUG("STACK_GRP_BLE_SMP");
                    break; 
                }
                case STACK_GRP_GATT: { 
                    APPLICATION_EVT_DEBUG("STACK_GRP_GATT");
                    break; 
                }
                default: { break; }
            }
            break; 
        }
        case APP_MSG_BLE_STACK_LOG: { break; }
        case APP_MSG_ZB_STACK_EVT: { break; }
        case APP_MSG_ZB_STACK_CB: { break; }
        case APP_MSG_APP_TRIGGER: { break; }
        case APP_MSG_SW1_PRESSED: 
        { 
            // APPLICATION_EVT_DEBUG("SW1_PRESSED");
    
            float temp = APPLICATION_TEMPSENSOR_Get();
            char buffer[128] = {0};
            sprintf(buffer, "Temperature: %2.2f�C",temp);
            APPLICATION_EVT_DEBUG(buffer);
            // APPLICATION_RGB_LED_GREEN_On();
            // APPLICATION_RGB_LED_SetColor(0x00,0xFF,0x00);
            
            break; 
        }
        case APP_MSG_SW1_RELEASED: 
        { 
            APPLICATION_EVT_DEBUG("SW1_RELEASED");
            // APPLICATION_RGB_LED_Off();
            break; 
        }
        case APP_MSG_RSSI_CHANGED:
        {
            RSSI_MSG_STRUCT_T *p_rssi_msg = (RSSI_MSG_STRUCT_T *)pMsg->msgData;
            // SYS_CONSOLE_PRINT("RSSI: 0x%04X - %d dBm\r\n",p_rssi_msg->conn_handle, p_rssi_msg->rssi);
            uint8_t channel = BLE_HANDOVER_GetChannel();
            // SYS_CONSOLE_PRINT("[%2d]RSSI: %d dBm\r\n", channel, p_rssi_msg->rssi);
            uint16_t addr = APPLICATION_CONFIG_GetCanAddress();
            HCI_EVT_READ_RSSI_COMMAND_COMPLETE_PARAMS_T param = {
                .status = STATUS_SUCCESS,
                .handle = p_rssi_msg->conn_handle, 
                .rssi = p_rssi_msg->rssi,
                .channel = channel,
            };
            
            APPLICATION_CMD_HciReadRssiCommandComplete(addr,&param);
            break;
        }
        
        default: { break; }
    }

}


/* *****************************************************************************
 End of File
 */
