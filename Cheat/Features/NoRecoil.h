#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class NoRecoil : public FeatureBase
	{
	public:
		NoRecoil(CheatData& cheatData)
			: FeatureBase(cheatData, "No Recoil")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
