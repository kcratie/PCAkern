/*
 * AutoCorl.h
 *
 *  Created on: Apr 11, 2014
 *      Author: kcratie
 */

#ifndef AUTOCORL_H_
#define AUTOCORL_H_

#include <cstdlib>
#include "IOAgent.h"

namespace PCAkern {

class AutoCorrelation {
public:
	AutoCorrelation(
			IIOAgent * IoAgent
			);
	virtual ~AutoCorrelation();
	int	Run();

	void SetCurrentConfig(
			CurrentConfig & Config
			);
private:
	IIOAgent * mIoAgent;
	CurrentConfig mConfig;
	pixel_t * mOutput;

	int ApplyTransform(
		pixel_t const Buf[],
		size_t const  Count
		);

};

AutoCorrelation * CreateAutoCorl(
		IIOAgent * IoAgent
		);

}	//end namespace PCAkern

#endif /* AUTOCORL_H_ */
