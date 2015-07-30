//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�I�u�W�F�Ǘ����W�X�^�N���X��`
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "common.h"
#include "../Object/Base/ObjBase.h"
#include <list>
#include <vector>
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
#define STGAE_ENTRY_MAX	(1000)

//�X�V���X�g
namespace eOBJSTAGE
{
	enum OBJLISTSTAGE
	{
		SYS,		//�V�X�e���E�I�v�V����
		BG,			// �w�i
		OBJ,		// �I�u�W�F�N�g
		EFFECT,		// �G�t�F�N�g
		UI,			// UI
		MESSAGE,	// ���b�Z�[�W
		PAUSE,		// �|�[�Y�I�u�W�F
		FADE,		// �t�F�[�h

		END,	//�I�[
	};
};

typedef std::list<CObjBase*> CObjList;
typedef std::vector<CObjList*> CObjListArray;

//�`�惊�X�g
static int	DrawArray[eOBJSTAGE::END+1] = 
{
	eOBJSTAGE::SYS,
	eOBJSTAGE::BG,
	eOBJSTAGE::OBJ,
	eOBJSTAGE::EFFECT,
	eOBJSTAGE::UI,
	eOBJSTAGE::MESSAGE,
	eOBJSTAGE::PAUSE,
	eOBJSTAGE::FADE,
	eOBJSTAGE::END,
};


//==�N���X�E�\���̐錾��===========================================================
class CObjBase;
// ��r���@
// �v���C�I���e�B���\�[�g
class CPriorityPredicate
{
public:
	bool operator() (CObjBase* obj1 , CObjBase* obj2)
	{	
		return  obj1->GetPriority() < obj2->GetPriority();
	}
};


class CRegister
{
private:
	static CRegister* m_pInstance;			// �C���X�^���X
	std::list<CObjBase*> m_pObjList;		// �I�u�W�F���X�g

	CObjListArray	m_ObjListArray;			//�I�u�W�F�N�g���X�g�̔z��
	int*			m_pDrawArray;			//�`��p���X�g�f�[�^�̃|�C���^
	
	std::vector<std::list<CObjBase*>> m_DrawArrayList;		// �`��z��ƃ��X�g

	bool			m_bCurPause;			// �|�[�Y�t���O
	bool			m_bNextPause;			// �|�[�Y�t���O

	UINT			m_nBeforeRSType;		// �����_�[�X�e�[�g�^�C�v

public:

	C_HANDLE handle;						// �n���h���V�X�e��

	// �R���X�g���N�^�E�f�X�g���N�^
	CRegister();
	~CRegister();

	void Entry(CObjBase* pObj,unsigned short nStage);				// ���X�g�ɒǉ�

	void DrawArrayListClear();
	
	void UpdateAll();						// ���X�g���X�V
	void AffineAll();						// ���X�g���A�t�B���ϊ�
	void CollisionAll();					// ���X�g�������蔻��

	void DrawAll();							// ���X�g���`��
	
	void ReleaseDeleteFlg();				// �폜�I�u�W�F�����
	void ReleaseScene(int nSceneID);		// �w��V�[���ɏ�������I�u�W�F�̂݉��
	void ReleaseAll();						// ���X�g�����

	void Cleanup();							// �j��

	void SetPause(bool bFlg){m_bNextPause = bFlg;}

	// �Q�b�g�Z�b�g
	static CRegister*	GetInstance(){return m_pInstance;}
	
	std::list<CObjBase*>*	GetObjList(unsigned int nListNo)
	{
		if(nListNo > eOBJSTAGE::END)
			return NULL;
		return m_ObjListArray[nListNo];
	}

	bool GetPauseFlg(){return m_bCurPause;}
	std::vector<std::list<CObjBase*>>&	GetDrawList(){return m_DrawArrayList;}
};
//=================================================================================


//==�}�N����`��===================================================================
#define REGISTER			((CRegister*)CRegister::GetInstance())
#define REGISTER_OBJ(a)		(CRegister::GetInstance()->GetObjList(a))
#define REGISTER_H_P(a,type)	((type)(CRegister::GetInstance()->handle.HandleToPointer(a)))

#define OBJ_LIST		std::list<CObjBase*>
#define OBJ_LIST_ITE	std::list<CObjBase*>::iterator
//=================================================================================



//=================================================================================
//	End of File
//=================================================================================