struct VSQuadOut {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};


float4 main(VSQuadOut vs_out) : SV_TARGET
{
	return float4(1,0,1,1);
}