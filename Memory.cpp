#include "Memory.h"

std::unordered_map<void*, Memory::PatchInfo> Memory::patches;

void Memory::PatchBytes(void* address, const char* bytes)
{
	// Get the length of the bytes.
	auto len = strlen(bytes);

	// Store the original bytes.
	PatchInfo info;
	info.address = address;
	info.originalBytes.resize(len);
	memcpy(info.originalBytes.data(), address, len);

	// Patch the bytes.
	DWORD oldProtect;
	VirtualProtect(address, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(address, bytes, len);
	VirtualProtect(address, len, oldProtect, &oldProtect);

	// Store the patch info.
	patches[address] = info;
}

void Memory::NopBytes(void* address, const size_t len)
{
	std::vector<char> nops(len, 0x90);
	PatchBytes(address, nops.data());
}

void Memory::RestoreBytes(void* address)
{
	auto it = patches.find(address);
	if (it != patches.end())
	{
		// Restore the original bytes.
		const auto& info = it->second;
		DWORD oldProtect;
		VirtualProtect(info.address, info.originalBytes.size(), PAGE_EXECUTE_READWRITE, &oldProtect);
		memcpy(info.address, info.originalBytes.data(), info.originalBytes.size());
		VirtualProtect(info.address, info.originalBytes.size(), oldProtect, &oldProtect);

		// Remove the patch info.
		patches.erase(it);
	}
}

uintptr_t Memory::PatternScan(uintptr_t module, const char* signature)
{
	static auto patternToByte = [](const char* pattern)
	{
		auto       bytes = std::vector<int>{};
		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else
			{
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	const auto dosHeader = (PIMAGE_DOS_HEADER)module;
	const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

	const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto       patternBytes = patternToByte(signature);
	const auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

	const auto s = patternBytes.size();
	const auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			// return reinterpret_cast<uintptr_t>(&scanBytes[i]);
			// Return the address of the pattern.
			return module + i;
		}
	}
	return NULL;
}

void* Memory::AllocateNearbyMemory(uintptr_t address, size_t size)
{
	// Get the system allocation granularity.
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// Calculate the start and end addresses.
	auto start = (address > si.dwAllocationGranularity) ? (address - si.dwAllocationGranularity) : 0;
	auto end = address + si.dwAllocationGranularity;

	// Iterate through the memory range and find a suitable location.
	MEMORY_BASIC_INFORMATION mbi;
	while (start < end && VirtualQuery((LPCVOID)start, &mbi, sizeof(mbi)))
	{
		// If the memory is free and large enough, allocate it.
		if (mbi.State == MEM_FREE && mbi.RegionSize >= size)
		{
			auto ptr = VirtualAlloc((LPVOID)start, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (ptr)
				return ptr;
		}

		// Move to the next memory region.
		start += si.dwAllocationGranularity;
	}

	// If we couldn't find a suitable location, just allocate the memory anywhere.
	return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}


