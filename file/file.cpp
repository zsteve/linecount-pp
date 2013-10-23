/** file.cpp - file system library for win32 ***/
/** (C) Stephen Zhang (zsteve) 2013          ***/
/** http://0x4c.phatcode.net                 ***/
/** description :
    parser for win32 file system, supporting
    basic functions for file and directory.  ***/

#include "file.h"
#include <windows.h>
#include <string>
#include <list>
#include "..\errorlog\log.h"

using namespace std;
extern error_log gError;

bool operator==(const disk_file& lh, const disk_file& rh)
{
    if(lh.path==rh.path)
    {
        return true;
    }
    return false;
}

bool disk_file::kill()
/*** description : kill() deletes the file from disk
***/
{
    if(path.empty())
    {
        gError.logerror("kill() - file nonexistent", true);
        return false;
    }
    if(!DeleteFile(path.c_str()))
    {
        gError.logerror("kill() - deletefile() returned NULL", false);
        return false;
    }
    return true;
}

bool disk_file::create()
/*** description : creates the file on the disk. if file already exists,
existing file is deleted and new file overwrites the old.
***/
{
    if(path.empty())
    {
        gError.logerror("create() - path is empty");
        return false;
    }
    HANDLE hFile=CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
        CREATE_ALWAYS, 0, NULL);
    if(hFile==INVALID_HANDLE_VALUE)
    {
        gError.logerror("create() - createfile returned invalid_handle_value", true);
        return false;
    }
    CloseHandle(hFile);
    return true;
}

bool disk_file::move(string& dest)
/*** description : moves the file in this->path to dest.
***/
{
    if(path.empty())
    {
        gError.logerror("move() - path is empty");
        return false;
    }
    if(!MoveFile(path.c_str(), dest.c_str()))
    {
        gError.logerror("move() - movefile() returned NULL");
        return false;
    }
    path=dest;
    return true;
}

bool disk_file::move(char* dest)
/*** description : moves the file in this->path to dest.
***/
{
    if(path.empty())
    {
        gError.logerror("move() - path is empty");
        return false;
    }
    if(!MoveFile(path.c_str(), dest))
    {
        gError.logerror("move() - movefile() returned NULL");
        return false;
    }
    path=dest;
    return true;
}

bool disk_file::copy(string& dest)
/*** description : copies the file in this->path to dest. this->path
remains the same (not updated)
***/
{
    if(path.empty())
    {
        gError.logerror("copy() - path is empty");
        return false;
    }
    if(!CopyFile(path.c_str(), dest.c_str(), true))
    {
        gError.logerror("copy() - copyfile() returned NULL");
        return false;
    }
    return true;
}

bool disk_file::copy(char* dest)
/*** description : copies the file in this->path to dest. this->path
remains the same (not updated)
***/
{
    if(path.empty())
    {
        gError.logerror("copy() - path is empty");
        return false;
    }
    if(!CopyFile(path.c_str(), dest, true))
    {
        gError.logerror("copy() - copyfile() returned NULL");
        return false;
    }
    return true;
}

bool file::write(char* data)
{
    return write((uchar*)data, strlen(data)+1);
}

bool file::write(uchar* data, int size)
/*** description : writes (does not append) to the file.
***/
{
    HANDLE hFile;
    int fSize=0;
	int written=0;
    hFile=CreateFile(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, 0, NULL);
    if(hFile!=INVALID_HANDLE_VALUE)
    {
		WriteFile(hFile, data, size, (LPDWORD)&written, NULL);
		CloseHandle(hFile);
		return TRUE;
    }
    gError.logerror("write() - hfile = invalid_handle_value");
    CloseHandle(hFile);
    return FALSE;
}

bool disk_file::setpath(string& path)
/*** description : sets the current path
***/
{
    if(path.empty())
    {
        gError.logerror("setpath() - path is empty");
        return false;
    }
    this->path=path;
    return true;
}

bool disk_file::setpath(char* path)
/*** description : sets the current path
***/
{
    if(path==NULL)
    {
        gError.logerror("setpath() - path is empty");
        return false;
    }
    this->path=path;
    return true;
}

string& disk_file::getpath()
/*** description : gets the current path
***/
{
    return path;
}

bool disk_file::exists()
/*** description : returns TRUE if this->path is a valid file,
FALSE if not.
***/
{
    void* hFile=CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, 0, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
		return false;
	return true;
}

int file::getsize()
/*** description : returns the size. -1 signifies failure.
***/
{
    HANDLE hFile=CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, 0, NULL);
    if(hFile!=INVALID_HANDLE_VALUE)
    {
        int fSize=GetFileSize(hFile, NULL);
        if(fSize!=-1)
        {
            CloseHandle(hFile);
            return fSize;
        }
    }
    gError.logerror("getsize() - file nonexistent");
    CloseHandle(hFile);
    return -1;
}

uchar* file::load()
/*** description : memory maps the file into this->data and returns it too.
***/
{
    HANDLE hFile;
    int fSize=0;
    if(data)
        GlobalFree(data);
    hFile=CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, 0, NULL);
    if(hFile!=INVALID_HANDLE_VALUE)
    {
        DWORD read;
        fSize=GetFileSize(hFile, NULL);
        data=(uchar*)GlobalAlloc(GPTR, fSize+1);
        if (data)
        {
            if(ReadFile(hFile, data, fSize, &read, NULL)!=0)
            {
                CloseHandle(hFile);
                return data;
            }
            gError.logerror("load() - readfile returned null");
            CloseHandle(hFile);
            return NULL;
        }
        gError.logerror("load() - data == null");
    }
    gError.logerror("load() - hfile == invalid_handle_value");
    return NULL;
}

bool directory::listcontents()
/*** description : wrapper for directory::listdir(). returns true for success, false for failure.
the results can be accessed through this->contents as a std::list<file>.
***/
{
        listdir(path);
    return true;
}
int i;
bool directory::listdir(string dir)
{
    WIN32_FIND_DATA findData;
    string newFile;
    HANDLE hFind;
    newFile=(dir+"\\*");
    hFind=FindFirstFile(newFile.c_str(), &findData);
    if(hFind==INVALID_HANDLE_VALUE)
    {
        return false;
    }
    if(findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && (strcmpi(findData.cFileName, "."))\
            && (strcmpi(findData.cFileName, "..")) && (strcmpi(findData.cFileName, "...")))
    {
        string newDir=(dir+"\\"+findData.cFileName);
        listdir(newDir);
    }
    else if ((findData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY))
    {
        file* foundFile=new file();
        string newPath=dir;
        newPath+="\\";
        newPath+=findData.cFileName;
        foundFile->setpath(newPath);
        contents.push_back(*foundFile);
    }
    for(;;)
    {
        if(!FindNextFile(hFind, &findData))
        {
            break;
        }
        if(findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && (strcmpi(findData.cFileName, "."))\
                && (strcmpi(findData.cFileName, "..")) && (strcmpi(findData.cFileName, "...")))
        {
            string newDir=(dir+"\\"+findData.cFileName);
            listdir(newDir);
        }
        else if ((findData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY))
        {
            file* foundFile=new file();
            string newPath=dir;
            newPath+="\\";
            newPath+=findData.cFileName;
            foundFile->setpath(newPath);
            contents.push_back(*foundFile);
        }
    }
}
