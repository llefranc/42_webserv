/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 14:23:57 by lucaslefran       #+#    #+#             */
/*   Updated: 2021/11/04 19:22:53 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


/* ------------------------------------------------------------- */
/* ------------------------ COPLIEN FORM ----------------------- */

Response::Response() {}

Response::Response(Request* req, const StatusLine& staLine, const std::vector<ServerInfo>* infoVirServs) :
	_infoVirServs(infoVirServs), _req(req), _staLine(staLine), _autoIndex(false) {}

Response::Response(const Response& c) : 
	_infoVirServs(c._infoVirServs), _req(c._req), _staLine(c._staLine), _buffer(c._buffer),
    _autoIndex(c._autoIndex) {}

Response::~Response() {}

Response& Response::operator=(Response a)
{
	swap(a, *this);
	return *this;
}


/* ------------------------------------------------------------- */
/* --------------------------- SETTERS ------------------------- */

void Response::setRequest(Request* req)
{
	_req = req;
}

void Response::setStatusLine(const StatusLine& staLine)
{
	_staLine = staLine;
}

void Response::setInfoVirtualServs(const std::vector<ServerInfo>* infoVirServs)
{
	_infoVirServs = infoVirServs;
}


/* ------------------------------------------------------------- */
/* --------------------------- GETTERS ------------------------- */

const StatusLine& Response::getStatusLine() const
{
	return _staLine;
}

int Response::getCode() const
{
	return _staLine.getCode();
}

const std::string& Response::getBuffer() const
{
	return _buffer;
}


/* ------------------------------------------------------------- */
/* --------------------------- METHODS ------------------------- */

void Response::clear()
{
	_staLine.clear();
	_buffer.clear();
}

void Response::fillBuffer()
{
	// If an error occured during request parsing
	if (_staLine.getCode() >= 400)
		return fillError(_staLine);

	try
	{
		// Keeping only host name and removing port
		std::string hostName(_req->getHeaders().find("host")->second);
		hostName = hostName.substr(0, hostName.find(':'));
		
		// Looking for the location block matching the URI. If returns NULL, then no appropriate block was found
		// and no additionnal configuration (index, root...) will change the URI
		std::pair<const std::string, const Location*> loc = locationSearcher(_infoVirServs,
				std::pair<std::string, std::string>(hostName, _req->getPath()));
		
		#if DEBUG
			if (loc.second)
				loc.second->printLocation(loc.first);
			else
				std::cout << "LOCATION: no match\n";
		#endif

        // Doing an HTTP redirection (301) if redirect field filled in matched location block
        if (loc.second && !loc.second->getRedirect().empty())
        {
            // Replacing location name in the URI with the redirect string set in config file
            std::string redirectedUri = _req->getPath();
            replaceLocInUri(&redirectedUri, loc.second->getRedirect(), loc.first);

            // Replacing previous requested URI with redirected URI for next client request
            // (Location header in 301 response will be set with this URI)
            _req->setPath(std::string("http://localhost:" + 
                    convertNbToString(loc.second->getPort()) + redirectedUri));

            throw StatusLine(301, REASON_301, "http redirection");
        }

        // Modifying URI with root and index directive if any, checking for the allowed methods
		std::string realUri = reconstructFullURI(_req->getMethod(), loc, _req->getPath());

        // Checking if the targeted file is a CGI based on his extension
		std::string *cgiName = getCgiExecutableName(realUri, loc.second);

        // Execute the appropriate method and fills the response buffer with status line + 
        // headers + body (if any). If an error occurs during this process, it will throw 
        // a StatusLine object with the appropriate error code.
		if (cgiName && (_req->getMethod() == GET || _req->getMethod() == POST))
			execCgi(realUri, cgiName);
		else if (_req->getMethod() == GET || _req->getMethod() == HEAD)
            execGet(realUri);
		else if (_req->getMethod() == POST)
            execPost(realUri);
		else if (_req->getMethod() == DELETE)
            execDelete(realUri);

		else
			throw (StatusLine(400, REASON_400, "request method do not exist"));
	}

	// If an error occured during the reponse creation
	catch (const StatusLine& errorStaLine)
	{
		fillError(errorStaLine);
	}
}


