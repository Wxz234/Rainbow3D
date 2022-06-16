struct VSQuadOut {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};

float4 main(VSQuadOut vs_out) : SV_TARGET
{
	float4 color = float4(0.5,0.5,0.5,1);

	return color;
}