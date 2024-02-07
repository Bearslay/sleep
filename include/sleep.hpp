#ifndef SLEEP
#define SLEEP

#include "ncursespp.hpp"
#include "timing.hpp"

#define TRANS_MAIN 0
#define TRANS_BUFFER 1
#define TRANS_WARNING 2

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

    class TransitionPoint {
        private:
            Timing::Alarm MainTime;
            Timing::Alarm BufferTime;
            Timing::Alarm WarningTime;

        public:
            TransitionPoint(unsigned char hour = 0, unsigned char minute = 0, unsigned char second = 0, std::vector<unsigned char> days = {}) {
                MainTime = BufferTime = WarningTime = Timing::Alarm(hour, minute, second, days);
            }

            bool checkMain(Timing::TimePoint comparison = Timing::mtime) {return MainTime.check(comparison);}
            void acknowledgeMain(Timing::TimePoint t = Timing::mtime) {MainTime.acknowledge(t);}

            bool checkBuffer(Timing::TimePoint comparison = Timing::mtime) {return BufferTime.check(comparison);}
            void acknowledgeBuffer(Timing::TimePoint t = Timing::mtime) {BufferTime.acknowledge(t);}

            bool checkWarning(Timing::TimePoint comparison = Timing::mtime) {return WarningTime.check(comparison);}
            void acknowledgeWarning(Timing::TimePoint t = Timing::mtime) {WarningTime.acknowledge(t);}

            bool setMainHour(unsigned char hour) {return MainTime.setHour(hour);}
            bool setMainMinute(unsigned char minute) {return MainTime.setHour(minute);}
            bool setMainSecond(unsigned char second) {return MainTime.setHour(second);}

            bool setBufferHour(unsigned char hour) {return BufferTime.setHour(hour);}
            bool setBufferMinute(unsigned char minute) {return BufferTime.setHour(minute);}
            bool setBufferSecond(unsigned char second) {return BufferTime.setHour(second);}

            bool setWarningHour(unsigned char hour) {return WarningTime.setHour(hour);}
            bool setWarningMinute(unsigned char minute) {return WarningTime.setHour(minute);}
            bool setWarningSecond(unsigned char second) {return WarningTime.setHour(second);}

            const std::string getMainTime(bool use24Hr = true) {return MainTime.getTimeFormatted(use24Hr);}
            const char getMainHourNum(bool use24Hr = true) {return MainTime.getHourNum(use24Hr);}
            const std::string getMainHourStr(bool use24Hr = true, bool addSuffix = false) {return MainTime.getHourStr(use24Hr, addSuffix);}
            const char getMainMinuteNum() {return MainTime.getMinuteNum();}
            const std::string getMainMinuteStr() {return MainTime.getMinuteStr();}
            const char getMainSecondNum() {return MainTime.getSecondNum();}
            const std::string getMainSecondStr() {return MainTime.getSecondStr();}

            const std::string getBufferTime(bool use24Hr = true) {return BufferTime.getTimeFormatted(use24Hr);}
            const char getBufferHourNum(bool use24Hr = true) {return BufferTime.getHourNum(use24Hr);}
            const std::string getBufferHourStr(bool use24Hr = true, bool addSuffix = false) {return BufferTime.getHourStr(use24Hr, addSuffix);}
            const char getBufferMinuteNum() {return BufferTime.getMinuteNum();}
            const std::string getBufferMinuteStr() {return BufferTime.getMinuteStr();}
            const char getBufferSecondNum() {return BufferTime.getSecondNum();}
            const std::string getBufferSecondStr() {return BufferTime.getSecondStr();}

            const std::string getWarningTime(bool use24Hr = true) {return WarningTime.getTimeFormatted(use24Hr);}
            const char getWarningHourNum(bool use24Hr = true) {return WarningTime.getHourNum(use24Hr);}
            const std::string getWarningHourStr(bool use24Hr = true, bool addSuffix = false) {return WarningTime.getHourStr(use24Hr, addSuffix);}
            const char getWarningMinuteNum() {return WarningTime.getMinuteNum();}
            const std::string getWarningMinuteStr() {return WarningTime.getMinuteStr();}
            const char getWarningSecondNum() {return WarningTime.getSecondNum();}
            const std::string getWarningSecondStr() {return WarningTime.getSecondStr();}
    };

    class Pi {
        private:
            npp::Window Win;
        
            TransitionPoint Uptime[7];
            TransitionPoint Downtime[7];

            bool IsCharging = false;
            bool Use24Hr = true;

            void rMainMenu() {
                // Time
                Win.wmstr(2, 40 - (!Use24Hr ? 8 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_8x8);
                Win.wmstr(7, 22, Timing::mtime.getDateFormatted(false, true));

                // Exit
                Win.dbox(4, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_RED);
                Win.wmstr(6, Win.gdimx() - 11, "X", MTEXT_6x6, NPP_RED);

                // Toggle Charging
                Win.dbox(12, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, IsCharging ? NPP_LIME : NPP_YELLOW);
                Win.wmstr(14, Win.gdimx() - 11, "C", MTEXT_6x6, IsCharging ? NPP_LIME : NPP_YELLOW);

                // Enter Settings
                Win.dbox(20, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_LIGHT_GRAY);
                Win.wmstr(22, Win.gdimx() - 11, "S", MTEXT_6x6, NPP_LIGHT_GRAY);

                // Downtime/Uptime
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(Win.gdimy() - 4, 3, L"Downtime ("), strtowstr(Timing::mtime.getWeekdayStr())), L"): "), strtowstr(Downtime[Timing::mtime.getWeekdayNum()].getMainTime()));
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(Win.gdimy() - 3, 3, L"Uptime   ("), strtowstr(Timing::mtime.getWeekdayStr())), L"): "), strtowstr(Uptime[Timing::mtime.getWeekdayNum()].getMainTime()));
            }

            void rSettingsMain() {
                // Time
                Win.wmstr(2, 42 - (!Use24Hr ? 6 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_6x6);

                // Back
                Win.dbox(4, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_RED);
                Win.wmstr(6, Win.gdimx() - 11, "X", MTEXT_6x6, NPP_RED);

                // Toggle Time Format
                Win.dbox(12, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, Use24Hr ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(14, Win.gdimx() - 11, "T", MTEXT_6x6, Use24Hr ? NPP_CYAN : NPP_TEAL);

                // Change Uptime
                Win.dbox(12, 40, 5, 19);
                Win.wstr(13, 43, L"Change Uptime");
                Win.wstr(Win.wstrp(Win.wstrp(15, 43, strtowstr(Timing::mtime.getWeekdayStr(false))), L": "), strtowstr(Uptime[Timing::mtime.getWeekdayNum()].getMainTime(true)));

                // Change Downtime
                Win.dbox(12, 61, 5, 19);
                Win.wstr(13, 63, L"Change Downtime");
                Win.wstr(Win.wstrp(Win.wstrp(15, 64, strtowstr(Timing::mtime.getWeekdayStr(false))), L": "), strtowstr(Downtime[Timing::mtime.getWeekdayNum()].getMainTime(true)));
            }

            /// @brief Go into the settings menu
            /// @returns True to stay in the program, false to quit the program
            void settingsMenu() {
                npp::Button back = npp::Button(Win.gposy() + 4, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                npp::Button timeFormat = npp::Button(Win.gposy() + 12, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                npp::Button changeUptime = npp::Button(Win.gposy() + 12, Win.gposx() + 40, 5, 19, {M1_CLICK});
                npp::Button changeDowntime = npp::Button(Win.gposy() + 12, Win.gposx() + 61, 5, 19, {M1_CLICK});

                Win.reset();
                Win.dbox();

                int ch, state;
                while (true) {
                    rSettingsMain();
                    state = update();

                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {
                        if (npp::Mouse.gmouse(ch)) {
                            if (back.cclick() == M1_CLICK) {
                                return;
                            } else if (timeFormat.cclick() == M1_CLICK) {
                                // Remove the characters representing the time in the old format
                                Win.wmstr(2, 42 - (!Use24Hr ? 6 : 0), Use24Hr ? "        " : "           ", MTEXT_6x6);
                                Use24Hr = !Use24Hr;
                            } else if (changeUptime.cclick() == M1_CLICK) {
                                settingsTransition(true);
                                Win.reset();
                                Win.dbox();
                            } else if (changeDowntime.cclick() == M1_CLICK) {
                                settingsTransition(false);
                                Win.reset();
                                Win.dbox();
                            }
                        }
                    } else {
                        if (ch == 'q') {return;}
                    }
                }
            }

            void rSettingsTransition(bool uptime, unsigned char transState, unsigned char day) {
                // Time
                Win.wmstr(2, 42 - (!Use24Hr ? 6 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_6x6);

                // Back
                Win.dbox(4, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_RED);
                Win.wmstr(6, Win.gdimx() - 11, "X", MTEXT_6x6, NPP_RED);

                // Copy Time
                Win.dbox(12, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_ORANGE);
                Win.wmstr(14, Win.gdimx() - 11, "C", MTEXT_6x6, NPP_ORANGE);

                // Paste Time
                Win.dbox(20, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_ORANGE);
                Win.wmstr(22, Win.gdimx() - 11, "P", MTEXT_6x6, NPP_ORANGE);

                // Sync Days
                Win.dbox(28, Win.gdimx() - 14, 7, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_YELLOW);
                Win.wmstr(30, Win.gdimx() - 11, "D", MTEXT_6x6, NPP_YELLOW);

                // Change Main Time
                Win.dbox(4, 4, 7, 9, {LIGHT_SOFT, DASHED_NONE}, transState == TRANS_MAIN ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(6, 7, "M", MTEXT_6x6, transState == TRANS_MAIN ? NPP_CYAN : NPP_TEAL);

                // Change Buffer Time
                Win.dbox(12, 4, 7, 9, {LIGHT_SOFT, DASHED_NONE}, transState == TRANS_BUFFER ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(14, 7, "B", MTEXT_6x6, transState == TRANS_BUFFER ? NPP_CYAN : NPP_TEAL);

                // Change Warning Time
                Win.dbox(20, 4, 7, 9, {LIGHT_SOFT, DASHED_NONE}, transState == TRANS_WARNING ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(22, 7, "W", MTEXT_6x6, transState == TRANS_WARNING ? NPP_CYAN : NPP_TEAL);

                // Sync Times
                Win.dbox(28, 4, 7, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_BLUE);
                Win.wmstr(30, 7, "T", MTEXT_6x6, NPP_BLUE);

                // Weekdays
                for (unsigned char i = 0; i < 7; i++) {
                    Win.dbox(36, 4 + i * 16, 7, 15, {LIGHT_SOFT, DASHED_NONE}, i == day ? NPP_LIGHT_GRAY : NPP_DARK_GRAY);
                    Win.wmstr(38, 6 + i * 16, Timing::Keys.weekdayNamesAbb[i], MTEXT_6x6, i == day ? NPP_LIGHT_GRAY : NPP_DARK_GRAY);
                }

                // Separation Box
                Win.dbox(6, 17, 29, 85);

                // Titles
                Win.wmstr(8, 22, uptime ? " CHANGE UPTIME " : "CHANGE DOWNTIME", MTEXT_8x8);
                Win.wmstr(13, 27 + (transState == TRANS_BUFFER ? 3 : 0) + (transState == TRANS_MAIN ? 7 : 0), transState == TRANS_MAIN ? "- Main Time -" : (transState == TRANS_BUFFER ? "- Buffer Time -" : "- Warning Time -"), MTEXT_6x6);
            
                // Current Transition Time
                Win.wmstr(23, 40, uptime ? (transState == TRANS_MAIN ? Uptime[day].getMainTime() : (transState == TRANS_BUFFER ? Uptime[day].getBufferTime() : Uptime[day].getWarningTime())) : (transState == TRANS_MAIN ? Downtime[day].getMainTime() : (transState == TRANS_BUFFER ? Downtime[day].getBufferTime() : Downtime[day].getWarningTime())), MTEXT_8x8);
            }

            void settingsTransition(bool uptime) {
                npp::Button back = npp::Button(Win.gposy() + 4, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                npp::Button copyTime = npp::Button(Win.gposy() + 12, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                npp::Button pasteTime = npp::Button(Win.gposy() + 20, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                npp::Button syncDays = npp::Button(Win.gposy() + 28, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                
                npp::Button editMain = npp::Button(Win.gposy() + 4, Win.gposx() + 4, 7, 9, {M1_CLICK});
                npp::Button editBuffer = npp::Button(Win.gposy() + 12, Win.gposx() + 4, 7, 9, {M1_CLICK});
                npp::Button editWarning = npp::Button(Win.gposy() + 20, Win.gposx() + 4, 7, 9, {M1_CLICK});
                npp::Button syncTimes = npp::Button(Win.gposy() + 28, Win.gposx() + 4, 7, 9, {M1_CLICK});
                
                npp::Button addHour = npp::Button(Win.gposy() + 17, Win.gposx() + 40, 5, 9, {M1_CLICK});
                npp::Button addMinute = npp::Button(Win.gposy() + 17, Win.gposx() + 55, 5, 9, {M1_CLICK});
                npp::Button addSecond = npp::Button(Win.gposy() + 17, Win.gposx() + 70, 5, 9, {M1_CLICK});
                npp::Button removeHour = npp::Button(Win.gposy() + 28, Win.gposx() + 40, 5, 9, {M1_CLICK});
                npp::Button removeMinute = npp::Button(Win.gposy() + 28, Win.gposx() + 55, 5, 9, {M1_CLICK});
                npp::Button removeSecond = npp::Button(Win.gposy() + 28, Win.gposx() + 70, 5, 9, {M1_CLICK});

                npp::Button toggle24Hr = npp::Button(Win.gposy() + 21, Win.gposx() + 21, 8, 13, {M1_CLICK});
                npp::Button toggleAMPM = npp::Button(Win.gposy() + 21, Win.gposx() + 85, 8, 13, {M1_CLICK});

                std::vector<npp::Button> weekday;
                for (unsigned char i = 0; i < 7; i++) {weekday.emplace_back(npp::Button(Win.gposy() + Win.gdimy() - 10, Win.gposx() + 4 + i * 16, 7, 15, {M1_CLICK}));}

                Win.reset();
                Win.dbox();

                int ch, state;
                unsigned char transState = TRANS_MAIN, day = Timing::mtime.getWeekdayNum();
                while (true) {
                    rSettingsTransition(uptime, transState, day);
                    state = update();

                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {
                        if (npp::Mouse.gmouse(ch)) {
                            if (back.cclick() == M1_CLICK) {
                                return;
                            }
                        }
                    } else {
                        if (ch == 'q') {return;}
                    }
                }
            }

            int update() {
                Timing::mtime.update(true);

                if (Uptime[Timing::mtime.getWeekdayNum()].checkMain()) {

                }

                return 0;
            }

        public:
            Pi() {
                Win = npp::Window(LINES / 2 - 45 / 2, COLS / 2 - 60, 45, 120);

                for (unsigned char i = 1; i < 6; i++) {
                    Uptime[i] = TransitionPoint(6, 0, 0, {i});
                    Downtime[i] = TransitionPoint(22, 0, 0, {i});
                }
                Uptime[0] = TransitionPoint(7, 0, 0, {0});
                Uptime[6] = TransitionPoint(7, 0, 0, {6});
                Downtime[0] = TransitionPoint(23, 0, 0, {0});
                Downtime[6] = TransitionPoint(23, 0, 0, {6});

                Timing::mtime.update();
            }

            int main() {
                npp::Button exit = npp::Button(Win.gposy() + 4, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                npp::Button charge = npp::Button(Win.gposy() + 12, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                npp::Button settings = npp::Button(Win.gposy() + 20, Win.gposx() + Win.gdimx() - 14, 7, 9, {M1_CLICK});
                
                Win.dbox();

                int ch, state;
                while (true) {
                    rMainMenu();
                    state = update();
                    
                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {
                        if (npp::Mouse.gmouse(ch)) {
                            if (exit.cclick() == M1_CLICK) {return npp::end(true);}
                            if (charge.cclick() == M1_CLICK) {IsCharging = !IsCharging;}
                            if (settings.cclick() == M1_CLICK) {
                                settingsMenu();
                                Win.reset();
                                Win.dbox();
                            }
                        }
                    } else {
                        if (ch == 'q') {return -1;}
                    }
                }
            }
    };
}

#endif
