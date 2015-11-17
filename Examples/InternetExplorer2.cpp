#include "stdafx.h"
#include "BrowserLib.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int choice;
	IExplorer InternetExplorer;
	SEARCHENGINE SearchEngine;
	while (1)
	{

		cout << "IEChecker" << endl;
		cout << "Commands: " << endl;
		cout << "1. Display IE Information" << endl;
		cout << "2. Change Search Engine" << endl;
		cout << "3. Toggle phishing filter" << endl;
		cout << "4. Set the Home Page" << endl;
		cout << "6. Change Install Path" << endl;
		cin >> choice;
		switch (choice)
		{
		case 1:
			if (exists("IExplorer") != 1)
			{
				cout << "Internet Explorer is not installed" << endl;
				break;
			}
			cout << "Version: " << InternetExplorer.GetVersion() << endl;
			cout << "Install Path: " << InternetExplorer.GetInstallPath() << endl;
			cout << "Home Page: " << InternetExplorer.GetHomePage() << endl;
			break;

		case 2:

			cout << "Search Engine Name" << endl;
			cin >> SearchEngine.DisplayName;
			cout << "Searh Url" << endl;
			cin >> SearchEngine.URL;
			cout << "Favicon:" << endl;
			cout << "1. Path" << endl;
			cout << "2. Url" << endl;
			cin >> choice;
			if (choice == 1)
			{
				cout << "Favicon Path" << endl;
				cin >> SearchEngine.FaviconPath;

			}
			if (choice == 2)
			{
				cout << "Favicon Url" << endl;
				cin >> SearchEngine.FaviconUrl;

			}
			cout << "Top Results Url" << endl;
			cin >> SearchEngine.TopResultUrl;
			cout << "Suggested Search Url" << endl;
			cin >> SearchEngine.SuggestionsUrl;
			InternetExplorer.SetSearchEngine(&SearchEngine);
			break;

		case 3:
			if (InternetExplorer.GetPhishingStatus(8) == 1)
			{
				InternetExplorer.TogglePhishFilter(0);
			}
			else
			{
				InternetExplorer.TogglePhishFilter(1);
			}

			break;

		case 4:
			WCHAR HomePage[256];
			cout << "Enter Home Page" << endl;
			wcin >> HomePage;
			InternetExplorer.SetHomePage(HomePage);

			break;
		case 5:
			WCHAR Path;
			cout << "Enter the path" << endl;
			wcin >> Path;
			if (InternetExplorer.ChangePath(Path) != 0)
			{
				cout << "Failed to set the path" << endl;
			}
		}
		system("pause");
		return 0;
	}
}
