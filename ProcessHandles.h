#ifndef _PROCESS_HANDLES_H_
#define _PROCESS_HANDLES_H_
#include <Vector>
#include <Windows.h>

#define SystemHandleInformation 16 

struct handleData_t {
    DWORD handleOwnerId;
    DWORD handleValue;
    DWORD handleAccess;

};


std::vector< handleData_t >FindProcessHandles( int targetPID );

#endif
