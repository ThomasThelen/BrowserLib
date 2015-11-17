#include <fstream>
#include <Windows.h>
#include <string>
#include <iostream>
using std::cout;
using std::cin;
using std::wcin;
using std::endl;
using std::string;
using std::wstring;

/********************************************************/
/********************************************************/
/*Author: Thomas Thelen                                 */
/*Project: https://github.com/ThomasThelen/BrowserLib   */
/********************************************************/

/*------------------------------------------------------*/
/*------------------------------------------------------*/
/************** Utility Functions************************/
/********************************************************/


wstring stringtowide(string sString) //Convert a string to a wide string 
{
	wstring wide = wstring(sString.begin(), sString.end());
	return wide;
}


string ReadRegValue(HKEY rootkey, WCHAR regpath[256], WCHAR regkey[256])
{
	HKEY tempKey;
	WCHAR regdata[256];
	DWORD regdatasize = sizeof(regdata);
	char regdatac[256];
	char junk = ' ';
	string registrydata;
	if (RegOpenKeyEx(rootkey, regpath, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &tempKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueExW(tempKey, regkey, NULL, NULL, (LPBYTE)regdata, &regdatasize) == ERROR_SUCCESS)
		{
			WideCharToMultiByte(CP_ACP, 0, regdata, -1, regdatac, 260, &junk, NULL);
			registrydata = regdatac;
			RegCloseKey(tempKey);
			return registrydata;
		}
	}
	else
	{
		RegCloseKey(tempKey);
		return "0";
	}
	RegCloseKey(tempKey);
}


