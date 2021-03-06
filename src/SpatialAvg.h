/*
 * SpatialAvg.h
 *
 *  Created on: Apr 6, 2014
 *      Author: kcratie
 */

#ifndef SPATIALAVG_H_
#define SPATIALAVG_H_


#include <cstdlib>
#include "IOAgent.h"

namespace PCAkern {

class SpatialAvg
{
public:
	SpatialAvg(
			IIOAgent * IoAgent
			);
	virtual ~SpatialAvg();



	int	Run();

	void SetCurrentConfig(
			CurrentConfig & Config
			);
private:
	IIOAgent * mIoAgent;
	CurrentConfig mConfig;

	pixel_t * mOutput;

	int ApplyTransform(
		pixel_t const buf[],
		size_t const Count
		);
	inline void mask_operation(
		pixel_t const * A,
		size_t i,
		size_t j);


};

SpatialAvg* CreateSpatialAvg(
		IIOAgent * IoAgent);

}

#endif /* SPATIALAVG_H_ */
