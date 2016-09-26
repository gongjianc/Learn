#include "head.h"

#define MAX_HTTP_LENGTH 409600

int main(int argc,char **argv)
{
    if(argc != 3){
        printf("error args\n");
        return -1;
    }

    char buf[4096];
    bzero(buf, sizeof(buf));

    FILE *fp;
    if( (fp = fopen("./chinaso.txt", "r")) == NULL){
        perror("open file error");
        return -1;
    }
    char *temp = NULL;
    temp = (char *)malloc(10240 * sizeof(char));
    fread(temp, 1024, 5, fp);
    fclose(fp);

    struct hostent *host = gethostbyname(argv[1]);
    if(!host){
        perror("gethostbyname");
        return -1;
    }
    for(int i = 0; host->h_aliases[i]; i++){
        printf("Aliases %d: %s\n", i + 1, host->h_aliases[i]);
    }
    printf("Address type: %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
    for(int i = 0; host->h_addr_list[i]; i++)
        printf("IP addr %d: %s\n", i + 1, inet_ntoa( *(struct in_addr *)host->h_addr_list[i]));

    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == sfd){
        perror("socket");
        return -1;
    }

    struct sockaddr_in ser;
    bzero(&ser,sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(atoi(argv[2]));
    ser.sin_addr.s_addr  = inet_addr(inet_ntoa( *(struct in_addr *)host->h_addr_list[0]));

    int ret;
    ret = connect(sfd,(struct sockaddr*)&ser,sizeof(struct sockaddr));
    if(-1 == ret){
        perror("connect");
        return -1;
    }

    printf("connect success\n");

    int fd;
    int flag = 0;
    do{
        printf("send data is =========\n%s\n", temp);
        send(sfd, temp, strlen(temp),0);
        printf("========================================RECV:=====================================\n");
        /* printf("========================================RECV:=====================================\n%s\n", buf); */
        recv_n(sfd, buf, MAX_HTTP_LENGTH);
    }while(0);

    sleep(3);

    if(-1 == close(sfd)){
        perror("close fd : ");
    }

    return 0;
}
