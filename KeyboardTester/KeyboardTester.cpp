// KeyboardTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "KeyboardTester.h"

HANDLE hStdin;
DWORD fdwSaveOldMode;
DWORD saveMouseButtonState = 0;
bool mouseButtonJustReleased = false;

VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
VOID FocusEventProc(FOCUS_EVENT_RECORD);
BOOL CompareInputRecord(INPUT_RECORD, INPUT_RECORD);
WORD getLRKeyCode(WORD vkCode);

void printDWORDAsBinary(DWORD value) {
    printf("\n");
    for (int i = 31; i >= 16; --i) printf("%c", (value & (1 << i)) ? '1' : '0');
    printf("\n");
    for (int i = 15; i >= 0; --i) printf("%c", (value & (1 << i)) ? '1' : '0');
    printf("\n");
}

void print(void* pString) {
    wchar_t* str = static_cast<wchar_t*>(pString);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str, wcslen(str), NULL, NULL);
}


int main(int argc, char* argv[])
{

    DWORD cNumRead, fdwMode, i;
    INPUT_RECORD irInBuf[128];
    int counter = 0;
    int MAX_RECORDS = 100;
    InitializeVKKeys();

    bool include_mouse_events = false;

    // Start iterating from index 1, as argv[0] is the program name.
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-m") {
            include_mouse_events = true;
            break;  // Exit the loop once -m is found
        }
    }

    // Get the standard input handle.
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit((LPSTR)"GetStdHandle");

    // Save the current input mode, to be restored on exit.
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
        ErrorExit((LPSTR)"GetConsoleMode");

    // Copy the current mode to fdwMode
    fdwMode = fdwSaveOldMode;

    // Enable window and mouse input events
    fdwMode |= ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;

    // To change the state of Quick Edit Mode, you must set the ENABLE_EXTENDED_FLAGS flag
    fdwMode |= ENABLE_EXTENDED_FLAGS;

    // Disable Quick Edit Mode
    fdwMode &= ~(ENABLE_QUICK_EDIT_MODE);

    if (!SetConsoleMode(hStdin, fdwMode))
        ErrorExit((LPSTR)"SetConsoleMode");
    

    printf("\nShowing details of each INPUT_RECORD read in from the console input buffer.\n");
    printf("Any buffer input; keyboard, mouse, resize event, etc.\n");
    printf("%3u input record events will be processed", MAX_RECORDS);

    // read the first input record
    ReadConsoleInput(
        hStdin,      // input buffer handle
        irInBuf,     // buffer to read into
        1,           // size of read buffer
        &cNumRead);
    INPUT_RECORD current_input_record = irInBuf[0];
    INPUT_RECORD previous_input_record = INPUT_RECORD();

    // Loop to read and handle the next 100 input events.
    while (counter <= MAX_RECORDS)
    {
        // Wait for the events.
        if (!ReadConsoleInput(
            hStdin,      // input buffer handle
            irInBuf,     // buffer to read into
            128,         // size of read buffer
            &cNumRead)) // number of records read
            ErrorExit(LPSTR("ReadConsoleInput"));
        
        // Dispatch the events to the appropriate handler.
        for (i = 0; i < cNumRead; i++)
        {
            counter++;
            current_input_record = irInBuf[i];
            
            if ( !include_mouse_events && current_input_record.EventType == MOUSE_EVENT) {
                counter--;
            }else if (include_mouse_events && mouseButtonJustReleased 
                && current_input_record.EventType == MOUSE_EVENT
                && current_input_record.Event.MouseEvent.dwEventFlags == MOUSE_MOVED
                && previous_input_record.Event.MouseEvent.dwEventFlags == 0) {
                mouseButtonJustReleased = false;
            }
            else if (CompareInputRecord(current_input_record, previous_input_record)) {
                
                switch (current_input_record.EventType)
                {
                case KEY_EVENT: // keyboard input
                    printf("\n %3u/%3u remaining :", (MAX_RECORDS + 1 - counter), MAX_RECORDS);
                    printf(" repeat %s ", VK_KEYS[ getLRKeyCode(current_input_record.Event.KeyEvent.wVirtualKeyCode)]);
                    break;

                case MOUSE_EVENT: // mouse input
                    //printf("\n %3u/%3u remaining : repeat ", (MAX_RECORDS + 1 - counter), MAX_RECORDS);
                    printf(", %u ", (MAX_RECORDS + 1 - counter));
                    MouseEventProc(current_input_record.Event.MouseEvent);
                    break;
                default:
                    printf("same something?");
                    break;
                }
            }
            else {
                printf("\n %3u/%3u remaining : ", (MAX_RECORDS + 1 - counter), MAX_RECORDS);
                switch (current_input_record.EventType)
                {
                case KEY_EVENT: // keyboard input
                    KeyEventProc(current_input_record.Event.KeyEvent);
                    break;

                case MOUSE_EVENT: // mouse input
                    MouseEventProc(current_input_record.Event.MouseEvent);
                    break;

                case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
                    ResizeEventProc(current_input_record.Event.WindowBufferSizeEvent);
                    break;

                case FOCUS_EVENT:  // disregard focus events
                    FocusEventProc(current_input_record.Event.FocusEvent);
                    break;

                case MENU_EVENT:   // disregard menu events
                    break;

                default:
                    ErrorExit(LPSTR("Unknown event type"));
                    break;
                }
            }
            previous_input_record = current_input_record;
        }
    }

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    return 0;
}

