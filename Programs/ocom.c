#include "ocom.h"

#include <mto_time.h>
#include <program.h>
#include <storage.h>
#include <sens_driver.h>

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
extern LiquidCrystal_I2C lcd;

HANDLE_t socket;
com_settings_t settings;
uint16_t sessionId = -1;

extern const version_t VERSION;
const version_t OCOM_VERSION = { 0, 0, 1, 0 };

time_stamp_t lastContact;
int available = 0;

void com_main(void)
{
    // HANDLE_t cfgStorage = getStorage(ST_CONFIG, NULL);
    // HANDLE_t cfgEntry = getEntry(cfgStorage, OCOM_PROGRAM_NAME);
    // if(!mto_read(cfgEntry, &settings, 0, sizeof(settings)))
    // {
    //     free_handle(cfgEntry);
    //     free_handle(cfgStorage);
    //     return;
    // }

    // free_handle(cfgEntry);
    // free_handle(cfgStorage);

    settings.timeoutTime = 60;
    settings.shutdownTime = 1800;
    settings.sockType = 0;

    socket = createSocket(settings.sockType, NULL_HANDLE);
    mto_flush(socket);

    lastContact = getCurrentTime();
    while (getCurrentTime() - lastContact < settings.shutdownTime)
    {   
        thread_yield

        available = mto_tell(socket);
        if(available > 0)
        {
            com_recive();
            lastContact = getCurrentTime();
        }
    }

    com_send(MT_DISCONNECT, NULL, 0, 0);
    
    free_handle(socket);

    thread_done
}

void com_send(msg_types_t msgType, const void* bodyBuffer, int bodyOffset, uint16_t bodySize)
{
    message_t msg;
    msg.msgType = msgType;
    msg.sessionId = sessionId;
    msg.msgSize = bodySize;
    mto_write(socket, &msg, 0, sizeof(message_t));

    if(bodyBuffer != NULL)
        mto_write(socket, bodyBuffer, bodyOffset, bodySize);

    mto_flush(socket);
}

int Test(int i){
return i;
}

void com_recive(void)
{
    if(com_timeOutHandler(sizeof(message_t)))
        goto finally;
    
    message_t msg;
    if(!mto_read(socket, &msg, 0, sizeof(message_t)))
        goto finally;

    if(com_timeOutHandler(msg.msgSize))
        goto finally;

    if(msg.sessionId != sessionId)
    {
        if(msg.msgType == MT_HS_INIT)
            com_handshake();
        else
            com_send(MS_NOT_AVAILABLE, NULL, 0, 0);

        goto finally;
    }

    switch (msg.msgType)
    {
    case MT_DISCONNECT:
        com_disconnect();
        break;

    case MT_THREADS_GET:
        threads_get();
        break;
    case MT_THREADS_START:
        threads_start();
        break;
    case MT_THREADS_KILL:
        threads_kill();
        break;

    case MT_TIME_GET:
        time_get();
        break;
    case MT_TIME_SET:
        time_set();
        break;

    case MT_STORAGE_GET:
        storage_get();
        break;
    case MT_ENTRY_ADD:
        entry_add();
        break;
    case MT_ENTRY_UPDATE:
        entry_update(msg.msgSize);
        break;
    case MT_ENTRY_REMOVE:
        entry_remove();
        break;
    case MT_ENTRY_READ:
        entry_read();
        break;

    case MT_GET_SENS_VAL:
        get_sens_val();
        break;
    
    default:
        com_send(MT_UNKNOWN, NULL, 0, 0);
        mto_flush(socket);
        break;
    }

finally:
    mto_flush(socket);
}

uint16_t createSessionId(void)
{
    sessionId-=2;
    return sessionId;

    // return lfsr();
}

BOOLEAN com_timeOutHandler(uint16_t msgSize)
{
    time_stamp_t timeOutTimeStamp = getCurrentTime();

    while (mto_tell(socket) < msgSize)
    {        
        if(getCurrentTime() - timeOutTimeStamp > settings.timeoutTime)
        {
            lcd.setCursor(0, 1);
            lcd.print("Timed Out");
            com_send(MT_TIMEOUT, NULL, 0, 0);
            mto_flush(socket);
            return TRUE;
        }

        delay(1000);
    }

    return FALSE;
}

void com_disconnect(void)
{
    com_send(MT_AOK, NULL, 0, 0);

    sessionId = 1;
}