bool exists(string browser)
{
	if (browser == "IExplorer" | browser=="InternetExplorer"|browser=="IE"|browser=="Internet Explorer" | browser=="internetexplorer"|browser=="internet explorer")
	{
		WCHAR InstallPath[256] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE";
		WCHAR InstallKey[256] = L"";
		if (ReadRegValue(HKEY_LOCAL_MACHINE, InstallPath, InstallKey) != "77")
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	if (browser == "Chrome" | browser=="chrome")
	{
		WCHAR InstallPath[256] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe";
		WCHAR InstallKey[256] = L"";
		if (ReadRegValue(HKEY_LOCAL_MACHINE, InstallPath, InstallKey) != "77")
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}



//Internet Explorer

struct SEARCHENGINE
{
	string DisplayName // 
		, FaviconPath // Local path the the search engine favicon
		, FaviconUrl // Remote path to the search engine favicon
		, SuggestionsUrl
		, TopResultUrl
		, URL
		, engine_name;
};

class IExplorer
{
public:

	IExplorer()
	{
		if (exists("IExplorer")==1)
		{
			std::cout << "Internet Explorer is Installed" << std::endl;
		}
	}

	virtual string GetHomePage();
	virtual string GetSearchEngine();
	virtual string GetVersion();
	virtual string GetInstallPath();
	virtual int GetPhishingStatus(int);
	int ChangePath(WCHAR);
	virtual int SetSearchEngine(SEARCHENGINE *searchengine);
	virtual int SetHomePage(WCHAR home_page[256]);
	int TogglePhishFilter(int set_value);


}iexplorer;


/*------------------------------------------------------*/
/*------------------------------------------------------*/
/************** Get Version *****************************/
/********************************************************/
string IExplorer::GetVersion()
{
	WCHAR homepath[256] = L"SOFTWARE\\Microsoft\\Internet Explorer";
	WCHAR homekey[256] = L"svcVersion";
	return ReadRegValue(HKEY_LOCAL_MACHINE, homepath, homekey);
}

/*------------------------------------------------------*/
/*------------------------------------------------------*/
/***************Get Install Path*************************/
/********************************************************/
string IExplorer::GetInstallPath()
{
	WCHAR InstallPath[256] = L"SOFTWARE\\Microsoft\\Internet Explorer\\MAIN";
	WCHAR InstallKey[256] = L"x86AppPath";
	return ReadRegValue(HKEY_LOCAL_MACHINE, InstallPath, InstallKey);
}

/*------------------------------------------------------*/
/*------------------------------------------------------*/
/***************Get Home Page****************************/
/********************************************************/

string IExplorer::GetHomePage()
{
	WCHAR homepath[256]=L"SOFTWARE\\Microsoft\\Internet Explorer\\MAIN";
	WCHAR homekey[256] = L"Start Page";
	return ReadRegValue(HKEY_CURRENT_USER, homepath, homekey);
}

/*------------------------------------------------------*/
/*------------------------------------------------------*/
/***************Get Search Engine************************/
/********************************************************/

string IExplorer::GetSearchEngine()
{
	wchar_t default_engine[256], current_engine[256];
	wstring search_scope, search_page, search_page_location;
	char csearch_engine[256];
	HKEY search_key;
	string search_engine;
	char junk = ' ';
	DWORD default_engine_size = sizeof(default_engine);
	search_scope = L"SOFTWARE\\Microsoft\\Internet Explorer\\SearchScopes";
	if (RegOpenKeyEx(HKEY_CURRENT_USER, search_scope.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &search_key) == ERROR_SUCCESS)
	{
		if (RegQueryValueExW(search_key, L"DefaultScope", NULL, NULL, (LPBYTE)default_engine, &default_engine_size) == ERROR_SUCCESS)
		{
			RegCloseKey(search_key);
			//construct the new registry path
			search_page_location = search_scope + L"\\" + default_engine;
			if (RegOpenKeyEx(HKEY_CURRENT_USER, search_page_location.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &search_key) == ERROR_SUCCESS)
			{
				DWORD search_page_size = sizeof(current_engine);
				if (RegQueryValueExW(search_key, L"URL", NULL, NULL, (LPBYTE)current_engine, &search_page_size) == ERROR_SUCCESS)
				{
					WideCharToMultiByte(CP_ACP, 0, current_engine, -1, csearch_engine, 260, &junk, NULL);
					search_engine = csearch_engine;
					RegCloseKey(search_key);
					return search_engine;
				}
			}
		}
	}
	else
	{
		RegCloseKey(search_key);
		return search_engine;
	}
	RegCloseKey(search_key);
}

/*------------------------------------------------------*/
/*------------------------------------------------------*/
/********Get Phishing Fileter Status ********************/
/********************************************************/
int IExplorer::GetPhishingStatus(int pversion)
{
	WCHAR InstallPath[256] = L"SOFTWARE\\Microsoft\\Internet Explorer\\PhishingFilter";
	string presult;
	if (pversion == 8)
	{
		WCHAR InstallKey[256] = L"EnabledV8";
		presult= ReadRegValue(HKEY_CURRENT_USER, InstallPath, InstallKey);
	}
	else
	{
	WCHAR InstallKey[256] = L"EnabledV9";
	presult = ReadRegValue(HKEY_CURRENT_USER, InstallPath, InstallKey);

	}
	if (presult == "\x2")
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/*------------------------------------------------------*/
/*------------------------------------------------------*/
/***************Set Home Page****************************/
/********************************************************/

int IExplorer::SetHomePage(WCHAR home_page[256])
{
	
	HKEY startpage_key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Internet Explorer\\MAIN", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &startpage_key) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(startpage_key, L"Start Page", 0, REG_SZ, (BYTE*)home_page, 256) == ERROR_SUCCESS)
		{
			RegCloseKey(startpage_key);
			return 0;
		}
	}
	else
	{
		std::cout << GetLastError() << std::endl;
		RegCloseKey(startpage_key);
		return 1;
	}
}

/*------------------------------------------------------*/
/*------------------------------------------------------*/
/***************Set Search Engine************************/
/********************************************************/
/*
Current Status:
Completed - Needs testing
*/

int IExplorer::SetSearchEngine(SEARCHENGINE *searchengine) // Call as SetSearchEngine(&searchengine)
{
	HKEY ScopeKey, SubKey, TempKey;
	wstring search_scope = L"SOFTWARE\\Microsoft\\Internet Explorer\\SearchScopes";
	if (RegOpenKeyEx(HKEY_CURRENT_USER, search_scope.c_str(), 0, KEY_ALL_ACCESS, &ScopeKey) == ERROR_SUCCESS) // Open SearchScopes
	{
		// Check to see if engine name is specified
		if (!searchengine->engine_name.empty())
		{
			// Overwrite the current search engine to the new one
			if (RegSetValueEx(ScopeKey, L"DefaultScope", 0, REG_SZ, (BYTE *)stringtowide(searchengine->engine_name).c_str(), 256) != ERROR_SUCCESS)
			{
				std::cout << "Failed to modify the DefailtScope key." << std::endl;
			}

			if (RegCreateKeyEx(ScopeKey, stringtowide(searchengine->engine_name).c_str(), 0, NULL, NULL, KEY_ALL_ACCESS, NULL, &SubKey, NULL) == ERROR_SUCCESS) // Create the sub-key
			{
				std::cout << "Creating search engine keys.." << std::endl;
				//Display Name
				if (!searchengine->DisplayName.empty())
				{
					if (RegSetValueEx(SubKey, L"DisplayName", 0, REG_SZ, (BYTE *)stringtowide(searchengine->DisplayName).c_str(), 256) != ERROR_SUCCESS)
					{
						std::cout << "Failed to modify the DisplayName key" << std::endl;
					}
				}
				//Favicon PAth
				if (!searchengine->FaviconPath.empty())
				{
					if (RegSetValueEx(SubKey, L"FaviconPath", 0, REG_SZ, (BYTE *)stringtowide(searchengine->FaviconPath).c_str(), 256) != ERROR_SUCCESS)
					{
						std::cout << "Failed to modify the FaviconPath key" << std::endl;
					}
				}
				//FaviconUrl
				if (!searchengine->FaviconUrl.empty())
				{
					if (RegSetValueEx(SubKey, L"FaviconUrl", 0, REG_SZ, (BYTE *)stringtowide(searchengine->FaviconUrl).c_str(), 256) != ERROR_SUCCESS)
					{
						std::cout << "Failed to modify the FaviconURL key" << std::endl;
					}
				}
				//SuggestionsUrl
				if (!searchengine->SuggestionsUrl.empty())
				{
					if (RegSetValueEx(SubKey, L"SuggestionsUrl", 0, REG_SZ, (BYTE *)stringtowide(searchengine->SuggestionsUrl).c_str(), 256) != ERROR_SUCCESS)
					{
						std::cout << "Failed to modify the SuggestionsUrl key" << std::endl;
					}
				}
				//TopResultUrl
				if (!searchengine->TopResultUrl.empty())
				{
					if (RegSetValueEx(SubKey, L"TopResultUrl", 0, REG_SZ, (BYTE *)stringtowide(searchengine->TopResultUrl).c_str(), 256) != ERROR_SUCCESS)
					{
						std::cout << "Failed to modify the TopResultUrl key" << std::endl;
					}
				}
				//URL
				if (!searchengine->URL.empty())
				{
					if (RegSetValueEx(SubKey, L"URL", 0, REG_SZ, (BYTE *)stringtowide(searchengine->URL).c_str(), 256) != ERROR_SUCCESS)
					{
						std::cout << "Failed to modify the URL key" << std::endl;
					}
				}
				return 0;
			}
			else
			{
				std::cout << "Failed to set the search engine" << endl;
				std::cout << "Error Code: " << GetLastError() << endl;
				RegCloseKey(SubKey);
				RegCloseKey(ScopeKey);
				return 63; //Could not create the search engine key directory
			}
		}
		else
		{
			RegCloseKey(ScopeKey);
			return 61; // Searh engine name not specified
		}
	}
	else
	{
		RegCloseKey(ScopeKey);
		return 60; // Could not open the registry
	}
}

/*------------------------------------------------------*/
/*------------------------------------------------------*/
/***************Toggle Phishing Filter*******************/
/********************************************************/
/*
Current Status:
Completed - Needs testing
*/

int IExplorer::TogglePhishFilter(int set_value) // Pass TRUE to turn on-FALSE to turn off
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Internet Explorer\\PhishingFilter", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD new_key;
		if (set_value == FALSE)
		{
			new_key = 00000001;
		}
		else
		{
			new_key = 00000002;
		}

		if (RegSetValueEx(hKey, L"EnabledV8", 0, REG_DWORD, (const BYTE*)&new_key, sizeof(new_key)) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return TRUE;
		}
		else if (RegSetValueEx(hKey, L"EnabledV9", 0, REG_DWORD, (const BYTE*)&new_key, sizeof(new_key)) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return TRUE;
		}
	}
	else
	{
		std::cout << "Failed setting the Phishing Filter" << endl;
		std::cout << "Error Code: " << GetLastError() << endl;
		RegCloseKey(hKey);
		return FALSE;
	}
	RegCloseKey(hKey);
}



