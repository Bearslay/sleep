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
            void acknowledgeMain(Timing::TimePoint t = Timing::mtime) {MainTime.acknowledge(t);}

            bool checkBuffer(Timing::TimePoint comparison = Timing::mtime) {return BufferTime.check(comparison);}
            void acknowledgeBuffer(Timing::TimePoint t = Timing::mtime) {BufferTime.acknowledge(t);}

            bool checkWarning(Timing::TimePoint comparison = Timing::mtime) {return WarningTime.check(comparison);}
            void acknowledgeWarning(Timing::TimePoint t = Timing::mtime) {WarningTime.acknowledge(t);}

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
                                Win.reset();
                                Win.dbox();
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
                Win.wmstr(23, 40, uptime ? (transState == TRANS_MAIN ? Uptime[day].getMainTime(Use24Hr) : (transState == TRANS_BUFFER ? Uptime[day].getBufferTime(Use24Hr) : Uptime[day].getWarningTime(Use24Hr))) : (transState == TRANS_MAIN ? Downtime[day].getMainTime(Use24Hr) : (transState == TRANS_BUFFER ? Downtime[day].getBufferTime(Use24Hr) : Downtime[day].getWarningTime(Use24Hr))), MTEXT_8x8);
            
                // Add Hour
                Win.dbox(17, 40, 5, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_LIME);
                Win.wmstr(18, 43, "^", MTEXT_6x6, NPP_LIME);
                // Add Minute
                Win.dbox(17, 55, 5, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_LIME);
                Win.wmstr(18, 58, "^", MTEXT_6x6, NPP_LIME);
                // Add Second
                Win.dbox(17, 70, 5, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_LIME);
                Win.wmstr(18, 73, "^", MTEXT_6x6, NPP_LIME);

                // Remove Hour
                Win.dbox(28, 40, 5, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_RED);
                Win.wmstr(29, 43, "v", MTEXT_6x6, NPP_RED);
                // Remove Minute
                Win.dbox(28, 55, 5, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_RED);
                Win.wmstr(29, 58, "v", MTEXT_6x6, NPP_RED);
                // Remove Second
                Win.dbox(28, 70, 5, 9, {LIGHT_SOFT, DASHED_NONE}, NPP_RED);
                Win.wmstr(29, 73, "v", MTEXT_6x6, NPP_RED);
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

                npp::Button weekday[7];
                for (unsigned char i = 0; i < 7; i++) {weekday[i] = npp::Button(Win.gposy() + Win.gdimy() - 10, Win.gposx() + 4 + i * 16, 7, 15, {M1_CLICK});}

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

            int update() {
                Timing::mtime.update(true);

                if (Uptime[Timing::mtime.getWeekdayNum()].checkMain()) {

                }

                return 0;
            }

            bool readData() {
                std::ifstream file;
                file.open("data.txt", std::ios::in);

                if (!file.is_open()) {return false;}

                std::string line;
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
                
                return true;
            }

            bool writeData() {
                std::ofstream file;
                file.open("data.txt", std::ios::out);

                if (!file.is_open()) {return false;}

                for (unsigned char i = 0; i < 7; i++) {file << Uptime[i].getMainTime() + ";" + (Uptime[i].getMainActivity() ? "1" : "0") + "|" + Uptime[i].getBufferTime() + ";" + (Uptime[i].getBufferActivity() ? "1" : "0") + "|" + Uptime[i].getWarningTime() + ";" + (Uptime[i].getWarningActivity() ? "1" : "0") + "\n";}
                for (unsigned char i = 0; i < 7; i++) {file << Downtime[i].getMainTime() + ";" + (Downtime[i].getMainActivity() ? "1" : "0") + "|" + Downtime[i].getBufferTime() + ";" + (Downtime[i].getBufferActivity() ? "1" : "0") + "|" + Downtime[i].getWarningTime() + ";" + (Downtime[i].getWarningActivity() ? "1" : "0") + "\n";}

                return true;
            }

        public:
            Pi() {
                Win = npp::Window(LINES / 2 - 45 / 2, COLS / 2 - 60, 45, 120);

                if (!readData()) {
                    // Some kind of popup about not being able to retrieve data
                }

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
