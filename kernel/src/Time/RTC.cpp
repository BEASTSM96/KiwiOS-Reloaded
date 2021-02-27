#include "RTC.h"

int century_register = 0x00;

namespace Time {
	unsigned char second = 0;
	unsigned char minute = 0;
	unsigned char hour = 0;
	unsigned char day = 0;
	unsigned char month = 1;
	unsigned int year = 2021;
}

int GetUpdateInProg()
{
	outb( CMOS_ADDRESS, 0x0A );
	return ( inb( CMOS_DATA ) & 0x80 );
}

unsigned char GetRTCRegister( int reg )
{
	outb( CMOS_ADDRESS, reg );
	return inb( CMOS_DATA );
}

void ReadRTC()
{
	unsigned char century;
	unsigned char last_second;
	unsigned char last_minute;
	unsigned char last_hour;
	unsigned char last_day;
	unsigned char last_month;
	unsigned char last_year;
	unsigned char last_century;
	unsigned char registerB;

	while( GetUpdateInProg() );

	Time::second = GetRTCRegister( 0x00 );
	Time::minute = GetRTCRegister( 0x02 );
	Time::hour = GetRTCRegister( 0x04 );
	Time::day = GetRTCRegister( 0x07 );
	Time::month = GetRTCRegister( 0x08 );
	Time::year = GetRTCRegister( 0x09 );

	if( century_register != 0 )
	{
		century = GetRTCRegister( century_register );
	}

	do
	{
		last_second = Time::second;
		last_minute = Time::minute;
		last_hour = Time::hour;
		last_day = Time::day;
		last_month = Time::month;
		last_year = Time::year;
		last_century = century;

		while( GetUpdateInProg() ); 
		Time::second = GetRTCRegister( 0x00 );
		Time::minute = GetRTCRegister( 0x02 );
		Time::hour = GetRTCRegister( 0x04 );
		Time::day = GetRTCRegister( 0x07 );
		Time::month = GetRTCRegister( 0x08 );
		Time::year = GetRTCRegister( 0x09 );
		if( century_register != 0 )
		{
			century = GetRTCRegister( century_register );
		}
	} while( ( last_second != Time::second ) || ( last_minute != Time::minute ) || ( last_hour != Time::hour ) ||
			 ( last_day != Time::day ) || ( last_month != Time::month ) || ( last_year != Time::year ) ||
			 ( last_century != century ) );

	registerB = GetRTCRegister( 0x0B );

	if( !( registerB & 0x04 ) )
	{
		Time::second = ( Time::second & 0x0F ) + ( ( Time::second / 16 ) * 10 );
		Time::minute = ( Time::minute & 0x0F ) + ( ( Time::minute / 16 ) * 10 );
		Time::hour = ( ( Time::hour & 0x0F ) + ( ( ( Time::hour & 0x70 ) / 16 ) * 10 ) ) | ( Time::hour & 0x80 );
		Time::day = ( Time::day & 0x0F ) + ( ( Time::day / 16 ) * 10 );
		Time::month = ( Time::month & 0x0F ) + ( ( Time::month / 16 ) * 10 );
		Time::year = ( Time::year & 0x0F ) + ( ( Time::year / 16 ) * 10 );
		if( century_register != 0 )
		{
			century = ( century & 0x0F ) + ( ( century / 16 ) * 10 );
		}
	}


	if( !( registerB & 0x02 ) && ( Time::hour & 0x80 ) )
	{
		Time::hour = ( ( Time::hour & 0x7F ) + 12 ) % 24;
	}

	if( century_register != 0 )
	{
		Time::year += century * 100;
	}
	else
	{
		Time::year += ( CURRENT_YEAR / 100 ) * 100;
		if( Time::year < CURRENT_YEAR ) Time::year += 100;
	}

}

const char* GetMonthName( int month )
{
	switch( month )
	{
		case 0:
			return "Error 0";

		case 1:
			return "January";

		case 2:
			return "February";

		case 3:
			return "March";

		case 4:
			return "April";

		case 5:
			return "May";

		case 6:
			return "June";

		case 7:
			return "July";

		case 8:
			return "August";

		case 9:
			return "September";

		case 10:
			return "October";

		case 11:
			return "November";

		case 12:
			return "December";

		default:
			break;
	}
}

const char* GetMonthName()
{
	switch( Time::month )
	{
		case 0:
			return "Error 0";

		case 1:
			return "January";

		case 2:
			return "February";

		case 3:
			return "March";

		case 4:
			return "April";

		case 5:
			return "May";

		case 6:
			return "June";

		case 7:
			return "July";

		case 8:
			return "August";

		case 9:
			return "September";

		case 10:
			return "October";

		case 11:
			return "November";

		case 12:
			return "December";

		default:
			break;
	}
}

