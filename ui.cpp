#include "ncursespp.hpp"
using namespace npp;

int main() {
    init();

    mwin.dbox();
    mwin.wint(mwin.wstrp(mwin.wintp(mwin.wstrp(1, 1, L"x: "), mwin.gdimx()), L", y: "), mwin.gdimy());
    mwin.wmstr(3, 1, "6x6 Test", MTEXT_6x6);
    mwin.wmstr(7, 1, "8x8 Test", MTEXT_8x8);
    mwin.gchar();

    end();
}
