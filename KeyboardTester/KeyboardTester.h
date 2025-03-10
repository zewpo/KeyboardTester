// KeyboardTester.h

#pragma once
//#include <string>

const int MAX_VK_CODE = 0xFE; // Maximum virtual key code
const char* VK_KEYS[MAX_VK_CODE + 1] = { };
static void InitializeVKKeys() {
    VK_KEYS[0x01] = "VK_LBUTTON";
    VK_KEYS[0x02] = "VK_RBUTTON";
    VK_KEYS[0x03] = "VK_CANCEL";
    VK_KEYS[0x04] = "VK_MBUTTON";
    VK_KEYS[0x05] = "VK_XBUTTON1";
    VK_KEYS[0x06] = "VK_XBUTTON2";
    VK_KEYS[0x07] = "reserved";
    VK_KEYS[0x08] = "VK_BACK";
    VK_KEYS[0x09] = "VK_TAB";
    VK_KEYS[0x0A] = "reserved";
    VK_KEYS[0x0B] = "reserved";
    VK_KEYS[0x0C] = "VK_CLEAR";
    VK_KEYS[0x0D] = "VK_RETURN";
    VK_KEYS[0x0E] = "unassigned";
    VK_KEYS[0x0F] = "unassigned";
    VK_KEYS[0x10] = "VK_SHIFT";
    VK_KEYS[0x11] = "VK_CONTROL";
    VK_KEYS[0x12] = "VK_MENU";
    VK_KEYS[0x13] = "VK_PAUSE";
    VK_KEYS[0x14] = "VK_CAPITAL";
    VK_KEYS[0x15] = "VK_KANA";
    VK_KEYS[0x16] = "VK_IME_ON";
    VK_KEYS[0x17] = "VK_JUNJA";
    VK_KEYS[0x18] = "VK_FINAL";
    VK_KEYS[0x19] = "VK_HANJA";
    VK_KEYS[0x1A] = "VK_IME_OFF";
    VK_KEYS[0x1B] = "VK_ESCAPE";
    VK_KEYS[0x1C] = "VK_CONVERT";
    VK_KEYS[0x1D] = "VK_NONCONVERT";
    VK_KEYS[0x1E] = "VK_ACCEPT";
    VK_KEYS[0x1F] = "VK_MODECHANGE";
    VK_KEYS[0x20] = "VK_SPACE";
    VK_KEYS[0x21] = "VK_PRIOR";
    VK_KEYS[0x22] = "VK_NEXT";
    VK_KEYS[0x23] = "VK_END";
    VK_KEYS[0x24] = "VK_HOME";
    VK_KEYS[0x25] = "VK_LEFT";
    VK_KEYS[0x26] = "VK_UP";
    VK_KEYS[0x27] = "VK_RIGHT";
    VK_KEYS[0x28] = "VK_DOWN";
    VK_KEYS[0x29] = "VK_SELECT";
    VK_KEYS[0x2A] = "VK_PRINT";
    VK_KEYS[0x2B] = "VK_EXECUTE";
    VK_KEYS[0x2C] = "VK_SNAPSHOT";
    VK_KEYS[0x2D] = "VK_INSERT";
    VK_KEYS[0x2E] = "VK_DELETE";
    VK_KEYS[0x2F] = "VK_HELP";
    VK_KEYS[0x30] = "VK_0";
    VK_KEYS[0x31] = "VK_1";
    VK_KEYS[0x32] = "VK_2";
    VK_KEYS[0x33] = "VK_3";
    VK_KEYS[0x34] = "VK_4";
    VK_KEYS[0x35] = "VK_5";
    VK_KEYS[0x36] = "VK_6";
    VK_KEYS[0x37] = "VK_7";
    VK_KEYS[0x38] = "VK_8";
    VK_KEYS[0x39] = "VK_9";
    VK_KEYS[0x3A] = "unassigned";
    VK_KEYS[0x3B] = "unassigned";
    VK_KEYS[0x3C] = "unassigned";
    VK_KEYS[0x3D] = "unassigned";
    VK_KEYS[0x3E] = "unassigned";
    VK_KEYS[0x3F] = "unassigned";
    VK_KEYS[0x40] = "unassigned";
    VK_KEYS[0x41] = "VK_A";
    VK_KEYS[0x42] = "VK_B";
    VK_KEYS[0x43] = "VK_C";
    VK_KEYS[0x44] = "VK_D";
    VK_KEYS[0x45] = "VK_E";
    VK_KEYS[0x46] = "VK_F";
    VK_KEYS[0x47] = "VK_G";
    VK_KEYS[0x48] = "VK_H";
    VK_KEYS[0x49] = "VK_I";
    VK_KEYS[0x4A] = "VK_J";
    VK_KEYS[0x4B] = "VK_K";
    VK_KEYS[0x4C] = "VK_L";
    VK_KEYS[0x4D] = "VK_M";
    VK_KEYS[0x4E] = "VK_N";
    VK_KEYS[0x4F] = "VK_O";
    VK_KEYS[0x50] = "VK_P";
    VK_KEYS[0x51] = "VK_Q";
    VK_KEYS[0x52] = "VK_R";
    VK_KEYS[0x53] = "VK_S";
    VK_KEYS[0x54] = "VK_T";
    VK_KEYS[0x55] = "VK_U";
    VK_KEYS[0x56] = "VK_V";
    VK_KEYS[0x57] = "VK_W";
    VK_KEYS[0x58] = "VK_X";
    VK_KEYS[0x59] = "VK_Y";
    VK_KEYS[0x5A] = "VK_Z";
    VK_KEYS[0x5B] = "VK_LWIN";
    VK_KEYS[0x5C] = "VK_RWIN";
    VK_KEYS[0x5D] = "VK_APPS";
    VK_KEYS[0x5E] = "reserved";
    VK_KEYS[0x5F] = "VK_SLEEP";
    VK_KEYS[0x60] = "VK_NUMPAD0";
    VK_KEYS[0x61] = "VK_NUMPAD1";
    VK_KEYS[0x62] = "VK_NUMPAD2";
    VK_KEYS[0x63] = "VK_NUMPAD3";
    VK_KEYS[0x64] = "VK_NUMPAD4";
    VK_KEYS[0x65] = "VK_NUMPAD5";
    VK_KEYS[0x66] = "VK_NUMPAD6";
    VK_KEYS[0x67] = "VK_NUMPAD7";
    VK_KEYS[0x68] = "VK_NUMPAD8";
    VK_KEYS[0x69] = "VK_NUMPAD9";
    VK_KEYS[0x6A] = "VK_MULTIPLY";
    VK_KEYS[0x6B] = "VK_ADD";
    VK_KEYS[0x6C] = "VK_SEPARATOR";
    VK_KEYS[0x6D] = "VK_SUBTRACT";
    VK_KEYS[0x6E] = "VK_DECIMAL";
    VK_KEYS[0x6F] = "VK_DIVIDE";
    VK_KEYS[0x70] = "VK_F1";
    VK_KEYS[0x71] = "VK_F2";
    VK_KEYS[0x72] = "VK_F3";
    VK_KEYS[0x73] = "VK_F4";
    VK_KEYS[0x74] = "VK_F5";
    VK_KEYS[0x75] = "VK_F6";
    VK_KEYS[0x76] = "VK_F7";
    VK_KEYS[0x77] = "VK_F8";
    VK_KEYS[0x78] = "VK_F9";
    VK_KEYS[0x79] = "VK_F10";
    VK_KEYS[0x7A] = "VK_F11";
    VK_KEYS[0x7B] = "VK_F12";
    VK_KEYS[0x7C] = "VK_F13";
    VK_KEYS[0x7D] = "VK_F14";
    VK_KEYS[0x7E] = "VK_F15";
    VK_KEYS[0x7F] = "VK_F16";
    VK_KEYS[0x80] = "VK_F17";
    VK_KEYS[0x81] = "VK_F18";
    VK_KEYS[0x82] = "VK_F19";
    VK_KEYS[0x83] = "VK_F20";
    VK_KEYS[0x84] = "VK_F21";
    VK_KEYS[0x85] = "VK_F22";
    VK_KEYS[0x86] = "VK_F23";
    VK_KEYS[0x87] = "VK_F24";
    VK_KEYS[0x88] = "VK_NAVIGATION_VIEW";
    VK_KEYS[0x89] = "VK_NAVIGATION_MENU";
    VK_KEYS[0x8A] = "VK_NAVIGATION_UP";
    VK_KEYS[0x8B] = "VK_NAVIGATION_DOWN";
    VK_KEYS[0x8C] = "VK_NAVIGATION_LEFT";
    VK_KEYS[0x8D] = "VK_NAVIGATION_RIGHT";
    VK_KEYS[0x8E] = "VK_NAVIGATION_ACCEPT";
    VK_KEYS[0x8F] = "VK_NAVIGATION_CANCEL";
    VK_KEYS[0x90] = "VK_NUMLOCK";
    VK_KEYS[0x91] = "VK_SCROLL";
    VK_KEYS[0x92] = "VK_OEM_FJ_JISHO/VK_OEM_NEC_EQUAL";
    VK_KEYS[0x93] = "VK_OEM_FJ_MASSHOU";
    VK_KEYS[0x94] = "VK_OEM_FJ_TOUROKU";
    VK_KEYS[0x95] = "VK_OEM_FJ_LOYA";
    VK_KEYS[0x96] = "VK_OEM_FJ_ROYA";
    VK_KEYS[0x97] = "unassigned";
    VK_KEYS[0x98] = "unassigned";
    VK_KEYS[0x99] = "unassigned";
    VK_KEYS[0x9A] = "unassigned";
    VK_KEYS[0x9B] = "unassigned";
    VK_KEYS[0x9C] = "unassigned";
    VK_KEYS[0x9D] = "unassigned";
    VK_KEYS[0x9E] = "unassigned";
    VK_KEYS[0x9F] = "unassigned";
    VK_KEYS[0xA0] = "VK_LSHIFT";
    VK_KEYS[0xA1] = "VK_RSHIFT";
    VK_KEYS[0xA2] = "VK_LCONTROL";
    VK_KEYS[0xA3] = "VK_RCONTROL";
    VK_KEYS[0xA4] = "VK_LMENU";
    VK_KEYS[0xA5] = "VK_RMENU";
    VK_KEYS[0xA6] = "VK_BROWSER_BACK";
    VK_KEYS[0xA7] = "VK_BROWSER_FORWARD";
    VK_KEYS[0xA8] = "VK_BROWSER_REFRESH";
    VK_KEYS[0xA9] = "VK_BROWSER_STOP";
    VK_KEYS[0xAA] = "VK_BROWSER_SEARCH";
    VK_KEYS[0xAB] = "VK_BROWSER_FAVORITES";
    VK_KEYS[0xAC] = "VK_BROWSER_HOME";
    VK_KEYS[0xAD] = "VK_VOLUME_MUTE";
    VK_KEYS[0xAE] = "VK_VOLUME_DOWN";
    VK_KEYS[0xAF] = "VK_VOLUME_UP";
    VK_KEYS[0xB0] = "VK_MEDIA_NEXT_TRACK";
    VK_KEYS[0xB1] = "VK_MEDIA_PREV_TRACK";
    VK_KEYS[0xB2] = "VK_MEDIA_STOP";
    VK_KEYS[0xB3] = "VK_MEDIA_PLAY_PAUSE";
    VK_KEYS[0xB4] = "VK_LAUNCH_MAIL";
    VK_KEYS[0xB5] = "VK_LAUNCH_MEDIA_SELECT";
    VK_KEYS[0xB6] = "VK_LAUNCH_APP1";
    VK_KEYS[0xB7] = "VK_LAUNCH_APP2";
    VK_KEYS[0xB8] = "reserved";
    VK_KEYS[0xB9] = "reserved";
    VK_KEYS[0xBA] = "VK_OEM_1";
    VK_KEYS[0xBB] = "VK_OEM_PLUS";
    VK_KEYS[0xBC] = "VK_OEM_COMMA";
    VK_KEYS[0xBD] = "VK_OEM_MINUS";
    VK_KEYS[0xBE] = "VK_OEM_PERIOD";
    VK_KEYS[0xBF] = "VK_OEM_2";
    VK_KEYS[0xC0] = "VK_OEM_3";
    VK_KEYS[0xC1] = "reserved";
    VK_KEYS[0xC2] = "reserved";
    VK_KEYS[0xC3] = "VK_GAMEPAD_A";
    VK_KEYS[0xC4] = "VK_GAMEPAD_B";
    VK_KEYS[0xC5] = "VK_GAMEPAD_X";
    VK_KEYS[0xC6] = "VK_GAMEPAD_Y";
    VK_KEYS[0xC7] = "VK_GAMEPAD_RIGHT_SHOULDER";
    VK_KEYS[0xC8] = "VK_GAMEPAD_LEFT_SHOULDER";
    VK_KEYS[0xC9] = "VK_GAMEPAD_LEFT_TRIGGER";
    VK_KEYS[0xCA] = "VK_GAMEPAD_RIGHT_TRIGGER";
    VK_KEYS[0xCB] = "VK_GAMEPAD_DPAD_UP";
    VK_KEYS[0xCC] = "VK_GAMEPAD_DPAD_DOWN";
    VK_KEYS[0xCD] = "VK_GAMEPAD_DPAD_LEFT";
    VK_KEYS[0xCE] = "VK_GAMEPAD_DPAD_RIGHT";
    VK_KEYS[0xCF] = "VK_GAMEPAD_MENU";
    VK_KEYS[0xD0] = "VK_GAMEPAD_VIEW";
    VK_KEYS[0xD1] = "VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON";
    VK_KEYS[0xD2] = "VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON";
    VK_KEYS[0xD3] = "VK_GAMEPAD_LEFT_THUMBSTICK_UP";
    VK_KEYS[0xD4] = "VK_GAMEPAD_LEFT_THUMBSTICK_DOWN";
    VK_KEYS[0xD5] = "VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT";
    VK_KEYS[0xD6] = "VK_GAMEPAD_LEFT_THUMBSTICK_LEFT";
    VK_KEYS[0xD7] = "VK_GAMEPAD_RIGHT_THUMBSTICK_UP";
    VK_KEYS[0xD8] = "VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN";
    VK_KEYS[0xD9] = "VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT";
    VK_KEYS[0xDA] = "VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT";
    VK_KEYS[0xDB] = "VK_OEM_4";
    VK_KEYS[0xDC] = "VK_OEM_5";
    VK_KEYS[0xDD] = "VK_OEM_6";
    VK_KEYS[0xDE] = "VK_OEM_7";
    VK_KEYS[0xDF] = "VK_OEM_8";
    VK_KEYS[0xE0] = "reserved";
    VK_KEYS[0xE1] = "VK_OEM_AX";
    VK_KEYS[0xE2] = "VK_OEM_102";
    VK_KEYS[0xE3] = "VK_ICO_HELP";
    VK_KEYS[0xE4] = "VK_ICO_00";
    VK_KEYS[0xE5] = "VK_PROCESSKEY";
    VK_KEYS[0xE6] = "VK_ICO_CLEAR";
    VK_KEYS[0xE7] = "VK_PACKET";
    VK_KEYS[0xE8] = "unassigned";
    VK_KEYS[0xE9] = "VK_OEM_RESET";
    VK_KEYS[0xEA] = "VK_OEM_JUMP";
    VK_KEYS[0xEB] = "VK_OEM_PA1";
    VK_KEYS[0xEC] = "VK_OEM_PA2";
    VK_KEYS[0xED] = "VK_OEM_PA3";
    VK_KEYS[0xEE] = "VK_OEM_WSCTRL";
    VK_KEYS[0xEF] = "VK_OEM_CUSEL";
    VK_KEYS[0xF0] = "VK_OEM_ATTN";
    VK_KEYS[0xF1] = "VK_OEM_FINISH";
    VK_KEYS[0xF2] = "VK_OEM_COPY";
    VK_KEYS[0xF3] = "VK_OEM_AUTO";
    VK_KEYS[0xF4] = "VK_OEM_ENLW";
    VK_KEYS[0xF5] = "VK_OEM_BACKTAB";
    VK_KEYS[0xF6] = "VK_ATTN";
    VK_KEYS[0xF7] = "VK_CRSEL";
    VK_KEYS[0xF8] = "VK_EXSEL";
    VK_KEYS[0xF9] = "VK_EREOF";
    VK_KEYS[0xFA] = "VK_PLAY";
    VK_KEYS[0xFB] = "VK_ZOOM";
    VK_KEYS[0xFC] = "VK_NONAME";
    VK_KEYS[0xFD] = "VK_PA1";
    VK_KEYS[0xFE] = "VK_OEM_CLEAR";
}


//std::string shortAsBinString(short value) {
//    std::string binary(16, '0'); // Initialize with 16 zeros
//
//    for (int i = 15; i >= 0; --i) {
//        if (value & (1 << i)) {
//            binary[i] = '1'; // Set the bit to '1' if corresponding bit in value is set
//        }
//    }
//
//    return binary;
//}