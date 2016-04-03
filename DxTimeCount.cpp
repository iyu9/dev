#define DEBUG
#define APPNAME "DirectX Sample Program"
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#include <string>
#include <locale.h>

#pragma once
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")

#pragma comment(lib,"winmm.lib")

#define SAFE_DELETE(p)  { if (p) { delete (p);     (p) = nullptr; } }
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = nullptr; } }
#define EMSG(x)         MessageBox(nullptr, x, "DirectX D3D", MB_OK);

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;
	DWORD color;
	FLOAT tu, tv;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

HWND                    g_hWnd = nullptr;
LPDIRECT3D9             g_pD3D = nullptr;
LPDIRECT3DDEVICE9       g_pDev = nullptr;
LPDIRECT3DTEXTURE9      g_pTexture = nullptr;
LPD3DXMESH              g_pMesh = nullptr;
LPD3DXFONT              g_pFont = nullptr;
LPDIRECT3DVERTEXBUFFER9 g_pVB = nullptr;
LPD3DXSPRITE            g_pSprite = nullptr;

LPDIRECTINPUT8 g_pDInput = nullptr;
LPDIRECTINPUTDEVICE8 g_pDIDevice = nullptr;

D3DMATERIAL9            material;
D3DLIGHT9               light;
D3DXVECTOR3             ViewForm(0.0f, 0.0f, -5.0f);

D3DXVECTOR3             pos;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam);

//read file name
char g_imgfile[] = "test.jpg";

//text
std::string g_message = "「私はあなた。何がしたい？」";
//std::string g_message = "abcdefghijk";
std::string g_const_message = g_message;
int message_index = 0;

//window param
int g_width = 800;
int g_height = 680;

//timer
float time = 0;
float CurrentTime = time;

HRESULT InitDirectInput(HINSTANCE hInstance)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, nullptr))) {
		return E_FAIL;
	};

	if (FAILED(g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevice, nullptr))) {
		return E_FAIL;
	};

	g_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	g_pDIDevice->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	g_pDIDevice->Acquire();

	return S_OK;
}

HRESULT InitFont(HWND hWnd)
{
	HRESULT hr = D3DXCreateFont(g_pDev, 24, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "ＭＳ ゴシック", &g_pFont);

	if FAILED(hr) {
		return(E_FAIL);
	}
	return S_OK;
}

