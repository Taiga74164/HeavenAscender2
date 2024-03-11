#include "NoReload.h"
#include "Memory.h"

namespace Cheat::Features
{
	void NoReload::Enable()
	{
		if (!m_enabled)
		{
			Memory::NopBytes(m_cheatData.noReload, 3);
			m_enabled = true;
		}
	}

	void NoReload::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.noReload);
			m_enabled = false;
		}
	}
}
