#include "XorShift.h"



XorShift::XorShift()
{
	SeedNoSet = true;
}


XorShift::~XorShift()
{
}



unsigned int XorShift::GetValue()
{
	if (SeedNoSet) setSeed();

	unsigned int t = x ^ (x << 11);
	x = y;
	y = z;
	z = w;
	w = w ^ (w >> 19) ^ t ^ (t >> 8);

	return w;
}

void XorShift::setSeed()
{
	x = 548787455;
	y = 842502087;
	z = 3579807591;
	w = 273326509;
	SeedNoSet = false;
}