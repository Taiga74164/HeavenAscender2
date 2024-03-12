#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class NoAggro : public FeatureBase
	{
	public:
		NoAggro(CheatData& cheatData)
			: FeatureBase(cheatData, "No Aggro")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
