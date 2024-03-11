#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class InfiniteSyringe : public FeatureBase
	{
	public:
		InfiniteSyringe(CheatData& cheatData)
			: FeatureBase(cheatData, "Infinite Syringe")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
