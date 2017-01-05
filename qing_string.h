#ifndef RQ_STRING_H
#define RQ_STRING_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

inline string getSaveFileName(const string& folder, const int n, const string& suffix)
{
    stringstream ss;
    ss << setw(10) << setfill('0') << n << suffix;
    string fileName = folder + "/" + ss.str() ;

    return fileName;
}

inline string getFolderNameFromFullPath(const string fullPath)
{
    int n = std::max((int)fullPath.find('/'), 0);
    string folder = fullPath.substr(0, n);              // ""

    if(folder == "." || folder == "")
        folder = "images";

    return folder;
}

inline string getFileNameFromFullPath(const string fullPath)
{
    int n = fullPath.rfind('/');
    string fileName = fullPath.substr(n+1);

    if(fileName.find('.') == -1)
        fileName = "";

    return fileName;
}

inline string getLastFolderFromFullPath(const string fullPath)
{
    int n = fullPath.rfind('/');
    string fileName = fullPath.substr(n+1);

    if(fileName.find('.') != -1)
    {
        string temppath = fullPath.substr(0,n);
        n = temppath.rfind('/');
        fileName = temppath.substr(n+1);
    }

    return fileName;
}

inline string getFileSuffix(const string fileName)
{
    int n = fileName.rfind('.');

    string suffix = "";
    if(n != -1)
    {
        suffix = fileName.substr(n+1);
    }
    return suffix;
}

inline string getFilePrefix(const string fileName)
{
    int n = fileName.rfind('.');
    if(n != -1)
        return fileName.substr(0, fileName.rfind('.'));
    else
        return "";
}


inline void readInStringList(const string fileName, vector<string>& strs)
{
    fstream fin(fileName.c_str(), ios::in);
    if(fin.is_open() == false)
    {
        cout << "failed to open " << fileName.c_str() << endl;
        return;
    }

    string file;
    while(fin >> file)
    {
        strs.push_back(file);
    }

    fin.close();
}

//folder
inline void writeOutStringList(const string& fileName, vector<string>& strs, const string& folder = "")
{
    fstream fout(fileName.c_str(), ios::out);
    if(fout.is_open() == false)
    {
        cout << "failed to open " << fileName.c_str() << endl;
        return ;
    }

    int n = strs.size();
    bool isStripped = (folder == "") ? false : true;

    for(int i = 0; i < n; ++i)
    {
        string temp = isStripped ? getFileNameFromFullPath(strs[i]) : strs[i];
        string file = (folder == "") ? temp : (folder + "/" + temp);

        fout << file << endl;
    }

    fout.close();
}

inline void qing_split_a_string_by_space(const string str, vector<string>& words)
{
    std::istringstream iss(str);
    do
    {
        std::string sub;
        iss >> sub;
        words.push_back(sub);
    } while (iss);
}

inline int string2int(const string& str)
{
    stringstream ss(str);

    int num;
    ss >> num;
    return num;
}

inline string int2string(const int& num)
{
    stringstream ss;
    ss << num;
    return ss.str();
}

inline string int2FormatString(const int& num, const int& w, const char ch)
{
    stringstream ss;
    ss << setfill(ch) << setw(w) << num ;
    return ss.str();
}

#endif // RQ_STRING_H
