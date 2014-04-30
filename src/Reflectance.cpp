//============================================================================
// Name        : Reflectance.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
//#include <stdio.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <sys/stat.h>
//#include <iostream>
#include <omp.h>
#include "Reflectance.h"


using namespace std;

namespace PCAkern
{

//TODO fix dummy values
const pixel_t Reflectance::A = 0.105;
const pixel_t Reflectance::B = 0.23;
const pixel_t Reflectance::S = 0.43;
const pixel_t Reflectance::L = 0.47;
const pixel_t Reflectance::La = 0.95;

const int Reflectance::MAX_THREADS = 4;

Reflectance::Reflectance(
		IIOAgent * IoAgent) :
		mIoAgent(IoAgent)
{}

Reflectance::~Reflectance(){}

/*
 * ApplyTransform()
 * The transformation is done is parallel. The buffer is
 * partitioned among the available threads and each one
 * processes its respective segment.
 * Transform[Input] is the function that performs per pixel
 * processing.
 * Buf[Input/Output] is expected to be at least elemsz*count
 * bytes in length. The transformation is done in place and
 * bytes zero through elemsz*count-1 will be modified on return.
 * Count[Input] is the number of elements in the Buf array.
 */
int
Reflectance::ApplyTransform(
	pixel_t Buf[],
	size_t Count)
{
	int status = 0;
	omp_set_num_threads(mConfig.NumProcs);
	//#pragma omp parallel for schedule(static)
	#pragma omp parallel for schedule(dynamic, 1<<mConfig.DimPow)
	for (size_t i = 0; i < Count; i++)
	{
		Refl(Buf[i]);
	}
	return status;
}

/*
 * Run()
 * Pulls a data buffer from work queue. Buffer will be
 * large enough to be split for all processing threads.
 */
int
Reflectance::Run()
{
	int status = 0;
	//try
	//{
		size_t count = 0;
		pixel_t * dataset = mIoAgent->GetDataset(count);


		status = ApplyTransform(dataset, count);
/*	catch(exception &e)
	{
		status = -1;
	}*/
	return status;
}

void
Reflectance::SetCurrentConfig(
		CurrentConfig & Config)
{
	mConfig=Config;
}


/*
 * ClassFactory
 */
Reflectance* CreateReflectance(
		IIOAgent * IoAgent)
{
	Reflectance * refl = new Reflectance(IoAgent);
	return refl;
}

} //end namespace PCAkern


