//Audio.h
//Author: Alan Berman
//30/4/2015

#ifndef AUDIO_H
#define AUDIO_H
#include <memory>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <string>

//Header file that includes the Audio class declaration
namespace BRMALA003
{
	using namespace std;
	template <typename T> class Audio
	{
		private:
		vector<T> data_vector;
		int noSamples;
		int sampleLength;
		//Adapted from framework given in assignment 4 brief
		public:
		
		//Constructor
		Audio (string file)
		{
			loadAudio(file);
		}
		~Audio() = default;
		//Return a reference to Audio_ptr
		//Write the Audio to outFile
		void saveAudio(string outFile)
		{
			ofstream output;
			output.open(outFile.c_str(), ios::out | ios::binary);
			for (auto i = data_vector.begin();i != data_vector.end(); ++i)
			{
			//	output.write((char *) data_vector.at(i),noSamples/sampleLength);
			}
		}
		//Read in the Audio inputFile
		void loadAudio(string inputFile)
		{
			ifstream file (inputFile, ifstream::binary);
			int length;
			if (file)
			{
				//Adapted from http://www.cplusplus.com/reference/istream/istream/tellg/
				file.seekg(0,file.end);
				length = file.tellg();
				sampleLength = length;
				file.seekg(0,file.beg);
				noSamples = length/(sizeof(int8_t));
				for (int i = 0; i<noSamples;++i)
				{
					char * buffer = new char [length/noSamples];
					file.read(buffer,(length/noSamples));
					file.seekg(length/noSamples);										
					data_vector[i]=*buffer; //Fails
					delete[] buffer;				
				}
				
			}
		}
		//Reverse an Audio clip
		void reverseAudio(Audio & aClip);
		//Ranged add
		Audio rangedAdd(Audio & clipOne, Audio & clipTwo);
		//Compute RMS
		void computeRMS(Audio & aClip);
		//Normalize
		void normalize(Audio & aClip); 
		//Copy Constructor
		Audio(Audio & rhs);
		//Move Constructor
		Audio(Audio && rhs);
		//Copy and Move Assignment Operators
		Audio & operator=(Audio & rhs); 
		Audio & operator=(Audio && rhs);
		//Operator overloads
		Audio operator|(Audio & rhs);
		Audio operator*(pair<float,float> f);
		Audio operator+(Audio & rhs);
		Audio operator^(pair<float,float> f);
		Audio operator*(int thresh_value);
		
	};
	
	
	template<> class Audio<pair<int16_t,int16_t>>
	{
		private:
		vector<pair<int16_t,int16_t>> data_vector;
		int noSamples;
		int sampleLength;
		//Adapted from framework given in assignment 4 brief
		public:
		
		//Constructor
		Audio(string file);
		~Audio() = default;
		//Return a reference to Audio_ptr
		//Write the Audio to outFile
		void saveAudio(string outFile);
		//Read in the Audio inputFile
		void loadAudio(string inputFile)
		{
			ifstream file (inputFile, ifstream::binary);
			int length;
			if (file)
			{
				//Adapted from http://www.cplusplus.com/reference/istream/istream/tellg/
				file.seekg(0,file.end);
				length = file.tellg();
				sampleLength = length;
				file.seekg(0,file.beg);
				noSamples = length/(sizeof(int16_t) * 2);
				for (int i = 0; i<noSamples;++i)
				{
					char * buffer = new char [length/noSamples];
					file.read(buffer,((length/noSamples)/2));
					file.seekg((length/noSamples)/2);
					data_vector[i].first=*buffer;
					file.read(buffer,((length/noSamples)/2));
					file.seekg((length/noSamples)/2);
					data_vector[i].second=*buffer;
						delete[] buffer;
				}
			}
		}
		//Reverse an Audio clip
		void reverseAudio(Audio & aClip);
		//Ranged add
		Audio rangedAdd(Audio & clipOne, Audio & clipTwo);
		//Compute RMS
		void computeRMS(Audio & aClip);
		//Normalize
		void normalize(Audio & aClip); 
		//Copy Constructor
		Audio(Audio & rhs);
		//Move Constructor
		Audio(Audio && rhs);
		//Copy and Move Assignment Operators
		Audio & operator=(Audio & rhs); 
		Audio & operator=(Audio && rhs);
		//Operator overloads
		Audio operator|(Audio & rhs);
		Audio operator*(pair<float,float> f);
		Audio operator+(Audio & rhs);
		Audio operator^(pair<float,float> f);
		Audio operator*(int thresh_value);
		
	};
}
#endif //AUDIO_H
