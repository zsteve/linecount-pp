/** file.h - file system browser subsystem **/

#ifndef FILE_H
#define FILE_H
#include <windows.h>
#include <list>
#include <string>

using namespace std;

typedef unsigned char uchar;

class disk_file
{
    private:
    public:
    disk_file()
    {
    }
    disk_file(const disk_file& src)
    {
        this->path=src.path.c_str();
    }
    friend bool operator==(const disk_file& lh, const disk_file& rh);
    virtual bool kill();
    virtual bool create();
    virtual bool move(string& dest);
    virtual bool move(char* dest);
    virtual bool copy(string& dest);
    virtual bool copy(char* dest);
    virtual bool exists();
    virtual bool setpath(string& path);
    virtual bool setpath(char* path);
    virtual string& getpath();
    protected:
    string path;
};
class file : public disk_file
{
    private:
    public:
    file()
    {
        data=NULL;
        size=0;
    }
    file(const file& src)
    {
        this->path=src.path;
        size=src.size;
    }
    ~file(){
        GlobalFree(data);
    }
    int getsize();
    uchar* load();
    bool write(uchar* data, int size);
    bool write(char* data);
    protected:
    uchar* data;
    int size;
};

class directory : public disk_file
{
    private:
    bool listdir(string dir);
    public:
    directory(){};
    directory(const directory& src)
    {
        this->path=src.path;
    }
    ~directory(){};
    bool listcontents();
    list<file> contents;
    protected:
};
#endif
