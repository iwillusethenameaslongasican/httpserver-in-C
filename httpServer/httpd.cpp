//
// Created by jackson on 17-3-1.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <zconf.h>
#include <sys/stat.h>
#include <ctype.h>
#include <wait.h>

#define SERVER_STRING "Server: httpd/0.1.0\r\n"

void error_die(const char *message)
{
    //处理出错信息
    perror(message);
    exit(-1);
}

void unimplemented(int connfd)
{
    char buf[1024];

    /* HTTP method 不被支持*/
    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    send(connfd, buf, strlen(buf), 0);
    /*服务器信息*/
    sprintf(buf, SERVER_STRING);
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(connfd, buf, strlen(buf), 0);
}

void bad_request(int client)
{
    char buf[1024];

    /*回应客户端错误的 HTTP 请求 */
    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "<P>Your browser sent a bad request, ");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    send(client, buf, sizeof(buf), 0);
}

void cannot_execute(int client)
{
    char buf[1024];

    /* 回应客户端 cgi 无法执行*/
    sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
    send(client, buf, strlen(buf), 0);
}

void not_found(int connfd)
{
    char buf[1024];

    //404页面
    sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
    send(connfd, buf, strlen(buf), 0);
    //服务器信息
    sprintf(buf, SERVER_STRING);
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><TITLE>NOT FOUND</TITLE>\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "your request because the resource specified\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "is unavaliable or nonexistent.\r\n");
    send(connfd, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(connfd, buf, strlen(buf), 0);
}

int initialAcceptFd(u_short *port)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1)
    {
        error_die("socket error");
    }

    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(*port);

    if(bind(listenfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        error_die("bind error");
    }
    /*如果当前指定端口是 0，则动态随机分配一个端口*/
    if (*port == 0)  /* if dynamically allocating a port */
    {
        socklen_t namelen = sizeof(servAddr);
        if (getsockname(listenfd, (struct sockaddr *)&servAddr, &namelen) == -1)
            error_die("getsockname");
        *port = ntohs(servAddr.sin_port);
    }
    //listen将主动套接字转换成被动套接字　把套接字的状态由CLOSED状态转换成LISTEN状态
    if(listen(listenfd, 5) < 0)
    {
        error_die("listen error");
    }
    return listenfd;

}

int readline(int sockfd, char *buf, int size)
{
    char c = '\0';
    int i = 0;
    int n;
    //把终止条件统一为\n，标准化buf数组
    while((i < size - 1) && c != '\n')
    {
        n = recv(sockfd, &c, 1, 0);

        if(n > 0)
        {
            if(c == '\r')
            {
                //MSG_PEEK标志使得下次读取还是可以读取这次读取的内容
                //可认为接收窗口没有移动
                n = recv(sockfd, &c, 1, MSG_PEEK);
                //如果读到\r,则推测接下来会读到\n
                if((n > 0) && (c == '\n'))
                {
                    //如果下一个字符时\n,则把它读取掉
                    recv(sockfd, &c, 1, 0);
                }
                else
                {
                    //否则把终止条件从\r改为\n
                    c = '\n';
                }
                //以上都会造成循环在下次判断c == \n而终止
                //相当于读到了一句结尾
            }
            //没有读到结尾，则把该字符放入buf数组末尾
            buf[i] = c;
            i ++;
        }
        else
        {   //如果没有读到数据，则退出循环
            c = '\n';
        }
    }
    //'\0'是字符串结束符
    //'\n'是换行符
    buf[i] = '\0';
    return i;

}