int IExplorer::ChangePath(WCHAR NewPath)
{
	WCHAR InstallPath[256] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE";
	HKEY PathKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, InstallPath, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &PathKey) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(PathKey, L"", 0, REG_SZ, (BYTE*)NewPath, 256) == ERROR_SUCCESS)
		{
			return 0;
		}
	}
	else
		return 1;
}

/*
class Chrome
{
public:

	Chrome()
	{

	}
	virtual string GetHomePage();
}chrome;



string Chrome::GetHomePage()
{
	// Get environment variables
	wchar_t username, appdata_path[256];
	wstring preferences_path;
	string file_line;
	if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, appdata_path) != S_OK)
	{
		return "0"; // Error code
	}
	preferences_path = appdata_path;
	preferences_path.append(L"\\Local\\Google\\Chrome\\User Data\\Default\\preferences.something");
	std::wcout << preferences_path << endl;
	ifstream filestream;
	filestream.open(preferences_path.c_str());
	if (filestream.is_open())
	{
		std::cout << "Failed to open preferenes file" << endl;
		return "0";
	}
	while (getline(filestream, file_line))
	{
		if (file_line.find("hompepage"))
		{
			//Exctract the url from the shit
		}
	}
	return "aaa";
}


string Chrome::SetHomePage()
{
ifstream  myFile;
string master_p = ""; // File location of master_pref
string home_page; // Current home page
string sig = "restore_on_startup_migrated"
while (getline(fileInput, line)) // Read each line
{
if (line.find(sig, 0) != string::npos)  // Until it find the signature
{
cout << "found: " << search << "line: " << curLine << endl;
}

}
myFile.close();
}
*/



