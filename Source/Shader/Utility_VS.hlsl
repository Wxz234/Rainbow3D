struct VSIN {
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};
struct VSOUT {
	float4 mypos : SV_POSITION;
	float2 uv : TEXCOORD;
};
VSOUT main(VSIN vs_in)
{
	VSOUT vs_out;
	vs_out.mypos = vs_in.pos;
	vs_out.uv = vs_in.uv;
	return vs_out;
}