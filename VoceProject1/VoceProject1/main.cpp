/*************************************************************************
*                                                                       *
* Voce                                                                  *
* Copyright (C) 2005                                                    *
* Tyler Streeter  tylerstreeter@gmail.com                               *
* All rights reserved.                                                  *
* Web: voce.sourceforge.net                                             *
*                                                                       *
* This library is free software; you can redistribute it and/or         *
* modify it under the terms of EITHER:                                  *
*   (1) The GNU Lesser General Public License as published by the Free  *
*       Software Foundation; either version 2.1 of the License, or (at  *
*       your option) any later version. The text of the GNU Lesser      *
*       General Public License is included with this library in the     *
*       file license-LGPL.txt.                                          *
*   (2) The BSD-style license that is included with this library in     *
*       the file license-BSD.txt.                                       *
*                                                                       *
* This library is distributed in the hope that it will be useful,       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
* license-LGPL.txt and license-BSD.txt for more details.                *
*                                                                       *
*************************************************************************/
#include "voce.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <tchar.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <time.h>
#include <chrono>
#include <thread>
#include <stdlib.h>

using namespace std;

#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <Windows.h>

#ifdef UNICODE_WAS_UNDEFINEDs
#undef UNICODE
#endif


vector<string> findWav(wstring path);
int playedNumber = 0;
int popCounter = 0;

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<string> split(const string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}

float calcWER(string reference, string hypothesis){

	float WER = 0;

	//	string reference = "";
	//string hypothesis = "who is there";

	vector<string> ref = split(reference, ' ');
	vector<string> hyp = split(hypothesis, ' ');



	//rezerwacja pamieci na macierz D
	int** D = new int*[ref.size() + 1];
	for (int i = 0; i < ref.size() + 1; i++){
		D[i] = new int[hyp.size() + 1];
	}

	for (int i = 0; i < ref.size() + 1; i++){
		for (int j = 0; j < hyp.size() + 1; j++){
			if (i == 0) {
				D[0][j] = j;

			}
			else if (j == 0){
				D[i][0] = i;
			}

		}
	}

	float substitution, insertion, deletion;

	for (int i = 1; i < ref.size() + 1; i++){
		for (int j = 1; j < hyp.size() + 1; j++){
			if (ref[i - 1] == hyp[j - 1]){
				D[i][j] = D[i - 1][j - 1];
			}
			else{
				substitution = D[i - 1][j - 1] + 1;
				insertion = D[i][j - 1] + 1;
				deletion = D[i - 1][j] + 1;
				D[i][j] = (std::min)({ substitution, insertion, deletion });
			}
		}
	}

	std::cout << D[ref.size()][hyp.size()];

	if (ref.size() > hyp.size()) WER = (float)D[ref.size()][hyp.size()] / (float)ref.size();
	else WER = (float)D[ref.size()][hyp.size()] / (float)hyp.size();


	//zwolnienie pamieci macierzy D
	for (int i = 0; i < ref.size() + 1; i++){
		delete[] D[i];
	}
	delete[] D;

	return WER;
}

/* synteza
int main(){
	voce::init("G:\\STUDIA W4\\Informatyka w gospodarce PROJEKT\\VOCE\\voce-0.9.1\\lib", true,false, "..\\grammar", "seat");

	voce::synthesize("seat");
	Sleep(70);
	voce::synthesize("raise heat by");
	Sleep(70);
	voce::synthesize("two");
	Sleep(70);
	voce::synthesize("degrees");


	cin.get();

	return 0;

}
*/


void play(vector<string> files){
	for (auto& it = files.begin(); it != files.end();){
		if (popCounter == playedNumber){
			wstring wstr(it->begin(), it->end());
			wstr = L"G:\\STUDIA W4\\Informatyka w gospodarce PROJEKT\\VOCE\\VoceProject1\\nagrania\\seat\\wbudowany-mic\\" + wstr;
			//wcout << wstr << endl;
			PlaySound(wstr.c_str(), NULL, SND_FILENAME | SND_SYNC);
			cout << "Reference: " << *it << endl;
			Sleep(1000);
			playedNumber++;
			it++;
		}
	
	}
}

void rec(){
	voce::init("G:\\STUDIA W4\\Informatyka w gospodarce PROJEKT\\VOCE\\voce-0.9.1\\lib", true, true, "..\\grammar", "seat");
	bool exit = false;
	int popCounter = 0;
	while (popCounter < 1){ // koniec po 8 wypowiedziach
		while (voce::getRecognizerQueueSize() > 0)
		{
			std::string s = voce::popRecognizedString();

			if (std::string::npos != s.rfind("exit"))
			{
				exit = true;
			}

			std::cout << "You said: " << s << std::endl;
			//	hypothesiss.push_back(s);
			++popCounter;
		}
	}
}

