#include "ImportedFunctions.h"
#include <Windows.h>
__NtOpenProcess     _NtOpenProcess;
__NtDuplicateObject _NtDuplicateObject;

uintptr_t GetFunctionAddress( LPCSTR module, LPCSTR function_name ) {
    HMODULE handle = GetModuleHandleA( module ); 

    if( !handle ) {
        return 0;
    }

    return reinterpret_cast<uintptr_t>( GetProcAddress( handle, function_name ) );
}
