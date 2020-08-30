
#ifndef APPLEWINEX_H
#define APPLEWINEX_H

#include <functional>


#ifdef _WINNT_

   /// <summary>
   /// Specialization of Win32's FindResource that looks for resources in our DLL
   /// </summary>
   HRSRC FindAppleWinResource(LPCSTR  lpName, LPCSTR  lpType);

   /// <summary>
   /// Specialization of Win32's SizeofResource that looks for resources in our DLL
   /// </summary>
   DWORD SizeofAppleWinResource(HRSRC hResInfo);

   /// <summary>
   /// Specialization of Win32's LoadResource that looks for resources in our DLL
   /// </summary>
   HGLOBAL LoadAppleWinResource(HRSRC hResInfo);

#endif

/// <summary>
/// A hook that calls the original WinMain, passing in this DLLs
/// instance handle
/// </summary>
void AppleWinMain(void);

/// <summary>
/// Function that gets called from the main loop so that my extensions
/// can carry out their business.
/// </summary>
/// <param name=""></param>
void ServiceApplewinExtensions(void);

void ApplewinInvokeSynchronous(const std::function<void()>& function);

#endif
