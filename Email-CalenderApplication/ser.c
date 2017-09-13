#include<stdio.h>
 #include<stdlib.h>
#include<netdb.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>
#include<string.h>
#include <sys/stat.h>
#define LENGTH 65536

int email_seq; 
char email_subject[50][LENGTH]; 
char email_body[50][LENGTH];
char email_username[50][LENGTH];
char email_firstname[50][LENGTH];
char email_lastname[50][LENGTH];
char email_time[50][LENGTH];
char email_read[50][LENGTH];
char email_delete[50][LENGTH];
int operation_email_seq;
int mail_seq_total;
int mail_seq_ND[50];
char current_user[20];
int total_user[30];
int totalEmailsLatest();
int totalCalReq();
char *th_user;

char * login(char buff[LENGTH]);
char * readline(FILE *fp);
int numoflines(FILE *fp);
int authenticateuser(FILE *fp, char user[20], char pwd[20], int n);
char* getfileName(FILE *fp, char user[1024],int n);
char * emailservice(char *nameoffile);
void mail_compose(char buff[2048]);
void compose(char sender[1024],char toSub[1024], char toBody[1024], char * emailfile, int n);
char * mail_inbox();
char * showInbox();
char * check_email(char buff[LENGTH]);
void email_forward(char buff[LENGTH]);
void email_reply(char buff[LENGTH]);
void delete_mail(char buff[LENGTH]);
void delete_mail_filter(char buff[LENGTH]);
char * filter_email(char buff[LENGTH]);
char * check_mail_filter(char buff[LENGTH]);
void filter_email_forward(char buff[LENGTH]);
void filter_email_reply(char buff[LENGTH]);
void unreadtoRead(int n);
char * checkuser(FILE *fp, char user[1024], int n);
void signup(char buff[LENGTH]);

void check_and_delete(char  *buf1, char *token2)
{

	char *token1,*token3;
	char calfile[50];
	char *line;
	int i=1,n;
	FILE *fp;

	char *token4=malloc(2048);
	strcpy(token4, buf1);

	token1 = strtok(token4,"|");
	

	printf("strings are %s %s\n", token1, token2);
	
	token3 = strtok(token1, ",");

	while(token3 != NULL)
	{

		strcpy(calfile, token3);
		strcat(calfile, "_calendar");

		fp = fopen(calfile, "r+");

		n = numoflines(fp);
		i=1;

		while (i<=n)
		{
		
			line = readline(fp);
			if (line == NULL)
				break;
			printf("line :%s %d\n", line,strlen(line));
			printf("line :%s %d\n", token2,strlen(token2));
			if(strcmp(line, token2) ==0)
			{
				fseek(fp, -2, SEEK_CUR);
				fputc('1', fp);
				fclose(fp);
				break;
			}
			i++;

		}
		
		token3 = strtok(NULL, ",");
	
	}
	
	return ;
	
}


	
	
void 
view_apt(char *send)
{
	FILE *fp;
	char calfile[50];
	int deleted = 0;
	char *line;
	int flag = 0;
	
	strcpy(calfile, current_user);
	strcat(calfile, "_calendar");

	fp = fopen(calfile, "r");
	
	if (fp == NULL)
	{
		strcat(send,"NULL");
		return;
	}

	while(1)
	{
		line = readline(fp);
		
		if(line == NULL)
		{
			if (flag == 0)
				strcat(send,"NULL");
			break;
		}
		line[strlen(line) - 1] = '%';

		strcat(send,line);
		flag = 1;
	}
	
	return;

}
	
char *
readline(FILE *fp)
{
	int i =0;
	char *buf = (char *)malloc(2048);
	while ((buf[i] = fgetc(fp)) != EOF)
	{
		if(buf[i] == '\n')
			break;
		
		i++;
	}

	if ( i == 0)
		return NULL;


	return buf;
}
//compose email
void compose_cal(char *sender,char*reciever, char *sub,char *body, char *place,char *start_time,char *end_time, char *date , char *t1)
{
	FILE *fp;
	char calfile[50];
	int deleted = 0;
	
	strcpy(calfile, reciever);
	strcat(calfile, "_calendar");

	fp = fopen(calfile, "a");
	fprintf(fp, "%s|%s|%s|%s|%s|%s|%s|%s|%d\n",start_time, end_time, date,sender,t1,sub,body,place,deleted);
	fclose(fp);
}

char * 
compose_cal_try(char *sender,char*reciever, char *sub,char *body, char *place,char *start_time,char *end_time, char *date )
{

	FILE *fp;
	char *token,*s1,*s2,*s3;
	int n,i;
	char calfile[50];
	int h1,m1,h2,m2,h3,m3,h4,m4;

	strcpy(calfile, reciever);
	strcat(calfile, "_calendar");

	fp = fopen(calfile, "r");

	if ( fp == NULL)
		return NULL;
	
	n = numoflines(fp);

	for (i=0;i<n;i++)
	{
		token = readline(fp);
		
		if (token == NULL)
			continue;

		s1 = strtok(token,"|");
		s2 = strtok(NULL, "|");
		s3 = strtok(NULL, "|");
		if ((s1 == NULL) || (s2 == NULL) || (s3 == NULL))
			continue;

		printf("%s %s\n", s2, s3);

		if(strcmp(date, s3) == 0)
		{
			sscanf(s1,"%d:%d", &h1, &m1);
			sscanf(s2,"%d:%d", &h2, &m2);
			sscanf(start_time,"%d:%d", &h3, &m3);
			sscanf(end_time,"%d:%d", &h4, &m4);
			
			h1 = h1*60 + m1;
			h2 = h2*60 + m2;
			h3 = h3*60 + m3;
			h4 = h4*60 + m4;

			printf(" %d %d %d %d\n", h1 , h2, h3, h4);

			if (((h1 <= h3) && (h2 >=h3)) || ((h1>=h3) && (h1<=h4)))
			{
				break;
			}

		}
	}

	if(i == n)
		return NULL;

	else
		return reciever;
}	
		
char * delete_apt(char buff[2048])
{
	char *token;
	int len,n,seq;
	char calfile[50];
	char *line;
	int i=1;
	FILE *fp;
	
	token = strtok(buff, "|");
	token = strtok(NULL,"|");

	seq = atoi(token);
	strcpy(calfile, current_user);
	strcat(calfile, "_calendar");

	fp = fopen(calfile, "r+");

	n = numoflines(fp);
	
	printf("number is %d %d\n", n,seq);

	if (seq <= n)
	{
		while (i<=seq)
		{
			line = readline(fp);
			i++;
		}

		fseek(fp, -2, SEEK_CUR);
		fputc('1', fp);
	
	}
	
	else
	{
	 fclose(fp);
	 return "FAIL";
	}

	 fclose(fp);

	return NULL;
	
}

