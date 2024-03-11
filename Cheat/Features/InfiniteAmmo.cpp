#include "InfiniteAmmo.h"
#include "Memory.h"

namespace Cheat::Features
{
	void InfiniteAmmo::Enable()
	{
		if (!m_enabled)
		{
			Memory::PatchBytes(m_cheatData.infAmmo + 4, "\x00");
			m_enabled = true;
		}
	}

	void InfiniteAmmo::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.infAmmo + 4);
			m_enabled = false;
		}
	}
}
