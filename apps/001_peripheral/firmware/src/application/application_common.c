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

#include "application_common.h"
#include <stdio.h>
#include <ctype.h>


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



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

uint16_t APPLICATION_COMMON_UintRangeCheck(uint16_t value, uint16_t rangeMin, uint16_t rangeMax)
{
    if ((value > rangeMax) || (value < rangeMin))
        return APPLICATION_RES_INVALID_PARA;
    else
        return APPLICATION_RES_SUCCESS;
}
    
uint16_t APPLICATION_COMMON_AsciiToDec(uint8_t asciiCode, uint8_t *value, uint8_t rangeMin, uint8_t rangeMax)
{
    if ((asciiCode >= '0') && (asciiCode <= '9'))
        *value = asciiCode - '0';
    else
        return APPLICATION_RES_INVALID_PARA;

    return APPLICATION_COMMON_UintRangeCheck(*value, rangeMin, rangeMax);
}

uint16_t APPLICATION_COMMON_U8AsciiToHex(uint8_t *p_asciCode, uint8_t *value)
{
    unsigned int hexValue;

    if (p_asciCode == NULL || !isxdigit(p_asciCode[0]) || !isxdigit(p_asciCode[1]))
        return APPLICATION_RES_INVALID_PARA;

    sscanf((const char *)&p_asciCode[0], "%02x", &hexValue);
    *value = (uint8_t)hexValue;
    return APPLICATION_RES_SUCCESS;
}

uint16_t APPLICATION_COMMON_U16AsciiToHex(uint8_t *p_asciCode, uint16_t *value)
{
    unsigned int hexValue;

    if (p_asciCode == NULL || !isxdigit(p_asciCode[0]) || !isxdigit(p_asciCode[1]) || !isxdigit(p_asciCode[2]) || !isxdigit(p_asciCode[3]))
        return APPLICATION_RES_INVALID_PARA;

    sscanf((const char *)&p_asciCode[0], "%04x", &hexValue);
    *value = (uint16_t)hexValue;
    return APPLICATION_RES_SUCCESS;
}

uint16_t APPLICATION_COMMON_U32AsciiToHex(uint8_t *p_asciCode, uint32_t *value)
{
    unsigned long hexValue;

    if (p_asciCode == NULL)
        return APPLICATION_RES_INVALID_PARA;

    for(uint8_t i=0; i<8; i++)
    {
        if(!isxdigit(p_asciCode[i])) return APPLICATION_RES_INVALID_PARA;
    }

    sscanf((const char *)&p_asciCode[0], "%08lx", &hexValue);
    *value = (uint32_t)hexValue;
    return APPLICATION_RES_SUCCESS;
}

uint16_t APPLICATION_COMMON_StringToHex(uint8_t *p_asciCode, uint8_t *p_hex, uint8_t hexLen)//ToDo: need to check the fail case in the code of the caller
{
    uint8_t i;

    for(i = 0; i < hexLen; i++)
    {
        if (APPLICATION_COMMON_U8AsciiToHex(&p_asciCode[2*i], (p_hex+i)) != APPLICATION_RES_SUCCESS)
            return APPLICATION_RES_FAIL;
    }

    return APPLICATION_RES_SUCCESS;
}

void APPLICATION_COMMON_Reset(void) {
    // A software Reset is performed as follows:
    // 1. Write the system unlock sequence
    CFG_REGS->CFG_SYSKEY = 0x00000000U;
    CFG_REGS->CFG_SYSKEY = 0xAA996655U;
    CFG_REGS->CFG_SYSKEY = 0x556699AAU;    
    // 2. Set the SWRST bit (RSWRST[0]) = 1.
    RCON_REGS->RCON_RSWRST |= RCON_RSWRST_SWRST_SWRST;
    // 3. Read the RSWRST register;
    uint8_t rswrst = RCON_REGS->RCON_RSWRST;
    
    (void)rswrst;   // suppress warning: unused variable 'rswrst' [-Wunused-variable]
    
    while ( true ); 
}


/* *****************************************************************************
 End of File
 */
