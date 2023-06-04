#pragma once
class XorShift
{
public:
	XorShift();
	~XorShift();

	unsigned int  GetValue();

 private:
	 unsigned int x, y, z, w;
	 bool SeedNoSet;
	 void setSeed();


};

