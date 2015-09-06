string XFile = "misc/nanase_pose.x";
//string XFile = "Tiger/tiger.x";
int BCLR = 0x000000ff;

// �ϊ��}�g���b�N�X
float4x4 matWVP : WORLDVIEWPROJECTION;  // ���[���h�~�r���[�~�ˉe
//                ^^^^^^^^^^^^^^^^^^^ �Z�}���e�B�N�X (���[�U��`)
float4x4 matW   : WORLD;                // ���[���h

float3 camera   : CAMERAPOSITION;       //���_���W

// �����x�N�g�� (���̐i�ޕ���)
float3 light
  <string UIDirectional = "Light Direction";>
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ �A�m�e�[�V����
= {-0.35f, -0.25f, 0.9f};

// �����̐F
float4  I_d = {1.0f, 1.0f, 1.0f, 1.0f};//�g�U��
float4  I_a = {0.2f, 0.2f, 0.2f, 1.0f};//����
float4  I_s = {1.0f, 1.0f, 1.0f, 1.0f};//���ʔ��ˌ�

// X�t�@�C���@�}�e���A��
float4  k_d : MATERIALDIFFUSE;   //�g�U�F
float4  k_s : MATERIALSPECULAR;  //���ʔ��ːF
float   k_p : MATERIALPOWER;     //���ʔ��ˎw��
float4  k_a : MATERIALAMBIENT;   //���F
texture tex : MATERIALTEXTURE;
bool    tex_enable : MATERIALTEXTUREVALID;

// �g�D�[���p�A���b�V���̔��a�擾
float	fRadius : MESHRADIUS;	// ���f���̔��a

// �T���v���X�e�[�g
sampler Sampler = sampler_state
{
    Texture = (tex);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


// ���ʒ��_�V�F�[�_
void CommonVS(float3 InPos    : POSITION , 
             float3 InNorm    : NORMAL , 
             float2 InTex    : TEXCOORD0,
             out float4 OutPos    : POSITION,
             out float2 OutTex    : TEXCOORD0,
             out float3 OutPosForPS : TEXCOORD1,    // �s�N�Z���V�F�[�f�B���O�p�ɏo��
             out float3 OutNorm    : TEXCOORD2)
{
    OutPos = mul(float4(InPos , 1) , matWVP);
    OutTex = InTex;
    OutPosForPS = mul(float4(InPos , 1) , matW).xyz;
    OutNorm = normalize(mul(InNorm , (float3x3)matW));
}


// �֊s�����_�V�F�[�_
void EdgeVS(float3 InPos    : POSITION , 
             float3 InNorm    : NORMAL , 
             out float4 OutPos    : POSITION)
{
    float3 P = InPos + InNorm * (fRadius * 0.0040f);
    OutPos = mul(float4(P , 1) , matWVP);
}


// �֊s���s�N�Z���V�F�[�_
void EdgePS(out float4 OutCol : COLOR)
{
    OutCol.rgb = 0.0f;
    OutCol.a = 1.0f;
}


// �s�N�Z���V�F�[�_
void testPS(float4 InDiff : COLOR0,
            float4 InSpec : COLOR1,
            float2 InTex  : TEXCOORD0,
            out float4 OutCol : COLOR0)
{
    OutCol = InDiff;
    if (tex_enable) {
        OutCol *= tex2D(Sampler, InTex);
    }
    OutCol += InSpec;
}

// �t�H���V�F�[�f�B���O(�s�N�Z���V�F�[�_)
void PhongPS(float2 InTex : TEXCOORD0,
             float3 InPos : TEXCOORD1,
             float3 InNorm : TEXCOORD2,
             out float4 OutCol : COLOR)
{
    float3 L = normalize(-light);
    float3 N = normalize(InNorm);
    float3 V = normalize(camera - InPos);
    float3 R = reflect(-V , N);                        // ���ʔ���

    float4 Diff = I_a * k_a + I_d * k_d * saturate(dot(L , N));
    float4 Spec = I_s * k_s * pow(saturate(dot(L , R)) , k_p * 0.25f);

    OutCol = Diff;
    if(tex_enable)
        OutCol *= tex2D(Sampler , InTex);
    OutCol += Spec;
}


// �g�D�[���V�F�[�f�B���O(�s�N�Z���V�F�[�_)
void ToonPS(float2 InTex : TEXCOORD0,
             float3 InPos : TEXCOORD1,
             float3 InNorm : TEXCOORD2,
             out float4 OutCol : COLOR)
{
    float3 L = normalize(-light);
    float3 N = normalize(InNorm);
    float3 V = normalize(camera - InPos);
    float3 R = reflect(-V , N);                        // ���ʔ���

    float4 Diff = I_a * k_a + I_d * k_d;// * saturate(dot(L , N));
    float4 Spec = 0.0f;//I_s * k_s * pow(saturate(dot(L , R)) , k_p * 0.25f);

    if(dot(L , N) < 0.4f) Diff *= 0.8f;

    OutCol = Diff;
    if(tex_enable)
        OutCol *= tex2D(Sampler , InTex);
    OutCol += Spec;
}


// �u�����V�F�[�f�B���O(�s�N�Z���V�F�[�_)
void BlinnPS(float2 InTex : TEXCOORD0,
             float3 InPos : TEXCOORD1,
             float3 InNorm : TEXCOORD2,
             out float4 OutCol : COLOR)
{
    float3 L = normalize(-light);
    float3 N = normalize(InNorm);
    float3 V = normalize(camera - InPos);
    float3 H = normalize(V + L);                        // ���ʔ���

    float4 Diff = I_a * k_a + I_d * k_d * saturate(dot(L , N));
    float4 Spec = I_s * k_s * pow(saturate(dot(N , H)) , k_p * 0.25f);

    OutCol = Diff;
    if(tex_enable)
        OutCol *= tex2D(Sampler , InTex);
    OutCol = OutCol * Diff + Spec;
}


// �N�b�N�E�g�����X�V�F�[�f�B���O(�s�N�Z���V�F�[�_)
void CookTrrancePS(float2 InTex : TEXCOORD0,
             float3 InPos : TEXCOORD1,
             float3 InNorm : TEXCOORD2,
             out float4 OutCol : COLOR)
{
    float3 L = normalize(-light);
    float3 N = normalize(InNorm);
    float3 V = normalize(camera - InPos);
    float3 H = normalize(V + L);                        // ���ʔ���

    float NV = dot(N , V);
    float NH = dot(N , H);
    float NH2 = NH * NH;
    float m = 0.35f;
    float m2 = m * m;
    float D = exp(- ( (1.0f - NH2) / NH2) / m2)
                   / (4.0f * m2 * NH2 * NH2);
    
    float VH = dot(V , H);
    float NL = dot(N , L);
    float G = min(min(2.0f * (NH * NV) / VH ,
                2.0f * (NH * NL) / VH) , 1.0f);

    float n = 10.0f;
    float c = dot(L , H);
    float g = sqrt(n * n + c * c - 1.0f);
    float gpc = g + c;
    float gmc = g - c;
    float d = gmc / gpc;
    float d2 = (c * gpc - 1.0f) / (c * gmc + 1.0f);

    float F = 0.5f * d * d * (1.0f + d2 * d2);

    float4 Diff = I_a * k_a + I_d * k_d * saturate(NL);
    float4 Spec = I_s * k_s * pow(saturate((D * G * F) / NV) , k_p * 0.25f);

    OutCol = Diff;
    if(tex_enable)
        OutCol *= tex2D(Sampler , InTex);
    OutCol += Spec;
}


// �t�H���V�F�[�f�B���O
technique tecPhone
{
    // �p�X
    pass p0
    {
        VertexShader = compile vs_2_0 CommonVS();
        PixelShader = compile ps_2_0 PhongPS();
    }
}


// �g�D�[���V�F�[�f�B���O
technique tecToon
{
    // �p�X
    pass p0
    {
        CullMode = CCW;
        VertexShader = compile vs_2_0 CommonVS();
        PixelShader = compile ps_2_0 ToonPS();
    }

    pass p1
    {
        CullMode = CW;
        VertexShader = compile vs_3_0 EdgeVS();
        PixelShader = compile ps_3_0 EdgePS();
    }
}


// �u�����V�F�[�f�B���O
technique tecBlinn
{
    // �p�X
    pass p0
    {
        VertexShader = compile vs_2_0 CommonVS();
        PixelShader = compile ps_2_0 BlinnPS();
    }
}


// �N�b�N�E�g�����X�V�F�[�f�B���O�e�N�j�b�N
technique tecCookTrrance
{
    // �p�X
    pass p0
    {
        VertexShader = compile vs_3_0 CommonVS();
        PixelShader = compile ps_3_0 CookTrrancePS();
    }
}