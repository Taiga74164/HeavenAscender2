#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class NoLaserOverheat : public FeatureBase
	{
	public:
		NoLaserOverheat(CheatData& cheatData)
			: FeatureBase(cheatData, "No Laser Overheat")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
