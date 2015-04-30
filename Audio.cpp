//Audio.cpp
//Author: Alan Berman
//30/4/2015

#include <memory>
#include <cstdint>
#include <vector>
#include <numeric>
#include <string>
#include "Audio.h"

//Creates mono/stereo audio clips (instances of the Audio class)
//The clips may be reversed, added, concatenated, cut, added over ranges
//adjusted by a volume factor or normalized. The RMS of the clips may
//also be retrieved.
namespace BRMALA003
{
	using namespace std;
	void Audio::loadAudio(string inputFile, int noChannels)
	{
		ifstream file (inputFile, ifstream::binary);
		int length;
		if (noChannels == 1)
		{
			if (file)
			{
				//Adapted from http://www.cplusplus.com/reference/istream/istream/tellg/
				file.seekg(0,file.end);
				length = file.tellg();
				file.seekg(0,file.beg);
				char * buffer = new char [length];
			}
		}
		else
		{
			
		}
		
		
	}
	
	
	
	
	
	
	
	
}
