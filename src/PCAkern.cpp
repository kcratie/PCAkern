//============================================================================
// Name        : PCAkern.cpp
// Author      : ken subratie
// Version     :
// Copyright   : Not worth copytighting
// Description : Contains the main() function
//============================================================================
#include <cstdlib>
#include <iostream>
#include "omp.h"
#include "Configuration.h"
#include "Supervisor.h"

using namespace std;
using namespace PCAkern;


int main(int argc, char* argv[])
{
	int status = 0;
	try
	{
		if (argc == 2)
		{
			string param(argv[1]);
			size_t eq = param.find("=");
			if(eq!=string::npos)
			{
				string key = param.substr(0, eq);
				if(key.compare("config")==0)
				{
					string fname = param.substr(eq+1);
					Supervisor sup(fname);
					sup.RunBenchmark();
				}
				else
					cout<<"No config file specified\n";
			}
		}
		else
		{
			cout<<"No configuration file specified\n";
		}
	}
	catch (exception& e)
	{
		status = -1;
		cout<<"exception: "<<e.what()<<"\n";
	}
	catch (...)
	{
		status = -2;
		cout<<"unhandled exception ...\n";
	}
	return status;

}
