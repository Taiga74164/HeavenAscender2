#include "InfiniteRailGun.h"
#include "Memory.h"

namespace Cheat::Features
{
	void InfiniteRailGun::Enable()
	{
		if (!m_enabled)
		{
			const BYTE infRailGunBytes[] = {
				0x43, 0xC7, 0x84, 0x3E, 0x2C, 0x2C, 0x00, 0x00, 0xFC, 0xFF, 0x3F, 0x40, // mov [r14+r15+00002C2C],403FFFFC { 3.00 }
				0xF3, 0x43, 0x0F, 0x10, 0x84, 0x3E, 0x2C, 0x2C, 0x00, 0x00,				// movss xmm0,[r14+r15+00002C2C]
			};

			AUTO_ASSEMBLE_TRAMPOLINE(m_cheatData.infRailGun, 14, infRailGunBytes);
			m_enabled = true;
		}
	}

	void InfiniteRailGun::Disable()
	{
		if (m_enabled)
		{
			Memory::RemoveTrampoline(m_cheatData.infRailGun);
			m_enabled = false;
		}
	}
}
