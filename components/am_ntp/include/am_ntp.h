#ifndef _AM_NTP_H_
#define _AM_NTP_H_

#include <stdbool.h>

/*
SNTP gives timestamp since 1900, but timestamp handled by unix systems start at 1970.
The current_timestamp = SNTP_timestamp - 1970_timestamp (2208988800) 
*/
#define NTP_1970_TIMESTAMP      2208988800
#define NTP_RELATIVE_GMT_BOG    25200

void am_ntp_init();

#endif