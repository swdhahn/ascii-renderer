#include "Keyboard.h"

red::Vector3 movement_direction;
bool endKeyBoardHandling = true;
bool hasFinishedResizing = false;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
        switch (wParam) {
        case WM_KEYDOWN:
			if (p->vkCode == 0x57) { // W
            	movement_direction.z = -1;
            } else if (p->vkCode == 0x53) { // S
				movement_direction.z = 1;
			} else if(p->vkCode == 0x41) { // A
				movement_direction.x = -1;
			} else if(p->vkCode == 0x44) { // D
				movement_direction.x = 1;
			}
			break;
        case WM_SYSKEYDOWN:
			break;
        case WM_KEYUP:
			if (p->vkCode == 0x57) { // W
            	movement_direction.z = 0;
            } else if (p->vkCode == 0x53) { // S
				movement_direction.z = 0;
			} else if(p->vkCode == 0x41) { // A
				movement_direction.x = 0;
			} else if(p->vkCode == 0x44) { // D
				movement_direction.x = 0;
			}
            break;
        case WM_SYSKEYUP:
		break;
        }
    } else if(nCode == WM_PAINT) {
        hasFinishedResizing = true;
    }
    return(CallNextHookEx(NULL, nCode, wParam, lParam));
}

void handleKeyInputs() {
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    endKeyBoardHandling = false;
    MSG msg;
    
    while (!endKeyBoardHandling && !GetMessage(&msg, NULL, NULL, NULL)) {    //this while loop keeps the hook
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);
}