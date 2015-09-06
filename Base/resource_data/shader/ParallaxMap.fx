// バンプマップ・視差マッピング・視差遮蔽マッピング
string XFile = "Bump/square.x";

// 光源ベクトル (光の進む方向)
float3 lightDir
<
    string UIDirectional = "Light Direction";
> = {-0.35f, -0.25f, 0.9f};
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ アノテーション

float3 CameraPos   : CAMERAPOSITION;        //視点座標

// 光源の色
float4  I_a = {0.2f, 0.2f, 0.2f, 1.0f};        //環境光
float4  I_d = {1.0f, 1.0f, 1.0f, 1.0f};        //拡散光
float4  I_s = {1.0f, 1.0f, 1.0f, 1.0f};        //鏡面反射光

float4    k_a    :    MATERIALAMBIENT;
float4    k_d    :    MATERIALDIFFUSE;
float4    k_s    :    MATERIALSPECULAR;
float4    k_n    :    MATERIALPOWER;

// バンプ用テクスチャ
texture Tex : MATERIALTEXTURE <string name = "Bump/stones.bmp";>;            // 通常テクスチャ
texture NormalHeightMap : NORMALTEXTURE <string name = "Bump/stones_NM_height.tga";>;

// 変換マトリックス
float4x4 World            :    WORLD;                // ワールド
float4x4 WorldViewProj    :    WORLDVIEWPROJECTION;  // ワールド×ビュー×射影

// 計算用定数
float MaxHeight = 0.05f;
float Division = 0.002f;


// サンプラステート
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


// バンプマッピング(頂点シェーダ)
void BumpMapVS( float3 InPos    : POSITION ,
                float3 InTan    : TANGENT,
                float3 InNorm   : NORMAL,
                float2 InTex    : TEXCOORD0,
                out float4 OutPos    : POSITION,
                out float2 OutTex    : TEXCOORD0,
                out float3 OutLight  : TEXCOORD1,
                out float3 OutView   : TEXCOORD2)    // ピクセルシェーディング用に出力
{
    OutPos = mul(float4(InPos , 1) , WorldViewProj);    // 射影頂点座標
    OutTex = InTex;                                        //テクスチャ座標

    float3 BiNormal = cross(InTan , InNorm);            // 従法線
    float4 P = mul(float4(InPos , 1), World);            // ワールド空間頂点座標
    float3 T = normalize(mul(InTan , (float3x3)World));    // 接ベクトル(タンジェント)
    float3 B = normalize(mul(BiNormal , (float3x3)World));    // ワールド空間上の接ベクトル
    float3 N = normalize(mul(InNorm , (float3x3)World));    // ワールド空間上の法線ベクトル
    float3 V = normalize(CameraPos - P);                // 視点へのベクトル
    float3 L = normalize(- lightDir);                    // ワールド空間上光源へのベクトル
    OutView.x = dot(V , B);                                // 視点へのベクトル
    OutView.y = dot(V , T);
    OutView.z = dot(V , N);
    
    OutLight.x = dot(L , B);                            // 光源へのベクトル
    OutLight.y = - dot(L , T);    
    OutLight.z = dot(L , N);
}


// バンプマッピングピクセルシェーダ
void BumpMapPS(float2 InTex : TEXCOORD0,
                float3 InLight : TEXCOORD1,
                float3 InView : TEXCOORD2,
                out float4 OutCol : COLOR)
{
    float3 L = normalize(InLight);                            // 光源へのベクトル
    float3 N = tex2D(NHSampler , InTex) * 2.0f - 1.0f;        // 視点へのベクトル
    N = normalize(N);
    float3 V = normalize(InView);                            // 視点へのベクトル
    float3 R = reflect(-V , N);                                // 鏡面反射

    float4 Diff = I_a * k_a + I_d * k_d * max(0, dot(L , N));    // 拡散光
    float4 Spec = I_s * k_s * pow(max(0 , dot(L , R)) , k_n * 0.25f);
    OutCol = tex2D(TexSampler , InTex) * Diff + Spec;
}


// 視差マッピング ピクセルシェーダ
void ParallaxMapPS(float2 InTex        :    TEXCOORD0,
                    float3 InLight    :    TEXCOORD1,
                    float3 InView    :    TEXCOORD2,
                    out float4 OutCol    :    COLOR)
{
    float3 L = normalize(InLight);                    // 光源へのベクトル
    float3 V = normalize(InView);                    // 視点へのベクトル
    float H = tex2D(NHSampler , InTex).a;            // 高さ取得
    float2 Tex = InTex + V.xy * H * MaxHeight;        // UV座標をずらす
    float3 N = (float3)tex2D(NHSampler , Tex) * 2.0f - 1.0f;    // 法線ベクトル(接空間)
    N = normalize(N);
    float3 R = reflect(-V , N);
    float4 Diff = I_a * k_a + I_d * k_d * max(0 , dot(L, N));    // 拡散光
    float4 Spec = I_s * k_s * pow(max(0, dot(L , R)), k_n * 0.25f);    // 鏡面反射
    OutCol = tex2D(TexSampler, Tex) * Diff + Spec;    // 最終的な色
}


// 視差遮蔽マッピング (ピクセルシェーダ)
void POMapPS(float2 InTex    :    TEXCOORD0,
             float3 InLight    :    TEXCOORD1,
             float3 InView    :    TEXCOORD2,
             out float4 OutCol    :    COLOR)
{
    float3 L = normalize(InLight);                    // 光源へのベクトル
    float3 V = normalize(InView);                    // 視点へのベクトル
    float2 dx = ddx(InTex);                            // x方向への勾配
    float2 dy = ddy(InTex);                            // y方向への勾配
    float4 NH = float4(0 ,0 ,1 ,1);                    // 法線ベクトル&高さ
    float2 offset = V.xy * Division;                // サンプリング位置へのオフセット
    float2 Tex = InTex;                                // サンプリング位置
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
    
    float3 N = normalize((float3)NH * 2.0f - 1.0f);                // 法線
    float3 R = reflect(-V ,N);                                    // 反射光
    float4 Diff = I_a * k_a + I_d * k_d * max(0, dot(L, N));    // 拡散光
    float4 Spec = I_s * k_s * pow(max(0, dot(L,R)) , k_n * 0.25f);    // 鏡面反射
    OutCol = tex2Dgrad(TexSampler , Tex , dx , dy) * Diff + Spec;    // 最終的な色
}


// --テクニック--
// バンプマッピング
technique tecBumpMap
{
    // パス
    pass p0
    {
        VertexShader = compile vs_3_0 BumpMapVS();
        PixelShader = compile ps_3_0 BumpMapPS();
    }
}


// 視差マッピング
technique tecParallaxMap
{
    // パス
    pass p0
    {
        VertexShader = compile vs_3_0 BumpMapVS();
        PixelShader = compile ps_3_0 ParallaxMapPS();
    }
}


// 視差遮蔽マッピング
technique tecParallaxOcclusionMap
{
    // パス
    pass p0
    {
        VertexShader = compile vs_3_0 BumpMapVS();
        PixelShader = compile ps_3_0 POMapPS();
    }
}