/*
 * AutoCorl.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: kcratie
 */

#include "AutoCorl.h"

namespace PCAkern {

AutoCorrelation::AutoCorrelation(
		IIOAgent * IoAgent) :
		mIoAgent(IoAgent){}

AutoCorrelation::~AutoCorrelation()
{}

int
AutoCorrelation::ApplyTransform(
	pixel_t Buf[],
	size_t Count)
{
	int status = 0;
	return status;
}

int
AutoCorrelation::Run()
{
	int status = 0;
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
