#include "InfiniteHealth.h"
#include "Memory.h"

namespace Cheat::Features
{
	void InfiniteHealth::Enable()
	{
		if (!m_enabled)
		{
			const BYTE getHealthBytes[] =
{
				0x48, 0x85, 0xDB,                               // test rbx,rbx
				0x74, 0x03,                                     // jz short @f
				0x45, 0x89, 0x38,                               // mov [r8],r15d
																// @@:
				0x49, 0x8B, 0x84, 0xDE, 0x28, 0x04, 0x00, 0x00, // mov rax,[r14+rbx*8+00000428]
				0x8B, 0x48, 0x10,                               // mov ecx,[rax+10]
			};
			
			AUTO_ASSEMBLE_TRAMPOLINE(m_cheatData.infHealth1, 14, getHealthBytes);

			const BYTE setHealthBytes[] = {
				0x48, 0x85, 0xD2,                                                       // test rdx,rdx
				0x75, 0x0C,                                                             // jnz short @f
				0x41, 0xC7, 0x84, 0x8B, 0x28, 0x4C, 0x00, 0x00, 0x0F, 0x27, 0x00, 0x00, // mov [r11+rcx*4+00004C28],#9999
				0x41, 0x8B, 0x84, 0x8B, 0x28, 0x4C, 0x00, 0x00,                         // mov eax,[r11+rcx*4+00004C28]
				0x48, 0x8B, 0x5C, 0x24, 0x20,                                           // mov rbx,[rsp+20]
				0x48, 0x8B, 0x74, 0x24, 0x28,                                           // mov rsi,[rsp+28]
			};

			AUTO_ASSEMBLE_TRAMPOLINE(m_cheatData.infHealth2, 18, setHealthBytes);
			m_enabled = true;
		}
	}

	void InfiniteHealth::Disable()
	{
		if (m_enabled)
		{
			Memory::RemoveTrampoline(m_cheatData.infHealth1);
			Memory::RemoveTrampoline(m_cheatData.infHealth2);
			m_enabled = false;
		}
	}
}
