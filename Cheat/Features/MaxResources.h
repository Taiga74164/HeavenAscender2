#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class MaxResources : public FeatureBase
	{
	public:
		MaxResources(CheatData& cheatData)
			: FeatureBase(cheatData, "Max Resources")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
