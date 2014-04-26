/*
 * fft2d.h
 *
 *  Created on: Apr 2, 2014
 *      Author: kcratie
 */

#ifndef FFT_H_
#define FFT_H_


#include <cstdlib>
#include "IOAgent.h"
#include <fftw3.h>

namespace PCAkern {

class fft2d {
public:
	fft2d(
		IIOAgent * IoAgent
		);
	virtual ~fft2d();

	int	Run();

	void BuildPlan();

	void SetCurrentConfig(
			CurrentConfig & Config
			);
private:
	IIOAgent * mIoAgent;
	CurrentConfig mConfig;
	pixel_t * mDataset;
	fftw_complex *mResultSet;
	fftw_plan plan_forward;
	bool isaPlan;

/*

	int ApplyTransform(
		pixel_t Buf[],
		size_t Count
		);
*/

};

fft2d* Createfft2d(
		IIOAgent * IoAgent);

}	//end namespace PCAkern

#endif /* FFT_H_ */
