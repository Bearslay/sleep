#ifndef SLEEP
#define SLEEP

#include "ncursespp.hpp"
#include "timing.hpp"

namespace Sleep {
    /// @brief Convert an std::string to an std::wstring
    /// @param input An std::string input
    /// @returns An std::wstring that should match the input (except for data type)
    std::wstring strtowstr(std::string input) {
        std::wstring str;
        for (unsigned int i = 0; i < input.length(); i++) {
            str += input[i];
        }
        return str;
    }

    class Pi {
        private:
            npp::Window Win;
            npp::Button Exit;
            npp::Button Charge;
            npp::Button Settings;

            Timing::Alarm Uptime[7];
            Timing::Alarm Downtime[7];

            bool IsCharging = false;
            bool Use24Hr = true;

            void rMainMenu() {
                // Exit
                Win.dbox(4, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, 2);
                Win.wmstr(6, Win.gdimx() - 11, "X", MTEXT_6x6, 2);

                // Toggle Charging
                Win.dbox(12, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, IsCharging ? 3 : 4);
                Win.wmstr(14, Win.gdimx() - 11, "C", MTEXT_6x6, IsCharging ? 3 : 4);

                // Enter Settings
                Win.dbox(20, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, Use24Hr ? 7 : 5);
                Win.wmstr(22, Win.gdimx() - 11, "T", MTEXT_6x6, Use24Hr ? 7 : 5);

                // Time
                Win.wmstr(2, 40 - (!Use24Hr ? 8 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_8x8);
                Win.wmstr(7, 22, Timing::mtime.getWeekdayStr(false) + ", " + Timing::mtime.getMonthStr(false) + " " + Timing::mtime.getDayStr(true) + ", " + Timing::mtime.getYearStr());

                // Downtime/Uptime
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(Win.gdimy() - 4, 3, L"Downtime ("), strtowstr(Timing::mtime.getWeekdayStr())), L"): "), strtowstr(Downtime[Timing::mtime.getWeekdayNum()].getTimeFormatted()));
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(Win.gdimy() - 3, 3, L"Uptime   ("), strtowstr(Timing::mtime.getWeekdayStr())), L"): "), strtowstr(Uptime[Timing::mtime.getWeekdayNum()].getTimeFormatted()));
            }

            void rSettings() {

            }

            /// @brief Go into the settings menu
            /// @returns True to stay in the program, false to quit the program
            bool settings() {
                int ch;

                while (true) {
                    Timing::mtime.update(true);
                    rSettings();

                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {

                    } else {
                        if (ch == 'e') {return true;}
                        else if (ch == 'q') {return false;}
                    }
                }
            }

        public:
            Pi() {
                Win = npp::Window(LINES / 2 - 45 / 2, COLS / 2 - 60, 45, 120);
                Exit = npp::Button(Win.gposy() + 4, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                Charge = npp::Button(Win.gposy() + 12, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                Settings = npp::Button(Win.gposy() + 20, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});

                for (unsigned char i = 1; i < 6; i++) {
                    Uptime[i] = Timing::Alarm(6, 0, 0, {i});
                    Downtime[i] = Timing::Alarm(22, 0, 0, {i});
                }
                Uptime[0] = Timing::Alarm(7, 0, 0, {0});
                Uptime[6] = Timing::Alarm(7, 0, 0, {6});
                Downtime[0] = Timing::Alarm(23, 0, 0, {0});
                Downtime[6] = Timing::Alarm(23, 0, 0, {6});

                Timing::mtime.update();
            }

            int main() {
                Win.dbox();

                int ch;
                while (true) {
                    Timing::mtime.update(true);
                    rMainMenu();
                    
                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {
                        if (npp::Mouse.gmouse(ch)) {
                            if (Exit.cclick() == M1_CLICK) {return npp::end(true);}
                            if (Charge.cclick() == M1_CLICK) {IsCharging = !IsCharging;}
                            if (Settings.cclick() == M1_CLICK) {if (!settings()) {return npp::end(true);}}
                        }
                    } else {
                        if (ch == 'q') {return npp::end(true);}
                    }
                }
            }
    };
}

#endif
