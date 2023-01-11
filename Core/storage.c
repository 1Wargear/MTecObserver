#include "storage.h"

storage_driver_t* drivers;
uint8_t driverCount;

void registerStorageDriver(storage_driver_t* storageDrivers, uint8_t size)
{
    drivers = storageDrivers;
    driverCount = size;
}

HANDLE_t addEntry(HANDLE_t storage, static_id_param_t name)
{
    storage_info_t info;
    if(!read(storage, &info, 0, sizeof(info)))
        return NULL_HANDLE;
    
    return drivers[info.driver].addEntry_ptr(storage, name);
}

HANDLE_t getEntry(HANDLE_t storage, static_id_param_t name)
{
    storage_info_t info;
    if(!read(storage, &info, 0, sizeof(info)))
        return NULL_HANDLE;
    
    return drivers[info.driver].getEntry_ptr(storage, name);
}

int removeEntry(HANDLE_t storage, static_id_param_t name)
{
    storage_info_t info;
    if(!read(storage, &info, 0, sizeof(info)))
        return NULL_HANDLE;
    
    return drivers[info.driver].removeEntry_ptr(storage, name);
}

int listEntries(HANDLE_t storage, static_id_t* buffer, int length)
{
    storage_info_t info;
    if(!read(storage, &info, 0, sizeof(info)))
        return NULL_HANDLE;
    
    return drivers[info.driver].listEntries_ptr(storage, buffer, length);
}

HANDLE_t getStorage(storage_type_t storageType, uint16_t *size)
{
    for (int i = 0; i < driverCount; i++)
    {
        if(drivers[i].supportedStorageTypes & storageType)
        {
            HANDLE_t handle = drivers[i].createInstance_ptr(storageType);

            if(handle == NULL_HANDLE)
                break;

            storage_info_t info;
            if(!read(handle, &info, 0, sizeof(info)))
                return NULL_HANDLE;

            *size = info.size;
            info.driver = i;
            info.type = storageType;

            write(handle, &info, 0, sizeof(info));

            return handle;
        }
    }
    
    return NULL_HANDLE;
}