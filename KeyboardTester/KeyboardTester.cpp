
#include "pch.h"
#include <windows.h>
#include <iostream>
// #include <conio.h> // this is old school MS-DOS!
#include <exception>
#include <string>
#include <codecvt>
#include <vector>
#include "KeyboardTester.h"

HANDLE hStdIn;
HANDLE hStdOut;

DWORD fdwSaveOldMode;
DWORD fdwMode;

DWORD saveMouseButtonState = 0;
bool mouseButtonJustReleased = false;

VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
VOID FocusEventProc(FOCUS_EVENT_RECORD);
VOID MenuEventProc(MENU_EVENT_RECORD);

VOID ErrorExit(LPSTR);

BOOL CompareInputRecord(INPUT_RECORD, INPUT_RECORD);
WORD getLRKeyCode(KEY_EVENT_RECORD);

std::string GetKeyName(BYTE);

bool isFocussed = true;

int main(int argc, char* argv[])
{

    auto x = "☺  🙂 🙂 👍 🚀";
    printf("0123456789\n%s\n", x);
    
    // printf("0123456789\n ☺  🙂 🙂 👍 🚀\n");

    // UTF-8 style: Prints 🙂
    // printf("\n%s", "\xf0\x9f\x98\x80");
    // // UTF-16 style: Also Prints 🙂
    // printf(" %s\n", "\U0001F600"); 


    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];
    int eventCounter = 0;
    int repeatCounter = 0;
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
    hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdIn == INVALID_HANDLE_VALUE)
        ErrorExit((LPSTR)"GetStdHandle");

    // Get the standard output handle.
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        ErrorExit((LPSTR)"GetStdHandle");

    // Save the current input mode, to be restored on exit.
    if (!GetConsoleMode(hStdIn, &fdwSaveOldMode))
        ErrorExit((LPSTR)"GetConsoleMode");

    // Copy the current mode to fdwMode
    fdwMode = fdwSaveOldMode;

    // Enable window and mouse input events
    fdwMode |= ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;

    // Ctrl-C something blahh?
    fdwMode |= ENABLE_PROCESSED_INPUT;

    // In order to change the state of Quick Edit Mode, 
    // you must set the ENABLE_EXTENDED_FLAGS flag
    fdwMode |= ENABLE_EXTENDED_FLAGS;

    // Disable Quick Edit Mode
    fdwMode &= ~(ENABLE_QUICK_EDIT_MODE);

    if (!SetConsoleMode(hStdIn, fdwMode))
        ErrorExit((LPSTR)"SetConsoleMode");

    InitializeVKKeys();

    SHORT prevKeyStates[256] = {0};

    printf("Monitoring keyboard state changes. Press 'Ctrl-C' to exit.\n");

    INPUT_RECORD previous_input_record = INPUT_RECORD();
    INPUT_RECORD current_input_record = INPUT_RECORD();


    try {
        while (true) 
        {
            if(previous_input_record.EventType == KEY_EVENT
              && previous_input_record.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
            {
                break;
            }

            // Method 1
            // continually monitor the state of each the 256 virtual keys map
            // and check for any changes.
            for (int i = 0; i < 256; ++i) {
                SHORT keyState = GetAsyncKeyState(i);
                if (keyState != prevKeyStates[i]) {
                    bool wasPressed = prevKeyStates[i] & 0x8000;
                    bool isPressed = keyState & 0x8000;
                    if( isFocussed && ! (  i == VK_CONTROL || i == VK_SHIFT || i == VK_MENU
                            || (i == VK_RETURN && eventCounter == 0)
                            || (!include_mouse_events && ( i == VK_LBUTTON || i == VK_MBUTTON || i == VK_RBUTTON ))
                          )
                        )
                    {
                        if (isPressed && !wasPressed) {
                            printf("Method 1: KeyState: Key: '%s' (u0x%02X) Down\n", GetKeyName(i).c_str(), i);
                        } else if (!isPressed && wasPressed) {
                            printf("Method 1: KeyState: Key: '%s' (u0x%02X) Up\n", GetKeyName(i).c_str(), i);
                        }
                    }
                    prevKeyStates[i] = keyState;
                }
            }

            // Method 2,  
            // Process the console input buffer
            
            // Avoid ReadConsoleInput blocking, waiting for input, 
            // by first checking with GetNumberOfConsoleInputEvents.
            DWORD numEvents = 0;
            if (!GetNumberOfConsoleInputEvents(hStdIn, &numEvents)){
                ErrorExit((LPSTR)"GetNumberOfConsoleInputEvents");
            }

            if (numEvents > 0){
                // Read from the console input buffer
                cNumRead = 0;
                if (!ReadConsoleInputW(
                    hStdIn,      // input buffer handle
                    irInBuf,     // buffer to read into
                    128, //numEvents,   // number of Events to put into irInBuf or sizeof(irInBuf)
                    &cNumRead))  // number of records read
                        ErrorExit(LPSTR("ReadConsoleInput"));

                // std::vector<BYTE> utf8buffer = std::vector<BYTE>();

                // Dispatch the events to the appropriate handler.
                for (i = 0; i < cNumRead; i++)
                {
                    // printf("%03u Method 2: InBuffer: ", eventCounter);
                    //ReadConsoleInput
                    current_input_record = irInBuf[i];
                    bool ignore_zeroth = false;
                    if(eventCounter == 0 
                        && current_input_record.EventType == KEY_EVENT
                        && current_input_record.Event.KeyEvent.wVirtualKeyCode == VK_RETURN
                        && current_input_record.Event.KeyEvent.bKeyDown == false)
                    {
                        ignore_zeroth = true;
                        eventCounter++;
                    }

                    // check for repeats, helps qieten it down.
                    if (CompareInputRecord(current_input_record, previous_input_record)) {
                        repeatCounter++;
                        eventCounter++;
                    } else {
                        if(repeatCounter > 0)
                        {
                            switch (previous_input_record.EventType) 
                            {
                            case KEY_EVENT:
                                printf(" -- Console Input KEY EVENT [%s] ", VK_KEYS[previous_input_record.Event.KeyEvent.wVirtualKeyCode]);
                                printf(" Repeated : %d times --\n", repeatCounter);
                                break;
                            case MOUSE_EVENT:
                                if(include_mouse_events){
                                    printf(" -- Console Input MOUSE EVENT ");
                                    printf(" Repeated : %d times --\n", repeatCounter);
                                }
                                break;
                            default:
                                printf(" -- Console Input EVENT");
                                printf(" Repeated : %d times --\n", repeatCounter);
                                break;
                            }
                        }
                        repeatCounter = 0;
                    }

                    if(!ignore_zeroth && repeatCounter == 0 &&
                        (include_mouse_events || current_input_record.EventType != MOUSE_EVENT))
                    {
                        eventCounter++;
                        
                        switch (current_input_record.EventType)
                        {
                        case KEY_EVENT: // keyboard input
                            // // just do pressed, and dont worry about released.
                            // if(current_input_record.Event.KeyEvent.bKeyDown
                            //     || current_input_record.Event.KeyEvent.wVirtualKeyCode == VK_MENU
                            //     || current_input_record.Event.KeyEvent.wVirtualKeyCode == 0 ){
                                printf("Method 2: InBuf %u/%u/%u: ", i+1, cNumRead, numEvents);
                                KeyEventProc(current_input_record.Event.KeyEvent);
                            // }
                            break;

                        case MOUSE_EVENT: // mouse input
                            if(include_mouse_events){
                                MouseEventProc(current_input_record.Event.MouseEvent);
                            }
                            break;

                        case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
                            ResizeEventProc(current_input_record.Event.WindowBufferSizeEvent);
                            break;

                        case FOCUS_EVENT:  // focus events
                            FocusEventProc(current_input_record.Event.FocusEvent);
                            break;

                        case MENU_EVENT:   // menu events
                            MenuEventProc(current_input_record.Event.MenuEvent);
                            break;

                        default:
                            ErrorExit(LPSTR("Unknown event type"));
                            break;
                        }
                    }
                    previous_input_record = current_input_record;
                }
            }

            // static int s = 0;
            // printf("%d \n",++s);
            Sleep(50);
        
        }

    } catch (const std::exception& e) {
        // Handle the exception
        std::cerr << "************* Error: *****************\n" 
                    << e.what() << std::endl; 
    }

    printf("\nBye!\n");
    std::cout << "Fin\n";
    // _getch();\

    // This is specific to windows only...
    system("pause");
    return 0;
}

