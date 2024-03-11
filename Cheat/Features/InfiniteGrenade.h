#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class InfiniteGrenade : public FeatureBase
	{
	public:
		InfiniteGrenade(CheatData& cheatData)
			: FeatureBase(cheatData, "Infinite Grenade")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}