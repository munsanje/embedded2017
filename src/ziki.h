#ifndef ZIKI_H
#define ZIKI_H

#include "queue.h"

xQueueHandle Global_Queue_Handle = 0;

/* Handles all tasks related to user-input; for instance
 * reading pots using ADC */
void uinput_main(void* p);

/* Handles LED display */
void output_main(void* p);

#endif /* ZIKI_H */
