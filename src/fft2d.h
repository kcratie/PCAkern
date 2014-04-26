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

	void SetCurrentConfig(
			CurrentConfig & Config
			);
private:
	IIOAgent * mIoAgent;
	CurrentConfig mConfig;

	fftw_complex *mResultSet;

	int ApplyTransform(
		pixel_t Buf[],
		size_t count
		);

};

fft2d* Createfft2d(
		IIOAgent * IoAgent);

}	//end namespace PCAkern

#endif /* FFT_H_ */
