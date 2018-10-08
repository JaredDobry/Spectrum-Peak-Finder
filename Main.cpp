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
	vector<pair<double, vector<pair<double, double>>>> peakMap;
	int curPeakIndex = 1;
	string fileWritePath = "C:/Data/";
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
		//Check if we are on positive slope or not 
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
				//Add some of the outside low values for better fitting
				firstInSeries -= 5;
				if (firstInSeries < 0)
					firstInSeries = 0;

				secondInSeries += 5;
				if (secondInSeries > dataMap.size() - 1)
					secondInSeries = dataMap.size() - 1;

				//Place into output string
				//totalOutString += "Peak " + to_string(curPeakIndex) + "\n";
				curPeakIndex++;
				double maxVal = 0;
				vector<pair<double, double>> peakData;
				for (int j = firstInSeries; j <= secondInSeries; j++)
				{
					if (dataMap.at(j).first > maxVal)
						maxVal = dataMap.at(j).first;
					peakData.push_back(pair<double, double>(dataMap.at(j).first, dataMap.at(j).second));
				}
				peakMap.push_back(pair<double, vector<pair<double, double>>>(maxVal, peakData));
				it = secondInSeries;
			}
		}
	}
	//Sort peaks by largest size
	vector<pair<double, vector<pair<double, double>>>> sortedMap;
	vector<int> usedIndex;
	while (sortedMap.size() < peakMap.size())
	{
		int index;
		double largest = 0;
		for (int i = 0; i < peakMap.size(); i++)
		{
			if (peakMap.at(i).first > largest)
			{
				bool used = false;
				//Check hasnt been used
				for (int v : usedIndex)
				{
					if (v == i)
						used = true;
				}
				if (used == false)
				{
					largest = peakMap.at(i).first;
					index = i;
				}
			}
		}
		sortedMap.push_back(peakMap.at(index));
		usedIndex.push_back(index);
	}

	cout << "Found " + to_string(peakMap.size()) + " peaks, how many peak files to output? (Sorted largest to smallest intensity) ";
	int choice;
	cin >> choice;
	if (choice > peakMap.size())
	{
		cout << "Larger choice than number of peaks, defaulting to all peaks" << endl;
		choice = peakMap.size();
	}
	else if (choice <= 0)
		return 0;
	//Output peaks to files here
	for (int i = 0; i < choice; i++)
	{
		string out = "";
		ofstream outFile;
		outFile.open(fileWritePath + "Peak" + to_string(i) + ".txt");
		for (pair<double, double> d : peakMap.at(i).second) 
		{
			out += to_string(d.first) + " " + to_string(d.second) + "\n";
		}
		outFile << out;
		outFile.close();
	}
}

