#include "ProcessHandles.h"
#include "ImportedFunctions.h"
#include "Util.h"
#include <iostream>
#include <string>

void LoadFunctions() {}

int main() {

    do {

        std::wstring processName;
        printf( "Enter target process name: " );
        getline( std::wcin, processName );
        int processId = GetPID( processName.c_str() );

        if( !processId ) {
            fprintf( stderr, "Failed to get process id\n" );
            break;
        }

        _NtOpenProcess = reinterpret_cast< __NtOpenProcess >    ( GetFunctionAddress( "ntdll.dll", "NtOpenProcess" ) );
        _NtDuplicateObject = reinterpret_cast< __NtDuplicateObject >( GetFunctionAddress( "ntdll.dll", "NtDuplicateObject" ) );

        for( auto i : FindProcessHandles( processId ) ) {
            printf( "Handle is open by process: %d\n", i.handleOwnerId );
            printf( "Handle value: %x\n", i.handleValue );
            printf( "Handle access: %x\n", i.handleAccess );
            printf( "\n" );
        }
    } while( 0 );


    system( "pause" );
    return 0;
}
