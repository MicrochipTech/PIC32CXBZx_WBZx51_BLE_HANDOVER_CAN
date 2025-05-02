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

#include "application_tempsensor.h"
#include "definitions.h"

#include <stdio.h>

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#define ADC_VREF (3.3f)
#define ADC_MAX_COUNT (4095)    // 12bit resolution
#define V_0C    (0.5f)          // Sensor Output Voltage at 0°C: 500mV
#define T_C     (0.01f)         // Temperature Coeffizeient; 10.0mV/°C

#if true
#define APPLICATION_TEMPSENSOR_DEBUG(__PRINT__)
#else
#define APPLICATION_TEMPSENSOR_DEBUG(__PRINT__)  do { \
                                              SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,"APPLICATION_TEMPSENSOR[%d] ", __LINE__); \
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

void APPLICATION_TEMPSENSOR_Initialize(void) {
    APPLICATION_TEMPSENSOR_DEBUG("APPLICATION_TEMPSENSOR_Initialize");
    // TODO: ADD IMPLEMENTATION
}

float APPLICATION_TEMPSENSOR_Get(void) {
    float temperature = 0.0f;
    ADCHS_GlobalEdgeConversionStart();
    while(!ADCHS_ChannelResultIsReady(ADCHS_CH2)) {};
    uint16_t result = ADCHS_ChannelResultGet(ADCHS_CH2);
    float voltage = (float)result * ADC_VREF / ADC_MAX_COUNT;
    temperature = (voltage - V_0C) / T_C;


    return temperature;
}


/* *****************************************************************************
 End of File
 */
