#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class InfiniteAmmo : public FeatureBase
	{
	public:
		InfiniteAmmo(CheatData& cheatData)
			: FeatureBase(cheatData, "Infinite Ammo")
		{
		}

		void Enable() override;
		void Disable() override;
	};
}