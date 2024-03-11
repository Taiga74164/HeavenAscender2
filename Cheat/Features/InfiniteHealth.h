#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class InfiniteHealth : public FeatureBase
	{
	public:
		InfiniteHealth(CheatData& cheatData)
			: FeatureBase(cheatData, "Infinite Health")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
