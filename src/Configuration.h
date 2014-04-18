/*
 * Config.h
 *
 *  Created on: Apr 6, 2014
 *      Author: kcratie
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdexcept>
#include <cstdlib>
#include <iostream>

using namespace std;

namespace PCAkern
{

typedef	double pixel_t;

struct PixelList
{
	pixel_t pix[];
};

enum BenchmarkID_t
{
	Refl= 0,
	FFT,
	SpAvg,
	AutoCorl,
	All,
};

class Configuration
{
public:
	void LoadConfig(const string & Filename);
	size_t MinNumProcs;
	size_t MaxNumProcs;
	size_t MinDimPow;
	size_t MaxDimPow;
	size_t NumItrs;
	string BenchmarkID;
	string OutputFilename;
};

struct CurrentConfig
{
	CurrentConfig();
	CurrentConfig(
		size_t NumProcs,
		size_t DimPow,
		size_t NumItrs,
		BenchmarkID_t BenchmarkID,
		string OutputFilename
		);
	~CurrentConfig();
	size_t NumProcs;
	size_t DimPow;
	size_t NumItrs;
	BenchmarkID_t BenchmarkID;
	string OutputFilename;
};

}
#endif /* CONFIG_H_ */
