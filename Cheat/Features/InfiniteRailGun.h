#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class InfiniteRailGun : public FeatureBase
	{
	public:
		InfiniteRailGun(CheatData& cheatData)
			: FeatureBase(cheatData, "Infinite Rail Gun")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
