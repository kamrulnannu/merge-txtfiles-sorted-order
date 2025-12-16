#include <bits/stdc++.h>
#include "InputFile.h"

using namespace std;

class MergeFile
{
    private:
        // Merged sorted outfile
        string m_OutFile;
        fstream m_outFD;        

        vector<InputFile> m_InputFiles;

        // Location of input text file
        string m_InputDir;

        /*
         * Due to shortage of Main Memory (RAM), it will read
         * at most m_NumLineRead lines from each of input text file
         *
         * Example:
         * Number of Input File = 10 (size(m_InputFilesa))
         * m_NumLineRead = 5 (User may enter this data from command line)
         * MaxLength of a line = 80 bytes.
         *
         * There will be at most 5*10 = 50 lines i.e. 50*80=4000 bytes
         * input data in the main memory at any given time.
         */
        size_t m_NumLineRead;

        struct pq_struct
        {
            size_t InFileIdx;  // index of input file from m_InputFiles
            vector<string> lines;

            bool operator < (const pq_struct & rhs) const
            {
                // For MinHeap

                if (strcasecmp(lines[0].c_str(), rhs.lines[0].c_str()) > 0)
                {
                    return true;
                }
                return false;
            }
        };

        void OpenOutFile();
        void Write2OutFile(const string & line);

    public:

        explicit MergeFile(size_t NumLineRead = 5) :
                m_NumLineRead(NumLineRead)
        {
            if (NumLineRead < 2)
            {
                cout << "INFO: Number of read line per input file is less than 2. "
                     << "Setting it to 2"<<endl;
                m_NumLineRead = 2;
            }
        }


        ~MergeFile()
        {
            if (m_outFD.is_open())
                m_outFD.close();
        }

        void SetInputDir(const string &InputDir) 
        {
            m_InputDir = InputDir;
        }

        void SetOutFile(const string &out)
        {
            m_OutFile = out;
            OpenOutFile();
        }

        void AddInputFile(const string &file)
        {
            string path = m_InputDir + "/" + file;

            InputFile inf(path);

            m_InputFiles.push_back(inf);
        }
        void MergeSortedFiles();
};
