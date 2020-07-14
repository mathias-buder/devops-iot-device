/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file os_time.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "os_time.h"
#include "OS.h"

#include <time.h>
#include <sys/time.h>

#include "esp_log.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
F32       os_time_stamp_ms_f32;

struct tm os_time_info_s;
time_t    os_time_t;
char      os_time_str_buf[64];

/*********************************************************************/
/*      LOCAL VARIABLES                                              */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
OS_TIME_C::OS_TIME_C()
{
    os_time_info_s.tm_sec   = 0U;    /**< tm_sec — the number of seconds after the minute, normally in the range 0 to 59, but can be up to 60 to allow for leap seconds. */
    os_time_info_s.tm_min   = 10U;   /**< tm_min — the number of minutes after the hour, in the range 0 to 59. */
    os_time_info_s.tm_hour  = 10U;   /**< tm_hour — the number of hours past midnight, in the range 0 to 23. */
    os_time_info_s.tm_mday  = 24U;   /**< tm_mday — the day of the month, in the range 1 to 31. */
    os_time_info_s.tm_mon   = 3U;    /**< tm_mon — the number of months since January, in the range 0 to 11. */
    os_time_info_s.tm_year  = 2020;  /**< tm_year — the number of years since 1900. */
    os_time_info_s.tm_wday  = 2U;    /**< tm_wday — the number of days since Sunday, in the range 0 to 6. */
    os_time_info_s.tm_yday  = 83U;   /**< tm_yday — the number of days since January 1, in the range 0 to 365. */
    os_time_info_s.tm_isdst = FALSE; /**< tm_isdst — a flag that indicates whether daylight saving time is in effect at the time described. The value is positive if daylight saving time is in effect, zero if it is not, and negative if the information is not available. */


    /* Set timezone to Berlin Standard Time */
    setenv( "TZ", "CET-1CEST,M3.5.0,M10.5.0/3", TRUE );
    tzset();

    ESP_LOGI( OS_LOG_MSG_TAG, "Constructor OS_TIME_C called" );
}

OS_TIME_C::~OS_TIME_C()
{

}


void OS_TIME_C::update()
{
    os_time_stamp_ms_f32 += 0.1F;


    time( &os_time_t );

    localtime_r( &os_time_t, &os_time_info_s );

    strftime( os_time_str_buf, sizeof( os_time_str_buf ), "%c", &os_time_info_s );

    ESP_LOGI( OS_LOG_MSG_TAG, "The current date/time in Berlin is: %s", os_time_str_buf );
}













void os_time_init( void )
{

    os_time_info_s.tm_sec   = 0U;    /**< tm_sec — the number of seconds after the minute, normally in the range 0 to 59, but can be up to 60 to allow for leap seconds. */
    os_time_info_s.tm_min   = 10U;   /**< tm_min — the number of minutes after the hour, in the range 0 to 59. */
    os_time_info_s.tm_hour  = 10U;   /**< tm_hour — the number of hours past midnight, in the range 0 to 23. */
    os_time_info_s.tm_mday  = 24U;   /**< tm_mday — the day of the month, in the range 1 to 31. */
    os_time_info_s.tm_mon   = 3U;    /**< tm_mon — the number of months since January, in the range 0 to 11. */
    os_time_info_s.tm_year  = 2020;  /**< tm_year — the number of years since 1900. */
    os_time_info_s.tm_wday  = 2U;    /**< tm_wday — the number of days since Sunday, in the range 0 to 6. */
    os_time_info_s.tm_yday  = 83U;   /**< tm_yday — the number of days since January 1, in the range 0 to 365. */
    os_time_info_s.tm_isdst = FALSE; /**< tm_isdst — a flag that indicates whether daylight saving time is in effect at the time described. The value is positive if daylight saving time is in effect, zero if it is not, and negative if the information is not available. */



    /* Set timezone to Berlin Standard Time */
    setenv( "TZ", "CET-1CEST,M3.5.0,M10.5.0/3", TRUE );
    tzset();
}

void os_time_update( void )
{

    os_time_stamp_ms_f32 += 0.1F;


    time( &os_time_t );

    localtime_r( &os_time_t, &os_time_info_s );

    strftime( os_time_str_buf, sizeof( os_time_str_buf ), "%c", &os_time_info_s );

    ESP_LOGI( OS_LOG_MSG_TAG, "The current date/time in Berlin is: %s", os_time_str_buf );
}
