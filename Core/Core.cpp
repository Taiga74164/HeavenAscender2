#include "Core.h"
#include <Exports/Exports.h>
#include <codecvt>
#include <psapi.h>
#include <Utils.h>
#include "Memory.h"
#include "CommandMenu.h"

#include "Cheat/CheatData.h"
#include "Cheat/Features/InfiniteAmmo.h"
#include "Cheat/Features/InfiniteBackpack.h"
#include "Cheat/Features/InfiniteGrenade.h"
#include "Cheat/Features/InfiniteHealth.h"
#include "Cheat/Features/InfiniteMissionTime.h"
#include "Cheat/Features/InfiniteRailGun.h"
#include "Cheat/Features/InfiniteSpecialWeapon.h"
#include "Cheat/Features/InfiniteStamina.h"
#include "Cheat/Features/InfiniteStratagems.h"
#include "Cheat/Features/InfiniteSyringe.h"
#include "Cheat/Features/MaxSpeed.h"
#include "Cheat/Features/MaxResources.h"
#include "Cheat/Features/NoAggro.h"
#include "Cheat/Features/NoLaserOverheat.h"
#include "Cheat/Features/NoRecoil.h"
#include "Cheat/Features/NoReload.h"

std::vector<std::shared_ptr<Cheat::FeatureBase>> Core::m_features;

#define ADD_FEATURE_TOGGLE(TAB, CLASS, CHEAT_DATA) \
    do { \
        auto feature = std::make_shared<CLASS>(CHEAT_DATA); \
        (TAB)->AddElement(std::make_unique<CommandMenu::Toggle>(feature->GetName(), [feature](bool enabled) { enabled ? feature->Enable() : feature->Disable(); })); \
        if (std::find(m_features.begin(), m_features.end(), feature) == m_features.end()) \
			m_features.push_back(feature); \
    } while (false)

#define ADD_FEATURE_SLIDER_FLOAT(TAB, CLASS, CHEAT_DATA, VALUE, MIN, MAX, STEP) \
	do { \
		auto feature = std::make_shared<CLASS>(CHEAT_DATA); \
		(TAB)->AddElement(std::make_unique<CommandMenu::SliderFloat>(feature->GetName() + " Value", VALUE, MIN, MAX, STEP)); \
		if (std::find(m_features.begin(), m_features.end(), feature) == m_features.end()) \
			m_features.push_back(feature); \
	} while (false)

void Core::Start()
{
	while (!GetModuleHandleA("game.dll") && !FindWindowA("stingray_window", nullptr))
	{
		LOG("[HeavenAscender2] game not found, waiting 3 seconds...");
		Sleep(3000);
	}

	CommandMenu::Menu menu;
	Cheat::CheatData cheatData;
	InitCheat(cheatData);
	
	auto featuresTab = std::make_unique<CommandMenu::Tab>("Features");
	auto aboutTab = std::make_unique<CommandMenu::Tab>("About");

	// Features tab content
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::InfiniteAmmo, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::InfiniteBackpack, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::InfiniteGrenade, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::InfiniteHealth, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::InfiniteMissionTime, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::InfiniteRailGun, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::InfiniteSpecialWeapon, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::InfiniteStamina, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::InfiniteStratagems, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::InfiniteSyringe, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::MaxSpeed, cheatData);
	ADD_FEATURE_SLIDER_FLOAT(featuresTab, Cheat::Features::MaxSpeed, cheatData, &cheatData.maxSpeedValue, 1.0f, 10.0f, 0.5f);
	
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::MaxResources, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::NoAggro, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::NoLaserOverheat, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::NoRecoil, cheatData);
	ADD_FEATURE_TOGGLE(featuresTab, Cheat::Features::NoReload, cheatData);
	// About tab content
	aboutTab->AddElement(std::make_unique<CommandMenu::Text>("Credits to:"));
	aboutTab->AddElement(std::make_unique<CommandMenu::Text>("- Taiga74164"));
	aboutTab->AddElement(std::make_unique<CommandMenu::Text>("- emoisback"));
	aboutTab->AddElement(std::make_unique<CommandMenu::Text>("- gir489 and contributors"));

	menu.AddTab(std::move(featuresTab));
	menu.AddTab(std::move(aboutTab));
	menu.Display();

	while (true)
	{
		menu.ProcessInput();
		for (const auto& features : m_features)
		{
			features->Update();
		}
		
		Sleep(50);
	}
}

#pragma region Initialization and DLL proxy stuff

void Core::Initialize(HINSTANCE hModule)
{
	if (Utils::GetCurrentProcessNameA().find("helldivers2") == std::string::npos) return;

	Utils::AttachConsole();

	LOG("[HeavenAscender2] Initializing...");
	// Get execution path
	std::vector<char> pathBuf;
	DWORD copied = 0;
	do
	{
		pathBuf.resize(pathBuf.size() + MAX_PATH);
		copied = GetModuleFileNameA(nullptr, pathBuf.data(), static_cast<DWORD>(pathBuf.size()));
	} while (copied >= pathBuf.size());

	pathBuf.resize(copied);

	const std::filesystem::path filepath(pathBuf.begin(), pathBuf.end());

	// Get file path of proxy, to lowercase the file name
	const auto proxyFilepath = GetModuleFilePath(hModule);
	auto       proxyFilename = proxyFilepath.filename().wstring();
	std::transform(proxyFilename.begin(), proxyFilename.end(), proxyFilename.begin(), towlower);

	// Make proxy name list
	std::wstring names;
	bool         _1 = true;
	for (const auto name : Exports::CompatibleFileNames)
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
	if (!Exports::IsFileNameCompatible(proxyFilename, &index))
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

	LOG("[HeavenAscender2] Created Thread.");
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
	MessageBoxA(nullptr, (reason + " " + (shouldKill ? "Preventing Startup" : "Continuing without HeavenAscender2") + "...").c_str(), "HeavenAscender2",
		MB_ICONERROR | MB_OK);
	if (shouldKill) KillProcess();
}

void Core::Error(const std::wstring& reason, const bool shouldKill)
{
	MessageBoxW(nullptr, (reason + L" " + (shouldKill ? L"Preventing Startup" : L"Continuing without HeavenAscender2") + L"...").c_str(), L"HeavenAscender2",
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
