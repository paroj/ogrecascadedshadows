float4 tex2D_inBranch(uniform sampler2D sampler, float2 texCoords)
{
	#ifdef TEX2D_FORCE_ZERO_GRAD_IN_BRANCH
		return tex2D(sampler, texCoords, 0, 0);
	#else
		return tex2D(sampler, texCoords);
	#endif
}
