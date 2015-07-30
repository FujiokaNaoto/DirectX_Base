//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	3D�I�u�W�F�N�g�Ǘ��N���X
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../System/common.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "Mesh.h"

#include <vector>
#include <tchar.h>
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================

//=================================================================================


//==�}�N����`��===================================================================
#define MESH_VECT			std::vector<CMesh*>							// ���b�V���z��
#define MESH_VECT_ITE		std::vector<CMesh*>::iterator				// ���b�V���z��ւ̃C�e���[�^

#define MESHNAME_VECT		std::vector<LPCTSTR>							// ���b�V�����z��
#define MESHNAME_VECT_ITE	std::vector<LPCTSTR>::iterator				// ���b�V�����z��ւ̃C�e���[�^
//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CMeshManage
{
private:
	static MESH_VECT		m_MeshVect;									// ���[�h���ꂽ���b�V����ێ����郊�X�g
	static MESHNAME_VECT	m_MeshNameVect;								// ���b�V���̖��O���i�[���Ă������X�g

public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CMeshManage();
	virtual ~CMeshManage();

	static CMeshManage*	Create();										// ����
	bool	Initialize();												// ������

	CMesh*	Search(LPCTSTR MeshName);									// ���łɃ��b�V�����i�[����Ă��邩������
	CMesh*	Entry(LPCTSTR MeshName);										// ���b�V�������X�g���ɓo�^
	void	VectorClear();												// �i�[�z������S�N���A
	bool	ClearOnce(LPCTSTR MeshName);									// �w�胊�\�[�X�̉��

	void	Release();													// �}�l�[�W�����
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================