//プログラムヘッダ*****************************************************************
//	プログラム概要	:	3Dオブジェクトメッシュ
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "../Base/ObjBase.h"
#include "../../Resource/Mesh.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================

//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================
class CObj3D : public CObjBase
{
public:
	// コンストラクタ・デストラクタ
	CObj3D();
	virtual ~CObj3D();

	static CObj3D*	Create(LPCTSTR meshName , D3DXVECTOR3 pos , 
		 int nSceneID , int nPriority);								// 生成

	static CObj3D*	Create(LPCTSTR meshName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nSceneID
		 , int nPriority);											// 生成

	virtual bool	Init(LPCTSTR meshName , D3DXVECTOR3 pos , 
		 int nSceneID , int nPriority);

	virtual bool	Init(LPCTSTR meshName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nSceneID
		 , int nPriority);


	virtual void	SetRS();
	virtual	void	SetRSLate();
	virtual void	Draw();											// 描画
	virtual void	DrawLate();										// 透明度付き描画

	virtual void	Release();										// 解放

	// --ゲットセット--
	CMesh*	GetMesh(){return m_pMesh;}								// メッシュデータ取得
	void	SetMesh(CMesh* pMesh){m_pMesh = pMesh;}					// メッシュデータセット

protected:
	CMesh*			m_pMesh;										// 3Dオブジェクトポインタ
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================