/* ------------------------------------------------------------- */
/* ----------------------- PRIVATE METHODS --------------------- */

void Response::execCgi(const std::string& realUri, std::string* cgiName)
{
    struct stat st;
    if (stat(realUri.c_str(), &st) == -1)
	{
        throw StatusLine(404, REASON_404, "fillCgi method: " + realUri);
	}

	Body cgiRep;
	CGI cgi(&cgiRep, _req, realUri, *cgiName);
	
	cgi.executeCGI();
	
	fillStatusLine(_staLine);
	fillServerHeader();
	fillDateHeader();
	fillContentlengthHeader(convertNbToString(cgiRep.getSize()));

	_buffer += cgiRep.getBody();
	
	delete cgiName;
}

void Response::fillContentlengthHeader(const std::string& size) 
{
	_buffer += "Content-Length: " + size + CLRF;
}

void Response::fillServerHeader() 
{
	_buffer += "Server: webserv\r\n";
}

void Response::fillDateHeader() 
{
	// current date and time on the current system
	time_t now = time(0);

	// convert now to string form
	char* date_time = ctime(&now);

	// Splitting date line and removing '\n'
	std::vector<std::string> date = splitWithSep(date_time, ' ');
	date.back().resize(4);

	// Formating header date.
	// ctime format = Thu May 20 14:33:40 2021 >> to header date format : Thu, 20 May 2021 12:16:42 GMT
	_buffer += "Date: " + date[0] + ", " + date[2] + " " + date[1] + " " + date[4] + " " + date[3] + " GMT" + CLRF;
}

