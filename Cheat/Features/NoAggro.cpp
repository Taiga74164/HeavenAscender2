#include "NoAggro.h"
#include "Memory.h"

namespace Cheat::Features
{
	void NoAggro::Enable()
	{
		if (!m_enabled)
		{
			Memory::PatchBytes(m_cheatData.noAggro, "\xEB");
			m_enabled = true;
		}
	}

	void NoAggro::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.noAggro);
			m_enabled = false;
		}
	}
}
