/*
 * Supervisor.cpp
 *
 *  Created on: Mar 25, 2014
 *      Author: kcratie
 */
#include <exception>
#include <iostream>
#include <memory>
#include <omp.h>
#include "PerfMon.h"
#include "Supervisor.h"
#include <math.h>

using namespace std;

namespace PCAkern {

#define dbgprnt																												\
{																															\
	static size_t stage=1,																											\
			numstages=(mConfig.MaxNumProcs-mConfig.MinNumProcs+1)*(mConfig.MaxDimPow-mConfig.MinDimPow+1);					\
	double sz=pow(1<<dpow, 2);																								\
	cout<<"completed stage " <<stage++ <<" of "<<numstages <<", " <<sz <<" elements in :" << pm->GetDuration()<< " secs\n";	\
}

Supervisor::Supervisor(const string & ConfigFile):
		mConfigFile(ConfigFile)
{}

Supervisor::~Supervisor() {}


void
Supervisor::RunRefl()
{
	try
	{
		string outfile = mConfig.OutputFilename;
		outfile.append("Refl").append(".csv");

		auto_ptr<IIOAgent> ioa (CreateIOAgent());
		auto_ptr<Reflectance> refl (CreateReflectance(ioa.get()));
		auto_ptr<PerfMon> pm (CreatePerfMon());
		pm->AddHeader("Reflectance", mConfig.NumItrs);

		for(size_t dpow=mConfig.MinDimPow; dpow<=mConfig.MaxDimPow; dpow++)
		{
			ioa->InitDataset(dpow);

			for(size_t nprocs=mConfig.MinNumProcs; nprocs<=mConfig.MaxNumProcs; nprocs++)
			{
				CurrentConfig cc(nprocs,
								dpow,
								mConfig.NumItrs,
								Refl, outfile);
				refl->SetCurrentConfig(cc);

				pm->StartTimer();
				for(size_t itr=0; itr<mConfig.NumItrs; itr++)
					refl->Run();

				pm->StopTimer();
				pm->AddMtericsEntry(cc.NumProcs, 1<<cc.DimPow);
				//dbgprnt;
			}
		}
		pm->Commit(outfile);
	}
	catch(exception &e)
	{
		cout<<"Reflectance benchmark failed with exception: "<<e.what()<<"\n";
	}
	return;
}

void
Supervisor::RunFFT()
{
	try{
		string outfile = mConfig.OutputFilename;
		outfile.append("FFT2D").append(".csv");

		auto_ptr<IIOAgent> ioa (CreateIOAgent(FFT));
		auto_ptr<fft2d> fft (Createfft2d(ioa.get()));
		auto_ptr<PerfMon> pm (CreatePerfMon());
		pm->AddHeader("FFT2D", mConfig.NumItrs);

		for(size_t dpow=mConfig.MinDimPow; dpow<=mConfig.MaxDimPow; dpow++)
		{
			ioa->InitDataset(dpow);

			for(size_t nprocs=mConfig.MinNumProcs; nprocs<=mConfig.MaxNumProcs; nprocs++)
			{
				CurrentConfig cc(nprocs,
								dpow,
								mConfig.NumItrs,
								FFT, outfile);
				fft->SetCurrentConfig(cc);
				fft->BuildPlan();	//exclude the plan time as it does not scale. The plan can be reused for different input but of equivalent dimensions.
				pm->StartTimer();
				for(size_t itr=0; itr<mConfig.NumItrs; itr++)
					fft->Run();

				pm->StopTimer();
				pm->AddMtericsEntry(cc.NumProcs, 1<<cc.DimPow);
				//dbgprnt;
			}
		}
		pm->Commit(outfile);
	}
	catch(exception &e)
	{
		cout<<"FFT benchmark failed with exception: "<<e.what()<<"\n";
	}
	return;
}

void
Supervisor::RunAutoCorl()
{
	try{
		string outfile = mConfig.OutputFilename;
		outfile.append("AutoCorl").append(".csv");

		auto_ptr<IIOAgent> ioa (CreateIOAgent());
		auto_ptr<AutoCorrelation> ac (CreateAutoCorl(ioa.get()));
		auto_ptr<PerfMon> pm (CreatePerfMon());
		pm->AddHeader("AutoCorl", mConfig.NumItrs);

		for(size_t dpow=mConfig.MinDimPow; dpow<=mConfig.MaxDimPow; dpow++)
		{
			ioa->InitDataset(dpow);

			for(size_t nprocs=mConfig.MinNumProcs; nprocs<=mConfig.MaxNumProcs; nprocs++)
			{
				CurrentConfig cc(nprocs,
								dpow,
								mConfig.NumItrs,
								AutoCorl, outfile);
				ac->SetCurrentConfig(cc);

				pm->StartTimer();
				for(size_t itr=0; itr<mConfig.NumItrs; itr++)
					ac->Run();

				pm->StopTimer();
				pm->AddMtericsEntry(cc.NumProcs, 1<<cc.DimPow);
				//dbgprnt;
			}
		}
		pm->Commit(outfile);
	}
	catch(exception &e)
	{
		cout<<"Auto corelation benchmark failed with exception: "<<e.what()<<"\n";
	}
	return;
}

void
Supervisor::RunSpAvg()
{
	try{
		string outfile = mConfig.OutputFilename;
		outfile.append("SpAvg").append(".csv");

		auto_ptr<IIOAgent> ioa (CreateIOAgent());
		auto_ptr<SpatialAvg> SplAvg (CreateSpatialAvg(ioa.get()));
		auto_ptr<PerfMon> pm (CreatePerfMon());
		pm->AddHeader("SpAvg", mConfig.NumItrs);

		for(size_t dpow=mConfig.MinDimPow; dpow<=mConfig.MaxDimPow; dpow++)
		{
			ioa->InitDataset(dpow);

			for(size_t nprocs=mConfig.MinNumProcs; nprocs<=mConfig.MaxNumProcs; nprocs++)
			{
				CurrentConfig cc(nprocs,
								dpow,
								mConfig.NumItrs,
								SpAvg, outfile);
				SplAvg->SetCurrentConfig(cc);

				pm->StartTimer();
				for(size_t itr=0; itr<mConfig.NumItrs; itr++)
					SplAvg->Run();

				pm->StopTimer();
				pm->AddMtericsEntry(cc.NumProcs, 1<<cc.DimPow);
				//dbgprnt;
			}
		}
		pm->Commit(outfile);
	}
	catch(exception &e)
	{
		cout<<"Spatial averaging benchmark failed with exception: "<<e.what()<<"\n";
	}

	return;
}

void
Supervisor::RunBenchmark()
{
	mConfig.LoadConfig(mConfigFile);

	if(0 == mConfig.BenchmarkID.compare(string("refl")))
	{
		RunBenchmark(Refl);
	}
	else if (0 == mConfig.BenchmarkID.compare(string("fft")))
	{
		RunBenchmark(FFT);
	}
	else if (0 == mConfig.BenchmarkID.compare(string("autocorl")))
	{
		RunBenchmark(AutoCorl);
	}
	else if (0 == mConfig.BenchmarkID.compare(string("spavg")))
	{
		RunBenchmark(SpAvg);
	}
	else if (0 == mConfig.BenchmarkID.compare(string("all")))
	{
		RunBenchmark(All);
	}
	else
		cout<<"invalid parameter\n";
}

void
Supervisor::RunBenchmark(BenchmarkID_t BenchmarkID)
{

	switch(BenchmarkID)
	{
	case Refl:
		RunRefl();
		break;
	case FFT:
		RunFFT();
		break;
	case AutoCorl:
		RunAutoCorl();
		break;
	case SpAvg:
		RunSpAvg();
		break;
	case All:
	default:
		RunRefl();
		RunFFT();
		RunAutoCorl();
		RunSpAvg();
		break;
	}
	return;
}

}
