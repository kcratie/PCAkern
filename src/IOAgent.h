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
};

class IOAgent: public virtual IIOAgent {
public:
	IOAgent();
	virtual ~IOAgent();

	void StartProducer();
	void InitDataset(size_t DimPower);
	pixel_t * GetDataset(size_t & Count);

private:
	//size_t mNumRows;
	//size_t mNumCols;
	size_t mDimPower;
	pixel_t *mBuf;
	double frand();
};

IIOAgent * CreateIOAgent();

}	//end namespace PCAkern

#endif /* IOAGENT_H_ */
