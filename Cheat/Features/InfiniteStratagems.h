#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class InfiniteStratagems : public FeatureBase
	{
	public:
		InfiniteStratagems(CheatData& cheatData)
			: FeatureBase(cheatData, "Infinite Stratagems")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
