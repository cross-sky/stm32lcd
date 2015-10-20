#ifndef  __STM32_UB_EEPROM_H
#define __STM32_UB_EEPROM_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"

void AT24CXX_WriteBuff(uint8_t dev, uint8_t reg, uint8_t *Buffer, uint16_t Length);

void AT24CXX_ReadBuffer(uint8_t dev, uint8_t reg, uint8_t *Buffer, uint16_t Length);

void AT24CxxTest(void);

#endif
