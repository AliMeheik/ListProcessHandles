#include "ProcessHandles.h"
#include "ImportedFunctions.h"
#include <winternl.h>
#pragma comment( lib, "ntdll.lib" )

HANDLE NtOpenProcessWrapper( DWORD access_rights, DWORD process_id ) {
    internals::CLIENT_ID internals = { ( HANDLE )process_id, NULL };
    internals::OBJECT_ATTRIBUTES object_att;
    InitializeObjectAttributes( &object_att, NULL, NULL, NULL, NULL );
    HANDLE processHandle = NULL;
    NTSTATUS ntStatus = _NtOpenProcess( &processHandle, access_rights, &object_att, &internals );
    return processHandle;
}


std::vector< handleData_t >FindProcessHandles( int targetPID ) {

    NTSTATUS status = 0;
    std::vector< handleData_t > handles;
    PBYTE buffer = nullptr; 

    do {
        //---------handle enumeration

        uint32_t bufferSize = 0x1000; //initial size
        buffer = new byte[bufferSize];

        status = NtQuerySystemInformation( static_cast< SYSTEM_INFORMATION_CLASS >( SystemHandleInformation ), buffer, bufferSize, reinterpret_cast< PULONG >( &bufferSize ) );

        if( !NT_SUCCESS( status ) ) {

            while( status == 0xC0000004 ) { //status info length mismatch
                delete[] buffer;
                buffer = new byte[ bufferSize ];
                status = NtQuerySystemInformation( static_cast< SYSTEM_INFORMATION_CLASS >( SystemHandleInformation ), buffer, bufferSize, reinterpret_cast< PULONG >( &bufferSize ) );
            }

            if( !NT_SUCCESS( status ) ) {
                fprintf( stdout, "Failed to enum process handles\n" );
                break;
            }

        }

        auto* pHandleInfo = reinterpret_cast< SYSTEM_HANDLE_INFORMATION* >( buffer );
        uint32_t handleCount = pHandleInfo->NumberOfHandles;
        auto* pHandleEntry = pHandleInfo->Handles;

        std::vector<SYSTEM_HANDLE_TABLE_ENTRY_INFO> handleEntries;

        for( uint32_t i = 0; i != handleCount; ++i ) {
            if( pHandleEntry[ i ].ObjectTypeIndex == 7 ) {  //TypeIndex 0x7 -> is ObjectType -> process 
                handleEntries.push_back( pHandleEntry[ i ] );
            }
        }

        delete[] buffer;

        //---------Filter handles

        int processId = 0;
        int currentProcessId = GetCurrentProcessId();
        HANDLE hProcess = nullptr;

        for( auto i : handleEntries ) {

            processId = i.UniqueProcessId;


            if( processId == currentProcessId ) {
                continue;
            }

            HANDLE hSomeProcess  = NtOpenProcessWrapper( PROCESS_DUP_HANDLE, i.UniqueProcessId );

            if( !hSomeProcess ) {
                continue;
            }

            HANDLE duplicatedHandle = nullptr;
            status = _NtDuplicateObject( hSomeProcess, reinterpret_cast<HANDLE>( i.HandleValue ),GetCurrentProcess(), &duplicatedHandle, NULL, NULL, DUPLICATE_SAME_ACCESS  );
            if( !NT_SUCCESS( status ) ) {
                continue;
            }

            int processPID = GetProcessId( duplicatedHandle );

            if( processPID == targetPID ) {
                handleData_t handle;
                handle.handleAccess = i.GrantedAccess;
                handle.handleOwnerId = i.UniqueProcessId;
                handle.handleValue = i.HandleValue;

                handles.push_back( handle );

            }
        }

    } while( 0 );

    if( !NT_SUCCESS( status ) ) {
        fprintf( stderr, "Error: %x\n", status );
    }

    return handles;
}




