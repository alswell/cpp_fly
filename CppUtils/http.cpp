#include "http.h"

HttpResponse::HttpResponse(int status, float version)
{
	m_strVersion.Format("HTTP/%.1f", version);
	m_nStatus = status;
	m_strStatus = "OK";
}

HttpResponse::HttpResponse(CSock _sock)
{
	m_sock = _sock;
	int n = -1;
	int total = 0;
	while (1)
	{
		char * p = m_sock.Read();
		if (!p)
			return;
		m_strResp += p;
		n = m_strResp.Find("\r\n\r\n");
		if (n != -1)
		{
			list<CStr> ls = m_strResp.Left(n).Split("\r\n");
			list<CStr> lsStatus = ls.front().Split(' ');
			m_strVersion = lsStatus.front();
			lsStatus.pop_front();
			m_nStatus = atoi(lsStatus.front());
			lsStatus.pop_front();
			m_strStatus = lsStatus.front();
			ls.pop_front();
			FOR_LIST(CStr, ls, it)
			{
				list<CStr> kv = it->Split(':', 1);
				m_dHead[kv.front()] = kv.back().Trim();
			}
			total = atoi(m_dHead["Content-Length"]) + n + 4;
		}
		if (total && m_strResp.Length() >= total)
			break;
	}
	m_strBody = m_strResp.Right(n + 4);
}

CStr &HttpResponse::operator [](const CStr &key)
{
	if (key == "body")
		return m_strBody;
	return m_dHead[key];
}

CStr &HttpResponse::operator ()()
{
	m_strResp.Format("%s %d %s\r\n", (const char*)m_strVersion, m_nStatus, (const char*)m_strStatus);
	FOR_DICT(CStr, m_dHead, it)
	{
		m_strResp.AppendFormat("%s: %s\r\n", (const char*)(it->first), (const char*)(it->second));
	}
	m_strResp += "\r\n";
	m_strResp += m_strBody;
}

HttpResponse &HttpResponse::operator ()(const CStr &strBody)
{
	m_strBody = strBody;
	return *this;
}


HttpRequest::HttpRequest(const char *host, int port, const char *url, const char *method, float version)
	: m_fVersion(version)
	, m_sock(host, port)

{
	m_dHead["Host"].Format("%s:%d", host, port);
	m_strMethod = method;
	m_strUrl = url;
	m_sock.SetTimeout(5);
}

HttpRequest::HttpRequest(const CStr &str)
{
	m_strReq = str;
	int n = m_strReq.Find("\r\n\r\n");
	if (n == -1)
		return;

	list<CStr> ls = m_strReq.Left(n).Split("\r\n");
	list<CStr> lsStatus = ls.front().Split(' ');
	m_strMethod = lsStatus.front();
	lsStatus.pop_front();
	m_strUrl = lsStatus.front();
	lsStatus.pop_front();
	m_fVersion = atof(lsStatus.front().Right(5));
	ls.pop_front();
	FOR_LIST(CStr, ls, it)
	{
		list<CStr> kv = it->Split(':', 1);
		m_dHead[kv.front()] = kv.back().Trim();
	}
	m_strBody = m_strReq.Right(n + 4);
}

CStr &HttpRequest::METHOD()
{
	return m_strMethod;
}

CStr &HttpRequest::URL()
{
	return m_strUrl;
}

CStr &HttpRequest::BODY()
{
	return m_strBody;
}

CStr &HttpRequest::operator [](const CStr &key)
{
	return m_dHead[key];
}

HttpRequest &HttpRequest::operator ()(const CStr &strBody)
{
	m_strBody = strBody;
	return *this;
}

CStr &HttpRequest::operator ()()
{
	m_strReq.Format("%s %s HTTP/%.1f\r\n", (const char*)m_strMethod, (const char*)m_strUrl, m_fVersion);
	FOR_DICT(CStr, m_dHead, it)
	{
		m_strReq.AppendFormat("%s: %s\r\n", (const char*)(it->first), (const char*)(it->second));
	}
	m_strReq += "\r\n";
	m_strReq += m_strBody;
}

HttpResponse HttpRequest::Send()
{
	m_sock.Write((*this)());
	return HttpResponse(m_sock);
}
