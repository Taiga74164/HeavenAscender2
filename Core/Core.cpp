#include "Core.h"
#include <Exports/Exports.h>
#include <codecvt>
#include <psapi.h>
#include <Utils.h>
#include "Memory.h"

#define AUTO_ASSEMBLE_TRAMPOLINE(ADDRESS, TRAMPOLINE_LENGTH, INSTRUCTIONS) \
	do { \
		auto allocMemory = Memory::AllocateNearbyMemory(ADDRESS, sizeof INSTRUCTIONS + 14); \
		Memory::CreateTrampoline(ADDRESS, allocMemory, TRAMPOLINE_LENGTH); \
		Memory::WriteInstructions(allocMemory, INSTRUCTIONS, sizeof INSTRUCTIONS, ADDRESS + TRAMPOLINE_LENGTH); \
	} while (0)

Core::Cheat cheat;

void Core::InitializeCheat()
{
	auto gameHandle = reinterpret_cast<uintptr_t>(GetModuleHandleA("game.dll"));

	LOG("[HD2Cheeto] Found game handle! Getting module information...");

	MODULEINFO moduleInfo;
	bool success = GetModuleInformation(GetCurrentProcess(), (HMODULE)gameHandle, &moduleInfo, sizeof(moduleInfo));
	if (!success)
	{
		LOG("[HD2Cheeto] Failed to get module information. Stopping...");
		return;
	}

	cheat.baseAddress = gameHandle;
	cheat.size = moduleInfo.SizeOfImage;

	LOG("[HD2Cheeto] Found game.dll at base address 0x%s, size %s.", std::format("{:X}", cheat.baseAddress), std::to_string(cheat.size));

	LOG("[HD2Cheeto] Press F1 to activate all cheats");
	LOG("[HD2Cheeto] Press F2 to deactivate all cheats");

	cheat.infGrenades = Memory::PatternScan(gameHandle, "41 ff 08 4a 8b 84 ed");
	cheat.infAmmo = Memory::PatternScan(gameHandle, "41 83 2c c2");
	cheat.infSyringe = Memory::PatternScan(gameHandle, "41 ff cf 3b c2 74 ?");
	cheat.infStamine = Memory::PatternScan(gameHandle, "f3 41 0f 11 08 8b 48 ? e8 ? ? ? ? 41 8b 47");
	cheat.infMissionTime = Memory::PatternScan(gameHandle, "f3 43 0f 11 84 f4");
	cheat.infHealth = Memory::PatternScan(gameHandle, "41 8b 84 8b ? ? ? ? 48 8b 5c 24");
	cheat.infStrategems1 = Memory::PatternScan(gameHandle, "0f 86 ? ? ? ? 0f 29 78 ? f3 0f 10 3d ? ? ? ? 44 0f 29 40");
	cheat.infStrategems2 = Memory::PatternScan(gameHandle, "f3 48 0f 2c c8 48 03 48 ? 48 89 8c 37 ? ? ? ? 8b 8c 37 ? ? ? ? 8d 41 ? 83 f8 ? 77 ? 8d 41 ? 89 84 37 ? ? ? ? 45 33 c9");
	// fn: 40 55 48 83 ec ? 3b 15 ? ? ? ? 41 8b e8 4c 8b 15 ? ? ? ? 0f 84 ? ? ? ? 45 8b 8a ? ? ? ? 45 33 c0 45 8b 9a ? ? ? ? 48 89 5c 24 ? 48 89 74 24 ? 44 0f af da 41 8d 71 ? 48 89 7c 24 ? 45 85 c9 74 ? 49 8b 9a ? ? ? ? 41 8b ba ? ? ? ? 66 0f 1f 84 00 ? ? ? ? 8b c6 43 8d 0c 18 48 23 c8 48 8d 04 cb 8b 0c cb 3b cf 74 ? 3b ca 74 ? 41 ff c0 45 3b c1 72 ? 33 c0 48 8b 7c 24 ? 48 8b 74 24 ? 48 8b 5c 24 ? 39 10 75 ? 8b 40 ? eb ? b8 ? ? ? ? 8b c8 ba
	// last mov [r8],ebp
	cheat.noReload = Memory::PatternScan(gameHandle, "41 89 28 49 8B 84 CA 28 20 00 00 8B 48 10");

	//LOG("cheat.infGrenades = %p", cheat.infGrenades);
	//LOG("cheat.infAmmo = %p", cheat.infAmmo);
	//LOG("cheat.infSyringe = %p", cheat.infSyringe);
	//LOG("cheat.infStamine = %p", cheat.infStamine);
	//LOG("cheat.infMissionTime = %p", cheat.infMissionTime);
	//LOG("cheat.infHealth = %p", cheat.infHealth);
	//LOG("cheat.infStrategems1 = %p", cheat.infStrategems1);
	//LOG("cheat.infStrategems2 = %p", cheat.infStrategems2);
	//LOG("cheat.noReload = %p", cheat.noReload);

	//cheat.noRecoil = Memory::PatternScan(gameHandle, "75 ? 45 8b c7 8b d3");
	//cheat.checkIsBlipSeen = Memory::PatternScan(gameHandle, "41 0f b6 44 97");
	//cheat.checkIfAlienHivesAreObstructed = Memory::PatternScan(gameHandle, "41 80 be ? ? ? ? ? 0f 85 ? ? ? ? 80 be");
	//cheat.checkIfMinorInterestBlipIsDiscovered = Memory::PatternScan(gameHandle, "0f 85 ? ? ? ? 48 8b 44 24 ? 80 78 ? ? 0f 84 ? ? ? ? f3 0f 5c 7d");
	//cheat.getMinorInterestBlipIcon = Memory::PatternScan(gameHandle, "0f 84 ? ? ? ? 48 8b 4c 24 ? f3 41 0f 10 4f");
	//cheat.checkMissionBlip = Memory::PatternScan(gameHandle, "0f 85 ? ? ? ? 49 8d b8");
}

