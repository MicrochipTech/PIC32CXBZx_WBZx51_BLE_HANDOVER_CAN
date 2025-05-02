/* ************************************************************************** */
/* Descriptive File Name

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
/** \file application_handover.h
    BLE Handover Demo include file
*/

#ifndef _APPLICATION_HANDOVER_H    /* Guard against multiple inclusion */
#define _APPLICATION_HANDOVER_H


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
#define HCI_OPCODE(ogf, ocf)                         (((ogf) << 10) + (ocf))    /**< Macro for generating 16-bit OpCode */
#define HCI_OGF(opcode)                              ((opcode) >> 10)   /**< Macro for decoding OGF from 16-bit OpCode */
#define HCI_OCF(opcode)                              ((opcode) & 0x03FF)    /**< Macro for decoding OCF from 16-bit OpCode */

/*
 * HCI Packet Type
 */
enum {
    HCI_COMMAND_PACKET_INDICATOR = 0x01,
    HCI_ACL_DATA_PACKET_INDICATOR = 0x02,
    HCI_SYNCHRONOUS_DATA_PACKET_INDICATOR = 0x03,
    HCI_EVENT_PACKET_INDICATOR = 0x04,
    HCI_ISO_DATA_PACKET_INDICATOR = 0x05,
};

/*
 * OpCode Group Field (OGF)
 */
enum {
    OGF_LINK_CONTROL_COMMANDS = 0x01,
    OGF_LINK_POLICY_COMMANDS = 0x02,
    OGF_HOST_CONTROLLER_AND_BASEBAND_COMMANDS = 0x03,
    OGF_INFORMATIONAL_PARAMETERS = 0x04,
    OGF_STATUS_PARAMETERS = 0x05,
    OGF_TESTING_COMMANDS = 0x06,
    OGF_LE_CONTROLLER_COMMANDS = 0x08,
    OGF_VENDOR_SPECIFIC_COMMANDS = 0x3F,
};

/*
 * HCI Commands And Events
 */
 /*
  * Link Control Commands
  */
enum {
    OCF_HCI_DISCONNECT  = 0x06,
};

 /*
  * Link Policy Commands
  */
 
/* 
  *  Controller & Baseband Commands
  */
enum {
    OCF_HCI_RESET = 0x03,
};
/*  Informational Parameters */
/*  Status Parameters */
enum {
    OCF_HCI_READ_RSSI = 0x0005,
};

/*  Testing Commands
 *  Events
 */

/*
 *  LE Controller Commands
 */
enum {
    OCF_LE_SET_ADVERTISING_ENABLE = 0x0A,
};

/*
 * Vendor specific OpCode Command Fields (OCF)
 */
enum {
    OCF_VENDOR_SPECIFIC_GET_NODE_VERSION = 0x01,
    OCF_VENDOR_SPECIFIC_RESET_NODE = 0x02,
    OCF_VENDOR_SPECIFIC_HANDOVER_1 = 0xE2,
    OCF_VENDOR_SPECIFIC_HANDOVER_2 = 0xE3,
    OCF_VENDOR_SPECIFIC_HANDOVER_3 = 0xE4,
};

