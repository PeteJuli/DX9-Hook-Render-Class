#ifndef _Render_DX9_HPP_
#define _Render_DX9_HPP_

// -----------------------------------------------------------------------------

#include <windows.h>
#include <fstream>
#include <iostream>
#include <string.h>

//DX9
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// -----------------------------------------------------------------------------

enum FontID
{
	FontMain,
	FontESP,
	FontMenu,
	MAX_FONT_ID
};

// -----------------------------------------------------------------------------

namespace DX9
{
	class Render_DX9
	{
	public:
		BOOL IsInit();
		void Init(IDirect3DDevice9 * pDevice);

		void OnLostDevice();
		void OnResetDevice();
		void Release();

		void Begin();
		void End();

		ID3DXFont * GetFont(INT FontNum);

		void DrawString(FLOAT X, FLOAT Y, D3DCOLOR Color, ID3DXFont * Font, std::string String, ...);
		void DrawStringBoxed(FLOAT X, FLOAT Y, D3DCOLOR ColorString, D3DCOLOR ColorBox, ID3DXFont * Font, std::string String);
		void DrawStringBoxedWithBorder(FLOAT X, FLOAT Y, FLOAT BorderThick, D3DCOLOR ColorString, D3DCOLOR ColorBox, D3DCOLOR ColorBorder, ID3DXFont * Font, std::string String);
		FLOAT GetStringWidth(ID3DXFont * Font, std::string String, ...);
		FLOAT GetStringHeight(ID3DXFont * Font, std::string String, ...);

		void DrawLine(FLOAT X, FLOAT Y, FLOAT X2, FLOAT Y2, D3DCOLOR Color);

		void DrawFilledRect(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, D3DCOLOR Color);
		void DrawRect(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, FLOAT Thick, D3DCOLOR Color);
		void DrawFilledRectWithBorder(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, FLOAT BorderThick, D3DCOLOR ColorRect, D3DCOLOR ColorBorder);
		void DrawCorneredBox(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, FLOAT Thick, D3DCOLOR Color);

		void DrawDot(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, D3DCOLOR Color);
		void DrawCross(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, D3DCOLOR Color);

	private:
		bool bInit;
		IDirect3DDevice9 * Device;
		ID3DXSprite * Sprite;
		ID3DXFont * Font[FontID::MAX_FONT_ID];

		IDirect3DPixelShader9 * PixelShader;
		IDirect3DBaseTexture9 * Texture;
		IDirect3DStateBlock9 * StateBlock;
	};

	extern Render_DX9 Render;
}



// -----------------------------------------------------------------------------

#endif // !_Render_DX9_HPP_

