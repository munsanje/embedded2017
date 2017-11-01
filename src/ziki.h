#ifndef ZIKI_H
#define ZIKI_H

#include "queue.h"

/* Handles all tasks related to user-input; for instance
 * reading pots using ADC */
void uinput_main(void* p);

/* Handles LED display */
void output_main(void* p);

#endif /* ZIKI_H */

#ifndef GLOBALS_H
#define GLOBALS_H

#ifndef GLOBALS_DECLS
	#define _DECL extern
#else
	#define _DECL
#endif /* GLOBALS_DECLSmake*/

_DECL xQueueHandle Global_Queue_Handle;

#endif /* GLOBALS_H */
