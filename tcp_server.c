#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<pthread.h>
#include <mysql.h>


void freeMem(char * result)
{
	if(result != NULL)
	{
		free(result);
		result = NULL;
	}
}

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

void logging(char * log){
FILE *pFile;
pFile=fopen("log.txt", "a");
fprintf(pFile, log);
fclose(pFile);
}

char* deleteUser(char * id){
MYSQL *conn;
   
   char *server = "localhost";
   char *user = "root";
   char *password = "root"; /* set me first */
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }


char *query="delete from login where id=";


char *temp = malloc(10000);

strcpy(temp, query);
strcat(temp,id);

printf("query %s \n", temp);

   /* send SQL query */
   if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

	
	/* close connection */
   
   	mysql_close(conn);
	freeMem(temp);
   
	return 0;
}


char* bookingDetailsForSpecificCustomer(char * customer){
MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;
   char *resultToSend = malloc(1000*1000);
   sprintf(resultToSend,"");
   char *server = "localhost";
   char *user = "root";
   char *password = "root"; /* set me first */
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }


char *query="select * from bookingdetails where customerid=";


char *temp = malloc(10000);

strcpy(temp, query);
strcat(temp,customer);

printf("query %s \n", temp);

   /* send SQL query */
   if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   res = mysql_use_result(conn);


   /* output table name */
   printf("MySQL Tables in mysql database:\n");
   /*while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);*/

	unsigned int    numFields;
	numFields = mysql_field_count(conn);
	int check = 0;
	//Just to add 3333# if there is any entry	
	int add3333=0;
	while(row = mysql_fetch_row(res)) // row pointer in the result set
	        {
			check=0;
			
			if(add3333==0){
				sprintf(resultToSend,"%s%s",resultToSend,"3333#");
				add3333=1;
			}
		   // printf("the entire size is %d",row);

	            for(int ii=0; ii < numFields; ii++)
            {
			
			/*if(sizeof(resultToSend) > (size - 100)){size = seize + 1000;
				resultToSend = realloc(size);}*/
			//char * result=row[ii];	
		if(check == 0)	{
 			sprintf(resultToSend,"%s%s",resultToSend,row[ii]);
			check = 1;
			}
		else       {
			sprintf(resultToSend,"%s-%s",resultToSend,row[ii]);
			
			   }
	            }
		    sprintf(resultToSend,"%s#",resultToSend);
	            printf("\n");
	        }

   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);
	if(strcmp(resultToSend,"")==0){
		sprintf(resultToSend,"%s#","4444");
}
	freeMem(temp);
	printf("\nBefore returning new %s",resultToSend);
	return resultToSend;

}



char* listAllUsers(){
MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;
   char *resultToSend = malloc(1000*1000);
   sprintf(resultToSend,"");
   char *server = "localhost";
   char *user = "root";
   char *password = "root"; /* set me first */
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }


char *query="select id,type from login where type='customer' or type='agent'";


char *temp = malloc(10000);

strcpy(temp, query);

printf("query %s \n", temp);

   /* send SQL query */
   if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   res = mysql_use_result(conn);


   /* output table name */
   printf("MySQL Tables in mysql database:\n");
   /*while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);*/

	unsigned int    numFields;
	numFields = mysql_field_count(conn);
	int check = 0;
	//Just to add 3333# if there is any entry	
	int add3333=0;
	while(row = mysql_fetch_row(res)) // row pointer in the result set
	        {
			check=0;
			
			if(add3333==0){
				sprintf(resultToSend,"%s%s",resultToSend,"3333#");
				add3333=1;
			}
		   // printf("the entire size is %d",row);

	            for(int ii=0; ii < numFields; ii++)
            {
			
			/*if(sizeof(resultToSend) > (size - 100)){size = seize + 1000;
				resultToSend = realloc(size);}*/
			//char * result=row[ii];	
		if(check == 0)	{
 			sprintf(resultToSend,"%s%s",resultToSend,row[ii]);
			check = 1;
			}
		else       {
			sprintf(resultToSend,"%s-%s",resultToSend,row[ii]);
			
			   }
	            }
		    sprintf(resultToSend,"%s#",resultToSend);
	            printf("\n");
	        }

   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);
	if(strcmp(resultToSend,"")==0){
		sprintf(resultToSend,"%s#","4444");
}
	freeMem(temp);
	printf("\nBefore returning %s",resultToSend);
	return resultToSend;

}



