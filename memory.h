#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <string>

class Memory
{
private:
	uintptr_t processId = 0;
	void* processHandle = nullptr;
	DWORD procID;

public:
	template <typename T> constexpr const T ReadMEM(uintptr_t& address) noexcept
	{
		ReadProcessMemory(processHandle, address, T value, sizeof(T), NULL);
		return value;
	}

	template <typename T> void WriteMEM(uintptr_t& address, T& value) noexcept
	{
		WriteProcessMemory(processHandle, address, &value, sizeof(T), NULL);
	}
	const uintptr_t GetModuleBaseAddr(const char* moduleName) noexcept
	{
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE || TH32CS_SNAPMODULE32, procID);
		if (hSnap != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);
			if (Module32First(hSnap, &modEntry))
			{
				do
				{
					if (!strcmp(modEntry.szModule, moduleName))
					{
						CloseHandle(hSnap);
						return (uintptr_t)modEntry.modBaseAddr;
					}
				} while (Module32Next(hSnap, &modEntry));
			}
		}
	}
};
