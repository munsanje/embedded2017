#include "queue.h"

#ifndef ZIKI_H
#define ZIKI_H

xQueueHandle Global_Queue_Handle;

/* Handles all tasks related to user-input; for instance
 * reading pots using ADC */
void uinput_main(void* p);

/* Handles LED display */
void output_main(void* p);

#endif /* ZIKI_H */
