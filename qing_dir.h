#ifndef QING_DIR_H
#define QING_DIR_H

#include "qing_common.h"

inline void qing_create_dir(const string& path)
{
    if(NULL==opendir(path.c_str()))
        mkdir(path.c_str(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

inline void qing_get_all_files(const string& basePath, vector<string>& files)
{
    DIR * dir;
    struct dirent * ptr;
    char base[1000];

    if ((dir=opendir(basePath.c_str())) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
            continue;
        else if (ptr->d_type == 8)   //file
        {
      //      printf("d_name:%s/%s\n", basePath.c_str(), ptr->d_name);
            files.push_back(string(ptr->d_name));
        }
        else if(ptr->d_type == 10)    //link file
        {
      //      printf("d_name:%s/%s\n",basePath.c_str(),ptr->d_name);
            files.push_back(string(ptr->d_name));
        }
        else if (ptr->d_type == 4)  //dir
        {
            memset(base, '\0', sizeof(base));
            strcpy(base, basePath.c_str());
            strcat(base, "/");
            strcat(base, ptr->d_name);
            qing_get_all_files(string(base), files);
        }
    }
}

//int getAllTypeFilesInOneFolder(const string& basePath, const string& fileType, vector<string>& files)
//{
//    fstream file_list("name.txt", std::ios::out);
//    file_list.close();

//    //写入文件名列表到file_list.txt
//    system("dir /a /b >> name.txt");

//    file_list.open("name.txt", std::ios::in);
//    string filename;
//    string extension;

//    while(!file_list.eof()){
//        getline(file_list, filename);
//        int location = filename.find(".", 0);
//        if(location != string::npos){
//            extension = filename.substr(location);
//            if(0 == extension.compare(fileType)){
//                // do something with file
//                cout << filename << endl;
//                files.push_back(filename);
//            }
//        }
//    }

//    system("PAUSE");

//}



#endif // QING_FILE_H
