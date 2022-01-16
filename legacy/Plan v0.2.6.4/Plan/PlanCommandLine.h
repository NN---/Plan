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
	template <int maxCmdLine=10>
	struct ParseCommandLineStruct
	{
		TCHAR szCmdLine[maxCmdLine];
		int nCmdShow;
	};
	static const int maxParseCommandLine=2;
	static const ParseCommandLineStruct<> aParseCommandLine[];
public:
	virtual void ParseCommandLine();
};