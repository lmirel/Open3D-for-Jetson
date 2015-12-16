// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2015 Qianyi Zhou <Qianyi.Zhou@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "FileSystem.h"

#ifdef WINDOWS
#include <dirent/dirent.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#endif

namespace three{

bool DirectoryExists(const std::string &directory)
{
	struct stat info;
	if (stat(directory.c_str(), &info) == -1)
		return false;
	return S_ISDIR(info.st_mode);
}

bool FileExists(const std::string &filename)
{
	struct stat info;
	if (stat(filename.c_str(), &info) == -1)
		return false;
	return S_ISREG(info.st_mode);
}

bool ListFilesInDirectory(const std::string &directory,
	std::vector<std::string> &filenames)
{
	if (directory.empty()) {
		return false;
	}
	DIR *dir;
	struct dirent *ent;
	struct stat st;
	dir = opendir(directory.c_str());
	if (!dir) {
		return false;
	}
	while ((ent = readdir(dir)) != NULL) {
		const std::string file_name = ent->d_name;
		if (file_name[0] == '.')
			continue;
		std::string full_file_name;
		if (directory.back() == '/' || directory.back() == '\\') {
			full_file_name = directory + file_name;
		} else {
			full_file_name = directory + "/" + file_name;
		}
		if (stat(full_file_name.c_str(), &st) == -1)
			continue;
		if (S_ISREG(st.st_mode))
			filenames.push_back(full_file_name);
	}
	closedir(dir);
	return true;
}

}	// namespace three
