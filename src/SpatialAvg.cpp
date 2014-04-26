/*
 * SpatialAvg.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: kcratie
 */


#include <omp.h>
#include "SpatialAvg.h"

using namespace std;

namespace PCAkern {

int mLPFilter[3][3] =  { {1, 2, 1},
						 {2, 4, 2},
						 {1, 2, 1}};
SpatialAvg::SpatialAvg(
		IIOAgent * IoAgent) :
		mIoAgent(IoAgent),
		mOutput(NULL)
{}

SpatialAvg::~SpatialAvg()
{}

inline void
SpatialAvg::mask_operation(
		pixel_t * InAr,
		size_t i,
		size_t j)
{

	const size_t N = 1<<mConfig.DimPow;
	size_t idx=(i*N+j);
	mOutput[i*N+j]=( (InAr[idx]*mLPFilter[1][1]) 		//middle
				+ (InAr[idx-1]*mLPFilter[1][0]) 		//mid left
				+ (InAr[idx+1]*mLPFilter[1][2]) 		//mid right
				+ (InAr[idx-N-1]*mLPFilter[0][0]) 		//upper left
				+ (InAr[idx-N]*mLPFilter[0][1]) 		//mid upper
				+ (InAr[idx-N+1]*mLPFilter[0][1]) 		//upper right
				+ (InAr[idx+N-1]*mLPFilter[2][0]) 		//lower left
				+ (InAr[idx+N]*mLPFilter[2][1]) 		// mid lower
				+ (InAr[idx+N+1]*mLPFilter[2][2]) )	// lower right
			/ 9;
}



int
SpatialAvg::ApplyTransform(
	pixel_t Buf[],
	size_t Count)
{
	int status = 0;
	size_t NumRows, NumCols;
	NumRows = NumCols = 1<<mConfig.DimPow;
	/* TODO:
	    //copying the first& last rows & columns directly
	    if(i == 0 || j == 0 || i == rows_available-1 || j == N-1){
	        Ap[i*N+j]= A[i*N+j];
	    }
	*/
	omp_set_num_threads(mConfig.NumProcs);
	#pragma omp parallel for schedule(static)
	//Performing the operation
	for(size_t i = 1; i < NumRows-1; i++)
	{
		for(size_t j = 1; j < NumCols-1; j++)
		{
			mask_operation(Buf, i, j);

		}
	}

	return status;
}

int
SpatialAvg::Run()
{

	int status = 0;
/*	try
	{*/
		size_t count = 0;
		pixel_t * dataset = mIoAgent->GetDataset(count);
		mOutput = mIoAgent->GetOuptBuffer();

		ApplyTransform(dataset, count);
/*	}
	catch(exception &e)
	{
		status = -1;
	}*/
	return status;
}

void
SpatialAvg::SetCurrentConfig(
		CurrentConfig & Config)
{
	mConfig=Config;
}

SpatialAvg* CreateSpatialAvg(
		IIOAgent * IoAgent)
{
	return new SpatialAvg(IoAgent);
}

}

