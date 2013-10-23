#include <iostream>
#include "file.h"
#include "..\errorlog\log.h"

using namespace std;

extern error_log gError;

int main()
{
    gError.setverbose(true);
    directory d;
    d.setpath("C:\\");
    d.listcontents();
    list<file>& c=d.contents;
    for(list<file>::iterator it=c.begin(); it!=c.end(); ++it)
    {
        cout << (*it).getpath() <<endl;
    }
    cout << c.size() << endl;
    return 0;
}
