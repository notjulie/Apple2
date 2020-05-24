
#include <Windows.h>
#include <deque>
#include <future>

/// <summary>
/// Instance handle of the DLL, vor access to resources
/// </summary>
static HINSTANCE myDLLInstanceHandle;

/// <summary>
/// tasks requested from other threads
/// </summary>
static std::deque<std::packaged_task<void()>> taskQueue;
static std::mutex taskQueueMutex;

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


/// <summary>
/// Function that gets called from the main loop so that my extensions
/// can carry out their business.
/// </summary>
/// <param name=""></param>
void ServiceApplewinExtensions(void)
{
	for (;;)
	{
		// get the next task
		std::packaged_task<void()> task;
		{
			std::lock_guard<std::mutex> lock(taskQueueMutex);
			if (taskQueue.empty())
				return;
			task = std::move(taskQueue.front());
			taskQueue.pop_front();
		}

		// execute
		task();
	}
}


void ApplewinInvoke(const std::function<void()>& function)
{
	// package up a task
	std::packaged_task<void()> task(function);

	// snag its future
	auto future = task.get_future();

	// enqueue it
	{
		std::lock_guard<std::mutex> lock(taskQueueMutex);
		taskQueue.push_back(std::move(task));
	}
	
	// wait on it
	future.wait();
}
