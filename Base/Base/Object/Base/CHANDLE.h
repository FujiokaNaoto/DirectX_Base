#pragma once		//これで重複インクルードを防ぐ

typedef unsigned int S_HANDLE;
class C_HANDLE
{
	struct THANDLE
	{
		unsigned short	Count;
		void			*Pointer;
	};
	unsigned int		m_HandleNum;	//現在使用中のハンドルの数
	THANDLE	*m_pBuf;
	unsigned int		m_BufNum;		//確保中のバッファの量※ハンドル数
	unsigned int		m_BufPos;		//バッファの位置
public:
	/// デフォルトコンストラクタ
	C_HANDLE( bool mem_init = true );
	/// デストラクタ
	virtual ~C_HANDLE();
	
	void ResizeBuf(unsigned int Num);			//バッファサイズ変更　			※使うハンドル最大数*4が望ましい
	S_HANDLE CreateHandle(void *Adr);		//アドレスからハンドルに変換	※０は帰ってこない
	void ReleaseHandle(S_HANDLE Handle);		//ハンドルを開放
	void *HandleToPointer(S_HANDLE Handle) const;	//ハンドルからアドレスに変換
	
	/// 初期化
	void Init( unsigned int Num );
};