void editFlightDetails(char *airline,char *flightid,char *source,char *destination,char *date){
MYSQL *conn;
   
   MYSQL_ROW row;
   
   char *server = "localhost";
   char *user = "root";
   char *password = "root"; /* set me first */
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }



char *query="UPDATE flightdetails set source=";

char *query1=",destination=";
char *query2=",date=";
char *query3=" where airline=";
char *query4=" and flightid=";

char *temp = malloc(10000);

strcpy(temp, query);
strcat(temp, source);

strcat(temp, query1);
strcat(temp, destination);

strcat(temp, query2);
strcat(temp, date);

strcat(temp, query3);
strcat(temp, airline);

strcat(temp, query4);
strcat(temp, flightid);


printf("query %s \n", temp);
	

	if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   	}

	
	/* close connection */
   
   	mysql_close(conn);
	freeMem(temp);
   
	return 0;

}

void insertFlightDetails(char *airline,char *flightid,char *source,char *destination,char *date){
MYSQL *conn;
   
   MYSQL_ROW row;
   
   char *server = "localhost";
   char *user = "root";
   char *password = "root"; /* set me first */
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }


char *query="INSERT INTO flightdetails (airline, flightid, source,destination,date) VALUES (";

char *temp = malloc(10000);

strcpy(temp, query);
sprintf(temp,"%s%s%s%s%s%s",temp,airline,flightid,source,destination,date);
strcat(temp, ");");



printf("query %s \n", temp);
	

	if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   	}

	
	/* close connection */
   	
   	mysql_close(conn);
	freeMem(temp);
   
	return 0;

}


void createAccount(char *id,char *pass,char *type){
MYSQL *conn;
  
   
   
   char *server = "localhost";
   char *user = "root";
   char *password = "root"; /* set me first */
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }


char *query="INSERT INTO login (id, password, type) VALUES (";

char *temp = malloc(1000);

strcpy(temp, query);
sprintf(temp,"%s%s%s%s",temp,id,pass,type);
strcat(temp, ");");



printf("query %s \n", temp);
	

	if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   	}
   	mysql_close(conn);
	freeMem(temp);
}

char* deleteBooking(int bookid, char *id){
MYSQL *conn;
   
   char *server = "localhost";
   char *user = "root";
   char *password = "root"; /* set me first */
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }


char *query="delete from bookingdetails where bookingid=";

char *query1=" and customerid=";
char *temp = malloc(10000);

strcpy(temp, query);

strcat(temp,bookid);

strcat(temp,query1);
strcat(temp,id);

printf("query %s \n", temp);

   /* send SQL query */
   if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

	
	/* close connection */
   
   	mysql_close(conn);
	freeMem(temp);
   
	return 0;
}


void insertBooking(char *customerid,char *flightid,char *source,char *destination,char *date,char *seats){
MYSQL *conn;
   
   char *server = "localhost";
   char *user = "root";
   char *password = "root"; /* set me first */
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

char *query="INSERT INTO bookingdetails (customerid, flightid, source,destination,date,seats) VALUES (";

char *temp = malloc(10000);

strcpy(temp, query);
sprintf(temp,"%s%s%s%s%s%s%s",temp,customerid,flightid,source,destination,date,seats);
strcat(temp, ");");



	printf("query %s \n", temp);
	
	if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   	}

	
	/* close connection */
   	mysql_close(conn);
	freeMem(temp);
	return 0;
}



char* bookingDetailsQuery(char *customerid){
   MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;
   char *resultToSend = malloc(1000*1000);
   sprintf(resultToSend,"");
   char *server = "localhost";
   char *user = "root";
   char *password = "root"; /* set me first */
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }


char *query="select * from bookingdetails where customerid=";

char *temp = malloc(10000);

strcpy(temp, query);
strcat(temp, customerid);

