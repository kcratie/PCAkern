/*
 * IOAgent.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: kcratie
 */

#include "IOAgent.h"

using namespace std;

namespace PCAkern {

IOAgent::IOAgent():
	mDimPower(0),
	mBuf(NULL)
{
	srand(123456789);
}

IOAgent::~IOAgent()
{
	delete mBuf;
}


void
IOAgent::InitDataset(size_t DimPower)
{
	mDimPower = DimPower;
	delete mBuf;
	size_t N = 1<<mDimPower;
	N=N*N;
	mBuf = new pixel_t[N];

	for (size_t i = 0; i < N; i++)
	{
		mBuf[i] = frand();
	}
	return;
}

pixel_t *
IOAgent::GetDataset(size_t & Count)
{
	Count = 1<<mDimPower;
	Count=Count*Count;
	return mBuf;
}

double
IOAgent::frand(void)
{
	double value;

	value = ((double) rand() / (RAND_MAX));

	return value;
}


IIOAgent *
CreateIOAgent()
{

	return new IOAgent();
}



}	//end namespace PCAkern
