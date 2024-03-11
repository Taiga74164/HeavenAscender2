#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class InfiniteBackpack: public FeatureBase
	{
	public:
		InfiniteBackpack(CheatData& cheatData)
			: FeatureBase(cheatData, "Infinite Backpack")
		{
		}

		void Enable() override;
		void Disable() override;
	};
}
