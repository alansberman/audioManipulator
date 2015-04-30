//Audio.h
//Author: Alan Berman
//30/4/2015

#ifndef AUDIO_H
#define AUDIO_H
#include <memory>
#include <cstdint>
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
		vector<int8_t> data_vector;
		int noSamples;
		//Adapted from framework given in assignment 4 brief
		public:
		
		//Constructor
		Audio<T>(string file);
		~Audio() = default;
		//Return a reference to Audio_ptr
		//Write the Audio to outFile
		void saveAudio(string outFile);
		//Read in the Audio inputFile
		void loadAudio(string inputFile);
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
		//Adapted from framework given in assignment 4 brief
		public:
		
		//Constructor
		Audio(string file);
		~Audio() = default;
		//Return a reference to Audio_ptr
		//Write the Audio to outFile
		void saveAudio(string outFile);
		//Read in the Audio inputFile
		void loadAudio(string inputFile);
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
