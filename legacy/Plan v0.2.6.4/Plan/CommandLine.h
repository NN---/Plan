//
// CommandLine v0.2.6.4
//


#pragma once

namespace NN
{

// class CommandLine
template <class TCommandLineInfo, int maxArgv=128>
class CommandLine
{
	LPTSTR pszCmdLine;
	TCommandLineInfo *pCommandLineInfo;
	int Argc;
	TCHAR *Argv[maxArgv+1];
protected:
	TCommandLineInfo *& CommandLineInfo () { return pCommandLineInfo; }
public:
	CommandLine(LPCTSTR lpszCommandLine = NULL)
	{
		pszCmdLine = NULL;
		pCommandLineInfo = new TCommandLineInfo();
		SetCommandLine(lpszCommandLine);
	}
	~CommandLine() { delete pCommandLineInfo; }
	const int GetArgc() const { return Argc; }
	const TCHAR ** GetArgv() const { return Argv; }
	const TCHAR * GetArgv(int nArgv) const { return Argv[nArgv]; }
	void SetCommandLine(LPCTSTR lpszCommandLine)
	{
		Argv[0] = 0;
		Argc = 0;

		if(!lpszCommandLine)
			return ;

		if(pszCmdLine)
			delete [] pszCmdLine;

		pszCmdLine = new TCHAR [lstrlen(lpszCommandLine)+1];
		if(!pszCmdLine)
			return ;

		lstrcpy(pszCmdLine,lpszCommandLine);

		if(*pszCmdLine == _T('\"'))	// If command line starts with a quote ("),
		{							// it's a quoted filename.  Skip to next quote.
			pszCmdLine++;
			Argv[0] = pszCmdLine;  // argv[0] == executable name
    
			while(*pszCmdLine && (*pszCmdLine != _T('\"')))
				pszCmdLine++;

			if(*pszCmdLine)			// Did we see a non-NULL ending?
				*pszCmdLine++ = 0;	// Null terminate and advance to next char
			else
				return;			// Oops!  We didn't see the end quote
		}
		else	// A regular (non-quoted) filename
		{
			Argv[0] = pszCmdLine;  // argv[0] == executable name

			while(*pszCmdLine && (*pszCmdLine != _T(' ')) && (*pszCmdLine != _T('\t')))
				pszCmdLine++;

			if(*pszCmdLine)
				*pszCmdLine++ = 0;	// Null terminate and advance to next char
		}

		// Done processing __argv[0] (i.e., the executable name).  Now do th
		// actual arguments

		Argc = 1;

		for(;;)
		{
			// Skip over any whitespace
			while(*pszCmdLine && (*pszCmdLine == _T(' ')) || (*pszCmdLine == _T('\t')))
				pszCmdLine++;

			if(!*pszCmdLine) // End of command line???
				return;

			if(*pszCmdLine == _T('\"'))	// Argument starting with a quote???
			{
				pszCmdLine++;   // Advance past quote character

				Argv[Argc++] = pszCmdLine;
				Argv[Argc] = 0;

				// Scan to end quote, or NULL terminator
				while(*pszCmdLine && (*pszCmdLine != _T('\"')))
					pszCmdLine++;
                
				if(!*pszCmdLine)
					return ;
            
				if(*pszCmdLine)
					*pszCmdLine++ = 0;	// Null terminate and advance to next char
			}
			else	// Non-quoted argument
			{
				Argv[Argc++] = pszCmdLine;
				Argv[Argc] = 0;

				// Skip till whitespace or NULL terminator
				while(*pszCmdLine && (*pszCmdLine != _T(' ')) && (*pszCmdLine != _T('\t')))
					pszCmdLine++;
            
				if(!*pszCmdLine)
					return ;
            
				if(*pszCmdLine)
					*pszCmdLine++ = 0;	// Null terminate and advance to next char
			}

			if(Argc >= maxArgv)
				return ;
		}

	}
	const TCommandLineInfo * GetCommandLineInfo () const { return pCommandLineInfo; }
	virtual void ParseCommandLine() = 0;
};

};