enum {
    HCI_STATUS_SUCCESS = 0x00, // Success
    HCI_STATUS_UNKNOWN_HCI_COMMAND = 0x01, // Unknown HCI Command
    HCI_STATUS_UNKNOWN_CONNECTION_IDENTIFIER = 0x02, // Unknown Connection Identifier
    HCI_STATUS_HARDWARE_FAILURE = 0x03, // Hardware Failure
    HCI_STATUS_PAGE_TIMEOUT = 0x04, // Page Timeout
    HCI_STATUS_AUTHENTICATION_FAILURE = 0x05, // Authentication Failure
    HCI_STATUS_PIN_OR_KEY_MISSING = 0x06, // PIN or Key Missing
    HCI_STATUS_MEMOARY_CAPACITY_EXCEEDED = 0x07, // Memory Capacity Exceeded
    HCI_STATUS_CONNECTION_TIMEOUT = 0x08, // Connection Timeout
    HCI_STATUS_CONNECTION_LIMIT_EXCEEDED = 0x09, // Connection Limit Exceeded
    HCI_STATUS_SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED = 0x0A, // Synchronous Connection Limit To A Device Exceeded
    HCI_STATUS_CONNECTION_ALREADY_EXISTS = 0x0B, // Connection Already Exists
    HCI_STATUS_COMMAND_DISALLOWED = 0x0C, // Command Disallowed
    HCI_STATUS_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES = 0x0D, // Connection Rejected due to Limited Resources
    HCI_STATUS_CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS = 0x0E, // Connection Rejected Due To Security Reasons
    HCI_STATUS_CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR = 0x0F, // Connection Rejected due to Unacceptable BD_ADDR
    HCI_STATUS_CONNECTION_ACCEPT_TIMEOUT_EXCEEDED = 0x10, // Connection Accept Timeout Exceeded
    HCI_STATUS_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE = 0x11, // Unsupported Feature or Parameter Value
    HCI_STATUS_INVALID_HCI_COMMAND_PARAMETERS = 0x12, // Invalid HCI Command Parameters
    HCI_STATUS_REMOTE_USER_TERMINATED_CONNECTION = 0x13, // Remote User Terminated Connection
    HCI_STATUS_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES = 0x14, // Remote Device Terminated Connection due to Low Resources
    HCI_STATUS_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_To_POWER_OFF = 0x15, // Remote Device Terminated Connection due to Power Off
    HCI_STATUS_CONNECTION_TERMINATED_BY_LOCAL_HOST = 0x16, // Connection Terminated By Local Host
    HCI_STATUS_REPEATED_ATTEMPTS = 0x17, // Repeated Attempts
    HCI_STATUS_PAIRING_NOT_ALLOWED = 0x18, // Pairing Not Allowed
    HCI_STATUS_UNKNOWN_LMP_PDU = 0x19, // Unknown LMP PDU
    HCI_STATUS_UNSUPPORTED_REMOTE_FEATURE_UNSUPPORTED_LMP_FEATURE = 0x1A, // Unsupported Remote Feature / Unsupported LMP Feature
    HCI_STATUS_SCO_OFFSET_REJECTED = 0x1B, // SCO Offset Rejected
    HCI_STATUS_SCO_INTERVAL_REJECTED = 0x1C, // SCO Interval Rejected
    HCI_STATUS_SCO_AIR_MODE_REJECTED = 0x1D, // SCO Air Mode Rejected
    HCI_STATUS_UNVALID_LMP_PARAMETERS_INVALIE_LL_PARAMETERS = 0x1E, // Invalid LMP Parameters / Invalid LL Parameters
    HCI_STATUS_UNSPECIFIED_ERRROR = 0x1F, // Unspecified Error
    HCI_STATUS_UNSUPPORTED_LMP_PARAMETER_VALUE_UNSUPPORTED_LL_PARAMETER_VALUE = 0x20, // Unsupported LMP Parameter Value / Unsupported LL Parameter Value
    HCI_STATUS_ROLE_CHANGE_NOT_ALLOWED = 0x21, // Role Change Not Allowed
    HCI_STATUS_LMP_RESPONSE_TIMEOUT_LL_RESPONSE_TIMEOUT = 0x22, // LMP Response Timeout / LL Response Timeout
    HCI_STATUS_LMP_ERROR_TRANSACTION_COLLISION_LL_PROCEDURE_COLLISION = 0x23, // LMP Error Transaction Collision / LL Procedure Collision
    HCI_STATUS_LMP_PDU_NOT_ALLOWED = 0x24, // LMP PDU Not Allowed
    HCI_STATUS_ENCRYPTION_MODE_NOT_ACCEPTABLE = 0x25, // Encryption Mode Not Acceptable
    HCI_STATUS_LINK_KEY_CANNOT_BE_CHANGED = 0x26, // Link Key cannot be Changed
    HCI_STATUS_REQUESTED_QOS_NOT_SUPPORTED = 0x27, // Requested QoS Not Supported
    HCI_STATUS_INSTANT_PASSED = 0x28, // Instant Passed
    HCI_STATUS_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED = 0x29, // Pairing With Unit Key Not Supported
    HCI_STATUS_DIFFERENT_TRANSACTION_COLLISION = 0x2A, // Different Transaction Collision
    HCI_STATUS_RESERVED_FOR_FUTURE_USE_X2B = 0x2B, // Reserved for future use
    HCI_STATUS_QOS_UNACCEPTABLE_PARAMETER = 0x2C, // QoS Unacceptable Parameter
    HCI_STATUS_QOS_REJECTED = 0x2D, // QoS Rejected
    HCI_STATUS_CHANNEL_CLASSIFICATION_NOT_SUPPORTED = 0x2E, // Channel Classification Not Supported
    HCI_STATUS_INSUFFICIENT_SECURITY = 0x2F, // Insufficient Security
    HCI_STATUS_PARAMETER_OUT_OF_MANDATORY_RANGE = 0x30, // Parameter Out Of Mandatory Range
    HCI_STATUS_RESERVED_FOR_FUTURE_USE_X31 = 0x31, // Reserved for future use
    HCI_STATUS_ROLE_SWITCH_PENDING = 0x32, // Role Switch Pending
    HCI_STATUS_RESERVED_FOR_FUTURE_USE_X33 = 0x33, // Reserved for future use
    HCI_STATUS_RESERVED_SLOT_VIOLATION = 0x34, // Reserved Slot Violation
    HCI_STATUS_ROLE_SWITCH_FAILED = 0x35, // Role Switch Failed
    HCI_STATUS_EXTENDED_INQUIRY_RESPONSE_TOO_LARGE = 0x36, // Extended Inquiry Response Too Large
    HCI_STATUS_SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST = 0x37, // Secure Simple Pairing Not Supported By Host
    HCI_STATUS_HOST_BUSY_PAIRING = 0x38, // Host Busy - Pairing
    HCI_STATUS_CONNECTION_REJECTED_DUE_TO_NO_SUITABLE_CHANNEL_FOUND = 0x39, // Connection Rejected due to No Suitable Channel Found
    HCI_STATUS_CONTROLLER_BUSY = 0x3A, // Controller Busy
    HCI_STATUS_UNACCEPTABLE_CONNECTION_PARAMETERS = 0x3B, // Unacceptable Connection Parameters
    HCI_STATUS_ADVERTISING_TIMEOUT = 0x3C, // Advertising Timeout
    HCI_STATUS_CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE = 0x3D, // Connection Terminated due to MIC Failure
    HCI_STATUS_CONNECTION_FAILED_TO_BE_ESTABLISHED_SYNCHRONIUATION_TIMEOUT = 0x3E, // Connection Failed to be Established / Synchronization Timeout
    HCI_STATUS_MAC_CONNECTION_FAILED = 0x3F, // MAC Connection Failed
    HCI_STATUS_COARSE_CLOCK_ADJUSTMENT_REJECTED = 0x40, // Coarse Clock Adjustment Rejected but Will Try to Adjust Using Clock Dragging
    HCI_STATUS_TYPE0_SUBMAP_NOT_DEFINED = 0x41, // Type0 Submap Not Defined
    HCI_STATUS_UNKNOWN_ADVERTISING_IDENTIFIER = 0x42, // Unknown Advertising Identifier
    HCI_STATUS_LIMIT_REACHED = 0x43, // Limit Reached
    HCI_STATUS_OPERATION_CANCELLED_BY_HOST = 0x44, // Operation Canceled by Host
    HCI_STATUS_PACKET_TO_LONG = 0x45, // Packet Too Long
};


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************


