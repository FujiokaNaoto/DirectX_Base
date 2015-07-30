//プログラムヘッダ*****************************************************************
//	プログラム概要	:	オブジェ管理レジスタクラス定義
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "common.h"
#include "../Object/Base/ObjBase.h"
#include <list>
#include <vector>
//=================================================================================


//==定数・列挙型宣言部=============================================================
#define STGAE_ENTRY_MAX	(1000)

//更新リスト
namespace eOBJSTAGE
{
	enum OBJLISTSTAGE
	{
		SYS,		//システム・オプション
		BG,			// 背景
		OBJ,		// オブジェクト
		EFFECT,		// エフェクト
		UI,			// UI
		MESSAGE,	// メッセージ
		PAUSE,		// ポーズオブジェ
		FADE,		// フェード

		END,	//終端
	};
};

typedef std::list<CObjBase*> CObjList;
typedef std::vector<CObjList*> CObjListArray;

//描画リスト
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


//==クラス・構造体宣言部===========================================================
class CObjBase;
// 比較方法
// プライオリティ順ソート
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
	static CRegister* m_pInstance;			// インスタンス
	std::list<CObjBase*> m_pObjList;		// オブジェリスト

	CObjListArray	m_ObjListArray;			//オブジェクトリストの配列
	int*			m_pDrawArray;			//描画用リストデータのポインタ
	
	std::vector<std::list<CObjBase*>> m_DrawArrayList;		// 描画配列とリスト

	bool			m_bCurPause;			// ポーズフラグ
	bool			m_bNextPause;			// ポーズフラグ

	UINT			m_nBeforeRSType;		// レンダーステートタイプ

public:

	C_HANDLE handle;						// ハンドルシステム

	// コンストラクタ・デストラクタ
	CRegister();
	~CRegister();

	void Entry(CObjBase* pObj,unsigned short nStage);				// リストに追加

	void DrawArrayListClear();
	
	void UpdateAll();						// リスト内更新
	void AffineAll();						// リスト内アフィン変換
	void CollisionAll();					// リスト内あたり判定

	void DrawAll();							// リスト内描画
	
	void ReleaseDeleteFlg();				// 削除オブジェを解放
	void ReleaseScene(int nSceneID);		// 指定シーンに所属するオブジェのみ解放
	void ReleaseAll();						// リスト内解放

	void Cleanup();							// 破棄

	void SetPause(bool bFlg){m_bNextPause = bFlg;}

	// ゲットセット
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


//==マクロ定義部===================================================================
#define REGISTER			((CRegister*)CRegister::GetInstance())
#define REGISTER_OBJ(a)		(CRegister::GetInstance()->GetObjList(a))
#define REGISTER_H_P(a,type)	((type)(CRegister::GetInstance()->handle.HandleToPointer(a)))

#define OBJ_LIST		std::list<CObjBase*>
#define OBJ_LIST_ITE	std::list<CObjBase*>::iterator
//=================================================================================



//=================================================================================
//	End of File
//=================================================================================