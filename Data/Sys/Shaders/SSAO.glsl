// Simple SSAO

/*
[configuration]

[OptionRangeFloat]
GUIName = Sample Range
OptionName = SAMPLE_RANGE
MinValue = 0.001
MaxValue = 0.04
StepAmount = 0.0001
DefaultValue = 0.01

[OptionRangeFloat]
GUIName = Filter Limit
OptionName = FILTER_LIMIT
MinValue = 0.0001
MaxValue = 0.01
StepAmount = 0.0001
DefaultValue = 0.002


[OptionRangeFloat]
GUIName = Max Depth
OptionName = MAX_DEPTH
MinValue = 0.0001
MaxValue = 0.02
StepAmount = 0.0001
DefaultValue = 0.015

[OptionRangeFloat]
GUIName = Min Depth
OptionName = MIN_DEPTH
MinValue = 0.0
MaxValue = 0.02
StepAmount = 0.0001
DefaultValue = 0.0002
[Stage]
EntryPoint = SSAO
[Stage]
EntryPoint = BlurH
[Stage]
EntryPoint = Merger
[/configuration]
*/
#define NUMSAMPLES 32
float3 GetNormalFromDepth(float fDepth) 
{ 
  	float depth1 = SampleDepthOffset(int2(0, 1));
  	float depth2 = SampleDepthOffset(int2(1, 0));
	float2 invres = GetInvResolution();
  	float3 p1 = float3(0,invres.y, depth1 - fDepth);
  	float3 p2 = float3(invres.x, 0, depth2 - fDepth);
  
  	float3 normal = cross(p1, p2);
  	normal.z = -normal.z;
  
  	return normalize(normal);
}
#define FILTER_RADIUS 2
void BlurH()
{
	const float4 zero = float4(0,0,0,0);
	float depth = SampleDepth();	
	float limit = GetOption(FILTER_LIMIT);
	float Weight = pow(2, FILTER_RADIUS);
	float count = Weight;
	float4 value = SamplePrev() * Weight;
	for(int i = 1; i < (FILTER_RADIUS + 1); i++)
	{
		Weight *= 0.5;
		int2 offset = int2(i, 0);
		float localWeight = min(sign(limit - abs(SampleDepthOffset(offset) - depth)) + 1.0, 1.0) * Weight;
		value +=  SamplePrevOffset(offset) * localWeight;
		count += localWeight;
		offset = -offset;
		localWeight = min(sign(limit - abs(SampleDepthOffset(offset) - depth)) + 1.0, 1.0) * Weight;
		value +=  SamplePrevOffset(offset) * localWeight;
		count += localWeight;
	}
	value = value / count;	 
	SetOutput(value);
}

void Merger()
{
	const float4 zero = float4(0,0,0,0);
	float depth = SampleDepth();
	float limit = GetOption(FILTER_LIMIT);
	float Weight = pow(2, FILTER_RADIUS);
	float count = Weight;
	float4 value = SamplePrev() * Weight;	
	for(int i = 1; i < (FILTER_RADIUS + 1); i++)
	{
		Weight *= 0.5;
		int2 offset = int2(i, 0);
		float localWeight = min(sign(limit - abs(SampleDepthOffset(offset) - depth)) + 1.0, 1.0) * Weight;
		value +=  SamplePrevOffset(offset) * localWeight;
		count += localWeight;
		offset = -offset;
		localWeight = min(sign(limit - abs(SampleDepthOffset(offset) - depth)) + 1.0, 1.0) * Weight;
		value +=  SamplePrevOffset(offset) * localWeight;
		count += localWeight;
	}
	value = value / count;	 
	SetOutput(value * Sample());
}

