#include "sman.h"

#include <storage.h>
#include <mto_time.h>
#include <program.h>

extern int sensorDriverCount;
extern sens_driver_t *sensorDrivers;

#include <LiquidCrystal_I2C.h>
extern LiquidCrystal_I2C lcd;

// HANDLE_t createSensor(static_id_param_t name, static_id_param_t driver, device_addr_t addr)
// {
//     HANDLE_t storage = getStorage(ST_DEVICES, NULL);
//     HANDLE_t entry = addEntry(storage, name);
//     sens_def_t sensor = { name, driver, addr.ifType, addr.addr, NULL_HANDLE };
//     write(entry, &sensor, 0, sizeof(sensor));
//     free_handle(storage);

//     stream_t dsInfo = { &read, &write, entry };
//     return createHandle(&dsInfo);
// }

// void deleteSensor(static_id_param_t name)
// {
//     HANDLE_t storage = getStorage(ST_DEVICES);
//     removeEntry(storage, name);
//     free_handle(storage);
// }

int readDsInfo(void* args, void* buffer, int offset, int length)
{
    HANDLE_t entryHandle = *(HANDLE_t*)args;
    mto_seek(entryHandle, sizeof(sens_def_t));
    return mto_read(entryHandle, buffer, offset, length);
}

int writeDsInfo(void* args, void* buffer, int offset, int length)
{
    HANDLE_t entryHandle = *(HANDLE_t*)args;
    mto_seek(entryHandle, sizeof(sens_def_t));
    return mto_write(entryHandle, buffer, offset, length);
}

void sens_man_main(void)
{
    uint16_t storageSize = 0;
    HANDLE_t tStorage = getStorage(ST_DEVICES, &storageSize);
    HANDLE_t vStorage = getStorage(ST_VALUES, NULL);

    static_id_t buffer[8];
    for(int i = 0; i < storageSize / ARRAY_SIZE(buffer) + 1; i++)
    {
        int sRead = listEntries(tStorage, buffer, i, ARRAY_SIZE(buffer));

        for (int i = 0; i < sRead; i++)
        {
            HANDLE_t hsensor = getEntry(tStorage, buffer[i].cstr);

            sens_def_t sdef;
            mto_seek(hsensor, 0);
            mto_read(hsensor, &sdef, 0, sizeof(sdef)-sizeof(HANDLE_t));
            for (int i = 0; i < sensorDriverCount; i++)
            {
                if(mto_sidcmp(sdef.driver.cstr, sensorDrivers[i].name.cstr))
                {
                    char valBuffer[MAX_VALUE_SIZE];
                    int valSize = sensorDrivers[i].measure_fun_ptr(&sdef, &valBuffer);
                    HANDLE_t valEntry = getEntry(vStorage, sdef.name.cstr);

                    time_stamp_t timeStamp = getCurrentTime();
                    mto_write(valEntry, &timeStamp, 0, sizeof(timeStamp));
                    mto_write(valEntry, &"\t", 0, 1);
                    mto_write(valEntry, valBuffer, 0, valSize);
                    mto_write(valEntry, &"\n", 0, 1);

                    free_handle(valEntry);

                    break;
                }
            }           

            free_handle(hsensor);

            // thread_yield
        }
    }

    free_handle(tStorage);
    free_handle(vStorage);

    thread_done
}