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
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstring>

#if defined __IOS__
    #include <CoreFoundation/CoreFoundation.h>
#elif defined __ANDROID__
	#include <zip.h>
#endif

#include "sgBase.h"
#include "sgDebug.h"

namespace sgResourceManager
{
	std::map<std::string*, sgBase*> resources;
	std::vector<std::string*> paths;

#if defined __ANDROID__
	zip *ZIPArchive = NULL;
	void loadZIP(const char* path)
	{
		sgLog("Loading APK %s", path);
		ZIPArchive = zip_open(path, 0, NULL);
		if(ZIPArchive == NULL)
		{
			sgLog("Error loading APK");
			return;
		}

		//Just for debug, print APK contents
/*		int numFiles = zip_get_num_files(ZIPArchive);
		for(int i=0; i<numFiles; i++)
		{
			const char* name = zip_get_name(ZIPArchive, i, 0);
			if(name == NULL)
			{
				sgLog("Error reading zip file name at index %i : %s", zip_strerror(ZIPArchive));
				return;
			}
			sgLog("File %i : %s\n", i, name);
		}*/
	}
#endif

	void addPath(const char *path)
	{
		paths.push_back(new std::string(path));
	}

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
#elif defined __ANDROID__
		std::string fnm("assets/");
		fnm += std::string(filename);
		char *ptr = new char[fnm.length()+1];
		strcpy(ptr, fnm.c_str());
		return (const char*)ptr;
#else
		for(int i = 0; i < paths.size(); i++)
		{
			std::string str(filename);
			str = *paths[i]+str;
			std::ifstream in(str.c_str());
			if(in.good())
			{
				char *ptr = new char[str.length()+1];
				strcpy(ptr, str.c_str());
				return (const char*)ptr;
			}
		}

		char *ptr2 = new char[std::string(filename).length()+1];
		strcpy(ptr2, filename);
        return (const char*)ptr2;
#endif
	}

	const char *getFileAsString(const char *filepath)
	{
#if defined __ANDROID__
		zip_file *file = zip_fopen(ZIPArchive, filepath, 0);
		char *buff = (char*)malloc(1000000);
		int bytes = zip_fread(file, buff, 1000000);
		zip_fclose(file);
		char *ptr = new char[bytes];
		strcpy(ptr, buff);
		free(buff);
		return (const char*)ptr;
#else
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
#endif
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
		resources.clear();

		for(int i = 0; i < paths.size(); i++)
		{
			delete paths[i];
		}
		paths.clear();
	}
	
	void recreateAll()
	{
		std::map<std::string*, sgBase*>::iterator it;
		for(it = resources.begin(); it != resources.end(); it++)
		{
			it->second->recreate(it->first->c_str());
		}
	}
}
