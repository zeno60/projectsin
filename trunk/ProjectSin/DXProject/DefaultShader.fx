/*******************************************************************
 *         Advanced 3D Game Programming with DirectX 10.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *	
 *	See license.txt for modification and distribution information
 *		copyright (c) 2007 by Peter Walsh, Wordware
 ******************************************************************/

matrix World;
matrix View;
matrix Projection;

#define MAX_LIGHTS 10	// Ensure this is the same as the C++ value

int4  g_viLightStatus;
float4 g_vLightColors[MAX_LIGHTS];
float4 g_vLightDirections[MAX_LIGHTS];
float4 g_vOutputColor;

////////////////////////////////////////////////////////////////////////////
// Default Vertex Shader
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : TEXCOORD0;
};

PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Norm = mul( input.Norm, World );
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    float4 finalColor = 0;
    
    //do NdotL lighting for 2 lights
    for(int i=0; i<g_viLightStatus.x; i++)
    {
        finalColor += saturate( dot( (float3)g_vLightDirections[i],input.Norm) * g_vLightColors[i] );
    }
    finalColor.a = 1;
    return finalColor;
}


//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid( PS_INPUT input) : SV_Target
{
    return g_vOutputColor;
}

////////////////////////////////////////////////////////////////////////////
// Default Technique
technique10 DefaultTechnique
{
    pass Pass0
    {
		SetGeometryShader(NULL);
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}

technique10 DefaultTechniqueLight
{
	pass Pass0
	{
		SetGeometryShader(NULL);
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetPixelShader(CompileShader(ps_4_0, PSSolid()));
    }
}