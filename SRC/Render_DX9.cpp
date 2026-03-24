#include "Render_DX9.hpp"

struct Vertex3D
{
	D3DXVECTOR3 Pos;
	FLOAT RHW;
	D3DCOLOR Color;
};

namespace DX9
{
	Render_DX9 Render;

	// -----------------------------------------------------------------------------

	BOOL Render_DX9::IsInit()
	{
		if (bInit)
		{
			return TRUE;
		}

		return FALSE;
	}

	void Render_DX9::Init(IDirect3DDevice9 * pDevice)
	{
		if (Device != pDevice && !bInit)
		{
			this->Device = pDevice;

			Device->CreateStateBlock(D3DSBT_ALL, &StateBlock);

			try
			{
				if (Sprite != NULL)
				{
					Sprite->Release();
				}

				for (int i = 0; i < FontID::MAX_FONT_ID; i++)
				{
					if (Font[i] != NULL)
					{
						Font[i]->Release();
					}
				}
			}
			catch (...) {}

			D3DXCreateSprite(Device, &Sprite);
			D3DXCreateFont(Device, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Tahoma", &Font[FontID::FontESP]);
			//D3DXCreateFont(Device, 10, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Tahoma", &Font[FontID::FontESP]);
			D3DXCreateFont(Device, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Tahoma", &Font[FontID::FontMenu]);

			bInit = true;
		}
	}

	// -----------------------------------------------------------------------------

	void Render_DX9::OnLostDevice()
	{
		if (Sprite != NULL)
			Sprite->OnLostDevice();

		for (int i = 0; i < FontID::MAX_FONT_ID; i++)
		{
			if (Font[i] != NULL)
			{
				Font[i]->OnLostDevice();
			}
		}
	}

	void Render_DX9::OnResetDevice()
	{
		if (Sprite != NULL)
			Sprite->OnResetDevice();

		for (int i = 0; i < FontID::MAX_FONT_ID; i++)
		{
			if (Font[i] != NULL)
			{
				Font[i]->OnResetDevice();
			}
		}
	}

	// -----------------------------------------------------------------------------

	void Render_DX9::Release()
	{
		if (Sprite)
			Sprite->Release();

		for (int i = 0; i < FontID::MAX_FONT_ID; i++)
		{
			if (Font[i])
			{
				Font[i]->Release();
			}
		}

		Device = nullptr;
	}

	// -----------------------------------------------------------------------------

	void Render_DX9::Begin()
	{
		Device->GetTexture(NULL, &Texture);
		Device->GetPixelShader(&PixelShader);
		StateBlock->Capture();
	
		Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	void Render_DX9::End()
	{
		Sprite->End();

		Device->SetTexture(NULL, Texture);
		Device->SetPixelShader(PixelShader);

		StateBlock->Apply();
		OnLostDevice();
	}

	// -----------------------------------------------------------------------------

	ID3DXFont * Render_DX9::GetFont(INT FontNum)
	{
		return Font[FontNum];
	}

	// -----------------------------------------------------------------------------

	void Render_DX9::DrawString(FLOAT X, FLOAT Y, D3DCOLOR Color, ID3DXFont * Font, std::string String, ...)
	{
		CHAR Buffer[101] = "";

		va_list ArgumentList;
		va_start(ArgumentList, String);
		vsnprintf_s(Buffer, sizeof(Buffer), sizeof(Buffer) - strlen(Buffer), String.c_str(), ArgumentList);
		va_end(ArgumentList);

		RECT Rect = { X, Y, X + 500, Y + 50 };
		Font->DrawText(Sprite, Buffer, -1, &Rect, DT_NOCLIP, Color);
		Sprite->Flush();
	}


	void Render_DX9::DrawStringBoxed(FLOAT X, FLOAT Y, D3DCOLOR ColorString, D3DCOLOR ColorBox, ID3DXFont * Font, std::string String)
	{
		float StringWidth = GetStringWidth(Font, String);
		float StringHeight = GetStringHeight(Font, String);

		DrawFilledRect(X - 5.5f, (Y - StringHeight) + 4.0f, StringWidth + 10.0f, StringHeight - 8.0f, ColorBox);
		DrawString(X, Y, ColorString, Font, String);
	}

	void Render_DX9::DrawStringBoxedWithBorder(FLOAT X, FLOAT Y, FLOAT BorderThick, D3DCOLOR ColorString, D3DCOLOR ColorBox, D3DCOLOR ColorBorder, ID3DXFont * Font, std::string String)
	{
		float StringWidth = GetStringWidth(Font, String);
		float StringHeight = GetStringHeight(Font, String);

		DrawFilledRectWithBorder(X - 6.0f, (Y - StringHeight) + 4.0f, StringWidth + 10.0f, StringHeight - 8.0f, BorderThick, ColorBox, ColorBorder);
		DrawString(X, Y, ColorString, Font, String);
	}


	FLOAT Render_DX9::GetStringWidth(ID3DXFont * Font, std::string String, ...)
	{
		CHAR Buffer[101] = "";

		va_list ArgumentList;
		va_start(ArgumentList, String);
		vsnprintf_s(Buffer, sizeof(Buffer), sizeof(Buffer) - strlen(Buffer), String.c_str(), ArgumentList);
		va_end(ArgumentList);

		RECT Rect = { 0, 0, 0, 0 };
		Font->DrawText(NULL, Buffer, -1, &Rect, DT_CALCRECT, NULL);

		return Rect.right - Rect.left;
	}

	FLOAT Render_DX9::GetStringHeight(ID3DXFont * Font, std::string String, ...)
	{
		CHAR Buffer[101] = "";

		va_list ArgumentList;
		va_start(ArgumentList, String);
		vsnprintf_s(Buffer, sizeof(Buffer), sizeof(Buffer) - strlen(Buffer), String.c_str(), ArgumentList);
		va_end(ArgumentList);

		RECT Rect = { 0, 0, 0, 0 };
		Font->DrawText(NULL, Buffer, -1, &Rect, DT_CALCRECT, NULL);

		return Rect.top - Rect.bottom;
	}

	// -----------------------------------------------------------------------------

	void Render_DX9::DrawLine(FLOAT X, FLOAT Y, FLOAT X2, FLOAT Y2, D3DCOLOR Color)
	{
		Vertex3D Vert[2] =
		{
		{ D3DXVECTOR3((FLOAT)X, (FLOAT)Y, 0.0f), 1.0f, Color },
		{ D3DXVECTOR3((FLOAT)X2, (FLOAT)Y2, 0.0f), 1.0f, Color }
		};

		Device->SetTexture(NULL, nullptr);
		Device->SetPixelShader(nullptr);
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_TRUE);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		Device->SetVertexShader(nullptr);
		Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

		Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, Vert, sizeof(Vertex3D));
	}

	// -----------------------------------------------------------------------------


	void Render_DX9::DrawFilledRect(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, D3DCOLOR Color)
	{
		Vertex3D Vertex[4] =
		{
			{ D3DXVECTOR3(X, Y, 0.0f), 1.0f, Color },
			{ D3DXVECTOR3(X + Width, Y, 0.0f), 1.0f, Color },
			{ D3DXVECTOR3(X, Y + Height, 0.0f), 1.0f, Color },
			{ D3DXVECTOR3(X + Width, Y + Height, 0.0f), 1.0f, Color }
		};

		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		Device->SetVertexShader(nullptr);
		Device->SetPixelShader(nullptr);
		Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		Device->SetTexture(NULL, nullptr);

		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof(Vertex3D));
	}

