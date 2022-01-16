#include "StdAfx.h"
#include "PlanCommandLine.h"

void PlanCommandLine::ParseCommandLine()
{
	int i,j,n;
	for(i=1,n=GetArgc();i<n;i++)
		for(j=0;j<maxParseCommandLine;j++)
			if(!lstrcmp(GetArgv(i),aParseCommandLine[j].szCmdLine))
			{
				CommandLineInfo()->nCmdShow=aParseCommandLine[j].nCmdShow;
				return;
			}
	CommandLineInfo()->nCmdShow=SW_SHOW;
}

const PlanCommandLine::ParseCommandLineStruct<> PlanCommandLine::aParseCommandLine[]=
{
	{_T("-min"),SW_MINIMIZE},
	{_T("-max"),SW_MAXIMIZE},
};