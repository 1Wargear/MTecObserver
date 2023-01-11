#ifndef MTO_STORAGE
#define MTO_STORAGE

#include "mto.h"

typedef struct storage_driver_t
{
    uint16_t supportedStorageTypes;

    HANDLE_t (*addEntry_ptr)(HANDLE_t instance, static_id_param_t name);
    HANDLE_t (*getEntry_ptr)(HANDLE_t instance, static_id_param_t name);
    int (*removeEntry_ptr)(HANDLE_t instance, static_id_param_t name);
    int (*listEntries_ptr)(HANDLE_t instance, static_id_t* buffer, int length);

    HANDLE_t (*createInstance_ptr)(uint16_t storageType);

} storage_driver_t;

typedef uint16_t storage_type_t;

enum DefaultStorageTypes
{
    ST_CONFIG   = 0b00000001,
    ST_TMP      = 0b00000010,
    ST_LOGS     = 0b00000100,
    ST_DRIVERS  = 0b00001000,
    ST_DEVICES  = 0b00010000,
    ST_PROGRAMS = 0b00100000,
    ST_TASKS    = 0b01000000,
    ST_VALUES   = 0b10000000
};

typedef struct storage_info_t
{
    storage_type_t type;
    uint8_t driver;
    uint8_t size;

} storage_info_t;


void registerStorageDriver(storage_driver_t* storageDrivers, uint8_t size);

HANDLE_t addEntry(HANDLE_t storage, static_id_param_t name);
HANDLE_t getEntry(HANDLE_t storage, static_id_param_t name);
int removeEntry(HANDLE_t storage, static_id_param_t name);
int listEntries(HANDLE_t storage, static_id_t* buffer, int length);

HANDLE_t getStorage(storage_type_t storageType, uint16_t *size);
#endif // !MTO_STORAGE