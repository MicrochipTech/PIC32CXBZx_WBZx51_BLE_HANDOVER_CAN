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
/** \file application_node.h
    Node include file
*/

#ifndef _APPLICATION_NODE_H    /* Guard against multiple inclusion */
#define _APPLICATION_NODE_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "definitions.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
#define NODE_NAME_SIZE  32  /**< Size of the node name in bytes */


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************
    
    /**
    Node status structure
     */
    struct NODE_STATUS {
        union {
            struct {
                uint32_t    reset_complete : 1,   /**< reset complete flag */
                            connected : 1,        /**< connected/unconnected flag */
                            handover_in_progress : 1, /**< handover in progress flag */
                            role : 1;   /**< role flag 0=ACTIVE,1=PASSIVE */
                
            };
            uint32_t v; /**< status value*/
        };
    };
    /**
    Node status structure (see ::NODE_STATUS)
     */
    typedef struct NODE_STATUS NODE_STATUS_T;
    
    /**
    Node related data structure
     */
    struct NODE {
        uint16_t can_address; /**< CAN address */
        char name[NODE_NAME_SIZE];  /**< Node name */
        NODE_STATUS_T status; /**< Node status */
        uint16_t conn_handle; /**< 16-bit BLE connection handle */
        int8_t rssi;  /**< current BLE RSSI value */
    };
    /**
    Node related data structure (see ::NODE) 
     */
    typedef struct NODE NODE_T;

    /*
     * HCI related
     */


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    /**
    Node Initialization
    @param p_node pointer to node data structure (see ::NODE)
     */
    extern void APPLICATION_NODE_Initialize(NODE_T *p_node);
    /**
    Node Main Task
    @param p_node pointer to node data structure (see ::NODE)
     */
    extern void APPLICATION_NODE_Tasks(NODE_T *p_node);
    /**
    Node Command/Event Processing
    @param p_node pointer to node data structure (see ::NODE)
    @param p_data pointer to command/event data
     */
    extern void APPLICATION_NODE_Process(NODE_T *p_node, uint8_t *p_data);
    
    /**
    Trigger Node Disconnection 
    @param p_node pointer to node data structure (see ::NODE)
    @param reason disconnection reason
     */
    extern void APPLICATION_NODE_HciDisconnect(NODE_T *p_node, uint8_t reason);
    /**
    Trigger Node Reset
    @param p_node pointer to node data structure (see ::NODE)
     */
    extern void APPLICATION_NODE_HciReset(NODE_T *p_node);
    /**
    Trigger BLE Handover
    @param p_node pointer to node data structure (see ::NODE)
    @param address can address of active node
     */
    extern void APPLICATION_NODE_HciHandover3(NODE_T *p_node, uint16_t address);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_NODE_H */

/* *****************************************************************************
 End of File
 */
