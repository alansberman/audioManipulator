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
#include <algorithm>
#include <vector>
#include <numeric>
#include <string>

//Header file that includes the Audio class declarations
//both general and specialised. The general template Audio
//class takes either a 8bit or 16bit mono channel file,
//while the specialised class takes a 8bit or 16bit stereo file.
//Both classes support a variety of options on the audio files:
//add,ranged add,cut,concatenate,reverse,normalize,RMS and volume factor
namespace BRMALA003
{
	using namespace std;
	//Used for normalize (mono files)
	template<typename T> class Func
	{
		private:
		float d,c;
		public:
		Func(float desired,float current)
		{
			d=desired;
			c=current;
		}
		T operator()(T item)
		{
			int max_16=INT16_MAX;
			int min_16=INT16_MAX;
			//Clamp values
			if  (is_same<T,int8_t>::value)
			{
				if (item*(d/c) > INT8_MAX)
				{
					item=INT8_MAX;
					return item;
				}
				else if (item*(d/c) < INT8_MIN)
				{
					item=INT8_MIN;
					return item;
				}
				else
				{
					item=item*(d/c);
					return item;
				}
			}
			else
			{
				if ((item*(d/c)) > max_16)
				{
					item=max_16;
				
					return item;
				}
				else if ((item*(d/c)) < min_16)
				{
					item=min_16;
					
					return item;
				}
				else
				{
					item=(item*(d/c));
					
					return item;
				}
			}
		}	
		
	};
	