/**
Union for 16-bit OpCode used for HCI Command
 */    
union OP_CODE{
    struct {
        uint16_t    ocf : 10,   /**< 10-bit OpCode Command Field (OCF) */
                    ogf : 6;    /**< 6-bit OpCode Group Field (OGF) */
    };
    uint16_t op_code;   /**< 16-bit OpCode */
};
/**
Union for 16-bit OpCode used for HCI Command
see ::OP_CODE
 */    
typedef union OP_CODE OP_CODE_T;

/**
Data structure for HCI commmand parameters of HCI command 'HCI_Disconnect'
 */
struct HCI_DISCONNECT_COMMAND_PARAMETERS {
    uint16_t connection_handle; /**< Connection Handle */
    uint8_t reason; /**< Disconnection Reset */
}__PACKED;
/**
Data structure for HCI commmand parameters of HCI command 'HCI_Disconnect'
see ::HCI_DISCONNECT_COMMAND_PARAMETERS
 */
typedef struct HCI_DISCONNECT_COMMAND_PARAMETERS HCI_DISCONNECT_COMMAND_PARAMETERS_T;

/**
Data structure for vendor specific HCI command 'Handover3'
 */
struct HCI_VENDOR_SPECIFIC_HANDOVER3_COMMAND_PARAMETERS {
    uint16_t address;   /**< CAN address of the active device */
}__PACKED;
/**
Data structure for vendor specific HCI command 'Handover3'
see ::HCI_VENDOR_SPECIFIC_HANDOVER3_COMMAND_PARAMETERS
 */
