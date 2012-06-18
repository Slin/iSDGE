//
//	sgResourceManager.cpp
//	iSDGE
//
//	Created by Nils Daumann on 17.04.10.
//	Copyright (c) 2010 Nils Daumann

//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:

//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.

//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#include "sgResourceManager.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

#if defined __IOS__
    #include <CoreFoundation/CoreFoundation.h>
#endif

#include "sgBase.h"
#include "sgDebug.h"

namespace sgResourceManager
{
	std::map<std::string*, sgBase*> resources;

	const char *getPath(const char *filename, const char *type)
	{
#if defined __IOS__
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
		CFStringRef str = CFURLCopyFileSystemPath(resourcesURL, kCFURLPOSIXPathStyle);
		CFRelease(resourcesURL);
		char *ptr = new char[CFStringGetLength(str)+1];
		CFStringGetCString(str, ptr, FILENAME_MAX, kCFStringEncodingASCII);
		CFRelease(str);


		std::string res(ptr);
		res += std::string("/");
		res += std::string(filename);
		res += std::string(".");
		res += std::string(type);

		delete[] ptr;
		ptr = new char[res.length()+1];
		strcpy(ptr, res.c_str());

		return (const char*)ptr;
#else
        return NULL;
#endif
	}

	const char *getPath(const char *filename)
	{
#if defined __IOS__
		char *ptr;
		std::string fnm(filename);

		if(fnm.find("/") != 0)
		{
			CFBundleRef mainBundle = CFBundleGetMainBundle();
			CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
			CFStringRef str = CFURLCopyFileSystemPath(resourcesURL, kCFURLPOSIXPathStyle);
			CFRelease(resourcesURL);
			ptr = new char[CFStringGetLength(str)+1];
			CFStringGetCString(str, ptr, FILENAME_MAX, kCFStringEncodingASCII);
			CFRelease(str);

			std::string res(ptr);
			res += std::string("/");
			res += std::string(filename);

			delete[] ptr;
			ptr = new char[res.length()+1];
			strcpy(ptr, res.c_str());
		}else
		{
			ptr = new char[fnm.length()+1];
			strcpy(ptr, fnm.c_str());
		}

		return (const char*)ptr;
#else
        return filename;
#endif
	}

	const char *getFileAsString(const char *filepath)
	{
		std::string buf("");
		std::string line("");
		std::ifstream in(filepath);

		while(std::getline(in,line))
		{
			line += std::string("\n");
			buf += line;
		}

		char *ptr = new char[buf.length()+1];
		strcpy(ptr, buf.c_str());
		ptr[buf.length()] = '\0';

		return (const char*)ptr;
	}

	sgBase *getResource(const char *name)
	{
		std::string *str = new std::string(name);
		std::map<std::string*, sgBase*>::iterator it;
		for(it = resources.begin(); it != resources.end(); it++)
		{
			if(it->first->compare(*str) == 0)
			{
				delete str;
				return it->second;
			}
		}

		delete str;
		return NULL;
	}

	void addResource(const char *name, sgBase *res)
	{
		std::string *str = new std::string(name);
		resources.insert(std::pair<std::string*,sgBase*>(str, res));
	}

	void addResource(sgBase *res)
	{
		std::string *str = new std::string("noname");
		resources.insert(std::pair<std::string*,sgBase*>(str, res));
	}

	void removeResource(sgBase *res)
	{
		std::map<std::string*, sgBase*>::iterator it;
		for(it = resources.begin(); it != resources.end(); it++)
		{
			if(it->second == res)
			{
				delete it->first;
				resources.erase(it);
				return;
			}
		}
	}

	void destroyAll()
	{
		std::map<std::string*, sgBase*>::iterator it;
		for(it = resources.begin(); it != resources.end(); it++)
		{
			delete it->first;
			delete it->second;
		}
	}
}
