using namespace std;
#include <fstream>


/*------------------------------------------------------*/
/*------------------------------------------------------*/
/************** Utility Functions************************/
/********************************************************/

wstring stringtowide(string sString) //Convert a string to a wide string
{
	wstring wide = wstring(sString.begin(), sString.end());
	return wide;
}



string ReadRegValue(HKEY rootkey, WCHAR regpath[256], WCHAR regkey[256]) //Read a registry value from a key
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
		return "77";
	}
	RegCloseKey(tempKey);
}


bool exists(string browser)
{
	if (browser == "IExplorer")
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
	if (browser == "Chrome")
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










//CHROME
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
if(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, appdata_path) != S_OK)
{
return "0"; // Error code
}
preferences_path = appdata_path;
preferences_path.append(L"\\Local\\Google\\Chrome\\User Data\\Default\\preferences.something");
wcout << preferences_path << endl;
ifstream filestream;
filestream.open(preferences_path.c_str());
if (filestream.is_open())
{
cout << "Failed to open preferenes file" << endl;
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

/*
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








//IEXPLORER

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
			cout << "Internet Explorer is Installed" << endl;
		}
	}

	virtual string GetHomePage();
	virtual string GetSearchEngine();
	virtual string GetVersion();
	virtual string GetInstallPath();
	virtual int GetPhishingStatus(int);
	//virtual map<string, string> GetPasswords();
	//virtual int IExplorer::GetAccountss();
	int ChangePath();
	virtual int SetSearchEngine(SEARCHENGINE *searchengine);
	virtual string SetHomePage(WCHAR home_page[256]);
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

string IExplorer::SetHomePage(WCHAR home_page[256])
{
	string a = "A";
	HKEY startpage_key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Internet Explorer\\MAIN", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &startpage_key) == ERROR_SUCCESS)
	{
		wcout << "wchar homepage: " << home_page << endl;
		if (RegSetValueEx(startpage_key, L"Start Page", 0, REG_SZ, (BYTE*)home_page, 256) == ERROR_SUCCESS)
		{
			RegCloseKey(startpage_key);
			return a;
		}
	}
	else
	{
		cout << GetLastError() << endl;
		RegCloseKey(startpage_key);
		return a;
	}
	RegCloseKey(startpage_key);
}

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
				cout << "Failed to modify the DefailtScope key." << endl;
			}

			if (RegCreateKeyEx(ScopeKey, stringtowide(searchengine->engine_name).c_str(), 0, NULL, NULL, KEY_ALL_ACCESS, NULL, &SubKey, NULL) == ERROR_SUCCESS) // Create the sub-key
			{
				cout << "Creating search engine keys.." << endl;
				//Display Name
				if (!searchengine->DisplayName.empty())
				{
					if (RegSetValueEx(SubKey, L"DisplayName", 0, REG_SZ, (BYTE *)stringtowide(searchengine->DisplayName).c_str(), 256) != ERROR_SUCCESS)
					{
						cout << "Failed to modify the DisplayName key" << endl;
					}
				}
				//Favicon PAth
				if (!searchengine->FaviconPath.empty())
				{
					if (RegSetValueEx(SubKey, L"FaviconPath", 0, REG_SZ, (BYTE *)stringtowide(searchengine->FaviconPath).c_str(), 256) != ERROR_SUCCESS)
					{
						cout << "Failed to modify the FaviconPath key" << endl;
					}
				}
				//FaviconUrl
				if (!searchengine->FaviconUrl.empty())
				{
					if (RegSetValueEx(SubKey, L"FaviconUrl", 0, REG_SZ, (BYTE *)stringtowide(searchengine->FaviconUrl).c_str(), 256) != ERROR_SUCCESS)
					{
						cout << "Failed to modify the FaviconURL key" << endl;
					}
				}
				//SuggestionsUrl
				if (!searchengine->SuggestionsUrl.empty())
				{
					if (RegSetValueEx(SubKey, L"SuggestionsUrl", 0, REG_SZ, (BYTE *)stringtowide(searchengine->SuggestionsUrl).c_str(), 256) != ERROR_SUCCESS)
					{
						cout << "Failed to modify the SuggestionsUrl key" << endl;
					}
				}
				//TopResultUrl
				if (!searchengine->TopResultUrl.empty())
				{
					if (RegSetValueEx(SubKey, L"TopResultUrl", 0, REG_SZ, (BYTE *)stringtowide(searchengine->TopResultUrl).c_str(), 256) != ERROR_SUCCESS)
					{
						cout << "Failed to modify the TopResultUrl key" << endl;
					}
				}
				//URL
				if (!searchengine->URL.empty())
				{
					if (RegSetValueEx(SubKey, L"URL", 0, REG_SZ, (BYTE *)stringtowide(searchengine->URL).c_str(), 256) != ERROR_SUCCESS)
					{
						cout << "Failed to modify the URL key" << endl;
					}
				}
				return 0;
			}
			else
			{
				cout << "Failed to set the search engine" << endl;
				cout<<"Error Code: "<<GetLastError() << endl;
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
		cout << "Failed setting the Phishing Filter" << endl;
		cout<<"Error Code: "<<GetLastError() << endl;
		RegCloseKey(hKey);
		return FALSE;
	}
	RegCloseKey(hKey);
}

/*
int IExplorer::GetAccountss()
{
wstring appdata_path, history_path;
wstring history_name, website_name;
string website_hash;
size_t text_start, text_stop; // Used when obtaining the text between () in the history filename
vector<wstring> directories; // Container to keep track of directories
HANDLE search_handle; // Handle to the file searcher
WIN32_FIND_DATA file_data; // File information from FindNExtFile is stored here
HKEY hkeyIntelli; // Handle to the IntelliForms registry key
//Check IE Version
//IF IE.Version>=7
//Open intellisense reg key
if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\IntelliForms\\Storage2", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkeyIntelli) == ERROR_SUCCESS)
{
//Open History folder
cout << "Starting password finder" << endl;
directories.push_back(history_path); //
for (int i(0), j(0); i <=j ; i++) //Iterate the History folder folders. j keeps track of the # of folders
{
search_handle = FindFirstFile(directories[i].c_str(), &file_data);
if (search_handle == INVALID_HANDLE_VALUE)
{
//Pass exception
cout << "Failed finding first directory" << endl;
}
directories.pop_back();
while (FindNextFile(search_handle,&file_data))
{
cout << "In while Loop" << endl;
history_name = file_data.cFileName;
text_start = history_name.find(L"(");
if (text_start != string::npos)
{
cout << "In first text if" << endl;
text_stop = history_name.find(L")");
if (text_stop != string::npos)
{
cout << "In second text if" << endl;
++text_start;
++text_stop;
website_name = history_name.substr(text_start, text_stop - text_start);
//website_hash = HashWebsite(website_name);
//CheckRegistryMatches(website_hash);

}
else
{
cout << "Failed to get end of history name" << endl;
}

}
else
{
cout << "Failed to get start of history name" << endl;
}

}

}
//Iterate Outer Folder - 3weeks, 2weeks, etc
//Iterate Inner folder
//Get folder name
//Extract inbetween ()
//Encrypt
//Iterate through intelisense & check for match
//If match - do something
//else repeat

}
FindClose(search_handle);
}*/