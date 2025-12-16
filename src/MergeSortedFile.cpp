#include "MergeSortedFile.h"

void MergeFile::OpenOutFile()
{
    string cmd = "rm -f ";
    cmd += m_OutFile;

    system(cmd.c_str());

    cmd = "touch ";
    cmd  += m_OutFile;

    // Create out file so that open succeed for reading (ios::in)
    system(cmd.c_str());

    // Open out file for read and write
    m_outFD.open(m_OutFile.c_str(), ios::out | ios::in);

    if (!m_outFD)
    {
       cerr << "Error creaing out file="<<m_OutFile<<endl;
       exit(5);
    }
}

void MergeFile::Write2OutFile(const string & line)
{
    //static int CurrentLengthOfFile = 0;
    static streampos CurrentLengthOfFile = 0;
    string oLine;

    // Read file from begining to check for duplicate line
    // m_outFD.seekg(0, m_outFD.beg);

    /*
     * Set seek position so that getline() will read
     * only the last line wrote in previous call to check the duplicate
     * entry in the out file. Note that out file is already sorted.
     */
    m_outFD.seekg(CurrentLengthOfFile, m_outFD.beg);

    while (std::getline(m_outFD, oLine))
    {
        //cout << "len ="<<oLine.length() <<", R=" << oLine<<endl;
        if (strcasecmp(oLine.c_str(), line.c_str()) == 0)
        {
            // Duplicate line is not allowed
            //cout<<"Duplicate line found: "<<line<<endl;
            return;
        }
    }
/*
    if (!m_outFD.good())
    {
        cout<<"DBG:NOT GOOD, eof="<< m_outFD.eof() << endl;
    }
*/
    /*
     * Clear eof flag which was set by above getline() call.
     * I need to clear so that writing to file succeeds.
     */
    m_outFD.clear();

    /* 
     * Save current seek position so that getline
     * will read line which I am about to write
     * in the file for duplicate entry check during
     * next call to Write2OutFile().
     */
    CurrentLengthOfFile = m_outFD.tellg();
    m_outFD << line << endl;
}

void MergeFile::MergeSortedFiles()
{
    priority_queue<pq_struct> PQ;
    size_t idx = 0;
    for (auto & inf : m_InputFiles)
    {
        pq_struct ps;
        ps.InFileIdx = idx++;
        inf.ReadLines(ps.lines, m_NumLineRead);

        PQ.push(ps);
    }

    while (!PQ.empty())
    {
        pq_struct ps = PQ.top();

        PQ.pop();

        Write2OutFile(ps.lines.front());

        ps.lines.erase(ps.lines.begin());

        if (ps.lines.size() == 0)
        {
            // Read more lines from file if any
            InputFile &inf = m_InputFiles[ps.InFileIdx];
            inf.ReadLines(ps.lines, m_NumLineRead);
            if (ps.lines.size())
            {
                PQ.push(ps);
            }
            // else eof() or error 
        }
        else
        {
            PQ.push(ps);
        }
    }
}