void TextDraw(LPD3DXFONT pFont, const char* text, int X, int Y)
{
	RECT rect = { X, Y, 0, 0 };
	pFont->DrawText(nullptr, text, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void TextureDraw(LPD3DXSPRITE &pSprite, LPDIRECT3DTEXTURE9 &pTex, int SrcX, int SrcY, int SrcW, int SrcH, int DstX, int DstY)
{
	RECT rect = { SrcX, SrcY, SrcW, SrcH };
	D3DXVECTOR3 Center(0, 0, 0);
	D3DXVECTOR3 Pos((float)DstX, (float)DstY, 0);
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pSprite->Draw(pTex, &rect, &Center, &Pos, 0xffffffff);
	pSprite->End();
}

HRESULT Init3DDev(HWND hwnd, LPDIRECT3D9 *d3d, LPDIRECT3DDEVICE9 *d3Device)
{
	D3DPRESENT_PARAMETERS   d3dpp;
	D3DDISPLAYMODE          dmode;

	*d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (*d3d == nullptr)
	{
		return E_FAIL;
	}

	if (FAILED((*d3d)->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dmode)))
	{
		return E_FAIL;
	}

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = dmode.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED((*d3d)->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, d3Device))) {

		if (FAILED((*d3d)->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, d3Device))) {

			if (FAILED((*d3d)->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hwnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, d3Device))) {
				EMSG("Create Device Error");
				return E_FAIL;
			}
		}
	}

	/*
	if (FAILED(DirectInput8Create(NULL, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL)))
	{
	return E_FAIL;
	}

	if (FAILED(g_pDInput->CreateDevice(GUID_SysKeyboard, NULL, NULL)))
	{
	return E_FAIL;
	}
	*/

	if (FAILED(D3DXCreateTextureFromFile(g_pDev, g_imgfile, &g_pTexture)))
	{
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(g_pDev, &g_pSprite)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT SetVertexBuffer()
{
	CUSTOMVERTEX Vertices[] =
	{
		{ g_width / 2 + pos.x, 0.0f + pos.y + 10 * time, 0.0f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
		{ g_width - 20 *  time, g_height / 2, 0.0f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
		{ 0.0f + 20 * time, g_height / 2, 0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
		{ g_width / 2, g_height - 10 * time, 0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },

		/*
		{ g_width / 2, 0.0f + pos.y, 10.0f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
		{ g_width, g_height / 2, 0.0f, 10.0f, 0xFFFFFFFF, 1.0f, 0.0f },
		{ 0.0f, g_height / 2, 0.0f, 10.0f, 0xFFFFFFFF, 0.0f, 1.0f },
		{ g_width / 2, g_height, 0.0f, 10.0f, 0xFFFFFFFF, 0.0f, 1.0f },
		*/
	};

	if (FAILED(g_pDev->CreateVertexBuffer(sizeof(Vertices), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, nullptr)))
	{
		return E_FAIL;
	}

	void* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(Vertices), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}
	memcpy(pVertices, Vertices, sizeof(Vertices));
	g_pVB->Unlock();

	return S_OK;
}

HRESULT Init3DDevices(void)
{
	if (FAILED(Init3DDev(g_hWnd, &g_pD3D, &g_pDev)))
	{
		return E_FAIL;
	}

	/*
	D3DXCreateTeapot(g_pDev, &g_pMesh, nullptr);
	*/
	material.Diffuse.r = material.Diffuse.g = 1.0f;
	material.Diffuse.b = 0.0f;
	material.Ambient.r = material.Ambient.g = material.Ambient.b = 0.5f;
	material.Specular.r = material.Specular.g = material.Specular.b = 0.0f;
	material.Emissive.r = material.Emissive.g = material.Emissive.b = 0.0f;
	material.Power = 0;

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 1.0f;
	light.Specular.r = light.Specular.g = light.Specular.b = 0.2f;
	light.Ambient.r = light.Ambient.g = light.Ambient.b = 0.5f;
	light.Direction = D3DXVECTOR3(1, -2, 1);

	return S_OK;
}

void Input()
{
	HRESULT hr;
	BYTE diKeyState[256];

	hr = g_pDIDevice->GetDeviceState(256, diKeyState);

	if (SUCCEEDED(hr))
	{
		if (diKeyState[DIK_LEFT] & 0x80)
		{
			g_message = "←";
			pos.x -= 10;
		}

		if (diKeyState[DIK_RIGHT] & 0x80)
		{
			g_message = "→";
			pos.x += 10;
		}

		if (diKeyState[DIK_UP] & 0x80)
		{
			g_message = "↑";
			pos.y += 10;
		}

		if (diKeyState[DIK_DOWN] & 0x80)
		{
			g_message = "↓";
			pos.y -= 10;
		}

		if (diKeyState[DIK_LSHIFT] & 0x80)
		{
			g_message = "SHIFT";
		}

		if (diKeyState[DIK_Z] & 0x80)
		{
			g_message = "Z";
		}

		if (diKeyState[DIK_X] & 0x80)
		{
			g_message = "X";
		}

		if (diKeyState[DIK_C] & 0x80)
		{
			g_message = "C";
		}
	}
	else {
		g_pDIDevice->Acquire();
	}
}


void SetupMatrices()
{
	RECT            rect;
	D3DXMATRIX      matWorld;
	D3DXMATRIX      matView;
	D3DXMATRIX      matProj;

	GetClientRect(g_hWnd, &rect);

#ifndef DEBUG
	//D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
	//g_pDev->SetTransform(D3DTS_WORLD, &matWorld);
#endif

	D3DXMatrixLookAtLH(&matView, &ViewForm,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	g_pDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.0f), (float)rect.right / (float)rect.bottom, 1, 50);
	g_pDev->SetTransform(D3DTS_PROJECTION, &matProj);
	g_pDev->SetRenderState(D3DRS_AMBIENT, 0x00080808);

	g_pDev->SetLight(0, &light);
	g_pDev->LightEnable(0, TRUE);
	g_pDev->SetMaterial(&material);
}

void Render(void)
{
	g_pDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(g_pDev->BeginScene()))
	{
		time += 0.001 * timeGetTime() - CurrentTime;
		CurrentTime = 0.001 * timeGetTime();

		g_message = std::to_string(time);

		//文字送りテスト
		for (int i = 0; i < g_const_message.length(); i++) {
			if (i <= time && time < i+1) {

				//マルチバイト判定
				/*
				if (mblen(&g_message[message_index], MB_CUR_MAX) == 2) {
					message_index += 2;
				}
				else {
					message_index++;
				}
				*/

				message_index = i;
				g_message = g_const_message.substr(0, i+1);  //マルチバイト送り必要
			}
		}
		//for (int i = 0; i < (int)g_message.size(); ++i) {
		//	char ch = str[i];
		//}

		TextDraw(g_pFont, g_message.c_str(), 0, 0);
		//TextureDraw(g_pSprite, g_pTexture, 0 + pos.x, 0 - pos.y, 1280 + pos.x, 720 + pos.y, 50, 50);
		g_pDev->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		//g_pDev->SetTexture(0, g_pTexture);

		SetupMatrices();

		//update vert test
		SetVertexBuffer();

		//g_pMesh->DrawSubset(0);
		g_pDev->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		g_pDev->EndScene();
	}

	g_pDev->Present(nullptr, nullptr, nullptr, nullptr);
}

void Release(void)
{
	SAFE_RELEASE(g_pSprite);
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pMesh);
	SAFE_RELEASE(g_pDev);
	SAFE_RELEASE(g_pD3D);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0L;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	MSG msg;

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, hInst,
		nullptr, nullptr, nullptr, nullptr, APPNAME, nullptr };

	if (RegisterClassEx(&wc) == 0)
	{
		return FALSE;
	}

	RECT rect;
	SetRect(&rect, 0, 0, g_width, g_height);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	rect.right = g_width;
	rect.bottom = g_height;
	rect.top = 0;
	rect.left = 0;

	g_hWnd = CreateWindowEx(0, APPNAME, APPNAME, WS_OVERLAPPEDWINDOW,
		0, 0, g_width, g_height,
		nullptr, nullptr, hInst, nullptr);

	if (FAILED(Init3DDevices()))
	{
		return FALSE;
	}

	if (FAILED(InitDirectInput(hInst)))
	{
		return FALSE;
	}

	if (FAILED(InitFont(g_hWnd)))
	{
		return FALSE;
	}

	if (FAILED(SetVertexBuffer()))
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(g_hWnd);

	CurrentTime = 0.001 * timeGetTime();

	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Input();
			Render();
		}
	}

	Release();
	UnregisterClass(APPNAME, wc.hInstance);
	return 0;
}
