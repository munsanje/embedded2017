#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "task.h"

#include "ziki.h"

#include "stm32f4xx.h"

#include "buttons.h"

#define button BUTTON_USER

// stores the values of the pots as updated by the ADC via DMA
uint8_t Pot[2] = {0,0};

// configures ADC1 to read PC0, PC1 and update Pot array above
void setup_pots();
void delay_ms();

//Button Config

void input_main(void* p) {
    setup_pots();
    button_type pb_instrument, pb_tempo, pb_playback, pb_save;
    PBInit("E0", &pb_instrument);
    PBInit("E1", &pb_tempo);
    PBInit("E2", &pb_playback);
    PBInit("E4", &pb_save);

    uint8_t instrumentSel, tempo, playback, save, x, y = 0;
    uint16_t input_snd = 0;
    for (;;) {
        //pot input
        instrumentSel = (uint8_t) PBGetState(&pb_instrument);
        tempo = (uint8_t) PBGetState(&pb_tempo);
        playback = (uint8_t) PBGetState(&pb_playback);
        save = (uint8_t) PBGetState(&pb_save);
        x = Pot[0] >> 3; // focus on 3 MSb's of 6-bit ADC output
        y = Pot[1] >> 3;

        input_snd = (instrumentSel<<9)+(tempo<<8)+(playback<<7)+(save<<6)+(x << 3)+y;
        xQueueSend(Q_HANDLE_INPUT_OUTPUT, &input_snd, 1);
    }

    vTaskDelete(NULL);
}

#define ADC1_CHANNEL_PC0 10
#define ADC1_CHANNEL_PC1 11

void setup_pots() {
    ADC_InitTypeDef       ADC_InitStruct;
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    DMA_InitTypeDef       DMA_InitStruct;
    GPIO_InitTypeDef      GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // configure PCO, PC1
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // set DMA2 to two values from read from ADC1
    DMA_InitStruct.DMA_Channel = DMA_Channel_0;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)((uintptr_t)&ADC1->DR);
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)((uintptr_t)&Pot);
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_BufferSize = 2;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStruct);
    DMA_Cmd(DMA2_Stream0, ENABLE);

    // set ADC1 to read PC0, PC1 and send to DMA2
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);

    ADC_DeInit();
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_6b;
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = 0;
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfConversion = 2;
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_RegularChannelConfig(ADC1, ADC1_CHANNEL_PC0, 1, ADC_SampleTime_144Cycles); // PC0
    ADC_RegularChannelConfig(ADC1, ADC1_CHANNEL_PC1, 2, ADC_SampleTime_144Cycles); // PC1

    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_SoftwareStartConv(ADC1);
}

void delay_ms(uint32_t milli)
{
	uint32_t delay = milli * 17612;              // approximate loops per ms at 168 MHz, Debug config
	for(; delay != 0; delay--);
}
