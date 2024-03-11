#include "InfiniteMissionTime.h"
#include "Memory.h"

namespace Cheat::Features
{
	void InfiniteMissionTime::Enable()
	{
		if (!m_enabled)
		{
			Memory::NopBytes(m_cheatData.infMissionTime, 10);
			m_enabled = true;
		}
	}

	void InfiniteMissionTime::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.infMissionTime);
			m_enabled = false;
		}
	}
}
