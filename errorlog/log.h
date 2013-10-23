#ifndef LOG_H
#define LOG_H

#include <iostream>
using namespace std;

class error_log{
    public:
    error_log()
    {
        verbose=false;
    }
    void setverbose(bool setting)
    {
        verbose=setting;
    }
    void logerror(const char* msg, bool fatal=false)
    {
        if(fatal || verbose)
            cout << "**ERROR** " << msg << endl;
    }
    private:
    bool verbose;
    protected:
};
#endif
