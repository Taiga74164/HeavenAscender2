#pragma once
#include <cstdint>

namespace Cheat
{
	struct CheatData {
		// Address of the module base
		uintptr_t baseAddress;
		// Addresses of the cheats
		uintptr_t infAmmo;
		uintptr_t infBackpack;
		uintptr_t infGrenades;
		uintptr_t infHealth1;
		uintptr_t infHealth2;
		uintptr_t infMissionTime;
		uintptr_t infRailGun;
		uintptr_t infSpecialWeapon;
		uintptr_t infStamina;
		uintptr_t infStratagems1;
		uintptr_t infStratagems2;
		uintptr_t infSyringe;
		uintptr_t maxResources;
		uintptr_t maxSpeed;
		uintptr_t noAggro;
		uintptr_t noLaserOverheat;
		uintptr_t noRecoil;
		uintptr_t noReload;
		uintptr_t checkIsBlipSeen;
		uintptr_t checkIfAlienHivesAreObstructed;
		uintptr_t checkIfMinorInterestBlipIsDiscovered;
		uintptr_t getMinorInterestBlipIcon;
		uintptr_t checkMissionBlip;

		// Fields for the cheats
		float maxSpeedValue = 2.0f;
	};

	void InitCheat(CheatData& cheatData);
}
