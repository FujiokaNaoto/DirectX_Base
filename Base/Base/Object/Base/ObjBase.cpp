//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�I�u�W�F�N�g�N���X�x�[�X
//*********************************************************************************


//==include��======================================================================
#include "../../System/GameManager.h"
#include "../../System/Register.h"
#include "ObjBase.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================

//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
UINT CObjBase::m_nUniqeID = 0;
//=================================================================================


//==�v���g�^�C�v�錾===============================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CObjBase::CObjBase():
m_Pos (D3DXVECTOR3(0.0f , 0.0f , 0.0f)) ,
m_Rot (DEFAULT_ROT) , 
m_Scall (DEFAULT_SCALL),

m_Spd (DEFAULT_SPD),
m_Accel (DEFAULT_ACCEL),

m_fRadius(0.0f),

m_bUseFlg (true),
m_bDrawLate(false),
m_bDeleteFlg(false),

m_nClassID (ID_NONE),
m_nSceneID(eScene_ID::NONE),
m_nPriority (0),
m_nParent(NULL),

m_nRSType(eRSType::NONE),
m_nRSTypeLate(eRSType::NONE),

m_nStatus(-1)
{
	m_nHandle = REGISTER->handle.CreateHandle(this);

	D3DXMatrixIdentity(&m_Matrix);				// �s�񏉊���
	m_nObjID = m_nUniqeID;						// ���j�[�NID���I�u�W�F�N�g�ɓo�^
	m_nUniqeID ++;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CObjBase::~CObjBase()
{
	REGISTER->handle.ReleaseHandle(m_nHandle);
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObjBase* CObjBase::Create(int nSceneID ,int nPriority)
{
	CObjBase*	pObj = new CObjBase();

	if(pObj->Init(nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObjBase* CObjBase::Create(D3DXVECTOR3 pos , int nSceneID , int nPriority)
{
	CObjBase*	pObj = new CObjBase();

	if(pObj->Init(pos , nSceneID , nPriority))
		return pObj;
	
	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CObjBase::Init(int nSceneID ,int nPriority)
{
	m_Pos = DEFAULT_POS;								// �ʒu
	m_Rot = DEFAULT_ROT;								// ��]�p
	m_Scall = DEFAULT_SCALL;							// �g�嗦

	m_nSceneID = nSceneID;								// �V�[��ID
	m_nPriority = nPriority;							// �v���C�I���e�B

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CObjBase::Init(D3DXVECTOR3 pos , int nSceneID , int nPriority)
{
	m_Pos = pos;								// �ʒu
	m_Rot = DEFAULT_ROT;								// ��]�p
	m_Scall = DEFAULT_SCALL;							// �g�嗦

	m_nSceneID = nSceneID;					// �V�[��ID
	m_nPriority = nPriority;					// �v���C�I���e�B

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void CObjBase::Update()
{
	if(!m_bUseFlg)				// �`�悵�Ȃ��Ƃ��͏����Ȃ�
		return;

	m_Spd += m_Accel;
	m_Pos += m_Spd;
}


//**�֐�***************************************************************************
//	�T�v	:	�A�t�B��
//*********************************************************************************
void CObjBase::Affine()
{
	if(!m_bUseFlg)				// �`�悵�Ȃ��Ƃ��͏����Ȃ�
		return;

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
}


//**�֐�***************************************************************************
//	�T�v	:	���
//*********************************************************************************
void CObjBase::Release()
{
	delete this;
}


bool CObjBase::asc( const CObjBase* left, const CObjBase* right ) 
{
	//�v���C�I���e�B�[�l + �`��X�e�[�Wx1000 ���\�[�g�l
		return left->m_nPriority + left->m_nDrawID * STGAE_ENTRY_MAX < right->m_nPriority + right->m_nDrawID * STGAE_ENTRY_MAX;
}

//=================================================================================
//	End of File
//=================================================================================