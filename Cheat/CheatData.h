#pragma once
#include <cstdint>

namespace Cheat
{
	struct CheatData {
		uintptr_t baseAddress;

		uintptr_t infAmmo;
		uintptr_t infBackpack;
		uintptr_t infGrenades;
		uintptr_t infHealth;
		uintptr_t infMissionTime;
		uintptr_t infRailGun;
		uintptr_t infSpecialWeapon;
		uintptr_t infStamina;
		uintptr_t infStratagems1;
		uintptr_t infStratagems2;
		uintptr_t infSyringe;
		uintptr_t maxResources;
		uintptr_t noLaserOverheat;
		uintptr_t noRecoil;
		uintptr_t noReload;
		// uintptr_t checkIsBlipSeen;
		// uintptr_t checkIfAlienHivesAreObstructed;
		// uintptr_t checkIfMinorInterestBlipIsDiscovered;
		// uintptr_t getMinorInterestBlipIcon;
		// uintptr_t checkMissionBlip;
	};

	void InitCheat(CheatData& cheatData);
}
