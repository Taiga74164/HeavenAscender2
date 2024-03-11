#include "InfiniteStamina.h"
#include "Memory.h"

namespace Cheat::Features
{
	void InfiniteStamina::Enable()
	{
		if (!m_enabled)
		{
			Memory::PatchBytes(m_cheatData.infStamina, "\xF3\x41\x0F\x11\x30");
			m_enabled = true;
		}
	}

	void InfiniteStamina::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.infStamina);
			m_enabled = false;
		}
	}
}
