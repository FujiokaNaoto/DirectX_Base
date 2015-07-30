//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	2D�I�u�W�F�N�g�x�[�X
//*********************************************************************************


//==include��======================================================================
#include "../../System/GameManager.h"
#include "Obj2D.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================

//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//==�ÓI�����o�ϐ���===============================================================

//=================================================================================


//==�v���g�^�C�v�錾===============================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CObj2D::CObj2D():
m_pTexture(NULL),
m_nAnimCnt(0),
m_nAnimFrame(1),
m_nTexPatternNum(1),
m_nPatternNumU(1),
m_nPatternNumV(1),
m_nCurPatternU(0),
m_nCurPatternV(0)
{
	m_nClassID = eClassCode::OBJ_2D;
	m_nRSType = m_nRSTypeLate = eRSType::OBJ_2D;				// �����_�[�X�e�[�g�^�C�v�ݒ�

	m_HitRect.m_fWidth = 0.0f;
	m_HitRect.m_fHeight = 0.0f;
	m_HitRect.m_fDepth = 0.0f;
	m_HitRect.m_nPointType = eHitRect::CENTER;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CObj2D::~CObj2D()
{

}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObj2D* CObj2D::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		 int nWidth , int nHeight , int nSceneID , int nPriority)
{
	CObj2D*		pObj = new CObj2D;

	if(pObj->Init(texName , pos , nWidth , nHeight , nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObj2D* CObj2D::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority)
{
	CObj2D*		pObj = new CObj2D;

	if(pObj->Init(texName , pos , rot , scall , nWidth , nHeight , nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CObj2D::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority)
{
	return Init(texName , pos , DEFAULT_ROT , DEFAULT_SCALL , nWidth , 
		nHeight , nSceneID , nPriority);
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CObj2D::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority)
{
	// --�����l�i�[--
	if(texName != NULL)
	{
		m_pTexture = MANAGER.GetTexManage()->Entry(texName);
	}

	m_Pos = pos;								// �ʒu
	m_Rot = rot;								// ��]�p
	m_Scall = scall;							// �g�嗦
	
	m_HitRect.m_fWidth = (float)nWidth;			// ��
	m_HitRect.m_fHeight = (float)nHeight;		// ����

	m_nSceneID = nSceneID;						// �V�[��ID
	m_nPriority = nPriority;					// �v���C�I���e�B

	// --������p���a--
	if(m_HitRect.m_fWidth > m_HitRect.m_fHeight)
		m_fRadius = m_HitRect.m_fWidth / 2.0f;
	else
		m_fRadius = m_HitRect.m_fHeight / 2.0f;

	// --�|���S��������--
	SetTex();
	SetVtx();
	SetDiffuse(255 , 255 , 255 , 255);

	Affine();

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	���
//*********************************************************************************
void CObj2D::Release()
{
	Uninit();
	delete this;
}


//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void CObj2D::Update()
{
	AnimUpdate();
}


//**�֐�***************************************************************************
//	�T�v	:	�A�j���[�V�����X�V
//*********************************************************************************
void CObj2D::AnimUpdate()
{
	m_nAnimCnt ++;

	if(m_nAnimFrame <= m_nAnimCnt)
	{
		m_nCurPatternU ++;
		if(m_nCurPatternU >= m_nPatternNumU)
		{
			m_nCurPatternU = 0;
			m_nCurPatternV ++;			

			if(m_nCurPatternV >= m_nPatternNumV)
				m_nCurPatternV = 0;
		}

		if(m_nTexPatternNum <= m_nCurPatternU + m_nCurPatternV * m_nPatternNumU)
				m_nCurPatternU = m_nCurPatternV = 0;

		m_nAnimCnt = 0;

		SetTex();									// �e�N�X�`���ʒu�ύX
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�A�t�B���ϊ�
//*********************************************************************************
void CObj2D::Affine()
{
	// �g���Ă��Ȃ��Ƃ��͏����Ȃ�
	if(!m_bUseFlg) return;

	D3DXMATRIX	matTrans , matRot , matScall;

	// ������
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScall);

	// �A�t�B��
	D3DXMatrixTranslation(&matTrans , m_Pos.x , m_Pos.y , m_Pos.z);			// �ړ�
	D3DXMatrixRotationYawPitchRoll(&matRot , m_Rot.y , m_Rot.x , m_Rot.z);	// ��]
	D3DXMatrixScaling(&matScall , m_Scall.x , m_Scall.y , m_Scall.z);		// �g�k

	m_Matrix = matRot * matScall * matTrans;


	for(int n = 0 ; n < 4 ; n ++)
	{
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_vVtx[n].vtx , (D3DXVECTOR3*) &m_vVtxBase[n].vtx , &m_Matrix);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_vVtxNTex[n].vtx , (D3DXVECTOR3*) &m_vVtxNTexBase[n].vtx , &m_Matrix);
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�����_�[�X�e�[�g�ݒ�
//*********************************************************************************
void CObj2D::SetRS()
{
	CGraphics::SemafoLock();
	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
	
	// --2D�p�ݒ�ɕύX--
	// ���C�g�I�t
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@ �u�����h�L����
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�o�b�t�@�I�t
	pDevice->SetRenderState(D3DRS_ZENABLE , FALSE);
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	��`��p�����_�[�X�e�[�g�ݒ�
//*********************************************************************************
void CObj2D::SetRSLate()
{
	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();

	// --2D�p�ݒ�ɕύX--
	// ���C�g�I�t
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// �A���t�@ �u�����h�L����
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�o�b�t�@�I�t
	pDevice->SetRenderState(D3DRS_ZENABLE , FALSE);

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	�`��
//*********************************************************************************
void CObj2D::Draw()
{
	// �g�p�t���O���I�t�܂��͌�`��t���O���I���Ȃ�`�悵�Ȃ�
	if(!m_bUseFlg || m_bDrawLate) return;

	CGraphics::SemafoLock();
	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();

	pDevice->SetTransform(D3DTS_WORLD , &m_Matrix);
	
	if(m_pTexture) 
	{
		pDevice->SetFVF(FVF_VERTEX_2D);
		pDevice->SetTexture(0 , *m_pTexture);				// �e�N�X�`���Z�b�g
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx , sizeof(VERTEX_2D));
	}
	else
	{
		pDevice->SetFVF(FVF_VERTEX_2D_TEXNONE);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtxNTex , sizeof(VERTEX_2D_TEXNONE));
	}
	
	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	�`��
//*********************************************************************************
void CObj2D::DrawLate()
{
	// �g�p�t���O���I�t�܂��͌�`��t���O���I�t�Ȃ�`�悵�Ȃ�
	if(!m_bUseFlg || ! m_bDrawLate) return;
	
	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
	pDevice->SetTransform(D3DTS_WORLD , &m_Matrix);

	if(m_pTexture) 
	{
		pDevice->SetFVF(FVF_VERTEX_2D);
		pDevice->SetTexture(0 , *m_pTexture);			// �e�N�X�`���Z�b�g
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx , sizeof(VERTEX_2D));
	}
	else
	{
		pDevice->SetFVF(FVF_VERTEX_2D_TEXNONE);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtxNTex , sizeof(VERTEX_2D_TEXNONE));
	}
	
	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	�e�N�X�`���Z�b�g
//*********************************************************************************
void CObj2D::SetTex()
{
	float fPerU = 1.0f / m_nPatternNumU;
	float fPerV = 1.0f / m_nPatternNumV;

	//�e�N�X�`���\��t��
	m_vVtx[0].tex = D3DXVECTOR2(0.0f  + m_nCurPatternU * fPerU , 0.0f  + m_nCurPatternV * fPerV);
	m_vVtx[1].tex = D3DXVECTOR2(fPerU + m_nCurPatternU * fPerU , 0.0f  + m_nCurPatternV * fPerV);
	m_vVtx[2].tex = D3DXVECTOR2(0.0f  + m_nCurPatternU * fPerU , fPerV + m_nCurPatternV * fPerV);
	m_vVtx[3].tex = D3DXVECTOR2(fPerU + m_nCurPatternU * fPerU , fPerV + m_nCurPatternV * fPerV);
}


//**�֐�***************************************************************************
//	�T�v	:	���_���Z�b�g
//*********************************************************************************
void CObj2D::SetVtx()
{
	//���_���W
	m_vVtx[0].vtx = m_vVtxBase[0].vtx = D3DXVECTOR4(- m_HitRect.m_fWidth / 2 , - m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtx[1].vtx = m_vVtxBase[1].vtx = D3DXVECTOR4(  m_HitRect.m_fWidth / 2 , - m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtx[2].vtx = m_vVtxBase[2].vtx = D3DXVECTOR4(- m_HitRect.m_fWidth / 2 ,   m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtx[3].vtx = m_vVtxBase[3].vtx = D3DXVECTOR4(  m_HitRect.m_fWidth / 2 ,   m_HitRect.m_fHeight / 2 , 0.0 , 1.0);

	
	m_vVtxNTex[0].vtx = m_vVtxNTexBase[0].vtx = D3DXVECTOR4(- m_HitRect.m_fWidth / 2 , - m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtxNTex[1].vtx = m_vVtxNTexBase[1].vtx = D3DXVECTOR4(  m_HitRect.m_fWidth / 2 , - m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtxNTex[2].vtx = m_vVtxNTexBase[2].vtx = D3DXVECTOR4(- m_HitRect.m_fWidth / 2 ,   m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtxNTex[3].vtx = m_vVtxNTexBase[3].vtx = D3DXVECTOR4(  m_HitRect.m_fWidth / 2 ,   m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
}


//**�֐�***************************************************************************
//	�T�v	:	���_���Z�b�g
//*********************************************************************************
void CObj2D::SetVtx(D3DXVECTOR4* vtx)
{
	//���_���W
	m_vVtx[0].vtx = m_vVtxBase[0].vtx = vtx[0];
	m_vVtx[1].vtx = m_vVtxBase[1].vtx = vtx[1];
	m_vVtx[2].vtx = m_vVtxBase[2].vtx = vtx[2];
	m_vVtx[3].vtx = m_vVtxBase[3].vtx = vtx[3];

	m_vVtxNTex[0].vtx = m_vVtxNTexBase[0].vtx = vtx[0];
	m_vVtxNTex[1].vtx = m_vVtxNTexBase[1].vtx = vtx[1];
	m_vVtxNTex[2].vtx = m_vVtxNTexBase[2].vtx = vtx[2];
	m_vVtxNTex[3].vtx = m_vVtxNTexBase[3].vtx = vtx[3];
}


//**�֐�***************************************************************************
//	�T�v	:	�F�ݒ�
//*********************************************************************************
void CObj2D::SetDiffuse(int r , int g , int b , int a)
{
	//�錾��
	int 	nLoopCnt;

	for(nLoopCnt = 0 ; nLoopCnt < 4 ; nLoopCnt ++)
	{
		m_vVtx[nLoopCnt].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//�F�w��
		m_vVtxNTex[nLoopCnt].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//�F�w��
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�A�j���[�V�������Z�b�g
//*********************************************************************************
void CObj2D::SetAnimParam(int nAnimFrame , int nTexPatternNum , int nUPatternNum , int nVPatternNum)
{
	// �A�j���[�V������񏉊���
	m_nAnimCnt = 0;
	m_nCurPatternU = 0;
	m_nCurPatternV = 0;

	// �A�j���[�V�������Z�b�g
	m_nAnimFrame = nAnimFrame;
	m_nTexPatternNum = nTexPatternNum;
	m_nPatternNumU = nUPatternNum;
	m_nPatternNumV = nVPatternNum;

	// �A�j���[�V�����Ή��e�N�X�`�����ɏ�������
	SetTex();
}


//**�֐�***************************************************************************
//	�T�v	:	�����蔻���`�擾
//*********************************************************************************
CHitRect CObj2D::GetHitRect()
{
	m_HitRect.m_Point = m_Pos;
	return m_HitRect;
}


//**�֐�***************************************************************************
//	�T�v	:	�����蔻���`�Z�b�g
//*********************************************************************************
void CObj2D::SetHitRect(CHitRect rect)
{
	m_HitRect = rect;

	SetVtx();
}


//**�֐�***************************************************************************
//	�T�v	:	���\�[�X�e�N�X�`���Z�b�g
//*********************************************************************************
void CObj2D::SetResTexture(TCHAR* texName)
{
	m_pTexture = MANAGER.GetTexManage()->Entry(texName);
}


//**�֐�***************************************************************************
//	�T�v	:	���\�[�X�e�N�X�`���Z�b�g
//*********************************************************************************
void CObj2D::SetResTexture(LPDIRECT3DTEXTURE9* tex)
{
	m_pTexture = tex;
}

//=================================================================================
//	End of File
//=================================================================================