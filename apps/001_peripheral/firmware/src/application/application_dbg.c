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

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
static const char *_gapStatusTable[] = {
    "GAP_STATUS_SUCCESS",//, - Successful status.\0",
    "GAP_STATUS_UNKNOWN_HCI_COMMAND",//, - Unknown HCI Command.\0",
    "GAP_STATUS_UNKNOWN_CONNECTION_IDENTIFIER",//, - Unknown Connection Identifier.\0",
    "GAP_STATUS_HARDWARE_FAILURE",//, - Hardware Failure.\0",
    "GAP_STATUS_PAGE_TIMEOUT",//, - Page Timeout.\0",
    "GAP_STATUS_AUTHENTICATION_FAILURE",//, - Authentication Failure.\0",
    "GAP_STATUS_PIN_KEY_MISSING",//, - PIN or Key Missing.\0",
    "GAP_STATUS_MEMORY_CAPACITY_EXCEEDED",//, - Memory Capacity Exceeded.\0",
    "GAP_STATUS_CONNECTION_TIMEOUT",//, - Connection Timeout.\0",
    "GAP_STATUS_CONNECTION_LIMIT_EXCEEDED",//, - Connection Limit Exceeded.\0",
    "GAP_STATUS_SYNCHRONOUS_CONNECTION_LIMIT_EXCEEDED",//, - Synchronous Connection Limit To A Device Exceeded.\0",
    "GAP_STATUS_CONNECTION_ALREADY_EXISTS",//, - Connection Already Exists.\0",
    "GAP_STATUS_COMMAND_DISALLOWED",//, - Command Disallowed.\0",
    "GAP_STATUS_REJECTED_DUE_TO_LIMITED_RESOURCES",//, - Connection Rejected due to Limited Resources.\0",
    "GAP_STATUS_REJECTED_DUE_TO_SECURITY_REASONS",//, - Connection Rejected due To Security Reasons.\0",
    "GAP_STATUS_REJECTED_DUE_TO_UNACCEPTABLE_BDADDR",//, - Connection Rejected due to Unacceptable BD_ADDR.\0",
    "GAP_STATUS_CONNECTION_ACCEPT_TIMEOUT",//, - Connection Accept Timeout Exceeded.\0",
    "GAP_STATUS_UNSUPPORTED_FEATURE",//, - Unsupported Feature or Parameter Value.\0",
    "GAP_STATUS_INVALID_HCI_COMMAND_PARAMETERS",//, - Invalid HCI Command Parameters.\0",
    "GAP_STATUS_REMOTE_TML_CONN",//, - Remote User Terminated Connection.\0",
    "GAP_STATUS_REMOTE_TML_CONN_LOW_RESOURCE",//, - Remote Device Terminated Connection due to Low Resources.\0",
    "GAP_STATUS_REMOTE_TML_CONN_POWEROFF",//, - Remote Device Terminated Connection due to Power Off.\0",
    "GAP_STATUS_LOCAL_HOST_TERMINATE_CONNECTION",//, - Connection Terminated By Local Host.\0",
    "GAP_STATUS_REPEATED_ATTEMPTS",//, - Repeated Attempts.\0",
    "GAP_STATUS_PAIRING_NOT_ALLOWED",//, - Pairing Not Allowed.\0",
    "GAP_STATUS_UNKNOWN_LMP_PDU",//, - Unknown LMP PDU.\0",
    "GAP_STATUS_UNSUPPORTED_REMOTE_FEATURE",//, - Unsupported Remote Feature / Unsupported LMP Feature.\0",
    "GAP_STATUS_SCO_OFFSET_REJECT",//, - SCO Offset Rejected.\0",
    "GAP_STATUS_SCO_INTEVAL_REJECT",//, - SCO Interval Rejected.\0",
    "GAP_STATUS_SCO_AIR_MODE_REJECT",//, - SCO Air Mode Rejected.\0",
    "GAP_STATUS_INVALID_LMP_PARAMETERS",//, - Invalid LMP Parameters / Invalid LL Parameters.\0",
    "GAP_STATUS_UNSPECIFIED_ERROR",//, - Unspecified Error.\0",
    "GAP_STATUS_UNSUPPORTED_LMP_PARAMETERS",//, - Unsupported LMP Parameter Value / Unsupported LL Parameter Value.\0",
    "GAP_STATUS_ROLE_CHANGE_NOT_ALLOW",//, - Role Change Not Allowed.\0",
    "GAP_STATUS_LMP_RESPONSE_TIMEOUT",//, - LMP Response Timeout / LL Response Timeout.\0",
    "GAP_STATUS_LMP_ERROR_TRANSACTION_COLLISION",//, - LMP Error Transaction Collision / LL Procedure Collision.\0",
    "GAP_STATUS_LMP_PDU_NOT_ALLOW",//, - LMP PDU Not Allowed.\0",
    "GAP_STATUS_ENCRYPTION_MODE_NOT_ACCEPTABLE",//, - Encryption Mode Not Acceptable.\0",
    "GAP_STATUS_LINK_KEY_CANNOT_BE_CHANGED",//, - Link Key cannot be Changed.\0",
    "GAP_STATUS_REQUESTED_QOS_NOT_SUPPORT",//, - Requested QoS Not Supported.\0",
    "GAP_STATUS_INSTANT_PASSED",//, - Instant Passed.\0",
    "GAP_STATUS_PAIRING_WITH_UNIT_KEY_NOT_SUPPORT",//, - Pairing With Unit Key Not Supported.\0",
    "GAP_STATUS_DIFFERENT_TRANSACTION_COLLISION",//, - Different Transaction Collision.\0",
    "GAP_STATUS_QOS_UNACCEPTABLE_PARAMETER",//, - QoS Unacceptable Parameter.\0",
    "GAP_STATUS_QOS_REJECTED",//, - QoS Rejected.\0",
    "GAP_STATUS_CHANNEL_CLASSIFICATION_NOT_SUPPORT",//, - Channel Classification Not Supported.\0",
    "GAP_STATUS_INSUFFICIENT_SECURITY",//, - Insufficient Security.\0",
    "GAP_STATUS_PARAMETER_OUT_OF_MANDATORY_RANGE",//, - Parameter Out Of Mandatory Range.\0",
    "GAP_STATUS_ROLE_SWITCH_PENDING",//, - Role Switch Pending.\0",
    "GAP_STATUS_RESERVED_SLOT_VIOLATION",//, - Reserved Slot Violation.\0",
    "GAP_STATUS_ROLE_SWITCH_FAILED",//, - Role Switch Failed.\0",
    "GAP_STATUS_EXTENDED_INQUIRY_RESPONSE_TOO_LARGE",//, - Extended Inquiry Response Too Large.\0",
    "GAP_STATUS_SECURE_SIMPLE_PAIRING_NOT_SUPPORT",//, - Secure Simple Pairing Not Supported By Host.\0",
    "GAP_STATUS_HOST_BUSY_PAIRING",//, - Host Busy - Pairing.\0",
    "GAP_STATUS_CONNECTION_REJECT_NON_SUITABLE_CHANNEL",//, - Connection Rejected due to No Suitable Channel Found.\0",
    "GAP_STATUS_CONTROLLER_BUSY",//, - Controller Busy.\0",
    "GAP_STATUS_UNACCEPTABLE_CONNECTION_PARAMETERS",//, - Unacceptable Connection Parameters.\0",
    "GAP_STATUS_ADVERTISING_TIMEOUT",//, - Advertising Timeout.\0",
    "GAP_STATUS_CONNECTION_TERMINATE_MIC_FAILURE",//, - Connection Terminated due to MIC Failure.\0",
    "GAP_STATUS_CONNECTION_FAILED_TO_BE_ESTABLISHED",//, - Connection Failed to be Established.\0",
    "GAP_STATUS_MAC_CONNECTION_FAILED",//, - MAC Connection Failed.\0",
    "GAP_STATUS_COARSE_CLOCK_ADJUSTMENT_REJECTED",//, - Coarse Clock Adjustment Rejected but Will Try to Adjust Using Clock Dragging.\0",
    "GAP_STATUS_TYPE0_SUBMAP_NOT_DEFINED",//, - Type0 Submap Not Defined.\0",
    "GAP_STATUS_UNKNOWN_ADVERTISING_IDENTIFIER",//, - Unknown Advertising Identifier.\0",
    "GAP_STATUS_LIMIT_REACHED",//, - Limit Reached.\0",
    "GAP_STATUS_OPERATION_CANCELLED_BY_HOST"// - Operation Cancelled by Host.\0"
};



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

const char * APPLICATION_DBG_GetGapStatusString(uint8_t id) {
    return _gapStatusTable[id];
}

/* *****************************************************************************
 End of File
 */
