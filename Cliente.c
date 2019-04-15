#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    struct sockaddr_in saddr;
    int sockfd, bytes, csize = sizeof saddr;
    char buff[1024],path[30], host[30], *data;
    struct hostent *he;
  
    if (argc != 3) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    int cont = 0, contUrl = 0, verifica = 0;
    memset(host,'\0',30);
    memset(path,'\0',30);
    
    printf("Preparando Caminho: "); 
    while(argv[1][cont] != '\0'){
        if (argv[1][cont] == '/' || verifica == 1){
            verifica = 1;                
            path[contUrl] = argv[1][cont];    
            
        }else{
            host[cont] = argv[1][cont];
        }
        
    contUrl++;
    cont++;
    }
    printf("host: %s        path: %s\n", host, path);
    printf("Montando Requisicao:    ");
    
    char requisicao[70];
    memset(requisicao,'\0',30);
 
    if (verifica == 1){
        strcat(requisicao,"GET ");
        strcat(requisicao,path);
    }else{
        strcat(requisicao,"GET /index.html\n");
       
    }
    printf("%s \n",requisicao);
    if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
        herror("gethostbyname");
        exit(1);
    }
 
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    saddr.sin_family = AF_INET;        
    saddr.sin_addr = *((struct in_addr *)he->h_addr);
    saddr.sin_port = htons(80);
    bzero(&(saddr.sin_zero), 8);  
    

    if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) == -1){
        perror("Erro ao criar o socket");
        exit(1);
    }
    
    printf("Vou conectar\n");
    if (connect(sockfd,(struct sockaddr *) &saddr, csize) == -1){
        perror("connect");
        exit(1);        
    }
    
    printf("%s",requisicao);
    printf("%s\n",buff);     
    FILE *Arquivo;

    Arquivo = fopen(argv[2],"wb");
    
    int data_size = bytes - (data - buff);
	fwrite(data, sizeof(char), data_size, Arquivo);
    send(sockfd,requisicao,strlen(requisicao),0);    
    bytes = 1;
    char statusRequisicao[4];
    int VerificaCabecalho =1;
    while(bytes!=0){
        memset (buff,'\0',1024);        
        if ((bytes = recv(sockfd,buff, 1024, 0)) == -1){
            perror("Erro ao receber mensagem");
            
        }   
        if (VerificaCabecalho){
            int i;
            for (i = 0; i < 3; i++)		//get the status code
            statusRequisicao[i] = buff[i+9];
            statusRequisicao[i++] = '\0';
	        printf("%s\n",statusRequisicao);
            if (atoi(statusRequisicao) == 200){
                printf("%s\n\n",buff);

                data = strstr( buff, "\r\n\r\n" );
                if ( data != NULL )
                    data += 4;

                VerificaCabecalho = 0;
                }else{
                    printf("Falha ao baixar o arquivo\n");
                    return 1;                    
                }
        } else{
            fwrite(buff,sizeof(char),bytes, Arquivo);
        }
    }

    fclose(Arquivo);
    close(sockfd);
   return 1;
}
