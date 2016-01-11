#include<d3d9.h>
#include<dinput.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dinput8.lib")

#define SAFE_RELEASE(p) if(p){(p)->Release();(p)=NULL;}

PDIRECT3D9 g_pD3D = NULL;                       //Direct3Dへアクセスするためのポインタ
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;          //ビデオカードへアクセスするためのポインタ


LPDIRECTINPUT8 g_pDInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDIDevice = NULL;

HRESULT Init(HWND);
void Render();
void Cleanup();

HRESULT Init(HWND hWnd){
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL){
		return E_FAIL;
	}

	//Direct3Dデバイスの生成
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	//DirectInput8インターフェイスの取得
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	//DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL);
	//g_pDInput->EnumDevices(DI8DEVCLASS_KEYBOARD, NULL,)
	//g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevice, NULL);
	// データフォーマットの設定（定義済みのキーボード用フォーマットを使用）
	//g_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	//g_pDIDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	//g_pDIDevice->Acquire();

	//ビデオカードのオブジェクト生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice))){
		return E_FAIL;
	}

	return S_OK;
}

//DX描画
void Render() {
    //背景を黒に塗りつぶす
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    g_pd3dDevice->BeginScene(); //描画開始
    g_pd3dDevice->EndScene();   //描画終了
    //描画した物をウィンドウに反映
    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

//デバイス解放
void Cleanup() {
	SAFE_RELEASE(g_pD3D);
	SAFE_RELEASE(g_pd3dDevice);
}

//メッセージ処理
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	LPTSTR lptStr = TEXT("Kitty on your lap");
	hdc = GetDC(hWnd);
	TextOut(hdc, 10, 10, lptStr, lstrlen(lptStr));
	ReleaseDC(hWnd, hdc);

	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		break;
	case WM_PAINT:
		Render();
		break;
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void GameMainLoop() {
	HRESULT hr;

	BYTE diKeyState[256];

	hr = g_pDIDevice->GetDeviceState(256, diKeyState);
	float tiger_x = 0;
	float tiger_y = 0;

	if (SUCCEEDED(hr))

	{

		if (diKeyState[DIK_LEFT] & 0x80)

		{

			tiger_x -= 0.1f;

		}

		if (diKeyState[DIK_RIGHT] & 0x80)

		{

			tiger_x += 0.1f;

		}

		if (diKeyState[DIK_UP] & 0x80)

		{

			tiger_y += 0.1f;

		}

		if (diKeyState[DIK_DOWN] & 0x80)

		{

			tiger_y -= 0.1f;

		}

	}

	else

	{

		g_pDIDevice->Acquire();

	}
}

//エントリポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR nCmdLine, int nCmdShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		NULL,
		WindowProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		NULL,NULL,
		TEXT("TEST"),
		NULL
	};

	if (!RegisterClassEx(&wcex)) {
		return 0;
	}

	hWnd = CreateWindow(
		TEXT("TEST"),
		TEXT("Kitty on your lap"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		NULL, 
		NULL,
		hInstance, 
		NULL
	);

	//ウィンドウ生成成功か
	if (hWnd == NULL) {
		return 0;
	}

	//グラフィックス初期化へ
	if (FAILED(Init(hWnd))) {
		return 0;
	}
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)){
		DispatchMessage(&msg);
		//GameMainLoop();
	}
	return msg.wParam;
}