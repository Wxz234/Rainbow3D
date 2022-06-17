struct VSQuadOut {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};

VSQuadOut getoutput(uint index) {
	VSQuadOut Out;

	Out.texcoord = float2((index << 1) & 2, index & 2);
	Out.position = float4(Out.texcoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
	return Out;
}

[NumThreads(1, 1, 1)]
[OutputTopology("triangle")]
void main(
	out vertices VSQuadOut verts[3]
) {
	SetMeshOutputCounts(3, 1);
	verts[0] = getoutput(0);
	verts[1] = getoutput(1);
	verts[2] = getoutput(2);
}