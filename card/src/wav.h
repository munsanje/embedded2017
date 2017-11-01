/*
 * wav.h
 *
 *  Created on: 31 Oct 2017
 *  @author: Munsanje Mweene
 *
 *  Set of functions for reading and writing wave files to SD card.
 *
 */

#ifndef WAV_H_
#define WAV_H_

// standard modules
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

// local modules
#include "ff.h"

// structs
typedef struct WAVE_HEADER{
	uint32_t ChunkID;  // RIFF header
	uint32_t ChunkSize;  // RIFF chunk size
	uint32_t Format;  // wave header
	uint32_t Subchunk1ID;  // fmt header
	uint32_t SubChunkSize; // Size of fmt chunk
	uint16_t AudioFormat; // Audio format, 1 = PCM, 6 = mulaw, 7 = alaw, 257 = IBM mu-law, 258 = IBM a-law, 259 = ADPC
	uint16_t NumChannels;  // number of channels, 1 = Mono, 2 = Stereo
	uint32_t SampleRate; // sampling frequency in Hz
	uint32_t ByteRate;  // bytes per second
	uint16_t BlockAlign;  // 2 = 16-bit mono, 4 = 16-bit stereo
	uint16_t BitsPerSample;  // bits per sample
	char Subchunk2ID[4];  // "data" string -- contains the letters "data"
	uint32_t Subchunk2Size;  // Length of data/number of samples
} wave_header;




// function prototypes
FRESULT init_sd(FATFS *FatFs);
FRESULT deinit_sd();
FRESULT read_wav_header(FIL *fp, const TCHAR* filename, wave_header* header);
FRESULT read_wav_data(FIL *fp, const TCHAR* filename, wave_header* header, uint16_t* buffer, uint32_t buffer_size);
FRESULT read_wav_file(FIL *fp, const TCHAR* filename, uint16_t* buffer, uint32_t buffer_size);
FRESULT write_wav_file(FIL *fp, const TCHAR* filename, wave_header* header, uint16_t* wav_data);
uint32_t get_wav_size(FIL *fp, const TCHAR* filename);
/** Code to add:
 *  1. code to traverse sd card according to instrument hierarchy
 *  2. code to stream from wave file
 *
 */

#endif /* WAV_H_ */
