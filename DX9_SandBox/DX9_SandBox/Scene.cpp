#include "Scene.h"

//
// Scene.cpp
// �V�[���̎���
//

#include <windows.h>
#include <mmsystem.h> //for timeGetTime()

#include "Scene.h"

#ifdef _DEBUG
#include <stdio.h> //for sprintf
#endif

//=============================================================
// ����
//=============================================================
//-------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//-------------------------------------------------------------
Scene::Scene()
	: m_pVB(0)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
Scene::~Scene()
{

	Destroy();
}

//-------------------------------------------------------------
// �V�[���𐶐�
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
// �߂�l
//		����������S_OK
//-------------------------------------------------------------
HRESULT Scene::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
	Destroy();

	if (pD3DDevice == 0) {
		return E_FAIL;
	}

	// �I�u�W�F�N�g��VertexBuffer�𐶐�
	if (FAILED(pD3DDevice->CreateVertexBuffer(3 * sizeof(MY_VERTEX),
		D3DUSAGE_WRITEONLY,
		MY_VERTEX_FVF,
		D3DPOOL_MANAGED, &m_pVB, NULL))) {
		return E_FAIL;
	}

	//VertexBuffer�̒��g�𖄂߂�
	MY_VERTEX* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);
	// �e���_�̈ʒu
	v[0].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	v[1].p = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	v[2].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	// �e���_�̐F
	v[0].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	v[1].color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	v[2].color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	m_pVB->Unlock();

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �r���[�C���O�s��̏�����
	//D3DXMatrixIdentity(&m_view);
	//D3DXMatrixLookAtLH(&m_view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f),
		//&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	return S_OK;
}

//-------------------------------------------------------------
// ���W�ϊ����s��
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
//-------------------------------------------------------------
void Scene::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if (pD3DDevice == 0) {
		return;
	}

	// �r���[�|�[�g�̎擾
	D3DVIEWPORT9    vp;
	if (FAILED(pD3DDevice->GetViewport(&vp))) {
		return;
	}

	// �A�X�y�N�g��̌v�Z
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	// �ˉe�s��̏�����
	//D3DXMatrixIdentity(&m_proj);
	//D3DXMatrixPerspectiveFovLH(&m_proj, D3DXToRadian(45.0f), aspect, 5.0f, 1000.0f);

	// �ˉe�s��̐ݒ�
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);

	// �J�����̐ݒ�
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);
}

//-------------------------------------------------------------
// �I�u�W�F�N�g���̕`��
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
//-------------------------------------------------------------
void Scene::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if (pD3DDevice == 0) {
		return;
	}

	if (m_pVB == 0) {
		return;
	}

	// ���W�ϊ��i�J�����̐ݒ�Ȃǁj
	Transform(pD3DDevice);

	// �O�p�`�̕`�揈��
	// ���_�o�b�t�@�̐ݒ�
	pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(MY_VERTEX));
	// ���_�o�b�t�@�̃t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(MY_VERTEX_FVF);
	// ���_�o�b�t�@�̕`��
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

}

//-------------------------------------------------------------
// �V�[���̔j��
//-------------------------------------------------------------
void Scene::Destroy()
{
	// ���_�o�b�t�@�̉��
	SAFE_RELEASE(m_pVB);
}
