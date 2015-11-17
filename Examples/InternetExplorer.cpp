#include "stdafx.h"
#include <fstream>
#include "BrowserLib.h"


void Log(string);

using namespace std;
int main()
{
	IExplorer InternetExplorer;
	string SearchEngine;
	WCHAR HomePage[256] = L"http://www.google.com";
	while (1)
	{
		if (InternetExplorer.GetHomePage() != "http://www.google.com")
		{
			string message = "Home page was changed";
			Log(message);
			InternetExplorer.SetHomePage(HomePage);
		}

		if (InternetExplorer.GetPhishingStatus(8) != 1)
		{
			string message = "Phishing filter was disabled";
			Log(message);
			InternetExplorer.TogglePhishFilter(1);
		}

		SearchEngine=InternetExplorer.GetSearchEngine();
		if (SearchEngine != "http://www.google.com")
		{
			string message = "The search engine was changed";
			Log(message);
		}
	}

	system("pause");
	return 0;
}

void Log(string message)
{
	ofstream LogFile;
	LogFile.open("Log.txt", ios::out | ios::app | );
	LogFile << message << endl;
	LogFile.close();
}