int main(int argc, char **argv)
{

	ofstream outputFile;
	string filename = ".\\dane\\seat-syntezator4.csv";
	bool exit = false;

	voce::init("G:\\STUDIA W4\\Informatyka w gospodarce PROJEKT\\VOCE\\voce-0.9.1\\lib", true, true, "..\\grammar", "seat");


	vector<string> wavFiles;
	vector<string> hypothesiss;
	wavFiles = findWav(L"G:\\STUDIA W4\\Informatyka w gospodarce PROJEKT\\VOCE\\VoceProject1\\nagrania\\seat\\wbudowany-mic\\");
	int filestotal = wavFiles.size();
	thread playthread(play, wavFiles);
	
	/*
	for (auto& it = wavFiles.begin(); it != wavFiles.end(); ++it){
			wstring wstr(it->begin(), it->end());
			wstr = L"G:\\STUDIA W4\\Informatyka w gospodarce PROJEKT\\VOCE\\VoceProject1\\nagrania\\" + wstr;
			wcout << wstr << endl;
			//2 threads
		//	thread recthread(rec);
		//	thread playthread(play,wstr);
			//PlaySound(wstr.c_str(), NULL, SND_FILENAME | SND_SYNC);
			cout << "Reference: " << *it << endl;

		//	playthread.join();
		}
	*/

		
	

	while (popCounter < filestotal){
		Sleep(1000);
		if (playedNumber > popCounter && voce::getRecognizerQueueSize() != NULL){
			while (voce::getRecognizerQueueSize() > 0)
			{
				cout << "recognizing" << endl;
				std::string s = voce::popRecognizedString();
				std::cout << "You said: " << s << std::endl;
				hypothesiss.push_back(s);
				Sleep(1000);
				++popCounter;

			}
		}
		else if (playedNumber > popCounter){
			hypothesiss.push_back("");
			cout << "pushing blanks" << endl;
			std::cout << "You said: " << std::endl;
			Sleep(1000);
			++popCounter;
		}
	}

		if (playthread.joinable()) playthread.join();

		outputFile.open(filename);
		outputFile << "REFERENCE; HYPOTHESIS; WER \n";
		
		float WER = 0;
		int i = 0;
		for (auto& it = wavFiles.begin(); it != wavFiles.end(); ++it){
			it->erase(it->end() - 4, it->end());
			if (i < popCounter){
				cout << *it << " " << *(hypothesiss.begin() + i) << endl;
				WER = calcWER(*it, *(hypothesiss.begin() + i)); //UWAGA niebezpieczne
				cout << "WER: " << WER << endl;

				outputFile << *it << ";" << *(hypothesiss.begin() + i) << ";" << WER << "\n";
				++i;
			}
		}

	//	wstring wstr(it->begin(), it->end());
	//	wstr = L"G:\\STUDIA W4\\Informatyka w gospodarce PROJEKT\\VOCE\\VoceProject1\\nagrania\\" + wstr;
	//	wcout << wstr << endl;
	//	PlaySound(wstr.c_str(), NULL, SND_FILENAME | SND_SYNC);
	//cout << "Reference: " << *it << endl;
	
	outputFile.close();
	cin.get();
	voce::destroy();
	return 0;
}



/*
int main(){
	vector<string> results;
	results = findWav(L"C:\\Users\\Marek\\Desktop\\pupa\\");
	cout << results[1];
	cin.get();
	return 0;
}*/

vector<string> findWav(wstring path){
	WIN32_FIND_DATA searchData;

	wstring::iterator p = path.end() - 1;
	wstring find = L"*.wav";

	while (p != path.begin()){
		if (*p != '\\'){
			path.erase(path.end() - 1);
		}
		else if (*p == '\\'){
			path += find;
			break;
		}
		--p;
	}

	vector<string> result;

	HANDLE sFile = FindFirstFile(path.c_str(), &searchData);
	wstring temp = searchData.cFileName;
	result.push_back(string(temp.begin(), temp.end()));

	while (FindNextFile(sFile, &searchData)){
		temp = searchData.cFileName;
		result.push_back(string(temp.begin(), temp.end()));
	}

	return result;
}



/*
while (voce::getRecognizerQueueSize() > 0)
{
	std::string s = voce::popRecognizedString();

	//porownaj teksty
	//oblicz WER


	// Check if the string contains 'exit'.
	if (std::string::npos != s.rfind("exit"))
	{
		exit = true;
	}

	/*
	if (std::string::npos != s.rfind("seat")){

	if (std::string::npos != s.rfind("move")){

	if (std::string::npos != s.rfind("right")){
	//	voce::synthesize("moving right");
	}

	if (std::string::npos != s.rfind("left")){
	//	voce::synthesize("moving left");
	}

	if (std::string::npos != s.rfind("forward")){
	//	voce::synthesize("moving forward");
	}

	}

	if (std::string::npos != s.rfind("lean")){

	if (std::string::npos != s.rfind("forward")){
	//	voce::synthesize("leaning forward");
	}

	if (std::string::npos != s.rfind("backward")){
	//	voce::synthesize("leaning back");
	}


	}
	}
	

	std::cout << "You said: " << s << std::endl;
	//voce::synthesize(s);
}
	}*/

