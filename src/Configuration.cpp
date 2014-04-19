/*
 * Config.cpp
 *
 *  Created on: Apr 8, 2014
 *      Author: kcratie
 */
#include <fstream>
#include "Configuration.h"
using namespace std;

namespace PCAkern
{

CurrentConfig::CurrentConfig(
	size_t NumProcs,
	size_t DimPow,
	size_t NumItrs,
	BenchmarkID_t BenchmarkID,
	string OutFilename
	) :
	NumProcs(NumProcs),
	DimPow(DimPow),
	NumItrs(NumItrs),
	BenchmarkID(BenchmarkID),
	OutputFilename(OutFilename)
	{}
CurrentConfig::CurrentConfig():
			NumProcs(0),
			DimPow(0),
			NumItrs(0),
			BenchmarkID(All){}
CurrentConfig::~CurrentConfig(){}

void
Configuration::LoadConfig(const string & Filename)
{
	string line;
	ifstream config(Filename.c_str());
	if (config.is_open())
	{
		while (getline (config, line))
		{
			size_t eq = line.find("=");
			if(eq!=string::npos)
			{
				string key = line.substr(0, eq);
				string val = line.substr(eq+1);
				if(key.compare("MinNumProcs")==0)
					MinNumProcs = atoi(val.c_str());
				else if (key.compare("MaxNumProcs")==0)
					MaxNumProcs = atoi(val.c_str());
				/*			  else if (key.compare("MinNumRows")==0)
				  MinNumRows = atoi(val.c_str());
			  else if (key.compare("MaxNumRows")==0)
				  MaxNumRows = atoi(val.c_str());
				 */			  else if (key.compare("MinDimPow")==0)
					 MinDimPow = atoi(val.c_str());
				 else if (key.compare("MaxDimPow")==0)
					 MaxDimPow = atoi(val.c_str());
				 else if (key.compare("NumItrs")==0)
					 NumItrs = atoi(val.c_str());
				 else if (key.compare("BenchmarkID")==0)
					 BenchmarkID.assign(val);
				 else if (key.compare("BaseOutputfile")==0)
					 OutputFilename.assign(val);
			}
		}
		config.close();
	}
	else
	{
		string emsg("Could not open config file:");
		emsg.append(Filename);
		throw logic_error(emsg);
	}
	return;
}

}
