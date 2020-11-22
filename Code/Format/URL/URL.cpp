#include "URL.h"
#include "Static/String.h"

using namespace std;
using namespace mx;

URL::URL(void) :
	m_usPort(0)
{
}

URL							URL::load(string& strURL)
{
	URL url;
	url._load(strURL);
	return url;
}

void						URL::_load(string& strURL2)
{
	string strURL = strURL2;
	string str;

	// protocol
	size_t uiPosition = strURL.find("://");
	if(uiPosition != string::npos)
	{
		str = strURL.substr(0, uiPosition);
		setProtocol(str);
		strURL = strURL.substr(uiPosition + 3);
	}
	else
	{
		str = MX_DEFAULT_URL_PROTOCOL;
		setProtocol(str);
	}
	
	// fragment
	uiPosition = strURL.find("#");
	if(uiPosition != string::npos)
	{
		str = strURL.substr(uiPosition);
		setFragment(str);
		strURL = strURL.substr(0, uiPosition);
	}
	else
	{
		str = "";
		setFragment(str);
	}
	
	// query
	uiPosition = strURL.find("?");
	if(uiPosition != string::npos)
	{
		str = strURL.substr(uiPosition);
		setQuery(str);
		strURL = strURL.substr(0, uiPosition);
	}
	else
	{
		str = "";
		setFragment(str);
	}
	
	// path
	uiPosition = strURL.find("/");
	if(uiPosition != string::npos)
	{
		str = strURL.substr(uiPosition);
		setPath(str);
		strURL = strURL.substr(0, uiPosition);
	}
	else
	{
		str = "/";
		setPath(str);
	}
	
	// port
	uiPosition = strURL.find(":");
	if(uiPosition != string::npos)
	{
		setPort(atoi(strURL.substr(uiPosition).c_str()));
		strURL = strURL.substr(0, uiPosition);
	}
	else
	{
		// todo - ftp, tls/ssl, etc.
		setPort(MX_DEFAULT_URL_HTTP_PORT);
	}
	
	// host
	uiPosition = strURL.find("/");
	if(uiPosition != string::npos)
	{
		str = strURL.substr(0, uiPosition);
		setHost(str);
		strURL = strURL.substr(uiPosition + 1);
	}
	else
	{
		setHost(strURL);
		strURL = "";
	}
}

vector<string>	URL::getHTTPHeaders(void)
{
	vector<string> vecHeaders;
	vecHeaders.push_back("GET " + getPath() + " HTTP/1.1");
	vecHeaders.push_back("Host: " + getHost());
	return vecHeaders;
}