	//General template class (mono .raw files)
	template <typename T> class Audio
	{
		private:
		vector<T> data_vector;
		int noSamples;
		int sampleLength;
		int duration;
		
		public:
		
		//Constructor
		Audio (string file,int sampleRate)
		{
			loadAudio(file,sampleRate);			
		}
		~Audio() = default;
		//Write the Audio to outFile
		void saveAudio(string outFile,int sampleRate)
		{
			ofstream output;
			string type = "";
			//If it's an 8bit file
			if  (is_same<T,int8_t>::value)
			{
				type="8";
			}
			else
			{
				type="16";
			}
			string sampleRateString = to_string(sampleRate);
			//Make the output file have an informative name
			outFile+="_"+sampleRateString+"_"+type+"_mono.raw";
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
				data_vector.resize(noSamples);
				//For each sample, read in the appropriate number of chars
				//after making a buffer for each sample
				for (int i = 0; i<noSamples;++i)
				{
					char * buffer = new char [sizeof(T)/sizeof(char)];
					file.read(buffer,(sizeof(T)/sizeof(char)));	
					data_vector[i]=(*((T*) (buffer))); 	
					delete[] buffer;			
				}				
			}
		}
		//Reverse an Audio clip using the Standard Template Library's
		//reverse()
		Audio reverseAudio(void)
		{
			Audio temp = *this;
			reverse(temp.data_vector.begin(),temp.data_vector.end());
			return temp;
		}
		//Ranged add
		//adds two ranges of two audio files (same size) [in seconds] together
		//and places the result in a new file
		Audio rangedAdd(Audio & rhs,int sampleRate,pair<int,int> a,pair<int,int> b)
		{
			Audio rhsCopy = rhs;
			Audio temp = *this;
			
			//find number of samples in a second(1 second <=> 44100 samples)
			//and thus the range represented in samples
			int range1 = (a.second * sampleRate) - (a.first * sampleRate);
			int range2 = (b.second * sampleRate) - (a.first * sampleRate);
			//Adjust numSamples
			temp.noSamples = range1;
			rhsCopy.noSamples = range2;
			temp.data_vector.clear();
			temp.data_vector.resize(temp.noSamples);
			rhsCopy.data_vector.clear();
			rhsCopy.data_vector.resize(rhsCopy.noSamples);
			copy(this->data_vector.begin()+(a.first*sampleRate),this->data_vector.begin()+(a.second*sampleRate),temp.data_vector.begin());
			copy(rhs.data_vector.begin()+(a.first*sampleRate),rhs.data_vector.begin()+(b.second*sampleRate),rhsCopy.data_vector.begin());
			Audio result =temp+rhsCopy;
			result.duration = temp.noSamples / (float) sampleRate;
			return result;  	
		}
		//Compute RMS
		float computeRMS(void)
		{
			float RMS = accumulate(data_vector.begin(),data_vector.end(),0,[](T  sum, T  item){return (sum + pow(item,2));});
			RMS=RMS/noSamples;
			RMS=sqrt(double(abs(RMS)));
			return RMS;
		}
		//Normalize
		Audio normalize(float left, float right,float currentRMS)
		{
			Audio temp = *this;
			temp.data_vector.clear();
			temp.data_vector.resize(temp.noSamples);
			if  (is_same<T,int8_t>::value)
			{
				Func<int8_t> f(left,currentRMS);
				transform(data_vector.begin(),data_vector.end(),temp.data_vector.begin(), f);
				return temp;
			}
			else
			{
				Func<int16_t> g(left,currentRMS);
				transform(data_vector.begin(),data_vector.end(),temp.data_vector.begin(), g);
				return temp;
			}
		} 
		//Copy Constructor
		Audio(const Audio & rhs)
		{
			noSamples = rhs.noSamples;	
			sampleLength = rhs.sampleLength;
			duration = rhs.duration;
			data_vector.resize(noSamples);
			//Copy the values of rhs's data_vector
			for (int j = 0; j< noSamples;++j)
			{
				data_vector[j] = rhs.data_vector[j];
			}
		}
		//Move Constructor
		Audio(Audio && rhs)
		{
			noSamples = rhs.noSamples;	
			sampleLength = rhs.sampleLength;
			duration = rhs.duration;
			data_vector.resize(noSamples);
			//Copy the values of rhs's data_vector
			for (int j = 0; j< noSamples;++j)
			{
				data_vector[j] = rhs.data_vector[j];
			}
			rhs.noSamples = -1;
			rhs.sampleLength = -1;
			rhs.duration=-1;
			rhs.data_vector.clear();
			
		}
		//getNoSamples
		int getNoSamples()
		{
			return noSamples;
		}
		//getDataVector
		vector<T> getDataVector()
		{
			return data_vector;
		}
		int getDuration()
		{
			return duration;
		}
		//Copy and Move Assignment Operators
		Audio & operator=(Audio & rhs)
		{
			noSamples = rhs.noSamples;	
			sampleLength = rhs.sampleLength;
			duration = rhs.duration;
			data_vector.clear();
			data_vector.resize(noSamples);
			//Copy the values of rhs's data_vector
			for (int j = 0; j< noSamples;++j)
			{
				data_vector[j] = rhs.data_vector[j];
			}
			return *this;
			
		}
		Audio & operator=(Audio && rhs)
		{
			noSamples = rhs.noSamples;	
			sampleLength = rhs.sampleLength;
			duration = rhs.duration;
			data_vector.clear();
			data_vector.resize(noSamples);
			//Copy the values of rhs's data_vector
			for (int j = 0; j< noSamples;++j)
			{
				data_vector[j] = rhs.data_vector[j];
			}
			rhs.noSamples = -1;
			rhs.sampleLength = -1;
			rhs.duration=-1;
			rhs.data_vector.clear();
			return *this;
		}
		//Operator overloads
		//Concatenate 2 files
		Audio operator|(Audio & rhs)
		{
			Audio temp = *this;
			//Store the old noSamples
			int oldNoSamples = temp.noSamples;
			//Set the concatenated number of samples (lhs + rhs)
			//and resize the data vector accordingly
			temp.noSamples = temp.noSamples + rhs.noSamples;
			temp.data_vector.resize(temp.noSamples);
			for (int i = oldNoSamples; i < temp.noSamples; ++i)
			{
				temp.data_vector[i] = rhs.data_vector[i-oldNoSamples];	
			}
			return temp;
		}
		//Change the volume of the file by factor f.first
		Audio operator*(pair<float,float> f)
		{
			Audio temp = *this;
			for (int j = 0; j< noSamples;++j)
			{
				temp.data_vector[j] = temp.data_vector[j]*(f.first);
			}
			return temp;
		}
		//Add 2 files
		Audio operator+(Audio & rhs)
		{
			Audio temp = *this;
			//If the file is 8bit
			int max16 = INT16_MAX; //prevents compiler warning
			int min16 = INT16_MIN;
			if (is_same<T,int8_t>::value)
			{
				for (int i = 0; i < noSamples; ++i)
				{
					//Clamp to the max/min value
					if (temp.data_vector[i] + rhs.data_vector[i] >INT8_MAX)
					{
						temp.data_vector[i] = INT8_MAX;
					}
					else if (temp.data_vector[i] + rhs.data_vector[i] < INT8_MIN)
					{
						temp.data_vector[i] = INT8_MIN;
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
					//Clamp to the max/min
					if (temp.data_vector[i] + rhs.data_vector[i] >max16)
					{
						temp.data_vector[i] = max16;
					}
					else if (temp.data_vector[i] + rhs.data_vector[i] < min16)
					{
						temp.data_vector[i] = min16;
					}
					else
					{
						temp.data_vector[i] = temp.data_vector[i] + rhs.data_vector[i];
					}
				}
			}
		
			return temp;
		}
		//Cut a file across a range (first value is start,
		//second value is end)
		Audio operator^(pair<float,float> f)
		{
			Audio temp=*this;
			int start=(int) f.first;
			int end=(int) f.second;	int counter=0;
			//Clear, get new number of samples and resize			
			temp.data_vector.clear();
			temp.noSamples = temp.noSamples - (end-start);
			temp.data_vector.resize(temp.noSamples);
			//Copy values til start
			for (int i = 0; i < start; ++i)
			{
				temp.data_vector[i]=data_vector[i];
			}
			
			//Copy values from start to end
			for (int j = start; j < temp.noSamples; ++j)
			{
				temp.data_vector[j]=data_vector[end+counter];
				++counter;
			}

			return temp;
			
		}
		
		
	};
	//Specialized template class (stereo .raw files)
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
		}
		~Audio() = default;
		//Write out the audio to outFile
		void saveAudio(string outFile,int sampleRate)
		{
			ofstream output;
			string type = "";
			//If the file is 8bit
			if  (is_same<T,int8_t>::value)
			{
				type="8";
			}
			else
			{
				type="16";
			}
			string sampleRateString = to_string(sampleRate);
			//Make the output file have an informative name
			outFile+="_"+sampleRateString+"_"+type+"_stereo.raw";
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
					//For each sample, read in the appropriate number of chars
					//after making a buffer for each of the values of the pair
					//(i.e. the left and right channels)
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
		//getNoSamples
		int getNoSamples()
		{
			return noSamples;
		}
		//getDataVector
		vector<pair<T,T>> getDataVector()
		{
			return data_vector;
		}
		int getDuration()
		{
			return duration;
		}
		//Reverse an Audio clip using the Standard Template Library's
		//reverse()
		Audio reverseAudio(void)
		{
			Audio temp = *this;
			reverse(temp.data_vector.begin(),temp.data_vector.end());
			return temp;
		}
		
		//Ranged add
		//adds two ranges of two audio files (same size) [in seconds] together
		//and places the result in a new file
		
		Audio rangedAdd(Audio & rhs,int sampleRate,pair<int,int> a,pair<int,int> b)
		{
			Audio rhsCopy = rhs;
			Audio temp = *this;
			
			//find number of samples in a second(1 second <=> 44100 samples)
			//and thus the range represented in samples
			int range1 = (a.second * sampleRate) - (a.first * sampleRate);
			int range2 = (b.second * sampleRate) - (a.first * sampleRate);
			//Adjust numSamples
			temp.noSamples = range1;
			rhsCopy.noSamples = range2;
			temp.data_vector.clear();
			temp.data_vector.resize(temp.noSamples);
			rhsCopy.data_vector.clear();
			rhsCopy.data_vector.resize(rhsCopy.noSamples);
			copy(this->data_vector.begin()+(a.first*sampleRate),this->data_vector.begin()+(a.second*sampleRate),temp.data_vector.begin());
			copy(rhs.data_vector.begin()+(a.first*sampleRate),rhs.data_vector.begin()+(b.second*sampleRate),rhsCopy.data_vector.begin());
			Audio result =temp+rhsCopy;
			return result;  	
		}
			
		
		//Compute RMS
		pair<float,float> computeRMS(void)
		{
			Audio temp = *this;
			//Make a data vector for the left and right values of the pair
			vector<T> leftChannelVector;
			leftChannelVector.resize(temp.noSamples);
			vector<T> rightChannelVector;
			rightChannelVector.resize(temp.noSamples);
			//Add the left values to the left vector and vice versa
			for (int i=0;i<noSamples;++i)
			{
				leftChannelVector[i]=data_vector[i].first;
				rightChannelVector[i]=data_vector[i].second;
			}
			//Calculate the RMSes using accumulate
			float RMSLeft = accumulate(leftChannelVector.begin(),leftChannelVector.end(),0,[](T sum, T item){return (sum + pow(item,2));});
			float RMSRight = accumulate(rightChannelVector.begin(),rightChannelVector.end(),0,[](T sum, T item){return (sum + pow(item,2));});
			RMSLeft=RMSLeft/noSamples;
			RMSLeft=sqrt(double(abs(RMSLeft)));
			RMSRight=RMSRight/noSamples;
			RMSRight=sqrt(double(abs(RMSRight)));
			//RMSRight=RMSRight/noSamples;
			pair<float,float>RMS = make_pair(RMSLeft,RMSRight);
			return RMS;
		}
		//Normalize
		/*
		Audio normalize(float left, float right,float currentRMSL, float currentRMSR)
		{
			Audio temp = *this;
			//Make a data vector for the left and right values of the pair
			vector<T> leftChannelVector;
			leftChannelVector.resize(temp.noSamples);
			vector<T> rightChannelVector;
			rightChannelVector.resize(temp.noSamples);
			//Add the left values to the left vector and vice versa
			for (int i=0;i<noSamples;++i)
			{
				leftChannelVector[i]=data_vector[i].first;
				rightChannelVector[i]=data_vector[i].second;
			}
			if  (is_same<T,int8_t>::value)
			{
				Func<int8_t> f(left,currentRMSL);
				transform(data_vector.begin(),data_vector.end(),leftChannelVector.begin(), f);
				Func<int8_t> g(right,currentRMSR);
				transform(data_vector.begin(),data_vector.end(),rightChannelVector.begin(), g);
				return temp;
			}
			else
			{
				Func<int16_t> f16(left,currentRMSL);
				transform(data_vector.begin(),data_vector.end(),leftChannelVector.begin(), f16);
				Func<int16_t> g16(right,currentRMSR);
				transform(data_vector.begin(),data_vector.end(),rightChannelVector.begin(), g16);
				return temp;
			}
		}  */
		//Copy Constructor
		Audio(Audio & rhs)
		{
			noSamples = rhs.noSamples;	
			sampleLength = rhs.sampleLength;
			duration = rhs.duration;
			data_vector.resize(noSamples);
			//Copy rhs' data_vectors values (both halves of pair)
			for (int j = 0; j< noSamples;++j)
			{
				data_vector[j].first = rhs.data_vector[j].first;
				data_vector[j].second = rhs.data_vector[j].second;
			}
		
		}
		//Move Constructor
		Audio(Audio && rhs)
		{
			noSamples = rhs.noSamples;	
			sampleLength = rhs.sampleLength;
			duration = rhs.duration;
			data_vector.resize(noSamples);
			//Copy the values of rhs's data_vector
			for (int j = 0; j< noSamples;++j)
			{
				data_vector[j].first = rhs.data_vector[j].first;
				data_vector[j].second = rhs.data_vector[j].second;
			}
			rhs.noSamples = -1;
			rhs.sampleLength = -1;
			rhs.duration = -1;
			rhs.data_vector.clear();
			
		}
		//Copy and Move Assignment Operators
	
		Audio & operator=(Audio & rhs)
		{
			noSamples = rhs.noSamples;	
			sampleLength = rhs.sampleLength;
			duration = rhs.duration;
			data_vector.clear();
			data_vector.resize(noSamples);
			//Copy the values of rhs's data_vector
			for (int j = 0; j< noSamples;++j)
			{
				data_vector[j].first = rhs.data_vector[j].first;
				data_vector[j].second = rhs.data_vector[j].second;
			}
			return *this;	
		}
		Audio & operator=(Audio && rhs)
		{
			noSamples = rhs.noSamples;	
			sampleLength = rhs.sampleLength;
			duration = rhs.duration;
			data_vector.clear();
			data_vector.resize(noSamples);
			//Copy the values of rhs's data_vector
			for (int j = 0; j< noSamples;++j)
			{
				data_vector[j].first = rhs.data_vector[j].first;
				data_vector[j].second = rhs.data_vector[j].second;
			}
			rhs.noSamples = -1;
			rhs.sampleLength = -1;
			rhs.duration = -1;
			rhs.data_vector.clear();
			return *this;
		}
		//Operator overloads
		
		//Concatenate 2 files
		Audio operator|(Audio & rhs)
		{
			Audio temp = *this;
			//Store the old noSamples
			int oldNoSamples = temp.noSamples;
			//Set the concatenated number of samples (lhs + rhs)
			//and resize the data vector accordingly
			temp.noSamples = temp.noSamples + rhs.noSamples;
			temp.data_vector.resize(temp.noSamples);
			for (int i = oldNoSamples; i < temp.noSamples; ++i)
			{
				temp.data_vector[i].first = rhs.data_vector[i-oldNoSamples].first;
				temp.data_vector[i].second = rhs.data_vector[i-oldNoSamples].second;	
			}
			
			return temp;
		}
		//Change the volume of the file by factor f
		//left channel by first, right by second
		Audio operator*(pair<float,float> f)
		{
			Audio temp = *this;
			for (int j = 0; j< noSamples;++j)
			{
				temp.data_vector[j].first = temp.data_vector[j].first*(f.first);
				temp.data_vector[j].second = temp.data_vector[j].second*(f.second);
			}
			return temp;
		}
		//Add 2 files
		Audio operator+(Audio & rhs)
		{
			Audio temp = *this;
			
			int max16stereo = INT16_MAX; //prevents compiler warning
			int min16stereo = INT16_MIN; 
			//If the audio file is 8bit
			if (is_same<T,int8_t>::value)
			{
				for (int i = 0; i < noSamples; ++i)
				{
					//Clamp to the max/min (both values of the pair)
					if (temp.data_vector[i].first + rhs.data_vector[i].first >INT8_MAX &&
					temp.data_vector[i].second + rhs.data_vector[i].second >INT8_MAX)
					{
						temp.data_vector[i].first = INT8_MAX;
						temp.data_vector[i].second = INT8_MAX;
					}
					else if (temp.data_vector[i].first + rhs.data_vector[i].first <INT8_MIN &&
					temp.data_vector[i].second + rhs.data_vector[i].second <INT8_MIN)
					{
						temp.data_vector[i].first = INT8_MIN;
						temp.data_vector[i].second = INT8_MIN;
					}
					//Clamp to the max/min (the first of the pair)
					else if (temp.data_vector[i].first + rhs.data_vector[i].first >INT8_MAX)
					{
						temp.data_vector[i].first = INT8_MAX;
						temp.data_vector[i].second = temp.data_vector[i].second + rhs.data_vector[i].second;
					}
					else if (temp.data_vector[i].first + rhs.data_vector[i].first <INT8_MIN)
					{
						temp.data_vector[i].first = INT8_MIN;
						temp.data_vector[i].second = temp.data_vector[i].second + rhs.data_vector[i].second;
					}
					//Clamp to the max/min (the second of the pair)
					else if (temp.data_vector[i].second + rhs.data_vector[i].second >INT8_MAX)
					{
						temp.data_vector[i].first = temp.data_vector[i].first + rhs.data_vector[i].first;
						temp.data_vector[i].second = INT8_MAX;
					}
					else if (temp.data_vector[i].second + rhs.data_vector[i].second <INT8_MIN)
					{
						temp.data_vector[i].first = temp.data_vector[i].first + rhs.data_vector[i].first;
						temp.data_vector[i].second = INT8_MIN;
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
					//Clamp to the max/min (both values of the pair)
					if (temp.data_vector[i].first + rhs.data_vector[i].first >max16stereo &&
					temp.data_vector[i].second + rhs.data_vector[i].second >max16stereo)
					{
						temp.data_vector[i].first = max16stereo;
						temp.data_vector[i].second = max16stereo;
					}
					else if (temp.data_vector[i].first + rhs.data_vector[i].first <min16stereo &&
					temp.data_vector[i].second + rhs.data_vector[i].second <min16stereo)
					{
						temp.data_vector[i].first = min16stereo;
						temp.data_vector[i].second = min16stereo;
					}
					//Clamp to the max/min (the first of the pair)
					else if (temp.data_vector[i].first + rhs.data_vector[i].first >max16stereo)
					{
						temp.data_vector[i].first = max16stereo;
						temp.data_vector[i].second = temp.data_vector[i].second + rhs.data_vector[i].second;
					}
					else if (temp.data_vector[i].first + rhs.data_vector[i].first <min16stereo)
					{
						temp.data_vector[i].first = min16stereo;
						temp.data_vector[i].second = temp.data_vector[i].second + rhs.data_vector[i].second;
					}
					//Clamp to the max/min (the second of the pair)
					else if (temp.data_vector[i].second + rhs.data_vector[i].second >max16stereo)
					{
						temp.data_vector[i].first = temp.data_vector[i].first + rhs.data_vector[i].first;
						temp.data_vector[i].second = max16stereo;
					}
					else if (temp.data_vector[i].second + rhs.data_vector[i].second <min16stereo)
					{
						temp.data_vector[i].first = temp.data_vector[i].first + rhs.data_vector[i].first;
						temp.data_vector[i].second = min16stereo;
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
		
		//Cut a file across a range (first value is start,
		//second value is end)
		Audio operator^(pair<float,float> f)
		{
			Audio temp=*this;
			int start=(int) f.first;
			int end=(int) f.second;
			int counter=0;
			//Clear, get new number of samples and resize
			temp.data_vector.clear();
			temp.noSamples = temp.noSamples - (end-start);
			temp.data_vector.resize(temp.noSamples);
			
			//Copy values til start
			for (int i = 0; i < start; ++i)
			{
				temp.data_vector[i].first = data_vector[i].first;
				temp.data_vector[i].second = data_vector[i].second;
			}
			
			//Copy values from start to end
			for (int j = start; j < temp.noSamples; ++j)
			{
				temp.data_vector[j].first = data_vector[end+counter].first;
				temp.data_vector[j].second = data_vector[end+counter].second;
				++counter;
			}
			
			return temp;
		}
		
	};
}
#endif //AUDIO_H
