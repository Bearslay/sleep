#include "ncursespp.hpp"
using namespace npp;

int main() {
    init();

    mwin.dbox();
    for (unsigned char i = 1; i < 16; i++) {
        mwin.wstr(1, i * 2 - 1, L"██", i);
    }
    mwin.wint(mwin.wstrp(mwin.wintp(mwin.wstrp(3, 1, L"x: "), mwin.gdimx()), L", y: "), mwin.gdimy());
    mwin.wmstr(5, 1, "6x6 Test", MTEXT_6x6);
    mwin.wmstr(9, 1, "8x8 Test", MTEXT_8x8);
    mwin.gchar();

    end();
}