char *
parse_recv(char *line)
{
	char *token;
	
	token = strtok(token,"|");
	token = strtok(NULL,"|");	
	token = strtok(NULL,"|");	
	token = strtok(NULL,"|");	
	token = strtok(NULL,"|");

	return token;
}
	
		
char * modify_apt1(char buff[2048],char *buf2)
{
	char *token;
	int len,n,seq;
	char calfile[50];
	char *line;
	int i=1;
	FILE *fp;
	
	token = strtok(buff, "|");
	token = strtok(NULL,"|");

	seq = atoi(token);
	strcpy(calfile, current_user);
	strcat(calfile, "_calendar");

	fp = fopen(calfile, "r+");

	n = numoflines(fp);
	
	printf("number is %d %d\n", n,seq);

	if (seq <= n)
	{
		while (i<=seq)
		{
			line = readline(fp);
			i++;
		}
		strcpy(buf2,line);
		token =parse_recv(line);
	}
	
	else
	{
	 fclose(fp);
	 return NULL;
	}

	 fclose(fp);

	return token;
	
}
char * create_apt(char buff[2048])
{
	int len;
	char buf[2048];
	char * token, *temp, sender[20], reciever[20][20], *result = NULL;
	char event_group[200];
	int count = 0 ,i,lastCount;
	char sub[100], body[200], place[50], date[20], start_time[20],end_time[20];
	strcpy(buf, buff);	
	token = strtok(buff, "|");
	printf("%s\n", token);
	token = strtok(NULL,"|");
	printf("%s\n", token);
	strcpy(event_group, token);
	token = strtok(token, ",");

	while(token != NULL)
	{
		strncpy(reciever[count++], token, strlen(token)+1);
		token = strtok(NULL,",");	
	}

	lastCount = count-1;
	strncpy(sender, reciever[0], strlen(reciever[0])+1);
	token = strtok(buf,"|");
	token= strtok(NULL,"|");
	token= strtok(NULL,"|");
	strncpy(date, token, strlen(token)+1);
	token= strtok(NULL,"|");
	strncpy(start_time, token, strlen(token)+1);
	token= strtok(NULL,"|");
	strncpy(end_time, token, strlen(token)+1);
	token= strtok(NULL,"|");
	strncpy(place, token, strlen(token)+1);
	token= strtok(NULL, "|");	
	strncpy(sub, token, strlen(token)+1);
	token= strtok(NULL, "|");	
	strncpy(body, token, strlen(token)+1);
	printf("\n Sender is %s",sender );
	printf("\nSubject is %s", sub);
	printf("\nBody is %s", body);
	for(i=0;i<count ;i++)
	{
		printf("\nReciever is %s", reciever[i]);
		result = compose_cal_try(sender,reciever[i],sub,body,place,start_time,end_time,date);
		
		if( result != NULL)
		{
			break;
		}
	}
	
	if (result == NULL)
	{
		for (i=0 ; i < count ; i++)
		{	
			compose_cal(sender,reciever[i],sub,body,place,start_time,end_time,date, event_group);
		}
	}

	return result;
	
}

