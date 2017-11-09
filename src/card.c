#include "FreeRTOS.h"
#include "task.h"

#include "codec.h"
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#include "ff.h"
// #include "sound.h"
#include "wav.h"


void playBuffer(uint16_t buff[]);
static uint16_t buffer[11025];
uint16_t bufferP[11025];
static uint16_t buffer_size = 11025;
uint8_t selected[8][8] = {
		{1,0,0,0,0,0,0,0},
		{0,1,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0},
		{0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0},
		{0,0,0,0,0,0,0,1},
};
uint8_t selectedInstr[8][8] = {
		{1,0,0,0,0,0,0,0},
		{0,2,0,0,0,0,0,0},
		{0,0,3,0,0,0,0,0},
		{0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,0,3,0,0},
		{0,0,0,0,0,0,2,0},
		{0,0,0,0,0,0,0,1}
};

char* files[3][8]= {{"mA.wav", "mCh.wav", "mCl.wav", "mDh.wav", "mDl.wav", "mFh.wav", "mFl.wav", "mG.wav"},
							 {"pA.wav", "pB.wav", "pC.wav", "pD.wav","pE.wav", "pF.wav", "pG.wav", "pG#.wav"},
						   {"xA.wav", "xB.wav", "xC.wav", "xD.wav", "xE.wav", "xG.wav", "xG#.wav"}};

uint8_t instrument=1;

void card_main(void* p) {


  uint8_t selectedInstrument=0;
	static FATFS FatFs;
	static FIL fp;
	FRESULT res = init_sd(&FatFs);  // initialize SD card
	const char* filename = "mA.wav";

	for(int z=0;z<11025;z++){
		bufferP[z]=0;
	}

	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			selectedInstrument= selectedInstr[i][j];
			if(selected[i][j]==1){
				res = open_file(&fp, files[selectedInstrument-1][j], FA_READ | FA_OPEN_ALWAYS);
				int m = 0; //
			res = read_wav_file(&fp, files[selectedInstrument-1][j], buffer, 11025);
			}
			for(int k=0;k<11025;k++){
				bufferP[k]=bufferP[k]+buffer[k];
			}

		}

		if(res == FR_OK) {
			playBuffer(bufferP);
		}


  }
	 // de-initialize SD card
	 	deinit_sd();
    for (;;) {
    }
    vTaskDelete(NULL);

}

void playBuffer(uint16_t buffer[]){
	uint16_t sampleCounter=0;
	uint16_t sample =0;

	for(int i=0;i<11025;i++){
		if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE))

						{
							SPI_I2S_SendData(CODEC_I2S, sample);

							//only update on every second sample to insure that L & R ch. have the same sample value
							if (sampleCounter & 0x00000001)
							{
								if (i < buffer_size){
									sample = buffer[i];
									i++;
								}else{
									i=0;
								}
							}
							sampleCounter++;
						}

	}


}
