//driver.cpp
// Author: Alan Berman
// 13/04/2015

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

//Driver that performs a variety of operations one/two 
//sound files
using namespace std;
int main(int argc, char * argv[])
{
	string file_one,file_two;
	string outFile = "out";
	//String used to parse args
	string s;
	int sampleRate, bitCount, noChannels, rangeOne, rangeTwo;
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
			i++;
			
		}
		if (s=="-b")
		{
			i++;
			//Extract the bitCount 
			s=string(argv[i]);
			bitCount = atoi(s.c_str());
			i++;
			
		}
		if (s=="-c")
		{
			i++;
			//Extract the number of channels
			s=string(argv[i]);
			noChannels = atoi(s.c_str());
			i++;
			
		}
		if (s=="-o")
		{
			i++;
			//Extract the output file name
			outFile=string(argv[i]);
			i++;
		}
		if (s=="-add")
		{
			i++;
			//Extract file 1's name from the args
			file_one=string(argv[i]);
			i++;
			//Extract file 2's name from the args
			file_two=string(argv[i]);
			//add
		}
		if (s=="-cut")
		{
			i++;
			
			//Extract range 1 from the args
			s=string(argv[i]);
			rangeOne = atoi(s.c_str());
			i++;
			//Extract range two from the args
			s=string(argv[i]);
			rangeTwo = atoi(s.c_str());
			//cut
		}
		if (s=="-radd")
		{
			i++;
			//Extract file 1's name from the args
			file_one=string(argv[i]);
			i++;
			//Extract file 2's name from the args
			file_two=string(argv[i]);
			//cut
		}
		
	}
	
	return 0;
}	