int main(int argc, char *argv[])
{
	int sockfd,newsockfd,clilen,random , flag=1;
	char sendBuff[LENGTH], recvBuff[LENGTH], * token,tempBuff[LENGTH] , *auth_login, *inbox_detail , *check_email_inbox, *filter_result;
	char * check_email_filter, *valid;
	struct sockaddr_in serv_addr, cli_addr;
	int n,count=0; 
	char str[1024];
	FILE *fpcfg;
        FILE *fp;
	int lines;
	int num, i , j , temp[50],check,start;
	time_t tloc;
	int pid;
	char *buf,*buf3;
	int new_e,new_f;
	char newemails[5], newCal[5];
	struct stat st = {0};
  	if(argc < 2)
	{
	  fprintf(stderr,"usage %s  port\n", argv[0]);
	  exit(0);
	}
		/* creating a socket */
	sockfd= socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0 )
	{
		perror("SERVER: ERROR OPENING SOCKET");
		exit(1);
	}
  
  		/* socket structure initialization */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port= htons(atoi(argv[1]));
  
		/* bind the host address */
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	{
		perror("SERVER: ERROR IN BINDING");
		exit(1);   
	}
		/* WAIT FOR AN INCOMING CONNECTION */
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	for(;;)
	{
		/* ACCEPT THE CONNECTION */
		newsockfd= accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		printf("%d" ,newsockfd);
		if(newsockfd < 0)
		{
			perror("SERVER: ERROR IN ACCEPT");
			exit(1);
		}
		pid = fork();
		if(pid <0)
		{
			perror("SERVER: ERROR ON FORK");
			exit(1);
		}
		//forking the process
		if(pid == 0)
		{
			setbuf(stdout,NULL);
			
			//child process will work
			//closing sockfd
			close(sockfd);
				

				if (stat("email", &st) == -1) {
   					 mkdir("email", 0777);
				}
			printf("before here");
			while(1)
			{
				printf("\nhere");
				memset(recvBuff, '\0', sizeof(recvBuff));
				n = read(newsockfd, recvBuff , sizeof(recvBuff));
				strcpy(tempBuff, recvBuff);
				if(n<0)
				{
					perror("READ ERROR");
				}
				printf("\nrecvBuff = %s ", recvBuff);
				token = strtok(tempBuff,"|");
				printf("token:%s\n", token);
				
				if(strcmp(token, "thread") == 0)
				{
					th_user = strtok(NULL, "|");
					printf("th_user = %s\n", th_user);
				}
				// option selection at starting
				if(recvBuff[0]=='a')
				{
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff,"login");	
				}
				if(recvBuff[0]=='b')
				{
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff,"signup");	
				}
				// login 
				if(strcmp(token, "login")==0)
				{
					printf("\nverification of login");
					printf("\nrecvBuff = %s ", recvBuff);
					auth_login = login(recvBuff);
					if(strcmp(auth_login,"true")==0)
					{
						memset(sendBuff, '\0', sizeof(sendBuff));
						strcpy(sendBuff, "true");	
					}
					else if(strcmp(auth_login,"false")==0)
					{
						memset(sendBuff, '\0', sizeof(sendBuff));
						strcpy(sendBuff, "false");
					}
				}
				//signup validate
				if(strcmp(token, "signup")==0)
				{
					printf("\nverification of login");
					printf("\nrecvBuff = %s ", recvBuff);
					lines = numoflines(fp);
					valid = checkuser(fp,recvBuff,lines );
					printf("---> %s",valid);
					if(strcmp(valid,"exist")==0)
					{
						memset(sendBuff, '\0', sizeof(sendBuff));
						strcpy(sendBuff, "exist");	
					}
					else if(strcmp(valid,"dont")==0)
					{
						memset(sendBuff, '\0', sizeof(sendBuff));
						strcpy(sendBuff, "dont");
					}
				}
				if(strcmp(token, "dosignup")==0)
				{
					printf("signing up");
					printf("\nrecvBuff = %s ", recvBuff);
					signup(recvBuff);
	
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "logout");
				}
				
				
				//email
				if(strcmp(recvBuff,"emailServ")==0)
				{
					printf("\nEmail Service");
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "emailServ");
				}

				//option of email
				//inbox
				if(strcmp(recvBuff,"inbox")==0)
				{
					printf("\nEmail inbox");
					inbox_detail = mail_inbox();
					printf("\n in  a main fun");
					printf("\n %s", inbox_detail);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, inbox_detail);
				}
				//compose
				if(strcmp(token,"compose")==0)
				{
					printf("\n ----> Compose Service %s", recvBuff);
					mail_compose(recvBuff);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "emailServ");
				}
				
				//create_apt
				if(strcmp(token,"create_apt") == 0)
				{
					printf("\n CREATE appointment \n");
					buf = create_apt(recvBuff);
					printf("buffer is %s\n", buf);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "ret_creat_apt");
					strcat(sendBuff, "|");
					if(buf != NULL)
					strcat(sendBuff,  buf);
					else
					strcat(sendBuff, "NULL");
					strcat(sendBuff,  "#");
					
				}
					
				
				//create_apt
				if(strcmp(token,"modify_apt2") == 0)
				{
					printf("\n MODIFY appointment2 \n");
					buf = create_apt(recvBuff);
					printf("buffer is %s\n", buf);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "ret_modify_apt2");
					strcat(sendBuff, "|");
					if(buf != NULL)
					strcat(sendBuff,  buf);
					else
					strcat(sendBuff, "NULL");
					strcat(sendBuff,  "#");
					
				}
					
				
				//delet_apt
				if(strcmp(token,"delete_apt") == 0)
				
				//delet_apt
				if(strcmp(token,"delete_apt") == 0)
				{
					printf("\n DELETE appointment \n");
					buf = delete_apt(recvBuff);
					printf("buffer is %s\n", buf);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "ret_delete_apt");
					strcat(sendBuff, "|");
					if(buf != NULL)
					strcat(sendBuff, "FAIL" );
					else
					strcat(sendBuff, "NULL");
					strcat(sendBuff,  "#");
					
				}
					
				
					
				
				//delet_apt
				if(strcmp(token,"modify_apt1") == 0)
				{
					printf("\n MODIFY appointment \n");
					buf3 = malloc(2048);
					buf = modify_apt1(recvBuff,buf3);
					
					check_and_delete(buf,buf3);
					printf("buffer is %s\n", buf);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "ret_modify_apt1");
					strcat(sendBuff, "|");
					if(buf != NULL)
					strcat(sendBuff, buf );
					else
					strcat(sendBuff, "NULL");
					strcat(sendBuff,  "#");
					
				}
				//view_apt
				if(strcmp(token,"view_apt") == 0)
				{
					printf("\n view appointments \n");
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "sview_apt");
					strcat(sendBuff, "|");

					view_apt(sendBuff);
					strcat(sendBuff,  "#");
					
				}
					
				// option of inbox
				//check email
				if(strcmp(token,"check_email")==0)
				{
					printf("\n-----> check email");
					check_email_inbox = check_email(recvBuff);
					printf("\n\ncheck email_detail = %s" , check_email_inbox);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, check_email_inbox);
				}
				//reply email
				if(strcmp(token,"email_reply")==0)
				{
					printf("\n-----> reply");
					email_reply(recvBuff);
					inbox_detail = mail_inbox();
					printf("\n in  a main fun");
					printf("\n %s", inbox_detail);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, inbox_detail);
				}
				//forward email
				if(strcmp(token,"email_forward")==0)
				{
					printf("\n-----> forward");
					email_forward(recvBuff);
					inbox_detail = mail_inbox();
					printf("\n in  a main fun");
					printf("\n %s", inbox_detail);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, inbox_detail);
				}
				
				//delete email

				if(strcmp(token,"delete_email")==0)
				{
					printf("\n delete email");
					delete_mail(recvBuff);
					inbox_detail = mail_inbox();
					printf("\n in  a main fun");
					printf("\n %s", inbox_detail);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, inbox_detail);	
				}
				//filter email

				if(strcmp(token,"filter_email")==0)
				{
					printf("\nfilter email");
					filter_result = filter_email(recvBuff);
					printf("filter result = %s . ", filter_result );
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, filter_result);
				}

				//go back email
				if(strcmp(recvBuff,"goback_email")==0)
				{
					printf("\n goback email");
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "emailServ");
				}

				//go back email
				if(strcmp(recvBuff,"goback_cal")==0)
				{
					printf("\n goback calendar");
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "calServ");
				}

				if(strcmp(token,"mail_filter_check")==0)
				{
					printf("\n----->Filter check email");
					check_email_filter = check_mail_filter(recvBuff);
					printf("\n\nFilter check email_detail = %s" , check_email_filter);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, check_email_filter);
					system("clear");
				}
				if(strcmp(token,"mail_filter_delete")==0)
				{
					printf("\n delete email");
					delete_mail_filter(recvBuff);
					inbox_detail = mail_inbox();
					printf("\n in  a main fun");
					printf("\n %s", inbox_detail);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, inbox_detail);	
				}
				//go back to mail,cal option
				if(strcmp(recvBuff,"goback")==0)
				{

					
					
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "true");
					
					
				}
				//calander
				//logout
				if(strcmp(token,"thread")==0)
				{
					new_e = totalEmailsLatest();
				 	new_f = totalCalReq();
					printf(" email count:%d\n " ,new_e);
					printf(" cal count:%d\n", new_f);
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "new_eF");
					sprintf(newemails, "%d",new_e);
					sprintf(newCal, "%d",new_f);
					strcat(sendBuff,"|");
					strcat(sendBuff,newemails);
					strcat(sendBuff,"|");
					strcat(sendBuff,newCal);
					printf("\nthread = %s", sendBuff);

				}

				if(strcmp(recvBuff,"calServ")==0)
				{
					printf("\n Cal Service");
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "calServ");
				}
				if(strcmp(recvBuff,"logout")==0)
				{
					if(strcmp(current_user, "admin123") == 0)
					{
						memset(sendBuff, '\0', sizeof(sendBuff));
						strcpy(sendBuff, "killing");	

						
					}
					else{
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "logout");}
				}
				if(strcmp(sendBuff, "killing") == 0)
				{
					n = write(newsockfd,sendBuff, strlen(sendBuff)+1);
					printf("\nsendBuff = %s ", sendBuff);
					//close(newsockfd);
					//exit(0);
					
					
					
				}
				else
				{
						n = write(newsockfd,sendBuff, strlen(sendBuff)+1);
						printf("\nsendBuff = %s ", sendBuff);
						if(n<0)
						{
							perror("WRITE ERROR");
						}
				}
			}
			exit(0);
		}
		else 
		{
			close(newsockfd);
			
		}
      
	}
  
}

