#include "InfiniteBackpack.h"
#include "Memory.h"

namespace Cheat::Features
{
	void InfiniteBackpack::Enable()
	{
		if (!m_enabled)
		{
			Memory::NopBytes(m_cheatData.infBackpack, 2);
			m_enabled = true;
		}
	}

	void InfiniteBackpack::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.infBackpack);
			m_enabled = false;
		}
	}
}
