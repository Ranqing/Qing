#ifndef QING_DIR_H
#define QING_DIR_H

#include "qing_common.h"

// using namespace std;


#ifdef _WIN32
inline void qing_create_dir(const string& path) {
 /*if(NULL==opendir(path.c_str()))
        mkdir(path.c_str(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);*/
    _mkdir(path.c_str());
}
#else

inline void qing_create_dir (const string &path) {
    if (NULL == opendir(path.c_str()))
        mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

#endif

#ifdef _WIN32
inline void qing_get_all_files(const string& path, vector<string>& files)
{
    _finddata_t file;
    long lf;
    //输入文件夹路径
    if ((lf=_findfirst(path.c_str(), &file)) == -1) {
        cout<<path<<" not found!!!"<<endl;
    } else {
        while(_findnext(lf, &file) == 0) {
            //输出文件名  
            cout<<file.name<<endl;  
            if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)  
                continue;  
            files.push_back(file.name);  
        }  
    }  
    _findclose(lf); 
}
#else

inline void qing_get_all_files (const string &path, vector<string> &files) {
    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir = opendir(path.c_str())) == NULL) {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr = readdir(dir)) != NULL) {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
            continue;
        else if (ptr->d_type == 8)   //file
        {
            //      printf("d_name:%s/%s\n", path.c_str(), ptr->d_name);
            files.push_back(string(ptr->d_name));
        } else if (ptr->d_type == 10)    //link file
        {
            //      printf("d_name:%s/%s\n",path.c_str(),ptr->d_name);
            files.push_back(string(ptr->d_name));
        } else if (ptr->d_type == 4)  //dir
        {
            memset(base, '\0', sizeof(base));
            strcpy(base, path.c_str());
            strcat(base, "/");
            strcat(base, ptr->d_name);
            qing_get_all_files(string(base), files);
        }
    }
}

inline void qing_get_all_files (const string &path, vector<string> &files, string keyword) {
    vector<string> all_files(0);
    qing_get_all_files(path, all_files);
    if (keyword == "") {
        copy(all_files.begin(), all_files.end(), back_inserter(files));
        return;
    }

    for (int i = 0; i < all_files.size(); ++i) {
        if (-1 != all_files[i].find(keyword)) {
            files.push_back(all_files[i]);
        }
    }
    return;
}

#endif

inline void qing_cwd () {
    char cwd[1024];
    getcwd(cwd, 1024);
    printf("\t%s\n", cwd);
}


#endif // QING_FILE_H
