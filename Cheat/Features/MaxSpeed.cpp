#include "MaxSpeed.h"
#include "Memory.h"
#include "Utils.h"

namespace Cheat::Features
{
	void MaxSpeed::Enable()
	{
		if (!m_enabled)
		{
			const BYTE maxSpeedBytes[] = {
				0x41, 0xC7, 0x46, 0x0C, 0x00, 0x00, 0xC0, 0x40,	// mov [r14+0C],40C00000 { 6.00 }
				0xF3, 0x41, 0x0F, 0x59, 0x56, 0x0C,				// mulss xmm2,[r14+0C]
				0xF3, 0x41, 0x0F, 0x59, 0x56, 0x10,				// mulss xmm2,[r14+10]
				0x0F, 0x28, 0xE2,								// movaps xmm4,xmm2
			};

			// AUTO_ASSEMBLE_TRAMPOLINE(m_cheatData.maxSpeed, 15, maxSpeedBytes);
			m_allocatedMemory = static_cast<uint8_t*>(Memory::AllocateNearbyMemory(m_cheatData.maxSpeed, sizeof maxSpeedBytes + 14));
			Memory::CreateTrampoline(m_cheatData.maxSpeed, m_allocatedMemory, 15);
			Memory::WriteInstructions(m_allocatedMemory, maxSpeedBytes, sizeof maxSpeedBytes, m_cheatData.maxSpeed + 15);
			m_enabled = true;
		}
	}

	void MaxSpeed::Disable()
	{
		if (m_enabled)
		{
			if (m_allocatedMemory != nullptr) m_allocatedMemory = nullptr;
			Memory::RemoveTrampoline(m_cheatData.maxSpeed);
			m_enabled = false;
		}
	}

	void MaxSpeed::Update()
	{
		if (m_enabled && m_allocatedMemory != nullptr)
			Utils::SetToHex(m_cheatData.maxSpeedValue, m_allocatedMemory + 4);
		// auto bytes = reinterpret_cast<BYTE*>(&m_cheatData.maxSpeedValue);
		// memcpy(m_allocatedMemory + 4, bytes, sizeof(float));
	}
}
