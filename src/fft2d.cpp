/*
 * fft.cpp
 *
 *  Created on: Apr 2, 2014
 *      Author: kcratie
 *
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
*/

#include <omp.h>
#include "fft2d.h"

using namespace std;

namespace PCAkern {

fft2d::fft2d(
		IIOAgent * IoAgent) :
		mIoAgent(IoAgent)
{
}

fft2d::~fft2d()
{
}



int
fft2d::ApplyTransform(
	pixel_t Buf[],
	size_t Count)
{
	int status = 0;
	size_t NumRows, NumCols;
	NumRows = NumCols = (1<<mConfig.DimPow);
	mResultSet = mIoAgent->GetFftOuptBuffer();

	fftw_plan plan_backward;
	fftw_plan plan_forward;

	plan_forward = fftw_plan_dft_r2c_2d(NumRows, NumCols, Buf, mResultSet, FFTW_MEASURE);
	fftw_execute(plan_forward);

	fftw_destroy_plan(plan_forward);

	return status;

}

int
fft2d::Run()
{

	int status = 0;


	try
	{
		size_t count = 0;
		pixel_t * dataset = mIoAgent->GetDataset(count);

		status = fftw_init_threads();
		if(status!=1)
			throw logic_error("Failed to the set number of threads for fft plan");
		//omp_get_max_threads()
		fftw_plan_with_nthreads(mConfig.NumProcs);

		ApplyTransform(dataset, count);

	}
	catch(exception &e)
	{
		status = -1;
	}
	return status;
}

void
fft2d::SetCurrentConfig(
		CurrentConfig & Config)
{
	mConfig=Config;
}

fft2d* Createfft2d(
		IIOAgent * IoAgent)
{
	return new fft2d(IoAgent);
}

}//end namespace PCAKern
