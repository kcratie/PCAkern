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
	/*
	 Create the output array OUT, which is of type FFTW_COMPLEX,
	 and of a size NX * NYH that is roughly half the dimension of the input data
	 (ignoring the fact that the input data is real, and the FFT
	 coefficients are complex).
	 */
	size_t NumRows, NumCols;
	NumRows = NumCols = (1<<mConfig.DimPow);
	size_t nyh = (NumRows / 2) + 1;
	mResultSet = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * NumCols * nyh);
	mReverseSet = new pixel_t[NumRows * NumCols];
}

fft2d::~fft2d()
{
	fftw_free(mResultSet);
	delete mReverseSet;
}



int
fft2d::ApplyTransform(
	pixel_t Buf[],
	size_t Count)
{
	int status = 0;
	size_t NumRows, NumCols;
	NumRows = NumCols = (1<<mConfig.DimPow);

	/*
	pixel_t *in;
	pixel_t *in2;
	int j;
	int nx = 500;
	int ny = 500;
	int nyh;
	double begin, end;
	in = Buf;
	in2 = mReverseSet;
	*/

	fftw_plan plan_backward;
	fftw_plan plan_forward;

	plan_forward = fftw_plan_dft_r2c_2d(NumRows, NumCols, Buf, mResultSet, FFTW_MEASURE);
	fftw_execute(plan_forward);

	// Recreate the input array.
	plan_backward = fftw_plan_dft_c2r_2d(NumRows, NumCols, mResultSet, mReverseSet, FFTW_ESTIMATE);
	fftw_execute(plan_backward);

	fftw_destroy_plan(plan_forward);
	fftw_destroy_plan(plan_backward);

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
