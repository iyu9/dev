//=============================================================
// Renderer.cpp
// レンダラークラスの実装
//=============================================================

#include "Renderer.h"
#include "Scene.h"

//=============================================================
// 実装
//=============================================================
//-------------------------------------------------------------
// デフォルトコンストラクタ
//-------------------------------------------------------------
Renderer::Renderer()
	: m_pD3D(0), m_pD3DDevice(0)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
Renderer::~Renderer()
{
	Finalize();
}

//-------------------------------------------------------------
// レンダラーの初期化
// 引数
//		hWnd         : ウィンドウハンドル
//		isFullScreen : フルスクリーンの場合TRUE
//		clientWidth  : クライアント領域の幅
//		clientHeight : クライアント領域の高さ
// 戻り値
//		成功したらS_OK 
//-------------------------------------------------------------
HRESULT Renderer::Initialize(HWND hWnd, BOOL isFullScreen, int clientWidth, int clientHeight)
{

	D3DDISPLAYMODE d3ddm;

	// Direct3D9オブジェクトの作成
	if ((m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION)) == 0) {
		return E_FAIL;	// 取得失敗
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータを初期化
	ZeroMemory(&m_D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	m_D3DPP.BackBufferCount = 1;
	if (isFullScreen) { // フルスクリーンの場合
		m_D3DPP.Windowed = FALSE;			// フルスクリーン表示の指定
		m_D3DPP.BackBufferWidth = clientWidth;		// フルスクリーン時の横幅
		m_D3DPP.BackBufferHeight = clientHeight;		// フルスクリーン時の縦幅
	}
	else {
		m_D3DPP.Windowed = TRUE;				// ウインドウ内表示の指定
	}
	m_D3DPP.BackBufferFormat = d3ddm.Format;			// カラーモードの指定
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 
	m_D3DPP.EnableAutoDepthStencil = TRUE;						// エラー対策
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D16;				// エラー対策

																// ディスプレイアダプタを表すためのデバイスを作成
																// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_D3DPP, &m_pD3DDevice))) {
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_D3DPP, &m_pD3DDevice))) {
			// 上記の設定が失敗したら
			// 描画と商店処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_D3DPP, &m_pD3DDevice))) {
				// 初期化失敗
				return E_FAIL;
			}
		}
	}


	return S_OK;
}

//---------------------------------------------------------
// デバイスの取得
// 戻り値
//		成功したらIDirect3DDevice9インターフェイスへのポインタ
//		失敗したら0
//---------------------------------------------------------
LPDIRECT3DDEVICE9 Renderer::GetDevice()
{
	return m_pD3DDevice;
}

//-------------------------------------------------------------
// シーンの描画
// 引数
//		pScene : シーンへのポインタ
// 戻り値
//		成功したらS_OK 
//-------------------------------------------------------------
void Renderer::RenderScene(Scene* pScene)
{
	if (pScene == 0) {
		return;
	}

	// ビューポートと深度バッファのクリアとステンシルバッファの削除
	if (FAILED(m_pD3DDevice->Clear(0, NULL, 		// クリアする領域は全面
		D3DCLEAR_TARGET |			// バックバッファを指定
		D3DCLEAR_ZBUFFER, 			// 深度バッファ（Zバッファ）を指定
		D3DCOLOR_XRGB(0, 0, 0), 		// クリアする色
		1.0f, 						// クリアする深度バッファ（Zバッファ）の値
		0))) {						// クリアするステンシルバッファの値
		return;
	}

	// 描画開始宣言
	if (SUCCEEDED(m_pD3DDevice->BeginScene())) {

		// モデルなどの描画
		pScene->Draw(m_pD3DDevice);

		// 描画終了宣言
		m_pD3DDevice->EndScene();
	}

	// 描画結果の転送
	if (FAILED(m_pD3DDevice->Present(0, 0, 0, 0))) {
		// デバイス消失から復帰
		m_pD3DDevice->Reset(&m_D3DPP);
	}
}

//-------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------
void Renderer::Finalize()
{
	// デバイスオブジェクトの解放
	SAFE_RELEASE(m_pD3DDevice);

	// DirectXGraphicsの解放
	SAFE_RELEASE(m_pD3D);
}