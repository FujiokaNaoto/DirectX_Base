//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	���ʃC���N���[�h�p�w�b�_
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../Object/Base/CHANDLE.h"

#include "audiere.h"
using namespace audiere;
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
#define RANDF	((rand() & 0xfff) * (1.0f / 4095.0f))
//=================================================================================


//==�}�N����`��===================================================================
// ���S���
#ifndef	SAFE_RELEASE
#define SAFE_RELEASE(x)	if(x){x->Release(); x = NULL;}
#endif

// ���S�폜
#ifndef	SAFE_DELETE
#define SAFE_DELETE(x)	if(x){delete x; x = NULL;}
#endif

// �z����S�폜
#ifndef	SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if(x){delete[] x ; x = NULL;}
#endif
//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//=================================================================================
//	End of File
//=================================================================================