#include "InfiniteStratagems.h"
#include "Memory.h"

namespace Cheat::Features
{
	void InfiniteStratagems::Enable()
	{
		if (!m_enabled)
		{
			Memory::PatchBytes(m_cheatData.infStratagems1, "\x90\xE9");
			Memory::NopBytes(m_cheatData.infStratagems2, 9);
			m_enabled = true;
		}
	}

	void InfiniteStratagems::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.infStratagems1);
			Memory::RestoreBytes(m_cheatData.infStratagems2);
			m_enabled = false;
		}
	}
}