int numoflines(FILE *fp)
{
	int n=0,ch_count=0;
	fp = fopen("configuration.txt", "r");
	while ((ch_count= fgetc(fp)) != EOF)
	{
		if (ch_count == '\n')
		n++;
	}
	fclose(fp);
	return n;

}
//check authentication 
int authenticateuser(FILE *fp, char user[20], char pwd[20], int n)
{
	char line_file[1024];
	int i,flag;
	int checkuser = 1;
	char * token;
	fp = fopen("configuration.txt", "r");
	for(i=0 ; i <= n ; i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
			//printf("line :%s" , line_file);
		
			token = strtok(line_file,"#");
			if(i<n)
			{
				if(checkuser==1)
				{
					if(strcmp(token,user)==0)
					{
				
						checkuser=0;
						flag=1;
					}
				}
			}
			if((i==n) && (checkuser==1))
			{
				
				flag=0;
				break;
			}
			if(flag==1)
			{
				token = strtok(NULL,"#");
				if(strcmp(token,pwd)==0)
				{
					flag=1;
					break;
				}
				else
				{
					
					flag=0;
					break;
				}
			
			}
		}
			
	}
	printf("\n flag= %d " ,flag);
	fclose(fp);
	//flag=0 means not authenticated
	return flag;	
}
char * checkuser(FILE *fp, char users[1024], int n)
{
	char * validate_user = malloc(20);
	char line_file[1024];
	int i,flag;
	char * token;
	char user[20];
	printf("IN A CHECK USER FIDGFD %s", users);
	token = strtok(users,"|");
	token = strtok(NULL,"|");
	strcpy(user,token);
	printf("\n-->%s user", user);
	fp = fopen("configuration.txt", "r");
	for(i=0 ; i < n ; i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
			//printf("line :%s" , line_file);
		}
		token = strtok(line_file,"#");
		if(strcmp(token,user)==0)
		{
			flag=1;
			break;
		}
		else
		{
			flag=0;
		}
	}
	//printf("\n flag= %d " ,flag);
	fclose(fp);
	//flag=1 means repeated user
	if(flag==1)
	{
		return "exist";
	}
	else
	{
		return "dont";
	}
	
}
void signup(char buff[LENGTH])
{
	char username[50], password[50], fname[50], lname[50], filenamesign[50], calFile[50];
	char *token;
	char signin[LENGTH];
	FILE *fpcfg;
	char * emailFile;
	token = strtok(buff,"|");
	token = strtok(NULL,"|");
	strcpy(username, token);
	token = strtok(NULL,"|");
	strcpy(password, token);
	token = strtok(NULL,"|");
	strcpy(fname, token);
	token = strtok(NULL,"|");
	strcpy(lname, token);
	strcpy(filenamesign, username);
	strcat(filenamesign,".txt");
	fpcfg = fopen("configuration.txt", "a");
	fprintf(fpcfg,"%s#%s#%s#%s#%s\n",username,password,fname,lname,filenamesign);
	fclose(fpcfg);
	emailFile = emailservice(filenamesign);
	fpcfg = fopen(emailFile, "a");
	fclose(fpcfg);
	strcpy(calFile, username);
	strcpy(calFile, "_calendar");
	fpcfg = fopen(calFile, "a");
	fclose(fpcfg);


	
}
char * login(char buff[LENGTH])
{
	char * token, username[20], password[20];
	char * auth_login = malloc(20);
	FILE * fp;
	int lines, check;
	memset(username,0,20);
	memset(password,0,20);
	memset(current_user,0,20);
	//printf("\n login detail is %s", buff);
	token = strtok(buff,"|");
	token = strtok(NULL, "|");
	strcpy(username, token);
	token = strtok(NULL, "|");
	strcpy(password,token);
	printf("username = %s, password = %s", username,password);
	lines = numoflines(fp);
	strcpy(current_user,username);
	check = authenticateuser(fp, username, password,lines);
	if(check == 1 )
	{
		return "true";
	}
	if(check == 0)
	{
		return "false";
	}
}

//get a file name or reciever
char* getfileName(FILE *fp, char user[1024],int n)
{
	int i;
	char *token;
	char line_file[1024];
	fp = fopen("configuration.txt", "r");
	for(i=0 ; i < n ; i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
			//printf("line :%s" , line_file);
		}
		token = strtok(line_file,"#");
		if(strcmp(token,user)==0)
		{
			token= strtok(NULL, "#");
			token= strtok(NULL, "#");
			token= strtok(NULL, "#");
			token= strtok(NULL, "#");
			break;
		}
	}
	fclose(fp);
	return token;
}
//open a file in a email subfolder
char * emailservice(char *nameoffile)
{

	char * emailfile = malloc(1024);

	strcpy(emailfile,"email/");
	strcat(emailfile, nameoffile);
	printf("=======>%s", emailfile);
	return emailfile;
}
void mail_compose(char buff[2048])
{
	int len;
	char * token, senderOfEmail[20], recieverOfEmail[20][20], email_Sub[500], email_Body[500], *nameOfFile, *email_File;
	int count = 0 ,i,lastCount,lines;
	FILE *fpcfg;
	
	token = strtok(buff, "|");
	token = strtok(NULL,",");
	while(token != NULL)
	{
		strncpy(recieverOfEmail[count++], token, strlen(token)+1);
		token = strtok(NULL,",");	
	}
	lastCount = count-1;
	strncpy(senderOfEmail, recieverOfEmail[0], strlen(recieverOfEmail[0])+1);
	token = strtok(recieverOfEmail[lastCount],"|");
	strncpy(recieverOfEmail[count-1], token, strlen(token)+1);
	token= strtok(NULL,"|");
	strncpy(email_Sub, token, strlen(token)+1);
	token= strtok(NULL, "|");	
	strncpy(email_Body, token, strlen(token)+1);
	printf("\n Sender is %s",senderOfEmail );
	printf("\nSubject is %s", email_Sub);
	printf("\nBody is %s", email_Body);
	for(i=1;i<count ;i++)
	{
		printf("\nReciever is %s", recieverOfEmail[i]);
		lines = numoflines(fpcfg);
		nameOfFile = getfileName(fpcfg, recieverOfEmail[i], lines);
		printf("\nFile name is %s", nameOfFile);
		emailservice(nameOfFile);	
		email_File = emailservice(nameOfFile);	
		printf("%s", email_File);
		compose(senderOfEmail,email_Sub,email_Body,email_File,lines);
	}
	
}
//compose email
void compose(char sender[1024],char toSub[1024], char toBody[1024],char * emailfile, int n)
{
	FILE *fp;
	char firstname[1024], lastname[1024], subject[1024], body[1024], time_send[1024];
	char *token;
	int i;
	char line_file[1024];
	struct tm *local;
	time_t t;
	token  = strtok(emailfile , ".txt");
	strncpy(emailfile,token, strlen(token)+1);
	strcat(emailfile,".txt");
	t = time(NULL);
	//local = localtime(&t);
	//printf("Local time and date: %s\n", asctime(local));
	//local = gmtime(&t);
	//printf("UTC time and date: %s\n", asctime(local));
	strncpy(time_send,asctime(localtime(&t)), strlen(asctime(localtime(&t)))-1);
	fp = fopen("configuration.txt", "r");
	for(i=0 ; i < n ; i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
			//printf("line :%s" , line_file);
		}
		token = strtok(line_file,"#");
		if(strcmp(token,sender)==0)
		{
			//password
			token= strtok(NULL, "#");
			//first name
			token= strtok(NULL, "#");
			strcpy(firstname,token);
			//lastname
			token= strtok(NULL, "#");
			strcpy(lastname,token);
			break;
		}
	} 
	fclose(fp);
	fp = fopen(emailfile, "a");
	fprintf(fp, "||(UNREAD)||ND||%s||%s||%s||%s||%s||%s\n", sender,firstname,lastname,toSub,toBody,time_send);
	fclose(fp);
}
int numoflinesInaFile(FILE *fp, char emailFiles[1024])
{
	int n=0,ch_count=0, len;
	char *token;
	token  = strtok(emailFiles , ".txt");
	strncpy(emailFiles,token, strlen(token)+1);
	strcat(emailFiles,".txt");
	//printf("----> %s hii", emailFiles);
	fp = fopen(emailFiles,"r");
	if(fp== NULL)
	{
		perror("FILE OPENING");
	}
	while ((ch_count= fgetc(fp)) != EOF)
	{
		if (ch_count == '\n')
		n++;
	}
	fclose(fp);
	return n;

}
void emailfileToDelete(FILE *fp, char emailFile[1024], int n )
{
	char *token;
	char line_file[1024];
	int i,j=0;
	token  = strtok(emailFile , ".txt");
	strncpy(emailFile,token, strlen(token)+1);
	strcat(emailFile,".txt");
	fp = fopen(emailFile, "r");
	for(i=0;i<n;i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
				token = strtok(line_file, "||");
				token = strtok(NULL, "||");	
				printf("----->%s", token);
				strncpy(email_delete[j++], token,strlen(token)+1);
				mail_seq_total++;
				

		}	
	}
	fclose(fp);
	//printf("%s", email_username[0]);
}

