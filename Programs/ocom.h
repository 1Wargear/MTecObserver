#ifndef MTO_OCOM
#define MTO_OCOM

#include <mto.h>
#include <mto_time.h>
#include <socket.h>

typedef struct com_settings_t
{
    time_stamp_t timeoutTime;
    time_stamp_t shutdownTime;
    socket_type_t sockType;

} com_settings_t;

typedef enum msg_types
{
    MT_AOK = 0x00,
    MT_NOK = 0x01,
    MT_TIMEOUT = 0x02,
    MT_DISCONNECT = 0x03,
    MS_NOT_AVAILABLE = 0x04,
    MT_UNKNOWN = 0x05,

    MT_BEGIN = 0x06,
    MT_END = 0x07,

    MT_HS_INIT = 0x10,
    MT_HS_ACK = 0x11,
    MT_HS_NAK = 0x12,

    MT_THREADS_GET = 0x13,
    MT_THREAD_INFO = 0x014,
    MT_THREADS_START = 0x15,
    MT_THREADS_KILL = 0x16,

    MT_TIME_SET = 0x17,
    MT_TIME_GET = 0x18,
    MT_TIME_INFO = 0x19,

    MT_STORAGE_GET = 0x20,
    MT_STORAGE_INFO = 0x21,
    MT_ENTRY_ADD = 0x22,
    MT_ENTRY_REMOVE = 0x23,
    MT_ENTRY_UPDATE = 0x24,
    MT_ENTRY_READ = 0x25,
    MT_ENTRY_CONTENT = 0x26,

    MT_GET_SENS_VAL = 0x30,
    MT_SENS_VAL = 0x31,

    MT_ERROR = 0xFFFFFFFF
} msg_types_t;

typedef struct message_t
{
    msg_types_t msgType;
    uint16_t sessionId;
    uint16_t msgSize;
} message_t;

typedef struct sys_info_t
{
    version_t version;
    version_t mtoVersion;
    version_t ocomVersion;   
    uint8_t light; 
    
} sys_info_t;

extern const version_t OCOM_VERSION;

void com_main(void);
#define OCOM_PROGRAM_NAME "OCOMP"
#define OCOM_PROGRAM { OCOM_PROGRAM_NAME, &com_main }

void com_send(msg_types_t msgType, const void* bodyBuffer, int bodyOffset, uint16_t bodySize);
void com_recive(void);

uint16_t createSessionId(void);
BOOLEAN com_timeOutHandler(uint16_t msgSize);
void com_disconnect(void);

BOOLEAN com_handshake(void);

void threads_get(void);
void threads_start(void);
void threads_kill(void);


void time_get(void);
void time_set(void);

void storage_get(void);
void entry_add(void);
void entry_update(uint16_t size);
void entry_remove(void);
void entry_read(void);

void get_sens_val(void);

#endif // !MTO_OCOM