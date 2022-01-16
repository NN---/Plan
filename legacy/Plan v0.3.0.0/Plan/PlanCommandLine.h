#ifndef __PLANCOMMANDLINE_H__
#define __PLANCOMMANDLINE_H__

#pragma once

#include "CommandLine.h"

struct PlanCommandLineInfo
{
	int nCmdShow;
};

class PlanCommandLine :	public NN::CommandLine<PlanCommandLineInfo>
{
	struct ParseCommandLineStruct
	{
		TCHAR szCmdLine[10];
		int nCmdShow;
	};
	static const int maxParseCommandLine=2;
	static const ParseCommandLineStruct aParseCommandLine[];
public:
	PlanCommandLine(){}
	~PlanCommandLine(){}
	virtual void ParseCommandLine();
};

#endif // __PLANCOMMANDLINE_H__