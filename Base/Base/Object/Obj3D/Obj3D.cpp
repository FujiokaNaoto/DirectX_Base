//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	3D�I�u�W�F�N�g�x�[�X
//*********************************************************************************


//==include��======================================================================
#include "../../System/GameManager.h"

#include "Obj3D.h"
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
CObj3D::CObj3D():
m_pMesh(NULL)
{
	m_nClassID = eClassCode::OBJ_3D;
	m_nRSType = eRSType::OBJ_3D;
	m_nRSType = eRSType::OBJ_3D_LATE;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CObj3D::~CObj3D()
{
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObj3D* CObj3D::Create(LPCTSTR meshName , D3DXVECTOR3 pos , 
	 int nSceneID , int nPriority)
{
	CObj3D*	pObj = new CObj3D;

	if(pObj->Init(meshName , pos , nSceneID , nPriority))
		return pObj;

	delete pObj;
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObj3D* CObj3D::Create(LPCTSTR meshName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nSceneID , int nPriority)
{
	CObj3D*	pObj = new CObj3D;

	if(pObj->Init(meshName , pos , rot , scall , nSceneID , nPriority))
		return pObj;

	delete pObj;
	return pObj;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CObj3D::Init(LPCTSTR meshName , D3DXVECTOR3 pos , 
		 int nSceneID , int nPriority)
{
	return Init(meshName , pos , DEFAULT_ROT , DEFAULT_SCALL , nSceneID , nPriority);
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CObj3D::Init(LPCTSTR meshName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nSceneID
		 , int nPriority)
{
	// --�����l�i�[--
	m_pMesh = MANAGER.GetMeshManage()->Entry(meshName);	// ���b�V�����[�h�Ɠo�^
	if(! m_pMesh)	return false;			// �o�^���s����NULL��Ԃ�

	m_Pos = pos;
	m_Rot = rot;
	m_Scall = scall;
	m_nSceneID = nSceneID;
	m_nPriority = nPriority;				// �v���C�I���e�B

	Affine();

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	���
//*********************************************************************************
void CObj3D::Release()
{
	Uninit();
	delete this;
}


//**�֐�***************************************************************************
//	�T�v	:	�����_�[�X�e�[�g�ݒ�
//*********************************************************************************
void CObj3D::SetRS()
{
	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();
	// �`��ݒ�
	pDevice->SetRenderState(D3DRS_ZENABLE , TRUE);						// Z�o�b�t�@�I��
	pDevice->SetRenderState(D3DRS_LIGHTING , TRUE);						// ���C�g�I��

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	��`��p�����_�[�X�e�[�g�ݒ�
//*********************************************************************************
void CObj3D::SetRSLate()
{
	CGraphics::SemafoLock();

	// �O���t�B�b�N�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	// �`��ݒ�
	pDevice->SetRenderState(D3DRS_ZENABLE , TRUE);						// Z�o�b�t�@�I��
	pDevice->SetRenderState(D3DRS_LIGHTING , TRUE);						// ���C�g�I��
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// �A���t�@�u�����h����

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	��`��
//			:	3D�ɂ����Ă̓t���O�̔�����s��Ȃ��œ����ƕs���������`�悷��
//			:	�s�����`��
//*********************************************************************************
void CObj3D::Draw()
{
	// �`��t���O���I�t�����b�V�����i�[����Ă��Ȃ��Ȃ�`�悵�Ȃ�
	if(!m_bUseFlg || !m_pMesh) return;			

	// �O���t�B�b�N�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
	m_pMesh->DrawNoAlpha(m_Matrix);
}


//**�֐�***************************************************************************
//	�T�v	:	��`��
//			:	3D�ɂ����Ă̓t���O�̔�����s��Ȃ��œ����ƕs���������`�悷��
//			:	�������`��
//*********************************************************************************
void CObj3D::DrawLate()
{
	// �`��t���O���I�t�����b�V�����i�[����Ă��Ȃ��Ȃ�`�悵�Ȃ�
	if(!m_bUseFlg || !m_pMesh) return;		

	// �O���t�B�b�N�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
	m_pMesh->DrawAlpha(m_Matrix);
}
//=================================================================================
//	End of File
//=================================================================================