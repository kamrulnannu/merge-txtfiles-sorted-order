#include <bits/stdc++.h>

using namespace std;

class InputFile
{
    private: 
        string m_fname;
        ifstream m_fileDescriptor;

        // This is last line read from the file.
        // This is used to check whether input file sorted
        string m_lastReadLine;

        ifstream OpenInputFile();
        bool IsBlankLine(const string & aLine) const;
        bool IsInputLineSorted(const string & aLine, const vector<string>& lines) const;
        bool IsDuplicateLine(const string &aLine, const vector<string> & lines) const;

    public:
        InputFile(const string &name) : 
            m_fname(name)
        {
            //m_fileDescriptor = OpenInputFile();
        }

        InputFile (const InputFile &rhs)
        {
            m_fname = rhs.m_fname;
            m_fileDescriptor = OpenInputFile();
            m_lastReadLine = rhs.m_lastReadLine;
        }

        ~InputFile()
        {
            if (m_fileDescriptor.is_open())
                m_fileDescriptor.close();
        }

        bool IsEOF() const
        {
            return m_fileDescriptor.eof();
        }

        bool ReadLines(vector<string> &Lines, const size_t NumLine);
};

