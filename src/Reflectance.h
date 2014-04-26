
#ifndef REFLECTANCE_H_
#define REFLECTANCE_H_

#include <cstdlib>
#include "IOAgent.h"
#include "Configuration.h"

namespace PCAkern {

class Reflectance
{
public:
	Reflectance(
			IIOAgent * IoAgent
			);
	~Reflectance();

	int	Run(
		);

	void SetCurrentConfig(
			CurrentConfig & Config
			);
private:
	CurrentConfig mConfig;

	static const int MAX_THREADS;
	static const pixel_t A;
	static const pixel_t B;
	static const pixel_t S;
	static const pixel_t L;
	static const pixel_t La;

	IIOAgent * mIoAgent;
	pixel_t * mOutBuf;

	/*+
	Reflectance is implemented to be exception safe. It does not validate input as
	it should be verified by the caller.
	Spectral Radiance is given by
	L = ( (A*p) / (1 - pe*S) ) + ( (B*pe) / (1 - pe*S) ) + La
	Where:
	p is the pixel surface reflectance
	pe is an average surface reflectance for the pixel and a surrounding region
	S is the spherical albedo of the atmosphere
	La is the radiance back scattered by the atmosphere without reaching the surface
	A and B are coefficients that depend on atmospheric and geometric conditions but are surface-independent.
	Of these, A, B, S and La are parameters derived from MODTRAN.
	-*/
	inline void Refl(pixel_t & p)
	{
		p = ((L - La - ((B*p) / (1-p*S))) * (1-p*S)) / A;
	}

	int ApplyTransform(
		pixel_t buf[],
		size_t count
		);

};

Reflectance* CreateReflectance(
		IIOAgent * IoAgent
		);

}

#endif //REFLECTANCE_H_
