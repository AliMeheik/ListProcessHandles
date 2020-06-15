#include "Util.h"
#include "Windows.h"
#include <TlHelp32.h> 
#include <string>

int GetPID( const wchar_t* processName ) {
    PROCESSENTRY32 process_buffer;
    int processId = 0;
	HANDLE process_snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

	if( process_snapshot != INVALID_HANDLE_VALUE ) {
		process_buffer.dwSize = sizeof( PROCESSENTRY32 );
		if( Process32First( process_snapshot, &process_buffer ) ) {
			do {
				if( !_wcsicmp( processName, process_buffer.szExeFile ) ) {
					processId = process_buffer.th32ProcessID;
                    break;
				}
			} while( Process32Next( process_snapshot, &process_buffer ) );
		}
        CloseHandle( process_snapshot );
	}
    return processId;
}
