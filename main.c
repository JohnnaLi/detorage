#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#include "make_log.h"


#define     FILE_UPLOAD  4000
#define     FILE_DOWNLOAD  4001
#define     FILE_DELETE  4002




#define SERV_PORT 8000

int fileapi(const char * filepath/*in*/, 
            int motion/*in*/, 
            char *fileResult/*out*/)
{
    int ret = 0;

    pid_t pid;
    int pipefd1[2];
    int pipefd2[2];
    pipe(pipefd1);
    pipe(pipefd2);
    pid = fork();
    if(pid == 0)
    {
        close(pipefd1[0]);
        close(pipefd2[0]);
        dup2(pipefd1[1], STDOUT_FILENO);//正常情况，传输fileid
        dup2(pipefd2[1], STDERR_FILENO);//异常，传输错误
        switch(motion)
        {
        case FILE_UPLOAD:
            execlp("fdfs_upload_file", "fdfs_upload_file", "./conf/client.conf",filepath, NULL);
            return ret;
        case FILE_DELETE:
            execlp("fdfs_delete_file", "fdfs_delete_file", "./conf/client.conf",filepath, NULL);
            return ret;
        case FILE_DOWNLOAD:
            execlp("fdfs_download_file", "fdfs_download_file", "./conf/client.conf",filepath, NULL);
            return ret;
        default:
            return ret;
        }


    }else if(pid > 0)
    {
        close(pipefd1[1]);
        close(pipefd2[1]);
//        char buf1[1024] = {0};//正常情况，传输fileid
  //      char buf2[1024] = {0};//异常，传输错误
        read(pipefd1[0], fileResult, 1024);
        fileResult[strlen(fileResult) - 1] = '\0';
        if(strlen(fileResult) != 0)
        {

            LOG("1111", "222", "%s ,%d", fileResult, 100);
           // strcpy(fileResult, buf1);
           // (*fileResult) = (char *)calloc(1024, 1);
           // memcpy((*fileResult), buf1, strlen(buf1));            
        }
        memset(fileResult, 0, 1024);
        read(pipefd2[0], fileResult, 1024);
        fileResult[strlen(fileResult) - 1] = '\0';
        if(strlen(fileResult) != 0){
            LOG("1111", "222", "%s ,%d", fileResult, 100);
          //  ret = -1;
          //  (*fileResult) = (char *)calloc(1024, 1);
          //  memcpy((*fileResult), buf2, strlen(buf2));
          //  strcpy(fileResult, buf2);
        }


    }

    return ret;         

}


int main()
{

    char file[1024] = "a.c";
    int motion = FILE_UPLOAD;
    char fileid[1024] = {0};
    char fileResult[1024] = {0};

    int ret = fileapi(file, motion, fileid);//上传文件
    if(ret == -1)
    {
        printf("FILE_UPLOAD err\n");

    }else{
        printf("FILE_UPLOAD success\n");
        printf("fileid=#%s#\n%d\n", fileid, (int)strlen(fileid));

    }
    //fileid[strlen(fileid) - 1] = '\0';



    ret = fileapi(fileid, FILE_DOWNLOAD, fileResult);//下载文件
    if(ret == -1)
    {
        printf("FILE_DOWNLOAD err\n");

    }else{
        printf("FILE_DOWNLOAD success\n");

    }

    ret = fileapi(fileid, FILE_DELETE, fileResult);//删除云端文件
    if(ret == -1)
    {
        printf("FILE_DELETE err\n");

    }else{
        printf("FILE_DELETE success\n");


    }


    return 0;

}

