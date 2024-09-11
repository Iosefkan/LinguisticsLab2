#pragma once

#include <TextTable.h>
#include <chrono>
#include <ctime>    

typedef std::chrono::time_point<std::chrono::system_clock> Time;

std::string GetString();

std::string GetLocalDateString(std::chrono::time_point<std::chrono::system_clock>& time_point);

Time GetTimePoint(const std::string& value);

size_t Split(const std::string& txt, std::vector<std::string>& strs, char ch);

int GetDaysBetweenTimeStamps(Time& tp1, Time& tp2);

std::string GetTimeDifferenceHHMMSS(Time& tp1, Time& tp2);

std::string Round(double value, int precision = 2);