#pragma once		//����ŏd���C���N���[�h��h��

typedef unsigned int S_HANDLE;
class C_HANDLE
{
	struct THANDLE
	{
		unsigned short	Count;
		void			*Pointer;
	};
	unsigned int		m_HandleNum;	//���ݎg�p���̃n���h���̐�
	THANDLE	*m_pBuf;
	unsigned int		m_BufNum;		//�m�ے��̃o�b�t�@�̗ʁ��n���h����
	unsigned int		m_BufPos;		//�o�b�t�@�̈ʒu
public:
	/// �f�t�H���g�R���X�g���N�^
	C_HANDLE( bool mem_init = true );
	/// �f�X�g���N�^
	virtual ~C_HANDLE();
	
	void ResizeBuf(unsigned int Num);			//�o�b�t�@�T�C�Y�ύX�@			���g���n���h���ő吔*4���]�܂���
	S_HANDLE CreateHandle(void *Adr);		//�A�h���X����n���h���ɕϊ�	���O�͋A���Ă��Ȃ�
	void ReleaseHandle(S_HANDLE Handle);		//�n���h�����J��
	void *HandleToPointer(S_HANDLE Handle) const;	//�n���h������A�h���X�ɕϊ�
	
	/// ������
	void Init( unsigned int Num );
};