// Function to get the name of a key based on its virtual key code
std::string GetKeyName(BYTE virtualKey) {

    UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

    // Add extended flag for certain keys
    switch (virtualKey) {
        case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
        case VK_PRIOR: case VK_NEXT: case VK_END: case VK_HOME:
        case VK_INSERT: case VK_DELETE: case VK_DIVIDE:
        case VK_NUMLOCK:
            scanCode |= 0x100;
            break;
    }

    // Use wchar_t for Unicode UTF-16 characters
    wchar_t wc_keyName[128]; 
    std::wstring ws_keyName;
    if (GetKeyNameTextW(scanCode << 16, wc_keyName, sizeof(wc_keyName) / sizeof(wchar_t)) > 0) {
        ws_keyName = wc_keyName;
        switch (virtualKey) {
        case VK_LSHIFT: case VK_LCONTROL: case VK_LMENU:
            if (ws_keyName.compare(0, 4, L"Left") == 0) {
                ws_keyName = ws_keyName.substr(5); 
            }
            ws_keyName = L"Left-" + ws_keyName;
            break;
        case VK_RSHIFT: case VK_RCONTROL: case VK_RMENU:
            if (ws_keyName.compare(0, 5, L"Right") == 0) {
                ws_keyName = ws_keyName.substr(6); 
            }
            ws_keyName = L"Right-" + ws_keyName;
            break;
        default:
            break;
        }
    } else {
        // Use the virtual key code as a string if the name is unknown
        ws_keyName = L"VK Code " + std::to_wstring(virtualKey); 
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string utf8_keyName = converter.to_bytes(ws_keyName);
    return utf8_keyName;
}

// Helper functions to check for high and low surrogates
bool isHighSurrogate(WCHAR ch) {
    return (ch >= 0xD800 && ch <= 0xDBFF);
}
bool isLowSurrogate(WCHAR ch) {
    return (ch >= 0xDC00 && ch <= 0xDFFF);
}

char32_t combineSurrogates(WCHAR highSurrogate, WCHAR lowSurrogate) {
  // Check if the input values are valid surrogates
  if (!isHighSurrogate(highSurrogate) || !isLowSurrogate(lowSurrogate)) {
    // Handle invalid input (e.g., throw an exception, return an error code)
    return 0; // Or some other error indicator
  }

  // Calculate the code point
  char32_t codePoint = 0x10000 + 
                      ((highSurrogate - 0xD800) << 10) + 
                      (lowSurrogate - 0xDC00);
  return codePoint;
}

static VOID KeyEventProc(KEY_EVENT_RECORD ker)
{

    WORD vkCode(ker.wVirtualKeyCode);
    if ( vkCode != 0 && vkCode < 255)
        vkCode = getLRKeyCode(ker);

    if( vkCode == 0){
        printf("Key: [(0)] ");
    } else if  (vkCode < 255) {
        printf("Key: [%s] ", VK_KEYS[vkCode] );
    } else {
        printf("Key: CODE: u0x%02X (%d) ", vkCode, vkCode);
    }

    static WCHAR firstWchar = 0;

    if (ker.uChar.UnicodeChar != 0)
    {
        WCHAR chU = ker.uChar.UnicodeChar;

        if (isHighSurrogate(chU)) {
            // Handle the first WCHAR of a surrogate pair
            firstWchar = chU; // Store the first WCHAR
            printf("uChars: {u%04X, u<pending>}", chU );
        } else if (isLowSurrogate(chU) && firstWchar != 0) {
            WCHAR uChars[5] = {'\'', firstWchar, chU, '\'', L'\0'};
            printf("uChars: {u%04X, u%04X} ", firstWchar ,chU);
            WriteConsoleW(hStdOut, uChars , 4, NULL, NULL);
            firstWchar = 0; // Reset for the next surrogate pair
        } else {
            firstWchar = 0; // Clear not a pair
            // Handle single-WCHAR characters or raw bytes
            printf("uChar: {u%04X}, ",ker.uChar.UnicodeChar);
            if(!iscntrl(ker.uChar.UnicodeChar)){
                WCHAR uChars[4] = {'\'',ker.uChar.UnicodeChar, '\'',  L'\0'};
                WriteConsoleW(hStdOut, uChars , 3, NULL, NULL);
            }
        }
    }
        
    /*
        ☺ (U+263A): This is the classic "smiling face" or "smiley".
        It's a simple, happy face with a smile and closed eyes.
        It's been around for a long time, even before emojis were common
        in digital communication.
        
        🙂 (U+1F642): This is the "slightly smiling face". 
        It's a bit more subtle than the classic smiley, with a gentle,
        closed-mouth smile. 

        👍 (U+1F44D): This is the "thumbs up" emoji. 

        🚀 (U+1F680): This is the "rocket" emoji.
    */

    printf(" rptCount:%lu", ker.wRepeatCount);
    printf(" vKeyCode:0x%02X", ker.wVirtualKeyCode);
    printf(" vScanCode:0x%02X", ker.wVirtualScanCode);
    printf(" vControlKeyState:0x%02X", ker.dwControlKeyState );

    if(ker.dwControlKeyState){
        printf(" (");
        if( ker.dwControlKeyState & RIGHT_ALT_PRESSED ) { 
            printf("RIGHT_ALT_PRESSED|");
        }
        if( ker.dwControlKeyState & LEFT_ALT_PRESSED ) { 
            printf("LEFT_ALT_PRESSED|");
        }
        if( ker.dwControlKeyState & RIGHT_CTRL_PRESSED ) { 
            printf("RIGHT_CTRL_PRESSED|");
        }
        if( ker.dwControlKeyState & LEFT_CTRL_PRESSED ) { 
            printf("LEFT_CTRL_PRESSED|");
        }
        if( ker.dwControlKeyState & SHIFT_PRESSED ) { 
            printf("SHIFT_PRESSED|");
        }
        if( ker.dwControlKeyState & NUMLOCK_ON ) { 
            printf("NUMLOCK_ON|");
        }
        if( ker.dwControlKeyState & SCROLLLOCK_ON ) { 
            printf("SCROLLLOCK_ON|");
        }
        if( ker.dwControlKeyState & CAPSLOCK_ON ) { 
            printf("CAPSLOCK_ON|");
        }
        if( ker.dwControlKeyState & ENHANCED_KEY ) { 
            printf("ENHANCED_KEY|");
        }
        if( ker.dwControlKeyState & NLS_DBCSCHAR ) { 
            printf("NLS_DBCSCHAR|");
        }
        if( ker.dwControlKeyState & NLS_ALPHANUMERIC ) { 
            printf("NLS_ALPHANUMERIC|");
        }
        if( ker.dwControlKeyState & NLS_KATAKANA ) { 
            printf("NLS_KATAKANA|");
        }
        if( ker.dwControlKeyState & NLS_HIRAGANA ) { 
            printf("NLS_HIRAGANA|");
        }
        if( ker.dwControlKeyState & NLS_ROMAN ) { 
            printf("NLS_ROMAN|");
        }
        if( ker.dwControlKeyState & NLS_IME_CONVERSION ) { 
            printf("NLS_IME_CONVERSION|");
        }
        if( ker.dwControlKeyState & ALTNUMPAD_BIT ) { 
            printf("ALTNUMPAD_BIT|");
        }
        if( ker.dwControlKeyState & NLS_IME_DISABLE ) { 
            printf("NLS_IME_DISABLE|");
        }
        printf("\b)");
    }
    

    if (ker.bKeyDown) {
        printf(" Down");
    }        
    else {
        printf(" Up");
    }
    printf("\n");
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
    printf("\n");
}

static VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
    printf("Resize Event: %d columns(x) by %d rows(y).", wbsr.dwSize.X, wbsr.dwSize.Y);
    printf("\n");
}

