#pragma once
//=============================================================
// Scene.h
// シーンクラスの定義
//=============================================================

#ifndef _Scene_h_
#define _Scene_h_

#include <d3d9.h>
#include <d3dx9.h>


#include "Common.h"

//=============================================================
// 依存するクラス
//=============================================================

//=============================================================
// Scene
// シーンクラス
//=============================================================
class Scene
{
public:
	//---------------------------------------------------------
	// デフォルトコンストラクタ
	//---------------------------------------------------------
	Scene();

	//---------------------------------------------------------
	// デストラクタ
	//---------------------------------------------------------
	~Scene();

public:
	//---------------------------------------------------------
	// シーンを生成
	// 引数
	//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
	// 戻り値
	//		成功したらS_OK
	//---------------------------------------------------------
	HRESULT Create(LPDIRECT3DDEVICE9 pD3DDevice);

	//---------------------------------------------------------
	// 座標変換を行う
	// 引数
	//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
	//---------------------------------------------------------
	void Transform(LPDIRECT3DDEVICE9 pD3DDevice);

	//---------------------------------------------------------
	// 描画
	// 引数
	//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
	//---------------------------------------------------------
	void Draw(LPDIRECT3DDEVICE9 pD3DDevice);

	//---------------------------------------------------------
	// シーンの破棄
	//---------------------------------------------------------
	void Destroy();

private:

	D3DXMATRIX m_proj;		// 射影行列
	D3DXMATRIX m_view;		// ビューイング行列

							// 頂点バッファの構造体
	struct MY_VERTEX {
		D3DXVECTOR3 p;		// 位置
		DWORD		color;	// 色
	};

	// 頂点バッファーのフォーマットの定義
#define MY_VERTEX_FVF  ( D3DFVF_XYZ | D3DFVF_DIFFUSE )

	LPDIRECT3DVERTEXBUFFER9	m_pVB;		// 頂点バッファへのポインタ

};

#endif // _Scene_h_