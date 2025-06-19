#pragma once

#include <windows.h>
#include <wininet.h>
#include <tlhelp32.h>
#include <vector>
#include <iostream>
#include <wininet.h>
#include <TlHelp32.h>

#pragma comment(lib, "wininet.lib")

#include "print.h"

DWORD GetProcId(const char* name) {
    PROCESSENTRY32 pe = { sizeof(pe) };
    HANDLE hs = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(hs, &pe)) {
        do {
            if (_stricmp(pe.szExeFile, name) == 0) {
                CloseHandle(hs);
                return pe.th32ProcessID;
            }
        } while (Process32Next(hs, &pe));
    }
    CloseHandle(hs);
    return 0;
}

bool Inject(const char* pName, const str& dllPath) {

    DWORD pId = NULL;
    while (!pId) {
        pId = GetProcId(pName);
        Sleep(100);
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, pId);
    if (!hProc) return 1;

    void* alloc = VirtualAllocEx(hProc, 0, strlen(dllPath.val) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!alloc) {
        err("failed to alloc memory");
        return 1;
    }

    if (!WriteProcessMemory(hProc, alloc, dllPath.val, strlen(dllPath.val) + 1, 0)) {
        VirtualFreeEx(hProc, alloc, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return 1;
    }

    HANDLE hThr = CreateRemoteThread(
        hProc,
        0, 0,
        (LPTHREAD_START_ROUTINE)LoadLibraryA,
        alloc,
        0, 0
    );

    if (!hThr) {
        VirtualFreeEx(hProc, alloc, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return 1;
    }

    WaitForSingleObject(hThr, INFINITE);

    VirtualFreeEx(hProc, alloc, 0, MEM_RELEASE);
    CloseHandle(hThr);
    CloseHandle(hProc);
}