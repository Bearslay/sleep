#ifndef TIMING
#define TIMING

#include "ncursespp.hpp"
#include <chrono>
#include <unordered_map>

#define JANUARY 0
#define FEBRUARY 1
#define MARCH 2
#define APRIL 3
#define MAY 4
#define JUNE 5
#define JULY 6
#define AUGUST 7
#define SEPTEMBER 8
#define OCTOBER 9
#define NOVEMBER 10
#define DECEMBER 11
#define JAN 0
#define FEB 1
#define MAR 2
#define APR 3
#define MAY 4
#define JUN 5
#define JUL 6
#define AUG 7
#define SEP 8
#define OCT 9
#define NOV 10
#define DEC 11

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6
#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

namespace Timing {
    const static struct {
        bool RealSync = false;
        bool Use24Hr = true;
        bool AddSuffix = false;
        bool FullName = true;
    } DefaultParams;

    const static struct {
        std::string weekdayNamesAbb[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        std::unordered_map<std::string, std::string> weekdayNamesFull = {{"Sun", "Sunday"}, {"Mon", "Monday"}, {"Tue", "Tuesday"}, {"Wed", "Wednesday"}, {"Thu", "Thursday"}, {"Fri", "Friday"}, {"Sat", "Saturday"}};
        std::unordered_map<std::string, unsigned char> weekdayNums = {{"Sun", 0}, {"Mon", 1}, {"Tue", 2}, {"Wed", 3}, {"Thu", 4}, {"Fri", 5}, {"Sat", 6}};
        
        std::string monthNamesAbb[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        std::unordered_map<std::string, std::string> monthNamesFull = {{"Jan", "January"}, {"Feb", "February"}, {"Mar", "March"}, {"Apr", "April"}, {"May", "May"}, {"Jun", "June"}, {"Jul", "July"}, {"Aug", "August"}, {"Sep", "September"}, {"Oct", "October"}, {"Nov", "November"}, {"Dec", "December"}};
        std::unordered_map<std::string, unsigned char> monthNums = {{"Jan", 0}, {"Feb", 1}, {"Mar", 2}, {"Apr", 3}, {"May", 4}, {"Jun", 5}, {"Jul", 6}, {"Aug", 7}, {"Sep", 8}, {"Oct", 9}, {"Nov", 10}, {"Dec", 11}};
        
        unsigned char monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    } Keys;

    class {
        private:
            std::time_t Time;
            double Rate = 1;
            unsigned long Accumulator = 0;
        
        public:
            std::time_t update() {
                if (Accumulator > 1000 / Rate) {
                    Time++;
                    Accumulator = 0;
                } else {
                    Accumulator++;
                }

                return Time;
            }

            void sync() {Time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());}

            void changeRate(double rate = 1) {Rate = rate < 0 || rate > 1000 ? 1 : rate;}

    } TimeManip;

    class TimePoint {
        private:
            unsigned long TimeNum = 18000;
            unsigned short SecondNum = 0;
            unsigned short MinuteNum = 0;
            unsigned short HourNum = 0;
            unsigned short DayNum = 1;
            unsigned short WeekdayNum = 0;
            unsigned short MonthNum = 0;
            unsigned long YearNum = 1970;

            std::string TimeStr = "Thu Jan 01 00:00:00 1970";
            std::string SecondStr = "00";
            std::string MinuteStr = "00";
            std::string HourStr = "00";
            std::string DayStr = "01";
            std::string WeekdayStr = "Thu";
            std::string MonthStr = "Jan";
            std::string YearStr = "1970";

        public:
            TimePoint() {update(true);}
            TimePoint(unsigned long time) {
                std::time_t t = time;

                TimeStr = std::ctime(&t);
                TimeNum = (unsigned long)t;
                
                setTime(TimeStr);
            }

            void setTime(std::string input) {
                TimeStr = input;
                SecondStr = getTimeStr().substr(17, 2);
                SecondNum = std::stoi(SecondStr);
                MinuteStr = getTimeStr().substr(14, 2);
                MinuteNum = std::stoi(MinuteStr);
                HourStr = getTimeStr().substr(11, 2);
                HourNum = std::stoi(HourStr);
                DayStr = getTimeStr().substr(8, 2);
                DayNum = std::stoi(DayStr);
                WeekdayStr = getTimeStr().substr(0, 3);
                WeekdayNum = Keys.weekdayNums.at(WeekdayStr);
                MonthStr = getTimeStr().substr(4, 3);
                MonthNum = Keys.monthNums.at(MonthStr);
                YearStr = getTimeStr().substr(20);
                if (YearStr[YearStr.length() - 1] == '\n' || YearStr[YearStr.length() - 1] == '\0') {YearStr.erase(YearStr.length() - 1);}
                YearNum = std::strtoul(YearStr.c_str(), NULL, 0);
            }
            void setTime(unsigned char weekday, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second, unsigned long year) {
                weekday = weekday < 0 ? 0 : (weekday > 6 ? 6 : weekday);
                month = month < 0 ? 0 : (month > 11 ? 11 : month);
                day = day < 1 ? 1 : (day > ((day == 29 && month == FEBRUARY) ? 29 : Keys.monthDays[month]) ? Keys.monthDays[month] : day);
                hour = hour < 0 ? 0 : (hour > 23 ? 23 : hour);
                minute = minute < 0 ? 0 : (minute > 59 ? 59 : minute);
                second = second < 0 ? 0 : (second > 59 ? 59 : second);
                year = year < 0 ? 0 : year;

                setTime(Keys.weekdayNamesAbb[weekday] + " " + Keys.monthNamesAbb[month] + " " + (day < 10 ? "0" : "") + std::to_string(day) + " " + (hour < 10 ? "0" : "") + std::to_string(hour) + ":" + (minute < 10 ? "0" : "") + std::to_string(minute) + ":" + (second < 10 ? "0" : "") + std::to_string(second) + " " + std::to_string(year));
            }
            void update(bool realSync = DefaultParams.RealSync) {
                std::time_t t = realSync ? std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) : TimeManip.update();

                TimeStr = std::ctime(&t);
                TimeNum = (unsigned long)t;
                
                setTime(TimeStr);
            }

            const std::string hourSuffix() {return (HourNum >= 12 && HourNum < 24) ? "PM" : "AM";}
            const unsigned short hour24to12(unsigned char hour) {return hour == 12 ? 12 : (hour > 12 ? hour - 12 : hour);}
            const unsigned short hour12to24(unsigned char hour, bool am) {return am ? (hour == 12 ? 0 : hour) : (hour == 12 ? 12 : hour + 12);}

            const bool isLeapYear() {return (YearNum % 400 == 0 || (YearNum % 100 != 0 && YearNum % 4 == 0)) ? true : false;}
            const bool checkLeapYear(unsigned short year) {return (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) ? true : false;}


            const unsigned long getTimeNum() {return TimeNum;}
            const std::string getTimeStr() {return TimeStr;}
            
            const unsigned short getSecondNum() {return SecondNum;}
            const std::string getSecondStr() {return SecondStr;}
            
            const unsigned short getMinuteNum() {return MinuteNum;}
            const std::string getMinuteStr() {return MinuteStr;}

            const unsigned short getHourNum(bool use24Hr = DefaultParams.Use24Hr) {return use24Hr ? HourNum : hour24to12(HourNum);}
            const std::string getHourStr(bool use24Hr = DefaultParams.Use24Hr, bool addSuffix = false) {return use24Hr ? HourStr : ((hour24to12(HourNum) < 10 ? "0" : "") + std::to_string(hour24to12(HourNum)) + (addSuffix ? hourSuffix() : ""));}

            const unsigned short getDayNum() {return DayNum;}
            const std::string getDayStr(bool addSuffix = DefaultParams.AddSuffix) {return !addSuffix ? DayStr : (DayStr + (DayStr[1] == '1' && DayNum != 11 ? "st" : (DayStr[1] == '2' && DayNum != 12 ? "nd" : (DayStr[1] == '3' && DayNum != 13 ? "rd" : "th"))));}

            const unsigned short getWeekdayNum() {return WeekdayNum;}
            const std::string getWeekdayStr(bool full = DefaultParams.FullName) {return full ? Keys.weekdayNamesFull.at(WeekdayStr) : WeekdayStr;}

            const unsigned short getMonthNum() {return MonthNum;}
            const std::string getMonthStr(bool full = DefaultParams.FullName) {return full ? Keys.monthNamesFull.at(MonthStr) : MonthStr;}

            const unsigned long getYearNum() {return YearNum;}
            const std::string getYearStr() {return YearStr;}

            const std::string getTimeFormatted(bool use24Hr = DefaultParams.Use24Hr) {return getHourStr(use24Hr) + ":" + getMinuteStr() + ":" + getSecondStr() + (!use24Hr ? " " + hourSuffix() : "");}


            const unsigned short secondsInMinute() {return SecondNum;}

            const unsigned short minuteInHour() {return MinuteNum;}
            const unsigned short secondsInHour() {return MinuteNum * 60 + SecondNum;}

            const unsigned short hoursInDay() {return HourNum;}
            const unsigned short minutesInDay() {return HourNum * 60 + MinuteNum;}
            const unsigned int secondsInDay() {return HourNum * 3600 + MinuteNum * 60 + SecondNum;}

            const unsigned short daysInWeek() {return WeekdayNum;}
            const unsigned short hoursInWeek() {return WeekdayNum * 24 + HourNum;}
            const unsigned short minutesInWeek() {return WeekdayNum * 1440 + HourNum * 60 + MinuteNum;}
            const unsigned int secondsInWeek() {return WeekdayNum * 122400 + HourNum * 3600 + MinuteNum * 60 + SecondNum;}
            
            const unsigned short weeksInMonth() {return daysInMonth() / 7;}
            const unsigned short daysInMonth() {return DayNum - 1;}
            const unsigned short hoursInMonth() {return daysInMonth() * 24 + HourNum;}
            const unsigned short minutesInMonth() {return daysInMonth() * 1440 + HourNum * 60 + MinuteNum;}
            const unsigned int secondsInMonth() {return daysInMonth() * 122400 + HourNum * 3600 + MinuteNum * 60 + SecondNum;}

            const unsigned short monthsInYear() {return MonthNum;}
            const unsigned short weeksInYear() {return daysInYear() / 7;}
            const unsigned short daysInYear() {
                unsigned short day = DayNum - 1;
                for (unsigned char i = 0; i < MonthNum; i++) {
                    day += Keys.monthDays[i];
                }
                return day;
            }
            const unsigned short hoursInYear() {return daysInYear() * 24 + HourNum;}
            const unsigned int minutesInYear() {return daysInYear() * 1440 + HourNum * 60 + MinuteNum;}
            const unsigned int secondsInYear() {return daysInYear() * 122400 + HourNum * 3600 + MinuteNum * 60 + SecondNum;}

            const unsigned long yearsSinceZero() {return YearNum;}
            const unsigned long monthsSinceZero() {return YearNum * 12 + MonthNum;}
            const unsigned long weeksSinceZero() {return daysSinceZero() / 7;}
            const unsigned long daysSinceZero() {return YearNum * 365 + daysInYear();}
            const unsigned long HoursSinceZero() {return daysSinceZero() * 24 + HourNum;}
            const unsigned long MinutesSinceZero() {return daysSinceZero() * 1440 + HourNum * 60 + MinuteNum;}
            const unsigned long SecondsSinceZero() {return daysSinceZero() * 122400 + HourNum * 3600 + MinuteNum * 60 + SecondNum;}
    } mtime;