typedef struct HCI_VENDOR_SPECIFIC_HANDOVER3_COMMAND_PARAMETERS HCI_VENDOR_SPECIFIC_HANDOVER3_COMMAND_PARAMETERS_T;

/**
Data structue for HCI command packet
 */
struct HCI_COMMAND_PACKET {
    union {
        struct {
            uint16_t    ocf : 10,   /**< 10-bit OpCode Command Field (OCF) */
                        ogf : 6;    /**< 6-bit OpCode Group Field (OGF) */
        };
        uint16_t op_code;   /**< 16-bit OpCode */
    };
    uint8_t parameter_total_length; /**< Length of all parameters contained in this packet. */
    union {
        uint8_t parameter[255]; /**< Command parameters */
        HCI_DISCONNECT_COMMAND_PARAMETERS_T hci_disconnect_command_parameters;  /**< Command parameters for HCI command 'HCI_Disconnect' */
        HCI_VENDOR_SPECIFIC_HANDOVER3_COMMAND_PARAMETERS_T hci_vendor_specific_handover3_command_parameters;    /**< Command parameters for HCI command 'HCI_Vendor_Specific_Handover3 */
    };
}__PACKED;
/**
Data structue for HCI command packet
see ::HCI_COMMAND_PACKET
 */
typedef struct HCI_COMMAND_PACKET HCI_COMMAND_PACKET_T;

/**
Data structure for HCI Event 'HCI_Connection_Complete'
 */
struct HCI_CONNECTION_COMPLETE_EVENT_PARAMETERS {
    uint8_t status;     /**< Connection status */
    uint16_t conn_handle;   /**< Connection handle for the new connection */
    uint8_t bd_addr[6];     /**< BD_ADDR of the other connected device forming the connection. */
    uint8_t link_type;      /**< Link Type */
    uint8_t encryption_enabled; /**< Encyrption Enabled */
}__PACKED;
/**
Data structure for HCI Event 'HCI_Connection_Complete'
see ::HCI_CONNECTION_COMPLETE_EVENT_PARAMETERS
 */
typedef struct HCI_CONNECTION_COMPLETE_EVENT_PARAMETERS HCI_CONNECTION_COMPLETE_EVENT_PARAMETERS_T;

/**
Data structure for HCI Event 'HCI_Disconnection_Complete'
 */
