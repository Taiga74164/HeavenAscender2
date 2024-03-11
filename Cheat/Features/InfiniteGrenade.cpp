#include "InfiniteGrenade.h"
#include "Memory.h"

namespace Cheat::Features
{
	void InfiniteGrenade::Enable()
	{
		if (!m_enabled)
		{
			Memory::NopBytes(m_cheatData.infGrenades, 3);
			m_enabled = true;
		}
	}

	void InfiniteGrenade::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.infGrenades);
			m_enabled = false;
		}
	}
}
