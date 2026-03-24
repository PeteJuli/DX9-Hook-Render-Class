#include "D3D9_Hook.hpp"

D3D9_Hook D3D9;

Hook::VMT Reset;
long __stdcall hkReset(IDirect3DDevice9 * Device, D3DPRESENT_PARAMETERS* p)
{
	DX9::Render.OnResetDevice();

	using func = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);//function prototype
	return Reset.GetTrampoline<func>()(Device, p);
}

Hook::VMT Present;
long __stdcall hkPresent(IDirect3DDevice9 * Device, const RECT * SourceRect, const RECT * DestRect, HWND DestWindowOverride, const RGNDATA * DirtyRegion)
{

	using func = long(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);//function prototype
	return Present.GetTrampoline<func>()(Device, SourceRect, DestRect, DestWindowOverride, DirtyRegion);
}

Hook::VMT BeginScene;
long __stdcall hkBeginScene(IDirect3DDevice9 * Device)
{

	using func = long(__stdcall*)(IDirect3DDevice9*);//function prototype
	return BeginScene.GetTrampoline<func>()(Device);
}

Hook::VMT EndScene;
long __stdcall hkEndScene(IDirect3DDevice9 * Device)
{
	if (!DX9::Render.IsInit())
		DX9::Render.Init(Device);

	DX9::Render.Begin();

	DX9::Render.DrawCross(ScreenX, ScreenY, 15.0f, 15.0f, D3DCOLOR_ARGB(255, 255, 0, 0));

	DX9::Render.End();

	using func = long(__stdcall*)(IDirect3DDevice9*);//function prototype
	return EndScene.GetTrampoline<func>()(Device);
}

void D3D9_Hook::Hook()
{
	if (GetModuleHandle("d3d9.dll"))
	{
		pDevice = NULL;

		while (pDevice == NULL)
		{
			Sleep(450);

			///Creating Direct3D device...\n
			pDevice = *(IDirect3DDevice9**)0x0;//Put Address here
		}

		//Hooks
		Reset.CreateHookEx(pDevice, hkReset, 16);
		//Present.CreateHookEx(pDevice, hkPresent, 17);
		//BeginScene.CreateHookEx(pDevice, hkBeginScene, 41);
		EndScene.CreateHookEx(pDevice, hkEndScene, 42);
	}
}