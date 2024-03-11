#include "NoRecoil.h"
#include "Memory.h"

namespace Cheat::Features
{
	void NoRecoil::Enable()
	{
		if (!m_enabled)
		{
			Memory::PatchBytes(m_cheatData.noRecoil, "\xEB");
			m_enabled = true;
		}
	}

	void NoRecoil::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.noRecoil);
			m_enabled = false;
		}
	}
}
