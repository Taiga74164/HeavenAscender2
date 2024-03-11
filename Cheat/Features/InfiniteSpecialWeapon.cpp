#include "InfiniteSpecialWeapon.h"
#include "Memory.h"

namespace Cheat::Features
{
	void InfiniteSpecialWeapon::Enable()
	{
		if (!m_enabled)
		{
			Memory::NopBytes(m_cheatData.infSpecialWeapon, 4);
			m_enabled = true;
		}
	}

	void InfiniteSpecialWeapon::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes(m_cheatData.infSpecialWeapon);
			m_enabled = false;
		}
	}
}
