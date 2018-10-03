#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main()
{
	double minimumValueReq;
	string in;
	cout << "Cutoff for minimum peak height: ";
	cin >> in;
	minimumValueReq = stod(in);
	vector<pair<double, double>> dataMap;
	int curPeakIndex = 1;
	string totalOutString = "";
	const char* fileWritePath = "C:/Data/DataOut.txt";
	const char* fileReadPath = "C:/Data/DataIn.txt";
	ifstream inFile;
	inFile.open(fileReadPath);
	//Do read in here and data analysis
	string s;
	//Read all data into our vector
	while (!inFile.eof())
	{
		getline(inFile, s);
		size_t spaceLocation = s.find_first_of("\t");
		if (spaceLocation == string::npos)
			continue;
		string firstNumS = s.substr(0, spaceLocation);
		string secondNumS = s.substr(spaceLocation + 1, s.length());
		double firstNum = stod(firstNumS);
		double secondNum = stod(secondNumS);
		dataMap.push_back(pair<double, double>(firstNum, secondNum));
	}
	inFile.close();
	//Do data analysis

	for (int it = 0; it < dataMap.size() - 2; it++) //-2 to deal with checking next 2 neighbors
	{
		//Search for things above the cutoff
		if (dataMap.at(it).first > minimumValueReq)
		{
			//Check that the next two values are below (local maximum caring for variance at peaks)
			if (dataMap.at(it + 1).first < dataMap.at(it).first && dataMap.at(it + 2).first < dataMap.at(it).first)
			{
				int firstInSeries;
				int secondInSeries;
				//New local maximum, find point before hand that has a negative value and point after
				bool foundFirst = false;
				bool foundSecond = false;
				int it2 = it - 1;
				int it3 = it + 1;
				while (!foundFirst)
				{
					if (dataMap.at(it2).first < 0 || it2 == 0)
					{
						firstInSeries = it2;
						foundFirst = true;
					}
					else
					{
						it2--;
					}
				}
				while (!foundSecond)
				{
					if (dataMap.at(it3).first < 0 || it2 == dataMap.size() - 1)
					{
						secondInSeries = it3;
						foundSecond = true;
					}
					else
					{
						it3++;
					}
				}
				//Place into output string
				//totalOutString += "Peak " + to_string(curPeakIndex) + "\n";
				curPeakIndex++;
				for (int j = firstInSeries; j <= secondInSeries; j++)
				{
					totalOutString += to_string(dataMap.at(j).first) + " " + to_string(dataMap.at(j).second) + "\n";
				}
				it = secondInSeries;
			}
		}
	}
	ofstream outFile;
	outFile.open(fileWritePath);
	outFile << totalOutString;
	outFile.close();
}

