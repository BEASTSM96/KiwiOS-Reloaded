//Kiwi RTC header

#pragma once

#include "../IO.h"

namespace Time {
	extern unsigned char second;
	extern unsigned char minute;
	extern unsigned char hour;
	extern unsigned char day;
	extern unsigned char month;
	extern unsigned int year;
}

#define LAST_YEAR 2020
#define CURRENT_YEAR 2021
#define NEXT_YEAR 2022
#define CENTURY 21

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

int GetUpdateInProg();
unsigned char GetRTCRegister( int reg );
void ReadRTC();

const char* GetMonthName( int month );
const char* GetMonthName();

const char* GetDayName();
const char* GetDayName( int day );

const char* GetDaysFromMonths( int day, int month );