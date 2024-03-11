#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class InfiniteSpecialWeapon : public FeatureBase
	{
	public:
		InfiniteSpecialWeapon(CheatData& cheatData)
			: FeatureBase(cheatData, "Infinite Special Weapon")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
