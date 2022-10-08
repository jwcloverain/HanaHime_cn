#include <Windows.h>
#include <detours.h>
#include "Tools.h"

DWORD g_dwExeImageBase = 0;
DWORD g_dwInputChar = 0;
DWORD g_dwOutputChar = 0;

typedef HFONT(WINAPI* pCreateFontA)(
int    cHeight,
	int    cWidth,
	int    cEscapement,
	int    cOrientation,
	int    cWeight,
	DWORD  bItalic,
	DWORD  bUnderline,
	DWORD  bStrikeOut,
	DWORD  iCharSet,
	DWORD  iOutPrecision,
	DWORD  iClipPrecision,
	DWORD  iQuality,
	DWORD  iPitchAndFamily,
	LPCSTR pszFaceName);
pCreateFontA rawCreateFontA = CreateFontA;

typedef HFONT(WINAPI* pCreateFontIndirectA)(
	const LOGFONTA* lplf);
pCreateFontIndirectA rawCreateFontIndirectA = CreateFontIndirectA;

HFONT WINAPI newCreateFontA(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
{
	pszFaceName = "SimHei";
	iCharSet = 0x86;
	return rawCreateFontA(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
}

HFONT WINAPI newCreateFontIndirectA(LOGFONTA* lplf)
{
	lplf->lfCharSet = 0x86;
	strcpy_s(lplf->lfFaceName, "SimHei");
	return rawCreateFontIndirectA(lplf);
}

VOID WriteTable()
{
	PBYTE pAlloc = 0;
	HANDLE hFile = 0;
	BOOL isRead = FALSE;

	hFile = CreateFileW(L"HanaHime_cn.table", GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		pAlloc = (PBYTE)VirtualAlloc(NULL, 0x43000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (pAlloc != NULL)
		{
			isRead = ReadFile(hFile, &pAlloc[0xFDE0], 0x33000, NULL, NULL);
			if (isRead)
			{
				WriteMemory((LPVOID)(g_dwExeImageBase + 0x234444), &pAlloc, sizeof(pAlloc));
				WriteMemory((LPVOID)(g_dwExeImageBase + 0x1C2538), &pAlloc[0xFDE0 + 0x32F00], 0x100);
			}
		}
		CloseHandle(hFile);
	}
}

VOID Patch()
{
	WriteTable();
	BYTE patchCharSet[] = { 0x86 };
	WriteMemory((LPVOID)(g_dwExeImageBase + 0x76B6F), patchCharSet, sizeof(patchCharSet));
}

VOID StartHook()
{
	//SetConsole();
	Patch();

	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)rawCreateFontA, newCreateFontA);
	DetourAttach(&(PVOID&)rawCreateFontIndirectA, newCreateFontIndirectA);
	DetourTransactionCommit();
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_dwExeImageBase = (DWORD)GetModuleHandleW(NULL);
		StartHook();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

VOID __declspec(dllexport) DirA() {}