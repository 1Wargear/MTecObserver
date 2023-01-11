#include "mto.h"

#ifdef WIN32
#include <stdio.h>
#endif // !WIN32

stream_t* streams;
uint8_t streamsSize;

const version_t VERSION = {0x00};

HANDLE_t createHandle(stream_t *streamable)
{
    if(streams == NULL)
    {
        streams = mto_alloc(NULL, sizeof(stream_t));
        zero(streams, sizeof(stream_t));
    }

    for (int i = 0; i < streamsSize; i++)
    {
        if(is_zero(&(streams[i]), sizeof(stream_t)))
        {
            mto_memcpy(streamable, &streams[i], sizeof(stream_t));
            return i;
        }
    }

    if(mto_alloc(streams, sizeof(stream_t) * ++streamsSize) == NULL)
        return NULL_HANDLE;

    mto_memcpy(streamable, &streams[streamsSize - 1], sizeof(stream_t));
    return streamsSize - 1;
}

void free_handle(HANDLE_t handle)
{
    zero(&streams[handle], sizeof(stream_t));

    if(handle == streamsSize - 1)
    {
        mto_alloc(streams, sizeof(stream_t) * --streamsSize);
    }
}

#define SINDEX streams[handle]

int read(HANDLE_t handle, void* buffer, int offset, int length)
{
    if(!checkHandle(handle))
        return 0;

    if(SINDEX.read_ptr == NULL)
        return 0;

    return SINDEX.read_ptr(buffer, offset, length);
}

int write(HANDLE_t handle, void* buffer, int offset, int length)
{
    if(!checkHandle(handle))
        return 0;

    if(SINDEX.write_ptr == NULL)
        return 0;

    return SINDEX.write_ptr(buffer, offset, length);
}

int seek(HANDLE_t handle, int length)
{
    if(!checkHandle(handle))
        return 0;

    if(SINDEX.seek_ptr == NULL)
        return 0;

    return SINDEX.seek_ptr(length);
}

int tell(HANDLE_t handle)
{
    if(!checkHandle(handle))
        return 0;

    if(SINDEX.tell_ptr == NULL)
        return 0;

    return SINDEX.tell_ptr();
}

int flush(HANDLE_t handle)
{
    if(!checkHandle(handle))
        return 0;

    if(SINDEX.flush_ptr == NULL)
        return 0;

    return SINDEX.flush_ptr();
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
    #ifdef WIN32
    return realloc(ptr, size);
    #endif // !WIN32
}

void* mto_memcpy(void* src, void* const dst, int length)
{
    #ifdef WIN32
    return memcpy(src, dst, length);
    #endif // !WIN32
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