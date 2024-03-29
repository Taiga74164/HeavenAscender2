#pragma once
#include "CheatData.h"
#include <string>

namespace Cheat
{
	class FeatureBase
	{
	public:
		FeatureBase(CheatData& cheatData, const std::string& name)
			: m_cheatData(cheatData), m_name(name), m_enabled(false)
		{
		}

		virtual ~FeatureBase() = default;
		
		virtual void Enable() = 0;
		virtual void Disable() = 0;
		virtual void Update() {}
		[[nodiscard]] std::string GetName() const { return m_name; }
		[[nodiscard]] bool IsEnabled() const { return m_enabled; }
		
	protected:
		CheatData& m_cheatData;
		std::string m_name;
		bool m_enabled;
	};
}