#include "ShowMapIcons.h"

#include "Memory.h"

namespace Cheat::Features
{
	void ShowMapIcons::Enable()
	{
		if (!m_enabled)
		{
			Memory::PatchBytes(m_cheatData.showMapCheckIsBlipSeen, "\xB8\x01\x00\x00\x00\x90");
			Memory::PatchBytes(m_cheatData.showMapCheckAlienHives, "\xF8\x90\x90\x90\x90\x90\x90\x90");
			Memory::PatchBytes(m_cheatData.showMapCheckMinorInterest, "\x90\xE9");
			Memory::PatchBytes(m_cheatData.showMapGetMinorInterest, "\x90\xE9");
			Memory::PatchBytes(m_cheatData.showMapCheckMissionBlip, "\x90\xE9");
			m_enabled = true;
		}
	}

	void ShowMapIcons::Disable()
	{
		if (m_enabled)
		{
			Memory::RestoreBytes({
				m_cheatData.showMapCheckIsBlipSeen,
				m_cheatData.showMapCheckAlienHives,
				m_cheatData.showMapCheckMinorInterest,
				m_cheatData.showMapGetMinorInterest,
				m_cheatData.showMapCheckMissionBlip
				});
			m_enabled = false;
		}
	}
}
