#include "InputFile.h"

ifstream InputFile::OpenInputFile()
{
    ifstream ifs;
    ifs.open(m_fname.c_str(), ios::in);

    if (!ifs)
    {
        cerr << "ERROR: Can not open input file = " << m_fname << endl;
        exit(3);
    }
    return ifs;
}

bool InputFile::IsBlankLine(const string & aLine) const
{
    if (aLine.length() == 0)
    {
        return true;
    }
    string temp (aLine.length(), ' ');
    if (temp == aLine)
    {
        // Input line is created of space
        return true;
    }
    for (auto ch : aLine)
    {
        if (std::isalnum(ch))
        {
            // line has at least one alpha numeric char: Not a blank line
            return false;
        }
    }

    // Thete is no alpha numueric character, consider it as blank line
    return true;
}

bool InputFile::IsInputLineSorted(const string & aLine, const vector<string> & Lines) const
{
    if (Lines.size() == 0)
    {
        return true;
    }

    const string & bk = Lines.back();
    if (strcasecmp(aLine.c_str(), bk.c_str()) >= 0)
    {
        return true;
    }

    cerr << "ERROR: Input file not sorted, file="<< m_fname << endl;
    cerr << "Previous Line ="<< bk << endl
         << "Current Line = " << aLine << endl;

    exit(4);
}

bool InputFile::IsDuplicateLine(const string & aLine, const vector<string> & Lines) const
{
    for (auto line : Lines)
    {
        if (strcasecmp(line.c_str(), aLine.c_str()) == 0)
        {
            return true;
        }
    }

    return false;
}

bool InputFile::ReadLines(vector<string> & Lines, const size_t NumLine)
{
    Lines.clear();

    if (m_fileDescriptor.eof())
    {
        if (m_lastReadLine.length())
        {
            Lines.push_back(m_lastReadLine);
            m_lastReadLine = "";
        }
        return false; // Indicator of eof
    }

    if (!m_fileDescriptor.good())
    {
        cerr << "ERROR: ReadLines - State of input stream is not good, file="
             <<m_fname<<endl;
        return false;
    }
    
    if (m_lastReadLine.length())
    {
        Lines.push_back(m_lastReadLine);
        m_lastReadLine = "";
    }
    string aLine;
  
    /*
     * Due to RAM shortage, the follwing code read at most
     * 1) NumLine lines for 1st read request
     * 2) (NumLine - 1) lines for next read request
     *
     * That means, at a given point of time there will be
     * at most NumLine lines in memory for each input text files
     */
    while (std::getline(m_fileDescriptor, aLine))
    {
        if (!IsBlankLine(aLine) && 
            IsInputLineSorted(aLine, Lines) &&
            !IsDuplicateLine(aLine, Lines))
        {
            Lines.push_back(aLine);

            if (Lines.size() == NumLine)
            {
                break;
            }
        }
    }

    if (Lines.size() == NumLine)
    {
        if (m_fileDescriptor.eof())
        {
            // all lines are read from input file
            m_lastReadLine = "";
            return false;
        }
        else
        {
            m_lastReadLine = Lines.back();
            Lines.pop_back();

            // There are more lines in input file
            return true;
        }
    }
    else if (m_fileDescriptor.eof())
    {
        m_lastReadLine = "";
    }

    return false;
}
