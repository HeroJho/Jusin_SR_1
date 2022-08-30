#pragma once

namespace Engine
{
	// D3DDECLUSAGE

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexture;
		
	}VTXTEX;

	typedef struct tagVertexColor
	{
		D3DXVECTOR3		vPosition;
		unsigned long	dwColor;
	}VTXCOL;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		HWND hWnd;
		unsigned int iWinSizeX;
		unsigned int iWinSizeY;
		WINMODE eWinMode;

	}GRAPHICDESC;

}
