#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class InfiniteStamina : public FeatureBase
	{
	public:
		InfiniteStamina(CheatData& cheatData)
			: FeatureBase(cheatData, "Infinite Stamina")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
