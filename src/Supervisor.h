/*
 * Supervisor.h
 *
 *  Created on: Mar 25, 2014
 *      Author: kcratie
 */

#ifndef SUPERVISOR_H_
#define SUPERVISOR_H_

#include "IOAgent.h"
#include "Configuration.h"
#include "Reflectance.h"
#include "fft2d.h"
#include "SpatialAvg.h"
#include "AutoCorl.h"

namespace PCAkern
{

class Supervisor
{
public:
	Supervisor(const string & ConfigFile);
	virtual ~Supervisor();
	void RunBenchmark(BenchmarkID_t BenchmarkID);
	void RunBenchmark();

private:
	//IIOAgent mIO;
	//Reflectance mReflectance;
	//fft2d mFft;
	//SpatialAvg mSpAvg;
	string mConfigFile;
	Configuration mConfig;

	void RunRefl(
		);

	void RunFFT(
		);

	void RunAutoCorl(
		);

	void RunSpAvg(
		);

};

}

#endif /* SUPERVISOR_H_ */