printf("query %s \n", temp);

   /* send SQL query */
   if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   res = mysql_use_result(conn);


   /* output table name */
   printf("MySQL Tables in mysql database:\n");
   /*while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);*/

	unsigned int    numFields;
	numFields = mysql_field_count(conn);
	int check = 0;
	//Just to add 3333# if there is any entry	
	int add3333=0;
	while(row = mysql_fetch_row(res)) // row pointer in the result set
	        {
			check=0;
			if(add3333==0){
				sprintf(resultToSend,"%s%s",resultToSend,"3333#");
				add3333=1;
			}
		   // printf("the entire size is %d",row);

	            for(int ii=0; ii < numFields; ii++)
            {
			
			/*if(sizeof(resultToSend) > (size - 100)){size = seize + 1000;
				resultToSend = realloc(size);}*/
			//char * result=row[ii];	
		if(check == 0)	{
 			sprintf(resultToSend,"%s%s",resultToSend,row[ii]);
			check = 1;
			}
		else       {
			sprintf(resultToSend,"%s-%s",resultToSend,row[ii]);
			
			   }
	            }
		    sprintf(resultToSend,"%s#",resultToSend);
	            printf("\n");
	        }

   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);
	freeMem(temp);
	if(strcmp(resultToSend,"")==0){
		sprintf(resultToSend,"%s#","4444");
	}
	return resultToSend;
}




char *mysqlFlightDetailsQuery(char *source, char *dest, char *date){
MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;
   char *resultToSend = malloc(1000*1000);
   sprintf(resultToSend,"");
   char *server = "localhost";
   char *user = "root";
   char *password = "root"; /* set me first */
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }


char *query="select * from flightdetails where source=";
char *query1=" and destination=";
char *query2=" and date=";

char *temp = malloc(10000);

strcpy(temp, query);
strcat(temp, source);

strcat(temp, query1);
strcat(temp, dest);

strcat(temp, query2);
strcat(temp, date);

printf("query %s \n", temp);

   /* send SQL query */
   if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   res = mysql_use_result(conn);


   /* output table name */
   printf("MySQL Tables in mysql database:\n");
   /*while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);*/

	unsigned int    numFields;
	numFields = mysql_field_count(conn);
	int check = 0;
	//Just to add 3333# if there is any entry	
	int add3333=0;
	while(row = mysql_fetch_row(res)) // row pointer in the result set
	        {
			check=0;
			if(add3333==0){
				sprintf(resultToSend,"%s%s",resultToSend,"3333#");
				add3333=1;
			}
		   // printf("the entire size is %d",row);

	            for(int ii=0; ii < numFields; ii++)
            {
		
		if(check == 0)	{
 			sprintf(resultToSend,"%s%s",resultToSend,row[ii]);
			check = 1;
			}
		else       {
			sprintf(resultToSend,"%s-%s",resultToSend,row[ii]);
			
			   }
	            }
		    sprintf(resultToSend,"%s#",resultToSend);
	            printf("\n");
	        }

   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);
	freeMem(temp);
	if(strcmp(resultToSend,"")==0){
		sprintf(resultToSend,"%s#","4444");
	}
	return resultToSend;
}


char* mysqlLoginQuery(char *id, char *pass){

   MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;
   char *resultToSend = malloc(1000);
   sprintf(resultToSend,"");
   char *server = "localhost";
   char *user = "root";
   char *password = "root";
   char *database = "airlinereservation";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }


char *query="select * from login where id=";
char *query1=" and password=";

char *temp = malloc(1000);
strcpy(temp, query);
strcat(temp, id);

strcat(temp, query1);
strcat(temp, pass);
printf("query %s", temp);

   /* send SQL query */
   if (mysql_query(conn, temp)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   res = mysql_use_result(conn);


   /* output table name */
   printf("MySQL Tables in mysql database:\n");

	unsigned int    numFields;
	numFields = mysql_field_count(conn);
	
	while(row = mysql_fetch_row(res)) // row pointer in the result set
	        {
	            for(int ii=0; ii < numFields; ii++)
            {
			sprintf(resultToSend,"%s",row[ii]);
			break;
	            }
		    sprintf(resultToSend,"%s",resultToSend);
	            printf("\n");
	        }

   mysql_free_result(res);
   mysql_close(conn);
	freeMem(temp);
	return resultToSend;
}