void Response::fillLastModifiedHeader(const char* uri)
{
	struct stat infFile;

	if (stat(uri, &infFile) == -1)
		throw StatusLine(404, REASON_404, "fillLastModifiedHeader method");

	struct tm* lm = localtime(&infFile.MTIMESPEC.tv_sec);

	const std::string day[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	const std::string mon[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	_buffer += "Last-Modified: " + day[lm->tm_wday - 1] + ", " + convertNbToString(lm->tm_mday) + " " + mon[lm->tm_mon] + " " 
			+ convertNbToString(lm->tm_year + 1900) + " " + convertNbToString(lm->tm_hour) + ":" + 
			convertNbToString(lm->tm_min) + ":" + convertNbToString(lm->tm_sec) + " GMT" + CLRF;
}

void Response::fillLocationHeader(const std::string& redirectedUri)
{
    _buffer += "Location:" + redirectedUri + CLRF;
}

void Response::fillStatusLine(const StatusLine& staLine)
{
	_buffer = "HTTP/1.1 " + convertNbToString(staLine.getCode()) + " " + staLine.getReason();
	if (!staLine.getAdditionalInfo().empty())
		_buffer += " (" + staLine.getAdditionalInfo() + ")";
	_buffer += CLRF;
}

void printLoc(const Location* loc)
{
	std::cout << "root: " << loc->getRoot() << "\n";
	for (std::vector<std::string>::const_iterator it = loc->getMethods().begin(); it != loc->getMethods().end(); it++)
		std::cout << "methods: " << *it << "\n";
		
	for (std::vector<std::string>::const_iterator it = loc->getIndex().begin(); it != loc->getIndex().end(); it++)
		std::cout << "index: " << *it << "\n";
}

void Response::replaceLocInUri(std::string* uri, const std::string& root, const std::string& locName)
{
	// Creating an iterator pointing just after the part that matched the location
	std::string::iterator it = uri->begin() + locName.size();

	// Going to the next '/' (for example, case "/authentificate/ok" matched location "/auth" with a root of "/test"
	//						  >> we need to replace "/authentificate" with "/test" in order to get "/test/ok")
	if (*it != '/' && locName.size() > 1)
		while (*it && *it != '/')
			++it;
	
	// Replacing beginning of the URI with the root path
	uri->erase(uri->begin(), it);

	// To correctly append indexs if last '/' is missing
	if (root[root.size() - 1] != '/')
		uri->insert(uri->begin(), '/');
		
	uri->insert(0, root);
}

std::string Response::addIndex(const std::string& uri, const std::vector<std::string>& indexs)
{
	struct stat infFile;
	
	for (std::vector<std::string>::const_iterator it = indexs.begin(); it != indexs.end(); ++it)
	{
		// Add each index to the uri
		std::string uriWithIndex(uri + *it);

		// And then try to access the URI resulting from this concatenation
		if (!stat(uriWithIndex.c_str(), &infFile))
			return uriWithIndex;
	}
	
	throw StatusLine(403, REASON_403, "trying to access a directory addIndex method");
}

void Response::checkMethods(int method, const std::vector<std::string>& methodsAllowed) const
{
	std::string tab[NB_METHODS] = { "GET", "HEAD", "POST", "DELETE" };

	for (std::vector<std::string>::const_iterator it = methodsAllowed.begin();
			it != methodsAllowed.end(); ++it)
		if (!it->compare(tab[method]))
			return ;

	throw StatusLine(405, REASON_405, "checkMethods method");
}

std::string Response::reconstructFullURI(int method,
		const std::pair<const std::string, const Location*>& loc, std::string uri)
{
	bool fileExist = true;
	struct stat infFile;


	// zero location block match the URI so the URI isn't modified
	if (!loc.second)
	{
        // Need to add a '.' for relative path access
        if (!uri.empty() && uri[0] == '/')
            uri.insert(uri.begin(), '.');
    
		// Checking if the file exist or if it's a directory. Case POST method, no 404 because it can create the file.
		if (stat(uri.c_str(), &infFile) == -1 && !(fileExist = false) && method != POST)
			throw StatusLine(404, REASON_404, "case no match with location block in reconstructlFullURI method: " + uri);
		if (fileExist && S_ISDIR(infFile.st_mode))
			throw StatusLine(403, REASON_403, "trying to access a directory case no match with"
					" location block in reconstructlFullURI method");

		return uri;
	}

	// Replacing the part of the URI that matched with the root path if there is one existing
	if (!loc.second->getRoot().empty() && !(method == POST && !loc.second->getUploadPath().empty()))
		replaceLocInUri(&uri, loc.second->getRoot(), loc.first);

	else if (method == POST && !loc.second->getUploadPath().empty())
		replaceLocInUri(&uri, loc.second->getUploadPath(), loc.first);
	
	// If no root in location block, or root doesn't start with a '.', need to add it to find the file using
	// relative path
	if (!uri.empty() && uri[0] == '/')
		uri.insert(uri.begin(), '.');

	// Checking if the path after root substitution is correct, and if it's a directory trying
	// to add indexs. Case POST method, no 404 because it can create the file.
	if (stat(uri.c_str(), &infFile) == -1 && !(fileExist = false) && method != POST)
		throw StatusLine(404, REASON_404, "reconstructFullURI method: " + uri);

    // Case we match a directory and an autoindex isn't set. We try all the possible indexs, if none
    // works addIndex throw a 403 error StatusLine object
	if (fileExist && S_ISDIR(infFile.st_mode) && !((method == GET || method == HEAD) && loc.second->getAutoIndex()))
		uri = addIndex(uri, loc.second->getIndex());

    else if (fileExist && S_ISDIR(infFile.st_mode) && ((method == GET || method == HEAD) && loc.second->getAutoIndex()))
    {
	    _autoIndex = true;
    }

	checkMethods(method, loc.second->getMethods());

	return uri;
}

void Response::fillError(const StatusLine& sta)
{    
    if (sta.getCode() != _staLine.getCode())
    {
	    _staLine = sta;
	}
	
	_buffer.clear();
	
	// Filling buffer with error code + some basic headers
	fillStatusLine(sta);
	fillServerHeader();
	fillDateHeader();

    // Case HTTP redirection
    if (_staLine.getCode() == 301)
	{
        fillLocationHeader(_req->getPath());
	}

	// Value of host header field in request
	const std::string* hostField = &_req->getHeaders().find("host")->second;
	const std::string hostValue(hostField->substr(0, hostField->find(':')));

	// Looking in each virtual server names if one match host header field value, if
    // not using default server
	const ServerInfo* servMatch = findVirtServ(_infoVirServs, hostValue);
    if (!servMatch)
	{
        servMatch = &_infoVirServs->front();
	}
	
	std::string pathError;
	std::string errorCodeHTML = "/" + convertNbToString(sta.getCode()) + ".html";

    // Custom error pages if set in config file
	if (!servMatch->getError().empty())
	{
        // Adding relative file access if not well filled in config file
        pathError = (servMatch->getError()[0] == '/') ? 
                "." + servMatch->getError() + errorCodeHTML: servMatch->getError() + errorCodeHTML;

		struct stat infFile;
		if (stat(pathError.c_str(), &infFile) == -1)
			pathError = DEFAULT_PATH_ERROR_PAGES + errorCodeHTML;
	}
    
	else
		pathError = DEFAULT_PATH_ERROR_PAGES + errorCodeHTML;

    // Filling buffer
	FileParser body(pathError.c_str(), true);

	fillContentlengthHeader(convertNbToString(body.getRequestFileSize()));
	_buffer += CLRF + body.getRequestFile();
}

void Response::postToFile(const std::string& uri)
{
	std::fstream postFile;

	postFile.open(uri.c_str(), std::ios::app);

	if (!postFile.is_open())
		throw (StatusLine(500, REASON_500, "failed to open file in post method"));
	
	postFile << _req->getBody().getBody();
}

void Response::execGet(const std::string& realUri)
{
    // Storing status line and some headers in buffer
    fillStatusLine(_staLine);
    fillServerHeader();
    fillDateHeader();
    
    if (!_autoIndex)
    {
        FileParser body(realUri.c_str(), true); // CAHNGER

        // Setting size after storing the body in FileParser object, then setting Last-Modified header
        fillContentlengthHeader(convertNbToString(body.getRequestFileSize()));
        fillLastModifiedHeader(realUri.c_str());
        _buffer += CLRF;

        // For GET, writing the body previously stored to the buffer
        if (_req->getMethod() == GET)
            _buffer += body.getRequestFile();
    }

    // Filling autoindex
    else
    {
        std::string autoIndexPage;
        autoIndexDisplayer(realUri, &autoIndexPage);

        fillContentlengthHeader(convertNbToString(autoIndexPage.size()));
        _buffer += CLRF + autoIndexPage;
    }
}

void Response::execPost(const std::string& realUri)
{
    // Need to create file so changing code 200 ("OK") to 201 ("created")
    struct stat infoFile;
    if (stat(realUri.c_str(), &infoFile) == -1)
        _staLine = StatusLine(201, REASON_201);
    
    // Creating a new file or appending to existing file post request payload. If 
    // opening failed, throws StatusLine object with error 500 (internal error)
    postToFile(realUri);
    
    // Storing status line and some headers in buffer
    fillStatusLine(_staLine);
    fillServerHeader();
    fillDateHeader();
}

void Response::execDelete(const std::string& realUri)
{
    if (remove(realUri.c_str()))
        throw (StatusLine(500, REASON_500, "remove function failed in DELETE method"));

    // Storing status line and some headers in buffer
    fillStatusLine(_staLine);
    fillServerHeader();
    fillDateHeader();    
}


/* ------------------------------------------------------------- */
/* --------------- NON-MEMBER FUNCTION OVERLOADS --------------- */

void swap(Response& a, Response& b)
{
	std::swap(a._infoVirServs, b._infoVirServs);
	std::swap(a._req, b._req);
	swap(a._staLine, b._staLine);
	std::swap(a._buffer, b._buffer);
    std::swap(a._autoIndex, b._autoIndex);
}
