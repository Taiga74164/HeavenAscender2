#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class ShowMapIcons : public FeatureBase
	{
	public:
		ShowMapIcons(CheatData& cheatData)
			: FeatureBase(cheatData, "Show Map Icons")
		{
		}
		
		void Enable() override;
		void Disable() override;
	};
}