struct HCI_DISCONNECTION_COMPLETE_EVENT_PARAMETERS {
    uint8_t status;     /**< Disconnetion status */
    uint16_t conn_handle;   /**< Connection_Handle which was disconnected */
    uint8_t reason; /**< Reason for disconnetion. */
}__PACKED;
/**
Data structure for HCI Event 'HCI_Disconnection_Complete'
see ::HCI_DISCONNECTION_COMPLETE_EVENT_PARAMETERS
 */
typedef struct HCI_DISCONNECTION_COMPLETE_EVENT_PARAMETERS HCI_DISCONNECTION_COMPLETE_EVENT_PARAMETERS_T;

/**
Data structure for Reset Command Complete event parameters
 */
struct HCI_RESET_COMMAND_COMPLETE_EVENT_PARAMETERS {
    uint8_t status; /**< Reset status */
}__PACKED;
/**
Data structure for Reset Command Complete event parameters
see ::HCI_RESET_COMMAND_COMPLETE_EVENT_PARAMETERS
 */
typedef struct HCI_RESET_COMMAND_COMPLETE_EVENT_PARAMETERS HCI_RESET_COMMAND_COMPLETE_EVENT_PARAMETERS_T;

/**
Data structure for Read Rssi Command Complete Event Parameters
 */
struct HCI_READ_RSSI_COMMAND_COMPLETE_EVENT_PARAMETERS {
    uint8_t status;     /**< HCI Read Rssi command status */
    uint16_t handle;    /**< The Handle for the connection for which the RSSI is to be read. */
    int8_t rssi;    /**< RSSI value */
}__PACKED;
/**
Data structure for Read Rssi Command Complete event parameters
see ::HCI_READ_RSSI_COMMAND_COMPLETE_EVENT_PARAMETERS
 */
typedef struct HCI_READ_RSSI_COMMAND_COMPLETE_EVENT_PARAMETERS HCI_READ_RSSI_COMMAND_COMPLETE_EVENT_PARAMTERS_T;

/**
Data structure for Reset Command Complete event parameters 
*/
struct HCI_SET_ADVERTISING_ENABLE_COMMAND_COMPLETE_EVENT_PARAMETERS {
    uint8_t status;     /**< Status */
}__PACKED;
/**
Data structure for Reset Command Complete event parameters 
see ::HCI_SET_ADVERTISING_ENABLE_COMMAND_COMPLETE_EVENT_PARAMETERS
*/
typedef struct HCI_SET_ADVERTISING_ENABLE_COMMAND_COMPLETE_EVENT_PARAMETERS HCI_SET_ADVERTISING_ENABLE_COMMAND_COMPLETE_EVENT_PARAMETERS_T;

/**
Data structure for Command Complete event
 */
struct HCI_COMMAND_COMPLETE_EVENT_PARAMETERS {
    uint8_t num_hci_command_packets;    /**< the number of HCI command packets which are allowed to be sent to the Controller from the Host */
    union {
        struct {
            uint16_t    ocf : 10,   /**< 10-bit OpCode Command Field (OCF) */
                        ogf : 6;    /**< 6-bit OpCode Group Field (OGF) */
        };
        uint16_t op_code;   /**< 16-bit OpCode */
    };
    union {
        uint8_t return_parameters[252]; /**< This is the return parameter(s) for the command specified in the Command_Opcode event parameter. */
        HCI_RESET_COMMAND_COMPLETE_EVENT_PARAMETERS_T hci_reset_parameters;     /**< return parameters for HCI command 'HCI_Reset' */
        HCI_READ_RSSI_COMMAND_COMPLETE_EVENT_PARAMTERS_T hci_read_rssi_parameters;  /**< return parameters for HCI command 'HCI_Read_Rssi' */
        HCI_SET_ADVERTISING_ENABLE_COMMAND_COMPLETE_EVENT_PARAMETERS_T hci_set_advertising_enable_parameters; /**< return parameters for HCI command 'LE Set Advertising enable' */
        
    };
}__PACKED;

