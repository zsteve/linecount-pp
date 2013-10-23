#ifndef SRCPARSE_H
#define SRCPARSE_H
#include <cstring>
#include <cstdlib>
#include <iostream>

class srcparse{
private:
	char* data;
public:
	class stats{
        public:
        stats()
        {
            chartotal=linetotal=linecode=0;
        }
        friend stats operator+(stats& rhs, stats& lhs);
        stats& operator=(stats lhs)
        {
            chartotal=lhs.chartotal;
            linetotal=lhs.linetotal;
            linecode=lhs.linecode;
        }
        int chartotal;
        int linetotal;
        int linecode;
	};
	srcparse()
	{
		data=NULL;
	}
	stats parse(char* data);
protected:
};
#endif
