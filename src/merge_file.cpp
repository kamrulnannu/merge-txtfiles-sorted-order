#include <bits/stdc++.h>
#include <sys/types.h>
#include <dirent.h>
#include "MergeSortedFile.h"

using namespace std;


static void Usage(const char * cmd)
{
    cerr << "ERROR: Invalid command line input." << endl;
    cout << "Usage: " << cmd << " InputDirName OutputFileName "
         << "[NumLinePerReadPerInputTextFile]" << endl;
    exit(1);
}

static void ReadDir(const char *dir_name, MergeFile & mf)
{
    DIR *dir = opendir(dir_name);

    if (dir == NULL)
    {
        cerr << "ERROR: ReadDir: Could not open input dir=" << dir_name << endl;
        exit(2);
    }

    struct dirent *dir_ptr;

    while((dir_ptr = readdir(dir)) != NULL)
    {
        if (strcmp(dir_ptr->d_name, ".") && // Don't use . and .. dir
            strcmp(dir_ptr->d_name, ".."))
        {
            mf.AddInputFile(dir_ptr->d_name);

            // cout << "FileName = " << dir_ptr->d_name << endl;
        }
    } 

    closedir(dir);
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        Usage(argv[0]);        
    }

    size_t NumLine = 5;

    if (argc == 4)
    {
        //NumLine = atoi(argv[3]);
        NumLine = strtoul(argv[3], 0, 10);
    }
    MergeFile mf(NumLine);

    mf.SetInputDir(argv[1]);
    mf.SetOutFile(argv[2]);
    ReadDir(argv[1], mf);
    mf.MergeSortedFiles();

    return 0;
}
