#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "Arduino.h"
#include "global.h"

#define NORMAL_STATE SET
#define PRESSED_STATE RESET

extern int button_flag[5];

#define SET 1
#define RESET 0

void getKeyInput();
int isButtonPressed(int button_index);
int isButtonLongPressed(int button_index);

#endif /* INC_BUTTON_H_ */