const char* GetDayName( int day )
{
	switch( day )
	{
		case 0:
			return "Error 0";

		case 1:
			return "Monday";

		case 2:
			return "Tuesday";

		case 3:
			return "Wednesday";

		case 4:
			return "Thursday";

		case 5:
			return "Friday";

		case 6:
			return "Saturday";

		case 7:
			return "Sunday";

		default:
			break;
	}
}

const char* GetDayName()
{
	switch( Time::day )
	{
		case 0:
			return "Error 0";

		case 1:
			return "Monday";

		case 2:
			return "Tuesday";

		case 3:
			return "Wednesday";

		case 4:
			return "Thursday";

		case 5:
			return "Friday";

		case 6:
			return "Saturday";

		case 7:
			return "Sunday";

		default:
			break;
	}
}

//There HAS to be a better way...

const char* GetDaysFromMonths( int day, int month )
{
	switch( Time::year )
	{
		case 0:
			return "Error 0";

		case 2021:
		{
			switch( month )
			{
				case 0:
					return "Error 0";

				case 1:
				{
					switch( Time::day )
					{
						case 0:
							return "Error 0";

						case 1:
							return "Friday";

						case 2:
							return "Saturday";

						case 3:
							return "Sunday";

						case 4:
							return "Monday";

						case 5:
							return "Tuesday";

						case 6:
							return "Wednesday";

						case 7:
							return "Thursday";

						case 8:
							return "Friday";

						case 9:
							return "Saturday";

						case 11:
							return "Sunday";

						case 12:
							return "Tuesday";

						case 13:
							return "Wednesday";

						case 14:
							return "Thursday";

						case 15:
							return "Friday";

						case 16:
							return "Saturday";

						case 17:
							return "Sunday";

						case 18:
							return "Monday";

						case 19:
							return "Tuesday";

						case 20:
							return "Wednesday";

						case 21:
							return "Thursday";

						case 22:
							return "Friday";

						case 23:
							return "Saturday";

						case 24:
							return "Sunday";

						case 25:
							return "Monday";

						case 26:
							return "Tuesday";

						case 27:
							return "Wednesday";

						case 28:
							return "Thursday";

						case 29:
							return "Friday";

						case 30:
							return "Saturday";

						case 31:
							return "Sunday";

						default:
							break;
					}
				}

				case 2:
				{
					switch( Time::day )
					{
						case 0:
							return "Error 0";

						case 1:
							return "Monday";

						case 2:
							return "Tuesday";

						case 3:
							return "Wednesday";

						case 4:
							return "Thursday";

						case 5:
							return "Friday";

						case 6:
							return "Saturday";

						case 7:
							return "Sunday";

						case 8:
							return "Monday";

						case 9:
							return "Tuesday";

						case 10:
							return "Wednesday";

						case 11:
							return "Thursday";

						case 12:
							return "Friday";

						case 13:
							return "Saturday";

						case 14:
							return "Sunday";

						case 15:
							return "Monday";

						case 16:
							return "Tuesday";

						case 17:
							return "Wednesday";

						case 18:
							return "Thursday";

						case 19:
							return "Friday";

						case 20:
							return "Saturday";

						case 21:
							return "Sunday";

						case 22:
							return "Monday";

						case 23:
							return "Tuesday";

						case 24:
							return "Wednesday";

						case 25:
							return "Thursday";

						case 26:
							return "Friday";

						case 27:
							return "Saturday";

						case 28:
							return "Sunday";

						default:
							break;
					}
				}

				case 3:
				{
					switch( Time::day )
					{
						case 0:
							return "Error 0";

						case 1:
							return "Monday";

						case 2:
							return "Tuesday";

						case 3:
							return "Wednesday";

						case 4:
							return "Thursday";

						case 5:
							return "Friday";

						case 6:
							return "Saturday";

						case 7:
							return "Sunday";

						case 8:
							return "Monday";

						case 9:
							return "Tuesday";

						case 10:
							return "Wednesday";

						case 11:
							return "Thursday";

						case 12:
							return "Friday";

						case 13:
							return "Saturday";

						case 14:
							return "Sunday";

						case 15:
							return "Monday";

						case 16:
							return "Tuesday";

						case 17:
							return "Wednesday";

						case 18:
							return "Thursday";

						case 19:
							return "Friday";

						case 20:
							return "Saturday";

						case 21:
							return "Sunday";

						case 22:
							return "Monday";

						case 23:
							return "Tuesday";

						case 24:
							return "Wednesday";

						case 25:
							return "Thursday";

						case 26:
							return "Friday";

						case 27:
							return "Saturday";

						case 28:
							return "Sunday";

						case 29:
							return "Monday";

						case 30:
							return "Tuesday";

						case 31:
							return "Wednesday";

						default:
							break;
					}

				}

				default:
					break;
			}
		}

		default:
			break;
	}
}
