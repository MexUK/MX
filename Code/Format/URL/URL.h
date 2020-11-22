#pragma once

#include "mx.h"
#include <string>
#include <vector>

#define MX_DEFAULT_URL_PROTOCOL		"http"
#define MX_DEFAULT_URL_HTTP_PORT	80

class mx::URL
{
public:
	URL(void);
	
	static URL					load(std::string& strURL);
	std::vector<std::string>	getHTTPHeaders(void);
	
	void						setProtocol(std::string& strProtocol) { m_strProtocol = strProtocol; }
	std::string					getProtocol(void) { return m_strProtocol; }
	
	void						setHost(std::string& strHost) { m_strHost = strHost; }
	std::string					getHost(void) { return m_strHost; }
	
	void						setPort(uint16 usPort) { m_usPort = usPort; }
	uint16						getPort(void) { return m_usPort; }
	
	void						setPath(std::string& strPath) { m_strPath = strPath; }
	std::string					getPath(void) { return m_strPath; }
	
	void						setQuery(std::string& strQuery) { m_strQuery = strQuery; }
	std::string					getQuery(void) { return m_strQuery; }
	
	void						setFragment(std::string& strFragment) { m_strFragment = strFragment; }
	std::string					getFragment(void) { return m_strFragment; }
	
private:
	void						_load(std::string& strURL);

private:
	std::string					m_strProtocol;
	std::string					m_strHost;
	uint16						m_usPort;
	std::string					m_strPath;
	std::string					m_strQuery;
	std::string					m_strFragment;
};