    class Alarm {
        private:
            TimePoint Time;
            std::vector<unsigned char> Days;
            bool Active = true;
            bool Acknowledged = false;

        public:
            Alarm(unsigned char hour = 0, unsigned char minute = 0, unsigned char second = 0, std::vector<unsigned char> days = {}) {
                hour = (hour < 0 || hour >= 24) ? mtime.getHourNum() : hour;
                minute = (minute < 0 || minute >= 60) ? mtime.getMinuteNum() : minute;
                second = (second < 0 || second >= 60) ? mtime.getSecondNum() : second;

                Time.setTime(mtime.getWeekdayNum(), mtime.getMonthNum(), mtime.getDayNum(), hour, minute, second, mtime.getYearNum());

                if (days.size() == 0) {
                    Days = {0, 1, 2, 3, 4, 5, 6};
                } else {
                    while (days.size() > 7) {days.erase(days.end());}
                    for (unsigned char i = 0; i < days.size(); i++) {if (days[i] < 0 || days[i] >= 7) {days.erase(days.begin() + i);}}
                    Days = days;
                }
            }

            /// @brief Determine whether the alarm should be "rining" or not
            /// @param comparison The time point to compare with the alarm's target time
            /// @returns True if the alarm should ring, false if not
            bool check(TimePoint comparison = mtime) {
                if (!Active) {return false;}
                if (Acknowledged) {return false;}

                bool sameDay = (Days.size() == 0) ? true : false;
                for (unsigned char i = 0; i < Days.size(); i++) {
                    if (Days[i] == comparison.getWeekdayNum()) {
                        sameDay = true;
                        break;
                    }
                }
                
                return comparison.getHourNum() >= Time.getHourNum() && comparison.getMinuteNum() >= Time.getMinuteNum() && comparison.getSecondNum() >= Time.getSecondNum() && sameDay ? true : false;
            }