BOOLEAN com_handshake(void)
{
    version_t otherVersion;
    mto_read(socket, &otherVersion, 0, sizeof(version_t));

    if(OCOM_VERSION.major != otherVersion.major)
    {
        com_send(MT_HS_NAK, &OCOM_VERSION, 0, sizeof(version_t));
        return FALSE;
    }

    sys_info_t sysinfo = {VERSION, MTO_VERSION, OCOM_VERSION, 0};

    sessionId = createSessionId();
    com_send(MT_HS_ACK, &sysinfo, 0, sizeof(sys_info_t));

    return TRUE;
}

void threads_get(void)
{
    #ifndef MTO_LIGHT
    com_send(MT_THREAD_INFO, NULL, 0, 14 * MAX_THREADS);
    #elif
    com_send(MT_THREAD_INFO, NULL, 0, 9 * MAX_THREADS);
    #endif // !MTO_LIGHT

    for (uint8_t i = 0; i < MAX_THREADS; i++)
    {
        mto_write(socket, &i, 0, 1);
        mto_write(socket, &(threads[i].status), 0, 4);
        mto_write(socket, &(threads[i].status), 0, 4);
        #ifndef MTO_LIGHT
        BOOLEAN found = FALSE;
        for (int j = 0; j < programCount; j++)
        {
            if(threads[i].ep == programs[j].entry_ptr)
            {
                mto_write(socket, &(programs[j].name.cstr), 0, 5);
                found = true;
                break;
            }
        }
        if(!found)
        {
            char empty[5] = {' ',' ',' ',' ',' '};
            mto_write(socket, &empty, 0, 5);
        }
        #endif // !MTO_LIGHT
    }
}

void threads_start(void)
{
    static_id_t progName;
    mto_read(socket, &progName, 0, sizeof(static_id_t));

    HANDLE_t handle = startProgram(progName.cstr);
    if(handle == NULL_HANDLE)
        com_send(MT_NOK, NULL, 0, 0);
    else
        com_send(MT_AOK, NULL, 0, 0);
}

void threads_kill(void)
{
    uint8_t tid;
    mto_read(socket, &tid, 0, sizeof(uint8_t));

    if(tid >= MAX_THREADS)
        com_send(MT_NOK, NULL, 0, 0);
    else
    {
        threads[tid].status = TS_DONE;
        com_send(MT_AOK, NULL, 0, 0);
    }
}

void time_get(void)
{
    time_stamp_t ts = getCurrentTime();
    com_send(MT_TIME_INFO, &ts, 0, sizeof(ts));
}

void time_set(void)
{
    time_stamp_t ts;
    mto_read(socket, &ts, 0, sizeof(ts));

    setTime(ts);

    com_send(MT_AOK, NULL, 0, 0);
}

void storage_get(void)
{
    storage_type_t st;
    mto_read(socket, &st, 0, sizeof(st));

    uint16_t size;
    HANDLE_t hStorage = getStorage(st, &size);

    if(hStorage == NULL_HANDLE)
    {
        com_send(MT_NOK, NULL, 0, 0);
        return;
    }

    storage_info_t info;
    com_send(MT_STORAGE_INFO, NULL, 0, size * sizeof(static_id_t) + sizeof(info));
    mto_read(hStorage, &info, 0, sizeof(info));
    mto_write(socket, &info, 0, sizeof(info));

    static_id_t ids[8];
    for(int i = 0; i < size / ARRAY_SIZE(ids) + 1; i++)
    {
        int alen = listEntries(hStorage, ids, i, ARRAY_SIZE(ids));
        mto_write(socket, ids, 0, alen * sizeof(static_id_t));
    }

    free_handle(hStorage);
}

void entry_add(void)
{
    storage_type_t st;
    mto_read(socket, &st, 0, sizeof(st));
    static_id_t name;
    mto_read(socket, &name, 0, sizeof(name));

    HANDLE_t hStorage = getStorage(st, NULL);

    if(hStorage != NULL_HANDLE)
    {
        HANDLE_t hEntry = addEntry(hStorage, name.cstr);
        if(hEntry != NULL_HANDLE)
        {
            free_handle(hEntry);
            free_handle(hStorage);

            com_send(MT_AOK, NULL, 0, 0);
            return;
        }

        free_handle(hStorage);
    }

    com_send(MT_NOK, NULL, 0, 0);
}