void emailfileToRead(FILE *fp, char emailFile[1024], int n )
{
	char *token;
	char line_file[1024];
	int i,j=0,k=0, count=0;
	token  = strtok(emailFile , ".txt");
	strncpy(emailFile,token, strlen(token)+1);
	strcat(emailFile,".txt");
	fp = fopen(emailFile, "r");
	for(i=0;i<n;i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
				
				//if(strcmp(email_delete[k++], "ND")==0)
				//{
				token = strtok(line_file, "||");

				//token = strtok(NULL, "||");	
				//printf("\n token = %s", token);
				strncpy(email_read[j++], token,strlen(token)+1);
				//email_seq++;
				
				//}
				//mail_seq_total++;

		}	
	}
	fclose(fp);
	//printf("%s", email_username[0]);
}

void emailfileToUser(FILE *fp, char emailFile[1024], int n )
{
	char *token;
	char line_file[1024];
	int i,j=0,k=0;
	token  = strtok(emailFile , ".txt");
	strncpy(emailFile,token, strlen(token)+1);
	strcat(emailFile,".txt");
	fp = fopen(emailFile, "r");
	for(i=0;i<n;i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
				//if(strcmp(email_delete[k++], "ND")==0)
				//{
				token = strtok(line_file, "||");
				token = strtok(NULL,"||");
				token = strtok(NULL,"||");
				//token = strtok(NULL, "||");	
				//printf("\n token = %s", token);
				strncpy(email_username[j++], token,strlen(token)+1);
				//email_seq++;
				//}

		}	
	}
	fclose(fp);
	//printf("%s", email_username[0]);
}

void emailfileToFname(FILE *fp, char emailFile[1024], int n )
{
	char *token;
	char line_file[1024];
	int i,j=0,k=0;
	token  = strtok(emailFile , ".txt");
	strncpy(emailFile,token, strlen(token)+1);
	strcat(emailFile,".txt");
	fp = fopen(emailFile, "r");
	for(i=0;i<n;i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
				//if(strcmp(email_delete[k++], "ND")==0)
				//{

				token = strtok(line_file, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL,"||");
				token = strtok(NULL,"||");
				//token = strtok(NULL, "||");	
				//printf("\n token = %s", token);
				strncpy(email_firstname[j++], token,strlen(token)+1);
				//}
		}	
	}
	fclose(fp);

}
void emailfileToLname(FILE *fp, char emailFile[1024], int n )
{
	char *token;
	char line_file[1024];
	int i,j=0,k=0;
	token  = strtok(emailFile , ".txt");
	strncpy(emailFile,token, strlen(token)+1);
	strcat(emailFile,".txt");
	fp = fopen(emailFile, "r");
	for(i=0;i<n;i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
				//if(strcmp(email_delete[k++], "ND")==0)
				//{

				token = strtok(line_file, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL,"||");
				token = strtok(NULL,"||");
				//token = strtok(NULL, "||");	
				//printf("\n token = %s", token);
				strncpy(email_lastname[j++], token,strlen(token)+1);
				//}
		}	
	}
	fclose(fp);

}
void emailfileToSub(FILE *fp, char emailFile[1024], int n )
{
	char *token;
	char line_file[1024];
	int i,j=0,k=0;
	token  = strtok(emailFile , ".txt");
	strncpy(emailFile,token, strlen(token)+1);
	strcat(emailFile,".txt");
	fp = fopen(emailFile, "r");
	for(i=0;i<n;i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
				//if(strcmp(email_delete[k++], "ND")==0)
				//{

				token = strtok(line_file, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL,"||");
				token = strtok(NULL,"||");
				//token = strtok(NULL, "||");	
				//printf("\n token = %s", token);
				strncpy(email_subject[j++], token,strlen(token)+1);
				//}
			
		}	
	}
	fclose(fp);

}
void emailfileToBody(FILE *fp, char emailFile[1024], int n )
{
	char *token;
	char line_file[1024];
	int i,j=0,k=0;
	token  = strtok(emailFile , ".txt");
	strncpy(emailFile,token, strlen(token)+1);
	strcat(emailFile,".txt");
	fp = fopen(emailFile, "r");
	for(i=0;i<n;i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
				//if(strcmp(email_delete[k++], "ND")==0)
				//{

				token = strtok(line_file, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL,"||");
				token = strtok(NULL,"||");
				//token = strtok(NULL, "||");	
				//printf("\n token = %s", token);
				strncpy(email_body[j++], token,strlen(token)+1);
				//}
			
		}	
	}
	fclose(fp);

}

void emailfileToTime(FILE *fp, char emailFile[1024], int n )
{
	char *token;
	char line_file[1024];
	int i,j=0,k=0;
	token  = strtok(emailFile , ".txt");
	strncpy(emailFile,token, strlen(token)+1);
	strcat(emailFile,".txt");
	fp = fopen(emailFile, "r");
	for(i=0;i<n;i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
				//if(strcmp(email_delete[k++], "ND")==0)
				//{
		
				token = strtok(line_file, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL, "||");
				token = strtok(NULL,"||");
				token = strtok(NULL,"||");
				//token = strtok(NULL, "||");	
				//printf("\n token = %s", token);
				strncpy(email_time[j++], token,strlen(token)+1);
				//}
			
		}	
	}
	fclose(fp);

}
char * mail_inbox()
{
	char * inbox_detail = malloc(LENGTH);
	int lines,email_option,len, check_email_option,mail_del;
	FILE *fpcfg;
	char *nameOfFile, *email_File, mail_del_seq[3];
	lines = numoflines(fpcfg);
	nameOfFile = getfileName(fpcfg, current_user, lines);
	printf("File name is %s\n", nameOfFile);
	emailservice(nameOfFile);	
	email_File= emailservice(nameOfFile);	
	printf("%s", email_File);
	lines = numoflinesInaFile(fpcfg, email_File);
	printf("%d", lines);
	email_seq = 0;
	mail_seq_total = 0;
	emailfileToDelete(fpcfg,email_File,lines);
	emailfileToRead(fpcfg,email_File,lines);
	emailfileToUser(fpcfg,email_File,lines);
	emailfileToFname(fpcfg,email_File,lines);
	emailfileToSub(fpcfg,email_File,lines);
	emailfileToLname(fpcfg,email_File,lines);
	emailfileToBody(fpcfg,email_File,lines);
	emailfileToTime(fpcfg,email_File,lines);
	inbox_detail = showInbox();
	if(inbox_detail[strlen(inbox_detail)-1] == '\n')
	{
		inbox_detail[strlen(inbox_detail)-1] = '\0';
	}
	return inbox_detail;
	/*if(email_option == 1)
	{
		printf("\n Check Email");
		check_email_option = check_email();
		if(check_email_option == 1)
		{
			printf("reply Email");
			email_reply(operation_email_seq,current_user);
		}
		else if(check_email_option == 2)
		{
			printf("Forware Email");
			email_forward(operation_email_seq,current_user);
		}
		else if(check_email_option == 3)
		{
			printf("DELETE EMAIL");
			delete_mail(operation_email_seq, current_user);
			
		}
		
	}
	else if(email_option == 2)
	{
		printf("\n Delete Email");
		printf("\nEnter a sequence number Which you want to delete:");
		clean_stdin();
		fgets(mail_del_seq,sizeof(mail_del_seq),stdin);
		mail_del = atoi(mail_del_seq);
		delete_mail(mail_del, current_user);
		
	}
	else if(email_option == 3)
	{
		printf("\n Filter Email");
		email_filter();
	}*/

}