            void acknowledge(TimePoint t = mtime) {Acknowledged = (check(t)) ? true : false;}

            void toggle() {Active = !Active;}
            void switchOn() {Active = true;}
            void switchOff() {Active = false;}

            const char getHourNum(bool use24Hr = true) {return Time.getHourNum(use24Hr);}
            const std::string getHourStr(bool use24Hr = true, bool addSuffix = false) {return Time.getHourStr(use24Hr, addSuffix);}
            const char getMinuteNum() {return Time.getMinuteNum();}
            const std::string getMinuteStr() {return Time.getMinuteStr();}
            const char getSecondNum() {return Time.getSecondNum();}
            const std::string getSecondStr() {return Time.getSecondStr();}

            const std::string getTimeFormatted(bool use24Hr = true) {return Time.getTimeFormatted(use24Hr);}

            std::vector<unsigned char> getDaysRaw() {return Days;}
            std::string getDaysStr(bool shorten = true, bool full = false) {
                std::string output;
                bool weekdays = true;
                bool weekend = true;
                if (Days.size() == 7) {return shorten ? "All Days" : (full ? "Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday" : "Sun, Mon, Tue, Wed, Thu, Fri, Sat");}
                
                for (unsigned char i = 0; i < Days.size(); i++) {
                    if (Days[i] == 0 || Days[i] == 6) {weekdays = false;}
                    if (Days[i] != 0 && Days[i] != 6) {weekend = false;}
                    output += (full ? Keys.weekdayNamesFull.at(Keys.weekdayNamesAbb[Days[i]]) : Keys.weekdayNamesAbb[Days[i]]) + ", ";
                }

                output.erase(output.length() - 2, 2);
                return shorten ? (weekdays && Days.size() == 5 ? "Weekdays" : (weekend && Days.size() == 2 ? "Weekend Days" : output)) : output;
            }
    };
}

#endif
