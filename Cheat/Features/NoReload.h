#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class NoReload : public FeatureBase
	{
	public:
		NoReload(CheatData& cheatData)
			: FeatureBase(cheatData, "No Reload")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
