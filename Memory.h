#pragma once
#include <unordered_map>
#include <vector>
#include <Windows.h>

class Memory
{
public:
	static void PatchBytes(void* address, const char* bytes);
	static void NopBytes(void* address, size_t len);
	static void RestoreBytes(void* address);
	static uintptr_t PatternScan(uintptr_t module, const char* signature);
	static void* AllocateNearbyMemory(uintptr_t address, size_t size);
	//static void CreateTrampolite(void* address, void* destination, size_t length);
	//static void RemoveTrampolite(void* address);
private:
	struct PatchInfo {
		std::vector<BYTE> originalBytes;
		void* address;
	};

	static std::unordered_map<void*, PatchInfo> patches;
};
