//driver.cpp
//Author: Alan Berman
//30/04/2015
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Audio.h"
//Driver that performs a variety of operations one/two 
//sound files
using namespace std;
int main(int argc, char * argv[])
{
	string file_one,file_two;
	string outFile = "out";
	//String used to parse args
	string s;
	int sampleRate, bitCount, noChannels;
	float rangeOne=1.0, rangeTwo=1.0;
	if (argc < 5)
	{
		cout << "Not enough arguments given. Try again!" << endl;
		return 0;
	}
	for (int i=1;i<argc;i++)
	{
		//Identify the option chosen
		s = string(argv[i]);
		if 	(s=="-r")
		{
			i++;
			//Extract the sample rate
			s=string(argv[i]);
			sampleRate = atoi(s.c_str());
		
		}
		if (s=="-b")
		{
			i++;
			//Extract the bitCount 
			s=string(argv[i]);
			bitCount = atoi(s.c_str());			
		}
		if (s=="-c")
		{
			i++;
			//Extract the number of channels
			s=string(argv[i]);
			noChannels = atoi(s.c_str());	
		}
		if (s=="-o")
		{
			i++;
			//Extract the output file name
			outFile=string(argv[i]);
		
		}
		if (s=="-add")
		{
			i++;
			//Extract file 1's name from the args
			file_one=string(argv[i]);
			i++;
			//Extract file 2's name from the args
			file_two=string(argv[i]);
			//Mono audio file
			if (noChannels == 1)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<int8_t> fileA_8(file_one,sampleRate);
					BRMALA003::Audio<int8_t> fileB_8(file_two,sampleRate);
					cout << "Adding " << file_one << " to " << file_two << "..." << endl;
					BRMALA003::Audio<int8_t> AplusB = fileA_8+fileB_8;
					AplusB.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<int16_t> fileA_16(file_one,sampleRate);
					BRMALA003::Audio<int16_t> fileB_16(file_two,sampleRate);
					cout << "Adding " << file_one << " to " << file_two << "..." << endl;
					BRMALA003::Audio<int16_t> AplusB16 = fileA_16+fileB_16;
					AplusB16.saveAudio(outFile,sampleRate);
				}
			}
			//Stereo audio file
			if (noChannels == 2)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<pair<int8_t,int8_t>> file_A_8(file_one,sampleRate);
					BRMALA003::Audio<pair<int8_t,int8_t>> file_B_8(file_two,sampleRate);
					cout << "Adding " << file_one << " to " << file_two << "..." << endl;
					BRMALA003::Audio<pair<int8_t,int8_t>> A_plus_B = file_A_8+file_B_8;
					A_plus_B.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<pair<int16_t,int16_t>> file_A_16(file_one,sampleRate);
					BRMALA003::Audio<pair<int16_t,int16_t>> file_B_16(file_two,sampleRate);
					cout << "Adding " << file_one << " to " << file_two << "..." << endl;
					BRMALA003::Audio<pair<int16_t,int16_t>> A_plus_B16 = file_A_16+file_B_16;
					A_plus_B16.saveAudio(outFile,sampleRate);
				}
			}
			//add
			cout << "Done!" << endl;
			break;
		}
		if (s=="-cut")
		{
			i++;
			//Extract range 1 from the args
			s=string(argv[i]);
			rangeOne = stof(s.c_str());
			i++;
			//Extract range two from the args
			s=string(argv[i]);
			rangeTwo = stof(s.c_str());
			i++;
			//Extract file 1's name from the args
			file_one=string(argv[i]);
			//Mono audio file
			if (noChannels == 1)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<int8_t> file_8(file_one,sampleRate);
					cout << "Cutting " << file_one << " across range " << rangeOne << " to " << rangeTwo << "..." << endl;
					pair<float,float>f = make_pair(rangeOne,rangeTwo);
					BRMALA003::Audio<int8_t> cut = file_8^(f);
					cut.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<int16_t> file_16(file_one,sampleRate);
					cout << "Cutting " << file_one << " across range " << rangeOne << " to " << rangeTwo << "..." << endl;
					pair<float,float>f = make_pair(rangeOne,rangeTwo);
					BRMALA003::Audio<int16_t> cut16 = file_16^(f);
					cut16.saveAudio(outFile,sampleRate);
				}
			}
			//Stereo audio file
			if (noChannels == 2)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<pair<int8_t,int8_t>> file8(file_one,sampleRate);
					cout << "Cutting " << file_one << " across range " << rangeOne << " to " << rangeTwo << "..." << endl;
					pair<float,float>f = make_pair(rangeOne,rangeTwo);
					BRMALA003::Audio<pair<int8_t,int8_t>> cutstereo = file8^(f);
					cutstereo.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<pair<int16_t,int16_t>> file16(file_one,sampleRate);
					cout << "Cutting " << file_one << " across range " << rangeOne << " to " << rangeTwo << "..." << endl;
					pair<float,float>f = make_pair(rangeOne,rangeTwo);
					BRMALA003::Audio<pair<int16_t,int16_t>> cutStereo16 = file16^(f);
					cutStereo16.saveAudio(outFile,sampleRate);
				}
			}
			//cut
			cout << "Done!" << endl;
			break;
		}
		if (s=="-radd")
		{
			i++;
			//Extract range 1 from the args
			s=string(argv[i]);
			int rangeOne = atoi(s.c_str());
			i++;
			
			//Extract seconds 1
			s=string(argv[i]);
			int s1=atoi(s.c_str());
			i++;
			//Extract range two from the args
			s=string(argv[i]);
			int rangeTwo = atoi(s.c_str()); 
			
			i++;
			//Extract seconds 2
			s=string(argv[i]);
			int s2=atoi(s.c_str());
			i++;
			//Extract file 1's name from the args
			file_one=string(argv[i]);
			i++;
			//Extract file 2's name from the args
			file_two=string(argv[i]);
			i++;
			if ((s1 - rangeOne)!=(s2-rangeTwo))
			{
				cout << "Ranges not equal. Cannot perform ranged add operation." << endl;
				break;
			}

			//Mono audio file
			if (noChannels == 1)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<int8_t> fileAradd_8(file_one,sampleRate);
					BRMALA003::Audio<int8_t> fileBradd_8(file_two,sampleRate);
					cout << "Range Adding " << file_one << " to " << file_two  << endl;
					cout << "across range " << rangeOne << " " << s1;
					cout << " and " << rangeTwo << " " << s2  << endl;
					pair<float,float>a = make_pair(rangeOne,s1);
					pair<float,float>b = make_pair(rangeTwo,s2);
					BRMALA003::Audio<int8_t> AraddB = fileAradd_8.rangedAdd(fileBradd_8,sampleRate,a,b);
					AraddB.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<int16_t> fileAradd_16(file_one,sampleRate);
					BRMALA003::Audio<int16_t> fileBradd_16(file_two,sampleRate);
					cout << "Range Adding " << file_one << " to " << file_two  << endl;
					cout << "across range " << rangeOne << " " << s1;
					cout << " and " << rangeTwo << " " << s2  << endl;
					pair<float,float>a16 = make_pair(rangeOne,s1);
					pair<float,float>b16 = make_pair(rangeTwo,s2);
					BRMALA003::Audio<int16_t> AraddB16 = fileAradd_16.rangedAdd(fileBradd_16,sampleRate,a16,b16);
					AraddB16.saveAudio(outFile,sampleRate);
				}
			}
			//Stereo audio file
			if (noChannels == 2)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<pair<int8_t,int8_t>> file_A_8(file_one,sampleRate);
					BRMALA003::Audio<pair<int8_t,int8_t>> file_B_8(file_two,sampleRate);
					cout << "Range Adding " << file_one << " to " << file_two  << endl;
					cout << "across range " << rangeOne << " " << s1;
					cout << " and " << rangeTwo << " " << s2  << endl;
					pair<float,float>astereo = make_pair(rangeOne,s1);
					pair<float,float>bstereo = make_pair(rangeTwo,s2);
					BRMALA003::Audio<pair<int8_t,int8_t>> A_radd_B = file_A_8.rangedAdd(file_B_8,sampleRate,astereo,bstereo);
					A_radd_B.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<pair<int16_t,int16_t>> file_A_16(file_one,sampleRate);
					BRMALA003::Audio<pair<int16_t,int16_t>> file_B_16(file_two,sampleRate);
					cout << "Range Adding " << file_one << " to " << file_two  << endl;
					cout << "across range " << rangeOne << " " << s1;
					cout << " and " << rangeTwo << " " << s2  << endl;
					pair<float,float>astereo16 = make_pair(rangeOne,s1);
					pair<float,float>bstereo16 = make_pair(rangeTwo,s2);
					BRMALA003::Audio<pair<int16_t,int16_t>> A_radd_B16 = file_A_16.rangedAdd(file_B_16,sampleRate,astereo16,bstereo16);
					A_radd_B16.saveAudio(outFile,sampleRate);
				}
			}
			//radd
			cout << "Done!" << endl;
			break;
		}
		//Concatenate 2 audio files
		if (s=="-cat")
		{
			i++;
			//Extract file 1's name from the args
			file_one=string(argv[i]);
			i++;
			//Extract file 2's name from the args
			file_two=string(argv[i]);
			i++;
			//Mono audio file
			if (noChannels == 1)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<int8_t> fileA_8(file_one,sampleRate);
					BRMALA003::Audio<int8_t> fileB_8(file_two,sampleRate);
					cout << "Concatenating " << file_one << " with " << file_two << "..." << endl;
					BRMALA003::Audio<int8_t> AcatB = fileA_8|fileB_8;
					AcatB.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<int16_t> fileA_16(file_one,sampleRate);
					BRMALA003::Audio<int16_t> fileB_16(file_two,sampleRate);
					cout << "Concatenating " << file_one << " with " << file_two << "..." << endl;
					BRMALA003::Audio<int16_t> AcatB16 = fileA_16|fileB_16;
					AcatB16.saveAudio(outFile,sampleRate);
				}
			}
			//Stereo audio file
			if (noChannels == 2)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<pair<int8_t,int8_t>> file_A_8(file_one,sampleRate);
					BRMALA003::Audio<pair<int8_t,int8_t>> file_B_8(file_two,sampleRate);
					cout << "Concatenating " << file_one << " with " << file_two << "..." << endl;
					BRMALA003::Audio<pair<int8_t,int8_t>> A_cat_B = file_A_8|file_B_8;
					A_cat_B.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<pair<int16_t,int16_t>> file_A_16(file_one,sampleRate);
					BRMALA003::Audio<pair<int16_t,int16_t>> file_B_16(file_two,sampleRate);
					cout << "Concatenating " << file_one << " with " << file_two << "..." << endl;
					BRMALA003::Audio<pair<int16_t,int16_t>> A_cat_B16 = file_A_16|file_B_16;
					A_cat_B16.saveAudio(outFile,sampleRate);
				}
			}
			//cat
			cout << "Done!" << endl;
			break;
		}
		if (s=="-v")
		{
			i++;
			//Extract range 1 from the args
			s=string(argv[i]);
			rangeOne =stof(s.c_str());
			i++;
			//Extract range two from the args
			s=string(argv[i]);
			rangeTwo =stof(s.c_str());
			i++;
			//Extract file 1's name from the args
			file_one=string(argv[i]);
			i++;
			//Mono audio file
			if (noChannels == 1)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<int8_t> file_8_v(file_one,sampleRate);
					cout << "Changing " << file_one << "'s volume by " << rangeOne <<  "..." << endl;
					pair<float,float>f = make_pair(rangeOne,rangeTwo);
					BRMALA003::Audio<int8_t> vol = file_8_v*(f);
					vol.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<int16_t> file_16_v(file_one,sampleRate);
					cout << "Changing " << file_one << "'s volume by " << rangeOne <<  "..." << endl;
					pair<float,float>g = make_pair(rangeOne,rangeTwo);
					BRMALA003::Audio<int16_t> vol16 = file_16_v*(g);
					vol16.saveAudio(outFile,sampleRate);
				}
			}
			//Stereo audio file
			if (noChannels == 2)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<pair<int8_t,int8_t>> file8stereo(file_one,sampleRate);
					cout << "Changing " << file_one << "'s left channel volume by " << rangeOne << " and its right channel volume by" << rangeTwo <<  "..." << endl;
					pair<float,float>f = make_pair(rangeOne,rangeTwo);
					BRMALA003::Audio<pair<int8_t,int8_t>> volstereo = file8stereo*(f);
					volstereo.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<pair<int16_t,int16_t>> file16stereo(file_one,sampleRate);
					cout << "Changing " << file_one << "'s volume by " << rangeOne << " and " << rangeTwo <<  "..." << endl;
					pair<float,float>f = make_pair(rangeOne,rangeTwo);
					BRMALA003::Audio<pair<int16_t,int16_t>> volStereo16 = file16stereo*(f);
					volStereo16.saveAudio(outFile,sampleRate);
				}
			}
			//v
			cout << "Done!" <<endl;
			break;
		}
		if (s=="-rev")
		{
			i++;
			//Extract file 1's name from the args
			file_one=string(argv[i]);
			i++;
			//Mono audio file
			if (noChannels == 1)
			{

				if (bitCount == 8)
				{
					BRMALA003::Audio<int8_t> file_8(file_one,sampleRate);
					cout << "Reversing " << file_one <<  "..." << endl;
					BRMALA003::Audio<int8_t> reversed = file_8.reverseAudio();
					reversed.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<int16_t> file_16(file_one,sampleRate);
					cout << "Reversing " << file_one <<  "..." << endl;
					BRMALA003::Audio<int16_t> reversed16 = file_16.reverseAudio();
					reversed16.saveAudio(outFile,sampleRate);
				}
			}
			//Stereo audio file
			if (noChannels == 2)
			{
				if (bitCount == 8)
				{
					BRMALA003::Audio<pair<int8_t,int8_t>> file_8_stereo(file_one,sampleRate);
					cout << "Reversing " << file_one <<  "..." << endl;
					BRMALA003::Audio<pair<int8_t,int8_t>> reversedStereo = file_8_stereo.reverseAudio();
					reversedStereo.saveAudio(outFile,sampleRate);
				}
				else if (bitCount == 16)
				{
					BRMALA003::Audio<pair<int16_t,int16_t>> file_16_stereo(file_one,sampleRate);
					cout << "Reversing " << file_one <<  "..." << endl;
					BRMALA003::Audio<pair<int16_t,int16_t>> reversedStereo16 = file_16_stereo.reverseAudio();
					reversedStereo16.saveAudio(outFile,sampleRate);
				}
			}
			//rev
			cout << "Done!" << endl;
			break;
		}
		if (s=="-rms")
		{
			i++;
			//Extract file 1's name from the args
			file_one=string(argv[i]);
			i++;
			//rms
			break;
		}
		if (s=="-norm")
		{
			float rms1, rms2;
			i++;
			//Extract range 1 from the args
			s=string(argv[i]);
			rms1 = float(atoi(s.c_str()));
			i++;
			//Extract range two from the args
			s=string(argv[i]);
			rms2 = float(atoi(s.c_str()));
			i++;
			//Extract file 1's name from the args
			file_one=string(argv[i]);
			i++;
			//norm
			break;
		}
		
	}
	
	return 0;
}	
