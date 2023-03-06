#include "mto.h"

#ifdef WIN32
#include <stdio.h>
#include <stdlib.h>
#endif // !WIN32
#ifdef ARDUINO
#include <stdio.h>
#include <stdlib.h>
#endif // !WIN32

#include <Arduino.h>

stream_t* streams;
uint8_t streamsSize;

const version_t MTO_VERSION = {0,1,0,0};

#define HADDR(h) h -1

#define SINDEX streams[HADDR(handle)]
#define SDATA SINDEX.data

HANDLE_t createHandle(stream_t *streamable)
{
    if(streams == NULL)
    {
        streams = (stream_t*)mto_alloc(NULL, sizeof(stream_t)*10);
        zero(streams, sizeof(stream_t) * 10);
        streamsSize = 10;
    }

    for (int i = 0; i < streamsSize; i++)
    {
        if(is_zero(&(streams[i]), sizeof(stream_t)))
        {
            mto_memcpy(streamable, &streams[i], sizeof(stream_t));
            return i + 1;
        }
    }

    // stream_t* newStremas = (stream_t*)mto_alloc(streams, sizeof(stream_t) * ++streamsSize);
    // if(newStremas  == NULL)
    //     return NULL_HANDLE;

    // mto_memcpy(streams, newStremas, sizeof(stream_t) * streamsSize);
    // mto_alloc(streams, 0);
    // streams = newStremas;

    // mto_memcpy(streamable, &streams[streamsSize - 1], sizeof(stream_t));
    // return streamsSize;
    return NULL_HANDLE;
}

#include <LiquidCrystal_I2C.h>
extern LiquidCrystal_I2C lcd;

void free_handle(HANDLE_t handle)
{
    if(!checkHandle(handle))
        return;

    if(SINDEX.close_ptr != NULL)
        SINDEX.close_ptr(SDATA);

    zero(&streams[HADDR(handle)], sizeof(stream_t));

    // if(handle == streamsSize)
    // {
    //     int newSize = sizeof(stream_t) * --streamsSize;
    //     stream_t* newStreams = (stream_t*)mto_alloc(streams, newSize);
    //     mto_memcpy(streams, newStreams, sizeof(stream_t) * streamsSize);
    //     mto_alloc(streams, 0);
    //     streams = newStreams;
    // }
}

int mto_read(HANDLE_t handle, void* buffer, int offset, int length)
{
    if(!checkHandle(handle))
        return 0;

    if(SINDEX.read_ptr == NULL)
        return 0;

    return SINDEX.read_ptr(buffer, offset, length, SDATA);
}

int mto_write(HANDLE_t handle, const void* buffer, int offset, int length)
{
    if(!checkHandle(handle))
        return 0;

    if(SINDEX.write_ptr == NULL)
        return 0;

    return SINDEX.write_ptr(buffer, offset, length, SDATA);
}

int mto_seek(HANDLE_t handle, int length)
{
    if(!checkHandle(handle))
        return 0;

    if(SINDEX.seek_ptr == NULL)
        return 0;

    return SINDEX.seek_ptr(length, SDATA);
}

int mto_tell(HANDLE_t handle)
{
    if(!checkHandle(handle))
        return 0;

    if(SINDEX.tell_ptr == NULL)
        return 0;

    return SINDEX.tell_ptr(SDATA);
}

int mto_flush(HANDLE_t handle)
{
    if(!checkHandle(handle))
        return 0;

    if(SINDEX.flush_ptr == NULL)
        return 0;

    return SINDEX.flush_ptr(SDATA);
}

inline BOOLEAN checkHandle(HANDLE_t handle)
{
    if(handle == NULL_HANDLE)
        return FALSE;

    if(handle < 0 || handle > streamsSize)
        return FALSE;

    return TRUE;
}

void* mto_alloc(void* ptr, int size)
{
    return realloc(ptr, size);
}

void* mto_memcpy(const void* src, void* dst, int length)
{
    return memcpy(dst, src, length);
}

BOOLEAN mto_sidcmp(static_id_param_t a, static_id_param_t b)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if(a[i] != b[i])
            return FALSE;
    }

    return TRUE;
}

void zero(void* ptr, int length)
{
    uint8_t* bptr = (uint8_t*)ptr;
    for(int i = 0; i < length; i++)
    {
        bptr[i] = NULL;
    }
}

BOOLEAN is_zero(void* ptr, int length)
{
    uint8_t* bptr = (uint8_t*)ptr;
    for(int i = 0; i < length; i++)
    {
        if(bptr[i] != NULL)
            return FALSE;
    }

    return TRUE;
}

unsigned lfsr(void)
{
    static unsigned r = 1;
    unsigned b = r & 1;
    r = (r >> 1) ^ (-b & 0xc3308398);
    return b;
}