#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#include "ff.h"
#include "sound.h"
#include "wav.h"

static FATFS FatFs;
static FIL fp;
void card_main(void* p) {
	FRESULT res = init_sd(&FatFs);  // initialize SD card
	const char* filename = "mA.wav";
	uint32_t buffer_size = get_wav_size(&fp, filename);
	uint16_t buffer[buffer_size];
    res = read_wav_file(&fp, filename, buffer, buffer_size);  // load in PCM data
	if(res == FR_OK) {
		playBuffer(buffer, buffer_size);
	}
	deinit_sd();  // de-initialize SD card

    for (;;) {
    }
    vTaskDelete(NULL);
}
