#include "logging.h"

#include "storage.h"
#include "mto_time.h"

#define SE_ERR "SEERR"
#define SE_LOG "SELOG"

void err(uint8_t code)
{
    HANDLE_t storage = getStorage(ST_LOGS, NULL);
    HANDLE_t entry = getEntry(storage, SE_LOG);

    mto_write(entry, &code, 0, sizeof(uint8_t));

    free_handle(entry);
    free_handle(storage);
}

void log(uint8_t lvl, char* msg, uint8_t length)
{
    HANDLE_t storage = getStorage(ST_LOGS, NULL);
    HANDLE_t entry = getEntry(storage, SE_LOG);

    time_stamp_t ts = getCurrentTime();
    mto_write(entry, &ts, 0, sizeof(time_stamp_t));

    mto_write(entry, &lvl, 0, sizeof(uint8_t));

    mto_write(entry, msg, 0, length);

    free_handle(entry);
    free_handle(storage);
}