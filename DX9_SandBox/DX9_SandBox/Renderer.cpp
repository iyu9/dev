//=============================================================
// Renderer.cpp
// �����_���[�N���X�̎���
//=============================================================

#include "Renderer.h"
#include "Scene.h"

//=============================================================
// ����
//=============================================================
//-------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//-------------------------------------------------------------
Renderer::Renderer()
	: m_pD3D(0), m_pD3DDevice(0)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
Renderer::~Renderer()
{
	Finalize();
}

//-------------------------------------------------------------
// �����_���[�̏�����
// ����
//		hWnd         : �E�B���h�E�n���h��
//		isFullScreen : �t���X�N���[���̏ꍇTRUE
//		clientWidth  : �N���C�A���g�̈�̕�
//		clientHeight : �N���C�A���g�̈�̍���
// �߂�l
//		����������S_OK 
//-------------------------------------------------------------
HRESULT Renderer::Initialize(HWND hWnd, BOOL isFullScreen, int clientWidth, int clientHeight)
{

	D3DDISPLAYMODE d3ddm;

	// Direct3D9�I�u�W�F�N�g�̍쐬
	if ((m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION)) == 0) {
		return E_FAIL;	// �擾���s
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^��������
	ZeroMemory(&m_D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	m_D3DPP.BackBufferCount = 1;
	if (isFullScreen) { // �t���X�N���[���̏ꍇ
		m_D3DPP.Windowed = FALSE;			// �t���X�N���[���\���̎w��
		m_D3DPP.BackBufferWidth = clientWidth;		// �t���X�N���[�����̉���
		m_D3DPP.BackBufferHeight = clientHeight;		// �t���X�N���[�����̏c��
	}
	else {
		m_D3DPP.Windowed = TRUE;				// �E�C���h�E���\���̎w��
	}
	m_D3DPP.BackBufferFormat = d3ddm.Format;			// �J���[���[�h�̎w��
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 
	m_D3DPP.EnableAutoDepthStencil = TRUE;						// �G���[�΍�
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D16;				// �G���[�΍�

																// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
																// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_D3DPP, &m_pD3DDevice))) {
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_D3DPP, &m_pD3DDevice))) {
			// ��L�̐ݒ肪���s������
			// �`��Ə��X������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_D3DPP, &m_pD3DDevice))) {
				// ���������s
				return E_FAIL;
			}
		}
	}


	return S_OK;
}

//---------------------------------------------------------
// �f�o�C�X�̎擾
// �߂�l
//		����������IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
//		���s������0
//---------------------------------------------------------
LPDIRECT3DDEVICE9 Renderer::GetDevice()
{
	return m_pD3DDevice;
}

//-------------------------------------------------------------
// �V�[���̕`��
// ����
//		pScene : �V�[���ւ̃|�C���^
// �߂�l
//		����������S_OK 
//-------------------------------------------------------------
void Renderer::RenderScene(Scene* pScene)
{
	if (pScene == 0) {
		return;
	}

	// �r���[�|�[�g�Ɛ[�x�o�b�t�@�̃N���A�ƃX�e���V���o�b�t�@�̍폜
	if (FAILED(m_pD3DDevice->Clear(0, NULL, 		// �N���A����̈�͑S��
		D3DCLEAR_TARGET |			// �o�b�N�o�b�t�@���w��
		D3DCLEAR_ZBUFFER, 			// �[�x�o�b�t�@�iZ�o�b�t�@�j���w��
		D3DCOLOR_XRGB(0, 0, 0), 		// �N���A����F
		1.0f, 						// �N���A����[�x�o�b�t�@�iZ�o�b�t�@�j�̒l
		0))) {						// �N���A����X�e���V���o�b�t�@�̒l
		return;
	}

	// �`��J�n�錾
	if (SUCCEEDED(m_pD3DDevice->BeginScene())) {

		// ���f���Ȃǂ̕`��
		pScene->Draw(m_pD3DDevice);

		// �`��I���錾
		m_pD3DDevice->EndScene();
	}

	// �`�挋�ʂ̓]��
	if (FAILED(m_pD3DDevice->Present(0, 0, 0, 0))) {
		// �f�o�C�X�������畜�A
		m_pD3DDevice->Reset(&m_D3DPP);
	}
}

//-------------------------------------------------------------
// �I������
//-------------------------------------------------------------
void Renderer::Finalize()
{
	// �f�o�C�X�I�u�W�F�N�g�̉��
	SAFE_RELEASE(m_pD3DDevice);

	// DirectXGraphics�̉��
	SAFE_RELEASE(m_pD3D);
}