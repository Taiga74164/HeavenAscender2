#pragma once
#pragma once
#include "../FeatureBase.h"

namespace Cheat::Features
{
	class MaxSpeed : public FeatureBase
	{
	public:
		MaxSpeed(CheatData& cheatData)
			: FeatureBase(cheatData, "Max Speed")
		{
		}
		
		void Enable() override;
		void Disable() override;
		void Update() override;

	private:
		uint8_t* m_allocatedMemory = nullptr;
	};
}
