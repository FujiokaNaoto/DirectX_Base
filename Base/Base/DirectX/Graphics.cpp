//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	DirectX Graphics9 �N���X
//*********************************************************************************

//==include��======================================================================
#include <windows.h>
#include "../System/common.h"
#include "Graphics.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
// �t�H���g��
const TCHAR* FONTNAME[FONT_MAX] = 
{
	_T("Terminal"),
	_T("HG�ۺ޼��M-PRO"),
	_T("HGP�n�p�p�߯�ߑ�"),
};

// �t�H���g�T�C�Y
const int	FONTSIZE[FONT_MAX] = 
{
	18,
	30,
	40,
};

namespace eGraphics
{
	const LPCTSTR	SEMAFO_NAME = _T("DEV_SEMAFO_NAME");		// �Z�}�t�H��
	const int		SEMAFO_START_CNT = 1;						// �Z�}�t�H�����J�E���g
	const int		SEMAFO_MAX_CNT = 1;							// �Z�}�t�H�ő�J�E���g

};
//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
LPDIRECT3DDEVICE9	CGraphics::m_pDevice = NULL;		// Direct3DDevice9 �I�u�W�F�N�g
D3DXMATRIX			CGraphics::m_MatProj;

HANDLE				CGraphics::m_SemafObjHnd = 0;	
HANDLE				CGraphics::m_SemafHnd = 0;		
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�O���t�B�b�N�� �̏�����
//*********************************************************************************
bool CGraphics::Initialize(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	// �f�o�C�X���e�[�u��
	struct TCreateDevice {
		D3DDEVTYPE	type;			// �f�o�C�X�̎��
		DWORD		behavior;		// �f�o�C�X�̓���
	};
	const int c_nMaxDevice = 3;
	const TCreateDevice device[c_nMaxDevice] = {
		{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// �n�[�h�E�F�A�V�F�[�_���g��
		{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// HAL
		{ D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// REF
	};
	int		nDev;			// �f�o�C�X��ʃJ�E���^

	// Direct3D�I�u�W�F�N�g�𐶐�
	if ((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		MessageBox(hWnd, _T("Direct3D�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}
	// Direct3D �������p�����[�^�̐ݒ�
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	ZeroMemory(&m_D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	m_D3DPP.BackBufferWidth        = (UINT)nWidth;				// �Q�[����ʃT�C�Y
	m_D3DPP.BackBufferHeight       = (UINT)nHeight;
	m_D3DPP.Windowed               = bWindow;					// �E�B���h�E���[�h��?
	m_D3DPP.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.EnableAutoDepthStencil = true;						// �y�o�b�t�@�L��
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;				// �y�o�b�t�@�t�H�[�}�b�g
	if (bWindow) 
	{
		// �E�B���h�E���[�h
		m_D3DPP.BackBufferFormat           = D3DFMT_UNKNOWN;				// �o�b�N�o�b�t�@
		m_D3DPP.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		m_D3DPP.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	} else {
		// �t���X�N���[�����[�h
		m_D3DPP.BackBufferFormat           = D3DFMT_X8R8G8B8;				// �o�b�N�o�b�t�@
		m_D3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
		m_D3DPP.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	for (nDev = 0; nDev < c_nMaxDevice; nDev++) {
		if (SUCCEEDED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, device[nDev].type, hWnd,	// �f�o�C�X���쐬
					  device[nDev].behavior, &m_D3DPP, &m_pDevice))) break;
	}
	if (nDev >= c_nMaxDevice) {
		MessageBox(hWnd, _T("�f�o�C�X�̍쐬�Ɏ��s���܂���"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// �t�H���g��ݒ�
	FontCreate(FONT_DEBUG);
	FontCreate(FONT_MESSAGE);
	FontCreate(FONT_SCORE);

	//-------- ��������R�c�֘A
	// �r���[�}�g���b�N�X�ݒ���V�[���Őݒ�
	
	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_MatProj, D3DXToRadian(FOVY),
		(float)m_nWidth / m_nHeight, NEAR_CLIP, FAR_CLIP);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_MatProj);

	// �}�e���A���̐ݒ�
	D3DMATERIAL9	mtrl;				// �}�e���A���I�u�W�F�N�g
	ZeroMemory(&mtrl, sizeof(mtrl));	// ��������[���ŃN���A
	mtrl.Diffuse.r = mtrl.Diffuse.g = mtrl.Diffuse.b = mtrl.Diffuse.a = 1.0f;
	mtrl.Ambient = mtrl.Diffuse;
	m_pDevice->SetMaterial(&mtrl);

	// ���C�g�̐ݒ�
	D3DXVECTOR3	vecDir;					// �������w�肷��x�N�g��
	
	ZeroMemory(&m_Light, sizeof(m_Light));
	m_Light.Type = D3DLIGHT_DIRECTIONAL;	// ���s����
	m_Light.Diffuse.r = m_Light.Diffuse.g = m_Light.Diffuse.b = m_Light.Diffuse.a = 1.0f;
	m_Light.Ambient.r = m_Light.Ambient.g = m_Light.Ambient.b = m_Light.Ambient.a = 0.2f;
	m_Light.Specular = m_Light.Diffuse;
	vecDir = D3DXVECTOR3(1.0f, -1.0f, 1.0f);					// ���̌��������߂�
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light.Direction, &vecDir);	// ���K�������x�N�g����Ԃ�
	m_pDevice->SetLight(0, &m_Light);
	m_pDevice->LightEnable(0, TRUE);	// ���C�g0��L��

	m_pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetSamplerState(0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR);

	// �A���t�@�l�̓}�e���A���~�e�N�X�`���ɐݒ�
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);


	// --�Z�}�t�H����--
	m_SemafObjHnd = 0;
	m_SemafObjHnd = 0;
	m_SemafObjHnd = CreateSemaphore(NULL , eGraphics::SEMAFO_START_CNT , 
		eGraphics::SEMAFO_MAX_CNT , eGraphics::SEMAFO_NAME);

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�O���t�B�b�N���̃N���[���A�b�v
//*********************************************************************************
void CGraphics::Finalize()
{
	for(int i = 0 ; i < FONT_MAX ; i ++)
		if(m_pFont[i]) SAFE_RELEASE(m_pFont[i]);	// D3DXFont �I�u�W�F�N�g�����

	SAFE_RELEASE(m_pDevice);						// Direct3DDevice �I�u�W�F�N�g�����
	SAFE_RELEASE(m_pD3D);							// Direct3D �I�u�W�F�N�g�����
}


//**�֐�***************************************************************************
//	�T�v	:	�`��N���X����
//*********************************************************************************
CGraphics* CGraphics::Create(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	CGraphics* pGraph = new CGraphics();
	if (pGraph) {
		if (!pGraph->Initialize(hWnd, nWidth, nHeight, bWindow)) {
			SAFE_DELETE(pGraph);
		}
	}
	return pGraph;
}


//**�֐�***************************************************************************
//	�T�v	:	�`��N���X���
//*********************************************************************************
void CGraphics::Release()
{
	Finalize();
	delete this;
}


//**�֐�***************************************************************************
//	�T�v	:	�����_�����O�J�n
//*********************************************************************************
bool CGraphics::Begin()
{
	// �r���[�|�[�g���N���A
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
					 D3DCOLOR_XRGB(128, 128, 255), 1.0f, 0);

	return SUCCEEDED(m_pDevice->BeginScene());	// �V�[���J�n
}


//**�֐�***************************************************************************
//	�T�v	:	�����_�����O�I��
//*********************************************************************************
void CGraphics::End()
{
	m_pDevice->EndScene();						// �V�[���I��
}


//**�֐�***************************************************************************
//	�T�v	:	�o�b�N�o�b�t�@����
//*********************************************************************************
void CGraphics::SwapBuffer()
{
	// �o�b�N�o�b�t�@�ɕ`�悵�����e��\��
	if (FAILED(m_pDevice->Present(NULL, NULL, NULL, NULL))) {
		m_pDevice->Reset(&m_D3DPP);
	}
}


//**�֐�***************************************************************************
//	�T�v	:	������`��(�t�H���g�F��)
//*********************************************************************************
void CGraphics::DrawText(int nX, int nY, LPCTSTR psz , int nFontType)
{
	RECT rcStr = {nX, nY, m_nWidth, m_nHeight};
	m_pFont[nFontType]->DrawText(NULL, psz, -1, &rcStr, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}


//**�֐�***************************************************************************
//	�T�v	:	������`��	(�t�H���g�F�w��)
//*********************************************************************************
void CGraphics::DrawText(int nX, int nY, LPCTSTR psz , int nFontType ,
						int nRed , int nGreen , int nBlue , int nAlpha)
{
	RECT rcStr = {nX, nY, m_nWidth, m_nHeight};
	m_pFont[nFontType]->DrawText(NULL, psz, -1, &rcStr, DT_LEFT, D3DCOLOR_RGBA(nRed , nGreen , nBlue , nAlpha));
}


//**�֐�***************************************************************************
//	�T�v	:	�Z�}�t�H���b�N
//*********************************************************************************
void CGraphics::SemafoLock()
{
	m_SemafHnd = OpenSemaphore(SEMAPHORE_ALL_ACCESS , FALSE , eGraphics::SEMAFO_NAME);

	WaitForSingleObject(m_SemafHnd , INFINITE);
}


//**�֐�***************************************************************************
//	�T�v	:	�Z�}�t�H�g�p�I��
//*********************************************************************************
void CGraphics::SemafoUnlock()
{
	ReleaseSemaphore(m_SemafHnd , 1 , NULL);
}


//**�֐�***************************************************************************
//	�T�v	:	������`��	(�t�H���g�F�w��)
//*********************************************************************************
void CGraphics::FontCreate(int nFontType)
{
	D3DXCreateFont(m_pDevice, FONTSIZE[nFontType] , 0, 0, 0,\
	FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FONTNAME[nFontType] , &m_pFont[nFontType]);
}
//=======================================================================================
//	End of File
//=======================================================================================