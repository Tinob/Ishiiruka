// Simple Depth

void main()
{
	float depth = SampleDepth();
	SetOutput(float4(depth,depth,depth,1.0));
}