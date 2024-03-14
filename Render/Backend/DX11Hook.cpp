#include "DX11Hook.h"

#include "HookManager.h"
#include "Utils.h"

#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")

typedef HRESULT(__stdcall* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Backend
{
	DX11Hook* DX11Hook::m_instance = nullptr;
	static IDXGISwapChainPresent fnIDXGISwapChainPresent;
	static ID3D11Device* pDevice = nullptr;
	
	DX11Hook::DX11Hook()
	{
		m_instance = this;
	}

	DX11Hook::~DX11Hook()
	{
		m_instance = nullptr;
	}

	DX11Hook* DX11Hook::GetInstance()
	{
		if (!m_instance)
			m_instance = new DX11Hook();
		return m_instance;
	}

	HRESULT __stdcall DX11Hook::PresentHook(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags)
	{
		static bool init = false;

		static ID3D11DeviceContext* pContext = nullptr;

		if (!init)
		{
			const auto result = (HRESULT)pSwapChain->GetDevice(__uuidof(pDevice), reinterpret_cast<void**>(&pDevice));
			if (SUCCEEDED(result))
			{
				pDevice->GetImmediateContext(&pContext);
				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);

				m_instance->OnInit(sd.OutputWindow, pDevice, pContext, pSwapChain);

				init = true;
			}
		}

		if (init)
			m_instance->OnRender(pContext);
		
		return CALL_ORIGIN(PresentHook, pSwapChain, syncInterval, flags); 
	}

	static IDXGISwapChainPresent FindPresent()
	{
		WNDCLASSEX wc { 0 };
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc = DefWindowProc;
		wc.lpszClassName = TEXT("Class");
	
		if (!RegisterClassEx(&wc))
			return nullptr;

		const HWND hWnd = CreateWindow(wc.lpszClassName, TEXT(""), WS_DISABLED, 0, 0, 0, 0, NULL, NULL, NULL, nullptr);
	
		IDXGISwapChain* pSwapChain;
	
		D3D_FEATURE_LEVEL featureLevel;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = TRUE;  //((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	
		// Main D3D11 Objects
		ID3D11DeviceContext* pContext = nullptr;
		ID3D11Device* pDevice = nullptr;
	
		if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP,     nullptr, 0, nullptr, 1, D3D11_SDK_VERSION, 
				&swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext)) &&
			FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, 
				&swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext)))
		{
			DestroyWindow(swapChainDesc.OutputWindow);
			UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));
	
			return nullptr;
		}
	
		const DWORD_PTR* pSwapChainVtable = reinterpret_cast<DWORD_PTR*>(pSwapChain);
		pSwapChainVtable = reinterpret_cast<DWORD_PTR*>(pSwapChainVtable[0]);
	
		auto swapChainPresent = reinterpret_cast<IDXGISwapChainPresent>(pSwapChainVtable[8]);
	
		pDevice->Release();
		//pContext->Release();
		pSwapChain->Release();
	
		DestroyWindow(swapChainDesc.OutputWindow);
		UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));
	
		return swapChainPresent;
	}

	void DX11Hook::InitializeHooks()
	{
		LOG("Initialized DX11Hook.");
		
		fnIDXGISwapChainPresent = FindPresent();
		if (fnIDXGISwapChainPresent == nullptr)
		{
			LOG("Failed to find Present function for DX11.");
			return;
		}

		HookManager::install(fnIDXGISwapChainPresent, PresentHook);
		
		LOG("DX11Hook Hooks installed.");
	}
}