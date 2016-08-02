#include "head.h"

int main(int argc,char **argv)
{
    if(argc != 3){
        printf("error args\n");
        return -1;
    }

    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == sfd){
        perror("socket");
        return -1;
    }


    struct sockaddr_in ser;
    bzero(&ser,sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(atoi(argv[2]));
    ser.sin_addr.s_addr  = inet_addr(argv[1]);

    int ret;
    ret = connect(sfd,(struct sockaddr*)&ser,sizeof(struct sockaddr));
    if(-1 == ret){
        perror("connect");
        return -1;
    }
    printf("connect success\n");

    char buf[128];
    int fd;
    int flag = 0;
    while(1){
        bzero(buf,sizeof(buf));
        ret = read(0,buf,sizeof(buf));
        if(-1 == ret){
            perror("read");
            return -1;
        }
        send(sfd,buf,strlen(buf),0);
        bzero(buf,sizeof(buf));
        ret = recv(sfd,buf,sizeof(buf),0);
        //printf("sizeof buf is %d\n",(int)sizeof(buf));
        printf("%s\n",buf);
        //下载文件
        if(memcmp(buf,"downloading...",14) == 0){
            printf("ready to receive files\n");
            data_t data;
            bzero(&data,sizeof(data));
            //接收文件名
            ret = recv(sfd,&data.len,sizeof(int),0);
            printf("file name,len is %d\n",data.len);
            if(-1 == ret){
                perror("recv1");
                return -1;
            }
            printf("data.len is %d\n",data.len);
            recv(sfd,data.buf,data.len,0);
            printf("recv name success,name is %s",data.buf);
            fd = open(data.buf,O_RDWR | O_CREAT,0666);
            if(-1 == fd){
                perror("open");
                return -1;
            }
            while(1){
                bzero(&data,sizeof(data));
                ret= recv(sfd,&data.len,4,0);
                if(-1 == ret){
                    perror("recv2");
                    return -1;
                }
                //为什么这里不是recv_n
                recv(sfd,data.buf,data.len,0);
                if(-1 == ret){
                    perror("recv3");
                    return -1;
                }
                //结束文件传输
                if(data.len == sizeof(int) && !memcmp(data.buf,&flag,4)){
                    break;
                }
                ret = write(fd,data.buf,data.len);
                if(ret < 0){
                    perror("write");
                    return -1;
                }
            }
        }
        if(memcmp(buf,"uploading...",12 == 0)){
            char index[128];
            bzero(index,sizeof(index));
            recv(sfd,index,sizeof(index),0);
            printf("index is %s\n",index);

            printf("ready to upload files\n");
            data_t data;
            bzero(&data,sizeof(data));
            //接收文件名
            data.len = strlen(index);
            strcpy(data.buf,index);

            /*data.len是data.buf的长度，+4是再加上int len的4个字节的长度*/
            ret = send(sfd,&data,data.len + 4,0);//?发送文件名
            if(-1 == ret){
                perror("send1");
                return -1;
            }
            fd = open(index,O_RDONLY);
            if(-1 == fd){
                perror("open");
                return -1;
            }
            //此处运用巧妙办法，直接将data.len设置为read的返回值
            while(bzero(&data,sizeof(data)),(data.len = read(fd,data.buf,sizeof(data.buf))) > 0){
                send_n(sfd,(char*)&data,data.len + 4);

                //send(sfd,&data,data.len + 4,0);
            }

            data.len = sizeof(int);
            memcpy(data.buf,&flag,4);
            ret = send(sfd,&data,data.len + 4,0);//?发送结束符
            if(-1 == ret){
                perror("send2");
                return -1;
            }

        }
    }
    close(sfd);
    return 0;
}