/**
Data structure for Command Complete event
see ::HCI_COMMAND_COMPLETE_EVENT_PARAMETERS
 */
typedef struct HCI_COMMAND_COMPLETE_EVENT_PARAMETERS HCI_COMMAND_COMPLETE_EVENT_PARAMETERS_T;

/**
Data structure for 'LE Connection Complete' event paramters
 */
struct HCI_LE_CONNECTION_COMPLETE_PARAMETERS {
    uint8_t subevent_code;  /**< Subevent code for the HCI_LE_Connection_Complete event (0x01) */
    uint8_t status;     /**< Connection status  */
    uint16_t connection_handle; /**< Connection Handle */
    uint8_t role;   /**< Connection Role (0x00: Connection is master/ 0x01: Connection is slave) */
    uint8_t peer_address_type;  /**< Peer Address Type (0x00: Peer is using a Public Device Address / Peer is using a Random Device Address) */
    uint8_t peer_address[6];    /**< Public Device Address or Random Device Address of the peer device */
    uint16_t connection_interval;   /**< Connection interval used in this connection. */
    uint16_t connection_latency;    /**< Slave latency for the connection in number of connection events. */
    uint16_t supervision_timeout;   /**< Connection supervision timeout */
    uint8_t master_clock_accuracy;  /**< Master Clock Accuracy */
}__PACKED;
/**
Data structure for 'LE Connection Complete' event paramters
see ::HCI_LE_CONNECTION_COMPLETE_PARAMETERS
 */
typedef struct HCI_LE_CONNECTION_COMPLETE_PARAMETERS HCI_LE_CONNECTION_COMPLETE_PARAMETERS_T;

/**
Data structure for HCI Event Packet
 */
struct HCI_EVENT_PACKET {
    uint8_t event_code;     /**< each event is asssigned a 1-octet event code used to uniquely identify different types of events. */
    uint8_t parameter_total_length; /**< length of all of the parameters contained in this packet. */
    union {
        uint8_t event_parameter[255];   /**< each event has a specified number parameters associated with it. */
        HCI_CONNECTION_COMPLETE_EVENT_PARAMETERS_T connection_complete;     /**< event parameters for 'Connection Complete' event */
        HCI_DISCONNECTION_COMPLETE_EVENT_PARAMETERS_T disconnection_complete;   /**< event parameters for 'Disconnection Complete' event */
        HCI_COMMAND_COMPLETE_EVENT_PARAMETERS_T command_complete;   /**< event parameters for 'Command Complete' event */
        HCI_LE_CONNECTION_COMPLETE_PARAMETERS_T le_connection_complete;   /**< event parameters for 'LE Connection Complete' event */
    };
}__PACKED;
/**
Data structure for HCI Event Packet
see ::HCI_EVENT_PACKET
 */
typedef struct HCI_EVENT_PACKET HCI_EVENT_PACKET_T;

/**
Data structure for HCI packets
 */
struct HCI_PACKET {
    uint8_t packet_type;    /**< Packet Type */
    union {
        HCI_COMMAND_PACKET_T command_packet;    /**< HCI Command Packet */
        HCI_EVENT_PACKET_T event_packet;    /**< HCI Event Packet */
    };
};
/**
Data structure for HCI packets
see ::HCI_PACKET
 */
typedef struct HCI_PACKET HCI_PACKET_T;

/*
 * Event Codes
 */
