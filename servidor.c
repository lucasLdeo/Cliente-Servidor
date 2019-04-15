#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    printf("Criando Servidor");
    char buff[300];
    char resposta[1024];
    
    int client,x;
    struct sockaddr_in saddr;

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(5000);


    bzero(&(saddr.sin_zero), 8); 
	
	printf("Servidor criado");    
    int server = socket(AF_INET,SOCK_STREAM, 0);
    int csize = sizeof saddr;
    int sockfd = socket(AF_INET,SOCK_STREAM, 0);
    char mensagem[30];


	FILE * Arquivo;


    
    bind(server, (struct sockaddr *) &saddr, sizeof saddr);
    listen(server,1);
    printf("esperando conexcao");
    
    while (1){
        client = accept(server,(struct sockaddr *) &saddr, &csize);
        if (client != -1){      
             printf("Cliente conectado"); 
        x = recv(client,buff, sizeof buff, 0);              
        Arquivo = fopen(buff,"rb");

        if (Arquivo != NULL){

            break;
        }else{
            strcpy(resposta,"HTTP/1.0 404 OK\n");
            printf("%s",resposta);
            send(client,resposta,strlen(resposta),0);      
            break;
        }
        puts(buff);
        fflush(stdout); 
        }
    }
    return 1;
}
