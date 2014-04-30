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
		mIoAgent(IoAgent),
		mDataset(NULL),
		mResultSet(NULL),
		isaPlan(false)
{}

fft2d::~fft2d()
{
	if(isaPlan)
	{
		fftw_destroy_plan(plan_forward);
	}
}

void
fft2d::BuildPlan()
{
	size_t count = 0;
	mDataset = mIoAgent->GetDataset(count);
	mResultSet = mIoAgent->GetFftOuptBuffer();

	int status = fftw_init_threads();
	if(status!=1)
		throw logic_error("Failed to the set number of threads for fft plan");

	if(isaPlan)
	{
		fftw_destroy_plan(plan_forward);
	}
	fftw_plan_with_nthreads(mConfig.NumProcs);
	isaPlan = true;

	size_t NumRows, NumCols;
	NumRows = NumCols = (1<<mConfig.DimPow);



	plan_forward = fftw_plan_dft_r2c_2d(NumRows, NumCols, mDataset, mResultSet, FFTW_MEASURE);

	return;
}

int
fft2d::Run()
{

	int status = 0;

/*	try
	{
	status = ApplyTransform(mDataset, count);
	}
*/
	fftw_execute(plan_forward);
/*	catch(exception &e)
	{
		status = -1;
	}*/
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
