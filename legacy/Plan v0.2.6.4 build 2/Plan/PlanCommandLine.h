//
// PlanCommandLine v0.2.6.4
//

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
	virtual void ParseCommandLine();
};