void Core::Start()
{
	while (!GetModuleHandleA("game.dll"))
	{
		LOG("[HD2Cheeto] game handle not found, waiting 3 seconds...");
		Sleep(3000);
	}

	InitializeCheat();

	while (true)
	{
		if (GetAsyncKeyState(VK_F1) & 0x8000 && !cheat.cheatsEnabled)
		{
			// Infinite Grenades
			Memory::NopBytes(cheat.infGrenades, 3);

			// Infinite Ammo
			Memory::PatchBytes((cheat.infAmmo + 4), "\x00");

			// Infinite Syringe
			Memory::NopBytes(cheat.infSyringe, 3);

			// Infinite Stamina
			Memory::PatchBytes(cheat.infStamine, "\xF3\x41\x0F\x11\x30");

			// Infinite Mission Time
			Memory::NopBytes(cheat.infMissionTime, 10);

			// Infinite Health
			BYTE infHealthBytes[] =
			{
				0x48, 0x85, 0xD2,                                                       // test rdx,rdx
				0x75, 0x0C,                                                             // jnz short @f
				0x41, 0xC7, 0x84, 0x8B, 0x28, 0x4C, 0x00, 0x00, 0x0F, 0x27, 0x00, 0x00, // mov [r11+rcx*4+00004C28],#9999
				0x41, 0x8B, 0x84, 0x8B, 0x28, 0x4C, 0x00, 0x00,                         // mov eax,[r11+rcx*4+00004C28]
				0x48, 0x8B, 0x5C, 0x24, 0x20,                                           // mov rbx,[rsp+20]
				0x48, 0x8B, 0x74, 0x24, 0x28,                                           // mov rsi,[rsp+28]
			};

			AUTO_ASSEMBLE_TRAMPOLINE(cheat.infHealth, 18, infHealthBytes);

			// Infinite Strategems
			//Memory::PatchBytes(cheat.infStrategems1, "\x90\xE9");
			//Memory::NopBytes(cheat.infStrategems2, 9);

			//Memory::PatchBytes((cheat.infStrategems1 + 0xC), "\x8B\x8C\x3E\xD0\x01\x00\x00");
			//Memory::PatchBytes((cheat.infStrategems1 + 0x13), "\x8D\x01\x90");
			//Memory::PatchBytes((cheat.infStrategems1 + 0x1E), "\x89\x84\x3E\xD0\x01\x00\x00");
			//Memory::PatchBytes((cheat.infStrategems1 + 0x1b), "\x8D\x01\x90");

			// No Reload
			Memory::NopBytes(cheat.noReload, 3);

			// No Recoil
			//Memory::PatchBytes(cheat.noRecoil, "\x74");

			// Show Map Icons
			//Memory::PatchBytes(cheat.checkIsBlipSeen, "\xB8\x01\x00\x00\x00\x90");
			//Memory::PatchBytes(cheat.checkIfAlienHivesAreObstructed, "\xF8\x90\x90\x90\x90\x90\x90\x90");
			//Memory::PatchBytes(cheat.checkIfMinorInterestBlipIsDiscovered, "\x90\xE9");
			//Memory::PatchBytes(cheat.getMinorInterestBlipIcon, "\xEB\x09");
			//Memory::PatchBytes(cheat.checkMissionBlip, "\x90\xE9");

			LOG("[HD2Cheeto] All Cheats enabled!");

			cheat.cheatsEnabled = true;
		}
		else if (GetAsyncKeyState(VK_F2) & 0x8000 && cheat.cheatsEnabled)
		{
			Memory::RestoreBytes(cheat.infGrenades);
			Memory::RestoreBytes(cheat.infAmmo);
			Memory::RestoreBytes(cheat.infSyringe);
			Memory::RestoreBytes(cheat.infStamine);
			Memory::RestoreBytes(cheat.infMissionTime);
			Memory::RemoveTrampoline(cheat.infHealth);
			Memory::RestoreBytes(cheat.noReload);

			LOG("[HD2Cheeto] All Cheats disabled!");

			cheat.cheatsEnabled = false;
		}

		Sleep(100);
	}
}

