//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�I�u�W�F�Ǘ����W�X�^�N���X����
//*********************************************************************************


//==include��======================================================================
#include "GameManager.h"
#include "Register.h"
#include <algorithm>
//=================================================================================


//==�萔�E�񋓌^��=================================================================

//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
CRegister*	CRegister::m_pInstance = new CRegister;
//=================================================================================


//==�v���g�^�C�v�錾===============================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CRegister::CRegister():
m_bCurPause(false),
m_bNextPause(false),
m_nBeforeRSType(eRSType::NONE)
{
	m_ObjListArray.clear();
	for(int i = 0; i < eOBJSTAGE:: END; i++)
	{
		CObjList* pObjList = new CObjList();
		m_ObjListArray.push_back( pObjList);
	}

	//�`��p���X�g�̃|�C���^��ێ�
	m_pDrawArray = DrawArray;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CRegister::~CRegister()
{
}


//**�֐�***************************************************************************
//	�T�v	:	�I�u�W�F�N�g���X�g�ɒǉ�
//*********************************************************************************
void CRegister::Entry(CObjBase* pObj,unsigned short nStage)
{
	/*
	m_pObjList.push_back(pObj);
	*/
	// ��Ȃ�i�[���Ȃ�
	if(! pObj)
		return;

	pObj->SetStageID((int)nStage);

	for(int i = 0 ; i < eOBJSTAGE::END + 1 ; i ++)
		if(DrawArray[i] == nStage)	pObj->SetDrawID(i);

	m_ObjListArray[nStage]->push_back(pObj);
}



//**�֐�***************************************************************************
//	�T�v	:	�`��z�񃊃X�g�N���A
//*********************************************************************************
void CRegister::DrawArrayListClear()
{
	/*
	unsigned int ArraySize = m_DrawArrayList.size();
	for(unsigned int i = 0 ; i < ArraySize ; i ++)
		m_DrawArrayList[i].clear();						// �`�惊�X�g���N���A
	*/
}


//**�֐�***************************************************************************
//	�T�v	:	�S�X�V
//*********************************************************************************
void CRegister::UpdateAll()
{
	/*
	// �T���p�C�e���[�^����
	OBJ_LIST_ITE begin = m_pObjList.begin();
	OBJ_LIST_ITE end = m_pObjList.end();
	*/

	// �X�V
	if(m_bCurPause != m_bNextPause)
		m_bCurPause = m_bNextPause;

	for(unsigned int Stage = 0; Stage < m_ObjListArray.size(); Stage++)
	{
		CObjList *pList = m_ObjListArray[Stage];
		CObjList::iterator _end = pList->end();
		for(CObjList::iterator i=pList->begin(); i!=_end; /* i++ */){
			CObjBase *pObj = *i;

			if(!m_bCurPause)
			{
				pObj->Update();
			}
			else
			{
				if(pObj->GetSceneID() == eScene_ID::PAUSE)
					pObj->Update();
			}

			if(pObj->GetDeleteFlg())
			{
				i = pList->erase(i);
				pObj->Release();
				continue;
			}
			i++;  // �����Ŏ��̃C�e���[�g��
		}
	}


	/*
	while( begin != end )
	{
		(*begin)->Update();
		++ begin;
	}
	*/
}


//**�֐�***************************************************************************
//	�T�v	:	�S�X�V
//*********************************************************************************
void CRegister::AffineAll()
{
	/*
	// �T���p�C�e���[�^����
	OBJ_LIST_ITE begin = m_pObjList.begin();
	OBJ_LIST_ITE end = m_pObjList.end();

	while( begin != end )
	{
		(*begin)->Affine();
		++ begin;
	}
	*/

	for(unsigned int Stage = 0; Stage < m_ObjListArray.size(); Stage++)
	{
		CObjList *pList = m_ObjListArray[Stage];
		CObjList::iterator _end = pList->end();
		for(CObjList::iterator i=pList->begin(); i!=_end; /* i++ */){
			CObjBase *pObj = *i;
			pObj->Affine();
			i++;  // �����Ŏ��̃C�e���[�g��
		}
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�S�����蔻�蔻��
//*********************************************************************************
void CRegister::CollisionAll()
{
	/*
	// �T���p�C�e���[�^����
	OBJ_LIST_ITE begin = m_pObjList.begin();
	OBJ_LIST_ITE end = m_pObjList.end();

	while( begin != end )
	{
		(*begin)->CheckCollision();
		++ begin;
	}
	*/

	for(unsigned int Stage = 0; Stage < m_ObjListArray.size(); Stage++)
	{//�Sobject������
		CObjList *pList = m_ObjListArray[Stage];
		CObjList::iterator _end = pList->end();
		for(CObjList::iterator i=pList->begin(); i!=_end; /* i++ */){
			CObjBase *pObj = *i;
			pObj->CheckCollision();
			i++;  // �����Ŏ��̃C�e���[�g��
		}
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�S�`��
//*********************************************************************************
void CRegister::DrawAll()
{
	/*
	OBJ_LIST_ITE DrawBegin;
	OBJ_LIST_ITE DrawEnd;

	unsigned int ArraySize = m_DrawArrayList.size();

	// �`��
	for(unsigned int i = 0 ; i < ArraySize ; i ++)
	{
		DrawBegin = m_DrawArrayList[i].begin();
		DrawEnd = m_DrawArrayList[i].end();

		for( ; DrawBegin != DrawEnd ; DrawBegin ++)
		{
			(*DrawBegin)->Draw();
		}
	}


	// �����x���ݕ`��
	for(unsigned int i = 0 ; i < ArraySize ; i ++)
	{
		DrawBegin = m_DrawArrayList[i].begin();
		DrawEnd = m_DrawArrayList[i].end();

		for( ; DrawBegin != DrawEnd ; DrawBegin ++)
		{
			(*DrawBegin)->DrawLate();
		}
	}
	*/



	std::vector <CObjBase*> vec;

	
	//�S�v�f��vector�ɃR�s�[
	for(unsigned int Stage = 0; Stage < m_ObjListArray.size(); Stage++)
	{//�Sobject������
		CObjList *pList = m_ObjListArray[m_pDrawArray[Stage]];
		CObjList::iterator _end = pList->end();
		for(CObjList::iterator i=pList->begin(); i!=_end; /* i++ */)
		{
			CObjBase *pObj = *i;
			
			if(pObj->GetUseFlg())
				vec.push_back(*i);
			//pObj->Draw();
			i++;  // �����Ŏ��̃C�e���[�g��
		}
	}

	//�v�f��������Ε`�悵�Ȃ�
	if(vec.empty())
		return;

	//vector��Priority���Ƀ\�[�g
	std::sort(vec.begin(),vec.end(),CObjBase::asc);

	std::vector<CObjBase*>::iterator _begin = vec.begin();
	std::vector<CObjBase*>::iterator _end = vec.end();

	//�`�揇���X�g������
	if(m_bCurPause)
	{
		for(; _begin != _end; _begin ++)
		{
			if((*_begin)->GetRS() != m_nBeforeRSType)
			{
				m_nBeforeRSType = (*_begin)->GetRS();
				(*_begin)->SetRS();
			}

			(*_begin)->Draw();
		}
	}
	else
	{
		for(; _begin != _end; _begin ++)
		{
			if((*_begin)->GetSceneID() != eScene_ID::PAUSE)
			{
				if((*_begin)->GetRS() != m_nBeforeRSType)
				{
					m_nBeforeRSType = (*_begin)->GetRS();
					(*_begin)->SetRS();
				}

				(*_begin)->Draw();
			}
		}
	}
	
	_begin = vec.begin();
	if(m_bCurPause)
	{
		for(; _begin != _end; _begin ++)
		{
			if((*_begin)->GetRS() != m_nBeforeRSType)
			{
				m_nBeforeRSType = (*_begin)->GetRS();
				(*_begin)->SetRSLate();
			}

			(*_begin)->DrawLate();
		}
	}
	else
	{
		for(; _begin != _end; _begin ++)
		{
			if((*_begin)->GetSceneID() != eScene_ID::PAUSE)
			{
				if((*_begin)->GetRS() != m_nBeforeRSType)
				{
					m_nBeforeRSType = (*_begin)->GetRS();
					(*_begin)->SetRSLate();
				}

				(*_begin)->DrawLate();
			}
		}
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�w��V�[���ɏ�������I�u�W�F�����
//*********************************************************************************
void CRegister::ReleaseScene(int nSceneID)
{
	/*
	OBJ_LIST_ITE begin = m_pObjList.begin();
	OBJ_LIST_ITE end = m_pObjList.end();
	
	while(begin != end)
	{
		if((*begin)->GetSceneID() == nSceneID)
		{
			(*begin)->Release();
			begin = m_pObjList.erase(begin);
		}
		else
			begin ++;
	}
	*/

	for(unsigned int Stage = 0; Stage < m_ObjListArray.size(); Stage++)
	{//�Sobject������
		CObjList *pList = m_ObjListArray[m_pDrawArray[Stage]];
		CObjList::iterator _end = pList->end();
		for(CObjList::iterator i=pList->begin(); i!=_end; /* i++ */){
			CObjBase *pObj = *i;
			if(pObj->GetSceneID() == nSceneID)
			{
				i = pList->erase(i);
				delete pObj;
				continue;
			}
			i++;  // �����Ŏ��̃C�e���[�g��
		}
	}

	


}


//**�֐�***************************************************************************
//	�T�v	:	�폜�I�u�W�F�����
//********************************************************************************
void CRegister::ReleaseDeleteFlg()
{
	/*
	// �T���p�C�e���[�^����
	OBJ_LIST_ITE begin = m_pObjList.begin();
	OBJ_LIST_ITE end = m_pObjList.end();

	while( begin != end )
	{
		if(!(*begin)->GetDeleteFlg())
		{
			begin ++;
			continue;
		}

		(*begin)->Release();
		begin = m_pObjList.erase(begin);

		end = m_pObjList.end();
	}
	begin = m_pObjList.begin();
	*/
}



//**�֐�***************************************************************************
//	�T�v	:	�S���
//*********************************************************************************
void CRegister::ReleaseAll()
{
	// �T���p�C�e���[�^����
	/*
	OBJ_LIST_ITE begin = m_pObjList.begin();
	OBJ_LIST_ITE end = m_pObjList.end();

	while( begin != end )
	{
		(*begin)->Release();
		begin = m_pObjList.erase(begin);
	}
	begin = m_pObjList.begin();
	*/

	for(unsigned int Stage = 0; Stage < m_ObjListArray.size(); Stage++)
	{
		//�Sobject������
		CObjList *pList = m_ObjListArray[Stage];		
		CObjList::iterator _end = pList->end();

		for(CObjList::iterator i=pList->begin(); i!=_end; /* i++ */)
		{
			CObjBase *pObj = *i;
			i = pList->erase(i);
				delete pObj;
				continue;
		}
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�S��� + ���g�̔j��
//*********************************************************************************
void CRegister::Cleanup()
{
	ReleaseAll();

	delete this;
}

//=================================================================================
//	End of File
//=================================================================================