void SSAO()
{
	float3 PoissonDisc[] = {
        float3(-0.367046f, 0.692618f, 0.0136723f),
        float3(0.262978f, -0.363506f, 0.231819f),
        float3(-0.734306f, -0.451643f, 0.264779f),
        float3(-0.532456f, 0.683096f, 0.552049f),
        float3(0.672536f, 0.283731f, 0.0694296f),
        float3(-0.194678f, 0.548204f, 0.56859f),
        float3(-0.87347f, -0.572741f, 0.923795f),
        float3(0.548936f, -0.717277f, 0.0201727f),
        float3(0.48381f, 0.691397f, 0.699088f),
        float3(-0.592273f, 0.41966f, 0.413953f),
        float3(-0.448042f, -0.957396f, 0.123234f),
        float3(-0.618458f, 0.112949f, 0.412946f),
        float3(-0.412763f, 0.122227f, 0.732078f),
        float3(0.816462f, -0.900815f, 0.741417f),
        float3(-0.0381787f, 0.511521f, 0.799768f),
        float3(-0.688284f, 0.310099f, 0.472732f),
        float3(-0.368023f, 0.720572f, 0.544206f),
        float3(-0.379192f, -0.55504f, 0.035371f),
        float3(0.15482f, 0.0353709f, 0.543779f),
        float3(0.153417f, -0.521409f, 0.943724f),
        float3(-0.168371f, -0.702933f, 0.145665f),
        float3(-0.673391f, -0.925657f, 0.61391f),
        float3(-0.479171f, -0.131993f, 0.659932f),
        float3(0.0549638f, -0.470809f, 0.420759f),
        float3(0.899594f, 0.955077f, 0.54857f),
        float3(-0.230689f, 0.660573f, 0.548112f),
        float3(0.0421461f, -0.19895f, 0.121799f),
        float3(-0.229774f, -0.30137f, 0.507492f),
        float3(-0.983642f, 0.468551f, 0.0393994f),
        float3(-0.00857568f, 0.440657f, 0.337046f),
        float3(0.730461f, -0.283914f, 0.789941f),
        float3(0.271828f, -0.226356f, 0.317026f),
        float3(-0.178869f, -0.946837f, 0.073336f),
        float3(0.389813f, -0.110508f, 0.0549944f),
        float3(0.0242622f, 0.893613f, 0.26957f),
        float3(-0.857601f, 0.0219429f, 0.45146f),
        float3(-0.15659f, 0.550401f, 3.05185e-005f),
        float3(0.0555742f, -0.354656f, 0.573412f),
        float3(-0.267373f, 0.117466f, 0.488571f),
        float3(-0.533799f, -0.431928f, 0.226661f),
        float3(0.49852f, -0.750908f, 0.412427f),
        float3(-0.300882f, 0.366314f, 0.558245f),
        float3(-0.176f, 0.511521f, 0.722465f),
        float3(-0.0514847f, -0.703543f, 0.180273f),
        float3(-0.429914f, 0.0774255f, 0.161534f),
        float3(-0.416791f, -0.788385f, 0.328135f),
        float3(0.127293f, -0.115146f, 0.958586f),
        float3(-0.34959f, -0.278481f, 0.168706f),
        float3(-0.645192f, 0.168798f, 0.577105f),
        float3(-0.190771f, -0.622669f, 0.257851f),
        float3(0.718986f, -0.275369f, 0.602039f),
        float3(-0.444258f, -0.872982f, 0.0275582f),
        float3(0.793512f, 0.0511185f, 0.33964f),
        float3(-0.143651f, 0.155614f, 0.368877f),
        float3(-0.777093f, 0.246864f, 0.290628f),
        float3(0.202979f, -0.61742f, 0.233802f),
        float3(0.198523f, 0.425153f, 0.409162f),
        float3(-0.629688f, 0.597461f, 0.120212f),
        float3(0.0448316f, -0.689566f, 0.0241707f),
        float3(-0.190039f, 0.426496f, 0.254463f),
        float3(-0.255776f, 0.722831f, 0.527451f),
        float3(-0.821528f, 0.303751f, 0.140172f),
        float3(0.696646f, 0.168981f, 0.404492f),
        float3(-0.240211f, -0.109653f, 0.463301f),
};


	float2 coords = GetCoordinates();
	float fCurrDepth = SampleDepth();
	float4 Occlusion = float4(1.0,1.0,1.0,1.0);
	if(fCurrDepth<0.9999) 
	{
		float3 vViewNormal = GetNormalFromDepth(fCurrDepth);
		Randomize();
		uint rndidx = uint(RandomSeedfloat(GetCoordinates()) * 8.0);
		float3 vRandom =  normalize(PoissonDisc[rndidx]);
		float fAO = 0;
		for(int s = 0; s < NUMSAMPLES; s++) 
		{
			float3 offset = PoissonDisc[s];
			float3 vReflRay = reflect(offset, vRandom);
		
			float fFlip = sign(dot(vViewNormal,vReflRay));
        	vReflRay   *= fFlip;
		
			float sD = fCurrDepth - (vReflRay.z * GetOption(SAMPLE_RANGE));
			float fSampleDepth = SampleDepthLocation(saturate(coords + (GetOption(SAMPLE_RANGE) * vReflRay.xy / fCurrDepth)));
			float fDepthDelta = saturate(sD - fSampleDepth);

			fDepthDelta *= 1-smoothstep(0,GetOption(MAX_DEPTH),fDepthDelta);

			if ( fDepthDelta > GetOption(MIN_DEPTH) && fDepthDelta < GetOption(MAX_DEPTH))
				fAO += pow(1 - fDepthDelta, 2.5);
		}
		fAO = saturate(1 - (fAO / float(NUMSAMPLES)) + GetOption(SAMPLE_RANGE));
		Occlusion = float4(fAO,fAO,fAO,fCurrDepth);
	}
	SetOutput(Occlusion);
}