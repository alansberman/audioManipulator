//UnitTests.cpp
//Author: Alan Berman
//15/05/2015

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include "Audio.h"
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

using namespace std;

TEST_CASE("Add 2 files","[]")
{
	string outFile = "bees18sec_44100_signed_8bit_mono.raw";
	string outFile2 = "frogss18sec_44100_signed_8bit_mono.raw";
	int sampleRate = 44100;
	BRMALA003::Audio<int8_t> bees(outFile,sampleRate);
	BRMALA003::Audio<int8_t> frogs(outFile2,sampleRate);
	BRMALA003::Audio<int8_t> newAudio = bees+frogs;
	//Produce file
	newAudio.saveAudio("beesAndFrogs",sampleRate);
	//Check files correct
	REQUIRE(newAudio.getNoSamples()==bees.getNoSamples());
	REQUIRE(newAudio.getDuration()==frogs.getDuration());
}

TEST_CASE("Volume factor a file","[]")
{
	string outFile = "bees18sec_44100_signed_16bit_stereo.raw";
	int sampleRate = 44100;
	BRMALA003::Audio<pair<int16_t,int16_t>> bees(outFile,sampleRate);
	pair<float,float>f = make_pair(0.2,0.5);
	BRMALA003::Audio<pair<int16_t,int16_t>> newAudio = bees*f;
	//Produce file
	newAudio.saveAudio("volumeBees",sampleRate);
	//Check files correct
	REQUIRE(newAudio.getNoSamples()==bees.getNoSamples());
	REQUIRE(newAudio.getDuration()==bees.getDuration());
}
TEST_CASE("Normalize a file","[]")
{
	string outFile = "bees18sec_44100_signed_8bit_mono.raw";
	int sampleRate = 44100;
	BRMALA003::Audio<int8_t> bees(outFile,sampleRate);
	float rms1 = 0.1;
	float rms2 = 0.2;
	float RMS = bees.computeRMS();
	BRMALA003::Audio<int8_t> newAudio = bees.normalize(rms1,rms2,RMS);
	//Produce file
	newAudio.saveAudio("volumeBees",sampleRate);
	//Check files correct
	REQUIRE(newAudio.getNoSamples()==bees.getNoSamples());
	REQUIRE(newAudio.getDuration()==bees.getDuration());
}

TEST_CASE("Cut a file across a range","[]")
{
	string outFile = "countdown40sec_44100_signed_16bit_stereo.raw";
	int sampleRate = 44100;
	BRMALA003::Audio<pair<int16_t,int16_t>> count(outFile,sampleRate);
	BRMALA003::Audio<pair<int16_t,int16_t>> countCopy(outFile,sampleRate);
	pair<float,float>f = make_pair(0,132300);
	BRMALA003::Audio<pair<int16_t,int16_t>> newAudio = count^(f);
	//Produce file
	newAudio.saveAudio("cutCount",sampleRate);
	//Check files correct
	REQUIRE(newAudio.getNoSamples()<count.getNoSamples());
}

TEST_CASE("Concatenate 2 files","[]")
{
	string outFile = "countdown40sec_44100_signed_8bit_stereo.raw";
	string outFile2 = "siren40sec_44100_signed_8bit_stereo.raw";
	int sampleRate = 44100;
	BRMALA003::Audio<pair<int8_t,int8_t>> count(outFile,sampleRate);
	BRMALA003::Audio<pair<int8_t,int8_t>> siren(outFile2,sampleRate);
	BRMALA003::Audio<pair<int8_t,int8_t>> newAudio = count|siren;
	//Produce file
	newAudio.saveAudio("countCatsiren",sampleRate);
	//Check files correct
	REQUIRE(newAudio.getNoSamples()>siren.getNoSamples());

}

//Reverse a file
TEST_CASE("Reverse ","[]")
{
	string outFile = "countdown40sec_44100_signed_8bit_mono.raw";
	int sampleRate = 44100;
	BRMALA003::Audio<int8_t> to_reverse(outFile,sampleRate);
	BRMALA003::Audio<int8_t> orig(outFile,sampleRate);
	to_reverse.reverseAudio();
	BRMALA003::Audio<int8_t> newAudio = to_reverse;
	//Produce file
	newAudio.saveAudio("reverse",sampleRate);
	REQUIRE(newAudio.getDuration()==orig.getDuration());
}


//Audio move constructor test
//Check resources released from rhs
TEST_CASE("Audio move constructor test","[]")
{
	string outFile = "countdown40sec_44100_signed_16bit_mono.raw";
	int sampleRate = 44100;
	BRMALA003::Audio<int16_t> aud_a(outFile,sampleRate);
	BRMALA003::Audio<int16_t> aud_a_moved=std::move(aud_a);
	BRMALA003::Audio<int16_t> aud_a_copy(outFile,sampleRate);
	REQUIRE(aud_a_moved.getNoSamples()==aud_a_copy.getNoSamples());
	REQUIRE(aud_a.getDataVector().size()==0);
	REQUIRE(aud_a.getNoSamples()==-1);
}
//Audio move assignment operator test
//Check resources released from rhs
TEST_CASE("Audio move assignment operator test","[]")
{
	string outFile = "countdown40sec_44100_signed_16bit_stereo.raw";
	string outFile2 = "countdown40sec_44100_signed_16bit_mono.raw";
	int sampleRate = 44100;
	BRMALA003::Audio<pair<int16_t,int16_t>> auda(outFile,sampleRate);
	BRMALA003::Audio<pair<int16_t,int16_t>> audb(outFile,sampleRate);
	audb=move(auda);
	BRMALA003::Audio<int16_t> aud_a_copy(outFile,sampleRate);
	//Check mono and stereo are different sizes
	REQUIRE(audb.getNoSamples()!=aud_a_copy.getNoSamples());
	REQUIRE(auda.getDataVector().size()==0);
	REQUIRE(auda.getNoSamples()==-1);
}
//Audio copy constructor test
TEST_CASE("Audio copy constructor test","[]")
{
	string outFile = "countdown40sec_44100_signed_8bit_stereo.raw";
	int sampleRate = 44100;
	BRMALA003::Audio<pair<int8_t,int8_t>> auda(outFile,sampleRate);
	BRMALA003::Audio<pair<int8_t,int8_t>> audb=auda;
	BRMALA003::Audio<pair<int8_t,int8_t>> auda_copy(outFile,sampleRate);
	REQUIRE(auda_copy.getNoSamples()==auda.getNoSamples());
	REQUIRE(auda_copy.getDuration()==audb.getDuration());
	REQUIRE(auda.getDataVector().size()!=0);
}
//Audio copy assignment operator test
TEST_CASE("Audio copy assignment operator test","[]")
{
	string outFile = "countdown40sec_44100_signed_8bit_stereo.raw";
	int sampleRate = 44100;
	BRMALA003::Audio<pair<int8_t,int8_t>> auda(outFile,sampleRate);
	BRMALA003::Audio<pair<int8_t,int8_t>> audb(outFile,sampleRate);
	audb=auda;
	BRMALA003::Audio<pair<int8_t,int8_t>> auda_copy(outFile,sampleRate);
	REQUIRE(auda_copy.getNoSamples()==audb.getNoSamples());
	REQUIRE(auda_copy.getDuration()==audb.getDuration());
	REQUIRE(auda.getDataVector().size()!=0);
	
}

