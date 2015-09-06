// �o���v�}�b�v�E�����}�b�s���O�E�����Օ��}�b�s���O
string XFile = "Bump/square.x";

// �����x�N�g�� (���̐i�ޕ���)
float3 lightDir
<
    string UIDirectional = "Light Direction";
> = {-0.35f, -0.25f, 0.9f};
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ �A�m�e�[�V����

float3 CameraPos   : CAMERAPOSITION;        //���_���W

// �����̐F
float4  I_a = {0.2f, 0.2f, 0.2f, 1.0f};        //����
float4  I_d = {1.0f, 1.0f, 1.0f, 1.0f};        //�g�U��
float4  I_s = {1.0f, 1.0f, 1.0f, 1.0f};        //���ʔ��ˌ�

float4    k_a    :    MATERIALAMBIENT;
float4    k_d    :    MATERIALDIFFUSE;
float4    k_s    :    MATERIALSPECULAR;
float4    k_n    :    MATERIALPOWER;

// �o���v�p�e�N�X�`��
texture Tex : MATERIALTEXTURE <string name = "Bump/stones.bmp";>;            // �ʏ�e�N�X�`��
texture NormalHeightMap : NORMALTEXTURE <string name = "Bump/stones_NM_height.tga";>;

// �ϊ��}�g���b�N�X
float4x4 World            :    WORLD;                // ���[���h
float4x4 WorldViewProj    :    WORLDVIEWPROJECTION;  // ���[���h�~�r���[�~�ˉe

// �v�Z�p�萔
float MaxHeight = 0.05f;
float Division = 0.002f;


// �T���v���X�e�[�g
sampler TexSampler = sampler_state
{
    Texture = (Tex);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler NHSampler = sampler_state
{
    Texture = (NormalHeightMap);
    MipFilter = LINEAR;
    MipFilter = LINEAR;
    MipFilter = LINEAR;
};


// �o���v�}�b�s���O(���_�V�F�[�_)
void BumpMapVS( float3 InPos    : POSITION ,
                float3 InTan    : TANGENT,
                float3 InNorm   : NORMAL,
                float2 InTex    : TEXCOORD0,
                out float4 OutPos    : POSITION,
                out float2 OutTex    : TEXCOORD0,
                out float3 OutLight  : TEXCOORD1,
                out float3 OutView   : TEXCOORD2)    // �s�N�Z���V�F�[�f�B���O�p�ɏo��
{
    OutPos = mul(float4(InPos , 1) , WorldViewProj);    // �ˉe���_���W
    OutTex = InTex;                                        //�e�N�X�`�����W

    float3 BiNormal = cross(InTan , InNorm);            // �]�@��
    float4 P = mul(float4(InPos , 1), World);            // ���[���h��Ԓ��_���W
    float3 T = normalize(mul(InTan , (float3x3)World));    // �ڃx�N�g��(�^���W�F���g)
    float3 B = normalize(mul(BiNormal , (float3x3)World));    // ���[���h��ԏ�̐ڃx�N�g��
    float3 N = normalize(mul(InNorm , (float3x3)World));    // ���[���h��ԏ�̖@���x�N�g��
    float3 V = normalize(CameraPos - P);                // ���_�ւ̃x�N�g��
    float3 L = normalize(- lightDir);                    // ���[���h��ԏ�����ւ̃x�N�g��
    OutView.x = dot(V , B);                                // ���_�ւ̃x�N�g��
    OutView.y = dot(V , T);
    OutView.z = dot(V , N);
    
    OutLight.x = dot(L , B);                            // �����ւ̃x�N�g��
    OutLight.y = - dot(L , T);    
    OutLight.z = dot(L , N);
}


// �o���v�}�b�s���O�s�N�Z���V�F�[�_
void BumpMapPS(float2 InTex : TEXCOORD0,
                float3 InLight : TEXCOORD1,
                float3 InView : TEXCOORD2,
                out float4 OutCol : COLOR)
{
    float3 L = normalize(InLight);                            // �����ւ̃x�N�g��
    float3 N = tex2D(NHSampler , InTex) * 2.0f - 1.0f;        // ���_�ւ̃x�N�g��
    N = normalize(N);
    float3 V = normalize(InView);                            // ���_�ւ̃x�N�g��
    float3 R = reflect(-V , N);                                // ���ʔ���

    float4 Diff = I_a * k_a + I_d * k_d * max(0, dot(L , N));    // �g�U��
    float4 Spec = I_s * k_s * pow(max(0 , dot(L , R)) , k_n * 0.25f);
    OutCol = tex2D(TexSampler , InTex) * Diff + Spec;
}


// �����}�b�s���O �s�N�Z���V�F�[�_
void ParallaxMapPS(float2 InTex        :    TEXCOORD0,
                    float3 InLight    :    TEXCOORD1,
                    float3 InView    :    TEXCOORD2,
                    out float4 OutCol    :    COLOR)
{
    float3 L = normalize(InLight);                    // �����ւ̃x�N�g��
    float3 V = normalize(InView);                    // ���_�ւ̃x�N�g��
    float H = tex2D(NHSampler , InTex).a;            // �����擾
    float2 Tex = InTex + V.xy * H * MaxHeight;        // UV���W�����炷
    float3 N = (float3)tex2D(NHSampler , Tex) * 2.0f - 1.0f;    // �@���x�N�g��(�ڋ��)
    N = normalize(N);
    float3 R = reflect(-V , N);
    float4 Diff = I_a * k_a + I_d * k_d * max(0 , dot(L, N));    // �g�U��
    float4 Spec = I_s * k_s * pow(max(0, dot(L , R)), k_n * 0.25f);    // ���ʔ���
    OutCol = tex2D(TexSampler, Tex) * Diff + Spec;    // �ŏI�I�ȐF
}


// �����Օ��}�b�s���O (�s�N�Z���V�F�[�_)
void POMapPS(float2 InTex    :    TEXCOORD0,
             float3 InLight    :    TEXCOORD1,
             float3 InView    :    TEXCOORD2,
             out float4 OutCol    :    COLOR)
{
    float3 L = normalize(InLight);                    // �����ւ̃x�N�g��
    float3 V = normalize(InView);                    // ���_�ւ̃x�N�g��
    float2 dx = ddx(InTex);                            // x�����ւ̌��z
    float2 dy = ddy(InTex);                            // y�����ւ̌��z
    float4 NH = float4(0 ,0 ,1 ,1);                    // �@���x�N�g��&����
    float2 offset = V.xy * Division;                // �T���v�����O�ʒu�ւ̃I�t�Z�b�g
    float2 Tex = InTex;                                // �T���v�����O�ʒu
    float height = MaxHeight;
    float buf = NH.a * MaxHeight;
    
    while(height > 0.0f)
    {
        NH = tex2Dgrad(NHSampler , Tex , dx , dy);
        if(NH.a * MaxHeight >= height)
            break;
        
        Tex -= offset;
        height -= V.z * Division;
    }
    
    float3 N = normalize((float3)NH * 2.0f - 1.0f);                // �@��
    float3 R = reflect(-V ,N);                                    // ���ˌ�
    float4 Diff = I_a * k_a + I_d * k_d * max(0, dot(L, N));    // �g�U��
    float4 Spec = I_s * k_s * pow(max(0, dot(L,R)) , k_n * 0.25f);    // ���ʔ���
    OutCol = tex2Dgrad(TexSampler , Tex , dx , dy) * Diff + Spec;    // �ŏI�I�ȐF
}


// --�e�N�j�b�N--
// �o���v�}�b�s���O
technique tecBumpMap
{
    // �p�X
    pass p0
    {
        VertexShader = compile vs_3_0 BumpMapVS();
        PixelShader = compile ps_3_0 BumpMapPS();
    }
}


// �����}�b�s���O
technique tecParallaxMap
{
    // �p�X
    pass p0
    {
        VertexShader = compile vs_3_0 BumpMapVS();
        PixelShader = compile ps_3_0 ParallaxMapPS();
    }
}


// �����Օ��}�b�s���O
technique tecParallaxOcclusionMap
{
    // �p�X
    pass p0
    {
        VertexShader = compile vs_3_0 BumpMapVS();
        PixelShader = compile ps_3_0 POMapPS();
    }
}