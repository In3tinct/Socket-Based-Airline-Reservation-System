#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<pthread.h>

//Function to split the string with hyphen - as delimitar
int split (const char *str, char c, char ***arr)
{
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *p;
    char *t;

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
            count++;
        p++;
    }

    *arr = (char**) malloc(sizeof(char*) * count);
    if (*arr == NULL)
        exit(1);

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
        {
            (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
            if ((*arr)[i] == NULL)
                exit(1);

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
    if ((*arr)[i] == NULL)
        exit(1);

    i = 0;
    p = str;
    t = ((*arr)[i]);
    while (*p != '\0')
    {
        if (*p != c && *p != '\0')
        {
            *t = *p;
            t++;
        }
        else
        {
            *t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }

    return count;
}


void *processClients(void* clientsock){

int n;
int client_sock = (int)clientsock;
char buf[1024];

char* responseCode;

//type of user
char *type="";

while(1)
{
bzero(buf,sizeof(buf));

if((n=recv(client_sock,buf,sizeof(buf),0)<=0)){
fflush(stdout);
perror("Socket closed by client");
close(client_sock);
break;
}


//Delimiter with hyphen, extracting keywords
    int i;
    char *s = buf;
    int c = 0;
    char **arr = NULL ;

    c = split(s, '-', &arr);

    //printf("found %d tokens.\n", c);

    /*for (i = 0; i < c; i++){
        printf("string #%d: %s\n", i, arr[i]);
	fflush(stdout);	
	}*/

//arr[c-1]=arr[c-1]+"/0";

//When the user is customer
if(strcmp(type, "customer")==0){
//Search for flight details
if(strcmp(arr[0], "1342") == 0){
char *source= arr[1];
char *destination=arr[2];
char *date=arr[3];
//Response as 3333-FlightId-Seats available
char *result="3333-AI21-5";
send(client_sock,result,sizeof(result),0);
}
//new booking
else if(strcmp(arr[0], "1343") == 0){
char *flightId= arr[1];
char *noOfSeats=arr[2];
char *date=arr[3];
char *result="Booking done";
send(client_sock,result,sizeof(result),0);
}
//Booking details
else if(strcmp(arr[0], "1344") == 0){


}

}//Main if ends



//For Login, Customer
if (strcmp(arr[0], "1234") == 0) {
if((strcmp(arr[1], "customer") == 0) && (strcmp(arr[2], "pass") == 0)){
responseCode="1111";
type="customer";
printf("Printing %s", type);
fflush(stdout);
send(client_sock,responseCode,sizeof(responseCode),0);
}else 
{
printf("Type %s", type);
responseCode="2222";
send(client_sock,responseCode,sizeof(responseCode),0);
}

} 
//For Login, Admin
else if(strcmp(arr[0], "1233") == 0){
type="admin";

}
//For Login, Airline provider
else if(strcmp(arr[0], "1235") == 0){
type="agent";
}
//Invalid choice
else{

}

}//while ends here
}

void requesthandler(int clientSock)
{
    void* clientSocket = (void*)clientSock;

    pthread_t threads;
    pthread_create( &threads, NULL, processClients, clientSocket);
}

int main(int argc, char **argv){

/*cli socket is returned by the accept function,
which is later used to send or receive data*/ 
int sock,cli;

/*client structure is filled when a client connects with details
like IP address port etc.*/
struct sockaddr_in server, client;
int len;
char firstmsg[]="Welcome to airline reservation system, Enter yoour choice \n 1)Register \n 2)Login";
char registerOrLogUser[]="Please enter username,password,type of user all seperated by space";

int sent;
int data_len;
int MAX_DATA=1024;
char data[MAX_DATA];

if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
perror("socket");
exit(-1);
}

server.sin_family=AF_INET;
server.sin_addr.s_addr=INADDR_ANY;
server.sin_port=htons(atoi(argv[1]));
bzero(&server.sin_zero,8);

len=sizeof(struct sockaddr_in);

if((bind(sock,(struct sockaddr *)&server, len))==-1){
perror("bind");
exit(-1);
}


if((listen(sock,5))==-1){
perror("Listen");
exit(-1);
}

/*accept is a blocking function, the code after that wont execute
until a client connects*/
printf("Server is listening and waiting for connections \n");
while(1){
if((cli=accept(sock,(struct sockaddr *)&client,&len))==-1){
perror("Accept");
exit(-1);
}
printf("Client connected \n");
requesthandler(cli);


}//while loop ends

}
