#include "stdafx.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <string>
#include <map>
#include <shlobj.h>
#include "BrowserLib.h"


int main()
{

	if (exists("IExplorer")==1)
	{
		cout << "Internet Explorer is installed with version " <<iexplorer.GetVersion() << endl;
		cout << "Its home page is " << iexplorer.GetHomePage() << endl;
		cout << "Its exe path is set to" << iexplorer.GetInstallPath() << endl;
		cout << "It's default serch engine url is " << iexplorer.GetSearchEngine() << endl;
		if (iexplorer.GetPhishingStatus(80) == 1)
		{
			cout << "The phishing filter is on" << endl;
		}
}

system("pause");
return 0;
}
