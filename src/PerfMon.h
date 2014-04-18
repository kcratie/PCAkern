/*
 * PerfMon.h
 *
 *  Created on: Apr 6, 2014
 *      Author: kcratie
 */

#ifndef PERFMON_H_
#define PERFMON_H_

#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include <vector>
#include "Configuration.h"

using namespace std;

namespace PCAkern {

struct PMonHeader {
	string KernelName;
	string Timestamp;
	size_t NumIterations;
};

struct PMonEntry {
	PMonEntry(){};
	~PMonEntry(){*(char*)0;}//test
	size_t Dimensions;
	size_t NumProcs;
	double Duration;
};

class PerfMon {
public:
	PerfMon();
	virtual ~PerfMon();
	void StartTimer();
	void StopTimer();
	double GetDuration();


	PMonHeader & AddHeader(
			const string & KernelName,
			size_t NumIterations
			);
	PMonEntry & AddMtericsEntry(
			size_t NumProcs,
			size_t Dimensions
			);
	void Commit(
			const string & Filename
			);

protected:
	double timerval() {							\
		struct timeval st;						\
		gettimeofday(&st, NULL);				\
		return st.tv_sec + st.tv_usec * 1e-6;	\
	}
	string Timestamp(
				);
	void WriteHeader(
			);
	void WriteMetrics(
			);
	void WriteSummary(
			const string & Summary
			);
	string DurationToString(
			double number
			);
	string IntToString(
			size_t number
			);
	PMonHeader mPerfHeader;
	vector <PMonEntry*> mPerfEntries;
	double mTimerStart, mTimerEnd;
	ofstream mOStr;
};

PerfMon * CreatePerfMon();

}

#endif /* PERFMON_H_ */
