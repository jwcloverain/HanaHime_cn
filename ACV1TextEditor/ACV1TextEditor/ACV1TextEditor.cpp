#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "EnumFiles.h"
using namespace std;

bool DumpText(string& strFileNmae)
{
	unsigned int lineCount = 1;

	ofstream transFile(strFileNmae + ".txt");
	ifstream scriptFile(strFileNmae);
	if (!transFile.is_open() || !scriptFile.is_open())
	{
		return false;
	}
	for (std::string line; getline(scriptFile, line); lineCount++)
	{
		if (line.size() > 0)
		{
			if ((unsigned char)line[0] >= 0x7B)
			{
				transFile << "Count:" << setw(8) << setfill('0') << lineCount << endl;
				transFile << "Raw:" << line << endl;
				transFile << "Tra:" << endl << endl;
			}
		}
	}
	scriptFile.close();
	transFile.close();

	return true;
}

bool InsetText(string& strFileName)
{
	unsigned int position = 0;
	vector<string> transFileLineList;
	vector<string> scriptFileLineList;
	vector<unsigned int> transTextPositionList;

	//Get Trans Text
	ifstream transFile(strFileName + ".txt");
	if (!transFile.is_open())
	{
		return false;
	}
	for (std::string line; getline(transFile, line);)
	{
		if (line.find("Count:") != string::npos)
		{
			sscanf_s(line.c_str(), "Count:%d", &position);
			getline(transFile, line);
			getline(transFile, line);
			line = line.substr(sizeof("Tra:") - 1);
			if (!line.empty())
			{
				transTextPositionList.push_back(position);
				transFileLineList.push_back(line);
			}
		}
	}
	transFile.close();

	//Read Script PerLine
	ifstream scriptFile(strFileName);
	if (!scriptFile.is_open())
	{
		return false;
	}

	for (std::string line; getline(scriptFile, line);)
	{
		scriptFileLineList.push_back(line);
	}
	scriptFile.close();

	//Out New Script File
	ofstream newScriptFile(strFileName + ".new", ios::binary);
	if (!newScriptFile.is_open())
	{
		return false;
	}
	//Replace Trans Text
	for (size_t i = 0; i < transTextPositionList.size(); i++)
	{
		scriptFileLineList[transTextPositionList[i] - 1] = transFileLineList[i];
	}
	 //Write Back ALL Line
	for (auto& line : scriptFileLineList)
	{
		newScriptFile << line << '\n';
	}
	newScriptFile.close();

	return true;
}

int main()
{
	bool isDump = FALSE;
	bool isInset = FALSE;
	char flag = 0;
	string basePathA = ".\\";
	std::vector<std::string> filesNameA;

	cout << "Input [ d ] to dump Text" << endl;
	cout << "Input [ i ] to insert Text" << endl;

	while (true)
	{
		EnumFilesA enumFileA(basePathA);
		filesNameA = enumFileA.GetFilesNameBasePath();

		cout << "\nInput:";
		cin >> flag;

		switch (flag)
		{
		case 'd':
			for (auto& f : filesNameA)
			{
				if (f.find(".", 2) == string::npos)
				{
					isDump = DumpText(f);
					if (isDump)
					{
						cout << "Dump:" << f << std::endl;
					}
					else
					{
						cout << "Failed:" << f << std::endl;
					}
				}
			}
			break;

		case 'i':

			for (auto& f : filesNameA)
			{
				if (f.find(".", 2) == string::npos)
				{
					isInset = InsetText(f);
					if (isInset)
					{
						cout << "Inset:" << f << std::endl;
					}
					else
					{
						cout << "Failed:" << f << std::endl;
					}
				}
			}
			break;

		default:
			cout << "Illegal instructions" << endl;
			break;
		}
	}
}
