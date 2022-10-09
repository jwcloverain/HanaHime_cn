#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "EnumFiles.h"
using namespace std;

void DumpText(string fileNmae)
{
	unsigned int lineCount = 1;
	char fstring[0xFF] = { 0 };
	vector<string> perLine;

	ifstream tFile(fileNmae);
	for (std::string str; getline(tFile, str); lineCount++)
	{
		if (str.size() > 0)
		{
			unsigned char a = str[0];
			if (a >= 0x7B)
			{
				sprintf_s(fstring, "Count:%08d \nRaw:", lineCount);
				str.insert(0, fstring);
				perLine.push_back(str);

			}
		}

	}
	tFile.close();

	ofstream oFile(fileNmae + ".txt");
	for (auto& vs : perLine)
	{
		oFile << vs << endl;
		oFile << "Tra:" << endl << endl;
	}
	oFile.close();
}

void InsetText()
{
	//To Be Continued
}

int main()
{
	string basePathA = ".\\";
	std::vector<std::string> filesNameA;
	EnumFilesA enumFileA(basePathA);
	filesNameA = enumFileA.GetFilesNameBasePath();

	for (auto& vf : filesNameA)
	{
		if (vf.find(".",2) == string::npos)
		{
			DumpText(vf);
		}
	}

}