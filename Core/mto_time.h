#ifndef MTO_TIME
#define MTO_TIME

#include "mto.h"

#define CURR_ALARM_ID "ALARM"

typedef uint32_t time_stamp_t; 
#define D_ACTION void (*fun_ptr)(void)

typedef struct date_time_t
{
    uint8_t year;
    uint8_t month;
    uint8_t day;

    uint8_t hour;
    uint8_t minute;
    uint8_t secound;

    enum DOW
    {
        DOW_Monday,
        DOW_Tuesday,
        DOW_Wenday,
        DOW_Thursday,
        DOW_Friday,
        DOW_Saturday,
        DOW_Sunday,
        DOW_EveryDay
    } dow;
    
} date_time_t;

typedef struct alarm_t
{
    static_id_t name;
    time_stamp_t timeStamp;
    D_ACTION;

} alarm_t;

typedef struct time_driver_t
{
    void (*init)(void);

    time_stamp_t (*getTime)(void);
    void (*setTime)(time_stamp_t currentTime);

    time_stamp_t (*getAlarm)(void);
    void (*setAlarm)(time_stamp_t alarm);

    BOOLEAN (*toDateTime)(time_stamp_t timeStamp, date_time_t* dateTime);
    BOOLEAN (*fromDateTime)(date_time_t* dateTime, time_stamp_t* timeStamp);

} time_driver_t;

void registerTimeDriver(time_driver_t *timeDriver);

time_stamp_t getCurrentTime(void);
void setTime(time_stamp_t currentTime);
BOOLEAN toDateTime(time_stamp_t timeStamp, date_time_t* dateTime);
BOOLEAN fromDateTime(date_time_t* dateTime, time_stamp_t* timeStamp);

BOOLEAN getAlarm(alarm_t* alarm);
void setAlarm(alarm_t* alarm);
void setAlarm_M(static_id_t name, time_stamp_t timeStamp, D_ACTION);

#endif // !MTO_TIME