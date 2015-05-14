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
		int duration;
		
		//Adapted from framework given in assignment 4 brief
		public:
		
		//Constructor
		Audio (string file,int sampleRate)
		{
			loadAudio(file,sampleRate);
			
		}
		~Audio() = default;
		//Write the Audio to outFile
		void saveAudio(string outFile)
		{
			ofstream output;
			output.open(outFile.c_str(), ios::out | ios::binary);
			output.write((char *) &data_vector[0], data_vector.size() *(sizeof(T)/sizeof(char)));
			output.close();		
		}
		
		
		//Read in the Audio inputFile
		void loadAudio(string inputFile,int sampleRate)
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
				duration = noSamples / (float) sampleRate;
				cout << "duration : " << duration << endl;
				data_vector.resize(noSamples);
				for (int i = 0; i<noSamples;++i)
				{
					char * buffer = new char [sizeof(T)/sizeof(char)];
					file.read(buffer,(sizeof(T)/sizeof(char)));								
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
			//If the file is 8bit
			if (is_same<T,int8_t>::value)
			{
				for (int i = 0; i < noSamples; ++i)
				{
					//Clamp to the max value
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
			else //16bit file
			{
				for (int i = 0; i < noSamples; ++i)
				{
					//Clamp to the max
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
		int duration;
		public:
		
		//Constructor
		Audio (string file1,int sampleRate)
		{
			loadAudio(file1,sampleRate);
		//	saveAudio("output.raw");
		}
		~Audio() = default;
		//Write out the audio to outFile
		void saveAudio(string outFile)
		{
			ofstream output;
			output.open(outFile.c_str(), ios::out | ios::binary);
			output.write((char *) &data_vector[0], data_vector.size() * ((sizeof(T)*2)/sizeof(char)));
			output.close();
		}
		//Read in the Audio inputFile
		void loadAudio(string inputFile,int sampleRate)
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
				duration = noSamples / (float) sampleRate;
				data_vector.resize(noSamples);
				for (int i = 0; i<noSamples;++i)
				{
					//Make a buffer for each of the values of the pair
					char * bufferLeft = new char [(sizeof(T)*2)/sizeof(char)];
					file.read(bufferLeft,(sizeof(T)/sizeof(char)));
					data_vector[i].first=(*((T*) (bufferLeft))); 
					char * bufferRight = new char [(sizeof(T)*2)/sizeof(char)];
					file.read(bufferRight,(sizeof(T)/sizeof(char)));
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
		Audio(Audio & rhs)
		{
		
			noSamples = rhs.noSamples;	
			sampleLength = rhs.sampleLength;
			data_vector.resize(noSamples);
			for (int j = 0; j< noSamples;++j)
			{
				data_vector[j].first = rhs.data_vector[j].first;
				data_vector[j].second = rhs.data_vector[j].second;
			}
		
		}
		//Move Constructor
		Audio(Audio && rhs);
		//Copy and Move Assignment Operators
		Audio & operator=(Audio & rhs); 
		Audio & operator=(Audio && rhs);
		//Operator overloads
		Audio operator|(Audio & rhs);
		Audio operator*(pair<float,float> f);
		//Adds 
		Audio operator+(Audio & rhs)
		{
			Audio temp = *this;
			//If the audio file is 8bit
			if (is_same<T,int8_t>::value)
			{
				for (int i = 0; i < noSamples; ++i)
				{
					//Clamp to the max (both values of the pair)
					if (temp.data_vector[i].first + rhs.data_vector[i].first >INT8_MAX &&
					temp.data_vector[i].second + rhs.data_vector[i].second >INT8_MAX)
					{
						temp.data_vector[i].first = INT8_MAX;
						temp.data_vector[i].second = INT8_MAX;
					}
					//Clamp to the max (the second of the pair)
					else if (temp.data_vector[i].first + rhs.data_vector[i].first >INT8_MAX)
					{
						temp.data_vector[i].first = INT8_MAX;
						temp.data_vector[i].second = temp.data_vector[i].second + rhs.data_vector[i].second;
					}
					//Clamp to the max (the first of the pair)
					else if (temp.data_vector[i].second + rhs.data_vector[i].second >INT8_MAX)
					{
						temp.data_vector[i].first = temp.data_vector[i].first + rhs.data_vector[i].first;
						temp.data_vector[i].second = INT8_MAX;
					}
					else
					{
						temp.data_vector[i].first = temp.data_vector[i].first + rhs.data_vector[i].first;
						temp.data_vector[i].second = temp.data_vector[i].second + rhs.data_vector[i].second;
					}
				}
			}
			else
			{
				for (int i = 0; i < noSamples; ++i)
				{
					//Clamp to the max (both values of the pair)
					if (temp.data_vector[i].first + rhs.data_vector[i].first >INT16_MAX &&
					temp.data_vector[i].second + rhs.data_vector[i].second >INT16_MAX)
					{
						temp.data_vector[i].first = INT16_MAX;
						temp.data_vector[i].second = INT16_MAX;
					}
					//Clamp to the max (the second of the pair)
					else if (temp.data_vector[i].first + rhs.data_vector[i].first >INT16_MAX)
					{
						temp.data_vector[i].first = INT16_MAX;
						temp.data_vector[i].second = temp.data_vector[i].second + rhs.data_vector[i].second;
					}
					//Clamp to the max (the first of the pair)
					else if (temp.data_vector[i].second + rhs.data_vector[i].second >INT16_MAX)
					{
						temp.data_vector[i].first = temp.data_vector[i].first + rhs.data_vector[i].first;
						temp.data_vector[i].second = INT16_MAX;
					}
					else
					{
						temp.data_vector[i].first = temp.data_vector[i].first + rhs.data_vector[i].first;
						temp.data_vector[i].second = temp.data_vector[i].second + rhs.data_vector[i].second;
					}
				
				}
			}
		
			return temp;
		}
		Audio operator^(pair<float,float> f);
		Audio operator*(int thresh_value);
		
	};
}
#endif //AUDIO_H
