#include "InfiniteSyringe.h"
#include "Memory.h"

namespace Cheat::Features
{
	void InfiniteSyringe::Enable()
	{
		if (!m_enabled)
		{
			Memory::NopBytes(m_cheatData.infSyringe, 3);
			m_enabled = true;
		}
	}

	void InfiniteSyringe::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.infSyringe);
			m_enabled = false;
		}
	}
}
