// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

struct VS_Input 
{
  float2 position : POS;
  float2 uv       : TEX;
};

struct VS_Output 
{
  float4 position : SV_POSITION;
  float2 uv       : TEXCOORD;
};

Texture2D    mytexture : register(t0);
SamplerState mysampler : register(s0);

cbuffer constant_buffer
{
  float3    color;      // 12
  float     padding0;   // 4
  float4x4  view_proj;  // 64
};

VS_Output vs_main(VS_Input input)
{
  VS_Output output;
  output.position = float4(input.position, 0.0f, 1.0f);
  output.uv = input.uv;
  return output;
}

float4 ps_main(VS_Output input) : SV_Target
{
  float4 result = mytexture.Sample(mysampler, input.uv);
  result.rgb *= color;
  return result;
}
