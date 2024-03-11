#include "NoLaserOverheat.h"
#include "Memory.h"

namespace Cheat::Features
{
	void NoLaserOverheat::Enable()
	{
		if (!m_enabled)
		{
			Memory::NopBytes(m_cheatData.noLaserOverheat, 4);
			m_enabled = true;
		}
	}

	void NoLaserOverheat::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.noLaserOverheat);
			m_enabled = false;
		}
	}
}
