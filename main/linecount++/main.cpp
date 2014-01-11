#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <cstdio>
#include <list>
#include "..\..\file\file.h"
#include "..\..\errorlog\log.h"
#include "..\..\srcparse\srcparse\srcparse\srcparse.h"

using std::list;
extern error_log gError;

string getExtension(string path)
{
    int i=path.length();
    for(; path[i]!='\\'; i--)
    {
        if(path[i]=='.')
        {
            // we have reached the '.', meaning that from here to
            // the null terminator is the extension.
            string ext;
            while(path[i])
            {
                ext+=path[i++];
            }
            return ext;
        }
    }
    return ("");
}

string getExtension(char* path)
{
    string pth=path;
    return getExtension(pth);
}

string allowedFiles[8]={".c", ".cpp", ".cc", ".h", ".hpp", ".hh", ".asm", ".inc"};

void processContents(directory d)
{
    /*** processes d.contents to remove all files which
    are not of type .c, .cpp, .cc, .h, .hpp, .hh, .asm, .inc , etc
    ***/
    srcparse parser;
    bool deleteFile=FALSE;
    if(!d.listcontents())
    {
        cout << "error : d.listcontents in processContents()" << endl;
        return;
    }
    for(list<file>::iterator it=d.contents.begin(); it!=d.contents.end(); ++it)
    {
        /* here we run thru all of them and check if they are of type ? in the
        table. if not, we delete them */
        for(int i=0; i<8; i++)
        {
            if(!strcmpi(getExtension(it->getpath()).c_str(), allowedFiles[i].c_str()))
            {
                // the file matches one of the ones allowed.
                deleteFile=false;
                break;
            }
            if(strcmpi(getExtension(it->getpath()).c_str(), allowedFiles[i].c_str()))
                deleteFile=true;
        }
        if(deleteFile)
        {
            cout << "removing file from list :" << it->getpath() << endl;
            d.contents.remove(*it);
            it=d.contents.begin();
        }
    }
    cout << "file list:" << endl;
    for(list<file>::iterator it=d.contents.begin();it!=d.contents.end(); ++it)
        cout << it->getpath() << endl;
    srcparse::stats stats;
    srcparse::stats accumulator;
    for(list<file>::iterator it=d.contents.begin();it!=d.contents.end(); ++it)
    {
        stats=parser.parse((char*)it->load());
        accumulator=accumulator+stats;
    }
    cout << "Chars total : \t\t" << accumulator.chartotal << endl;
    cout << "Lines total : \t\t" << accumulator.linetotal << endl;
    cout << "Lines of code total : \t" << accumulator.linecode << endl;

}

int main(int argc, char *argv[])
{
    cout << "linecount++ v0.1b" << endl;
    directory d;
    if(argc<2)
    {
        cout << "error : no directory specified" << endl;
        return 0;
    }
    d.setpath("C:\\Users\\Stephen\\Code\\projects\\dataquery");
    if(argc>2 && !strcmpi(argv[2], "-v"))
        gError.setverbose(true);
    cout << d.getpath() << endl;
    d.listcontents();
    processContents(d);
    getchar();
    return 0;
}
