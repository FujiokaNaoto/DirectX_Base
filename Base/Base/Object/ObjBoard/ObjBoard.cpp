//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	���[���h��|���S���x�[�X
//*********************************************************************************


//==include��======================================================================
#include "../../System/GameManager.h"

#include "ObjBoard.h"
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
CObjBoard::CObjBoard():
m_pTexture(NULL) , 
m_nWidth(0),
m_nHeight(0),
m_bBillFlg(true),

m_nAnimCnt(0),
m_nAnimFrame(1),
m_nTexPatternNum(1),
m_nPatternNumU(1),
m_nPatternNumV(1),
m_nCurPatternU(0),
m_nCurPatternV(0)
{
	m_nClassID = eClassCode::OBJ_BOARD;
	m_nRSType = m_nRSTypeLate = eRSType::BILLBOARD;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CObjBoard::~CObjBoard()
{

}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObjBoard* CObjBoard::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority)
{
	CObjBoard*		pObj = new CObjBoard;

	if(pObj->Init(texName , pos , rot , scall , nWidth , nHeight ,
		nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObjBoard* CObjBoard::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
	 int nWidth , int nHeight , int nSceneID , int nPriority)
{
	CObjBoard*		pObj = new CObjBoard;

	if(pObj->Init(texName , pos , nWidth , nHeight , nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CObjBoard::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority)
{
	return Init(texName , pos , DEFAULT_ROT , DEFAULT_SCALL , nWidth , nHeight ,
		nSceneID , nPriority);
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CObjBoard::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
	D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
	int nHeight , int nSceneID , int nPriority)
{
	m_pTexture = MANAGER.GetTexManage()->Entry(texName);

	if(! m_pTexture)	return false;			// �o�^���s����NULL��Ԃ�
	m_Pos = pos;
	m_Rot = rot;
	m_Scall = scall;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_nSceneID = nSceneID;
	m_nPriority = nPriority;

	// --������p���a--
	if(m_nWidth > m_nHeight)
		m_fRadius = nWidth / 2.0f;
	else
		m_fRadius = nHeight / 2.0f;

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
void CObjBoard::Release()
{
	Uninit();
	delete this;
}


//**�֐�***************************************************************************
//	�T�v	:	�A�t�B���ϊ�
//*********************************************************************************
void CObjBoard::Affine()
{
	D3DXMATRIX	matTrans , matRot , matScall , matBill;

	// ������
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScall);		

	// �r���{�[�h������
	if(m_bBillFlg)
	{
		D3DXMatrixIdentity(&matBill);

		// �J���������擾
		CCamera* pCamera = MANAGER.GetCamera();
		D3DXMATRIX	matCamera;
		D3DXMatrixIdentity(&matCamera);
		D3DXMatrixLookAtLH(&matCamera , &pCamera->GetPos() , &pCamera->GetLook() , &pCamera->GetUp());

		// �]�u�s������
		matBill._11 = matCamera._11;
		matBill._12 = matCamera._21;
		matBill._13 = matCamera._31;

		matBill._21 = matCamera._12;
		matBill._22 = matCamera._22;
		matBill._23 = matCamera._32;

		matBill._31 = matCamera._13;
		matBill._32 = matCamera._23;
		matBill._33 = matCamera._33;

		// �ړ���������
		matBill._41 = 0.0f;
		matBill._42 = 0.0f;
		matBill._43 = 0.0f;

		// �g��
		D3DXMatrixScaling(&matScall , m_Scall.x , m_Scall.y , m_Scall.z);
		// �ړ�
		D3DXMatrixTranslation(&matTrans , m_Pos.x , m_Pos.y , m_Pos.z);

		// ���[���h�}�g���N�X�ɃZ�b�g
		m_Matrix = matScall * matBill * matTrans;
	}

	// �r���{�[�h�����Ȃ�
	else
	{
		// ��]
		D3DXMatrixRotationYawPitchRoll(&matRot , 
			m_Rot.y , m_Rot.x , m_Rot.z);
		// �g��
		D3DXMatrixScaling(&matScall , m_Scall.x , m_Scall.y , m_Scall.z);
		// �ړ�
		D3DXMatrixTranslation(&matTrans , m_Pos.x , m_Pos.y , m_Pos.z);

		// ���[���h�}�g���N�X�ɃZ�b�g
		m_Matrix = matScall * matRot * matTrans;
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�����_�[�X�e�[�g�ݒ�
//*********************************************************************************
void CObjBoard::SetRS()
{
	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	pDevice->SetFVF(FVF_VERTEX_BOARD);

	pDevice->SetRenderState(D3DRS_ZENABLE , TRUE);						// Z�o�b�t�@�I��
	pDevice->SetRenderState(D3DRS_LIGHTING , FALSE);					// ���C�g�I�t
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// �A���t�@�u�����h����
	
	// �����p�^�[��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	�����_�[�X�e�[�g�ݒ�
//*********************************************************************************
void CObjBoard::SetRSLate()
{
	CGraphics::SemafoLock();

	// �O���t�B�b�N�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	// ���_�X�g���[���ݒ�
	pDevice->SetFVF(FVF_VERTEX_BOARD);

	// �����_�[�X�e�[�g�ݒ�
	pDevice->SetRenderState(D3DRS_ZENABLE , TRUE);						// Z�o�b�t�@�I��
	pDevice->SetRenderState(D3DRS_LIGHTING , FALSE);					// ���C�g�I�t
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// �A���t�@�u�����h����
	
	// �����p�^�[��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	//pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	�`��
//*********************************************************************************
void CObjBoard::Draw()
{
	// �`��t���O���I�t�܂��͌�`��t���O���I���Ȃ�`�悵�Ȃ�
	if(!m_bUseFlg || m_bDrawLate) return;

	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);

	if(m_pTexture)
	{
		pDevice->SetFVF(FVF_VERTEX_BOARD);
		pDevice->SetTexture(0 , *m_pTexture);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx , sizeof(VERTEX_BOARD));
	}
	else
	{
		pDevice->SetFVF(FVF_VERTEX_BOARD_TEXNONE);
		pDevice->SetTexture(0 , *m_pTexture);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx_NTex , sizeof(VERTEX_BOARD_TEXNONE));
	}


	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	��`��
//*********************************************************************************
void CObjBoard::DrawLate()
{
	// �`��t���O���I�t�܂��͌�`��t���O���I�t�Ȃ�`�悵�Ȃ�
	if(!m_bUseFlg || ! m_bDrawLate) return;		

	CGraphics::SemafoLock();

	// �O���t�B�b�N�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	pDevice->SetTexture(0 , *m_pTexture);								// �e�N�X�`���Z�b�g
	pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);						// �A�t�B��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx , sizeof(VERTEX_BOARD));

	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	�e�N�X�`���Z�b�g
//*********************************************************************************
void CObjBoard::SetTex()
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
void CObjBoard::SetVtx()
{
	//���_���W
	m_vVtx[0].vtx = D3DXVECTOR3(- (float)m_nWidth / 2 ,   (float)m_nHeight / 2 , 0.0);
	m_vVtx[1].vtx = D3DXVECTOR3(  (float)m_nWidth / 2 ,   (float)m_nHeight / 2 , 0.0);
	m_vVtx[2].vtx = D3DXVECTOR3(- (float)m_nWidth / 2 , - (float)m_nHeight / 2 , 0.0);
	m_vVtx[3].vtx = D3DXVECTOR3(  (float)m_nWidth / 2 , - (float)m_nHeight / 2 , 0.0);

	m_vVtx_NTex[0].vtx = D3DXVECTOR3(- (float)m_nWidth / 2 ,   (float)m_nHeight / 2 , 0.0);
	m_vVtx_NTex[1].vtx = D3DXVECTOR3(  (float)m_nWidth / 2 ,   (float)m_nHeight / 2 , 0.0);
	m_vVtx_NTex[2].vtx = D3DXVECTOR3(- (float)m_nWidth / 2 , - (float)m_nHeight / 2 , 0.0);
	m_vVtx_NTex[3].vtx = D3DXVECTOR3(  (float)m_nWidth / 2 , - (float)m_nHeight / 2 , 0.0);
}


//**�֐�***************************************************************************
//	�T�v	:	���_���Z�b�g
//*********************************************************************************
void CObjBoard::SetVtx(D3DXVECTOR3* vtx)
{
	//���_���W
	m_vVtx[0].vtx = vtx[0];
	m_vVtx[1].vtx = vtx[1];
	m_vVtx[2].vtx = vtx[2];
	m_vVtx[3].vtx = vtx[3];

	m_vVtx_NTex[0].vtx = vtx[0];
	m_vVtx_NTex[1].vtx = vtx[1];
	m_vVtx_NTex[2].vtx = vtx[2];
	m_vVtx_NTex[3].vtx = vtx[3];
}


//**�֐�***************************************************************************
//	�T�v	:	�F�ݒ�
//*********************************************************************************
void CObjBoard::SetDiffuse(int r , int g , int b , int a)
{
	//�錾��
	int 	nLoopCnt;

	for(nLoopCnt = 0 ; nLoopCnt < 4 ; nLoopCnt ++)
	{
		m_vVtx[nLoopCnt].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//�F�w��
		m_vVtx_NTex[nLoopCnt].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//�F�w��
	}

}


//**�֐�***************************************************************************
//	�T�v	:	�A�j���[�V�������Z�b�g
//*********************************************************************************
void CObjBoard::SetAnimParam(int nAnimFrame , int nTexPatternNum , 
		int nUPatternNum , int nVPatternNum)
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
//	�T�v	:	�A�j���[�V�����X�V
//*********************************************************************************
void CObjBoard::AnimUpdate()
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
//=================================================================================
//	End of File
//=================================================================================