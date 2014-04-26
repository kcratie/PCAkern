/*
 * IOAgent.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: kcratie
 */

#include "IOAgent.h"

using namespace std;

namespace PCAkern {

IOAgent::IOAgent(
		BenchmarkID_t Benchmark = All):
	mDimPower(0),
	mReadBuf(NULL),
	mWriteBuf(NULL),
	mFftWriteBuf(NULL),
	mBenchmark(Benchmark)
{
	srand(123456789);
}

IOAgent::~IOAgent()
{
	delete mReadBuf;
	delete mWriteBuf;
	if(mFftWriteBuf)
		fftw_free(mFftWriteBuf);
}

size_t
IOAgent::AllocBuffers()
{
	delete mReadBuf;
	delete mWriteBuf;
	if(mFftWriteBuf)
		fftw_free(mFftWriteBuf);

	size_t N = 1<<mDimPower;
	size_t M=N*N;
	mReadBuf = new pixel_t[M];

	switch (mBenchmark)
	{
	case FFT:
		/*
		 Create the output array OUT, which is of type FFTW_COMPLEX, and of a size
		 NX * NYH that is roughly half the dimension of the input data (ignoring
		 the fact that the input data is real, and the FFT coefficients are complex).
		 */
		mFftWriteBuf = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N * (N/2+1));
		break;
	default:
		mWriteBuf = new pixel_t[M];
		break;
	}

	return M;
}

void
IOAgent::InitDataset(
		size_t DimPower)
{
	mDimPower = DimPower;
	size_t N = AllocBuffers();
	for (size_t i = 0; i < N; i++)
	{
		mReadBuf[i] = frand();
	}
	return;
}

pixel_t *
IOAgent::GetDataset(
		size_t & Count)
{
	Count = 1<<mDimPower;
	Count=Count*Count;
	return mReadBuf;
}

pixel_t *
IOAgent::GetOuptBuffer()
{
	return mWriteBuf;
}

fftw_complex *
IOAgent::GetFftOuptBuffer()
{
	return mFftWriteBuf;
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

IIOAgent *
CreateIOAgent(BenchmarkID_t Benchmark)
{
	return new IOAgent(Benchmark);
}


}	//end namespace PCAkern
