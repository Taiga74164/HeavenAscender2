#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class InfiniteMissionTime : public FeatureBase
	{
	public:
		InfiniteMissionTime(CheatData& cheatData)
			: FeatureBase(cheatData, "Infinite Mission Time")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
