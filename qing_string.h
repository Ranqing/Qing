#ifndef QING_STRING_H
#define QING_STRING_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using std::stringstream;
using std::fstream;
using std::ofstream;
using std::ifstream;

//older version
inline string getSaveFileName(const string &folder, const int n, const string &suffix) {
    stringstream ss;
    ss << setw(10) << setfill('0') << n << suffix;
    string fileName = folder + "/" + ss.str();
    return fileName;
}

inline string qing_get_save_filename(const string &folder, const int n, const string suffix) {
    stringstream ss;
    ss << setw(10) << setfill('0') << n << suffix;
    string fileName = folder + "/" + ss.str();
    return fileName;
}

//older version
inline string getFolderNameFromFullPath(const string fullPath, const string default_folder = "images") {
    int n = std::max((int) fullPath.find('/'), 0);
    string folder = fullPath.substr(0, n);              // ""
    if (folder == "." || folder == "")
        folder = default_folder;
    return folder;
}

inline string qing_get_folder_name_from_full_path(const string fullPath, const string default_folder = "images") {
    int n = std::max((int) fullPath.find('/'), 0);
    string folder = fullPath.substr(0, n);              // ""
    if (folder == "." || folder == "") folder = default_folder;
    return folder;
}

//older version
inline string getFileNameFromFullPath(const string fullPath) {
    int n = fullPath.rfind('/');
    string fileName = fullPath.substr(n + 1);
    if (fileName.find('.') == -1)
        fileName = "";
    return fileName;
}

inline string qing_get_file_name_from_full_path(const string fullPath) {
    int n = fullPath.rfind('/');
    string fileName = fullPath.substr(n + 1);
    if (fileName.find('.') == -1) fileName = "";
    return fileName;
}

//older version
inline string getLastFolderFromFullPath(const string fullPath) {
    int n = fullPath.rfind('/');
    string fileName = fullPath.substr(n + 1);

    if (fileName.find('.') != -1) {
        string temppath = fullPath.substr(0, n);
        n = temppath.rfind('/');
        fileName = temppath.substr(n + 1);
    }
    return fileName;
}

inline string qing_get_last_folder_from_full_path(const string fullPath) {
    int n = fullPath.rfind('/');
    string fileName = fullPath.substr(n + 1);

    if (fileName.find('.') != -1) {
        string temppath = fullPath.substr(0, n);
        n = temppath.rfind('/');
        fileName = temppath.substr(n + 1);
    }
    return fileName;
}

//older version
inline string getFileSuffix(const string fileName) {
    int n = fileName.rfind('.');

    string suffix = "";
    if (n != -1) {
        suffix = fileName.substr(n + 1);
    }
    return suffix;
}

inline string qing_get_file_suffix(const string fileName) {
    int n = fileName.rfind('.');
    string suffix = "";
    if (n != -1) {
        suffix = fileName.substr(n + 1);
    }
    return suffix;
}

//older version
inline string getFilePrefix(const string fileName) {
    int n = fileName.rfind('.');
    if (n != -1)
        return fileName.substr(0, fileName.rfind('.'));
    else
        return "";
}

inline string qing_get_file_prefix(const string fileName) {
    int n = fileName.rfind('.');
    if (n != -1)
        return fileName.substr(0, fileName.rfind('.'));
    else
        return "";
}


//older version
inline void readInStringList(const string fileName, vector<string> &strs) {
    fstream fin(fileName.c_str(), ios::in);
    if (fin.is_open() == false) {
        cout << "failed to open " << fileName.c_str() << endl;
        return;
    }

    string file;
    while (fin >> file) {
        strs.push_back(file);
    }

    fin.close();
}

inline void qing_read_in_string_list(const string fileName, vector<string> &strs) {
    fstream fin(fileName.c_str(), ios::in);
    if (fin.is_open() == false) {
        cout << "failed to open " << fileName.c_str() << endl;
        return;
    }

    string file;
    while (fin >> file) {
        strs.push_back(file);
    }
    fin.close();
}

//older version
//folder
inline void writeOutStringList(const string &fileName, vector<string> &strs, const string &folder = "") {
    fstream fout(fileName.c_str(), ios::out);
    if (fout.is_open() == false) {
        cout << "failed to open " << fileName.c_str() << endl;
        return;
    }

    int n = strs.size();
    bool isStripped = (folder == "") ? false : true;

    for (int i = 0; i < n; ++i) {
        string temp = isStripped ? getFileNameFromFullPath(strs[i]) : strs[i];
        string file = (folder == "") ? temp : (folder + "/" + temp);

        fout << file << endl;
    }

    fout.close();
}

inline void qing_write_out_string_list(const string &fileName, vector<string> &strs, const string &folder = "") {
    fstream fout(fileName.c_str(), ios::out);
    if (fout.is_open() == false) {
        cout << "failed to open " << fileName.c_str() << endl;
        return;
    }

    int n = strs.size();
    bool isStripped = (folder == "") ? false : true;
    for (int i = 0; i < n; ++i) {
        string temp = isStripped ? getFileNameFromFullPath(strs[i]) : strs[i];
        string file = (folder == "") ? temp : (folder + "/" + temp);
        fout << file << endl;
    }
    fout.close();
}

inline int qing_string_2_int(const string &str) {
    stringstream ss(str);
    int num;
    ss >> num;
    return num;
}

inline string qing_int_2_string(const int &num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

inline string qing_int_2_format_string(const int &num, const int &w, const char ch) {
    stringstream ss;
    ss << setfill(ch) << setw(w) << num;
    return ss.str();
}

inline double qing_string_2_double(const string &str) {
    stringstream ss(str);
    double num;
    ss >> num;
    return num;
}

inline string qing_double_2_string(const double &num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

inline void qing_split_a_string_by_space(const string str, vector<string> &words) {
    std::istringstream iss(str);
    do {
        std::string sub;
        iss >> sub;
        words.push_back(sub);
    } while (iss);
}

inline bool qing_check_file_suffix(const string fileName, const string suffix) {
    string sstr = fileName.substr(fileName.rfind('.'), fileName.length() - fileName.rfind('.'));
    if (suffix != sstr)
        return false;
    else
        return true;
}

inline void qing_split(const std::string& str, char ch, std::vector<std::string>* result) {
  result->clear();
  std::stringstream ss(str);
  std::string segment;
  while (std::getline(ss, segment, ch)) {
    result->push_back(segment);
  }
}

#endif // QING_STRING_H
