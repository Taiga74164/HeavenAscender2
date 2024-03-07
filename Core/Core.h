#pragma once
#include <Windows.h>
#include <filesystem>

class Core
{
public:
	struct Cheat {
		uintptr_t baseAddress;
		size_t size;

		uintptr_t infGrenades;
		uintptr_t infAmmo;
		uintptr_t infSyringe;
		uintptr_t infStamine;
		uintptr_t infMissionTime;
		uintptr_t infHealthCheck;
		uintptr_t infHealth;
		uintptr_t infStrategems;
		uintptr_t noReload;
		uintptr_t noRecoil;
		uintptr_t checkIsBlipSeen;
		uintptr_t checkIfAlienHivesAreObstructed;
		uintptr_t checkIfMinorInterestBlipIsDiscovered;
		uintptr_t getMinorInterestBlipIcon;
		uintptr_t checkMissionBlip;

		bool cheatsEnabled = false;
	};

	static void Initialize(HINSTANCE hModule);
	static void Start();
	static void InitializeCheat();
private:
	static HMODULE LoadOriginalProxy(const std::filesystem::path& proxyFilepath, const std::wstring& proxyFilepathNoExt);
	static std::filesystem::path GetModuleFilePath(HMODULE moduleHandle);

	static void Error(const std::string& reason, bool shouldKill = false);
	static void Error(const std::wstring& reason, bool shouldKill = false);
	static void KillProcess();
};