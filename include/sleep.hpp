#ifndef SLEEPNEW
#define SLEEPNEW

#include "ncursespp.hpp"
#include "timing.hpp"
#include <fstream>

#define PERIOD_DOWNTIME 0
#define PERIOD_UPTIME 1

#define TIME_MAIN 0
#define TIME_BUFFER 1
#define TIME_WARNING 2

#define POP_UPWARN 0
#define POP_UPMAIN 1
#define POP_DOWNWARN 2
#define POP_DOWNMAIN 3

#define SCR_HOME 0
#define SCR_SETTINGS_MAIN 1
#define SCR_SETTINGS_CRITPTS 2
#define SCR_SETTINGS_POPUPS 3
#define SCR_POPUPS_CRITPTS 4

#define CHARGE_TOTAL 0
#define CHARGE_DOWNTIME 1
#define CHARGE_UPTIME 2

namespace Sleep {
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
    std::string secondsToTime(unsigned int input) {
        unsigned char hours = input / 3600;
        unsigned char minutes = (input - hours * 3600) / 60;
        unsigned char seconds = input - hours * 3600 - minutes * 60;

        return (hours < 10 ? "0" : "") + std::to_string(hours) + ":" + (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    }

    class RPi {
        private:
            npp::Window Win;
            std::vector<npp::Button> RightColumn, LeftColumn;
            
            Timing::Alarm Now, PrevSecond;
            std::vector<std::vector<std::vector<Timing::Alarm>>> CritPts;

            unsigned char Weekday = Timing::mtime.getWeekdayNum();

            bool IsCharging = false;
            bool Use24Hr = true;

            bool IsUptime;
            unsigned char Screen = SCR_HOME;

            std::vector<unsigned int> Charges = {0, 0, 0};
            std::vector<std::vector<bool>> Popups = {{false, false, true, true}, {false, false, false, false}};
            
            struct {
                bool enable = true;
                std::vector<bool> useBuffer = {true, true};

            } Settings_CritPts;

            struct {
                npp::Button changeUptime, changeDowntime;
            } Uniques_sMain;

            struct {
                std::vector<npp::Button> addTime, removeTime, weekdays;
                
                bool uptime = false;
                unsigned char time = TIME_MAIN, weekday;
                std::vector<unsigned char> copyTime;
            } Uniques_sCritPts;

            struct {
                npp::Button upWarning, upMain, downWarning, downMain;
            } Uniques_sPopups;

            struct {
                npp::Button ok;

                unsigned char oldScreen = SCR_HOME;
                bool uptime, warning;
            } Uniques_pCritPts;

            void changeScreen(unsigned char screen) {
                Screen = screen;
                Win.reset();
                Win.dbox();
            }

            void rHome() {
                // Date
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
                Win.dbox(21, 2, 3, 67, {LIGHT_HARD, DASHED_NONE}, IsUptime ? NPP_MAGENTA : NPP_PURPLE);
                Win.dvline(21, 36, 3, false, {LIGHT_HARD, DASHED_TRIPLE}, IsUptime ? NPP_MAGENTA : NPP_PURPLE);
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(22, 3, L"Main Downtime ("), strtowstr(tupper(Timing::mtime.getWeekdayStr(false)))), L"): "), strtowstr(CritPts[PERIOD_DOWNTIME][Weekday][TIME_MAIN].getTimeFormatted(Use24Hr)), NPP_WHITE, "bo");
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(22, 37, L"Main Uptime ("), strtowstr(tupper(Timing::mtime.getWeekdayStr(false)))), L"): "), strtowstr(CritPts[PERIOD_UPTIME][Weekday][TIME_MAIN].getTimeFormatted(Use24Hr)), NPP_WHITE, "bo");
            
                // Charging Amounts
                Win.dbox(16, 10, 5, 51, {LIGHT_HARD, DASHED_NONE});
                unsigned int secondsTillCritPt = CritPts[PERIOD_DOWNTIME][Weekday][TIME_MAIN].timeUntil_Seconds(CritPts[PERIOD_UPTIME][Weekday == 6 ? 0 : Weekday + 1][TIME_MAIN]);
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(17, 12, L"Charging [Total]    - "), strtowstr(secondsToTime(Charges[CHARGE_TOTAL])), NPP_WHITE, "bo"), L"   GOAL: "), strtowstr(secondsToTime(secondsTillCritPt)));
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(18, 12, L"Charging [Uptime]   - "), strtowstr(secondsToTime(Charges[CHARGE_UPTIME])), NPP_WHITE, "bo"), L"   GOAL: "), L"N/A", NPP_WHITE, "bo");
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(19, 12, L"Charging [Downtime] - "), strtowstr(secondsToTime(Charges[CHARGE_DOWNTIME])), NPP_WHITE, "bo"), L"   GOAL: "), strtowstr(secondsToTime(secondsTillCritPt)));
                Win.dvline(16, 43, 5, false, {LIGHT_HARD, DASHED_TRIPLE});

                // Popup Debugging (remove later)
                for (unsigned char i = 0; i < 4; i++) {
                    Win.dbox(1 + i * 5, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, i % 2 == 0 ? NPP_ORANGE : NPP_YELLOW);
                    Win.wmstr(2 + i * 5, 4, i % 2 == 0 ? "W" : "M", MTEXT_6x6, i % 2 == 0 ? NPP_ORANGE : NPP_YELLOW);
                }
            }
            void rSettings_Main() {
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
                Win.wstr(Win.wstrp(Win.wstrp(9, 30 - (Use24Hr ? 0 : 2), strtowstr(tupper(Timing::mtime.getWeekdayStr(false)))), Use24Hr ? L": " : L" = "), strtowstr(CritPts[PERIOD_UPTIME][Weekday][TIME_MAIN].getTimeFormatted(Use24Hr)), NPP_WHITE, "bo");

                // Change Downtime
                Win.dbox(11, 27, 5, 19);
                Win.wstr(12, 30, L"Edit Downtime");
                Win.wstr(13, 30, L"-------------");
                Win.wstr(Win.wstrp(Win.wstrp(14, 30 - (Use24Hr ? 0 : 2), strtowstr(tupper(Timing::mtime.getWeekdayStr(false)))), Use24Hr ? L": " : L" = "), strtowstr(CritPts[PERIOD_DOWNTIME][Weekday][TIME_MAIN].getTimeFormatted(Use24Hr)), NPP_WHITE, "bo");
            }
            void rSettings_CritPts() {
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
                Win.dbox(1, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, Uniques_sCritPts.time == TIME_MAIN ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(2, 4, "M", MTEXT_6x6, Uniques_sCritPts.time == TIME_MAIN ? NPP_CYAN : NPP_TEAL);
                // Change Buffer Time
                Win.dbox(6, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, Uniques_sCritPts.time == TIME_BUFFER ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(7, 4, "B", MTEXT_6x6, Uniques_sCritPts.time == TIME_BUFFER ? NPP_CYAN : NPP_TEAL);
                // Change Warning Time
                Win.dbox(11, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, Uniques_sCritPts.time == TIME_WARNING ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(12, 4, "W", MTEXT_6x6, Uniques_sCritPts.time == TIME_WARNING ? NPP_CYAN : NPP_TEAL);
                // Sync Times
                Win.dbox(16, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_BLUE);
                Win.wmstr(17, 4, "T", MTEXT_6x6, NPP_BLUE);

                // Weekdays
                for (unsigned char i = 0; i < 7; i++) {
                    Win.dbox(21, 5 + i * 9, 3, 7, {HEAVY_BOTH, DASHED_NONE}, i == Uniques_sCritPts.weekday ? NPP_LIGHT_GRAY : NPP_DARK_GRAY);
                    Win.wstr(22, 7 + i * 9, strtowstr(tupper(Timing::Keys.weekdayNamesAbb[i])), i == Uniques_sCritPts.weekday ? NPP_LIGHT_GRAY : NPP_DARK_GRAY, "bo");
                }

                // Editing Box
                Win.dbox(4, 10, 14, 51);
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(5, 21 - Uniques_sCritPts.time, L"Editing: ", NPP_WHITE, "bo"), Uniques_sCritPts.uptime ? L"Uptime (" : L"Downtime (", NPP_WHITE, "bo"), Uniques_sCritPts.time == TIME_MAIN ? L"Main" : (Uniques_sCritPts.time == TIME_BUFFER ? L"Buffer" : L"Warning"), NPP_WHITE, "bo"), L" Time)", NPP_WHITE, "bo");
                
                // Information Box
                Win.dbox(18, 10, 3, 51);
                Win.dvline(18, 26, 3, false, {LIGHT_HARD, DASHED_TRIPLE});
                Win.dvline(18, 43, 3, false, {LIGHT_HARD, DASHED_TRIPLE});
                Win.wstr(Win.wstrp(19, 11, L"M: "), strtowstr(CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][TIME_MAIN].getTimeFormatted(Use24Hr)));
                Win.wstr(Win.wstrp(19, 28, L"B: "), strtowstr(CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][TIME_BUFFER].getTimeFormatted(Use24Hr)));
                Win.wstr(Win.wstrp(19, 45, L"W: "), strtowstr(CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][TIME_WARNING].getTimeFormatted(Use24Hr)));

                // Current Transition Time
                Win.wmstr(10, 20, CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getTimeFormatted(Use24Hr).substr(0, 8), MTEXT_6x6);
                if (!Use24Hr) {
                    Win.dbox(10, 53, 3, 6, {HEAVY_BOTH, DASHED_NONE}, NPP_TEAL);
                    Win.wstr(11, 55, strtowstr(CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].hourSuffix()), NPP_TEAL, "bo");
                }

                // Add/Remove buttons
                for (unsigned char i = 0; i < 3; i++) {
                    Win.wmstr(7, 22 + i * 12, "^", MTEXT_6x6, NPP_LIME);
                    Win.wmstr(13, 22 + i * 12, "v", MTEXT_6x6, NPP_RED);
                }
            }
            void rSettings_Popups() {
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
                Win.dbox(13, 4, 5, 31, {LIGHT_SOFT, DASHED_NONE}, Popups[0][POP_UPWARN] ? NPP_LIME : NPP_RED);
                Win.wmstr(14, 6, "WARNING", MTEXT_6x6, Popups[0][POP_UPWARN] ? NPP_LIME : NPP_RED);
                Win.dbox(18, 4, 5, 31, {LIGHT_SOFT, DASHED_NONE}, Popups[0][POP_UPMAIN] ? NPP_LIME : NPP_RED);
                Win.wmstr(19, 12, "MAIN", MTEXT_6x6, Popups[0][POP_UPMAIN] ? NPP_LIME : NPP_RED);
                Win.dbox(13, 36, 5, 31, {LIGHT_SOFT, DASHED_NONE}, Popups[0][POP_DOWNWARN] ? NPP_LIME : NPP_RED);
                Win.wmstr(14, 38, "WARNING", MTEXT_6x6, Popups[0][POP_DOWNWARN] ? NPP_LIME : NPP_RED);
                Win.dbox(18, 36, 5, 31, {LIGHT_SOFT, DASHED_NONE}, Popups[0][POP_DOWNMAIN] ? NPP_LIME : NPP_RED);
                Win.wmstr(19, 44, "MAIN", MTEXT_6x6, Popups[0][POP_DOWNMAIN] ? NPP_LIME : NPP_RED);
            }
            void rPopup_CritPts() {
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
                Win.wstrp(Win.wstrp(10, 23 + (Uniques_pCritPts.warning ? 4 : 0) + (Uniques_pCritPts.uptime ? 1 : 0), Uniques_pCritPts.uptime ? L"UPTIME" : L"DOWNTIME"), Uniques_pCritPts.warning ? L" IS IN:" : L" BUFFER ENDS IN:");
                Win.wstr(12, 31 - (Use24Hr ? 0 : 1), strtowstr(CritPts[Uniques_pCritPts.uptime][Weekday][Uniques_pCritPts.warning ? TIME_WARNING : TIME_MAIN].getTimeFormatted(Use24Hr)));

                std::vector<char> untilVals = Now.timeUntil_List(CritPts[Uniques_pCritPts.uptime && Uniques_pCritPts.warning ? PERIOD_UPTIME : PERIOD_DOWNTIME][Weekday][Uniques_pCritPts.warning ? TIME_MAIN : TIME_BUFFER]);
                std::string untilStr = std::to_string(untilVals[0]) + " hours" + ", " + std::to_string(untilVals[1]) + " minutes" + ", " + std::to_string(untilVals[2]) + " seconds";
                Win.wstr(11, Win.gdimx() / 2 - untilStr.length() / 2, strtowstr(untilStr));
            }

            bool hHome() {
                if (RightColumn[0].cclick() == M1_CLICK) {
                    return false;
                } else if (RightColumn[1].cclick() == M1_CLICK) {
                    IsCharging = !IsCharging;
                } else if (RightColumn[2].cclick() == M1_CLICK) {
                    changeScreen(SCR_SETTINGS_MAIN);
                }
                // Debugging: remove later
                else if (LeftColumn[0].cclick() == M1_CLICK) {
                    Uniques_pCritPts.oldScreen = Screen;
                    Screen = SCR_POPUPS_CRITPTS;

                    Uniques_pCritPts.uptime = true;
                    Uniques_pCritPts.warning = true;
                    Popups[1][POP_UPWARN] = true;
                } else if (LeftColumn[1].cclick() == M1_CLICK) {
                    Uniques_pCritPts.oldScreen = Screen;
                    Screen = SCR_POPUPS_CRITPTS;

                    Uniques_pCritPts.uptime = true;
                    Uniques_pCritPts.warning = false;
                    Popups[1][POP_UPMAIN] = true;
                } else if (LeftColumn[2].cclick() == M1_CLICK) {
                    Uniques_pCritPts.oldScreen = Screen;
                    Screen = SCR_POPUPS_CRITPTS;

                    Uniques_pCritPts.uptime = false;
                    Uniques_pCritPts.warning = true;
                    Popups[1][POP_DOWNWARN] = true;
                } else if (LeftColumn[3].cclick() == M1_CLICK) {
                    Uniques_pCritPts.oldScreen = Screen;
                    Screen = SCR_POPUPS_CRITPTS;

                    Uniques_pCritPts.uptime = false;
                    Uniques_pCritPts.warning = false;
                    Popups[1][POP_DOWNMAIN] = true;
                }

                return true;
            }
            void hSettings_Main() {
                if (RightColumn[0].cclick() == M1_CLICK) {
                    changeScreen(SCR_HOME);
                } else if (RightColumn[1].cclick() == M1_CLICK) {
                    Use24Hr = !Use24Hr;
                    Win.reset();
                    Win.dbox();
                } else if (RightColumn[2].cclick() == M1_CLICK) {
                    changeScreen(SCR_SETTINGS_POPUPS);
                } else if (Uniques_sMain.changeUptime.cclick() == M1_CLICK) {
                    changeScreen(SCR_SETTINGS_CRITPTS);
                    Uniques_sCritPts.uptime = true;
                } else if (Uniques_sMain.changeDowntime.cclick() == M1_CLICK) {
                    changeScreen(SCR_SETTINGS_CRITPTS);
                    Uniques_sCritPts.uptime = false;
                }
            }
            void hSettings_CritPts() {
                if (RightColumn[0].cclick() == M1_CLICK) {
                    changeScreen(SCR_SETTINGS_MAIN);
                } else if (RightColumn[1].cclick() == M1_CLICK) {
                    Uniques_sCritPts.copyTime[0] = CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getHourNum();
                    Uniques_sCritPts.copyTime[1] = CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getMinuteNum();
                    Uniques_sCritPts.copyTime[2] = CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getSecondNum();
                } else if (RightColumn[2].cclick() == M1_CLICK) {
                    CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].setHour(Uniques_sCritPts.copyTime[0]);
                    CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].setMinute(Uniques_sCritPts.copyTime[1]);
                    CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].setSecond(Uniques_sCritPts.copyTime[2]);
                } else if (RightColumn[3].cclick() == M1_CLICK) {
                    Timing::Alarm source = CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time];

                    for (unsigned char i = 0; i < 7; i++) {
                        CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time] = source;
                    }
                } else if (LeftColumn[0].cclick() == M1_CLICK) {
                    Uniques_sCritPts.time = TIME_MAIN;
                    Win.reset();
                    Win.dbox();
                } else if (LeftColumn[1].cclick() == M1_CLICK) {
                    Uniques_sCritPts.time = TIME_BUFFER;
                    Win.reset();
                    Win.dbox();
                } else if (LeftColumn[2].cclick() == M1_CLICK) {
                    Uniques_sCritPts.time = TIME_WARNING;
                    Win.reset();
                    Win.dbox();
                } else if (LeftColumn[3].cclick() == M1_CLICK) {
                    Timing::Alarm source = CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time];

                    for (unsigned char i = 0; i < 3; i++) {
                        CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][i].setHour(source.getHourNum());
                        CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][i].setMinute(source.getMinuteNum());
                        CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][i].setSecond(source.getSecondNum());
                    }
                } else if (Uniques_sCritPts.addTime[0].cclick() == M1_CLICK) {
                    CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].setHour(CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getHourNum() == 23 ? 0 : CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getHourNum() + 1);
                } else if (Uniques_sCritPts.addTime[1].cclick() == M1_CLICK) {
                    CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].setMinute(CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getMinuteNum() == 59 ? 0 : CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getMinuteNum() + 1);
                } else if (Uniques_sCritPts.addTime[2].cclick() == M1_CLICK) {
                    CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].setSecond(CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getSecondNum() == 59 ? 0 : CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getSecondNum() + 1);
                } else if (Uniques_sCritPts.removeTime[0].cclick() == M1_CLICK) {
                    CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].setHour(CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getHourNum() == 0 ? 23 : CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getHourNum() - 1);
                } else if (Uniques_sCritPts.removeTime[1].cclick() == M1_CLICK) {
                    CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].setMinute(CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getMinuteNum() == 0 ? 59 : CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getMinuteNum() - 1);
                } else if (Uniques_sCritPts.removeTime[2].cclick() == M1_CLICK) {
                    CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].setSecond(CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getSecondNum() == 0 ? 59 : CritPts[Uniques_sCritPts.uptime][Uniques_sCritPts.weekday][Uniques_sCritPts.time].getSecondNum() - 1);
                }

                // Change current weekday
                for (unsigned char i = 0; i < 7; i++) {
                    if (Uniques_sCritPts.weekdays[i].cclick() == M1_CLICK) {
                        Uniques_sCritPts.weekday = i;
                        break;
                    }
                }
            }
            void hSettings_Popups() {
                if (RightColumn[0].cclick() == M1_CLICK) {
                    Screen = SCR_SETTINGS_MAIN;
                    Win.reset();
                    Win.dbox();
                } else if (Uniques_sPopups.upWarning.cclick() == M1_CLICK) {
                    Popups[0][POP_UPWARN] = !Popups[0][POP_UPWARN];
                } else if (Uniques_sPopups.upMain.cclick() == M1_CLICK) {
                    Popups[0][POP_UPMAIN] = !Popups[0][POP_UPMAIN];
                } else if (Uniques_sPopups.downWarning.cclick() == M1_CLICK) {
                    Popups[0][POP_DOWNWARN] = !Popups[0][POP_DOWNWARN];
                } else if (Uniques_sPopups.downMain.cclick() == M1_CLICK) {
                    Popups[0][POP_DOWNMAIN] = !Popups[0][POP_DOWNMAIN];
                }
            }
            void hPopups_CritPts() {
                if (Uniques_pCritPts.ok.cclick() == M1_CLICK) {
                    CritPts[Uniques_pCritPts.uptime][Weekday][Uniques_pCritPts.warning ? TIME_WARNING : TIME_MAIN].acknowledge();
                    Popups[1][(!Uniques_pCritPts.warning) + (!Uniques_pCritPts.uptime) * 2] = false;
                    Screen = Uniques_pCritPts.oldScreen;
                    Win.reset(4, 10, 17, 51);
                }
            }

            void update() {
                Timing::mtime.update(true);
                PrevSecond = Now;
                Now = Timing::Alarm(Timing::mtime.getHourNum(), Timing::mtime.getMinuteNum(), Timing::mtime.getSecondNum());
                Weekday = Timing::mtime.getWeekdayNum();

                // New day happens
                if (Now.getTimeFormatted() == "00:00:00") {
                    for (unsigned char i = 0; i < 2; i++) {
                        for (unsigned char j = 0; j < 7; j++) {
                            for (unsigned char k = 0; k < 3; k++) {
                                CritPts[i][j][k].unacknowledge();
                            }
                        }
                    }
                }

                // Switch from downtime to uptime
                if (Now.getTimeFormatted() == CritPts[PERIOD_UPTIME][Weekday][TIME_BUFFER].getTimeFormatted()) {
                    Charges[CHARGE_UPTIME] = 0;
                    Charges[CHARGE_DOWNTIME] = 0;
                    Charges[CHARGE_TOTAL] = 0;
                }

                // All of the different UIs show the time and have to constantly update it so its rendered here for convenience
                Win.wmstr(1, 20 - (!Use24Hr ? 6 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_6x6);

                // Show popups
                if (!Popups[1][POP_UPWARN] && Popups[0][POP_UPWARN] && CritPts[PERIOD_UPTIME][Weekday][TIME_WARNING].check()) {
                    Uniques_pCritPts.oldScreen = Screen;
                    Screen = SCR_POPUPS_CRITPTS;

                    Uniques_pCritPts.uptime = true;
                    Uniques_pCritPts.warning = true;
                    Popups[1][POP_UPWARN] = true;
                } else if (!Popups[1][POP_UPMAIN] && Popups[0][POP_UPMAIN] && CritPts[PERIOD_UPTIME][Weekday][TIME_MAIN].check()) {
                    Uniques_pCritPts.oldScreen = Screen;
                    Screen = SCR_POPUPS_CRITPTS;

                    Uniques_pCritPts.uptime = true;
                    Uniques_pCritPts.warning = false;
                    Popups[1][POP_UPMAIN] = true;
                } else if (!Popups[1][POP_DOWNWARN] && Popups[0][POP_DOWNWARN] && CritPts[PERIOD_DOWNTIME][Weekday][TIME_WARNING].check()) {
                    Uniques_pCritPts.oldScreen = Screen;
                    Screen = SCR_POPUPS_CRITPTS;

                    Uniques_pCritPts.uptime = false;
                    Uniques_pCritPts.warning = true;
                    Popups[1][POP_DOWNWARN] = true;
                } else if (!Popups[1][POP_DOWNMAIN] && Popups[0][POP_DOWNMAIN] && CritPts[PERIOD_DOWNTIME][Weekday][TIME_MAIN].check()) {
                    Uniques_pCritPts.oldScreen = Screen;
                    Screen = SCR_POPUPS_CRITPTS;

                    Uniques_pCritPts.uptime = false;
                    Uniques_pCritPts.warning = false;
                    Popups[1][POP_DOWNMAIN] = true;
                }

                // Switch between uptime and downtime
                if (Now.getTimeFormatted() == CritPts[PERIOD_DOWNTIME][Weekday][TIME_BUFFER].getTimeFormatted()) {
                    IsUptime = false;
                } else if (Now.getTimeFormatted() == CritPts[PERIOD_UPTIME][Weekday][TIME_BUFFER].getTimeFormatted()) {
                    IsUptime = true;
                }

                // Check charging
                if (Now.getTimeFormatted() > PrevSecond.getTimeFormatted()) {
                    if (IsCharging) {
                        Charges[CHARGE_TOTAL]++;
                        Charges[IsUptime + 1]++;
                    } else {
                        if (!IsUptime) {
                            // LOG AN INFRACTION
                        }
                    }
                }
            }

            bool readAlarms() {
                std::ifstream file;
                std::string line;

                file.open("data.txt", std::ios::in);
                if (!file.is_open()) {return false;}

                for (unsigned char i = 0; i < 2; i++) {
                    for (unsigned char j = 0; j < 7; j++) {
                        std::getline(file, line);

                        for (unsigned char k = 0; k < 3; k++) {
                            CritPts[i][j][k].setHour(stoi(line.substr(k * 11, 2)));
                            CritPts[i][j][k].setMinute(stoi(line.substr(3 + k * 11, 2)));
                            CritPts[i][j][k].setSecond(stoi(line.substr(6 + k * 11, 2)));

                            if (line.substr(9 + k * 11, 1) == "1") {CritPts[i][j][k].switchOn();}
                            else {CritPts[i][j][k].switchOff();}
                        }
                    }
                }

                file.close();
                return true;
            }
            bool readSettings() {
                std::ifstream file;
                std::string line;

                file.open("settings.txt", std::ios::in);
                if (!file.is_open()) {return false;}

                std::getline(file, line);
                for (unsigned char i = 0; i < 4; i++) {
                    Popups[0][i] = line[i] == '1' ? true : false;
                }

                std::getline(file, line);
                Use24Hr = line[0] == '1' ? true : false;

                file.close();
                return true;
            }
            bool readData() {return readAlarms() && readSettings() ? true : false;}

            bool writeAlarms() {
                std::ofstream file;
                file.open("data.txt", std::ios::out);
                if (!file.is_open()) {return false;}

                for (unsigned char i = 0; i < 2; i++) {
                    for (unsigned char j = 0; j < 7; j++) {
                        for (unsigned char k = 0; k < 3; k++) {
                            file << (k > 0 ? "|" : "") << CritPts[i][j][k].getTimeFormatted(true) + ":" + (CritPts[i][j][k].getActivity() ? "1" : "0");
                        }
                        file << "\n";
                    }
                }

                file.close();
                return true;
            }
            bool writeSettings() {
                std::ofstream file;
                file.open("settings.txt", std::ios::out);
                if (!file.is_open()) {return false;}

                file << (Popups[0][POP_UPWARN] ? '1' : '0') << (Popups[0][POP_UPMAIN] ? '1' : '0') << (Popups[0][POP_DOWNWARN] ? '1' : '0') << (Popups[0][POP_DOWNMAIN] ? '1' : '0') << "\n" << (Use24Hr ? '1' : '0');

                file.close();
                return true;
            }
            bool writeData() {return writeAlarms() && writeSettings() ? true : false;}

        public:
            RPi() {
                Win = npp::Window(LINES / 2 - 12, COLS / 2 - 35, 25, 71);
            
                for (unsigned char i = 0; i < 2; i++) {
                    CritPts.emplace_back();
                    for (unsigned char j = 0; j < 7; j++) {
                        CritPts[i].emplace_back();
                        for (unsigned char k = 0; k < 3; k++) {
                            CritPts[i][j].emplace_back();
                        }
                    }
                }

                if (!readData()) {
                    // Error
                }

                for (unsigned char i = 0; i < 4; i++) {
                    LeftColumn.emplace_back(npp::Button(Win.gposy() + 1 + i * 5, Win.gposx() + 2, 5, 7, {M1_CLICK}));
                    RightColumn.emplace_back(npp::Button(Win.gposy() + 1 + i * 5, Win.gposx() + 62, 5, 7, {M1_CLICK}));
                }

                Uniques_sMain.changeUptime = npp::Button(Win.gposy() + 6, Win.gposx() + 27, 5, 19, {M1_CLICK});
                Uniques_sMain.changeDowntime = npp::Button(Win.gposy() + 11, Win.gposx() + 27, 5, 19, {M1_CLICK});

                for (unsigned char i = 0; i < 3; i++) {
                    Uniques_sCritPts.addTime.emplace_back(npp::Button(Win.gposy() + 6, Win.gposx() + 20 + i * 12, 5, 7, {M1_CLICK}));
                    Uniques_sCritPts.removeTime.emplace_back(npp::Button(Win.gposy() + 12, Win.gposx() + 20 + i * 12, 5, 7, {M1_CLICK}));
                }
                for (unsigned char i = 0; i < 7; i++) {
                    Uniques_sCritPts.weekdays.emplace_back(npp::Button(Win.gposy() + 21, Win.gposx() + 5 + i * 9, 3, 7, {M1_CLICK}));
                }
                Uniques_sCritPts.copyTime.emplace_back(CritPts[Uniques_sCritPts.uptime][Weekday][Uniques_sCritPts.time].getHourNum());
                Uniques_sCritPts.copyTime.emplace_back(CritPts[Uniques_sCritPts.uptime][Weekday][Uniques_sCritPts.time].getMinuteNum());
                Uniques_sCritPts.copyTime.emplace_back(CritPts[Uniques_sCritPts.uptime][Weekday][Uniques_sCritPts.time].getSecondNum());
                Uniques_sCritPts.weekday = Weekday;

                Uniques_sPopups.upWarning = npp::Button(Win.gposy() + 13, Win.gposx() + 4, 5, 31, {M1_CLICK});
                Uniques_sPopups.upMain = npp::Button(Win.gposy() + 18, Win.gposx() + 4, 5, 31, {M1_CLICK});
                Uniques_sPopups.downWarning = npp::Button(Win.gposy() + 13, Win.gposx() + 36, 5, 31, {M1_CLICK});
                Uniques_sPopups.downMain = npp::Button(Win.gposy() + 18, Win.gposx() + 36, 5, 31, {M1_CLICK});

                Uniques_pCritPts.ok = npp::Button(Win.gposy() + 14, Win.gposx() + 28, 6, 15, {M1_CLICK});

                Timing::mtime.update(true);
                Now = Timing::Alarm(Timing::mtime.getHourNum(), Timing::mtime.getMinuteNum(), Timing::mtime.getSecondNum());
                Weekday = Timing::mtime.getWeekdayNum();
                IsUptime = Now.getTimeFormatted() > CritPts[PERIOD_UPTIME][Weekday][TIME_BUFFER].getTimeFormatted() ? true : false;

                update();
            }

            int main() {
                Win.reset();
                Win.dbox();

                int ch;
                while (true) {
                    switch (Screen) {
                        case SCR_HOME:
                            rHome();
                            break;
                        case SCR_SETTINGS_MAIN:
                            rSettings_Main();
                            break;
                        case SCR_SETTINGS_CRITPTS:
                            rSettings_CritPts();
                            break;
                        case SCR_SETTINGS_POPUPS:
                            rSettings_Popups();
                            break;
                        case SCR_POPUPS_CRITPTS:
                            rPopup_CritPts();
                            break;
                    }
                    update();

                    if ((ch = Win.gchar(false)) == KEY_MOUSE) {
                        if (npp::Mouse.gmouse(ch)) {
                            switch (Screen) {
                                case SCR_HOME:
                                    if (!hHome()) {
                                        if (!writeData()) {
                                            // Error
                                        }
                                        return -1;
                                    }
                                    break;
                                case SCR_SETTINGS_MAIN:
                                    hSettings_Main();
                                    break;
                                case SCR_SETTINGS_CRITPTS:
                                    hSettings_CritPts();
                                    break;
                                case SCR_SETTINGS_POPUPS:
                                    hSettings_Popups();
                                    break;
                                case SCR_POPUPS_CRITPTS:
                                    hPopups_CritPts();
                                    break;
                            }
                        }
                    } else {
                        if (ch == 'q') {
                            break;
                        }
                    }
                }

                if (!writeData()) {
                    // Error
                }
                return -1;
            }
    };
}

#endif /* SLEEPNEW */
