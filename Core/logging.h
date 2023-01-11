#ifndef MTO_LOGGING
#define MTO_LOGGING

#include "mto.h"

enum LogLvl
{
    LL_Trace = 0,
    LL_Info = 1,
    LL_Warn = 2,
    LL_Error = 3
};

enum ErrCode
{
    ERR_ALL = 0,
    ERR_NODRIVER = 1
};


//System API
void err(uint8_t code);

//User API
void log(uint8_t lvl, char* msg, uint8_t length);

#endif // !MTO_LOGGING