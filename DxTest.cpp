#define DEBUG
#define APPNAME "DirectX Sample Program"
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#include <string>

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
#define EMSG(x)         MessageBox(NULL,x,"DirectX D3D",MB_OK);

struct CUSTOMVERTEX
{
  FLOAT x, y, z, rhw;
  DWORD color;
  FLOAT tu, tv;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

HWND                    g_hWnd;
LPDIRECT3D9             g_pD3D =      nullptr;
LPDIRECT3DDEVICE9       g_pDev =      nullptr;
LPDIRECT3DTEXTURE9      g_pTexture =  nullptr;
LPD3DXMESH              g_pMesh =     nullptr;
LPD3DXFONT              g_pFont =     nullptr;
LPDIRECT3DVERTEXBUFFER9 g_pVB =       nullptr;
LPD3DXSPRITE            g_pSprite =   nullptr;

LPDIRECTINPUT8 g_pDInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDIDevice = NULL;

D3DMATERIAL9            material;
D3DLIGHT9               light;
D3DXVECTOR3             ViewForm(0.0f, 0.0f, -5.0f);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam);

char g_imgfile[] = "test.png";
std::string g_message = ">HELLO";

HRESULT InitFont(HWND hWnd)
{
  HRESULT hr = D3DXCreateFont(g_pDev, 24, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
    ANTIALIASED_QUALITY, FF_DONTCARE, "ＭＳ ゴシック", &g_pFont);
  if FAILED(hr){ return(E_FAIL); }
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
  if (FAILED(DirectInput8Create(NULL, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL))) {
    return E_FAIL;
  };

  if (FAILED(g_pDInput->CreateDevice(GUID_SysKeyboard, NULL, NULL))){
    return E_FAIL;
  };
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

HRESULT InitVertexBuffer()
{
  CUSTOMVERTEX Vertices[] =
  {
    { 50.00f, 50.00f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
    { 100.0f, 50.00f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
    { 100.0f, 100.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
    { 50.00f, 100.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f }
  };

  if (FAILED(g_pDev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
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

  D3DXCreateTeapot(g_pDev, &g_pMesh, nullptr);

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

void SetupMatrices(void)
{
  RECT            rect;
  D3DXMATRIX      matWorld;
  D3DXMATRIX      matView;
  D3DXMATRIX      matProj;

  GetClientRect(g_hWnd, &rect);

#ifndef DEBUG
  D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
  g_pDev->SetTransform(D3DTS_WORLD, &matWorld);
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
    TextDraw(g_pFont, g_message.c_str(), 0, 0);
    TextureDraw(g_pSprite, g_pTexture, 0, 0, 1280, 720, 50, 50);
    g_pDev->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
    //g_pDev->SetTexture(0, g_pTexture);

    SetupMatrices();
    g_pMesh->DrawSubset(0);

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

    case WM_KEYDOWN:
      if (wParam == VK_SHIFT)
      {
        g_message = "SHIFT";
      }

      if (wParam == 0x5A)
      {
        g_message = "Z";
      }

      if (wParam == 0x58)
      {
        g_message = "X";
      }

      if (wParam == 0x43)
      {
        g_message = "C";
      }

      if (wParam == VK_UP)
      {
        g_message = "KEY_UP";
      }
      if (wParam == VK_DOWN)
      {
        g_message = "KEY_DOWN";
      }
      if (wParam == VK_RIGHT)
      {
        g_message = "KEY_RIGHT";
      }
      if (wParam == VK_LEFT)
      {
        g_message = "KEY_LEFT";
      }

      break;
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
  SetRect(&rect, 0, 0, 400, 400);
  AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
  rect.right = 400;
  rect.bottom = 400;
  rect.top = 0;
  rect.left = 0;

  g_hWnd = CreateWindowEx(0, APPNAME, APPNAME, WS_OVERLAPPEDWINDOW,
    400, 400, 800, 800,
    nullptr, nullptr, hInst, nullptr);

  if (FAILED(Init3DDevices()))
  {
    return FALSE;
  }

  InitFont(g_hWnd);
  InitVertexBuffer();
  ShowWindow(g_hWnd, SW_SHOWDEFAULT);
  UpdateWindow(g_hWnd);

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
      Render();
    }
  }

  Release();
  UnregisterClass(APPNAME, wc.hInstance);
  return 0;
}
