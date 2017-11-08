/* Code to initialize and read push buttons on arbitrary pins on ports A to E and lines 0 to 4.
 * NOTE lines are restricted to 0 to 4 due to the allocation of External Interrupt Lines (EXTI)
 *
 *
 *  Created on: 08 Nov 2017
 *      Author: Munsanje Mweene
 */


#include "buttons.h"


GPIO_TypeDef *GPIOPorts[5] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE};
uint16_t GPIOPins[5] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4};
uint32_t GPIOClock[5] = {RCC_AHB1Periph_GPIOA, RCC_AHB1Periph_GPIOB, RCC_AHB1Periph_GPIOC, RCC_AHB1Periph_GPIOD, RCC_AHB1Periph_GPIOE};
uint32_t GPIOEXTI_Line[5] = {EXTI_Line0, EXTI_Line1, EXTI_Line2, EXTI_Line3, EXTI_Line4};
uint8_t GPIOEXTI_Port_Source[5] = {EXTI_PortSourceGPIOA, EXTI_PortSourceGPIOB, EXTI_PortSourceGPIOC, EXTI_PortSourceGPIOD, EXTI_PortSourceGPIOE};
uint8_t GPIOEXTI_Pin_Source[5] = {EXTI_PinSource0, EXTI_PinSource1, EXTI_PinSource2, EXTI_PinSource3, EXTI_PinSource4};
int8_t GPIOEXTI_IRQ[5] = {EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn};


/** Initialize push button at port specified
 * @param pin_name: string giving port name in form "A0", "B1", etc. NOTE letters must be capitalized!
 * @param button_port: Reference to GPIO_TypeDef* object. NOTE this value is initialized by this function.
 * 		Caller must simply declare GPIO_TypeDef* button_port; and pass it into this function as &button_port
 * @param button_pin: Reference to variable in which to store the pin number. As above, simply declare and pass
 * */
void PBInit(const char* pin_name, GPIO_TypeDef** button_port, uint32_t* button_pin) {
	uint8_t letter = (uint8_t) pin_name[0] - (uint8_t) 'A';
	uint8_t num = (uint8_t) pin_name[1] - (uint8_t) '0';


	uint32_t PB_CLK = GPIOClock[letter];
	uint16_t PB_PIN = GPIOPins[num];
	GPIO_TypeDef *PB_PORT = GPIOPorts[letter];
	uint16_t PB_EXTI_PORT_SOURCE = GPIOEXTI_Port_Source[letter];
	uint32_t PB_EXTI_LINE = GPIOEXTI_Line[num];
	uint8_t PB_EXTI_PIN_SOURCE = GPIOEXTI_Pin_Source[num];
	int8_t PB_EXTI_IRQn = GPIOEXTI_IRQ[num];

	*button_port = PB_PORT;
	*button_pin = PB_PIN;

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	/* Enable the BUTTON Clock */
	RCC_AHB1PeriphClockCmd(PB_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure Button pin as input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = PB_PIN;
	GPIO_Init(PB_PORT, &GPIO_InitStructure);

	/* Connect Button EXTI Line to Button GPIO Pin */
	SYSCFG_EXTILineConfig(PB_EXTI_PORT_SOURCE, PB_EXTI_PIN_SOURCE);

	/* Configure Button EXTI line */
	EXTI_InitStructure.EXTI_Line = PB_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set Button EXTI Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = PB_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

/*Return state of push button
 * @param button_port: Reference to GPIO_TypeDef object initialized by call to PBInit, or other method
 * @param button_pin: Reference to variable containing number of pin
 * @return integer specifying value of push button*/
uint32_t PBGetState(GPIO_TypeDef** button_port, uint32_t* button_pin) {
  return GPIO_ReadInputDataBit(*button_port, *button_pin);
}

/*External Interrupt handler for line 0*/
void EXTI0_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI2_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

void EXTI3_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

void EXTI4_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}
