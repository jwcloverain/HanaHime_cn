#pragma once
#include <Windows.h>
#include <iostream>


VOID WINAPI SetConsole();
VOID WriteMemory(LPVOID lpAddress, LPCVOID lpBuffer, SIZE_T nSize);
BOOL SetHook(DWORD dwRawAddr, DWORD dwTarAddr, SIZE_T szRawSize);