char * showInbox()
{
	int i,n,k=0;
	char * inbox_detail = malloc(LENGTH);
	char seq[2], inbox_buff[LENGTH],time_buff[100];
	memset(inbox_buff,'\0',LENGTH);
	strcpy(inbox_buff, "inboxxx|");
	printf("\n********Welcome user*******\n");
	printf("\n  Status   Username  firstname  lastname\tsubject                      time\n");
	printf("-------------------------------------------------------------------------------------------------\n");
	for(i=0;i<mail_seq_total;i++)
	{
		if(strcmp(email_delete[i], "ND")==0)
		{	
		//printf("**************************************************************\n");
		printf("%d)%s",++k, email_read[i]);
		strcat(inbox_buff, email_read[i]);
		strcat(inbox_buff,"|");
		printf("%5s", email_username[i]);
		strcat(inbox_buff, email_username[i]);
		strcat(inbox_buff,"|");
		printf("%12s", email_firstname[i]);
		strcat(inbox_buff, email_firstname[i]);
		strcat(inbox_buff,"|");
		printf("%9s", email_lastname[i]);
		strcat(inbox_buff, email_lastname[i]);
		strcat(inbox_buff,"|");
		printf("%15s",  email_subject[i]);
		strcat(inbox_buff, email_subject[i]);
		strcat(inbox_buff,"|");
		//printf("  Body : %s\n",  email_body[i]);
		printf("%40s", email_time[i]);
		memset(time_buff,'\0', 100);
		strncpy(time_buff, email_time[i], strlen(email_time[i])-1);
		strcat(inbox_buff, time_buff);
		strcat(inbox_buff,"#");
		mail_seq_ND[email_seq] = i;
		email_seq++;
		}
	}
	printf("\n inbox= %s",inbox_buff);
	strncpy(inbox_detail, inbox_buff, strlen(inbox_buff)+1);
	return inbox_detail;
}
char * check_email(char buff[LENGTH])
{
	char * check_email_inbox = malloc(LENGTH);
	char a[3] , email_time_new[100] , check_email_buff[LENGTH];
	int n,email_show, len;
	char * token;
	token = strtok(buff,"|");
	token = strtok(NULL, "|");
	strcpy(a,token);
	n = atoi(a);
	printf("--->  n is %d ", n);
	operation_email_seq = mail_seq_ND[n-1] ; 
	memset(check_email_buff, '\0' , LENGTH);
	strcpy(check_email_buff,"checkkk|");
	printf("email show = %d" , n);
	printf("\n***************Email is : ************\n");
	printf("Username  : %s\n", email_username[mail_seq_ND[n-1]]);
	printf("Firstname : %s\n", email_firstname[mail_seq_ND[n-1]]);
	printf("Lastname  : %s\n", email_lastname[mail_seq_ND[n-1]]);
	printf("Subject   : %s\n",  email_subject[mail_seq_ND[n-1]]);
	printf("Body      : %s\n",  email_body[mail_seq_ND[n-1]]);
	printf("Time      : %s\n", email_time[mail_seq_ND[n-1]]);
	memset(email_time_new, '\0' , 100);
	strncpy(email_time_new , email_time[mail_seq_ND[n-1]] ,strlen(email_time[mail_seq_ND[n-1]] ) - 1 );
	strcat(check_email_buff , email_username[mail_seq_ND[n-1]]);
	strcat(check_email_buff,"|");
	strcat(check_email_buff , email_firstname[mail_seq_ND[n-1]]);
	strcat(check_email_buff,"|");
	strcat(check_email_buff , email_lastname[mail_seq_ND[n-1]]);
	strcat(check_email_buff,"|");
	strcat(check_email_buff , email_subject[mail_seq_ND[n-1]]);
	strcat(check_email_buff,"|");
	strcat(check_email_buff , email_body[mail_seq_ND[n-1]]);
	strcat(check_email_buff,"|");
	strcat(check_email_buff , email_time_new);
	strcat(check_email_buff,"|");
	strcpy(check_email_inbox , check_email_buff);
	unreadtoRead(mail_seq_ND[n-1]);
	return check_email_inbox;
}

void email_reply(char buff[LENGTH])
{
	int n, lines;
	char *email_File, *nameOfFile;
	FILE *fpcfg;
	char * token , email_Body[500];
	n = operation_email_seq;
	token = strtok(buff,"|");
	token = strtok(NULL, "|");
	strcpy(email_Body , token);
	printf("\nContent is = %s",email_Body);
	printf("\nsender  is %s", current_user);	
	printf("\nReciever is %s", email_username[n]);
	printf("\nSubject is %s", email_subject[n]);
	lines = numoflines(fpcfg);
	nameOfFile = getfileName(fpcfg, email_username[n], lines);
	printf("\nFile name is %s", nameOfFile);
	emailservice(nameOfFile);	
	email_File= emailservice(nameOfFile);	
	printf("%s", email_File);
	compose(current_user,email_subject[n],email_Body,email_File,lines);
	
}
void email_forward(char buff[LENGTH])
{
	int n, lines,count=0,i;
	n = operation_email_seq;
	char *email_File, *nameOfFile, *token,recieverOfEmail[20][20];
	FILE *fpcfg;
	token = strtok(buff, "|");
	token = strtok(NULL, ",");
	while(token != NULL)
	{
		strncpy(recieverOfEmail[count++], token, strlen(token)+1);
		printf("\n%s", token);
		token = strtok(NULL,",");
	}
	printf("\nContent is = %s",email_body[n]);
	printf("\nsender  is %s", current_user);	
	printf("\nSubject is %s", email_subject[n]);
	for(i=0;i<count ;i++)
	{
		printf("\nReciever is %s", recieverOfEmail[i]);
		lines = numoflines(fpcfg);
		nameOfFile = getfileName(fpcfg, recieverOfEmail[i], lines);
		printf("\nFile name is %s", nameOfFile);
		emailservice(nameOfFile);	
		email_File= emailservice(nameOfFile);	
		printf("%s", email_File);
		compose(current_user,email_subject[n],email_body[n],email_File,lines);
	}

}

