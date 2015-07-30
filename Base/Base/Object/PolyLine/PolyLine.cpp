//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	2D�I�u�W�F�N�g�x�[�X
//*********************************************************************************


//==include��======================================================================
#include "../../System/GameManager.h"

#include "PolyLine.h"
#include "../../DirectX/Graphics.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================

//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//==�ÓI�����o�ϐ���===============================================================

//=================================================================================


//==�v���g�^�C�v�錾===============================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CObjLine::CObjLine():
m_pTexture(NULL)
{
	m_nClassID = eClassCode::OBJ_2D;

	m_HitRect.m_fWidth = 0.0f;
	m_HitRect.m_fHeight = 0.0f;
	m_HitRect.m_fDepth = 0.0f;
	m_HitRect.m_nPointType = eHitRect::CENTER;
	m_fAnimeTime = 0.0f;
	m_nAnimeSpeed = 0;
	m_bAnimeFlg = false;
	m_nWaitTime = 0;
	m_nWaitCnt = 0;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CObjLine::~CObjLine()
{

}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObjLine* CObjLine::Create(LPCTSTR texName , D3DXVECTOR3 Spos ,  D3DXVECTOR3 Epos,
		 int nWidth,int nPartition , int nCurvType, int nSceneID , int nPriority)
{

	D3DXVECTOR3 vec;
	D3DXVECTOR3 vec2;

	switch(nCurvType)
	{
	case POLYLINE_TYPE::HERMITE:
		vec.x = 0;
		vec.y = 0;
		return Create(texName , Spos ,Epos , vec , vec, nWidth, nPartition, nCurvType, nSceneID , nPriority);
		break;

	case POLYLINE_TYPE::BEZIER:
		vec = Spos;
		vec2 = Epos;
		return Create(texName , Spos ,Epos , vec , vec2, nWidth, nPartition, nCurvType, nSceneID , nPriority);
		break;
	}
	return Create(texName , Spos ,Epos , vec , vec2, nWidth, nPartition, nCurvType, nSceneID , nPriority);
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObjLine* CObjLine::Create(LPCTSTR texName ,  D3DXVECTOR3 Spos ,D3DXVECTOR3 Epos , 
	D3DXVECTOR3 Svec, D3DXVECTOR3 Evec , int nWidth ,int nPartition, int nCurvType, int nSceneID , int nPriority)
{
	CObjLine*		pObj = new CObjLine;

	// --�����l�i�[--
	pObj->m_pTexture = MANAGER.GetTexManage()->Entry(texName);
	if(! pObj->m_pTexture)	return NULL;			// �o�^���s����NULL��Ԃ�
	
	pObj->m_Spos = Spos;
	pObj->m_Epos = Epos;
	pObj->m_Svec = Svec;
	pObj->m_Evec = Evec;

	pObj->m_HitRect.m_fWidth = (float)nWidth;		// ��
	pObj->m_nPartition = nPartition;				// ������
	pObj->m_nCurvType = nCurvType;					// �Ȑ��^�C�v
	pObj->m_nSceneID = nSceneID;					// �V�[��ID
	pObj->m_nPriority = nPriority;					// �v���C�I���e�B
	
	return pObj;
}

//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
void CObjLine::Init(LPCTSTR texName , D3DXVECTOR3 Spos ,  D3DXVECTOR3 Epos ,
		 int nWidth ,int nPartition ,int nCurvType, int nSceneID , int nPriority)
{
	D3DXVECTOR3 vec;
	D3DXVECTOR3 vec2;

	switch(nCurvType)
	{
	case POLYLINE_TYPE::HERMITE:
		vec.x = 0;
		vec.y = 0;
		return Init(texName , Spos ,Epos , vec , vec, nWidth, nPartition, nCurvType, nSceneID , nPriority);
		break;

	case POLYLINE_TYPE::BEZIER:
		vec = Spos;
		vec2 = Epos;
		return Init(texName , Spos ,Epos , vec , vec2, nWidth, nPartition, nCurvType, nSceneID , nPriority);
		break;
	}
	return Init(texName , Spos ,Epos , vec , vec2, nWidth, nPartition, nCurvType, nSceneID , nPriority);
}

//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
void CObjLine::Init(LPCTSTR texName , D3DXVECTOR3 Spos , D3DXVECTOR3 Epos,
			D3DXVECTOR3 Svec ,D3DXVECTOR3 Evec , int nWidth ,int nPartition,int nCurvType, int nSceneID , int nPriority)
{
	// --�����l�i�[--
	m_pTexture = MANAGER.GetTexManage()->Entry(texName);
	
	m_Spos = Spos;
	m_Epos = Epos;
	m_Svec = Svec;
	m_Evec = Evec;

	m_HitRect.m_fWidth = (float)nWidth;		// ��
	m_nPartition = nPartition;				// ������
	m_nCurvType = nCurvType;					// �Ȑ��^�C�v
	m_nSceneID = nSceneID;					// �V�[��ID
	m_nPriority = nPriority;					// �v���C�I���e�B
}



//**�֐�***************************************************************************
//	�T�v	:	���
//*********************************************************************************
void CObjLine::Release()
{
	Uninit();
	delete this;
}


//**�֐�***************************************************************************
//	�T�v	:	�A�t�B���ϊ�
//*********************************************************************************
void CObjLine::Affine()
{
	// �g���Ă��Ȃ��Ƃ��͏����Ȃ�
	if(!m_bUseFlg) return;

	D3DXMATRIX	matTrans , matRot , matScall;

	// ������
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScall);

	// �A�t�B��
	D3DXMatrixTranslation(&matTrans , m_Pos.x , m_Pos.y , m_Pos.z);			// �ړ�
	D3DXMatrixRotationYawPitchRoll(&matRot , m_Rot.y , m_Rot.x , m_Rot.z);	// ��]
	D3DXMatrixScaling(&matScall , m_Scall.x , m_Scall.y , m_Scall.z);		// �g�k

	m_Matrix = matRot * matScall * matTrans;

	/*
	for(int n = 0 ; n < 4 ; n ++)
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_vVtx[n].vtx , (D3DXVECTOR3*) &m_vVtxBase[n].vtx , &m_Matrix);
		*/
}

//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void CObjLine::Update()
{
	if(m_bAnimeFlg && m_nWaitCnt ? (m_nWaitCnt-- ? false:false) :true)
	{
		m_fAnimeTime += (1.0f / (float)m_nPartition) * (float)m_nAnimeSpeed;

		while(m_fAnimeTime > 1)
		{
			m_fAnimeTime = 0.0f;
			m_nWaitCnt = m_nWaitTime;
		}

	}
}

//**�֐�***************************************************************************
//	�T�v	:	�`��
//*********************************************************************************
void CObjLine::Draw()
{
	// �g�p�t���O���I�t�܂��͌�`��t���O���I���Ȃ�`�悵�Ȃ�
	if(!m_bUseFlg || m_bDrawLate) return;			
	
	// --2D�p�ݒ�ɕύX--
	// ���C�g�I�t
	CGraphics::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	// �A���t�@ �u�����h�L����
	CGraphics::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    CGraphics::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    CGraphics::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Z�o�b�t�@�I�t
	CGraphics::GetDevice()->SetRenderState(D3DRS_ZENABLE , FALSE);

	// �e�N�X�`���Z�b�g
	if(m_pTexture) CGraphics::GetDevice()->SetTexture(0 , *m_pTexture);

	CGraphics::GetDevice()->SetFVF(FVF_VERTEX_2D);
	CGraphics::GetDevice()->SetTransform(D3DTS_WORLD , &m_Matrix);

	//���_�ɋȐ���̓_����
	SetLineVtx();
	/*
	//�Ȑ���̓_���Q�l�Ƀ|���S���`��p���_���Z�b�g
	SetVtx();
	//�e�N�X�`���Z�b�g
	SetTex();
	//�f�B�t���[�Y�Z�b�g
	//SetDiffuse(255,255,255,255);
	*/
	SetAll();

	for(int i=0; i < m_nPartition -1; i++)
	{

		VERTEX_2D VTX[4];

		VTX[0] = m_vVtx[i+1][2];
		VTX[1] = m_vVtx[i+1][1];
		VTX[2] = m_vVtx[i][2];
		VTX[3] = m_vVtx[i][1];

		if(!(VTX[0].tex.y > VTX[2].tex.y))
			CGraphics::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , VTX , sizeof(VERTEX_2D));
	}	
	// --2D�p�ݒ��߂�--
	// Z�o�b�t�@�I��
	CGraphics::GetDevice()->SetRenderState(D3DRS_ZENABLE , TRUE);
	 // �A���t�@ �u�����h������
    CGraphics::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	// ���C�g�I��
	CGraphics::GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//**�֐�***************************************************************************
//	�T�v	:	�`��
//*********************************************************************************
void CObjLine::DrawLate()
{
	// �g�p�t���O���I�t�܂��͌�`��t���O���I�t�Ȃ�`�悵�Ȃ�
	if(!m_bUseFlg || ! m_bDrawLate) return;
	
	// --2D�p�ݒ�ɕύX--
	// ���C�g�I�t
	CGraphics::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	// �A���t�@ �u�����h�L����
	CGraphics::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    CGraphics::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    CGraphics::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�o�b�t�@�I�t
	CGraphics::GetDevice()->SetRenderState(D3DRS_ZENABLE , FALSE);

	// �e�N�X�`���Z�b�g
	if(m_pTexture) CGraphics::GetDevice()->SetTexture(0 , *m_pTexture);

	CGraphics::GetDevice()->SetFVF(FVF_VERTEX_2D);
	CGraphics::GetDevice()->SetTransform(D3DTS_WORLD , &m_Matrix);
	CGraphics::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx , sizeof(VERTEX_2D));
	
	// --2D�p�ݒ��߂�--
	// Z�o�b�t�@�I��
	CGraphics::GetDevice()->SetRenderState(D3DRS_ZENABLE , TRUE);
	 // �A���t�@ �u�����h������
    CGraphics::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	CGraphics::GetDevice()->SetRenderState(D3DRS_ZENABLE , TRUE);

	// ���C�g�I��
	CGraphics::GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//**�֐�***************************************************************************
//	�T�v	:	�n�_�A�I�_�̃Z�b�g
//*********************************************************************************
void CObjLine::SetLinePos(D3DXVECTOR3 Spos, D3DXVECTOR3 Epos)	
{
	m_Spos = Spos;
	m_Epos = Epos;
}


//**�֐�***************************************************************************
//	�T�v	:	�x�N�g��or���_�̃Z�b�g
//*********************************************************************************
void CObjLine::SetVector(D3DXVECTOR3 Svec, D3DXVECTOR3 Evec)	
{
	m_Svec = Svec;
	m_Evec = Evec;
}


//**�֐�***************************************************************************
//	�T�v	:	�A�j���[�V�������Z�b�g
//*********************************************************************************
void CObjLine::AnimeSet(int Speed, int WaitTime)
{
	m_bAnimeFlg = true;
	m_nAnimeSpeed = Speed;
	m_nWaitTime = WaitTime;
}

//**�֐�***************************************************************************
//	�T�v	:	�A�j���[�V������~
//*********************************************************************************
void CObjLine::AnimeStop()
{
	m_bAnimeFlg = false;
}

//**�֐�***************************************************************************
//	�T�v	:	�e�N�X�`���Z�b�g
//*********************************************************************************
void CObjLine::SetTex()
{

	for(int i = 0; i < m_nPartition; i++)
	{

		m_vVtx[i][2].tex = D3DXVECTOR2(0.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
			(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
			(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

		m_vVtx[i][1].tex = D3DXVECTOR2(1.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
			(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
		(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

	}
}


//**�֐�***************************************************************************
//	�T�v	:	���_���Z�b�g
//*********************************************************************************
void CObjLine::SetVtx()
{
	//�|���S���`��p���_���Z�b�g
	for(int i = 0; i < m_nPartition; i++)
	{
		D3DXVECTOR3 vec;
		D3DXVECTOR3 Lvec;
		D3DXVECTOR3 Rvec;

		vec.x = m_vVtx[i+1][0].vtx.x - m_vVtx[i][0].vtx.x;
		vec.y = m_vVtx[i+1][0].vtx.y - m_vVtx[i][0].vtx.y;
		vec.z = 0;

		D3DXVec3Normalize(&vec,&vec);
		vec.x = vec.x* (float)m_HitRect.m_fWidth*0.5f;
		vec.y = vec.y* (float)m_HitRect.m_fWidth*0.5f;

		Lvec.x = -vec.y;
		Lvec.y = vec.x;
		Rvec.x = vec.y;
		Rvec.y = -vec.x;

		m_vVtx[i][1].vtx.x = m_vVtx[i][0].vtx.x + Lvec.x;
		m_vVtx[i][1].vtx.y = m_vVtx[i][0].vtx.y + Lvec.y;
		m_vVtx[i][2].vtx.x = m_vVtx[i][0].vtx.x + Rvec.x;
		m_vVtx[i][2].vtx.y = m_vVtx[i][0].vtx.y + Rvec.y;

		if(i == m_nPartition - 2)
		{
			i++;
			m_vVtx[i][1].vtx.x = m_vVtx[i][0].vtx.x + Lvec.x;
			m_vVtx[i][1].vtx.y = m_vVtx[i][0].vtx.y + Lvec.y;
			m_vVtx[i][2].vtx.x = m_vVtx[i][0].vtx.x + Rvec.x;
			m_vVtx[i][2].vtx.y = m_vVtx[i][0].vtx.y + Rvec.y;
		}
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�Ȑ���̒��_���Z�b�g
//*********************************************************************************
void CObjLine::SetLineVtx()
{
	float t = 0;
	switch(m_nCurvType)
		{
		case POLYLINE_TYPE::HERMITE:
				for(int i = 0; t <= 1 ; t += (float)((float)1/(float)m_nPartition),i++)
				{
					D3DXVECTOR3 Out;
					D3DXVec3Hermite(&Out,
									&m_Spos,
									&m_Svec,
									&m_Epos,
									&m_Evec,
									t);

					m_vVtx[i][0].vtx.x = Out.x;
					m_vVtx[i][0].vtx.y = Out.y;
				}
				break;


		case  POLYLINE_TYPE::BEZIER:
				for(int i = 0; t <= 1 ; t += (float)((float)1/(float)m_nPartition),i++)
				{
					D3DXVECTOR3 Out;
					Out.x = (1-t)*(1-t)*(1-t)*m_Spos.x + 3*(1-t)*(1-t)*t*m_Svec.x + 3*(1-t)*t*t*m_Evec.x + t*t*t*m_Epos.x;
					Out.y = (1-t)*(1-t)*(1-t)*m_Spos.y + 3*(1-t)*(1-t)*t*m_Svec.y + 3*(1-t)*t*t*m_Evec.y + t*t*t*m_Epos.y;

					m_vVtx[i][0].vtx.x = Out.x;
					m_vVtx[i][0].vtx.y = Out.y;
				}
				break;

		}
}


//**�֐�***************************************************************************
//	�T�v	:	�F�ݒ�
//*********************************************************************************
void CObjLine::SetDiffuse(int r , int g , int b , int a)
{
	//�錾��
	int 	nLoopCnt;
	for(nLoopCnt = 0 ; nLoopCnt < m_nPartition ; nLoopCnt ++)
	{
		m_vVtx[nLoopCnt][1].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//�F�w��
		m_vVtx[nLoopCnt][2].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//�F�w��
	}
}

//**�֐�***************************************************************************
//	�T�v	:	���_���S�ݒ�
//********************************************************************************* 
void CObjLine::SetAll()
{
	float t = 0.0f;
	for(int i = 0; i < m_nPartition; i++,t += 1.0f/(float)m_nPartition)
	{
		//�|���S���`��p���_���Z�b�g
		D3DXVECTOR3 vec;
		D3DXVECTOR3 Lvec;
		D3DXVECTOR3 Rvec;

		float fWidth = m_HitRect.m_fWidth;
		D3DCOLOR fDiffuse = D3DCOLOR_RGBA(255,255,255,255);

		AddEffect(t,&fWidth,&fDiffuse);

		vec.x = m_vVtx[i+1][0].vtx.x - m_vVtx[i][0].vtx.x;
		vec.y = m_vVtx[i+1][0].vtx.y - m_vVtx[i][0].vtx.y;
		vec.z = 0;

		D3DXVec3Normalize(&vec,&vec);
		vec.x = vec.x* (float)fWidth*0.5f;
		vec.y = vec.y* (float)fWidth*0.5f;

		Lvec.x = -vec.y;
		Lvec.y = vec.x;
		Rvec.x = vec.y;
		Rvec.y = -vec.x;

		m_vVtx[i][1].vtx.x = m_vVtx[i][0].vtx.x + Lvec.x;
		m_vVtx[i][1].vtx.y = m_vVtx[i][0].vtx.y + Lvec.y;
		m_vVtx[i][2].vtx.x = m_vVtx[i][0].vtx.x + Rvec.x;
		m_vVtx[i][2].vtx.y = m_vVtx[i][0].vtx.y + Rvec.y;

		//�e�N�X�`���ݒ�
		m_vVtx[i][2].tex = D3DXVECTOR2(0.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
		(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
		(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

		m_vVtx[i][1].tex = D3DXVECTOR2(1.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
		(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
		(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

		m_vVtx[i][1].diffuse = fDiffuse;		//�F�w��
		m_vVtx[i][2].diffuse = fDiffuse;		//�F�w��


		if(i == m_nPartition - 2)
		{
			i++;
			t = 1.0f;
			fWidth = m_HitRect.m_fWidth;
			fDiffuse = D3DCOLOR_RGBA(255,255,255,255);

			AddEffect(t,&fWidth,&fDiffuse);
			m_vVtx[i][1].vtx.x = m_vVtx[i][0].vtx.x + Lvec.x;
			m_vVtx[i][1].vtx.y = m_vVtx[i][0].vtx.y + Lvec.y;
			m_vVtx[i][2].vtx.x = m_vVtx[i][0].vtx.x + Rvec.x;
			m_vVtx[i][2].vtx.y = m_vVtx[i][0].vtx.y + Rvec.y;
			//�e�N�X�`���ݒ�
			m_vVtx[i][2].tex = D3DXVECTOR2(0.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
			(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
			(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

			m_vVtx[i][1].tex = D3DXVECTOR2(1.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
			(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
			(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

			m_vVtx[i][1].diffuse = fDiffuse;		//�F�w��
			m_vVtx[i][2].diffuse = fDiffuse;		//�F�w��
		}


	}

}

//**�֐�***************************************************************************
//	�T�v	:	�ǉ����ʋL�q�p�֐�(�ʏ�͋�)
//*********************************************************************************
void CObjLine::AddEffect(float t, float* Width, D3DCOLOR* Diffuse)
{
	//�e�X�g�p�L�q
	if(t < 0.1f)
		*Diffuse = D3DCOLOR_RGBA(255,255,255,(int)((float)255*t*(float)10));

	if(t > 0.9f)
		*Diffuse = D3DCOLOR_RGBA(255,255,255,(int)((float)255*(1.0f-t)*(float)10));

	//*Width = ((abs(t-0.5f)*-1.0f)+1.0f)* (*Width*2);
	*Width = t/2**Width*2 + *Width/2;
}


//**�֐�***************************************************************************
//	�T�v	:	�����蔻���`�擾
//*********************************************************************************
CHitRect CObjLine::GetHitRect()
{
	m_HitRect.m_Point = m_Pos;
	return m_HitRect;
}
//=================================================================================
//	End of File
//=================================================================================