/************************************************************************************
*************************************************************************************
*****************************/
void *processClients(void* clientsock){

int n;
int client_sock = (int)clientsock;
char buf[1024];

char* responseCode;

//type of user
char *type="";
char *customerId="";
char *agentId="";

while(1)
{
bzero(buf,sizeof(buf));

if((n=recv(client_sock,buf,sizeof(buf),0)<=0)){
fflush(stdout);
perror("Socket closed by client what");
close(client_sock);
break;
}

sprintf(buf,"%s%s",buf,"-");
//printf("Buffed %s \n",buf);
//Delimiter with hyphen, extracting keywords
    int i;
    char *s = buf;
    int c = 0;
    char **arr = NULL ;

    c = split(s, '-', &arr);



//When the user is customer
if(strcmp(type, "customer")==0){

//Search for flight details
if(strcmp(arr[0], "1342") == 0){

char *source = malloc(100);
   sprintf(source,"");
sprintf(source,"%s%s%s","'",arr[1],"'");

char *destination = malloc(100);
   sprintf(destination,"");
sprintf(destination,"%s%s%s","'",arr[2],"'");

char *date = malloc(100);
   sprintf(date,"");
sprintf(date,"%s%s%s","'",arr[3],"'");

char * result=mysqlFlightDetailsQuery(source,destination,date);

printf("Result %s \n",result);

//Response as 3333-FlightId-Seats available
//char *result="3333-AI21-5";
send(client_sock,result,strlen(result),0);

freeMem(source);
freeMem(destination);
freeMem(result);
}
//New booking
else if(strcmp(arr[0], "1343") == 0){

char *customer=malloc(100);
sprintf(customer,"%s%s%s","'",customerId,"',");
char *flightId=malloc(100);
sprintf(flightId,"%s%s%s","'",arr[1],"',");
char *source=malloc(100);
sprintf(source,"%s%s%s","'",arr[2],"',");
char *destination=malloc(100);
sprintf(destination,"%s%s%s","'",arr[3],"',");
char *date=malloc(100);
sprintf(date,"%s%s%s","'",arr[4],"',");

char *seats=malloc(100);
sprintf(seats,"%s%s%s","'",arr[5],"'");
insertBooking(customer,flightId,source,destination,date,seats);
char * result="3333";
send(client_sock,result,strlen(result),0);
freeMem(customer);
freeMem(flightId);
freeMem(source);
freeMem(destination);
freeMem(date);
freeMem(seats);
}
//Booking details
else if(strcmp(arr[0], "1344") == 0){
char *customer = malloc(100);
   sprintf(customer,"");
sprintf(customer,"%s%s%s","'",customerId,"'");

char * result=bookingDetailsQuery(customer);
send(client_sock,result,strlen(result),0);
printf("Booking details result %s \n",result);
freeMem(customer);
freeMem(result);
}
//Delete booking by customer
else if(strcmp(arr[0], "1345") == 0){

char *bookingid = malloc(100);
   sprintf(bookingid,"");
sprintf(bookingid,"%s%s%s","'",arr[1],"'");

char *customer = malloc(100);
   sprintf(customer,"");
sprintf(customer,"%s%s%s","'",customerId,"'");

deleteBooking(bookingid,customer);
char * result="3333";
send(client_sock,result,strlen(result),0);
freeMem(customer);
freeMem(bookingid);
}

}//Main if ends

//When the user is agent/airline
else if(strcmp(type, "agent")==0){
//Add new flight
if(strcmp(arr[0], "5001") == 0){
char *agent=malloc(100);
sprintf(agent,"%s%s%s","'",agentId,"',");
char *flightId=malloc(100);
sprintf(flightId,"%s%s%s","'",arr[1],"',");
char *source=malloc(100);
sprintf(source,"%s%s%s","'",arr[2],"',");
char *destination=malloc(100);
sprintf(destination,"%s%s%s","'",arr[3],"',");
char *date=malloc(100);
sprintf(date,"%s%s%s","'",arr[4],"'");

insertFlightDetails(agent,flightId,source,destination,date);

char * result="3333";
send(client_sock,result,strlen(result),0);
freeMem(agent);
freeMem(flightId);
freeMem(source);
freeMem(destination);
freeMem(date);

}//Edit flight details
else if(strcmp(arr[0], "5002") == 0){

char *agent=malloc(100);
sprintf(agent,"%s%s%s","'",agentId,"'");
char *flightId=malloc(100);
sprintf(flightId,"%s%s%s","'",arr[1],"'");
char *source=malloc(100);
sprintf(source,"%s%s%s","'",arr[2],"'");
char *destination=malloc(100);
sprintf(destination,"%s%s%s","'",arr[3],"'");
char *date=malloc(100);
sprintf(date,"%s%s%s","'",arr[4],"'");

editFlightDetails(agent,flightId,source,destination,date);

char * result="3333";
send(client_sock,result,strlen(result),0);
freeMem(agent);
freeMem(flightId);
freeMem(source);
freeMem(destination);
freeMem(date);
}

}
//When the user is admin
else if(strcmp(type, "admin")==0){
//List All users
if(strcmp(arr[0], "7001") == 0){
char * result=listAllUsers();

printf("Result %s \n",result);

send(client_sock,result,strlen(result),0);
freeMem(result);
}
else if(strcmp(arr[0], "7003") == 0){

char *customer=malloc(100);
sprintf(customer,"%s%s%s","'",arr[1],"'");
char * result=bookingDetailsForSpecificCustomer(customer);

printf("Result %s \n",result);

send(client_sock,result,strlen(result),0);
freeMem(result);
freeMem(customer);

}else if(strcmp(arr[0], "7002") == 0){

char *id=malloc(100);
sprintf(id,"%s%s%s","'",arr[1],"'");
deleteUser(id);

char * result="3333";
send(client_sock,result,strlen(result),0);
freeMem(id);
} 

}



//Existing Users, For Login, Customer

if (strcmp(arr[0], "1234") == 0 || strcmp(arr[0], "1233") == 0 || strcmp(arr[0], "1235") == 0) {

char *user = malloc(200);
   sprintf(user,"");
sprintf(user,"%s%s%s","'",arr[1],"'");

char *pass = malloc(100);
   sprintf(pass,"");
sprintf(pass,"%s%s%s","'",arr[2],"'");


char * result=mysqlLoginQuery(user,pass);

time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s, sizeof(s), "%c", tm);

if(strcmp(result, "") == 0){

sprintf(user,"%s%s%s%s","\nUnsucessful logging attempt by: ",arr[1]," on ",s);
logging(user);

responseCode="2222";
send(client_sock,responseCode,strlen(responseCode),0);
}else 
{
sprintf(user,"%s%s%s%s","\nUser loggedin: ",arr[1]," on ", s);
logging(user);

responseCode="1111";

if (strcmp(arr[0], "1234") == 0){

//Setting type of user
type="customer";
//Setting customer name for further use in bookingdetails
customerId=result;

}
//For Login, Admin
else if (strcmp(arr[0], "1233") == 0){
type="admin";

}
//For Login, airline agent
else if (strcmp(arr[0], "1235") == 0){
type="agent";
agentId=result;
}

freeMem(user);
freeMem(pass);
fflush(stdout);

send(client_sock,responseCode,strlen(responseCode),0);

}//User exist else ends 1111

} //Main If ends



//For New Users
if (strcmp(arr[0], "1243") == 0 || strcmp(arr[0], "1244") == 0 || strcmp(arr[0], "1245") == 0) {

char *user = malloc(100);
sprintf(user,"%s%s%s","'",arr[1],"',");

char *pass = malloc(100);
sprintf(pass,"%s%s%s","'",arr[2],"',");

char *localtype = malloc(100);

if(strcmp(arr[0], "1244") == 0){
printf("1");
type="customer";
customerId=arr[1];
sprintf(localtype,"%s%s%s","'","customer","'");
}
//For Login, Admin
else if(strcmp(arr[0], "1243") == 0){
type="admin";
sprintf(localtype,"%s%s%s","'","admin","'");
}
//For Login, Airline provider
else if(strcmp(arr[0], "1245") == 0){
type="agent";
agentId=arr[1];
sprintf(localtype,"%s%s%s","'","agent","'");
}


createAccount(user,pass,localtype);

freeMem(user);
freeMem(pass);
freeMem(localtype);

responseCode="1111";
send(client_sock,responseCode,strlen(responseCode),0);
} //New user If ends here



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
