//Audio.h
//Author: Alan Berman
//30/4/2015

#ifndef AUDIO_H
#define AUDIO_H
#include <memory>
#include <cstdint>
#include <type_traits>
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
			saveAudio("output.raw");
		}
		~Audio() = default;
		//Write the Audio to outFile
		void saveAudio(string outFile)
		{
			ofstream output;
			output.open(outFile.c_str(), ios::out | ios::binary);
			for (auto i = data_vector.begin();i != data_vector.end(); ++i)
			{
				output << *i;
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
				noSamples = length/(sizeof(T));
				data_vector.resize(noSamples);
				for (int i = 0; i<noSamples;++i)
				{
					char * buffer = new char [sizeof(T)];
					file.read(buffer,(sizeof(T)));								
					data_vector[i]=(*((T*) (buffer))); 	
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
		Audio(const Audio & rhs)
		{
			noSamples = rhs.noSamples;	
			sampleLength = rhs.sampleLength;
			data_vector.resize(noSamples);
			for (int j = 0; j< noSamples;++j)
			{
				data_vector[j] = rhs.data_vector[j];
			}
		}
		//Move Constructor
		Audio(Audio && rhs)=default;
		//Copy and Move Assignment Operators
		Audio & operator=(Audio & rhs)=default; 
		Audio & operator=(Audio && rhs)=default;
		//Operator overloads
		Audio operator|(Audio & rhs);
		Audio operator*(pair<float,float> f);
		//Add
		Audio operator+(Audio & rhs)
		{
			Audio temp = *this;
			if (is_same<T,int8_t>::value)
			{
				for (int i = 0; i < noSamples; ++i)
				{
					if (temp.data_vector[i] + rhs.data_vector[i] >INT8_MAX)
					{
						temp.data_vector[i] = INT8_MAX;
					}
					else
					{
						temp.data_vector[i] = temp.data_vector[i] + rhs.data_vector[i];
					}
				}
			}
			else
			{
				for (int i = 0; i < noSamples; ++i)
				{
					if (temp.data_vector[i] + rhs.data_vector[i] >INT16_MAX)
					{
						temp.data_vector[i] = INT16_MAX;
					}
					else
					{
						temp.data_vector[i] = temp.data_vector[i] + rhs.data_vector[i];
					}
				}
			}
			/*//typedef typename vector<T>::iterator it;
			//typename vector<T>::iterator beg = temp.begin();
			//typename  vector<T>::iterator end = temp.end();
			//typename vector<T>::iterator rhs_beg = rhs.begin();
			auto iterator beg = temp.begin();
			auto iterator end = temp.end();
			auto iterator rhs_beg = rhs.begin();
			//Clamp to int8_t's max value if sum > max
			if (typeid(T)==int8_t)
			{
				//Iterate through the audio files
				while (beg!=end)
				{
				
					if (*beg + *rhs_beg >INT8_MAX)
					{
					   *beg = INT8_MAX;
					   ++beg;
					   ++rhs_beg; 
					}
					else
					{
						*beg = *beg + *rhs_beg;
						++beg;
						++rhs_beg; 
					} 
				}
			}
			else
			{
			//Iterate through the audio files
				while (beg!=end)
				{
					//Clamp to the max if sum > max
					if (*beg + *rhs_beg >INT16_MAX)
					{
					   *beg = INT16_MAX;
					   ++beg;
					   ++rhs_beg; 
					}
					else
					{
						*beg = *beg + *rhs_beg;
						++beg;
						++rhs_beg; 
					} 
				}
			
			}	 */
			return temp;
		}
		Audio operator^(pair<float,float> f);
		Audio operator*(int thresh_value);
		
	};
	
	template<typename T> class Audio<pair<T,T>>
	{
		private:
		vector<pair<T,T>> data_vector;
		int noSamples;
		int sampleLength;
		public:
		
		//Constructor
		Audio (string file1)
		{
			loadAudio(file1);
			saveAudio("output.raw");
		}
		~Audio() = default;
		//Write out the audio to outFile
		void saveAudio(string outFile)
		{
			ofstream output;
			output.open(outFile.c_str(), ios::out | ios::binary);
			for (auto i = data_vector.begin();i != data_vector.end(); ++i)
			{
				output << (*i).first << (*i).second;
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
				noSamples = length/((sizeof(T)) * 2);
				data_vector.resize(noSamples);
				for (int i = 0; i<noSamples;++i)
				{
					char * bufferLeft = new char [(sizeof(T))];
					file.read(bufferLeft,(sizeof(T)));
					data_vector[i].first=(*((T*) (bufferLeft))); 
					char * bufferRight = new char [(sizeof(T))];
					file.read(bufferRight,(sizeof(T)));
					data_vector[i].second=(*((T*) (bufferRight))); 
					delete[] bufferLeft;
					delete[] bufferRight;
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
