/*
 * IOAgent.h
 *
 *  Created on: Apr 6, 2014
 *      Author: kcratie
 */

#ifndef IOAGENT_H_
#define IOAGENT_H_
#include "Configuration.h"

#include <cstdlib>
#include <fstream>
#include <fftw3.h>
#include "Configuration.h"

using namespace std;

namespace PCAkern {

class IIOAgent
{
public:
	IIOAgent(){}
	virtual ~IIOAgent(){}
	virtual void InitDataset(size_t DimPower)=0;
	virtual pixel_t * GetDataset(size_t & Count)=0;
	virtual pixel_t * GetOuptBuffer() = 0;
	virtual fftw_complex * GetFftOuptBuffer() = 0;
};

class IOAgent: public virtual IIOAgent {
public:
	IOAgent(BenchmarkID_t Benchmark);
	virtual ~IOAgent();

	void StartProducer();
	void InitDataset(size_t DimPower);
	pixel_t * GetDataset(size_t & Count);
	pixel_t * GetOuptBuffer();
	fftw_complex * GetFftOuptBuffer();

private:
	size_t mDimPower;
	pixel_t *mReadBuf;
	pixel_t *mWriteBuf;
	fftw_complex *mFftWriteBuf;
	BenchmarkID_t mBenchmark;

	double frand();
	size_t AllocBuffers();
};

IIOAgent * CreateIOAgent();
IIOAgent * CreateIOAgent(BenchmarkID_t Benchmark);

}	//end namespace PCAkern

#endif /* IOAGENT_H_ */