	void Render_DX9::DrawRect(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, FLOAT Thick, D3DCOLOR Color)
	{
		DrawFilledRect(X, Y, Width, Thick, Color); //top
		DrawFilledRect(X + Width, Y, Thick, Height + Thick, Color); //right
		DrawFilledRect(X, Y + Height, Width, Thick, Color); //bottom
		DrawFilledRect(X, Y, Thick, Height, Color); //left
	}

	void Render_DX9::DrawFilledRectWithBorder(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, FLOAT BorderThick, D3DCOLOR ColorRect, D3DCOLOR ColorBorder)
	{
		DrawFilledRect(X, Y, Width, Height, ColorRect);
		DrawRect(X, Y, Width, Height, BorderThick, ColorBorder);
	}

	void Render_DX9::DrawCorneredBox(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, FLOAT Thick, D3DCOLOR Color)
	{
		//Left Top
		DrawFilledRect(X, Y, Width / 3.5, Thick, Color);//Horizontal
		DrawFilledRect(X, Y, Thick, Height / 4, Color);//Vertikal

		//Right Top
		DrawFilledRect(X + Width - Width / 3.5, Y, Width / 3.5, Thick, Color);//Horizontal
		DrawFilledRect(X + Width - Thick, Y, Thick, Height / 4, Color);//Vertikal

		//Left Bottom 
		DrawFilledRect(X, Y + Height - Height / 4, Thick, Height / 4, Color);//Vertikal
		DrawFilledRect(X, Y + Height - Thick, Width / 3.5, Thick, Color);//Horizontal

		//Right Bottom 
		DrawFilledRect(X + Width - Thick, Y + Height - Height / 4, Thick, Height / 4, Color);//Vertikal
		DrawFilledRect(X + Width - Width / 3.5, Y + Height - Thick, Width / 3.5, Thick, Color);//Horizontal
	}

	// -----------------------------------------------------------------------------

	void Render_DX9::DrawDot(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, D3DCOLOR Color)
	{
		DrawFilledRect(X - 1.0f, Y - 1.0f, Width + 2.0f, Height + 2.0f, Color);
		DrawFilledRect(X, Y, Width, Height, Color);
	}

	void Render_DX9::DrawCross(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, D3DCOLOR Color)
	{
		DrawFilledRect(X - Width, Y - 0.5f, (Width * 2.0f), 1.0f, Color);
		DrawFilledRect(X - 0.5f, Y - Height, 1.0f, (Height * 2.0f), Color);
	}

	// -----------------------------------------------------------------------------

}