#pragma region Initialization and DLL proxy stuff

void Core::Initialize(HINSTANCE hModule)
{
	if (Utils::GetCurrentProcessNameA().find("helldivers2") == std::string::npos) return;

	Utils::AttachConsole();

	LOG("[HD2Cheeto] Initializing...");
	// Get execution path
	std::vector<char> pathBuf;
	DWORD             copied = 0;
	do
	{
		pathBuf.resize(pathBuf.size() + MAX_PATH);
		copied = GetModuleFileNameA(nullptr, pathBuf.data(), static_cast<DWORD>(pathBuf.size()));
	} while (copied >= pathBuf.size());

	pathBuf.resize(copied);

	const std::filesystem::path filepath(pathBuf.begin(), pathBuf.end());

	// Get file path of proxy, tolowercase the file name
	const auto proxyFilepath = GetModuleFilePath(hModule);
	auto       ProxyFilename = proxyFilepath.filename().wstring();
	std::transform(ProxyFilename.begin(), ProxyFilename.end(), ProxyFilename.begin(), towlower);

	// Make proxy name list
	std::wstring names;
	bool         _1 = true;
	for (auto name : Exports::CompatibleFileNames)
	{
		if (_1)
		{
			_1 = false;
			names += name;
		}
		else
		{
			names += L", ";
			names += name;
		}
	}

	// Check if is compatible proxy
	std::size_t index = -1;
	if (!Exports::IsFileNameCompatible(ProxyFilename, &index))
	{
		Error(L"Proxy has an incompatible file name!\nValid names are: " + names + L"\n", true);
		return;
	}

	// Load original libs
	const HMODULE originalDll = LoadOriginalProxy(proxyFilepath, proxyFilepath.filename().stem().wstring());
	if (!originalDll)
	{
		Error(L"Failed to Load original " + proxyFilepath.wstring() + L"!", true);
		return;
	}

	// Load original lib exports
	Exports::Load(index, originalDll);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Start, hModule, 0, nullptr);

	LOG("[HD2Cheeto] Created Thread.");
}

HMODULE Core::LoadOriginalProxy(const std::filesystem::path& proxyFilepath, const std::wstring& proxyFilepathNoExt)
{
	HMODULE originalDll = LoadLibraryW((proxyFilepathNoExt + L"_original.dll").c_str());

	if (!originalDll)
	{
		wchar_t system32_path[MAX_PATH];

		if (GetSystemDirectoryW(system32_path, MAX_PATH) == NULL)
		{
			Error("Failed to get System32 directory!");
			KillProcess();
			return nullptr;
		}

		const auto path = std::filesystem::path(system32_path);
		originalDll = LoadLibraryW((path / proxyFilepath.filename()).c_str());
	}

	return originalDll;
}

std::filesystem::path Core::GetModuleFilePath(HMODULE moduleHandle)
{
	wchar_t path[MAX_PATH];
	GetModuleFileNameW(moduleHandle, path, MAX_PATH);
	return path;
}

void Core::Error(const std::string& reason, const bool shouldKill)
{
	MessageBoxA(nullptr, (reason + " " + (shouldKill ? "Preventing Startup" : "Continuing without HD2Cheeto") + "...").c_str(), "HD2Cheeto",
		MB_ICONERROR | MB_OK);
	if (shouldKill) KillProcess();
}

void Core::Error(const std::wstring& reason, const bool shouldKill)
{
	MessageBoxW(nullptr, (reason + L" " + (shouldKill ? L"Preventing Startup" : L"Continuing without HD2Cheeto") + L"...").c_str(), L"HD2Cheeto",
		MB_ICONERROR | MB_OK);
	if (shouldKill) KillProcess();
}

void Core::KillProcess()
{
	const HANDLE current_process = GetCurrentProcess();
	TerminateProcess(current_process, NULL);
	CloseHandle(current_process);
}

#pragma endregion
