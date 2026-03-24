#ifndef _D3D9_Hook_HPP_
#define _D3D9_Hook_HPP_

#include "Render_DX9.hpp"

class D3D9_Hook
{
public:
	void Hook();

private:
	IDirect3DDevice9 * pDevice;
};
extern D3D9_Hook D3D9;


#endif // !_hD3D9_Hook_HPP_
