#include "srcparse.h"

srcparse::stats operator+(srcparse::stats& rhs, srcparse::stats& lhs)
{
    srcparse::stats n;
    n.chartotal=rhs.chartotal+lhs.chartotal;
    n.linetotal=rhs.linetotal+lhs.linetotal;
    n.linecode=rhs.linecode+lhs.linecode;
    return n;
}

srcparse::stats srcparse::parse(char* data)
{
	/* srcparse::parse */
	/* info:
	gets statistics for a source file, skipping '//' and '/* .. *.. comment blocks.
	sets linecount to 1 and starts from the beginning. when '//' is encountered,
	skip until '\n' is encountered. when '/*' is encountered, skip until comment block close
	is found. every time newline is encountered, increment newline counter, etc.
	*/
	const char cr=0x0D; // CR = 13
	const char lf=0x0A;	// LF = 10
	bool inLineComment=false;
	bool inBlockComment=false;
	bool lineHasChars=false; // this flag is set to true when meaningful chars have been found.
	stats rStats;
	rStats.chartotal=0;
	rStats.linecode=0;
	rStats.linetotal=0;
	for(int i=0; data[i]; i++, rStats.chartotal++)
	{
		if(data[i]=='/' && data[i+1]=='*' && inBlockComment==false)
		{
			// looks like we hit a block comment
			inBlockComment=true;
			continue;
		}
		if(data[i]=='*' && data[i+1]=='/' && inBlockComment==true)
		{
			// looks like we hit a block comment closer
			inBlockComment=false;
			continue;
		}
		if(data[i]=='/' && data[i+1]=='/' && inLineComment==false && inBlockComment==false)
		{
			inLineComment=true;
			continue;
		}
		if(data[i]!=' ' && data[i]!='\t' && data[i] && data[i]!='\n' && lineHasChars==false && inBlockComment==false && inLineComment==false)
			lineHasChars=true;
		if(data[i]==cr || data[i]==lf)
		{
			/* here, it is important to make allowance for both UNIX and DOS format text.
			UNIX has CR+LF while DOS only has LF */
			if(data[i]==cr && data[i+1]==lf)
			{
				/* cr+lf means it's a *nix file*/
				i+=2;
			}
			else if(data[i]==lf)
			{
				/* lf only means it's a DOS file*/
				i++;
			}
			rStats.linetotal++;
			if(!inBlockComment && lineHasChars)
				rStats.linecode++;	// only increment code lines if we're not in a comment and the line
									// has meaningful chars
			lineHasChars=false;
			inLineComment=false;
		}
		if(data[i]==NULL)
		{
			rStats.linetotal++;
			if(!inBlockComment && lineHasChars)
				rStats.linecode++;	// only increment code lines if we're not in a comment and the line
									// has meaningful chars
			lineHasChars=false;
			inLineComment=false;
		}
	}
	return rStats;
}
