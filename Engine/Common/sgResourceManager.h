//
//	sgResourceManager.h
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

#ifndef __SGRESOURCEMANAGER_H__
#define __SGRESOURCEMANAGER_H__

class sgBase;

/**
 * Resource manager namespace. Manages the system memory and automatically frees registered objects.
 */
namespace sgResourceManager
{
#if defined __ANDROID__
	void loadZIP(const char* path);
#endif

	/**
	 *	Add path.
	 *	Add a path to search for files at.
	 * @param path the path to add
	 */
	void addPath(const char *path);

	/**
	 *	Get path.
	 *	Get the path of a file in the filesystem.
	 * @param filename the name of the file without type specifier
	 * @param type the type specifier
	 * @return the path to the given file
	 */
	const char *getPath(const char *filename, const char *type);

	/**
	 *	Get path.
	 *	Get the path of a file in the filesystem.
	 * @param filename the name of the file
	 * @return the path to the given file
	 */
	const char *getPath(const char *filename);

	/**
	 *	Get file content.
	 *	Reads a file and returns its content as a string.
	 * @param filepath the path to the file
	 * @return the files content string
	 */
	const char *getFileAsString(const char *filepath);

	/**
	 *	Get resource.
	 *	Reads a file and returns its content as a string.
	 * @param name the name of the resource
	 * @return pointer to the instance or NULL if there is none
	 */
	sgBase *getResource(const char *name);

	/**
	 *	Add resource.
	 *	Adds a resource to the ressource manager which will be freed automatically.
	 * @param name the name to add the resource with
	 * @param res pointer to the resource to add
	 */
	void addResource(const char *name, sgBase *res);

	/**
	 *	Add resource.
	 *	Adds a resource to the resource manager which will be freed automatically.
	 * @param res pointer to the resource to add
	 */
	void addResource(sgBase *res);

	/**
	 *	Remove resource.
	 *	Removes a resource from the resource manager.
	 * @param res pointer to the resource to remove
	 */
	void removeResource(sgBase *res);

	/**
	 *	Destroy all.
	 *	Frees all resources.
	 */
	void destroyAll();
}

#endif
