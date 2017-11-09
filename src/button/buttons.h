/*
 * buttons.h
 *
 *  Created on: 08 Nov 2017
 *      Author: Munsanje Mweene
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "stm32f4xx.h"

typedef struct BUTTON_TYPE {
  GPIO_TypeDef* button_port;
  uint16_t button_pin;
} button_type;

void PBInit(const char* pin_name, button_type* button);
uint32_t PBGetState(button_type* button);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);


#endif /* BUTTONS_H_ */
