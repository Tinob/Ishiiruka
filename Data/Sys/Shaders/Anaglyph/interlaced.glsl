// Simple interlaced of 2 layers

void main()
{
	SetOutput(SampleLayer(int(GetFragmentCoord().y) & 1));
}
