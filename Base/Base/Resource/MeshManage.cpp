//**プログラムヘッダ***************************************************************
//	プログラム概要	:	メッシュ管理クラス実装
//*********************************************************************************


//==include部======================================================================
#include "../System/GameManager.h"
#include "../System/common.h"
#include "MeshManage.h"
//=================================================================================


//==定数・列挙型部=================================================================

//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================

//=================================================================================


//==静的メンバ変数部===============================================================
MESH_VECT		CMeshManage::m_MeshVect(0);
MESHNAME_VECT	CMeshManage::m_MeshNameVect(0);
//=================================================================================


//==プロトタイプ宣言===============================================================

//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CMeshManage::CMeshManage()
{

}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CMeshManage::~CMeshManage()
{

}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CMeshManage* CMeshManage::Create()
{
	CMeshManage*	pManage = new CMeshManage;

	if(! pManage->Initialize())
	{
		::MessageBox(NULL , _T("MeshManage生成エラー") , _T("error") , MB_ICONERROR | MB_OK);
		return NULL;
	}

	return pManage;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CMeshManage::Initialize()
{
	return true;
}


//**関数***************************************************************************
//	概要	:	配列内検索
//			:	指定した名前のメッシュへのポインタを返す
//*********************************************************************************
CMesh* CMeshManage::Search(LPCTSTR MeshName)
{
	for(UINT i = 0 ; i < m_MeshNameVect.size() ; i ++)
	{
		if(_tcscmp(m_MeshNameVect[i] , MeshName) != 0)
			continue;
			
		// 一致したらそのメッシュへのポインタを返す
		return (CMesh*)(m_MeshVect[i]);
	}

	// 見つからなかったのでNULLを返す
	return NULL;
}

//**関数***************************************************************************
//	概要	:	配列へ登録
//			:	指定したメッシュへのポインタを返す
//			:	ロード失敗したときはNULLを返す
//*********************************************************************************
CMesh* CMeshManage::Entry(LPCTSTR MeshName)
{
	CMesh*	pMesh = NULL;
	
	// すでに指定したファイルがあるかを検索
	pMesh = Search(MeshName);

	// あればそれを返す
	if(pMesh)
		return pMesh;


	// なかった場合は新規でロード
	pMesh = new CMesh;
	if(!pMesh->Initialize(MeshName))
	{
		TCHAR dbg[256];
		_stprintf_s(dbg , _T("Xファイル読み込みエラー%s\n") , MeshName);
		::MessageBox(NULL , dbg , _T("error") , MB_ICONERROR | MB_OK);
		return false;
	}
	pMesh->InitParticle();
	
	// メッシュとメッシュ名を格納
	m_MeshVect.push_back(pMesh);
	m_MeshNameVect.push_back(MeshName);

	return pMesh;
}


//**関数***************************************************************************
//	概要	:	配列を完全クリア
//*********************************************************************************
void CMeshManage::VectorClear()
{
	// リスト全解放
	for(UINT i = 0 ; i < m_MeshVect.size() ; i ++)
	{	
		m_MeshVect[i]->Finalize();
		m_MeshNameVect[i] = NULL;
	}

	m_MeshVect.clear();
	m_MeshNameVect.clear();
}


//**関数***************************************************************************
//	概要	:	マネージャ解放
//*********************************************************************************
void CMeshManage::Release()
{
	VectorClear();

	delete this;
}


//**関数***************************************************************************
//	概要	:	個別解放(ファイル名指定)
//			:	解放失敗時はfalseを返す
//*********************************************************************************
bool CMeshManage::ClearOnce(LPCTSTR MeshName)
{
	MESHNAME_VECT_ITE iteName = m_MeshNameVect.begin();
	MESH_VECT_ITE iteMesh = m_MeshVect.begin();

	for(UINT i = 0 ; i < m_MeshNameVect.size() ; i ++ , iteName ++ , iteMesh ++)
	{
		if(_tcscmp(m_MeshNameVect[i] , MeshName) != 0)
			continue;
			
		// 一致したら削除
		m_MeshVect[i]->Finalize();
		m_MeshNameVect[i] = NULL;
		m_MeshNameVect.erase(iteName);
		m_MeshVect.erase(iteMesh);

		return true;
	}

	return false;
}


//=================================================================================
//	End of File
//=================================================================================