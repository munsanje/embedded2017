/*
 * wav.c
 *
 *  Created on: 31 Oct 2017
 *      Author: sanje
 */

// local modules
#include "wav.h"

#define LED_ORANGE LED3
#define LED_GREEN LED4
#define LED_RED LED5
#define LED_BLUE LED6

// private variables

static uint8_t mount_state = 0;  // 1 means successfully mounted card, 0 means unsuccessful

FRESULT init_sd(FATFS *FatFs) {
	FRESULT res = FR_OK;
	if(mount_state == 0) {
		FRESULT res = f_mount(FatFs, "", 1);
		mount_state = res == FR_OK ? 1 : 0;
	}
	return res;
}

FRESULT deinit_sd() {
	FRESULT res = FR_OK;
	if(mount_state == 1) {
		res = f_mount(0, "", 1);
		mount_state = res == FR_OK ? 0 : 1;
	}
	return res;
}

/** Read the header of a wav file into memory
 * @see wave_header
 * @param fp FIL object
 * @param filename name of file on SD card
 * @param header pointer to struct in which to store header information
 * @return res result of operation*/
FRESULT read_wav_header(FIL *fp, const TCHAR* filename, wave_header* header) {
	FRESULT res = f_open(fp, filename, FA_READ);
	if(res == FR_OK) {
		uint16_t bytes_read = 0;
		res = f_read(fp, header, 44, &bytes_read);
	}
	f_close(fp);
	return res;
}

/** Read `buffer_size` words (16 bits) of data portion of wav file into memory
 * @param fp FIL object
 * @param filename name of file on SD card
 * @param header pointer to struct in which to store header information
 * @param buffer pointer to array in which to store data
 * @param buffer_size integer specifying number of words (16 bits) to read into `buffer`
 * @return res result of operation
 * */

FRESULT read_wav_data(FIL *fp, const TCHAR* filename, wave_header* header, uint16_t* buffer, uint32_t buffer_size) {
	FRESULT res = f_open(fp, filename, FA_READ);
	if(res == FR_OK) {
		uint16_t bytes_read = 44;  // file offset
		f_lseek(fp, bytes_read);  // seek to file offset of 44 bytes to beginning of data portion
		res = f_read(fp, buffer, buffer_size * (header->BitsPerSample/8), &bytes_read);
	}
	f_close(fp);
	return res;
}

/** Write sound data to wave file, given header and PCM data
 * @param fp FIL object
 * @param filename name of file on SD card
 * @param header pointer to struct in which to store header information
 * @param wav_data pointer to array containing PCM data
 * @return res result of operation
 * */
FRESULT write_wav_file(FIL *fp, const TCHAR* filename, wave_header* header, uint16_t* wav_data) {
	FRESULT res = f_open(fp, filename, FA_WRITE | FA_CREATE_ALWAYS);
	if(res == FR_OK) {
		uint16_t bytes_written = 0;
		f_write(fp, header, 44, &bytes_written);
		res = f_write(fp, wav_data, header->Subchunk2Size, &bytes_written);
	}
	f_close(fp);
	return res;
}


/** Get size of data portion of wave file
 * @param filename name of wave file on sd card
 * @return size of data portion of file
 * */
uint32_t get_wav_size(FIL *fp, const TCHAR* filename) {
	uint32_t size = 0;
	wave_header header;
	FRESULT res = read_wav_header(fp, filename, &header);
	if(res == FR_OK)
		size = header.Subchunk2Size / (header.BitsPerSample / 8);

	return size;  // return 0 if failed to read header of wav file
}

/** Mount sd card and read wave file specified
 * @param filename name of file on sd card*/
FRESULT read_wav_file(FIL *fp, const TCHAR* filename, uint16_t* buffer, uint32_t buffer_size) {
	STM_EVAL_LEDOn(LED_BLUE);
	// get header
	wave_header header;
	read_wav_header(fp, filename, &header);
	// get PCM
	uint16_t it;
	for(it = 0; it < buffer_size; it++) buffer[it] = 0; // zero out buffer
	FRESULT res = read_wav_data(fp, filename, &header, buffer, buffer_size);
	// unmount
	if(res == FR_OK)
		STM_EVAL_LEDOff(LED_BLUE);

	return res;


}




