/*
 * buttons.h
 *
 *  Created on: 08 Nov 2017
 *      Author: Munsanje Mweene
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "stm32f4_discovery.h"
#include "stm32f4xx.h"

void PBInit(const char* pin_name, GPIO_TypeDef** button_port, uint32_t* button_pin);
uint32_t PBGetState(GPIO_TypeDef** button_port, uint32_t* button_pin);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);


#endif /* BUTTONS_H_ */

