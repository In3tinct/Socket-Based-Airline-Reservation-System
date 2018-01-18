
/*
 * A TCP client which request a file from server and then sends the checksum of the file rx to
 * server  
 *
 * Usage : ./client <serverIP/hostname> <serverportno> <filename_wanted>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

#define MAXSIZE 2014 
#define selftest 1
#define DEBUG 1

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char **argv)
{
    int  sockfd,portno,n,file_size=-1;
    struct sockaddr_in servaddr;
    struct hostent *server;
    char *hostname;
    char buffer[MAXSIZE];
    char buf[MAXSIZE];
    int choice=-1,choice_2=-1,choice_3=-1;

    char msg[MAXSIZE];
    char *username = malloc(MAXSIZE);
    char *passwd = malloc(MAXSIZE);

#if selftest
    /* checking for no of arguments passed */
    if (argc != 3)
        error("usage: a.out <server-hostname/IP address> <portno>");

    hostname = argv[1];
    portno = atoi(argv[2]);

    /* getting server IP from hostname */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* creating socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        error("ERROR opening socket");

    /* building the server's IP address data structure */
    bzero((char *) &servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
            (char *)&servaddr.sin_addr.s_addr, server->h_length);
    servaddr.sin_port = htons(portno); 

    /* connect to server */
    n= connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if( n< 0)
        error("connection  error");
#endif 

    //The menu of client starts here 
label_1:     
    choice=-1;
    printf(" choose from following menu\n");
    printf("\t 1. customer \n \t 2. admin \n \t 3. flight-agent \n \t 4. exit from menu \n");  
    scanf("%d",&choice);
    if(choice <1 || choice >4)
    {
        printf(" wrong choice entered,try again ...\n");
        goto label_1;
    }
    if(choice==4) exit(0);

label_2:
    choice_2=-1;
    printf(" choose the type of client you are \n");
    printf("\t 1. existing customer \n \t 2. new customer \n \t 3. exit from menu \n"); 
    scanf("%d",&choice_2);
    if(choice_2 <1 || choice_2 >3)
    {
        printf(" wrong choice entered,try again ...\n");
        goto label_2;
    }
    if(choice_2 == 3) 
      exit(0); //exiting the program 
    else if(choice_2 == 1) //promting username and pwd 
    {
        printf(" enter your username : ");
        scanf("%s",username);
        printf(" enter your password : ");
        scanf("%s",passwd);
        
        //preparing the msg format 
        if(choice == 1)
            snprintf(msg,sizeof(msg),"%s%s%s%s%s", "1234","-",username,"-",passwd);
        else if( choice == 2)
            snprintf(msg,sizeof(msg),"%s%s%s%s%s", "1233","-",username,"-",passwd);
        else if( choice == 3)
            snprintf(msg,sizeof(msg),"%s%s%s%s%s", "1235","-",username,"-",passwd);
    }
    else if(choice_2 == 2) //new client 
    {
        //reading username and pwd from user
        printf(" enter your desired username : ");
        scanf("%s",username);
        printf(" enter your password : ");
        scanf("%s",passwd);

        //preparing the msg format 
        if(choice == 1)
            snprintf(msg,sizeof(msg),"%s%s%s%s%s", "1134","-",username,"-",passwd);
        else if( choice == 2)
            snprintf(msg,sizeof(msg),"%s%s%s%s%s", "1133","-",username,"-",passwd);
        else if( choice == 3)
            snprintf(msg,sizeof(msg),"%s%s%s%s%s", "1135","-",username,"-",passwd);
     }

#if DEBUG
        printf("\n the msg created is %s\n",msg);
#endif        

#if selftest      
        //sending the user's existing/desired login and passwd to server
        bzero(buf,sizeof(buf));
        strcpy(buf,msg);
        n = write(sockfd,buf,strlen(buf)); 
        if (n < 0) error("ERROR writing to socket");

        //receving the response from server  
        bzero(buf,sizeof(buf));
        n = read(sockfd,buf,sizeof(buf)-1);
        if (n < 0) error("ERROR writing to socket");

        //to-do : check for server's response and print a msg to user :
        printf("Buffer %s", buf);
	if(buf=="1234"){
	printf("Welcome customer you have been logged in succesfully\n");	
	}
	//printf(" your account has been created with username : %s",username);
#endif        
        //return 0; //to be removed later 


label_3:
    choice_3=-1;
    if(choice == 1)
    {
        printf(" choose the type of operation from menu \n");
        printf("\t 1. search flight details, -- usage format : source-destination-date, Eg: LAX-SFO-12/1/2016\n");
        printf("\t 2. book a flight, -- usage format : flightID-seatno \n");
        printf("\t 3. check the booking details, -- usage format : bookingID \n"); 
        printf("\t 4. exit from menu \n");
        scanf("%d",&choice_3);

        if(choice_3 <1 || choice_3 >4)
        {
            printf(" wrong choice entered,try again ...\n");
            goto label_3;
        }
        if(choice_3==4) exit(0);

        char *request = malloc(MAXSIZE);
        printf("\t enter your request :  ");
        scanf("%s",request);
        bzero(buf,sizeof(buf));

        if(choice_3==1) //requesting flight details  
        {
            snprintf(buf,sizeof(buf),"%s%s%s","1342","-",request); 
            printf("final sending request is %s \n",buf);
        }
        else if(choice_3 == 2) //new booking
        {
            snprintf(buf,sizeof(buf),"%s%s%s","1343","-",request); 
            printf("final sending request is %s \n",buf);
        }
        else if(choice_3 == 3) //requesting booking details 
        {
            snprintf(buf,sizeof(buf),"%s%s%s","1344","-",request); 
            printf("final sending request is %s \n",buf);
        }

#if selftest          
        //sending request to server 
        n = write(sockfd,buf,strlen(buf)); 
        if (n < 0) error("ERROR writing to socket");

        // rx response from server 
        bzero(buf,sizeof(buf));
        n = read(sockfd,buf,sizeof(buf)-1);
        if (n < 0) error("ERROR writing to socket");
       
        //printing the rx message from server for request sent 
        printf(" rx request from server is %s \n",buf);
#endif 
        /*to-do : rx search results from server and print to client 
         * extract the first words and based on that display the results 
         */

    }
    else if(choice == 2) //admin
    {
        printf(" currently not supported\n");
        goto label_1;
    }
    else //travel agent 
    {
        printf(" currently not supported\n");
        goto label_1;
    }
    return 0;

}