void delete_mail(char buff[LENGTH])
{
	char *email_File, *nameOfFile,email_Body[LENGTH], *token ,del[3];
	FILE *fpcfg;
	int lines,len,i, delete, n;	
	token = strtok(buff, "|");
	token = strtok(NULL , "|");
	strcpy(del,token);
	delete = atoi(del);
	if(delete == 0)
	{
		n = operation_email_seq;
	}
	else
	{
		n = delete;
	}
	strcpy(email_delete[mail_seq_ND[n-1]],"D");
	lines = numoflines(fpcfg);
	nameOfFile = getfileName(fpcfg,current_user , lines);
	printf("\nFile name is %s", nameOfFile);
	emailservice(nameOfFile);	
	email_File= emailservice(nameOfFile);	
	printf("%s", email_File);
	token  = strtok(email_File , ".txt");
	strncpy(email_File,token, strlen(token)+1);
	strcat(email_File,".txt");
	printf("%s", email_File);
	fpcfg = fopen(email_File, "w");
	for(i=0;i<mail_seq_total;i++)
	{
	fprintf(fpcfg, "||%s||%s||%s||%s||%s||%s||%s||%s",email_read[i],email_delete[i],email_username[i],email_firstname[i],email_lastname[i],email_subject[i],email_body[i],email_time[i]);
	}
	fclose(fpcfg);	
}

