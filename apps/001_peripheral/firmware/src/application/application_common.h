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
/** \file application_common.h
  COMMON defines file
 */


#ifndef _APPLICATION_COMMON_H    /* Guard against multiple inclusion */
#define _APPLICATION_COMMON_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "application.h"
#include "mba_error_defs.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
#define APPLICATION_RES_SUCCESS                         MBA_RES_SUCCESS                               /**< Execution successfully. */
#define APPLICATION_RES_FAIL                            MBA_RES_FAIL                                  /**< Execution fail. */
#define APPLICATION_RES_OOM                             MBA_RES_OOM                                   /**< Out of memory. */
#define APPLICATION_RES_INVALID_PARA                    MBA_RES_INVALID_PARA                          /**< Invalid parameters. */
#define APPLICATION_RES_NO_RESOURCE                     MBA_RES_NO_RESOURCE                           /**< No resource. */
#define APPLICATION_RES_BAD_STATE                       MBA_RES_BAD_STATE                             /**< Bad State. */
#define APPLICATION_RES_PENDING_DUE_TO_SECURITY         MBA_RES_PENDING_DUE_TO_SECURITY               /**< Pending the request due to security process. */
#define APPLICATION_RES_BUSY                            MBA_RES_BUSY                                  /**< Execution fail due to system is busy. */
#define APPLICATION_RES_COMPLETE                        0x1000                                       /**< Some procedure is complete to distinguish execution successfully */


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************



    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    /**
    Check if variable value in in range [rangeMin:rangeMax]
    @param value  value to check
    @param rangeMin minimum value
    @param rangeMax maximmum value
    @return status, APPLICATION_RES_SUCCESS if in range, APPLICATION_RES_INVALID_PARA otherwise
     */
    extern uint16_t APPLICATION_COMMON_UintRangeCheck(uint16_t value, uint16_t rangeMin, uint16_t rangeMax);
    /**
    Convert ASCII variable('0'-'9') to decimal number and check range
    @param asciiCode Ascii variable('0'-'9')
    @param value pointer to store decimal value
    @param rangeMin minimum value
    @param rangeMax maximmum value
    @return status, APPLICATION_RES_SUCCESS if in range, APPLICATION_RES_INVALID_PARA otherwise
     */
    extern uint16_t APPLICATION_COMMON_AsciiToDec(uint8_t asciiCode, uint8_t *value, uint8_t rangeMin, uint8_t rangeMax);
    /** 
    Convert 8-bit ASCII hex number (2 digits) to Hexadecimal number
    @param p_asciCode 8-bit ASCII hex number (2 digits)
    @param value pointer to store hex value
    @return status, APPLICATION_RES_SUCCESS if successful, APPLICATION_RES_INVALID_PARA otherwise
     */
    extern uint16_t APPLICATION_COMMON_U8AsciiToHex(uint8_t *p_asciCode, uint8_t *value);
    /**
    Convert 16-bit ASCII hex number (4 digits) to Hexadecimal number
    @param p_asciCode 16-bit ASCII hex number (4 digits)
    @param value pointer to store hex value
    @return status, APPLICATION_RES_SUCCESS if successful, APPLICATION_RES_INVALID_PARA otherwise
     */
    extern uint16_t APPLICATION_COMMON_U16AsciiToHex(uint8_t *p_asciCode, uint16_t *value);
    /**
    Convert 32-bit ASCII hex number (8 digits) to Hexadecimal number
    @param p_asciCode 32-bit ASCII hex number (8 digits)
    @param value pointer to store hex value
    @return status, APPLICATION_RES_SUCCESS if successful, APPLICATION_RES_INVALID_PARA otherwise
     */
    extern uint16_t APPLICATION_COMMON_U32AsciiToHex(uint8_t *p_asciCode, uint32_t *value);
    /**
    Convert String to Hexadecimal number
    @param p_asciCode pointer to hex string start
    @param p_hex pointer to start address for hex number array
    @param hexLen number of hex values to convert
    @return status, APPLICATION_RES_SUCCESS if successful, APPLICATION_RES_INVALID_PARA otherwise
     */
    extern uint16_t APPLICATION_COMMON_StringToHex(uint8_t *p_asciCode, uint8_t *p_hex, uint8_t hexLen);

    /** 
    Reset Device
    */
    extern void APPLICATION_COMMON_Reset(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_COMMON_H */

/* *****************************************************************************
 End of File
 */
