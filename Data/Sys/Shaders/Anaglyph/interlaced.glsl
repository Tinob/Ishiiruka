// Simple interlaced of 2 layers

void main()
{
	SetOutput(SampleLayer(int(gl_FragCoord.y) & 1));
}