static VOID FocusEventProc(FOCUS_EVENT_RECORD fer)
{
    isFocussed = fer.bSetFocus;
    printf("Focus Event: %s", fer.bSetFocus ? "Focussed" : "Unfocussed");
    printf("\n");
}

static void MenuEventProc(MENU_EVENT_RECORD mer)
{
    printf("Menu Event: %u", mer.dwCommandId);
    printf("\n");
}

// Returns 'true' only if same(ish) INPUT_RECORD, eg useful for repeated.
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

static WORD getLRKeyCode(KEY_EVENT_RECORD ker) {

    WORD lrKeyCode = ker.wVirtualKeyCode;

    auto ctrl_state = ker.dwControlKeyState;

    //UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
    UINT unMappedVkCode = MapVirtualKey(ker.wVirtualScanCode , MAPVK_VSC_TO_VK_EX);
    if(unMappedVkCode != 0){
        lrKeyCode = unMappedVkCode;
    }

    if (lrKeyCode == VK_LCONTROL ) {
        if( !( ctrl_state & LEFT_CTRL_PRESSED) 
           && (ctrl_state & RIGHT_CTRL_PRESSED || ctrl_state & ENHANCED_KEY)
        ){
            lrKeyCode = VK_RCONTROL;
        }
    }

    if (lrKeyCode == VK_LMENU ) {
        // if( !( ctrl_state & LEFT_ALT_PRESSED) 
        //    && (ctrl_state & RIGHT_ALT_PRESSED || ctrl_state & ENHANCED_KEY)
        // ){
        if(ctrl_state & ENHANCED_KEY){
            lrKeyCode = VK_RMENU;
        }
    }

    return lrKeyCode;


    /*
        RIGHT_ALT_PRESSED     0x0001
        LEFT_ALT_PRESSED      0x0002
        RIGHT_CTRL_PRESSED    0x0004
        LEFT_CTRL_PRESSED     0x0008
        SHIFT_PRESSED         0x0010
    */

//    auto ctrl_state = ker.dwControlKeyState;

    // WORD lrKeyCode = vkCode;
    // //SHORT keyState = 0;
    // SHORT keyLState = 0;
    // SHORT keyRState = 0;
    // switch (vkCode) {
    // case VK_SHIFT:
    //     keyLState = GetAsyncKeyState(VK_LSHIFT);
    //     if (keyLState != 0) {
    //         lrKeyCode = VK_LSHIFT;
    //     }
    //     keyRState = GetAsyncKeyState(VK_RSHIFT);
    //     if (keyRState != 0) {
    //         lrKeyCode = VK_RSHIFT;
    //     }
    //     break;
    // case VK_CONTROL:
    //     keyLState = GetAsyncKeyState(VK_LCONTROL);
    //     if (keyLState != 0) {
    //         lrKeyCode = VK_LCONTROL;
    //     }
    //     keyRState = GetAsyncKeyState(VK_RCONTROL);
    //     if (keyRState != 0) {
    //         lrKeyCode = VK_RCONTROL;
    //     }
    //     break;
    // case VK_MENU:
    //     keyLState = GetAsyncKeyState(VK_LMENU);
    //     if (keyLState != 0) {
    //         lrKeyCode = VK_LMENU;
    //     }
    //     keyRState = GetAsyncKeyState(VK_RMENU);
    //     if (keyRState != 0) {
    //         lrKeyCode = VK_RMENU;
    //     }
    //     break;
    // }
    // return lrKeyCode;


    /////////////////////////////////


    // //ker.wVirtualKeyCode;
    // WORD lrKeyCode = vkCode;
    // //SHORT keyState = 0;
    // SHORT keyLState = 0;
    // SHORT keyRState = 0;
    // switch (vkCode) {
    // case VK_SHIFT:
    //     keyLState = GetAsyncKeyState(VK_LSHIFT);
    //     if (keyLState != 0) {
    //         lrKeyCode = VK_LSHIFT;
    //     }
    //     keyRState = GetAsyncKeyState(VK_RSHIFT);
    //     if (keyRState != 0) {
    //         lrKeyCode = VK_RSHIFT;
    //     }
    //     break;
    // case VK_CONTROL:
    //     keyLState = GetAsyncKeyState(VK_LCONTROL);
    //     if (keyLState != 0) {
    //         lrKeyCode = VK_LCONTROL;
    //     }
    //     keyRState = GetAsyncKeyState(VK_RCONTROL);
    //     if (keyRState != 0) {
    //         lrKeyCode = VK_RCONTROL;
    //     }
    //     break;
    // case VK_MENU:
    //     keyLState = GetAsyncKeyState(VK_LMENU);
    //     if (keyLState != 0) {
    //         lrKeyCode = VK_LMENU;
    //     }
    //     keyRState = GetAsyncKeyState(VK_RMENU);
    //     if (keyRState != 0) {
    //         lrKeyCode = VK_RMENU;
    //     }
    //     break;
    // }
    // return lrKeyCode;
}

static VOID ErrorExit(LPSTR lpszMessage)
{
    printf("\n\n ERROR -----\n\n");
    system("pause");

    fprintf(stderr, "%s\n", lpszMessage);

    // Restore input mode on exit.

    SetConsoleMode(hStdIn, fdwSaveOldMode);

    printf("\n\n ----- ERROR\n\n");
    system("pause");

    ExitProcess(0);
}
