#include "time.h"
#include "storage.h"

time_driver_t* driver;

void registerTimeDriver(time_driver_t *timeDriver)
{
    driver = timeDriver;
}

time_stamp_t getCurrentTime(void)
{
    return driver->getCurrentTime();
}

void setTime(time_stamp_t currentTime)
{
    driver->setTime(currentTime);
}

BOOLEAN getAlarm(alarm_t* alarm)
{
    alarm->timeStamp = driver->getAlarm();

    BOOLEAN rv = FALSE;

    HANDLE_t storage = getStorage(ST_TMP, NULL);
    HANDLE_t entry = getEntry(storage, CURR_ALARM_ID);
    if(entry != NULL_HANDLE)
    {
        if(read(entry, alarm, 0, sizeof(alarm_t)));
            rv = TRUE;
    }

    free_handle(entry);
    free_handle(storage);

    return rv;
}

void setAlarm(alarm_t alarm)
{
    driver->setAlarm(alarm.timeStamp);
}

void setAlarm_M(static_id_t name, time_stamp_t timeStamp, D_ACTION)
{
    alarm_t alarm = {name, timeStamp, fun_ptr};
    setAlarm(alarm);

    uint16_t storageSize;
    HANDLE_t storage = getStorage(ST_TMP, &storageSize);

    HANDLE_t entry = getEntry(storage, CURR_ALARM_ID);
    if(entry == NULL_HANDLE)
        entry = addEntry(storage, CURR_ALARM_ID);

    write(entry, &alarm, 0, sizeof(alarm_t));
    
    free_handle(entry);
    free_handle(storage);
}

date_time_t toDateTime(time_stamp_t timeStamp);