void write_header(int connfd)
{
    char buf[1024];

    //正常的HTTP header
    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    send(connfd, buf, strlen(buf), 0);
    //服务器信息
    strcpy(buf, SERVER_STRING);
    send(connfd, buf, strlen(buf), 0);
    strcpy(buf, "Content_Type: text/html\r\n");
    send(connfd, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(connfd, buf, strlen(buf), 0);
}

void copy_file(int connfd, FILE *resource)
{
    char buf[1024];

    fgets(buf, sizeof(buf), resource);
    while(!feof(resource))
    {
        send(connfd, buf, strlen(buf), 0);
        fgets(buf, sizeof(buf), resource);
    }
}

void display_file(int connfd, char *filename)
{
    FILE *resource;
    int charnums = 1;
    char buf[1024];
    //读取并丢弃header的剩下数据
    while((charnums > 0) && (strcmp(buf, "\n") != 0))
    {
        charnums = readline(connfd, buf, sizeof(buf));
    }

    resource = fopen(filename, "r");
    if(resource == NULL)
    {
        not_found(connfd);
    }
    else
    {
        //写HTTP header
        write_header(connfd);
        //复制文件
        copy_file(connfd, resource);
    }
    fclose(resource);
}

void execute_cgi(int connfd, char *path, char *method, char *query_string)
{
    char buf[1024];
    int cgi_output[2];
    int cgi_input[2];
    int charnums = 1;
    char c;
    int content_length = -1;
    pid_t pid;
    int status;

    buf[0] = 'A', buf[1] = '\0';

    //如果请求方法是GET　则把header全部读取并丢弃
    if(strcasecmp(method, "GET") == 0)
    {
        while((charnums > 0) && (strcmp(buf, "\n")))
        {
            charnums = readline(connfd, buf, sizeof(buf));
        }
    }
    else
    {
        //对POST的HTTP请求找出content_length
        charnums = readline(connfd, buf, sizeof(buf));
        while((charnums > 0) && (strcmp(buf, "\n")))
        {
            //利用\n进行分割
            buf[15] = '\0';
            if(strcasecmp(buf, "Content-Length:") == 0)
            {
                //content-length初始化为“Content-Length:”后面的数字
                content_length = atoi(&buf[16]);
            }
            //如果没找到，则继续寻找
            charnums = readline(connfd, buf, sizeof(buf));
        }
        if(content_length == -1)
        {
            bad_request(connfd);
            return;
        }
    }

    //正确，HTTP状态码　２００
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(connfd, buf, strlen(buf), 0);

    //建立管道
    if(pipe(cgi_input) < 0)
    {
        cannot_execute(connfd);
        return;
    }
    if(pipe(cgi_output) < 0)
    {
        cannot_execute(connfd);
        return;
    }

    if((pid = fork()) < 0)
    {
        cannot_execute(connfd);
        return;
    }
    //child：　CGI　script
    if(pid == 0)
    {
        char meth_env[255];
        char query_env[255];
        char length_env[255];

        //把STDOUT重定向到cgi_output的写入端
        dup2(cgi_output[1], 1);
        //把STDIN重定向到cgi_input的读取端
        dup2(cgi_input[0], 0);
        //关闭cgi_input的写入端　和　cgi_output的读取端
        close(cgi_input[1]);
        close(cgi_output[0]);
        //设置requset_method的环境变量
        sprintf(meth_env, "REQUEST_METHOD=%s", method);
        putenv(meth_env);

        if(strcasecmp(method, "GET") == 0)
        {
            //设置query_string的环境变量
            sprintf(query_env, "QUERY_STRING=%s", query_string);
            putenv(query_env);
        }//POST
        else
        {
            //设置content_length的环境变量
            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
            putenv(length_env);
        }
        execl(path, path, NULL);
        exit(0);

    }
    else//parent
    {
        //关闭cgi_input的读取端　和 cgi_output的写入端
        close(cgi_input[0]);
        close(cgi_output[1]);
        //如果是POST，则cgi程序从标准输入读取数据
        if(strcasecmp(method, "POST") == 0)
        {
            //接收POST过来的数据
            for(int i = 0; i < content_length; i ++)
            {
                recv(connfd, &c, 1, 0);
                //把POST数据写入cgi_input，现在重定向到STDIN，而子进程cgi程序从STDIN读取数据
                //所以这里相当于是把数据写入cgi程序
                write(cgi_input[1], &c, 1);
            }
            //读取cgi_output的管道输出到客户端（相当于读cgi程序的结果）
            while(read(cgi_output[0], &c, 1) > 0)
            {
                send(connfd, &c, 1, 0);
            }

            //关闭管道
            close(cgi_input[1]);
            close(cgi_output[0]);
            //等待子进程
            waitpid(pid, &status, 0);
        }
    }
}

void *doService(void *client)
{
    int connfd = *(int *)client;
    char buf[1024];
    //存放客户端的请求方法
    char method[255];
    //存放客户端请求的url地址
    char url[255];
    char path[512];
    int numchar;
    //如果请求的是运行cgi脚本，则cgi变量设为１
    int cgi = 0;
    struct stat st;
    size_t i = 0, j = 0;
    //请求的数据
    //（GET提交，请求的数据会附在URL之后，以？分割URL和传输数据，多个参数用＆连接）
    //（POST提交，把提交的数据放置在HTTP包的包体中
    char *query_string = NULL;

    //读取客户端请求的第一行
    numchar = readline(connfd, buf, sizeof(buf));
    //读取客户端的请求方法（get 或　post) 存到method数组
    while(!isspace((int)buf[j]) && i < sizeof(method) - 1)
    {
        method[i++] = buf[j++];
    }
    method[i] = '\0';
    //服务器只处理GET请求和POST请求
    if(strcasecmp(method, "GET") && strcasecmp(method,"POST"))
    {
        unimplemented(connfd);
        return NULL;
    }
    //如果客户端的请求方法是POST　则cgi标志设为１
    if(strcasecmp(method, "POST") == 0)
    {
        cgi = 1;
    }

    i = 0;
    while(isspace((int)buf[j]) && j < sizeof(buf))
    {
        j ++;
    }
    while((!isspace((int)buf[j])) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
    {
        //存下url
        url[i++] = buf[j ++];
    }
    url[i] = '\0';
    //处理GET请求
    if(strcasecmp(method, "GET") == 0)
    {
        //把请求的数据先初始化为整个url地址
        query_string = url;

        while(*query_string != '?' && *query_string != '\0')
        {
            ++ query_string;
        }
        if(*query_string == '?')
        {
            cgi = 1;
            *query_string = '\0';
            ++ query_string;
        }
    }

    //格式化url到path数组,html文件都在htdocs中
    sprintf(path, "htdocs%s", url);
    //默认情况为index.html
    if(path[strlen(path) - 1] == '/')
    {
        strcat(path, "index.html");
    }
    //根据路径找到对应文件信息
    //如果找不到对应文件
    if(stat(path, &st) == -1)
    {
        //读取并丢弃套接字剩下的所有头部信息
        while((numchar > 0) && (strcmp("\n", buf) != 0))
        {
            numchar = readline(connfd, buf, sizeof(buf));
        }
        not_found(connfd);
    }//找到相应文件
    else
    {
        //如果是个目录
        if((st.st_mode & S_IFMT) == S_IFDIR)
        {
            strcat(path, "index.html");
        }
        //如果时cgi脚本文件
        //S_IXUSR文件所有者具有可执行权限 S_IXGRP用户组具有可执行权限　S_IXOTH其他用户具有可执行权限
        if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
        {
            cgi = 1;
        }

        if(!cgi)
        {
            //如果不是cgi,直接把服务器文件显示出来
            display_file(connfd, path);
        }
        else
        {
            //否则执行cgi程序
            execute_cgi(connfd, path, method, query_string);
        }
    }
    //断开客户端的连接，因为HTTP是无连接的
    close(connfd);

}

int main() {

    int clientfd, listenfd;
    struct sockaddr_in cliAddr;
    socklen_t cliAddrLen;
    pthread_t threadid;
    u_short port = 0;

    bzero(&cliAddr, sizeof(cliAddr));

    listenfd = initialAcceptFd(&port);
    printf("httpserver run in port:%d\n", port);

    while(1)
    {
        cliAddrLen = sizeof(cliAddr);
        int connfd = accept(listenfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
        if(connfd == -1)
        {
            error_die("accept error");
        }

        if(pthread_create(&threadid, NULL, doService, (void *)&connfd) != 0)
        {
            error_die("pthread_create error");
        }
    }
    close(listenfd);

    return (0);
}