#ifndef _IMPORTED_FUNCTIONS_H_
#define _IMPORTED_FUNCTIONS_H_
#include <Windows.h>
namespace internals {
    typedef struct _CLIENT_ID
    {
        HANDLE UniqueProcess;
        HANDLE UniqueThread;
    } CLIENT_ID, *PCLIENT_ID;

    typedef struct _UNICODE_STRING
    {
        USHORT Length;
        USHORT MaximumLength;
        PWSTR  Buffer;
    } UNICODE_STRING, *PUNICODE_STRING;

    typedef struct _OBJECT_ATTRIBUTES {
        ULONG           Length;
        HANDLE          RootDirectory;
        PUNICODE_STRING ObjectName;
        ULONG           Attributes;
        PVOID           SecurityDescriptor;
        PVOID           SecurityQualityOfService;
    }  OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
}

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
    USHORT UniqueProcessId;
    USHORT CreatorBackTraceIndex;
    UCHAR ObjectTypeIndex;
    UCHAR HandleAttributes;
    USHORT HandleValue;
    PVOID Object;
    ULONG GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;


using __NtOpenProcess       = NTSTATUS ( __stdcall* )( PHANDLE, ACCESS_MASK, internals::POBJECT_ATTRIBUTES, internals::PCLIENT_ID );
using __NtDuplicateObject   = NTSTATUS ( __stdcall* )( HANDLE, HANDLE, HANDLE, PHANDLE, ACCESS_MASK, ULONG, ULONG );

extern __NtOpenProcess      _NtOpenProcess;
extern __NtDuplicateObject  _NtDuplicateObject;

uintptr_t GetFunctionAddress( LPCSTR module, LPCSTR function_name );

#endif
