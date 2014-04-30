/*
 * AutoCorl.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: kcratie
 */
#include <omp.h>
#include "AutoCorl.h"

namespace PCAkern {

AutoCorrelation::AutoCorrelation(
		IIOAgent * IoAgent) :
		mIoAgent(IoAgent){}

AutoCorrelation::~AutoCorrelation()
{}

int
AutoCorrelation::ApplyTransform(
	pixel_t const Buf[],
	size_t const Count)
{
	int status = 0;
	size_t N = 1<<mConfig.DimPow;
	pixel_t x=N;
	//size_t chunk = mConfig.DimPow;
	omp_set_num_threads(mConfig.NumProcs);
	//#pragma omp parallel for schedule(dynamic, chunk) firstprivate(x)
	#pragma omp parallel for schedule(static) firstprivate(x)
	for(size_t k=0;k<N;k++)
	{
		pixel_t y=N;
		for(size_t l=0;l<N;l++)
		{
			pixel_t temp=0;
			for(size_t i=0;i<N-k;i++)
			{
				for(size_t j=0;j<N-l;j++)
				{
					temp= temp + Buf[(i*N)+j]*Buf[((i+k)*N)+(j+l)];
				}
			}
			mOutput[(k*N)+l]= temp/(x*y);
			y--;
		}
		x--;
	}
	return status;
}

int
AutoCorrelation::Run()
{
	int status = 0;
/*	try
	{*/
		size_t count = 0;

		pixel_t * dataset = mIoAgent->GetDataset(count);
		mOutput = mIoAgent->GetOuptBuffer();

		status = ApplyTransform(dataset, count);

/*	}
	catch(exception &e)
	{
		status = -1;
	}*/

	return status;
}

void
AutoCorrelation::SetCurrentConfig(
		CurrentConfig & Config)
{
	mConfig=Config;
}

AutoCorrelation* CreateAutoCorl(
		IIOAgent * IoAgent)
{
	return new AutoCorrelation(IoAgent);
}


}