enum {
    HCI_INQUIRY_COMPLETE = 0x01,
    HCI_INQUIRY_RESULT = 0x02,
    HCI_CONNECTION_COMPLETE = 0x03,
    HCI_CONNECTION_REQUEST = 0x04,
    HCI_DISCONNECTION_COMPLETE = 0x05,
    HCI_AUTHENTICATION_COMPLETE = 0x06,
    HCI_REMOTE_NAME_REQUEST_COMPLETE = 0x07,
    HCI_REMOTE_ENCRYPTION_CHANGE = 0x08,
    HCI_CHANGE_CONNECTION_LINK_KEY_COMPLETE = 0x09,
    HCI_MASTER_LINK_KEY_COMPLETE = 0x0A,
    HCI_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE = 0x0B,
    HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE = 0x0C,
    HCI_QOS_SETUP_COMPLETE = 0x0D,
    HCI_COMMAND_COMPLETE = 0x0E,
    HCI_COMMAND_STATUS = 0x0F,
    HCI_HARDWARE_ERROR = 0x10,
    HCI_FLUSH_OCCURED = 0x11,
    HCI_ROLE_CHANGE = 0x12,
    HCI_NUMBER_OF_COMPLETED_PACKETS = 0x13,
    HCI_MODE_CHANGE = 0x14,
    HCI_RETURN_LINK_KEYS = 0x15,
    HCI_PIN_CODE_REQUEST = 0x16,
    HCI_LINK_KEY_REQUEST = 0x17,
    HCI_LINK_KEY_NOTIFICATION = 0x18,
    HCI_LOOPBACK_COMMAND = 0x19,
    HCI_DATA_BUFFER_OVERFLOW = 0x1A,
    HCI_MAX_SLOTS_CHANGE = 0x1B,
    HCI_READ_CLOCK_OFFSET_COMPLETE = 0x1C,
    HCI_CONNECTION_PACKET_TYPE_CHANGED = 0x1D,
    HCI_QOS_VIOLATION = 0x1E,
    
    HCI_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT = 0x20,
    HCI_FLOW_SPECIFICATION_COMPLETE = 0x21,
    HCI_INQUIRY_RESULT_WITH_RSSI = 0x22,
    HCI_READ_REMOTE_EXTENDED_FEATURES_COMPLETE = 0x23,
    
    HCI_SYNCHRONOUS_CONNECTION_COMPLETE = 0x2C,
    HCI_SYNCHRONOUS_CONNECTION_CHANGED = 0x2D,
    HCI_SNIFF_SUBRATING = 0x2E,
    HCI_EXTENDED_INQUIRY_RESULT = 0x2F,
    HCI_ENCRYPTION_KEY_REFRESH_COMPLETE = 0x30,
    HCI_IO_CAPABILITY_REQUEST = 0x31,
    HCI_IO_CAPABILITY_RESOURCE = 0x32,
    HCI_USER_CONFIRMATION_REQUEST = 0x33,
    HCI_USER_PASSKEY_REQUEST = 0x34,
    HCI_REMOTE_OOB_DATA_REQUEST = 0x35,
    HCI_SIMPLE_PAIRING_COMPLETE = 0x36,
    
    HCI_LINK_SUPERVISION_TIMEOUT_CHANGED = 0x38,
    HCI_ENHANCED_FLUSH_COMPLETE = 0x39,
    HCI_USER_PASSKEY_NOTIFICATION = 0x3B,
    HCI_KEYPRESS_NOTIFICATION = 0x3C,
    HCI_REMOTE_HOST_SUPPORTED_FEATURES_NOTIFICATION = 0x3D,
    
    HCI_PHYSICAL_LINK_COMPLETE = 0x40,
    HCI_CHANNEL_SELECTED = 0x41,
    HCI_DISCONNECTION_PHYSICAL_LINK_COMPLETE = 0x42,
    HCI_PHYSICAL_LINK_LOSS_EARLY_WARNING = 0x43,
    HCI_PHYSICAL_LINK_RECOVERY = 0x44,
    HCI_LOGICAL_LINK_COMPLETE = 0x45,
    HCI_DISCONNECTION_LOGICAL_LINK_COMPLETE = 0x46,
    HCI_FLOW_SPEC_MODIFY_COMPLETE = 0x47,
    HCI_NUMBER_OF_COMPLETED_DATA_BLOCKS = 0x48,
    HCI_AMP_START_TEST = 0x49,
    HCI_AMP_TEST_END = 0x4A,
    HCI_AMP_RECEIVER_REPORT = 0x4B,
    HCI_SHORT_RANGE_MODE_CHANGE_COMPLETE = 0x4C,
    HCI_AMP_STATUS_CHANGE = 0x4D,
  