char * filter_email(char buff[LENGTH])
{
	char * filter_result = malloc(LENGTH);
	char choice[2], user[20], seq[2],mail_del_seq[3];
	char * token;
	int len,n,i, count=0,actual_count=0,k,j,mail_del;
	token  = strtok(buff,"|");
	token = strtok(NULL, "|");
	strcpy(choice , token);
	n = atoi(choice);
	if(n==1)
	{
		printf("\n Filter by username");
		token = strtok(NULL,"|");
		printf("\n--->filter token = = = =  %s\n",token);
		strcpy(user,token);
		for(i=0;i<email_seq;i++)
		{
			
				if(strcmp(email_username[mail_seq_ND[i]],user)==0)
				{
					
						total_user[count++]= i;
						actual_count++;
				}
			
				
		}
		if(actual_count==0)
		{
			printf("\nNo Emails with this username");
			strcpy(filter_result, "filter_res|NOOOOO|");
			
		}
		else
		{
			count = 0;
			strcpy(filter_result, "filter_res|YYY|");
			printf("\n  Status   Username\tfirstname\tlastname\tsubject                      time\n");
			printf("-------------------------------------------------------------------------------------------------\n");
			for(i=0;i<actual_count;i++)
			{
				//printf("**************************************************************\n");
				printf("%d)%s", i+1, email_read[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_read[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%5s", email_username[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_username[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%18s", email_firstname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_firstname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%15s", email_lastname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_lastname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%15s",  email_subject[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_subject[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				//printf("  Body : %s\n",  email_body[i]);
				printf("%40s", email_time[mail_seq_ND[total_user[count]]]);
				strncat(filter_result, email_time[mail_seq_ND[total_user[count]]], strlen(email_time[mail_seq_ND[total_user[count]]])-1);
				strcat(filter_result,"#");
				count++;
			}	
		}
	}
	else if(n==2)
	{
		printf("\n Filter by subject content");
		token = strtok(NULL,"|");
		printf("\n--->filter token = = = =  %s\n",token);
		strcpy(user,token);
		for(i=0;i<email_seq;i++)
		{
			
				if(strstr(email_subject[mail_seq_ND[i]],user)!= NULL)
				{
					
						total_user[count++]= i;
						actual_count++;	
				}
			
				
		}
		if(actual_count==0)
		{
			printf("\nNo Emails with this subject");
			strcpy(filter_result, "filter_res|NOOOOO|");
		}
		else
		{
			count = 0;
			strcpy(filter_result, "filter_res|YYY|");
			printf("\n  Status   Username\tfirstname\tlastname\tsubject                      time\n");
			printf("-------------------------------------------------------------------------------------------------\n");
			for(i=0;i<actual_count;i++)
			{
				//printf("**************************************************************\n");
				printf("%d)%s", i+1, email_read[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_read[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%5s", email_username[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_username[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%18s", email_firstname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_firstname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%15s", email_lastname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_lastname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%15s",  email_subject[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_subject[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				//printf("  Body : %s\n",  email_body[i]);
				printf("%40s", email_time[mail_seq_ND[total_user[count]]]);
				strncat(filter_result, email_time[mail_seq_ND[total_user[count]]], strlen(email_time[mail_seq_ND[total_user[count]]])-1);
				strcat(filter_result,"#");
				count++;
			}	
		}
	}
	else if(n==3)
	{
		printf("\n Filter by Date/Time");
		token = strtok(NULL,"|");
		printf("\n--->filter token = = = =  %s\n",token);
		strcpy(user,token);
		for(i=0;i<email_seq;i++)
		{
			
				if(strstr(email_time[mail_seq_ND[i]],user)!= NULL)
				{
					
						total_user[count++]= i;
						actual_count++;
				}
			
				
		}
		if(actual_count==0)
		{
			printf("\nNo Emails with this DATE/TIME");
			strcpy(filter_result, "filter_res|NOOOOO|");
		}
		else
		{
			count = 0;
			strcpy(filter_result, "filter_res|YYY|");
			printf("\n  Status   Username\tfirstname\tlastname\tsubject                      time\n");
			printf("-------------------------------------------------------------------------------------------------\n");
			for(i=0;i<actual_count;i++)
			{
				//printf("**************************************************************\n");
				printf("%d)%s", i+1, email_read[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_read[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%5s", email_username[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_username[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%18s", email_firstname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_firstname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%15s", email_lastname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_lastname[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				printf("%15s",  email_subject[mail_seq_ND[total_user[count]]]);
				strcat(filter_result, email_subject[mail_seq_ND[total_user[count]]]);
				strcat(filter_result,"|");
				//printf("  Body : %s\n",  email_body[i]);
				printf("%40s", email_time[mail_seq_ND[total_user[count]]]);
				strncat(filter_result, email_time[mail_seq_ND[total_user[count]]], strlen(email_time[mail_seq_ND[total_user[count]]])-1);
				strcat(filter_result,"#");
				count++;
			}	
		}
	}
	return filter_result;
}
char * check_mail_filter(char buff[LENGTH])
{
	char * check_email_filter = malloc(LENGTH);
	char a[3] , email_time_new[100] , check_email_buff[LENGTH];
	int n,email_show, len;
	char * token;
	token = strtok(buff,"|");
	token = strtok(NULL, "|");
	strcpy(a,token);
	n = atoi(a);
	n = n-1;
	n = total_user[n];
	operation_email_seq = mail_seq_ND[n];
	printf("--->  n is %d ", n);
	memset(check_email_buff, '\0' , LENGTH);
	strcpy(check_email_buff,"filterrr|");
	printf("email show = %d" , n);
	printf("\n***************Email is : ************\n");
	printf("Username  : %s\n", email_username[mail_seq_ND[n]]);
	printf("Firstname : %s\n", email_firstname[mail_seq_ND[n]]);
	printf("Lastname  : %s\n", email_lastname[mail_seq_ND[n]]);
	printf("Subject   : %s\n",  email_subject[mail_seq_ND[n]]);
	printf("Body      : %s\n",  email_body[mail_seq_ND[n]]);
	printf("Time      : %s\n", email_time[mail_seq_ND[n]]);
	memset(email_time_new, '\0' , 100);
	strncpy(email_time_new , email_time[mail_seq_ND[n]] ,strlen(email_time[mail_seq_ND[n]] ) - 1 );
	strcat(check_email_buff , email_username[mail_seq_ND[n]]);
	strcat(check_email_buff,"|");
	strcat(check_email_buff , email_firstname[mail_seq_ND[n]]);
	strcat(check_email_buff,"|");
	strcat(check_email_buff , email_lastname[mail_seq_ND[n]]);
	strcat(check_email_buff,"|");
	strcat(check_email_buff , email_subject[mail_seq_ND[n]]);
	strcat(check_email_buff,"|");
	strcat(check_email_buff , email_body[mail_seq_ND[n]]);
	strcat(check_email_buff,"|");
	strcat(check_email_buff , email_time_new);
	strcat(check_email_buff,"|");
	strcpy(check_email_filter , check_email_buff);
	unreadtoRead(mail_seq_ND[n]);
	return check_email_filter;
}

void delete_mail_filter(char buff[LENGTH])
{
	char *email_File, *nameOfFile,email_Body[LENGTH], *token ,del[3];
	FILE *fpcfg;
	int lines,len,i, delete, n;	
	token = strtok(buff, "|");
	token = strtok(NULL , "|");
	strcpy(del,token);
	delete = atoi(del);
	if(delete == 0)
	{
		n = operation_email_seq;
	}
	else
	{
		n = delete;
	}
	n = n-1;
	n = total_user[n];
	strcpy(email_delete[mail_seq_ND[n]],"D");
	lines = numoflines(fpcfg);
	nameOfFile = getfileName(fpcfg,current_user , lines);
	printf("\nFile name is %s", nameOfFile);
	emailservice(nameOfFile);	
	email_File= emailservice(nameOfFile);	
	printf("%s", email_File);
	token  = strtok(email_File , ".txt");
	strncpy(email_File,token, strlen(token)+1);
	strcat(email_File,".txt");
	printf("%s", email_File);
	fpcfg = fopen(email_File, "w");
	for(i=0;i<mail_seq_total;i++)
	{
	fprintf(fpcfg, "||%s||%s||%s||%s||%s||%s||%s||%s",email_read[i],email_delete[i],email_username[i],email_firstname[i],email_lastname[i],email_subject[i],email_body[i],email_time[i]);
	}
	fclose(fpcfg);	
}
void unreadtoRead(int n)
{
	char *email_File, *nameOfFile,email_Body[LENGTH], *token ,del[3];
	FILE *fpcfg;
	int lines,len,i;	
	strcpy(email_read[n],"( SEEN )");
	lines = numoflines(fpcfg);
	nameOfFile = getfileName(fpcfg,current_user , lines);
	printf("\nFile name is %s", nameOfFile);
	emailservice(nameOfFile);	
	email_File= emailservice(nameOfFile);	
	printf("%s", email_File);
	token  = strtok(email_File , ".txt");
	strncpy(email_File,token, strlen(token)+1);
	strcat(email_File,".txt");
	printf("%s", email_File);
	fpcfg = fopen(email_File, "w");
	for(i=0;i<mail_seq_total;i++)
	{
	fprintf(fpcfg, "||%s||%s||%s||%s||%s||%s||%s||%s",email_read[i],email_delete[i],email_username[i],email_firstname[i],email_lastname[i],email_subject[i],email_body[i],email_time[i]);
	}
	fclose(fpcfg);	
}

int totalEmailsLatest()
{
	char emailFile[100], temp_user[50];
	FILE * fp;
	int n=0,ch_count=0;
	strcpy(emailFile,"email/");
	strcpy(temp_user, th_user);
	strcat(temp_user,".txt");
	strcat(emailFile, temp_user);
	fp = fopen(emailFile,"r");
	while ((ch_count= fgetc(fp)) != EOF)
	{
		if (ch_count == '\n')
		n++;
	}
	fclose(fp);
	//printf("total emails = %d", n);
	return n ;
}

int totalCalReq()
{
	
	char calFile[100];
	int n=0,ch_count=0;
	FILE *fp;
	strcpy(calFile, th_user);
	strcat(calFile,"_calendar");
	fp = fopen(calFile,"r");
	while ((ch_count= fgetc(fp)) != EOF)
	{
		if (ch_count == '\n')
		n++;
	}
	fclose(fp);
	return n ;
	
}
/*void filter_email_forward(char buff[LENGTH])
{
	int n, lines,count=0,i;
	n = operation_email_seq;
	char *email_File, *nameOfFile, *token,recieverOfEmail[20][20];
	FILE *fpcfg;
	token = strtok(buff, "|");
	token = strtok(NULL,"|");
	token = strtok(NULL, ",");
	while(token != NULL)
	{
		strncpy(recieverOfEmail[count++], token, strlen(token)+1);
		printf("\n%s", token);
		token = strtok(NULL,",");
	}
	printf("\nContent is = %s",email_body[n]);
	printf("\nsender  is %s", current_user);	
	printf("\nSubject is %s", email_subject[n]);
	for(i=0;i<count ;i++)
	{
		printf("\nReciever is %s", recieverOfEmail[i]);
		lines = numoflines(fpcfg);
		nameOfFile = getfileName(fpcfg, recieverOfEmail[i], lines);
		printf("\nFile name is %s", nameOfFile);
		emailservice(nameOfFile);	
		email_File= emailservice(nameOfFile);	
		printf("%s", email_File);
		compose(current_user,email_subject[n],email_body[n],email_File,lines);
	}

}*/
/*void filter_email_reply(char buff[LENGTH])
{
	int n, lines;
	char *email_File, *nameOfFile;
	FILE *fpcfg;
	char * token , email_Body[500];
	n = operation_email_seq;
	token = strtok(buff,"|");
	token = strtok(NULL, "|");
	token = strtok(NULL, "|");
	strcpy(email_Body , token);
	printf("\nContent is = %s",email_Body);
	printf("\nsender  is %s", current_user);	
	printf("\nReciever is %s", email_username[n]);
	printf("\nSubject is %s", email_subject[n]);
	lines = numoflines(fpcfg);
	nameOfFile = getfileName(fpcfg, email_username[n], lines);
	printf("\nFile name is %s", nameOfFile);
	emailservice(nameOfFile);	
	email_File= emailservice(nameOfFile);	
	printf("%s", email_File);
	compose(current_user,email_subject[n],email_Body,email_File,lines);
}*/

