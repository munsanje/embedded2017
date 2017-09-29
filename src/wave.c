/*
 * wave.c
 *
 *  Created on: Sep 29, 2017
 *      Author: Munsanje Mweene
 */

#include "stddef.h"
#include "wave.h"

// FatFs modules
#include "ff.h"


/*Read wave file header into struct
 * @param header the wave_file struct to be populated with header information
 * @param wave_path string giving path to file
 * @return the number of bytes read into header struct. Should be 44
 */
int read_header(wave_header* header, FILE* wave_path) {
	size_t bytes_read = fread(&header, 1, sizeof(wav_hdr), wave_file);
	return bytes_read;
}

FILE* read_wav(const char* wave_path) {
	FILE* file_obj = fopen(wave_path, "r");
	wave_file file;
	size_t bytes_read = read_header(&file.header, &file_obj);
	if(bytes_read  == 0) {
		return 0;
	}




}








