#ifndef CKEYBOARD
#define CKEYBOARD

#include <Windows.h>
#include "Maths.h"

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void handleKeyInputs();

extern red::Vector3 movement_direction;
extern bool endKeyBoardHandling;
extern bool hasFinishedResizing;

#endif // CKEYBOARD