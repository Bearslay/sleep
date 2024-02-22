#ifndef SLEEPNEW
#define SLEEPNEW

#include "ncursespp.hpp"
#include "timing.hpp"
#include <fstream>\

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

    class RPi {
        private:
            npp::Window Win;
            std::vector<npp::Button> RightColumn, LeftColumn;
            
            Timing::Alarm Now;
            std::vector<std::vector<std::vector<Timing::Alarm>>> CritPts;

            unsigned char Weekday = Timing::mtime.getWeekdayNum();

            bool IsCharging = false;
            bool Use24Hr = true;

            bool IsUptime = true;
            unsigned char Screen = SCR_HOME;

            std::vector<unsigned int> Charges = {0, 0, 0};
            std::vector<std::vector<bool>> Popups = {{false, false, true, true}, {false, false, false, false}};

            struct {
                npp::Button changeUptime, changeDowntime;
            } Settings_Main_Unique;

            struct {
                
                
                bool uptime = false;
                unsigned char time = TIME_MAIN;
            } Settings_CritPts_Unique;

            struct {
                npp::Button upWarning, upMain, downWarning, downMain;
            } Settings_Popups_Unique;

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
                Win.dbox(1, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, Settings_CritPts_Unique.time == TIME_MAIN ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(2, 4, "M", MTEXT_6x6, Settings_CritPts_Unique.time == TIME_MAIN ? NPP_CYAN : NPP_TEAL);
                // Change Buffer Time
                Win.dbox(6, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, Settings_CritPts_Unique.time == TIME_BUFFER ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(7, 4, "B", MTEXT_6x6, Settings_CritPts_Unique.time == TIME_BUFFER ? NPP_CYAN : NPP_TEAL);
                // Change Warning Time
                Win.dbox(11, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, Settings_CritPts_Unique.time == TIME_WARNING ? NPP_CYAN : NPP_TEAL);
                Win.wmstr(12, 4, "W", MTEXT_6x6, Settings_CritPts_Unique.time == TIME_WARNING ? NPP_CYAN : NPP_TEAL);
                // Sync Times
                Win.dbox(16, 2, 5, 7, {LIGHT_SOFT, DASHED_NONE}, NPP_BLUE);
                Win.wmstr(17, 4, "T", MTEXT_6x6, NPP_BLUE);

                // Weekdays
                for (unsigned char i = 0; i < 7; i++) {
                    Win.dbox(21, 5 + i * 9, 3, 7, {HEAVY_BOTH, DASHED_NONE}, i == Weekday ? NPP_LIGHT_GRAY : NPP_DARK_GRAY);
                    Win.wstr(22, 7 + i * 9, strtowstr(tupper(Timing::Keys.weekdayNamesAbb[i])), i == Weekday ? NPP_LIGHT_GRAY : NPP_DARK_GRAY, "bo");
                }

                // Editing Box
                Win.dbox(4, 10, 14, 51);
                Win.wstr(Win.wstrp(Win.wstrp(Win.wstrp(5, 21 - Settings_CritPts_Unique.time, L"Editing: ", NPP_WHITE, "bo"), Settings_CritPts_Unique.uptime ? L"Uptime (" : L"Downtime (", NPP_WHITE, "bo"), Settings_CritPts_Unique.time == TIME_MAIN ? L"Main" : (Settings_CritPts_Unique.time == TIME_BUFFER ? L"Buffer" : L"Warning"), NPP_WHITE, "bo"), L" Time)", NPP_WHITE, "bo");
                
                // Information Box
                Win.dbox(18, 10, 3, 51);
                Win.dvline(18, 26, 3, false, {LIGHT_HARD, DASHED_TRIPLE});
                Win.dvline(18, 43, 3, false, {LIGHT_HARD, DASHED_TRIPLE});
                Win.wstr(Win.wstrp(19, 11, L"M: "), strtowstr(CritPts[Settings_CritPts_Unique.uptime][Weekday][TIME_MAIN].getTimeFormatted(Use24Hr)));
                Win.wstr(Win.wstrp(19, 28, L"B: "), strtowstr(CritPts[Settings_CritPts_Unique.uptime][Weekday][TIME_BUFFER].getTimeFormatted(Use24Hr)));
                Win.wstr(Win.wstrp(19, 45, L"W: "), strtowstr(CritPts[Settings_CritPts_Unique.uptime][Weekday][TIME_WARNING].getTimeFormatted(Use24Hr)));

                // Current Transition Time
                Win.wmstr(10, 20, CritPts[Settings_CritPts_Unique.uptime][Weekday][Settings_CritPts_Unique.time].getTimeFormatted(Use24Hr).substr(0, 8), MTEXT_6x6);
                if (!Use24Hr) {
                    Win.dbox(10, 53, 3, 6, {HEAVY_BOTH, DASHED_NONE}, NPP_TEAL);
                    Win.wstr(11, 55, strtowstr(CritPts[Settings_CritPts_Unique.uptime][Weekday][Settings_CritPts_Unique.time].hourSuffix()), NPP_TEAL, "bo");
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

            bool hHome() {
                if (RightColumn[0].cclick() == M1_CLICK) {
                    return false;
                } else if (RightColumn[1].cclick() == M1_CLICK) {
                    IsCharging = !IsCharging;
                } else if (RightColumn[2].cclick() == M1_CLICK) {
                    Screen = SCR_SETTINGS_MAIN;
                    Win.reset();
                    Win.dbox();
                }

                return true;
            }
            void hSettings_Main() {
                if (RightColumn[0].cclick() == M1_CLICK) {
                    Screen = SCR_HOME;
                    Win.reset();
                    Win.dbox();
                } else if (RightColumn[1].cclick() == M1_CLICK) {
                    Use24Hr = !Use24Hr;
                    Win.reset();
                    Win.dbox();
                } else if (RightColumn[2].cclick() == M1_CLICK) {
                    Screen = SCR_SETTINGS_POPUPS;
                    Win.reset();
                    Win.dbox();
                } else if (Settings_Main_Unique.changeUptime.cclick() == M1_CLICK) {
                    Screen = SCR_SETTINGS_CRITPTS;
                    Settings_CritPts_Unique.uptime = true;
                    Win.reset();
                    Win.dbox();
                } else if (Settings_Main_Unique.changeDowntime.cclick() == M1_CLICK) {
                    Screen = SCR_SETTINGS_CRITPTS;
                    Settings_CritPts_Unique.uptime = false;
                    Win.reset();
                    Win.dbox();
                }
            }
            void hSettings_CritPts() {

            }
            void hSettings_Popups() {
                if (RightColumn[0].cclick() == M1_CLICK) {
                    Screen = SCR_SETTINGS_MAIN;
                    Win.reset();
                    Win.dbox();
                } else if (Settings_Popups_Unique.upWarning.cclick() == M1_CLICK) {
                    Popups[0][POP_UPWARN] = !Popups[0][POP_UPWARN];
                } else if (Settings_Popups_Unique.upMain.cclick() == M1_CLICK) {
                    Popups[0][POP_UPMAIN] = !Popups[0][POP_UPMAIN];
                } else if (Settings_Popups_Unique.downWarning.cclick() == M1_CLICK) {
                    Popups[0][POP_DOWNWARN] = !Popups[0][POP_DOWNWARN];
                } else if (Settings_Popups_Unique.downMain.cclick() == M1_CLICK) {
                    Popups[0][POP_DOWNMAIN] = !Popups[0][POP_DOWNMAIN];
                }
            }

            void update() {
                Timing::mtime.update(true);
                Now = Timing::Alarm(Timing::mtime.getHourNum(), Timing::mtime.getMinuteNum(), Timing::mtime.getSecondNum());
                Weekday = Timing::mtime.getWeekdayNum();

                // All of the different UIs show the time and have to constantly update it so its rendered here for funsies
                Win.wmstr(1, 20 - (!Use24Hr ? 6 : 0), Timing::mtime.getTimeFormatted(Use24Hr), MTEXT_6x6);
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
            
                for (unsigned char i = 0; i < 4; i++) {
                    LeftColumn.emplace_back(npp::Button(Win.gposy() + 1 + i * 5, Win.gposx() + 2, 5, 7, {M1_CLICK}));
                    RightColumn.emplace_back(npp::Button(Win.gposy() + 1 + i * 5, Win.gposx() + 62, 5, 7, {M1_CLICK}));
                }

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

                Settings_Main_Unique.changeUptime = npp::Button(Win.gposy() + 6, Win.gposx() + 27, 5, 19, {M1_CLICK});
                Settings_Main_Unique.changeDowntime = npp::Button(Win.gposy() + 11, Win.gposx() + 27, 5, 19, {M1_CLICK});

                // a

                Settings_Popups_Unique.upWarning = npp::Button(Win.gposy() + 13, Win.gposx() + 4, 5, 31, {M1_CLICK});
                Settings_Popups_Unique.upMain = npp::Button(Win.gposy() + 18, Win.gposx() + 4, 5, 31, {M1_CLICK});
                Settings_Popups_Unique.downWarning = npp::Button(Win.gposy() + 13, Win.gposx() + 36, 5, 31, {M1_CLICK});
                Settings_Popups_Unique.downMain = npp::Button(Win.gposy() + 18, Win.gposx() + 36, 5, 31, {M1_CLICK});

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
