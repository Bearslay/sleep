#ifndef SLEEP
#define SLEEP

#include <fstream>
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
    std::string tupper(std::string input) {
        for (unsigned int i = 0; i < input.length(); i++) {
            input[i] = toupper(input[i]);
        }
        return input;
    }
    std::string tlower(std::string input) {
        for (unsigned int i = 0; i < input.length(); i++) {
            input[i] = tolower(input[i]);
        }
        return input;
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

            void toggleMain() {MainTime.toggle();}
            void activateMain() {MainTime.switchOn();}
            void deactivateMain() {MainTime.switchOff();}
            bool getMainActivity() {return MainTime.getActivity();}

            void toggleBuffer() {BufferTime.toggle();}
            void activateBuffer() {BufferTime.switchOn();}
            void deactivateBuffer() {BufferTime.switchOff();}
            bool getBufferActivity() {return BufferTime.getActivity();}

            void toggleWarning() {WarningTime.toggle();}
            void activateWarning() {WarningTime.switchOn();}
            void deactivateWarning() {WarningTime.switchOff();}
            bool getWarningActivity() {return WarningTime.getActivity();}

            bool checkMain(Timing::TimePoint comparison = Timing::mtime) {return MainTime.check(comparison);}
            void acknowledgeMain() {MainTime.acknowledge();}
            void unacknowledgeMain() {MainTime.unacknowledge();}

            bool checkBuffer(Timing::TimePoint comparison = Timing::mtime) {return BufferTime.check(comparison);}
            void acknowledgeBuffer() {BufferTime.acknowledge();}
            void unacknowledgeBuffer() {BufferTime.unacknowledge();}

            bool checkWarning(Timing::TimePoint comparison = Timing::mtime) {return WarningTime.check(comparison);}
            void acknowledgeWarning() {WarningTime.acknowledge();}
            void unacknowledgeWarning() {WarningTime.unacknowledge();}

            bool setMainHour(unsigned char hour) {return MainTime.setHour(hour);}
            bool setMainMinute(unsigned char minute) {return MainTime.setMinute(minute);}
            bool setMainSecond(unsigned char second) {return MainTime.setSecond(second);}

            bool setBufferHour(unsigned char hour) {return BufferTime.setHour(hour);}
            bool setBufferMinute(unsigned char minute) {return BufferTime.setMinute(minute);}
            bool setBufferSecond(unsigned char second) {return BufferTime.setSecond(second);}

            bool setWarningHour(unsigned char hour) {return WarningTime.setHour(hour);}
            bool setWarningMinute(unsigned char minute) {return WarningTime.setMinute(minute);}
            bool setWarningSecond(unsigned char second) {return WarningTime.setSecond(second);}

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

            const std::string mainHourSuffix() {return MainTime.hourSuffix();}
            const std::string bufferHourSuffix() {return BufferTime.hourSuffix();}
            const std::string warningHourSuffix() {return WarningTime.hourSuffix();}

            const std::vector<char> warningUntilMain_List() {return WarningTime.timeUntil_List(MainTime);};
            const std::vector<char> warningUntilBuffer_List() {return WarningTime.timeUntil_List(BufferTime);};
            const std::vector<char> mainUntilBuffer_List() {return MainTime.timeUntil_List(BufferTime);};
            const std::vector<char> mainUntilWarning_List() {return MainTime.timeUntil_List(WarningTime);};
            const std::vector<char> bufferUntilWarning_List() {return BufferTime.timeUntil_List(WarningTime);};
            const std::vector<char> bufferUntilMain_List() {return BufferTime.timeUntil_List(MainTime);};

            const std::vector<char> untilWarning_List(Timing::Alarm comparison) {return comparison.timeUntil_List(WarningTime);}
            const std::vector<char> untilMain_List(Timing::Alarm comparison) {return comparison.timeUntil_List(MainTime);}
            const std::vector<char> untilBuffer_List(Timing::Alarm comparison) {return comparison.timeUntil_List(BufferTime);}
    };

    class Pi {
        private:
            npp::Window Win;
            Timing::Alarm Now;
        
            TransitionPoint Uptime[7];
            TransitionPoint Downtime[7];

            bool IsCharging = false;
            bool Use24Hr = true;
            unsigned int ChargingVal = 0;

            struct {
                bool upWarning = false;
                bool upMain = false;
                bool downWarning = true;
                bool downMain = true;
            } Popups;

            void popupsAcknowledge(bool acknowledge, bool uptime, bool warning) {
                unsigned char day = Timing::mtime.getWeekdayNum();

                if (uptime) {
                    if (warning) {
                        if (acknowledge) {Uptime[day].acknowledgeWarning();}
                        else {Uptime[day].unacknowledgeWarning();}
                    } else {
                        if (acknowledge) {Uptime[day].acknowledgeMain();}
                        else {Uptime[day].unacknowledgeMain();}
                    }
                } else {
                    if (warning) {
                        if (acknowledge) {Downtime[day].acknowledgeWarning();}
                        else {Downtime[day].unacknowledgeWarning();}
                    } else {
                        if (acknowledge) {Downtime[day].acknowledgeMain();}
                        else {Downtime[day].unacknowledgeMain();}
                    }
                }
            }

            void rTransitionPopup(bool uptime, bool warning) {
                // Time
                Win.wmstr(1, 20 - (!Use24Hr ? 6 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_6x6);

                // Frame
                Win.reset(4, 10, 17, 51);
                Win.dbox(4, 10, 17, 51, {HEAVY_BOTH, DASHED_NONE});
                Win.dhline(9, 10, 51, false, {LIGHT_HARD, DASHED_TRIPLE});

                // OK Button
                Win.dbox(14, 28, 6, 15, {LIGHT_SOFT, DASHED_NONE}, NPP_LIME);
                Win.wmstr(15, 31, "OK", MTEXT_8x8, NPP_LIME);

                // Title
                Win.wmstr(5, 13, "!WARNING!", MTEXT_8x8, Now.getSecondNum() % 2 == 0 ? NPP_RED : NPP_ORANGE);

                // Status Message
                std::vector<char> untilVals;
                std::string untilStr;
                if (uptime) {
                    Win.wstrp(Win.wstrp(10, 24 + (warning ? 4 : 0), L"UPTIME "), warning ? L"IS IN:" : L"BUFFER ENDS IN:");
                    Win.wstr(12, 31 - (Use24Hr ? 0 : 1), warning ? strtowstr(Uptime[Timing::mtime.getWeekdayNum()].getMainTime(Use24Hr)) : strtowstr(Uptime[Timing::mtime.getWeekdayNum()].getBufferTime(Use24Hr)));
                    
                    untilVals = warning ? Uptime[Timing::mtime.getWeekdayNum()].untilMain_List(Now) : Uptime[Timing::mtime.getWeekdayNum()].untilBuffer_List(Now);
                } else {
                    Win.wstrp(Win.wstrp(10, 23 + (warning ? 4 : 0), L"DOWNTIME "), warning ? L"IS IN:" : L"BUFFER ENDS IN:");
                    Win.wstr(12, 31 - (Use24Hr ? 0 : 1), warning ? strtowstr(Downtime[Timing::mtime.getWeekdayNum()].getMainTime(Use24Hr)) : strtowstr(Downtime[Timing::mtime.getWeekdayNum()].getBufferTime(Use24Hr)));
                    
                    untilVals = warning ? Downtime[Timing::mtime.getWeekdayNum()].untilMain_List(Now) : Downtime[Timing::mtime.getWeekdayNum()].untilBuffer_List(Now);
                }

                untilStr = std::to_string(untilVals[0]) + " hours" + ", " + std::to_string(untilVals[1]) + " minutes" + ", " + std::to_string(untilVals[2]) + " seconds";
                Win.wstr(11, Win.gdimx() / 2 - untilStr.length() / 2, strtowstr(untilStr));
            }

            void transitionPopup(bool uptime, bool warning) {
                npp::Button ok = npp::Button(Win.gposy() + 14, Win.gposx() + 28, 6, 15, {M1_CLICK});

                int ch, state;
                while (true) {
                    rTransitionPopup(uptime, warning);
                    state = update();

                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {
                        if (npp::Mouse.gmouse(ch)) {
                            if (ok.cclick() == M1_CLICK) {
                                popupsAcknowledge(true, uptime, warning);
                                Win.reset(4, 10, 17, 51);
                                return;
                            }
                        }
                    } else {
                        if (ch == 'q') {
                            popupsAcknowledge(true, uptime, warning);
                            Win.reset(4, 10, 17, 51);
                            return;
                        }
                    }
                }
            }

            void rMainMenu() {
                // Time
                Win.wmstr(1, 20 - (!Use24Hr ? 6 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_6x6);
                Win.wstr(5, 26, strtowstr(Timing::mtime.getDateFormatted(false, true)), NPP_WHITE, "bo");

                // Exit
                Win.dbox(1, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_RED);
                Win.wmstr(2, 64, "X", MTEXT_6x6, NPP_RED);

                // Toggle Charging
                Win.dbox(6, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, IsCharging ? NPP_LIME : NPP_YELLOW);
                Win.wmstr(7, 64, "C", MTEXT_6x6, IsCharging ? NPP_LIME : NPP_YELLOW);

                // Enter Settings
                Win.dbox(11, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_LIGHT_GRAY);
                Win.wmstr(12, 64, "S", MTEXT_6x6, NPP_LIGHT_GRAY);

                // Downtime/Uptime
                Win.dbox(21, 2, 3, 67, {LIGHT_HARD, DASHED_NONE});
                Win.dvline(21, 36, 3, false, {LIGHT_HARD, DASHED_TRIPLE});
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(22, 3, L"Main Downtime ("), strtowstr(tupper(Timing::mtime.getWeekdayStr(false)))), L"): "), strtowstr(Downtime[Timing::mtime.getWeekdayNum()].getMainTime(Use24Hr)), NPP_WHITE, "bo");
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(22, 37, L"Main Uptime ("), strtowstr(tupper(Timing::mtime.getWeekdayStr(false)))), L"): "), strtowstr(Uptime[Timing::mtime.getWeekdayNum()].getMainTime(Use24Hr)), NPP_WHITE, "bo");
            
                // Popup Debugging
                for (unsigned char i = 0; i < 4; i++) {
                    Win.dbox(1 + i * 5, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, i % 2 == 0 ? NPP_ORANGE : NPP_YELLOW);
                    Win.wmstr(2 + i * 5, 4, i % 2 == 0 ? "W" : "M", MTEXT_6x6, i % 2 == 0 ? NPP_ORANGE : NPP_YELLOW);
                }
            }

            void rSettingsMain() {
                // Time
                Win.wmstr(1, 20 - (!Use24Hr ? 6 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_6x6);

                // Back
                Win.dbox(1, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_RED);
                Win.wmstr(2, 64, "X", MTEXT_6x6, NPP_RED);

                // Toggle Time Format
                Win.dbox(6, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, Use24Hr ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(7, 64, "F", MTEXT_6x6, Use24Hr ? NPP_CYAN : NPP_TEAL);

                // Toggle Popups
                Win.dbox(11, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_ORANGE);
                Win.wmstr(12, 64, "P", MTEXT_6x6, NPP_ORANGE);

                // Change Uptime
                Win.dbox(6, 27, 5, 19);
                Win.wstr(7, 31, L"Edit Uptime");
                Win.wstr(8, 30, L"-------------");
                Win.wstr(Win.wstrp(Win.wstrp(9, 30 - (Use24Hr ? 0 : 2), strtowstr(tupper(Timing::mtime.getWeekdayStr(false)))), Use24Hr ? L": " : L" = "), strtowstr(Uptime[Timing::mtime.getWeekdayNum()].getMainTime(Use24Hr)), NPP_WHITE, "bo");

                // Change Downtime
                Win.dbox(11, 27, 5, 19);
                Win.wstr(12, 30, L"Edit Downtime");
                Win.wstr(13, 30, L"-------------");
                Win.wstr(Win.wstrp(Win.wstrp(14, 30 - (Use24Hr ? 0 : 2), strtowstr(tupper(Timing::mtime.getWeekdayStr(false)))), Use24Hr ? L": " : L" = "), strtowstr(Downtime[Timing::mtime.getWeekdayNum()].getMainTime(Use24Hr)), NPP_WHITE, "bo");
            }

            void settingsMenu() {
                npp::Button back = npp::Button(Win.gposy() + 1, Win.gposx() + 62, 5, 7, {M1_CLICK});
                npp::Button timeFormat = npp::Button(Win.gposy() + 6, Win.gposx() + 62, 5, 7, {M1_CLICK});
                npp::Button popupToggle = npp::Button(Win.gposy() + 11, Win.gposx() + 62, 5, 7, {M1_CLICK});

                npp::Button changeUptime = npp::Button(Win.gposy() + 6, Win.gposx() + 27, 5, 19, {M1_CLICK});
                npp::Button changeDowntime = npp::Button(Win.gposy() + 11, Win.gposx() + 27, 5, 19, {M1_CLICK});

                Win.reset();
                Win.dbox();

                int ch, state;
                while (true) {
                    rSettingsMain();
                    state = update();
                    popups();

                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {
                        if (npp::Mouse.gmouse(ch)) {
                            if (back.cclick() == M1_CLICK) {
                                return;
                            } else if (timeFormat.cclick() == M1_CLICK) {
                                Win.reset();
                                Win.dbox();
                                Use24Hr = !Use24Hr;
                            } else if (popupToggle.cclick() == M1_CLICK) {
                                settingsPopupToggle();
                                Win.reset();
                                Win.dbox();
                            }
                            else if (changeUptime.cclick() == M1_CLICK) {
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
                Win.wmstr(1, 20 - (!Use24Hr ? 6 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_6x6);

                // Back
                Win.dbox(1, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_RED);
                Win.wmstr(2, 64, "X", MTEXT_6x6, NPP_RED);
                // Copy Time
                Win.dbox(6, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_ORANGE);
                Win.wmstr(7, 64, "C", MTEXT_6x6, NPP_ORANGE);
                // Paste Time
                Win.dbox(11, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_ORANGE);
                Win.wmstr(12, 64, "P", MTEXT_6x6, NPP_ORANGE);
                // Sync Days
                Win.dbox(16, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_YELLOW);
                Win.wmstr(17, 64, "D", MTEXT_6x6, NPP_YELLOW);

                // Change Main Time
                Win.dbox(1, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, transState == TRANS_MAIN ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(2, 4, "M", MTEXT_6x6, transState == TRANS_MAIN ? NPP_CYAN : NPP_TEAL);
                // Change Buffer Time
                Win.dbox(6, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, transState == TRANS_BUFFER ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(7, 4, "B", MTEXT_6x6, transState == TRANS_BUFFER ? NPP_CYAN : NPP_TEAL);
                // Change Warning Time
                Win.dbox(11, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, transState == TRANS_WARNING ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(12, 4, "W", MTEXT_6x6, transState == TRANS_WARNING ? NPP_CYAN : NPP_TEAL);
                // Sync Times
                Win.dbox(16, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_BLUE);
                Win.wmstr(17, 4, "T", MTEXT_6x6, NPP_BLUE);

                // Weekdays
                for (unsigned char i = 0; i < 7; i++) {
                    Win.dbox(21, 5 + i * 9, 3, 7, {HEAVY_BOTH, DASHED_NONE}, i == day ? NPP_LIGHT_GRAY : NPP_DARK_GRAY);
                    Win.wstr(22, 7 + i * 9, strtowstr(tupper(Timing::Keys.weekdayNamesAbb[i])), i == day ? NPP_LIGHT_GRAY : NPP_DARK_GRAY, "bo");
                }

                // Editing Box
                Win.dbox(4, 10, 14, 51);
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(5, 21 - transState, L"Editing: ", NPP_WHITE, "bo"), uptime ? L"Uptime (" : L"Downtime (", NPP_WHITE, "bo"), transState == TRANS_MAIN ? L"Main" : (transState == TRANS_BUFFER ? L"Buffer" : L"Warning"), NPP_WHITE, "bo"), L" Time)", NPP_WHITE, "bo");
                
                // Information Box
                Win.dbox(18, 10, 3, 51);
                Win.dvline(18, 26, 3, false, {LIGHT_HARD, DASHED_TRIPLE});
                Win.dvline(18, 43, 3, false, {LIGHT_HARD, DASHED_TRIPLE});
                Win.wstr(Win.wstrp(19, 11, L"M: "), strtowstr(uptime ? Uptime[day].getMainTime(Use24Hr) : Downtime[day].getMainTime(Use24Hr)));
                Win.wstr(Win.wstrp(19, 28, L"B: "), strtowstr(uptime ? Uptime[day].getBufferTime(Use24Hr) : Downtime[day].getBufferTime(Use24Hr)));
                Win.wstr(Win.wstrp(19, 45, L"W: "), strtowstr(uptime ? Uptime[day].getWarningTime(Use24Hr) : Downtime[day].getWarningTime(Use24Hr)));

                // Current Transition Time
                Win.wmstr(10, 20, uptime ? (transState == TRANS_MAIN ? Uptime[day].getMainTime(Use24Hr).substr(0, 8) : (transState == TRANS_BUFFER ? Uptime[day].getBufferTime(Use24Hr).substr(0, 8) : Uptime[day].getWarningTime(Use24Hr).substr(0, 8))) : (transState == TRANS_MAIN ? Downtime[day].getMainTime(Use24Hr).substr(0, 8) : (transState == TRANS_BUFFER ? Downtime[day].getBufferTime(Use24Hr).substr(0, 8) : Downtime[day].getWarningTime(Use24Hr).substr(0, 8))), MTEXT_6x6);
                if (!Use24Hr) {
                    Win.dbox(10, 53, 3, 6, {HEAVY_BOTH, DASHED_NONE}, NPP_TEAL);
                    Win.wstr(11, 55, strtowstr(uptime ? (transState == TRANS_MAIN ? Uptime[day].mainHourSuffix() : (transState == TRANS_BUFFER ? Uptime[day].bufferHourSuffix() : Uptime[day].warningHourSuffix())) : (transState == TRANS_MAIN ? Downtime[day].mainHourSuffix() : (transState == TRANS_BUFFER ? Downtime[day].bufferHourSuffix() : Downtime[day].warningHourSuffix()))), NPP_TEAL, "bo");
                }

                // Add Hour
                Win.wmstr(7, 22, "^", MTEXT_6x6, NPP_LIME);
                // Add Minute
                Win.wmstr(7, 34, "^", MTEXT_6x6, NPP_LIME);
                // Add Second
                Win.wmstr(7, 46, "^", MTEXT_6x6, NPP_LIME);

                // Remove Hour
                Win.wmstr(13, 22, "v", MTEXT_6x6, NPP_RED);
                // Remove Minute
                Win.wmstr(13, 34, "v", MTEXT_6x6, NPP_RED);
                // Remove Second
                Win.wmstr(13, 46, "v", MTEXT_6x6, NPP_RED);
            }

            void settingsTransition(bool uptime) {
                npp::Button back = npp::Button(Win.gposy() + 1, Win.gposx() + 62, 5, 7, {M1_CLICK});
                npp::Button copyTime = npp::Button(Win.gposy() + 6, Win.gposx() + 62, 5, 7, {M1_CLICK});
                npp::Button pasteTime = npp::Button(Win.gposy() + 11, Win.gposx() + 62, 5, 7, {M1_CLICK});
                npp::Button syncDays = npp::Button(Win.gposy() + 16, Win.gposx() + 62, 5, 7, {M1_CLICK});
                
                npp::Button editMain = npp::Button(Win.gposy() + 1, Win.gposx() + 2, 5, 7, {M1_CLICK});
                npp::Button editBuffer = npp::Button(Win.gposy() + 6, Win.gposx() + 2, 5, 7, {M1_CLICK});
                npp::Button editWarning = npp::Button(Win.gposy() + 11, Win.gposx() + 2, 5, 7, {M1_CLICK});
                npp::Button syncTimes = npp::Button(Win.gposy() + 16, Win.gposx() + 2, 5, 7, {M1_CLICK});
                
                npp::Button addHour = npp::Button(Win.gposy() + 6, Win.gposx() + 20, 5, 7, {M1_CLICK});
                npp::Button addMinute = npp::Button(Win.gposy() + 6, Win.gposx() + 32, 5, 7, {M1_CLICK});
                npp::Button addSecond = npp::Button(Win.gposy() + 6, Win.gposx() + 44, 5, 7, {M1_CLICK});
                npp::Button removeHour = npp::Button(Win.gposy() + 12, Win.gposx() + 20, 5, 7, {M1_CLICK});
                npp::Button removeMinute = npp::Button(Win.gposy() + 12, Win.gposx() + 32, 5, 7, {M1_CLICK});
                npp::Button removeSecond = npp::Button(Win.gposy() + 12, Win.gposx() + 44, 5, 7, {M1_CLICK});

                npp::Button weekday[7];
                for (unsigned char i = 0; i < 7; i++) {weekday[i] = npp::Button(Win.gposy() + 21, Win.gposx() + 5 + i * 9, 3, 7, {M1_CLICK});}

                Win.reset();
                Win.dbox();

                int ch, state;
                unsigned char transState = TRANS_MAIN, day = Timing::mtime.getWeekdayNum();
                unsigned char copyHour, copyMinute, copySecond;
                while (true) {
                    rSettingsTransition(uptime, transState, day);
                    state = update();

                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {
                        if (npp::Mouse.gmouse(ch)) {
                            if (back.cclick() == M1_CLICK) {
                                return;
                            } else if (syncDays.cclick() == M1_CLICK) {
                                TransitionPoint temp = uptime ? Uptime[day] : Downtime[day];
                                
                                for (unsigned char i = 0; i < 7; i++) {
                                    if (uptime) {
                                        Uptime[i] = temp;
                                    } else {
                                        Downtime[i] = temp;
                                    }
                                }
                            } else if (editMain.cclick() == M1_CLICK && transState != TRANS_MAIN) {
                                transState = TRANS_MAIN;
                                Win.reset();
                                Win.dbox();
                            } else if (editBuffer.cclick() == M1_CLICK && transState != TRANS_BUFFER) {
                                transState = TRANS_BUFFER;
                                Win.reset();
                                Win.dbox();
                            } else if (editWarning.cclick() == M1_CLICK && transState != TRANS_WARNING) {
                                transState = TRANS_WARNING;
                                Win.reset();
                                Win.dbox();
                            }
                            // Everything else here is specific to the Transition State, so has been split into the three possible states
                            else {
                                if (transState == TRANS_MAIN) {
                                    if (syncTimes.cclick() == M1_CLICK) {
                                        unsigned char hour = uptime ? Uptime[day].getMainHourNum() : Downtime[day].getMainHourNum();
                                        unsigned char minute = uptime ? Uptime[day].getMainMinuteNum() : Downtime[day].getMainMinuteNum();
                                        unsigned char second = uptime ? Uptime[day].getMainSecondNum() : Downtime[day].getMainSecondNum();

                                        if (uptime) {
                                            Uptime[day].setBufferHour(hour);
                                            Uptime[day].setWarningHour(hour);
                                            Uptime[day].setBufferMinute(minute);
                                            Uptime[day].setWarningMinute(minute);
                                            Uptime[day].setBufferSecond(second);
                                            Uptime[day].setWarningSecond(second);
                                        } else {
                                            Downtime[day].setBufferHour(hour);
                                            Downtime[day].setWarningHour(hour);
                                            Downtime[day].setBufferMinute(minute);
                                            Downtime[day].setWarningMinute(minute);
                                            Downtime[day].setBufferSecond(second);
                                            Downtime[day].setWarningSecond(second);
                                        }
                                    } else if (addHour.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setMainHour(Uptime[day].getMainHourNum() == 23 ? 0 : Uptime[day].getMainHourNum() + 1);}
                                        else {Downtime[day].setMainHour(Downtime[day].getMainHourNum() == 23 ? 0 : Downtime[day].getMainHourNum() + 1);}
                                    } else if (addMinute.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setMainMinute(Uptime[day].getMainMinuteNum() == 59 ? 0 : Uptime[day].getMainMinuteNum() + 1);}
                                        else {Downtime[day].setMainMinute(Downtime[day].getMainMinuteNum() == 59 ? 0 : Downtime[day].getMainMinuteNum() + 1);}
                                    } else if (addSecond.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setMainSecond(Uptime[day].getMainSecondNum() == 59 ? 0 : Uptime[day].getMainSecondNum() + 1);}
                                        else {Downtime[day].setMainSecond(Downtime[day].getMainSecondNum() == 59 ? 0 : Downtime[day].getMainSecondNum() + 1);}
                                    } else if (removeHour.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setMainHour(Uptime[day].getMainHourNum() == 0 ? 23 : Uptime[day].getMainHourNum() - 1);}
                                        else {Downtime[day].setMainHour(Downtime[day].getMainHourNum() == 0 ? 23 : Downtime[day].getMainHourNum() - 1);}
                                    } else if (removeMinute.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setMainMinute(Uptime[day].getMainMinuteNum() == 0 ? 59 : Uptime[day].getMainMinuteNum() - 1);}
                                        else {Downtime[day].setMainMinute(Downtime[day].getMainMinuteNum() == 0 ? 59 : Downtime[day].getMainMinuteNum() - 1);}
                                    } else if (removeSecond.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setMainSecond(Uptime[day].getMainSecondNum() == 0 ? 59 : Uptime[day].getMainSecondNum() - 1);}
                                        else {Downtime[day].setMainSecond(Downtime[day].getMainSecondNum() == 0 ? 59 : Downtime[day].getMainSecondNum() - 1);}
                                    } else if (copyTime.cclick() == M1_CLICK) {
                                        copyHour = uptime ? Uptime[day].getMainHourNum() : Downtime[day].getMainHourNum();
                                        copyMinute = uptime ? Uptime[day].getMainMinuteNum() : Downtime[day].getMainMinuteNum();
                                        copySecond = uptime ? Uptime[day].getMainSecondNum() : Downtime[day].getMainSecondNum();
                                    } else if (pasteTime.cclick() == M1_CLICK) {
                                        if (uptime) {
                                            Uptime[day].setMainHour(copyHour);
                                            Uptime[day].setMainMinute(copyMinute);
                                            Uptime[day].setMainSecond(copySecond);
                                        } else {
                                            Downtime[day].setMainHour(copyHour);
                                            Downtime[day].setMainMinute(copyMinute);
                                            Downtime[day].setMainSecond(copySecond);
                                        }
                                    }
                                } else if (transState == TRANS_BUFFER) {
                                    if (syncTimes.cclick() == M1_CLICK) {
                                        unsigned char hour = uptime ? Uptime[day].getBufferHourNum() : Downtime[day].getBufferHourNum();
                                        unsigned char minute = uptime ? Uptime[day].getBufferMinuteNum() : Downtime[day].getBufferMinuteNum();
                                        unsigned char second = uptime ? Uptime[day].getBufferSecondNum() : Downtime[day].getBufferSecondNum();

                                        if (uptime) {
                                            Uptime[day].setMainHour(hour);
                                            Uptime[day].setWarningHour(hour);
                                            Uptime[day].setMainMinute(minute);
                                            Uptime[day].setWarningMinute(minute);
                                            Uptime[day].setMainSecond(second);
                                            Uptime[day].setWarningSecond(second);
                                        } else {
                                            Downtime[day].setMainHour(hour);
                                            Downtime[day].setWarningHour(hour);
                                            Downtime[day].setMainMinute(minute);
                                            Downtime[day].setWarningMinute(minute);
                                            Downtime[day].setMainSecond(second);
                                            Downtime[day].setWarningSecond(second);
                                        }
                                    } else if (addHour.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setBufferHour(Uptime[day].getBufferHourNum() == 23 ? 0 : Uptime[day].getBufferHourNum() + 1);}
                                        else {Downtime[day].setBufferHour(Downtime[day].getBufferHourNum() == 23 ? 0 : Downtime[day].getBufferHourNum() + 1);}
                                    } else if (addMinute.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setBufferMinute(Uptime[day].getBufferMinuteNum() == 59 ? 0 : Uptime[day].getBufferMinuteNum() + 1);}
                                        else {Downtime[day].setBufferMinute(Downtime[day].getBufferMinuteNum() == 59 ? 0 : Downtime[day].getBufferMinuteNum() + 1);}
                                    } else if (addSecond.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setBufferSecond(Uptime[day].getBufferSecondNum() == 59 ? 0 : Uptime[day].getBufferSecondNum() + 1);}
                                        else {Downtime[day].setBufferSecond(Downtime[day].getBufferSecondNum() == 59 ? 0 : Downtime[day].getBufferSecondNum() + 1);}
                                    } else if (removeHour.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setBufferHour(Uptime[day].getBufferHourNum() == 0 ? 23 : Uptime[day].getBufferHourNum() - 1);}
                                        else {Downtime[day].setBufferHour(Downtime[day].getBufferHourNum() == 0 ? 23 : Downtime[day].getBufferHourNum() - 1);}
                                    } else if (removeMinute.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setBufferMinute(Uptime[day].getBufferMinuteNum() == 0 ? 59 : Uptime[day].getBufferMinuteNum() - 1);}
                                        else {Downtime[day].setBufferMinute(Downtime[day].getBufferMinuteNum() == 0 ? 59 : Downtime[day].getBufferMinuteNum() - 1);}
                                    } else if (removeSecond.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setBufferSecond(Uptime[day].getBufferSecondNum() == 0 ? 59 : Uptime[day].getBufferSecondNum() - 1);}
                                        else {Downtime[day].setBufferSecond(Downtime[day].getBufferSecondNum() == 0 ? 59 : Downtime[day].getBufferSecondNum() - 1);}
                                    } else if (copyTime.cclick() == M1_CLICK) {
                                        copyHour = uptime ? Uptime[day].getBufferHourNum() : Downtime[day].getBufferHourNum();
                                        copyMinute = uptime ? Uptime[day].getBufferMinuteNum() : Downtime[day].getBufferMinuteNum();
                                        copySecond = uptime ? Uptime[day].getBufferSecondNum() : Downtime[day].getBufferSecondNum();
                                    } else if (pasteTime.cclick() == M1_CLICK) {
                                        if (uptime) {
                                            Uptime[day].setBufferHour(copyHour);
                                            Uptime[day].setBufferMinute(copyMinute);
                                            Uptime[day].setBufferSecond(copySecond);
                                        } else {
                                            Downtime[day].setBufferHour(copyHour);
                                            Downtime[day].setBufferMinute(copyMinute);
                                            Downtime[day].setBufferSecond(copySecond);
                                        }
                                    }
                                } else {
                                    if (syncTimes.cclick() == M1_CLICK) {
                                        unsigned char hour = uptime ? Uptime[day].getWarningHourNum() : Downtime[day].getWarningHourNum();
                                        unsigned char minute = uptime ? Uptime[day].getWarningMinuteNum() : Downtime[day].getWarningMinuteNum();
                                        unsigned char second = uptime ? Uptime[day].getWarningSecondNum() : Downtime[day].getWarningSecondNum();

                                        if (uptime) {
                                            Uptime[day].setMainHour(hour);
                                            Uptime[day].setBufferHour(hour);
                                            Uptime[day].setMainMinute(minute);
                                            Uptime[day].setBufferMinute(minute);
                                            Uptime[day].setMainSecond(second);
                                            Uptime[day].setBufferSecond(second);
                                        } else {
                                            Downtime[day].setMainHour(hour);
                                            Downtime[day].setBufferHour(hour);
                                            Downtime[day].setMainMinute(minute);
                                            Downtime[day].setBufferMinute(minute);
                                            Downtime[day].setMainSecond(second);
                                            Downtime[day].setBufferSecond(second);
                                        }
                                    } else if (addHour.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setWarningHour(Uptime[day].getWarningHourNum() == 23 ? 0 : Uptime[day].getWarningHourNum() + 1);}
                                        else {Downtime[day].setWarningHour(Downtime[day].getWarningHourNum() == 23 ? 0 : Downtime[day].getWarningHourNum() + 1);}
                                    } else if (addMinute.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setWarningMinute(Uptime[day].getWarningMinuteNum() == 59 ? 0 : Uptime[day].getWarningMinuteNum() + 1);}
                                        else {Downtime[day].setWarningMinute(Downtime[day].getWarningMinuteNum() == 59 ? 0 : Downtime[day].getWarningMinuteNum() + 1);}
                                    } else if (addSecond.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setWarningSecond(Uptime[day].getWarningSecondNum() == 59 ? 0 : Uptime[day].getWarningSecondNum() + 1);}
                                        else {Downtime[day].setWarningSecond(Downtime[day].getWarningSecondNum() == 59 ? 0 : Downtime[day].getWarningSecondNum() + 1);}
                                    } else if (removeHour.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setWarningHour(Uptime[day].getWarningHourNum() == 0 ? 23 : Uptime[day].getWarningHourNum() - 1);}
                                        else {Downtime[day].setWarningHour(Downtime[day].getWarningHourNum() == 0 ? 23 : Downtime[day].getWarningHourNum() - 1);}
                                    } else if (removeMinute.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setWarningMinute(Uptime[day].getWarningMinuteNum() == 0 ? 59 : Uptime[day].getWarningMinuteNum() - 1);}
                                        else {Downtime[day].setWarningMinute(Downtime[day].getWarningMinuteNum() == 0 ? 59 : Downtime[day].getWarningMinuteNum() - 1);}
                                    } else if (removeSecond.cclick() == M1_CLICK) {
                                        if (uptime) {Uptime[day].setWarningSecond(Uptime[day].getWarningSecondNum() == 0 ? 59 : Uptime[day].getWarningSecondNum() - 1);}
                                        else {Downtime[day].setWarningSecond(Downtime[day].getWarningSecondNum() == 0 ? 59 : Downtime[day].getWarningSecondNum() - 1);}
                                    } else if (copyTime.cclick() == M1_CLICK) {
                                        copyHour = uptime ? Uptime[day].getWarningHourNum() : Downtime[day].getWarningHourNum();
                                        copyMinute = uptime ? Uptime[day].getWarningMinuteNum() : Downtime[day].getWarningMinuteNum();
                                        copySecond = uptime ? Uptime[day].getWarningSecondNum() : Downtime[day].getWarningSecondNum();
                                    } else if (pasteTime.cclick() == M1_CLICK) {
                                        if (uptime) {
                                            Uptime[day].setWarningHour(copyHour);
                                            Uptime[day].setWarningMinute(copyMinute);
                                            Uptime[day].setWarningSecond(copySecond);
                                        } else {
                                            Downtime[day].setWarningHour(copyHour);
                                            Downtime[day].setWarningMinute(copyMinute);
                                            Downtime[day].setWarningSecond(copySecond);
                                        }
                                    }
                                }
                            }

                            // Switch the weekday to edit
                            for (unsigned char i = 0; i < 7; i++) {
                                if (weekday[i].cclick() == M1_CLICK) {day = i;}
                            }
                        }
                    } else {
                        if (ch == 'q') {return;}
                    }
                }
            }

            void rSettingsPopupToggle() {
                // Time
                Win.wmstr(1, 20 - (!Use24Hr ? 6 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_6x6);

                // Back
                Win.dbox(1, 62, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_RED);
                Win.wmstr(2, 64, "X", MTEXT_6x6, NPP_RED);

                // Titles
                Win.wmstr(5, 10, "TOGGLE POPUPS", MTEXT_6x6);
                Win.wmstr(10, 8, "UPTIME", MTEXT_6x6);
                Win.wmstr(10, 36, "DOWNTIME", MTEXT_6x6);

                // Box
                Win.dbox(9, 2, 15, 67, {LIGHT_HARD, DASHED_NONE});

                // Buttons
                Win.dbox(13, 4, 5, 31, {LIGHT_SOFT, DASHED_NONE}, Popups.upWarning ? NPP_LIME : NPP_RED);
                Win.wmstr(14, 6, "WARNING", MTEXT_6x6, Popups.upWarning ? NPP_LIME : NPP_RED);
                Win.dbox(18, 4, 5, 31, {LIGHT_SOFT, DASHED_NONE}, Popups.upMain ? NPP_LIME : NPP_RED);
                Win.wmstr(19, 12, "MAIN", MTEXT_6x6, Popups.upMain ? NPP_LIME : NPP_RED);
                Win.dbox(13, 36, 5, 31, {LIGHT_SOFT, DASHED_NONE}, Popups.downWarning ? NPP_LIME : NPP_RED);
                Win.wmstr(14, 38, "WARNING", MTEXT_6x6, Popups.downWarning ? NPP_LIME : NPP_RED);
                Win.dbox(18, 36, 5, 31, {LIGHT_SOFT, DASHED_NONE}, Popups.downMain ? NPP_LIME : NPP_RED);
                Win.wmstr(19, 44, "MAIN", MTEXT_6x6, Popups.downMain ? NPP_LIME : NPP_RED);
            }

            void settingsPopupToggle() {
                npp::Button back = npp::Button(Win.gposy() + 1, Win.gposx() + 62, 5, 7, {M1_CLICK});
                npp::Button upWarning = npp::Button(Win.gposy() + 13, Win.gposx() + 4, 5, 31, {M1_CLICK});
                npp::Button upMain = npp::Button(Win.gposy() + 18, Win.gposx() + 4, 5, 31, {M1_CLICK});
                npp::Button downWarning = npp::Button(Win.gposy() + 13, Win.gposx() + 36, 5, 31, {M1_CLICK});
                npp::Button downMain = npp::Button(Win.gposy() + 18, Win.gposx() + 36, 5, 31, {M1_CLICK});

                Win.reset();
                Win.dbox();

                int ch, state;
                while (true) {
                    rSettingsPopupToggle();
                    state = update();
                    popups();

                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {
                        if (npp::Mouse.gmouse(ch)) {
                            if (back.cclick() == M1_CLICK) {
                                return;
                            } else if (upWarning.cclick() == M1_CLICK) {
                                Popups.upWarning = !Popups.upWarning;
                            } else if (upMain.cclick() == M1_CLICK) {
                                Popups.upMain = !Popups.upMain;
                            } else if (downWarning.cclick() == M1_CLICK) {
                                Popups.downWarning = !Popups.downWarning;
                            } else if (downMain.cclick() == M1_CLICK) {
                                Popups.downMain = !Popups.downMain;
                            }
                        }
                    } else {
                        if (ch == 'q') {
                            return;
                        }
                    }
                }
            }

            void popups() {
                unsigned char day = Timing::mtime.getWeekdayNum();

                if (Popups.upWarning && Uptime[day].checkWarning()) {
                    transitionPopup(true, true);
                } else if (Popups.upMain && Uptime[day].checkMain()) {
                    transitionPopup(true, false);
                } else if (Popups.downWarning && Downtime[day].checkWarning()) {
                    transitionPopup(false, true);
                } else if (Popups.downMain && Downtime[day].checkMain()) {
                    transitionPopup(false, false);
                }
            }

            int update() {
                Timing::mtime.update(true);
                Now = Timing::Alarm(Timing::mtime.getHourNum(), Timing::mtime.getMinuteNum(), Timing::mtime.getSecondNum());

                if (IsCharging) {

                }

                return 0;
            }

            bool readData() {
                std::ifstream file;
                std::string line;

                file.open("data.txt", std::ios::in);
                if (!file.is_open()) {return false;}

                for (unsigned char i = 0; i < 7; i++) {
                    std::getline(file, line);

                    Uptime[i] = TransitionPoint(0, 0, 0, {i});
                    Uptime[i].setMainHour(stoi(line.substr(0, 2)));
                    Uptime[i].setMainMinute(stoi(line.substr(3, 2)));
                    Uptime[i].setMainSecond(stoi(line.substr(6, 2)));
                    Uptime[i].setBufferHour(stoi(line.substr(11, 2)));
                    Uptime[i].setBufferMinute(stoi(line.substr(14, 2)));
                    Uptime[i].setBufferSecond(stoi(line.substr(17, 2)));
                    Uptime[i].setWarningHour(stoi(line.substr(22, 2)));
                    Uptime[i].setWarningMinute(stoi(line.substr(25, 2)));
                    Uptime[i].setWarningSecond(stoi(line.substr(28, 2)));

                    if (line.substr(9, 1) == "1") {Uptime[i].activateMain();}
                    else {Uptime[i].deactivateMain();}
                    if (line.substr(20, 1) == "1") {Uptime[i].activateBuffer();}
                    else {Uptime[i].deactivateBuffer();}
                    if (line.substr(31, 1) == "1") {Uptime[i].activateWarning();}
                    else {Uptime[i].deactivateWarning();}
                }
                for (unsigned char i = 0; i < 7; i++) {
                    std::getline(file, line);

                    Downtime[i] = TransitionPoint(0, 0, 0, {i});
                    Downtime[i].setMainHour(stoi(line.substr(0, 2)));
                    Downtime[i].setMainMinute(stoi(line.substr(3, 2)));
                    Downtime[i].setMainSecond(stoi(line.substr(6, 2)));
                    Downtime[i].setBufferHour(stoi(line.substr(11, 2)));
                    Downtime[i].setBufferMinute(stoi(line.substr(14, 2)));
                    Downtime[i].setBufferSecond(stoi(line.substr(17, 2)));
                    Downtime[i].setWarningHour(stoi(line.substr(22, 2)));
                    Downtime[i].setWarningMinute(stoi(line.substr(25, 2)));
                    Downtime[i].setWarningSecond(stoi(line.substr(28, 2)));

                    if (line.substr(9, 1) == "1") {Downtime[i].activateMain();}
                    else {Downtime[i].deactivateMain();}
                    if (line.substr(20, 1) == "1") {Downtime[i].activateBuffer();}
                    else {Downtime[i].deactivateBuffer();}
                    if (line.substr(31, 1) == "1") {Downtime[i].activateWarning();}
                    else {Downtime[i].deactivateWarning();}
                }

                file.close();
                file.open("settings.txt", std::ios::in);
                if (!file.is_open()) {return false;}

                getline(file, line);
                Popups.upWarning = line[0] == '1' ? true : false;
                Popups.upMain = line[1] == '1' ? true : false;
                Popups.downWarning = line[2] == '1' ? true : false;
                Popups.downMain = line[3] == '1' ? true : false;

                getline(file, line);
                Use24Hr = line[0] == '1' ? true : false;

                file.close();
                return true;
            }

            bool writeData() {
                std::ofstream file;
                file.open("data.txt", std::ios::out);
                if (!file.is_open()) {return false;}

                for (unsigned char i = 0; i < 7; i++) {
                    file << Uptime[i].getMainTime() + ";" + (Uptime[i].getMainActivity() ? "1" : "0") + "|" + Uptime[i].getBufferTime() + ";" + (Uptime[i].getBufferActivity() ? "1" : "0") + "|" + Uptime[i].getWarningTime() + ";" + (Uptime[i].getWarningActivity() ? "1" : "0") + "\n";
                }
                for (unsigned char i = 0; i < 7; i++) {
                    file << Downtime[i].getMainTime() + ";" + (Downtime[i].getMainActivity() ? "1" : "0") + "|" + Downtime[i].getBufferTime() + ";" + (Downtime[i].getBufferActivity() ? "1" : "0") + "|" + Downtime[i].getWarningTime() + ";" + (Downtime[i].getWarningActivity() ? "1" : "0") + "\n";
                }

                file.close();
                file.open("settings.txt", std::ios::out);
                if (!file.is_open()) {return false;}

                file << (Popups.upWarning ? '1' : '0') << (Popups.upMain ? '1' : '0') << (Popups.downWarning ? '1' : '0') << (Popups.downMain ? '1' : '0') << "\n" << (Use24Hr ? '1' : '0');

                file.close();
                return true;
            }

        public:
            Pi() {
                Win = npp::Window(LINES / 2 - 12, COLS / 2 - 35, 25, 71);
                Now = Timing::Alarm(Timing::mtime.getHourNum(), Timing::mtime.getMinuteNum(), Timing::mtime.getSecondNum());

                if (!readData()) {
                    // Some kind of popup about not being able to retrieve data
                }

                Timing::mtime.update();
            }

            int main() {
                npp::Button exit = npp::Button(Win.gposy() + 1, Win.gposx() + 62, 5, 7, {M1_CLICK});
                npp::Button charge = npp::Button(Win.gposy() + 6, Win.gposx() + 62, 5, 7, {M1_CLICK});
                npp::Button settings = npp::Button(Win.gposy() + 11, Win.gposx() + 62, 5, 7, {M1_CLICK});
                
                npp::Button popupDebugging[4];
                for (unsigned char i = 0; i < 4; i++) {
                    popupDebugging[i] = npp::Button(Win.gposy() + 1 + i * 5, Win.gposx() + 2, 5, 7, {M1_CLICK});
                }
                
                Win.dbox();

                int ch, state;
                while (true) {
                    rMainMenu();
                    state = update();
                    
                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {
                        if (npp::Mouse.gmouse(ch)) {
                            if (exit.cclick() == M1_CLICK) {
                                if (!writeData()) {
                                    // Some kind of pop-up about not saving settings or smth
                                }
                                return -1;
                            } else if (charge.cclick() == M1_CLICK) {
                                IsCharging = !IsCharging;
                            } else if (settings.cclick() == M1_CLICK) {
                                settingsMenu();
                                Win.reset();
                                Win.dbox();
                            }
                            else if (Popups.upWarning && popupDebugging[0].cclick() == M1_CLICK) {
                                transitionPopup(true, true);
                            } else if (Popups.upMain && popupDebugging[1].cclick() == M1_CLICK) {
                                transitionPopup(true, false);
                            } else if (Popups.downWarning && popupDebugging[2].cclick() == M1_CLICK) {
                                transitionPopup(false, true);
                            } else if (Popups.downMain && popupDebugging[3].cclick() == M1_CLICK) {
                                transitionPopup(false, false);
                            }
                        }
                    } else {
                        if (ch == 'q') {
                            if (!writeData()) {
                                // Some kind of pop-up about not saving settings or smth
                            }
                            return -1;
                        }
                    }
                }
            }
    };
}

#endif
