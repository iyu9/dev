#include "Scene.h"

//
// Scene.cpp
// シーンの実装
//

#include <windows.h>
#include <mmsystem.h> //for timeGetTime()

#include "Scene.h"

#ifdef _DEBUG
#include <stdio.h> //for sprintf
#endif

//=============================================================
// 実装
//=============================================================
//-------------------------------------------------------------
// デフォルトコンストラクタ
//-------------------------------------------------------------
Scene::Scene()
	: m_pVB(0)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
Scene::~Scene()
{

	Destroy();
}

//-------------------------------------------------------------
// シーンを生成
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
// 戻り値
//		成功したらS_OK
//-------------------------------------------------------------
HRESULT Scene::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
	Destroy();

	if (pD3DDevice == 0) {
		return E_FAIL;
	}

	// オブジェクトのVertexBufferを生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(3 * sizeof(MY_VERTEX),
		D3DUSAGE_WRITEONLY,
		MY_VERTEX_FVF,
		D3DPOOL_MANAGED, &m_pVB, NULL))) {
		return E_FAIL;
	}

	//VertexBufferの中身を埋める
	MY_VERTEX* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);
	// 各頂点の位置
	v[0].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	v[1].p = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	v[2].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	// 各頂点の色
	v[0].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	v[1].color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	v[2].color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	m_pVB->Unlock();

	// レンダリングステートパラメータの設定
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ビューイング行列の初期化
	//D3DXMatrixIdentity(&m_view);
	//D3DXMatrixLookAtLH(&m_view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f),
		//&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	return S_OK;
}

//-------------------------------------------------------------
// 座標変換を行う
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
//-------------------------------------------------------------
void Scene::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if (pD3DDevice == 0) {
		return;
	}

	// ビューポートの取得
	D3DVIEWPORT9    vp;
	if (FAILED(pD3DDevice->GetViewport(&vp))) {
		return;
	}

	// アスペクト比の計算
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	// 射影行列の初期化
	//D3DXMatrixIdentity(&m_proj);
	//D3DXMatrixPerspectiveFovLH(&m_proj, D3DXToRadian(45.0f), aspect, 5.0f, 1000.0f);

	// 射影行列の設定
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);

	// カメラの設定
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);
}

//-------------------------------------------------------------
// オブジェクト等の描画
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
//-------------------------------------------------------------
void Scene::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if (pD3DDevice == 0) {
		return;
	}

	if (m_pVB == 0) {
		return;
	}

	// 座標変換（カメラの設定など）
	Transform(pD3DDevice);

	// 三角形の描画処理
	// 頂点バッファの設定
	pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(MY_VERTEX));
	// 頂点バッファのフォーマットの設定
	pD3DDevice->SetFVF(MY_VERTEX_FVF);
	// 頂点バッファの描画
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

}

//-------------------------------------------------------------
// シーンの破棄
//-------------------------------------------------------------
void Scene::Destroy()
{
	// 頂点バッファの解放
	SAFE_RELEASE(m_pVB);
}
