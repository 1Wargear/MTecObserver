#ifndef MTEC_OBSERVER
#define MTEC_OBSERVER

#ifdef TEST
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
#else
#include <stdint.h>
#endif 

typedef uint32_t HANDLE_t;
#define NULL_HANDLE 0x00000000
#define NULL_WRITEABLE_HANDLE_MASK 0x80000000

#ifndef NULL
#define NULL 0
#endif

#ifndef BOOLEAN
#define BOOLEAN uint8_t
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define STREAMABLE_DATA_SIZE 16
#define MAX_HANDLES_IN_FLIGHT 16

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct version_t
{
    char major;
    char minor;
    char patch;
    char build;

} version_t;
extern const version_t MTO_VERSION;

typedef struct static_id_t
{
    char cstr[5];

} static_id_t;
typedef const char* static_id_param_t;

typedef struct device_addr_t
{
    uint8_t ifType;
    uint16_t addr;

} device_addr_t;

typedef struct stream_t
{
    int (*read_ptr)(void* buffer, int offset, int length, uint8_t* streamData);
    int (*write_ptr)(const void* buffer, int offset, int length, uint8_t* streamData);
    int (*seek_ptr)(int length, uint8_t* streamData);
    int (*tell_ptr)(uint8_t* streamData);
    int (*flush_ptr)(uint8_t* streamData);

    void (*close_ptr)(uint8_t* streamData);

    uint8_t data[STREAMABLE_DATA_SIZE];

} stream_t;

int mto_read(HANDLE_t handle, void* buffer, int offset, int length);
int mto_write(HANDLE_t handle, const void* buffer, int offset, int length);
int mto_seek(HANDLE_t handle, int length);
int mto_tell(HANDLE_t handle);
int mto_flush(HANDLE_t handle);

HANDLE_t createHandle(stream_t *streamable);
inline BOOLEAN checkHandle(HANDLE_t handle);
void free_handle(HANDLE_t handle);

void* mto_alloc(void* ptr, int size);
void* mto_memcpy(const void* src, void* dst, int length);
void zero(void* ptr, int length);
BOOLEAN is_zero(void* ptr, int length);
unsigned lfsr(void);

BOOLEAN mto_sidcmp(static_id_param_t a, static_id_param_t b);

void onTimerWakeup(void);
void onUserWakeup(void);

#endif // !MTEC_OBSERVER