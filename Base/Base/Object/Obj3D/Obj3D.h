//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	3D�I�u�W�F�N�g���b�V��
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../Base/ObjBase.h"
#include "../../Resource/Mesh.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================

//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CObj3D : public CObjBase
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CObj3D();
	virtual ~CObj3D();

	static CObj3D*	Create(LPCTSTR meshName , D3DXVECTOR3 pos , 
		 int nSceneID , int nPriority);								// ����

	static CObj3D*	Create(LPCTSTR meshName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nSceneID
		 , int nPriority);											// ����

	virtual bool	Init(LPCTSTR meshName , D3DXVECTOR3 pos , 
		 int nSceneID , int nPriority);

	virtual bool	Init(LPCTSTR meshName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nSceneID
		 , int nPriority);


	virtual void	SetRS();
	virtual	void	SetRSLate();
	virtual void	Draw();											// �`��
	virtual void	DrawLate();										// �����x�t���`��

	virtual void	Release();										// ���

	// --�Q�b�g�Z�b�g--
	CMesh*	GetMesh(){return m_pMesh;}								// ���b�V���f�[�^�擾
	void	SetMesh(CMesh* pMesh){m_pMesh = pMesh;}					// ���b�V���f�[�^�Z�b�g

protected:
	CMesh*			m_pMesh;										// 3D�I�u�W�F�N�g�|�C���^
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================