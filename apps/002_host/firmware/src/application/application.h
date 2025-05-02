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
/** \file application.h
    Application include file
*/

#ifndef _APPLICATION_H    /* Guard against multiple inclusion */
#define _APPLICATION_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "definitions.h"
#include "application_node.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
    enum {
        ID_HOST = 0x45A,
        ID_BROADCAST = 0x45A,
        ID_NODE_1 = 0x45B,
        ID_NODE_2 = 0x45C,
        ID_NODE_3 = 0x45D,
        ID_NODE_4 = 0x45E
    };
    #define NUMBER_OF_NODES 4 /**< Number of BLE Nodes in application */
    
    /**
     Application Error Codes
     */
    typedef enum {
        ERROR_SUCCESS,  /**< No Error */
        ERROR_CAN,    /**< CAN Error */
        ERROR_NOT_IMPLEMENTED,  /**< Feature Not Implemented */
        ERROR_INVALID_ID, /**< Invalid Command/Event ID */
        NUM_ERROR_CODES /**< Number of Error Codes */
    }APPLICATION_ERROR_CODE_T;
    
    


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /** 
    Data structure for command decoding
    Contains command id and corresponding command function
    */    
    struct FUNC_PAIR {
        uint16_t id;  /**< function id */
        void *func;   /**< function pointer to corresponding function  */
    };
    /**
    Data structure for command decoding
    see ::FUNC_PAIR
     */
    
    typedef struct FUNC_PAIR FUNC_PAIR_T;

    /**
    Data structure for handover demo
     */
    typedef struct {
        bool handover_enabled;  /**< true if handover is enabled, otherwise false */
    }APPLICATION_DATA_T;


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    /**
    Application data. (see ::APPLICATION_DATA_T) 
     */
    extern APPLICATION_DATA_T data;
    /**
    Node data. (see ::NODE_T)
     */
    extern NODE_T nodes[NUMBER_OF_NODES];
    /**
    Broadcast data (see ::NODE_T)
     */
    extern NODE_T broadcast;
    /**
    Application Initialization.
     */
    extern void APPLICATION_Initialize(void);
    /**
    Application Main Task.
     */
    extern void APPLICATION_Tasks(void);
    /**
    Returns Command/Event function according to given op_code/event_code
    @param id given op_code/event_code
    @param pFuncTable function table containing Command/Event function pointers
    @param size number of functions in given function table
    @return function pointer if id is valid, otherwise NULL is returned
     */
    extern void *APPLICATION_GetFunction(uint16_t id, const FUNC_PAIR_T *pFuncTable, size_t size);
    /**
    Error Handler of application
    @param errorCode  error code (see ::APPLICATION_ERROR_CODE_T)
     */
    extern void APPLICATION_ErrorHandler(APPLICATION_ERROR_CODE_T errorCode);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_H */

/* *****************************************************************************
 End of File
 */