static VOID ErrorExit(LPSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    ExitProcess(0);
}

static VOID KeyEventProc(KEY_EVENT_RECORD ker)
{

    //next_input_record
    // 
    //printf("Keyboard event: [ %s ], " ,  (ker.uChar.UnicodeChar >= 65 && ker.uChar.UnicodeChar <= 126 ) ? ""+ker.uChar.UnicodeChar : " " );
    
    //if (ker.uChar.UnicodeChar >= 32 && ker.uChar.UnicodeChar <= 126) {
    //    printf("Keyboard event: [ %lc ], ", ker.uChar.UnicodeChar);
    //}
    //else {
    //    if (ker.uChar.UnicodeChar == VK_LBUTTON) {
    //    }
    //}
    /*
     * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
     * Used only as parameters to GetAsyncKeyState() and GetKeyState().
     * No other API or message will distinguish left and right keys in this way.
     */


    WORD vkCode = getLRKeyCode(ker.wVirtualKeyCode);

    //SHORT keyState = GetAsyncKeyState(ker.wVirtualKeyCode);
    //if (keyState < 0) {
    //    printf("Right ");
    //}
    //else if (keyState & 0x8000) {
    //    printf("Left ");
    //}
    //WCHAR x = ker.uChar.UnicodeChar;
    printf("Key: [ %s ], ", VK_KEYS[vkCode] );

    if (ker.uChar.UnicodeChar)
    {
        printf(" uChar u%04X", ker.uChar.UnicodeChar);
        //auto wc = u'\u263A'; // Unicode for ☺ (a printable character)
        //printf("-- ☺ %lc --", wc);
        
        // 🙂 (U + 1F600)
        // 👍 (U + 1F44D)
        // 🚀 (U + 1F680)
        if (!iswcntrl(ker.uChar.UnicodeChar)) {
            printf(" '%lc'", ker.uChar.UnicodeChar);
        }
    }

    printf(" wRepeatCount %lu, ", ker.wRepeatCount);
    printf(" wVirtualKeyCode 0x%02X, ", ker.wVirtualKeyCode);
    printf(" wVirtualScanCode 0x%02X, ", ker.wVirtualScanCode);


    if (ker.bKeyDown) {
        printf("key pressed");
    }        
    else {
        printf("key released");
    }
}

static VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
    printf("Mouse at: (%d, %d) ", mer.dwMousePosition.X , mer.dwMousePosition.Y);

    switch (mer.dwEventFlags)
    {
    case 0:
        /// Mouse Button Released...
        if ((saveMouseButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
            && !(mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
            printf("Left button released");
            mouseButtonJustReleased = true;
        }
        else if ((saveMouseButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
            && !(mer.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)) {
            printf("2nd from Left button released");
            mouseButtonJustReleased = true;
        }
        else if ((saveMouseButtonState & FROM_LEFT_3RD_BUTTON_PRESSED)
            && !(mer.dwButtonState & FROM_LEFT_3RD_BUTTON_PRESSED)) {
            printf("3nd from Left button released");
            mouseButtonJustReleased = true;
        }
        else if ((saveMouseButtonState & FROM_LEFT_4TH_BUTTON_PRESSED)
            && !(mer.dwButtonState & FROM_LEFT_4TH_BUTTON_PRESSED)) {
            printf("4th from Left button released");
            mouseButtonJustReleased = true;
        }
        else if ((saveMouseButtonState & RIGHTMOST_BUTTON_PRESSED)
            && !(mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED)) {
            printf("Right button released");
            mouseButtonJustReleased = true;
        }

        /// Mouse Button Pressed...
        if ((mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
            && !(saveMouseButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
            printf("Left button pressed");
        }
        else if ((mer.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
            && !(saveMouseButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)) {
            printf("2nd from Left button pressed");
        }
        else if ((mer.dwButtonState & FROM_LEFT_3RD_BUTTON_PRESSED)
            && !(saveMouseButtonState & FROM_LEFT_3RD_BUTTON_PRESSED)) {
            printf("3nd from Left button pressed");
        }
        else if ((mer.dwButtonState & FROM_LEFT_4TH_BUTTON_PRESSED)
            && !(saveMouseButtonState & FROM_LEFT_4TH_BUTTON_PRESSED)) {
            printf("4th from Left button pressed");
        }
        else if ((mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
            && !(saveMouseButtonState & RIGHTMOST_BUTTON_PRESSED)) {
            printf("Right button pressed");
        }
        saveMouseButtonState = mer.dwButtonState;
        break;
    case DOUBLE_CLICK:
        /// Mouse Button Double Clicked...
        if ((mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
            && !(saveMouseButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
            printf("Left Button Double Clicked");
        }
        else if ((mer.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
            && !(saveMouseButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)) {
            printf("2nd from Left Button Double Clicked");
        }
        else if ((mer.dwButtonState & FROM_LEFT_3RD_BUTTON_PRESSED)
            && !(saveMouseButtonState & FROM_LEFT_3RD_BUTTON_PRESSED)) {
            printf("3nd from Left Button Double Clicked");
        }
        else if ((mer.dwButtonState & FROM_LEFT_4TH_BUTTON_PRESSED)
            && !(saveMouseButtonState & FROM_LEFT_4TH_BUTTON_PRESSED)) {
            printf("4th from Left Button Double Clicked");
        }
        else if ((mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
            && !(saveMouseButtonState & RIGHTMOST_BUTTON_PRESSED)) {
            printf("Right button Button Double Clicked");
        }
        saveMouseButtonState = mer.dwButtonState;
        //printf("double click");
        //mouseButtonJustReleased = true;
        break;
    case MOUSE_HWHEELED:
        printf("horizontal mouse wheel");
        break;
    case MOUSE_MOVED:
        //if (mer.dwButtonState != saveMouseButtonState) {
            printf("mouse moved");
        //}
        break;
    case MOUSE_WHEELED:
        printf("vertical mouse wheel");
        break;
    default:
        printf("unknown");
        break;
    }
}

static VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
    printf("Resize Event: %d columns(x) by %d rows(y).", wbsr.dwSize.X, wbsr.dwSize.Y);
}

static VOID FocusEventProc(FOCUS_EVENT_RECORD fer)
{
    printf("Focus Event: %s", fer.bSetFocus ? "Focussed" : "Unfocussed");
}
// useful, if same(ish) INPUT_RECORD is repeatedly being sent in...
static BOOL CompareInputRecord(INPUT_RECORD a, INPUT_RECORD b)
{
    //printf("\nCompareInputRecord\n");

    BOOL key_same = (
        a.EventType == KEY_EVENT && b.EventType == KEY_EVENT
        && a.Event.KeyEvent.bKeyDown && b.Event.KeyEvent.bKeyDown
        && (a.Event.KeyEvent.wVirtualKeyCode == b.Event.KeyEvent.wVirtualKeyCode)
        && (a.Event.KeyEvent.wVirtualScanCode == b.Event.KeyEvent.wVirtualScanCode)
        && (a.Event.KeyEvent.dwControlKeyState == b.Event.KeyEvent.dwControlKeyState)
        && (a.Event.KeyEvent.uChar.AsciiChar == b.Event.KeyEvent.uChar.AsciiChar)
        && (a.Event.KeyEvent.uChar.UnicodeChar == b.Event.KeyEvent.uChar.UnicodeChar)
    );

    BOOL mouse_same = (
        a.EventType == MOUSE_EVENT && b.EventType == MOUSE_EVENT
        && (a.Event.MouseEvent.dwEventFlags == b.Event.MouseEvent.dwEventFlags)
        && (a.Event.MouseEvent.dwEventFlags != 0 || a.Event.MouseEvent.dwButtonState == b.Event.MouseEvent.dwButtonState)
    );

    BOOL are_same = key_same || mouse_same;
    //printf(key_same ? "  *** KEY SAME ***\n" : "");
    //printf(mouse_same ? "  *** MOUSE SAME ***\n" : "");
    
    return are_same;
}

static WORD getLRKeyCode(WORD vkCode) {
    //ker.wVirtualKeyCode;
    WORD lrKeyCode = vkCode;
    //SHORT keyState = 0;
    SHORT keyLState = 0;
    SHORT keyRState = 0;
    switch (vkCode) {
    case VK_SHIFT:
        //keyState = GetAsyncKeyState(VK_SHIFT);
        keyLState = GetAsyncKeyState(VK_LSHIFT);
        keyRState = GetAsyncKeyState(VK_RSHIFT);

        if ( //keyState & 0x8000
            /* && */ keyLState & 0x8000
            //&& keyRState == 0
            ) {
            lrKeyCode = VK_LSHIFT;
        }
        else if (//keyState & 0x8000
            //&& keyLState == 0
            /* && */ keyRState & 0x8000
            ) {
            lrKeyCode = VK_RSHIFT;
        }
        break;
    case VK_CONTROL:
        //keyState = GetAsyncKeyState(VK_CONTROL);
        keyLState = GetAsyncKeyState(VK_LCONTROL);
        keyRState = GetAsyncKeyState(VK_RCONTROL);

        if (//keyState & 0x8000
            /* && */ keyLState & 0x8000
            //&& keyRState == 0
            ) {
            lrKeyCode = VK_LCONTROL;
        }
        else if (//keyState & 0x8000
            //&& keyLState == 0
            /* && */ keyRState & 0x8000
            ) {
            lrKeyCode = VK_RCONTROL;
        }
        break;
    case VK_MENU:
        //keyState = GetAsyncKeyState(VK_MENU);
        keyLState = GetAsyncKeyState(VK_LMENU);
        keyRState = GetAsyncKeyState(VK_RMENU);

        if (//keyState & 0x8000
            /* && */ keyLState & 0x8000
            // && keyRState == 0
            ) {
            lrKeyCode = VK_LMENU;
        }
        else if (//keyState & 0x8000
            //&& keyLState == 0
            /* && */ keyRState & 0x8000
            ) {
            lrKeyCode = VK_RMENU;
        }
        break;
    }
    return lrKeyCode;
}
