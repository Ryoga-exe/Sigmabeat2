#include <Siv3D.hpp> // OpenSiv3D v0.6.4
#include <Windows.h>

constexpr size_t ButtonSize = 6;
constexpr char Keys[ButtonSize] = { 'L', 'K', 'J', 'F', 'D', 'S' };

void KeyAction(WORD VirtualKey, BOOL bKeepPressing) {
    INPUT input[1];
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = VirtualKey;
    input[0].ki.wScan = MapVirtualKey(input[0].ki.wVk, 0);
    // input[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
    input[0].ki.time = 0;
    input[0].ki.dwExtraInfo = ::GetMessageExtraInfo();
    ::SendInput(1, input, sizeof(INPUT));
    if (!bKeepPressing) {
        input[0].ki.dwFlags = KEYEVENTF_KEYUP;
        ::SendInput(1, input, sizeof(INPUT));
    }
}

void Main() {
    // シリアルポートの一覧を取得
    const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();
    const Array<String> options = infos.map([](const SerialPortInfo& info) {
		return U"{} ({})"_fmt(info.port, info.description);
    }) << U"none";

    Serial serial;
    size_t index = (options.size() - 1);

    Array<bool> state(ButtonSize);
    Array<bool> key(ButtonSize, false);
    int8 idx = 0;

    while (System::Update()) {
        const bool isOpen = serial.isOpen();

        if (SimpleGUI::RadioButtons(index, options, Vec2(200, 60))) {
            ClearPrint();

            if (index == (options.size() - 1)) {
                serial = Serial();
            }
            else {
                Print << U"Open {}"_fmt(infos[index].port);

                // シリアルポートをオープン
                if (serial.open(infos[index].port, 250000)) {
                    Print << U"Succeeded";
                }
                else {
                    Print << U"Failed";
                }
            }
        }

        if (const size_t available = serial.available()) {
            auto line = serial.readBytes();
            Print << line;
            for (auto e : line) {
                if (e == '0') {
                    if (idx >= ButtonSize) {
                        continue;
                    }
                    state[idx] = false;
                }
                else if (e == '1') {
                    if (idx >= ButtonSize) {
                        continue;
                    }
                    state[idx] = true;
                }
                else {
                    ClearPrint();
                    Print << state;

                    for (uint8 i = 0; i < ButtonSize; i++) {
                        if (state[i]) {
                            if (!key[i]) {
                                KeyAction(Keys[i], TRUE);
                                key[i] = true;
                            }
                        }
                        else {
                            if (key[i]) {
                                KeyAction(Keys[i], FALSE);
                                key[i] = false;
                            }
                        }
                    }
                    idx = -1;
                }
                idx++;
            }

        }
        
        double width = static_cast<double>(Scene::Width()) / ButtonSize;
        for (uint8 i = 0; i < ButtonSize; i++) {
            if (state[i]) {
                RectF(width * i, 300.0, width, 200.0).draw(Palette::Red);
            }
        }
    }
}
