/*
 * PerfMon.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: kcratie
 */


#include <time.h>
#include <map>
#include <memory>
#include <sstream>
#include <omp.h>
#include "PerfMon.h"

using namespace std;

namespace PCAkern {


PerfMon::PerfMon():
		mTimerStart(0),
		mTimerEnd(0)
{}

PerfMon::~PerfMon()
{
	vector <PMonEntry*>::iterator vitr = mPerfEntries.begin();
	for(; vitr!=mPerfEntries.end(); vitr++)
	{
		delete *vitr;
	}
}

string
PerfMon::Timestamp(void)
{
# define TIME_SIZE 40

	static char time_buffer[TIME_SIZE];
	const struct tm *tm;
	size_t len;
	time_t now;

	now = time(NULL);
	tm = localtime(&now);

	len = strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);

	return string(time_buffer);
# undef TIME_SIZE
}

void
PerfMon::StartTimer()
{
	mTimerStart = omp_get_wtime();
	//mTimerStart = timerval();
}

void
PerfMon::StopTimer()
{
	mTimerEnd = omp_get_wtime();
	//mTimerEnd = timerval();

}

double
PerfMon::GetDuration()
{
	if (mTimerEnd <= mTimerStart)
		return 0;
	return mTimerEnd - mTimerStart;
}

PMonHeader &
PerfMon::AddHeader(
		const string & KernelName,
		size_t NumIterations)
{
	mPerfHeader.KernelName = KernelName;
	mPerfHeader.Timestamp = Timestamp();
	mPerfHeader.NumIterations = NumIterations;
	return mPerfHeader;
}

PMonEntry &
PerfMon::AddMtericsEntry(
		size_t NumProcs,
		size_t Dimensions)
{
	auto_ptr<PMonEntry> pme(new PMonEntry);
	pme->NumProcs = NumProcs;
	pme->Dimensions = Dimensions;
	pme->Duration = GetDuration();
	mPerfEntries.push_back(pme.get());
	return *pme.release();
}

void
PerfMon::Commit(
	const string & Filename)
{
	mOStr.open(Filename.c_str());
	if(!mOStr.is_open())
	{
		string emsg("Could not open metrics file:");
		emsg.append(Filename);
		throw logic_error(emsg);
	}
	WriteHeader();
	WriteMetrics();
	WriteSummary("Reflectance");
	mOStr.close();
}

void
PerfMon::WriteHeader()
{

  if (mOStr.is_open())
  {
	  mOStr<<"Benchmark,"<<mPerfHeader.KernelName<<",";
	  mOStr<<mPerfHeader.Timestamp<<",\n";
	  mOStr<<"Number of Iterations,"<<mPerfHeader.NumIterations<<",\n";
	  //mOStr<<"Number of threads,\n";

  }
  else
  {
	string emsg("Metrics file not opened.");
	throw logic_error(emsg);
  }
}

void
PerfMon::WriteMetrics()
{
	size_t nprocs = 0;
	string nthreads("Number of threads,");
	map<size_t, string> printable;
	  if (mOStr.is_open())
	  {
		  vector <PMonEntry*>::iterator vitr = mPerfEntries.begin();
		  for(; vitr!=mPerfEntries.end(); vitr++)
		  {	//build each row of the output storing in a map
			  string & row = printable[(*vitr)->Dimensions];
			  if (row.length()==0)
			  {
				  row.append(DurationToString((*vitr)->Dimensions)).append(",");
			  }
			  if(nprocs<(*vitr)->NumProcs)
			  {
				  nprocs=(*vitr)->NumProcs;
				  nthreads.append(IntToString(nprocs)).append(",");
			  }
			  row.append(DurationToString((*vitr)->Duration));
			  row.append(",");
		  }
		  nthreads.append("\nDataset N,");
		  printable[0]=nthreads;
		  map<size_t, string>::iterator mitr=printable.begin();
		  for(; mitr!=printable.end(); mitr++)
		  {	//print all the rows in the map
			  string & line=mitr->second;
			  mOStr<<line.c_str()<<"\n";
		  }
	  }
	  else
	  {
		string emsg("Reflection metrics file not opend");
		throw logic_error(emsg);
	  }
}

void
PerfMon::WriteSummary(const string & Summary)
{
	if (mOStr.is_open())
	{
		mOStr<<"End of Benchmark,"<<Summary<<","<<Timestamp()<<",\n";
	}
	else
	{
		string emsg("Reflection metrics file not opened");
		throw logic_error(emsg);
	}
	return;
}



string
PerfMon::DurationToString(
		double number)
{
   stringstream ss;
   ss << number;
   return ss.str();
}

string
PerfMon::IntToString(
		size_t number)
{
   stringstream ss;
   ss << number;
   return ss.str();
}

PerfMon * CreatePerfMon()
{
	return new PerfMon;
}


}//end namespace PCAkern
