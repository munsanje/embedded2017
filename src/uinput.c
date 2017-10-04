#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"


const uint16_t LEDS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
const uint16_t LED[4] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

void setup_adc() {
    // enable peripherals
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // ADC interface clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN, ENABLE); // GPIO interface clock for ADC

    // setup GPIO pins for ADC
	GPIO_InitTypeDef GPIO_InitStructre;
	GPIO_StructInit(&GPIO_InitStructre);
	GPIO_InitStructre.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStructre.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructre.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructre);

    // configure ADC
	ADC_DeInit();
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_6b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = 2;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1, &ADC_InitStructure);

    // select channels for conversion
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_144Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_144Cycles);
    
    // enable conversion
    ADC_ContinuousModeCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    /*ADC_Cmd(ADC1, ENABLE);*/
}

void setup_dma() {
    RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct); // default values
    DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToMemory;
}



uint16_t readADC1() {
    ADC_SoftwareStartConv(ADC1);//Start the conversion
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
    return ADC_GetConversionValue(ADC1); //Return the converted data
}

void uinput_main(void* p) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOD, &gpio);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    setup_adc();

    uint16_t val = 0;

    uint8_t x,y = 0;

    for (;;) {
        val = readADC1();
        x = val/16;
        y = 3 - x;

        GPIOB->ODR = (x << 6) | (y << 4);
        GPIO_ResetBits(GPIOD, LEDS);
        GPIO_SetBits(GPIOD, LED[x]);
    }
    vTaskDelete(NULL);
}
