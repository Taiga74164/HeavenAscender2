#include "CheatData.h"
#include "Memory.h"
#include "Utils.h"

void Cheat::InitCheat(CheatData& cheatData)
{
	const auto gameHandle = reinterpret_cast<uintptr_t>(GetModuleHandleA("game.dll"));
	cheatData.baseAddress = gameHandle;
	
	cheatData.infAmmo = Memory::PatternScan(gameHandle, "41 83 2c c2");
	cheatData.infBackpack = Memory::PatternScan(gameHandle, "2B C6 4D 8D 85 48 04 00 00");
	cheatData.infGrenades = Memory::PatternScan(gameHandle, "41 ff 08 4a 8b 84 ed");
	cheatData.infHealth1 = Memory::PatternScan(gameHandle, "45 89 38 49 8b 84 de");
	cheatData.infHealth2 = Memory::PatternScan(gameHandle, "41 8b 84 8b ? ? ? ? 48 8b 5c 24");
	cheatData.infMissionTime = Memory::PatternScan(gameHandle, "f3 43 0f 11 84 f4");
	cheatData.infRailGun = Memory::PatternScan(gameHandle, "f3 43 0f 11 84 37 ? ? ? ? 84 c9");
	cheatData.infSpecialWeapon = Memory::PatternScan(gameHandle, "44 89 7f ? 41 80 bc 24");
	cheatData.infStamina = Memory::PatternScan(gameHandle, "f3 41 0f 11 08 8b 48 ? e8 ? ? ? ? 41 8b 47");
	cheatData.infStratagems1 = Memory::PatternScan(gameHandle, "0f 86 ? ? ? ? 0f 29 78 ? f3 0f 10 3d ? ? ? ? 44 0f 29 40");
	cheatData.infStratagems2 = Memory::PatternScan(gameHandle, "f3 48 0f 2c c8 48 03 48 ? 48 89 8c 37 ? ? ? ? 8b 8c 37 ? ? ? ? 8d 41 ? 83 f8 ? 77 ? 8d 41 ? 89 84 37 ? ? ? ? 45 33 c9");
	cheatData.infSyringe = Memory::PatternScan(gameHandle, "41 ff cf 3b c2 74 ?");
	cheatData.maxSpeed = Memory::PatternScan(gameHandle, "f3 41 0f 59 56 ? f3 41 0f 59 56");
	cheatData.maxResources = Memory::PatternScan(gameHandle, "45 01 b4 8a ? ? ? ? 48 8d 82");
	cheatData.noAggro = Memory::PatternScan(gameHandle, "74 ? 45 8b 81 ? ? ? ? 45 85 c0");
	// fn: 40 55 48 83 ec ? 3b 15 ? ? ? ? 41 8b e8 4c 8b 15 ? ? ? ? 0f 84 ? ? ? ? 45 8b 8a ? ? ? ? 45 33 c0 45 8b 9a ? ? ? ? 48 89 5c 24 ? 48 89 74 24 ? 44 0f af da 41 8d 71 ? 48 89 7c 24 ? 45 85 c9 74 ? 49 8b 9a ? ? ? ? 41 8b ba ? ? ? ? 66 0f 1f 84 00 ? ? ? ? 8b c6 43 8d 0c 18 48 23 c8 48 8d 04 cb 8b 0c cb 3b cf 74 ? 3b ca 74 ? 41 ff c0 45 3b c1 72 ? 33 c0 48 8b 7c 24 ? 48 8b 74 24 ? 48 8b 5c 24 ? 39 10 75 ? 8b 40 ? eb ? b8 ? ? ? ? 8b c8 ba
	// last mov [r8],ebp
	cheatData.noLaserOverheat = Memory::PatternScan(gameHandle, "f3 0f 11 09 4c 8b c1 49 8b 84 df");
	cheatData.noRecoil = Memory::PatternScan(gameHandle, "75 ? 45 8b c7 8b d3");
	cheatData.noReload = Memory::PatternScan(gameHandle, "41 89 28 49 8B 84 CA 28 20 00 00 8B 48 10");
	cheatData.showMapCheckIsBlipSeen = Memory::PatternScan(gameHandle, "41 0f b6 44 97");
	cheatData.showMapCheckAlienHives = Memory::PatternScan(gameHandle, "41 80 be ? ? ? ? ? 0f 85 ? ? ? ? 80 be");
	cheatData.showMapCheckMinorInterest = Memory::PatternScan(gameHandle, "0f 85 ? ? ? ? 48 8b 44 24 ? 80 78 ? ? 0f 84 ? ? ? ? f3 0f 5c 7d");
	cheatData.showMapGetMinorInterest = Memory::PatternScan(gameHandle, "0f 84 ? ? ? ? 48 8b 4c 24 ? f3 41 0f 10 4f");
	cheatData.showMapCheckMissionBlip = Memory::PatternScan(gameHandle, "0f 85 ? ? ? ? 49 8d b8");
}
