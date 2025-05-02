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

#ifndef _BLE_HANDOVER_HCI_H    /* Guard against multiple inclusion */
#define _BLE_HANDOVER_HCI_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
    #define PACKED                  __attribute__((packed))
    #define BD_ADDRESS_LEN  6

    #define HCI_VENDOR_SPECIFIC_HANDOVER_1  0xE2
    #define HCI_VENDOR_SPECIFIC_HANDOVER_2  0xE3
    #define HCI_VENDOR_SPECIFIC_HANDOVER_3  0xE4

    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************
    
    /*
     * Link Control commands
     */
    /*
     * Link Policy commands
     */
     /* 
      * Controller & Baseband commands
      */
    /*
     * Reset command
     */
    struct HCI_EVT_RESET_COMMAND_COMPLETE_PARAMS {
        uint8_t status;
    }PACKED; 
    typedef struct HCI_EVT_RESET_COMMAND_COMPLETE_PARAMS HCI_EVT_RESET_COMMAND_COMPLETE_PARAMS_T;

     /* 
      * Informational parameters
     */
     /* 
      * Status parameters
      */
    /*
     * Read RSSI command
     */
    struct HCI_EVT_READ_RSSI_COMMAND_COMPLETE_PARAMS{
        uint8_t status;
        uint16_t handle;
        uint8_t rssi;
        uint8_t channel;
    }PACKED;
    typedef struct HCI_EVT_READ_RSSI_COMMAND_COMPLETE_PARAMS HCI_EVT_READ_RSSI_COMMAND_COMPLETE_PARAMS_T;
    
     /* Testing commands
     */
    
     /* 
      * LE Controller commands
      */
    /*
     * LE Set Advertising Enable command
     */
    struct HCI_EVT_SET_ADVERTISING_ENABLE_COMMAND_COMPLETE_PARAMS {
        uint8_t status;
    }PACKED; 
    typedef struct HCI_EVT_SET_ADVERTISING_ENABLE_COMMAND_COMPLETE_PARAMS HCI_EVT_SET_ADVERTISING_ENABLE_COMMAND_COMPLETE_PARAMS_T;
    

    
     /* 
      * Events
      */
    /* Connection Complete event
     */
    struct HCI_EVT_CONNECTION_COMPLETE {
        uint8_t status;
        uint16_t conn_handle;
        uint8_t bd_addr[6];
        uint8_t link_type;
        uint8_t encryption_enabled;
    }PACKED;
    typedef struct HCI_EVT_CONNECTION_COMPLETE HCI_EVT_CONNECTION_COMPLETE_T;
    
    /*
     * Disconnection Complete event
     */
    struct HCI_EVT_DISCONNECTION_COMPLETE {
        uint8_t status;
        uint16_t conn_handle;
        uint8_t reason;
    }PACKED;
    typedef struct HCI_EVT_DISCONNECTION_COMPLETE HCI_EVT_DISCONNECTION_COMPLETE_T;
    
    /*
     * Command Complete event
     */
    
    struct HCI_EVT_COMMAND_COMPLETE {

        uint8_t num_hci_command_packets;
        union {
            struct {
                uint16_t    ocf : 10,
                            ogf : 6;
            };
            uint16_t op_code;
        };
        union {
            uint8_t return_parameters[255];
            HCI_EVT_RESET_COMMAND_COMPLETE_PARAMS_T hci_reset;
            HCI_EVT_READ_RSSI_COMMAND_COMPLETE_PARAMS_T hci_read_rssi;
            HCI_EVT_SET_ADVERTISING_ENABLE_COMMAND_COMPLETE_PARAMS_T hci_set_advertising_enable;
        };
    }PACKED;
    typedef struct HCI_EVT_COMMAND_COMPLETE HCI_EVT_COMMAND_COMPLETE_T;
    /*
     * LE Meta event
     */
    /*
     * LE Connection Complete event
     */
    struct HCI_EVT_LE_CONNECTION_COMPLETE {
        uint8_t subevent_code;
        uint8_t status;
        uint16_t conn_handle;
        uint8_t role;
        uint8_t peer_addr_type;
        uint8_t peer_addr[BD_ADDRESS_LEN];
        uint16_t connection_interval;
        uint16_t connection_latency;
        uint16_t supervision_timeout;
        uint8_t master_clock_accuracy;
    }PACKED;
    typedef struct HCI_EVT_LE_CONNECTION_COMPLETE HCI_EVT_LE_CONNECTION_COMPLETE_T;

    
    struct HCI_DISCONNECT_COMMAND_PARAMETER {
        uint16_t connection_handle;
        uint8_t reason;
    }PACKED;
    typedef struct HCI_DISCONNECT_COMMAND_PARAMETER HCI_DISCONNECT_COMMAND_PARAMETER_T;

    /*
     * HCI COMMAND PACKET
     */
    
    struct HCI_COMMAND_PACKET {
        union {
            struct {
                uint16_t    ocf : 10,
                            ogf : 6;
            };
            uint16_t op_code;
        };
        uint8_t parameter_total_length;
        union {
            uint8_t parameter[255];
            HCI_DISCONNECT_COMMAND_PARAMETER_T hci_disconnect_command_parameters;
        };
    }PACKED;
    typedef struct HCI_COMMAND_PACKET HCI_COMMAND_PACKET_T;
    
    /*
     * HCI ACL DATA PACKET
     */
    struct _HCI_ACL_DATA_PACKET {
        uint16_t    handle : 12,
                    packet_boundary_flag : 2,
                    broadcast_flag : 2;
        uint32_t data_total_length;
        uint8_t data[27];
    }PACKED;
    typedef struct HCI_ACL_DATA_PACKET HCI_ACL_DATA_PACKET_T;

    /*
     * HCI SYNCHRONOUS DATA PACKET
     */
    struct HCI_SYNCHRONOUS_DATA_PACKET {
        uint16_t    connection_handle : 12,
                    packet_status_flag : 2,
                    rfu : 2;
        uint8_t data_total_length;
        uint8_t data[255];
    }PACKED;
    typedef struct HCI_SYNCHRONOUS_DATA_PACKET HCI_SYNCHRONOUS_DATA_PACKET_T;
    
    /*
     * HCI EVENT PACKET
     */
    
    struct HCI_EVENT_PACKET {
        uint8_t event_code;
        uint8_t parameter_total_length;
        union {
            uint8_t event_parameter[255];
            HCI_EVT_COMMAND_COMPLETE_T command_complete_event_parameters;
            HCI_EVT_CONNECTION_COMPLETE_T connection_complete_event_parameters;
            HCI_EVT_DISCONNECTION_COMPLETE_T disconnection_complete_event_parameters;
            HCI_EVT_LE_CONNECTION_COMPLETE_T le_connection_complete_event_parameters;
        };
    }PACKED;
    typedef struct HCI_EVENT_PACKET HCI_EVENT_PACKET_T;

    /*
     * HCI ISO DATA PACKET
     */
    struct HCI_ISO_DATA_PACKET {
        uint16_t    connection_handle : 12,
                    packet_boundary_flag : 2,
                    time_stamp_field : 1,
                    rfu1 : 1;
        uint16_t    iso_data_total_length : 14,
                    rfu2 : 2;
        uint8_t iso_data_load[255];
    }PACKED;
    typedef struct HCI_ISO_DATA_PACKET HCI_ISO_DATA_PACKET_T;
    
    /*
     * HCI PACKET
     */
    
    struct HCI_PACKET {
        uint8_t packet_type;
        union {
            HCI_COMMAND_PACKET_T command_packet;
            // HCI_ACL_DATA_PACKET_T acl_data_packet;
            // HCI_SYNCHRONOUS_DATA_PACKET_T synchronous_data_packet;
            HCI_EVENT_PACKET_T event_packet;
            // HCI_ISO_DATA_PACKET_T iso_data_packet;
        };
    }PACKED;
    typedef struct HCI_PACKET HCI_PACKET_T;    


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _BLE_HANDOVER_HCI_H */

/* *****************************************************************************
 End of File
 */
