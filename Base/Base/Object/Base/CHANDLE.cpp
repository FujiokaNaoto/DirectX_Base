#include "CHANDLE.h"
#include "../../Window/Window.h"
#include <assert.h>
//C_OBJHANDLE///////////////////////////////////////////////////////////////////////////////////

/// デフォルトコンストラクタ
C_HANDLE::C_HANDLE( bool mem_init )
{
	m_pBuf = NULL;
	m_BufNum = 0;		//確保中のバッファの量※ハンドル数
	m_BufPos = 0;		//バッファの位置
	m_HandleNum = 0;
	
	if( mem_init == true )
		ResizeBuf(512 * 4);
}


/// デストラクタ
C_HANDLE::~C_HANDLE()
{
	free( m_pBuf );
}


/// 初期化
/// コンストラクタでmem_initをfalseにしたら明示的に初期化する
void C_HANDLE::Init( unsigned int Num )
{
	m_pBuf = (THANDLE*) malloc( sizeof(THANDLE)*Num );
	memset( m_pBuf, 0, sizeof(THANDLE)*Num );
	for( unsigned int i = 0; i < Num; i++)
		m_pBuf[i].Count = 1;
	
	m_BufNum = Num;
}


void C_HANDLE::ResizeBuf(unsigned int Num)
{
	assert(Num >= m_BufNum);	//バッファを小さくすることはできません
	m_pBuf = (THANDLE*)realloc(m_pBuf, Num*sizeof(THANDLE));
	for(unsigned int i = m_BufNum; i < Num; i++)
	{
		memset(&m_pBuf[i], 0, sizeof(THANDLE));
		m_pBuf[i].Count = 1;
	}
	m_BufNum = Num;
}

S_HANDLE C_HANDLE::CreateHandle(void *Adr)
{
	assert(Adr);	//値がNULLはハンドルにできません。
	if(m_HandleNum * 4 > m_BufNum)
	{
		ResizeBuf(m_BufNum * 2);
	}
	do  {
		m_BufPos++;
		if(m_BufPos >= m_BufNum)	m_BufPos = 0;
	} while (m_pBuf[m_BufPos].Pointer); 
	m_HandleNum++;
	m_pBuf[m_BufPos].Pointer = Adr;
	m_pBuf[m_BufPos].Count++;
	if(m_pBuf[m_BufPos].Count == 0)	m_pBuf[m_BufPos].Count = 1;
	return 	(m_pBuf[m_BufPos].Count << 16) + m_BufPos;
}

void C_HANDLE::ReleaseHandle(S_HANDLE Handle)
{
	unsigned int Pos = Handle & 0xffff;
	m_pBuf[Pos].Pointer = NULL;
	m_HandleNum--;
}

void *C_HANDLE::HandleToPointer(S_HANDLE Handle) const
{
	if(Handle == 0)	return NULL;
	unsigned int Pos = Handle & 0xffff;
	unsigned int Cnt = Handle >> 16;
	if(m_pBuf[Pos].Count != Cnt)	return NULL;
	return m_pBuf[Pos].Pointer;
}
