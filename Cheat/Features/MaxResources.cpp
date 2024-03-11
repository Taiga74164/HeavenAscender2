#include "MaxResources.h"
#include "Memory.h"

namespace Cheat::Features
{
	void MaxResources::Enable()
	{
		if (!m_enabled)
		{
			BYTE maxResourcesBytes[] = {
				0x41, 0x81, 0x84, 0x8A, 0xEC, 0x17, 0x00, 0x00, 0xF3, 0x01, 0x00, 0x00, // add [r10+rcx*4+000017EC],000001F3 { 499 }
				0x41, 0x81, 0x84, 0x8A, 0xF0, 0x17, 0x00, 0x00, 0xF9, 0x00, 0x00, 0x00, // add [r10+rcx*4+000017F0],000000F9 { 249 }
				0x41, 0x83, 0x84, 0x8A, 0xF4, 0x17, 0x00, 0x00, 0x63,					// add dword ptr [r10+rcx*4+000017F4],63 { 99 }
				0x48, 0x8D, 0x82, 0x99, 0x00, 0x00, 0x00,								// lea rax,[rdx+00000099]
			};

			AUTO_ASSEMBLE_TRAMPOLINE(m_cheatData.maxResources, 15, maxResourcesBytes);
			m_enabled = true;
		}
	}

	void MaxResources::Disable()
	{
		if (m_enabled)
		{
			Memory::RemoveTrampoline(m_cheatData.maxResources);
			m_enabled = false;
		}
	}
}
