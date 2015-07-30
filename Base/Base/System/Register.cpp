//**プログラムヘッダ***************************************************************
//	プログラム概要	:	オブジェ管理レジスタクラス実装
//*********************************************************************************


//==include部======================================================================
#include "GameManager.h"
#include "Register.h"
#include <algorithm>
//=================================================================================


//==定数・列挙型部=================================================================

//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================

//=================================================================================


//==静的メンバ変数部===============================================================
CRegister*	CRegister::m_pInstance = new CRegister;
//=================================================================================


//==プロトタイプ宣言===============================================================

//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
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

	//描画用リストのポインタを保持
	m_pDrawArray = DrawArray;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CRegister::~CRegister()
{
}


//**関数***************************************************************************
//	概要	:	オブジェクトリストに追加
//*********************************************************************************
void CRegister::Entry(CObjBase* pObj,unsigned short nStage)
{
	/*
	m_pObjList.push_back(pObj);
	*/
	// 空なら格納しない
	if(! pObj)
		return;

	pObj->SetStageID((int)nStage);

	for(int i = 0 ; i < eOBJSTAGE::END + 1 ; i ++)
		if(DrawArray[i] == nStage)	pObj->SetDrawID(i);

	m_ObjListArray[nStage]->push_back(pObj);
}



//**関数***************************************************************************
//	概要	:	描画配列リストクリア
//*********************************************************************************
void CRegister::DrawArrayListClear()
{
	/*
	unsigned int ArraySize = m_DrawArrayList.size();
	for(unsigned int i = 0 ; i < ArraySize ; i ++)
		m_DrawArrayList[i].clear();						// 描画リストをクリア
	*/
}


//**関数***************************************************************************
//	概要	:	全更新
//*********************************************************************************
void CRegister::UpdateAll()
{
	/*
	// 探査用イテレータ準備
	OBJ_LIST_ITE begin = m_pObjList.begin();
	OBJ_LIST_ITE end = m_pObjList.end();
	*/

	// 更新
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
			i++;  // ここで次のイテレートに
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


//**関数***************************************************************************
//	概要	:	全更新
//*********************************************************************************
void CRegister::AffineAll()
{
	/*
	// 探査用イテレータ準備
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
			i++;  // ここで次のイテレートに
		}
	}
}


//**関数***************************************************************************
//	概要	:	全あたり判定判定
//*********************************************************************************
void CRegister::CollisionAll()
{
	/*
	// 探査用イテレータ準備
	OBJ_LIST_ITE begin = m_pObjList.begin();
	OBJ_LIST_ITE end = m_pObjList.end();

	while( begin != end )
	{
		(*begin)->CheckCollision();
		++ begin;
	}
	*/

	for(unsigned int Stage = 0; Stage < m_ObjListArray.size(); Stage++)
	{//全objectを周る
		CObjList *pList = m_ObjListArray[Stage];
		CObjList::iterator _end = pList->end();
		for(CObjList::iterator i=pList->begin(); i!=_end; /* i++ */){
			CObjBase *pObj = *i;
			pObj->CheckCollision();
			i++;  // ここで次のイテレートに
		}
	}
}


//**関数***************************************************************************
//	概要	:	全描画
//*********************************************************************************
void CRegister::DrawAll()
{
	/*
	OBJ_LIST_ITE DrawBegin;
	OBJ_LIST_ITE DrawEnd;

	unsigned int ArraySize = m_DrawArrayList.size();

	// 描画
	for(unsigned int i = 0 ; i < ArraySize ; i ++)
	{
		DrawBegin = m_DrawArrayList[i].begin();
		DrawEnd = m_DrawArrayList[i].end();

		for( ; DrawBegin != DrawEnd ; DrawBegin ++)
		{
			(*DrawBegin)->Draw();
		}
	}


	// 透明度込み描画
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

	
	//全要素をvectorにコピー
	for(unsigned int Stage = 0; Stage < m_ObjListArray.size(); Stage++)
	{//全objectを周る
		CObjList *pList = m_ObjListArray[m_pDrawArray[Stage]];
		CObjList::iterator _end = pList->end();
		for(CObjList::iterator i=pList->begin(); i!=_end; /* i++ */)
		{
			CObjBase *pObj = *i;
			
			if(pObj->GetUseFlg())
				vec.push_back(*i);
			//pObj->Draw();
			i++;  // ここで次のイテレートに
		}
	}

	//要素が無ければ描画しない
	if(vec.empty())
		return;

	//vectorをPriority順にソート
	std::sort(vec.begin(),vec.end(),CObjBase::asc);

	std::vector<CObjBase*>::iterator _begin = vec.begin();
	std::vector<CObjBase*>::iterator _end = vec.end();

	//描画順リストを巡回
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


//**関数***************************************************************************
//	概要	:	指定シーンに所属するオブジェを解放
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
	{//全objectを周る
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
			i++;  // ここで次のイテレートに
		}
	}

	


}


//**関数***************************************************************************
//	概要	:	削除オブジェを解放
//********************************************************************************
void CRegister::ReleaseDeleteFlg()
{
	/*
	// 探査用イテレータ準備
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



//**関数***************************************************************************
//	概要	:	全解放
//*********************************************************************************
void CRegister::ReleaseAll()
{
	// 探査用イテレータ準備
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
		//全objectを周る
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


//**関数***************************************************************************
//	概要	:	全解放 + 自身の破棄
//*********************************************************************************
void CRegister::Cleanup()
{
	ReleaseAll();

	delete this;
}

//=================================================================================
//	End of File
//=================================================================================