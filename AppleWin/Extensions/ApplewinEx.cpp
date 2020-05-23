
#include <Windows.h>

/// <summary>
/// Instance handle of the DLL, vor access to resources
/// </summary>
static HINSTANCE myDLLInstanceHandle;

/// <summary>
/// DLL entry point; needed in order to capture the DLL's instance handle
/// </summary>
BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		myDLLInstanceHandle = hinstDLL;
		break;

	default:
		break;
	}

	return TRUE;
}

/// <summary>
/// A hook that calls the original WinMain, passing in this DLLs
/// instance handle
/// </summary>
void AppleWinMain(void)
{
	WinMain(myDLLInstanceHandle, NULL, "", 0);
}

/// <summary>
/// Specialization of Win32's FindResource that looks for resources in our DLL
/// </summary>
HRSRC FindAppleWinResource(LPCSTR  lpName, LPCSTR  lpType)
{
	return FindResource(myDLLInstanceHandle, lpName, lpType);
}

/// <summary>
/// Specialization of Win32's SizeofResource that looks for resources in our DLL
/// </summary>
DWORD SizeofAppleWinResource(HRSRC hResInfo)
{
	return SizeofResource(myDLLInstanceHandle, hResInfo);
}

/// <summary>
/// Specialization of Win32's LoadResource that looks for resources in our DLL
/// </summary>
HGLOBAL LoadAppleWinResource(HRSRC hResInfo)
{
	return LoadResource(myDLLInstanceHandle, hResInfo);
}