void entry_update(uint16_t size)
{
    size = size - sizeof(storage_type_t) - sizeof(static_id_t);

    storage_type_t st;
    mto_read(socket, &st, 0, sizeof(st));
    static_id_t name;
    mto_read(socket, &name, 0, sizeof(name));

    HANDLE_t hStorage = getStorage(st, NULL);

    if(hStorage != NULL_HANDLE)
    {
        HANDLE_t hEntry = getEntry(hStorage, name.cstr);
        if(hEntry != NULL_HANDLE)
        {
            mto_seek(hEntry, 0);

            uint8_t buffer[32];
            for (int i = 0; i < size / ARRAY_SIZE(buffer) + 1; i++)
            {
                int toWrite = mto_read(socket, &buffer, 0, ARRAY_SIZE(buffer));
                mto_write(hEntry, &buffer, 0, toWrite);
            }

            mto_flush(hEntry);

            com_send(MT_AOK, NULL, 0, 0);

            free_handle(hEntry);
            free_handle(hStorage);
            return;
        }

        free_handle(hStorage);
    }

    com_send(MT_NOK, NULL, 0, 0);
}

void entry_remove(void)
{
    storage_type_t st;
    mto_read(socket, &st, 0, sizeof(st));
    static_id_t name;
    mto_read(socket, &name, 0, sizeof(name));

    HANDLE_t hStorage = getStorage(st, NULL);

    if(hStorage != NULL_HANDLE)
    {
        if(removeEntry(hStorage, name.cstr))
        {
            com_send(MT_AOK, NULL, 0, 0);
            free_handle(hStorage);
            return;
        }

        free_handle(hStorage);
    }

    com_send(MT_NOK, NULL, 0, 0);
}

void entry_read(void)
{
    storage_type_t st;
    mto_read(socket, &st, 0, sizeof(st));
    static_id_t name;
    mto_read(socket, &name, 0, sizeof(name));
    int seeker, length = 0;
    mto_read(socket, &seeker, 0, sizeof(seeker));
    mto_read(socket, &length, 0, sizeof(length));

    HANDLE_t hStorage = getStorage(st, NULL);

    if(hStorage != NULL_HANDLE)
    {
        HANDLE_t hEntry = getEntry(hStorage, name.cstr);
        if(hEntry != NULL_HANDLE)
        {
            mto_seek(hEntry, seeker);

            int flen = mto_tell(hEntry);
            com_send(MT_ENTRY_CONTENT, NULL, 0, (length > flen ? flen : length));

            uint8_t buffer[32];
            for (int i = 0; i < flen / ARRAY_SIZE(buffer) + 1; i++)
            {
                int alen = mto_read(hEntry, &buffer, 0, ARRAY_SIZE(buffer));
                if(alen < 0) break;
                mto_write(socket, &buffer, 0, alen);

                if(alen < ARRAY_SIZE(buffer))
                    break;
            }

            free_handle(hEntry);
            free_handle(hStorage);
            return;
        }

        free_handle(hStorage);
    }

    com_send(MT_NOK, NULL, 0, 0);
}

extern int sensorDriverCount;
extern sens_driver_t *sensorDrivers;
void get_sens_val(void)
{
    static_id_t sensName;
    mto_read(socket, &sensName, 0, sizeof(sensName));

    HANDLE_t dStore = getStorage(ST_DEVICES, NULL);

    if(dStore == NULL_HANDLE)
    {
        com_send(MT_NOK, NULL, 0, 0);
        return;
    }

    HANDLE_t hsensor = getEntry(dStore, sensName.cstr);

    if(hsensor == NULL_HANDLE)
    {
        com_send(MT_NOK, NULL, 0, 0);
        free_handle(dStore);
        return;
    }

    mto_seek(hsensor, 0);

    sens_def_t sdef;
    if(!mto_read(hsensor, &sdef, 0, sizeof(sdef)))
    {
        com_send(MT_NOK, NULL, 0, 0);
        free_handle(dStore);
        free_handle(dStore);
        return;
    }

    BOOLEAN success = FALSE;
    for (int i = 0; i < sensorDriverCount; i++)
    {
        if(mto_sidcmp(sdef.driver.cstr, sensorDrivers[i].name.cstr))
        {
            char valBuffer[16];
            int valSize = sensorDrivers[i].measure_fun_ptr(&sdef, &valBuffer);

            com_send(MT_SENS_VAL, &valBuffer, 0, valSize);
            success = TRUE;
            break;
        }
    }     

    if(!success)  
        com_send(MT_NOK, NULL, 0, 0); 

    free_handle(hsensor);
    free_handle(dStore);
}