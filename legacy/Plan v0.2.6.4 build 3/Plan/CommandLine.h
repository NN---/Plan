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
	LPTSTR m_pszCmdLine;
	TCommandLineInfo m_CommandLineInfo;
	int m_Argc;
	TCHAR *m_Argv[maxArgv+1];
protected:
	TCommandLineInfo &CommandLineInfo () { return m_CommandLineInfo; }
public:
	CommandLine(LPCTSTR lpszCommandLine = NULL)
	{
		m_pszCmdLine = NULL;
		SetCommandLine(lpszCommandLine);
	}
	~CommandLine()
	{
		delete [] m_pszCmdLine;
	}
	const int GetArgc() const { return m_Argc; }
	const TCHAR ** GetArgv() const { return m_Argv; }
	const TCHAR * GetArgv(int nArgv) const { return m_Argv[nArgv]; }
	void SetCommandLine(LPCTSTR lpszCommandLine)
	{
		m_Argv[0] = 0;
		m_Argc = 0;

		if(!lpszCommandLine)
			return ;

		if(m_pszCmdLine)
			delete [] m_pszCmdLine;

		m_pszCmdLine = new TCHAR [lstrlen(lpszCommandLine)+1];
		if(!m_pszCmdLine)
			return ;

		lstrcpy(m_pszCmdLine,lpszCommandLine);

		if(*m_pszCmdLine == _T('\"'))	// If command line starts with a quote ("),
		{							// it's a quoted filename.  Skip to next quote.
			m_pszCmdLine++;
			m_Argv[0] = m_pszCmdLine;  // m_Argv[0] == executable name
    
			while(*m_pszCmdLine && (*m_pszCmdLine != _T('\"')))
				m_pszCmdLine++;

			if(*m_pszCmdLine)			// Did we see a non-NULL ending?
				*m_pszCmdLine++ = 0;	// Null terminate and advance to next char
			else
				return;			// Oops!  We didn't see the end quote
		}
		else	// A regular (non-quoted) filename
		{
			m_Argv[0] = m_pszCmdLine;  // m_Argv[0] == executable name

			while(*m_pszCmdLine && (*m_pszCmdLine != _T(' ')) && (*m_pszCmdLine != _T('\t')))
				m_pszCmdLine++;

			if(*m_pszCmdLine)
				*m_pszCmdLine++ = 0;	// Null terminate and advance to next char
		}

		// Done processing __argv[0] (i.e., the executable name).  Now do th
		// actual arguments

		m_Argc = 1;

		for(;;)
		{
			// Skip over any whitespace
			while(*m_pszCmdLine && (*m_pszCmdLine == _T(' ')) || (*m_pszCmdLine == _T('\t')))
				m_pszCmdLine++;

			if(!*m_pszCmdLine) // End of command line???
				return;

			if(*m_pszCmdLine == _T('\"'))	// Argument starting with a quote???
			{
				m_pszCmdLine++;   // Advance past quote character

				m_Argv[m_Argc++] = m_pszCmdLine;
				m_Argv[m_Argc] = 0;

				// Scan to end quote, or NULL terminator
				while(*m_pszCmdLine && (*m_pszCmdLine != _T('\"')))
					m_pszCmdLine++;
                
				if(!*m_pszCmdLine)
					return ;
            
				if(*m_pszCmdLine)
					*m_pszCmdLine++ = 0;	// Null terminate and advance to next char
			}
			else	// Non-quoted argument
			{
				m_Argv[m_Argc++] = m_pszCmdLine;
				m_Argv[m_Argc] = 0;

				// Skip till whitespace or NULL terminator
				while(*m_pszCmdLine && (*m_pszCmdLine != _T(' ')) && (*m_pszCmdLine != _T('\t')))
					m_pszCmdLine++;
            
				if(!*m_pszCmdLine)
					return ;
            
				if(*m_pszCmdLine)
					*m_pszCmdLine++ = 0;	// Null terminate and advance to next char
			}

			if(m_Argc >= maxArgv)
				return ;
		}

	}
	const TCommandLineInfo & GetCommandLineInfo () const { return m_CommandLineInfo; }
	virtual void ParseCommandLine() = 0;
};

};