    /*
     * LE Meta Event
     */
    HCI_LE_META = 0x3E,
    
    HCI_TRIGGERED_CLOCK_CAPTURE = 0x4E,
    HCI_SYNCHRONIZATION_TRAIN_COMPLETE = 0x4F,
    
    HCI_SYNCHRONIZATION_TRAIN_RECEIVED = 0x50,
    HCI_CONNECTION_SLAVE_BROADCAST_RECEIVE = 0x51,
    HCI_CONNECTIONLESS_SLAVE_BROADCAST_TIMEOUT = 0x52,
    HCI_TRUNCATED_PAGE_COMPLETE = 0x53,
    HCI_SLAVE_PAGE_RESPONSE_TIMEOUT = 0x54,
    HCI_CONNECTIONLESS_SLAVE_BROADCAST_CHANNEL_MAP_CHANGE = 0x55,
    HCI_INQUIRY_RESPONSE_NOTIFICATION = 0x56,
    HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED = 0x57,
    HCI_SAM_STATUS_CHANGE = 0x58,
    
};
/*
 * Subevent Code
 */
enum {
    HCI_LE_CONNECTION_COMPLETE =  0x01,
    HCI_LE_ADVERTISING_REPORT = 0x02,
    HCI_LE_CONNECTION_UPDATE_COMPLETE = 0x03,
    HCI_LE_READ_REMOTE_FEATURES_COMPLETE = 0x04,
    HCI_LE_LONG_TERM_KEY_REQUEST = 0x05,
    HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST = 0x06,
    HCI_LE_DATA_LENGTH_CHANGE = 0x07,
    HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE = 0x08,
    HCI_LE_GENERATE_DHKEY_COMPLETE = 0x09,
    HCI_LE_ENHANCED_CONNECTION_COMPLETE = 0x0A,
    HCI_LE_DIRECTED_ADVERTISING_REPORT = 0x0B,
    HCI_LE_PHY_UPDATE_COMPLETE = 0x0C,
    HCI_LE_EXTENDED_ADVERTISING_REPORT = 0x0D,
    HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHMENT = 0x0E,
    HCI_LE_PERIODIC_ADVERTISING_REPORT = 0x0F,
    HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST = 0x10,
    HCI_LE_SCAN_TIMEOUT = 0x11,
    HCI_LE_ADVERTISING_SET_TERMINATED = 0x12,
    HCI_LE_SCAN_REQUEST_RECEIVED = 0x13,
    HCI_LE_CHANNEL_SELECTION_ALGORITHM = 0x14,
    HCI_LE_CONNECTIONLESS_IQ_REPORT = 0x15,
    HCI_LE_CONNECTION_IQ_REPORT = 0x16,
    HCI_LE_CTE_REQUEST_FAILED = 0x17,
    HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED = 0x18,
    HCI_LE_CIS_ESTABLISHED = 0x19,
    HCI_LE_CIS_REQUEST = 0x1A,
    HCI_LE_CREATE_BIG_COMPLETE = 0x1B,
    HCI_LE_TERMINATE_BIG_COMPLETE = 0x1C,
    HCI_LE_BIG_SYNC_ESTABLISHED = 0x1D,
    HCI_LE_BIG_SYNC_LOST = 0x1E,
    HCI_LE_REQUEST_PEER_SCA_COMPLETE = 0x1F,
    HCI_LE_PATH_LOSS_THRESHOLD = 0x20,
    HCI_LE_TRANSMIT_POWER_REPORTING = 0x21,
    HCI_LE_BIGINFO_ADVERTISING_REPORT = 0x022,
    
};
    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_HANDOVER_H */

/* *****************************************************************************
 End of File
 */
