#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<pthread.h>

#define LENGTH 65536

int total_email_count = 0;
int logout_service = 1 ;
int total_cal_count = 0;
struct sockaddr_in servaddr;

char current_user[20];
char * signup_valid(); 
void clean_stdin(void);						// for fgets to read continuous lines. It will not read \n as input
char * login(); 							// login service
char * signup(); 							// signup service
int doOperation();							// Select service mail or calander
int mail_service(); 						// mail service option 
char * mail_compose(); 						// compose email service
int  showInbox(char buff[LENGTH]);			// Display inbox
int check_email(char buff[LENGTH]); 		// check the selected email
char * email_reply(); 						// Reply to Email
char * email_forward();						// Forward to Email
char * filter_option(); 					// Select the option of filter
char * mail_filter(char buff[LENGTH]);		// Filter Result display
int filter_check_email(char buff[LENGTH]);  //Filter check email
char signupuser[50];
void *backgroundthread();

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int sockfd;

void
display_event(char *buf, int n)
{
	char * token;
	int i=0,choice,j;

	char str[50];

	if (buf[strlen(buf) - 1] == '1')
	{
		return;
	}
	else
	{
		printf("seq no: %d)\n",n);
		while(i<=strlen(buf) -2)
		{
			j = 0;
			while((buf[i]!='|') && (i <= strlen(buf) -2))
			{
				str[j] = buf[i];
				j++;
				i++;
			}
			str[j]='\0';

			choice++;
			i++;

			switch( choice )
			{
				case 3:
					printf("\tdate :%s\n", str);
					break;

				case 1:
					printf("start_time : %s", str);
					break;

				case 2:
					printf("\t end_time :%s", str);
					break;

				case 4:
					printf("From: %s\n", str);
					break;

				case 8:
					printf("Place :%s\n", str);	
					break;

				case 6:
					printf("subject :%s\n", str);	
					break;


				case 7:
					printf("Description :%s\n", str);	
					break;

				default:
					break;
			}
		}	
	}


}
void
display_return_delete_apt(char * buff)
{
	char * token;
		
	token = strtok(buff, "|");
	token = strtok(NULL, "#");

	if (strcmp(token, "NULL") == 0)
	{
		printf ("!!! DELETION OF APPOINTMENT SUCCESFUL\n");
		sleep(2);
	}
	else
	{
		printf("DELETION FAILED!!!");
		sleep(2);
	}
	
	return;
}

char *
process_apt1(char * buff)
{
	char * token;
	char *buf;
		
	token = strtok(buff, "|");
	token = strtok(NULL, "#");

	if (strcmp(token, "NULL") == 0)
	{
		printf ("Wrong Sequence Number can't Modify!!!\n");
		sleep(2);
	}

	buf = malloc(2048);
	strcpy(buf, token);

	
	return buf;
}



char *
process_apt2(char * buff)
{
	char * token;
		
	token = strtok(buff, "|");
	token = strtok(NULL, "#");

	if (strcmp(token, "NULL") != 0)
	{
		printf ("Modification not possible due to conflicts!!!\n");
		sleep(2);
	}
	else
	{
		printf("Modification succesful !!\n");
		sleep(2);
	}
	
	return token;
}


void
display_return_create_apt(char * buff)
{
	char * token;
		
	token = strtok(buff, "|");
	token = strtok(NULL, "#");

	if (strcmp(token, "NULL") == 0)
	{
		printf ("!!! CREATION OF APPOINTMENT SUCCESFUL\n");
		sleep(2);
	}
	else
	{
		printf("Appointment Failed due to conflict with user:%s\n", token);
		sleep(2);
	}
	
	return;
}
char * create_appointment( char *recv)
{
	char * cal_detail = (char *)malloc(sizeof(char) * 2048);
	char toRecievers[100], toSub[500], toBody[500], tocal[2048], cu_user[20],date[20],start_time[20],end_time[20],place[100];
	int len;

	printf("recv is %s\n", recv);

	printf("\n =====> Calendar Event  Creation\n");
	if (recv == NULL)
	{
		printf("\nEnter  the Participating Memebers details(seperate users using ','):\n");
		fgets(toRecievers,sizeof(toRecievers),stdin);
		len = strlen(toRecievers);
		if(toRecievers[len-1]== '\n')
		{
			toRecievers[len-1] = '\0';
		}
		strcpy(cu_user,current_user);
		strcat(cu_user, ",");
		strcat(cu_user,toRecievers);
		strcpy(toRecievers,cu_user);
	}
	else
	{
		strcpy(toRecievers, recv);
	}
		strcat(toRecievers, "|");
	printf("Enter the date in (MM/DD/YYYY) Format:\n");
	fgets(date, sizeof(date), stdin);
	len = strlen(date);
	if(date[len-1]== '\n')
	{
		date[len-1] = '\0';
	}
	strcat(date, "|");
	
	printf("Eneter the Meeting start time in (HH:MM) Format:\n");
	fgets(start_time, sizeof(start_time), stdin);
	len = strlen(start_time);
	if(start_time[len-1]== '\n')
	{
		start_time[len-1] = '\0';
	}
	strcat(start_time, "|");

	printf("Enter the Meeting end time in (HH:MM) Format:\n");
	fgets(end_time, sizeof(end_time), stdin);
	len = strlen(end_time);
	if(end_time[len-1]== '\n')
	{
		end_time[len-1] = '\0';
	}
	strcat(end_time, "|");

	printf("Enter the Meeting place:\n");
	fgets(place, sizeof(place), stdin);
	len = strlen(place);
	if(place[len-1]== '\n')
	{
		place[len-1] = '\0';
	}
	strcat(place, "|");
	
	printf("Enter Subject of Calendar:\n");
	fgets(toSub, sizeof(toSub),stdin);
	len = strlen(toSub);
	if(toSub[len-1]== '\n')
	{
		toSub[len-1] = '\0';
	}
	strcat(toSub, "|");
	printf("Enter Body of Calendar:\n");
	fgets(toBody, sizeof(toBody),stdin);
	len = strlen(toBody);
	if(toBody[len-1]== '\n')
	{
		toBody[len-1] = '\0';
	}

	strcat(toBody,"|");
	/* Copy all the contents into a buffer and pass ito the server*/
	strcpy(tocal,toRecievers);
	strcat(tocal,date);
	strcat(tocal,start_time);
	strcat(tocal, end_time);
	strcat(tocal, place);
	strcat(tocal, toSub);
	strcat(tocal, toBody);
	strcpy(cal_detail, tocal);
	return cal_detail;
}

int calendar_service()
{	
	char option[2];
	int n,len,flag;
	system("clear");
	do{
		printf("What do you want to do : \n");
		printf("1. View Appointments\n");
		printf("2. Create Appointments\n");
		printf("3. Go back to menu\n");
		printf("Enter your choice here:");
		//fflush(stdout);
		fgets(option,sizeof(option),stdin);
		clean_stdin();
		n = atoi(option);
		if(n<0 || n >3)
		{
			printf("\n ***************Please select valid option***********\n");
			flag =1;
		}
		else
		{
			flag=0;
		}	
	}while(flag == 1);
	return n;
}

int sview_apt(char buff[LENGTH])
{
	char * token, *token2,temp[LENGTH],temp2[LENGTH],seq[2];
	char s1[50],s2[50],s3[50],s4[50],s5[50],s6[50],s7[50],s8[50];
	int cal_seq=0, k=0,i,n,flag,d;
	strcpy(temp,buff);
	token = strtok(temp,"|");
	token = strtok(NULL,"#");
	strcpy(temp2,token);

	if (strcmp(token, "NULL") == 0)
	{
		printf("\nNO Events to display\n");
		sleep(2);
		return 3;
	}

	token = strtok(temp2, "%");

	while (token!=NULL)
	{
		display_event(token, ++k);
		token = strtok(NULL, "%");
	}
		
/*
	
	while(token!= NULL)
	{
		cal_seq++;
		token = strtok(NULL, "#");
	}

	token = strtok(buff,"|");
	for(i=0;i<cal_seq; i ++)
	{
		token = strtok(NULL,"|");
		printf(" seq no: %d)%s		",++k, token);
		token = strtok(NULL, "|");
		printf("date:%s", token);
		token = strtok(NULL, "|");
		printf("start_time:%12s", token);
		token = strtok(NULL,"|");
		printf(" end_time:%9s\n", token);
		token = strtok(NULL, "|");
		printf("place:%15s\n", token);
		token = strtok(NULL, "|");
		printf("subject:%s\n", token);	
		token = strtok(NULL, "#");	
		printf("body:%s\n\n", token);
	}
*/
	do{
		printf("\nWhat do you want to do?");
		printf("\n1. Delete From the event");
		printf("\n2. Modify the Event");
		printf("\n3. Go Back");
		printf("\n Enter your choice:");
		fgets(seq,2,stdin);
		clean_stdin();
		n = atoi(seq);
		if(n<0 || n>3)
		{
			printf("\n Please choose valid option");
			flag = 1;
		}
		else
		{
			flag = 0;
		}
	}while(flag==1);
	//printf("\nreturning n value %d", n);
	return n;
}

int main(int argc, char *argv[])
{
	int num,n,calId;
	char *temp;
	char number;
	int operationId, mailserviceId,calserviceId, inboxId, checkEmailId, filtercheckId;
	char sendline[1024];
	char sendBuff[LENGTH], recvBuff[LENGTH] , input[2], tempBuff[LENGTH], seq[3], seq_del[3], temporary[2048];
	char * loginDetail, *token_set, *mail_detail,*calendar_detail, *token, *email_forward_content, *email_reply_content, *filter_choice, *signupvalid, *signupdetail;
	char *cal_detail;
	char * mail_filter_num;
	struct in_addr ip_addr;
	struct hostent *server;
	pthread_t bgthread;
  	if(argc < 3)
	{
	  fprintf(stderr,"usage %s hostname port\n", argv[0]);
	  exit(0);
	}
  /*CREATING SOCKET FOR CLIENT*/	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		printf("\n ERROR IN CREATING CLIENT SOCKET\n");
		exit(2);
	}

  /* SOCKET CREATION */
	server=gethostbyname(argv[2]);
	ip_addr = *(struct in_addr *)(server ->h_addr);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
  // servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_addr.s_addr = inet_addr(inet_ntoa(ip_addr));
	servaddr.sin_port = htons(atoi(argv[1]));
  
  /* CONNECTION OF CLIENT TO SERVER */
  
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("\n ERROR IN CONNECTING\n");
		exit(3);
	}
	printf("\n client: connected to server \n");
  
  /* ASK USER FOR CHOICES */
   while(1)
	{
		setbuf(stdout,NULL);
		pthread_mutex_lock(&lock);
		printf("\nHELLO USER \n-->WHAT YOU WANT TO DO? \n-->(a)LOGIN \n-->(b)SIGN UP \n-->(c)quit\n");
    
		printf("\n ENTER YOUR CHOICE ( a / b / c ): ");
		fgets(input, sizeof(input), stdin);
		clean_stdin();
		memset(sendBuff,'\0',sizeof(sendBuff));
		sendBuff[0]= input[0];
		if(input[0] == 'c')
		{
			printf("\n\n\nQuit\n\n\n");
			close(sockfd);
			exit(0);
		}
		//printf("\nsendBuff = %s", sendBuff);		
				
		n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
		pthread_mutex_unlock(&lock);
		if(n<0)
		{
			perror("WRITE ERROR");
		}
		//printf("\n------> recvBuff= %s", recvBuff);
		while(1)
		{
			if(strcmp(recvBuff,"login")==0)
			{
				pthread_mutex_lock(&lock);
				loginDetail=login();
				//printf("loginDetail= %s", loginDetail);
				// writing login detail  to server
				memset(sendBuff,'\0',sizeof(sendBuff));
				strcpy(sendBuff,"login");
				strcat(sendBuff, "|");
				strcat(sendBuff,loginDetail);
				//printf("\nsendBuff = %s", sendBuff);		
						
				n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
				pthread_mutex_unlock(&lock);
			}
			if(strcmp(recvBuff,"signup") == 0)
			{
				//signup service
				pthread_mutex_lock(&lock);
				printf(" signup");
				memset(sendBuff , '\0', sizeof(sendBuff));
				signupvalid = signup_valid();
				strcpy(sendBuff,"signup");
				strcat(sendBuff,"|");
				strcat(sendBuff,signupvalid);
				printf("-->%s\n",sendBuff );
						
				n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
				pthread_mutex_unlock(&lock);
			}
			if(strcmp(recvBuff,"exist") == 0)
			{
				printf("\nThis username is already exist please choose different username\n");
				memset(recvBuff,'\0',sizeof(recvBuff));
				break;
			}
			if(strcmp(recvBuff,"killing") == 0)
			{
				printf("KILL");
				close(sockfd);
				exit(0);
			}
			if(strcmp(recvBuff,"dont") == 0)
			{
				pthread_mutex_lock(&lock);
				printf("\nsuccess\n");
				signupdetail = signup();
				memset(sendBuff , '\0', sizeof(sendBuff));
		
				strcpy(sendBuff,"dosignup");
				strcat(sendBuff,"|");
				strcat(sendBuff,signupdetail);
				printf("-->%s\n",sendBuff );
				//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
						
				n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
				pthread_mutex_unlock(&lock);
				
			}
			//if autheticated
			if(strcmp(recvBuff,"true") == 0)
			{
				token = strtok(loginDetail, "|");
				strcpy(current_user, token);
				logout_service = 0;
				system("clear");
				//total_email_count = totalEmailsLatest();
				//total_cal_count = totalCalReq();
				printf("\n\t\t\t WELCOME %s \t\t\n", current_user);
				operationId = doOperation();
				if(operationId == 1)
				{
					pthread_mutex_lock(&lock);
					printf("\n Email service");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff,"emailServ");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				if(operationId == 2)
				{
					pthread_mutex_lock(&lock);
					printf("\n Calander service");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff,"calServ");
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
				}
				if(operationId == 3)
				//else
				{
					pthread_mutex_lock(&lock);
					printf("\n logout");
					logout_service = 1;
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff,"logout");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
			}
			if(logout_service == 0)
			{
				pthread_create(&bgthread,NULL,backgroundthread,NULL);
			}
			// if not authenticated 
			if(strcmp(recvBuff,"false") == 0)
			{
				system("clear");
				printf("\n USERNAME AND PASSWORD DO NOT MATCH PLEASE TRY AGAIN\n");
				memset(recvBuff,'\0',sizeof(recvBuff));
				break;
			}
			//if email service
			if(strcmp(recvBuff,"emailServ") == 0)
			{
				system("clear");
				printf("\n\t\t\t EMAIL SERVICE \n");
				mailserviceId = mail_service();
				if(mailserviceId == 1)
				{
					pthread_mutex_lock(&lock);
					printf("\n Email service");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff,"inbox");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				if(mailserviceId == 2)
				{
					pthread_mutex_lock(&lock);
					printf("\n Compose Email");
					memset(recvBuff,'\0',sizeof(recvBuff));
					mail_detail = mail_compose();
					//printf("\n detail of Buffer while sending mail is %s", mail_detail);
					//sleep(5);
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff,"compose");
					strcat(sendBuff,"|");
					strcat(sendBuff, mail_detail);
					//printf("\n detail of Buffer while sending mail is %s", mail_detail);
					//sleep(5);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				//if(mailserviceId == 3)
				else
				{
					pthread_mutex_lock(&lock);
					printf("\n logout");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff,"goback");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				
			}
			//Calendar service
			if(strcmp(recvBuff,"calServ") == 0)
			{
				system("clear");
				printf("\n\t\t\t CALENDAR SERVICE \n");
				calserviceId = calendar_service();
				if(calserviceId == 1)
				{
					pthread_mutex_lock(&lock);
					printf("\n view  Appointment");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff,"view_apt");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				if(calserviceId == 2)
				{
					pthread_mutex_lock(&lock);
					printf("\n create appointment");
					memset(recvBuff,'\0',sizeof(recvBuff));
					calendar_detail = create_appointment(NULL);
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff,"create_apt");
					strcat(sendBuff,"|");
					strcat(sendBuff, calendar_detail);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				//if(calserviceId == 3)
				else
				{
					pthread_mutex_lock(&lock);
					printf("\n MAIL & Calendar MENU");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff,"goback");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				
			}
			//Calendar service
			if(strcmp(tempBuff, "sview_apt") == 0)
			{
				system("clear");
				
				printf("\n\t\t\t********** %s 's CALENDAR **********\n", current_user);
				printf("\t\t\t---------------------------------------\n");
				//printf("%s", recvBuff);
				calId = sview_apt(recvBuff);
				if(calId == 1)
				{
					pthread_mutex_lock(&lock);	
					printf("DELETE APPOINTMENT");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "delete_apt|");
					printf("\nEnter the sequence to delete:"); 
					fgets(seq, 3 , stdin);
					if(seq[strlen(seq)-1] == '\n')
					{	
						seq[strlen(seq) -1 ] = '\0';
					}
					strcat(sendBuff, seq);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 ); 
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				if(calId == 2)
				{
					pthread_mutex_lock(&lock);
					printf("MODIFY APPOINTMENT");
					printf("\nWhich event do you want to modify:");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "modify_apt1|");
					printf("\nEnter the sequence number:"); 
					fgets(seq, 3 , stdin);
					if(seq[strlen(seq)-1] == '\n')
					{	
						seq[strlen(seq) -1 ] = '\0';
					}
					strcat(sendBuff, seq);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 ); 
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				//if(calId == 3)
				else
				{
					pthread_mutex_lock(&lock);
					printf("GO BACK");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "goback_cal");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
			}

			if(strcmp(tempBuff, "ret_modify_apt1") == 0)
			{
				temp = process_apt1(recvBuff);
				if ( temp == NULL)
				{
					pthread_mutex_lock(&lock);
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "goback_cal");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				else
				{
					pthread_mutex_lock(&lock);
					printf("\n create new appointment");
					memset(recvBuff,'\0',sizeof(recvBuff));
					calendar_detail = create_appointment(temp);
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff,"modify_apt2");
					strcat(sendBuff,"|");
					strcat(sendBuff, calendar_detail);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
			}
				
			

			if(strcmp(tempBuff, "ret_modify_apt2") == 0)
			{
				pthread_mutex_lock(&lock);
				process_apt2(recvBuff);
				memset(sendBuff,'\0',sizeof(sendBuff));
				strcpy(sendBuff, "goback_cal");
				//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
						
				n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
				pthread_mutex_unlock(&lock);
			}


			if(strcmp(tempBuff, "ret_creat_apt") == 0)
			{
				pthread_mutex_lock(&lock);
				display_return_create_apt(recvBuff);
				memset(sendBuff,'\0',sizeof(sendBuff));
				strcpy(sendBuff, "goback_cal");
				//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
						
				n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
				pthread_mutex_unlock(&lock);
			}


			if(strcmp(tempBuff, "ret_delete_apt") == 0)
			{
				pthread_mutex_lock(&lock);
				display_return_delete_apt(recvBuff);
				memset(sendBuff,'\0',sizeof(sendBuff));
				strcpy(sendBuff, "goback_cal");
				//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
						
				n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
				pthread_mutex_unlock(&lock);
			}


			//inbox print
			if(strcmp(tempBuff,"inboxxx") == 0)
			{
				system("clear");
				
				printf("\n\t\t\t********** %s 's INBOX **********\n", current_user);
				printf("\t\t\t---------------------------------------\n");
				//printf("%s", recvBuff);
				inboxId = showInbox(recvBuff);
				if(inboxId == 1)
				{
					pthread_mutex_lock(&lock);
					printf("CHECK EMAIL");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "check_email|");
					printf("\nEnter a sequence to check email:"); 
					fgets(seq, 3 , stdin);
					clean_stdin();
					if(seq[strlen(seq)-1] == '\n')
					{
						seq[strlen(seq) -1 ] = '\0';
						
					}
					strcat(sendBuff, seq);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 ); 
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				if(inboxId == 2)
				{
					pthread_mutex_lock(&lock);
					printf("DELETE EMAIL");
					printf("\nWhich message do you want to delete:");
					fgets(seq_del, sizeof(seq_del), stdin);
					clean_stdin();
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "delete_email|");
					strncat(sendBuff,seq_del,strlen(seq_del)-1);
					printf("%s . ", sendBuff);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				if(inboxId == 3)
				{
					pthread_mutex_lock(&lock);
					printf("FILTER EMAIL");
					filter_choice =  filter_option();
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "filter_email|");
					strcat(sendBuff, filter_choice);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				//if(inboxId == 4)
				else
				{
					pthread_mutex_lock(&lock);							
					printf("GO BACK");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "goback_email");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
			}
			// check email
			if(strcmp(tempBuff,"checkkk") == 0)
			{
				system("clear");
				printf("\n***************Email is : ************\n");
				checkEmailId = check_email(recvBuff);
				if(checkEmailId == 1)
				{
					pthread_mutex_lock(&lock);
					printf("\n Reply Email");
					email_reply_content = email_reply();
					printf("\n reply : %s", email_reply_content);
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, email_reply_content);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				if(checkEmailId == 2)
				{
					pthread_mutex_lock(&lock);
					printf("\n Forward Email");
					email_forward_content = email_forward();
					printf("\n Forward : %s", email_forward_content);
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, email_forward_content);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				/*if(checkEmailId == 3)
				{
					printf("\n Delete Email");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "delete_email|0");
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
				}*/
				//if(checkEmailId == 3)
				else
				{	pthread_mutex_lock(&lock);
					printf("\n Go Back");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "inbox");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
			}
			if(strcmp(tempBuff,"filter_res") == 0)
			{
				mail_filter_num = mail_filter(recvBuff);
				printf("\n ---> %s.", mail_filter_num);
				if(strcmp(mail_filter_num,"NOOOOO")==0)
				{
					pthread_mutex_lock(&lock);
					sleep(3);
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "inbox");
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				else
				{
					pthread_mutex_lock(&lock);
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, mail_filter_num);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );	
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
			}
			if(strcmp(tempBuff,"filterrr") == 0)
			{
				//system("clear");
				printf("\n***************Email is : ************\n");
				filtercheckId = filter_check_email(recvBuff);
				if(filtercheckId == 1)
				{
					pthread_mutex_lock(&lock);
					printf("\n Reply Email");
					email_reply_content = email_reply();
					printf("\n reply : %s", email_reply_content);
					/*memset(temporary,'\0',sizeof(temporary));
					strcpy(temporary,"filter_rep|");
					strcat(temporary,email_reply_content);
					memset(sendBuff,'\0',sizeof(sendBuff));*/
					strcpy(sendBuff, email_reply_content);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				if(filtercheckId == 2)
				{
					pthread_mutex_lock(&lock);
					printf("\n Forward Email");
					email_forward_content = email_forward();
					printf("\n Forward : %s", email_forward_content);
					/*memset(temporary,'\0',sizeof(temporary));
					strcpy(temporary,"filter_fwd|");
					strcat(temporary,email_forward_content);*/
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, email_forward_content);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}
				/*if(filtercheckId == 3)
				{
					printf("\n Delete Email");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "delete_email|0");
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
				}*/
				//if(filtercheckId == 3)
				else
				{
					pthread_mutex_lock(&lock);
					printf("\n Go Back");
					memset(sendBuff,'\0',sizeof(sendBuff));
					strcpy(sendBuff, "inbox" );
					//printf("going back from filter: %s", sendBuff);
					//n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
							
					n= write(sockfd, sendBuff, strlen(sendBuff)+1 );
					pthread_mutex_unlock(&lock);
				}	
			}
			//if calander service
			if(strcmp(recvBuff,"false") == 0)
			{
				system("clear");
				printf("\n CALANDER SERVICE \n");
				memset(recvBuff,'\0',sizeof(recvBuff));
				break;
			}
			//if logout 

			if(strcmp(recvBuff,"logout") == 0)
			{
				system("clear");
				memset(recvBuff,'\0',sizeof(recvBuff));
				break;
			}
			memset(recvBuff,'\0',sizeof(recvBuff));
			//n = read(sockfd,recvBuff,sizeof(recvBuff));
			pthread_mutex_lock(&lock);
			n = read(sockfd,recvBuff,sizeof(recvBuff));
			pthread_mutex_unlock(&lock);
			if(n<0)
			{
				perror("READ ERROR");
			}
			memset(tempBuff,'\0', sizeof(tempBuff));
			strcpy(tempBuff, recvBuff);
			//printf("\nrecvBuff=%s", recvBuff);
			token = strtok(tempBuff,"|");
			//printf("\n---> token = %s.", token);
			strcpy(tempBuff, token);
		}
	}
	close(sockfd);
}
 
void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
} 
char * signup_valid()
{
	char * signupDetail = malloc(20);
	printf("\nEnter Username: ");
	fgets(signupuser,sizeof(signupuser), stdin);
	
	if(signupuser[strlen(signupuser)-1 ]== '\n')
	{
		signupuser[strlen(signupuser)-1] = '\0';
	}
	strcpy(signupDetail, signupuser);
	return signupDetail;
	
		
}
char * signup()
{
	char * sign = malloc(500);
	char tempo[50];
	strcpy(sign, signupuser);
	strcat(sign,"|");
	printf("\nEnter a password : ");
	fgets(tempo,sizeof(tempo), stdin);

	if(tempo[strlen(tempo)-1 ]== '\n')
	{
		tempo[strlen(tempo)-1] = '\0';
	}
	strcat(sign, tempo);
	strcat(sign,"|");
	//clean_stdin();
	printf("\nENTER YOUR FIRST NAME: ");
	fgets(tempo,sizeof(tempo), stdin);
	if(tempo[strlen(tempo)-1 ]== '\n')
	{
		tempo[strlen(tempo)-1] = '\0';
	}
	strcat(sign, tempo);
	strcat(sign,"|");
	//clean_stdin();
	printf("\nENTER YOUR LAST NAME: ");
	fgets(tempo,sizeof(tempo), stdin);
	if(tempo[strlen(tempo)-1 ]== '\n')
	{
		tempo[strlen(tempo)-1] = '\0';
	}
	strcat(sign, tempo);
	strcat(sign,"|");
	return sign;
	
}
char * login()
{
	char * loginDetail = malloc(20);
	char password[20], username[20];
	printf("\nEnter Username: ");
	fgets(current_user,sizeof(current_user), stdin);
	//clean_stdin();
	if(current_user[strlen(current_user)-1 ]== '\n')
	{
		current_user[strlen(current_user)-1] = '\0';
	}
	strcpy(username,current_user);
	printf("\nEnter Password: ");
	fgets(password,sizeof(password),stdin);
	//clean_stdin();
	if(password[strlen(password)-1 ]== '\n')
	{
		password[strlen(password)-1] = '\0';
	}
	strcat(username,"|");
	strcat(username,password);
	strcpy(loginDetail, username);
	//printf("\nlogin detail = %s hii", loginDetail);
	return loginDetail;
}

int doOperation()
{
	int n,flag ,len;
	char op_option[2];
	do{
		printf("Which Service do you want: \n");
		printf("1. Email Service\n");
		printf("2. Calander Service\n");
		printf("3. Logout\n");
		printf("Enter your choice:");
		memset(op_option, '\0',2);
		//fflush(stdout);
		fgets(op_option, sizeof(op_option), stdin);
		clean_stdin();
		n = atoi(op_option);
		if(n<0 || n > 3)
		{
			printf("\n ***************Please select valid option***********\n");
			flag=1;
		}
		else
		{
			flag = 0;
		}
	}while(flag == 1);		
	return n; 
	
}  
int mail_service()
{	
	char option[2];
	int n,len,flag;
	system("clear");
	do{
		printf("What do you want to do : \n");
		printf("1. Check Inbox\n");
		printf("2. Compose Email\n");
		printf("3. Go back to menu\n");
		printf("Enter your choice here:");
		//fflush(stdout);
		fgets(option,sizeof(option),stdin);
		clean_stdin();
		n = atoi(option);
		if(n<0 || n >3)
		{
			printf("\n ***************Please select valid option***********\n");
			flag =1;
		}
		else
		{
			flag=0;
		}	
	}while(flag == 1);
	return n;
}

char * mail_compose()
{
	char * mail_detail = malloc(2048);
	char toRecievers[100], toSub[500], toBody[500], toEmail[2048], cu_user[20];
	int len;
	//pthread_mutex_lock(&lock);
	printf("\n =====> NEW Email Creation\n");
	printf("\nEnter usernames to send email (seperate users using ','):\n");
	fgets(toRecievers,sizeof(toRecievers),stdin);
	len = strlen(toRecievers);
	if(toRecievers[len-1]== '\n')
	{
		toRecievers[len-1] = '\0';
	}
	strcpy(cu_user,current_user);
	strcat(cu_user, ",");
	strcat(cu_user,toRecievers);
	strcpy(toRecievers,cu_user);
	strcat(toRecievers, "|");
	printf("Enter Subject of Email:\n");
	fgets(toSub, sizeof(toSub),stdin);
	len = strlen(toSub);
	if(toSub[len-1]== '\n')
	{
		toSub[len-1] = '\0';
	}
	strcat(toSub, "|");
	printf("Enter Body of Email:\n");
	fgets(toBody, sizeof(toBody),stdin);
	len = strlen(toBody);
	if(toBody[len-1]== '\n')
	{
		toBody[len-1] = '\0';
	}
	strcat(toBody,"|");
	strcat(toSub, toBody);
	strcat(toRecievers,toSub);
	len = strlen(toRecievers);
	if(toRecievers[len-1]== '\n')
	{
		toRecievers[len-1] = '\0';
	}
	strncpy(toEmail,toRecievers,len+1);	
	strcpy(mail_detail, toEmail);
	//pthread_mutex_unlock(&lock);
	return mail_detail;
}
int showInbox(char buff[LENGTH])
{
	char * token, temp[LENGTH], seq[2];
	int email_seq=0, k=0,i,n,flag;
	strcpy(temp,buff);
	token = strtok(temp,"#");
	while(token!= NULL)
	{
		email_seq++;
		token = strtok(NULL, "#");
	}
	printf("\n  Status   Username  firstname  lastname\tsubject                      time\n");
	printf("-------------------------------------------------------------------------------------------------\n");
	token = strtok(buff,"|");
	for(i=0;i<email_seq; i ++)
	{
		token = strtok(NULL,"|");
		printf("%d)%s",++k, token);
		token = strtok(NULL, "|");
		printf("%5s", token);
		token = strtok(NULL, "|");
		printf("%12s", token);
		token = strtok(NULL,"|");
		printf("%9s", token);
		token = strtok(NULL, "|");
		printf("%15s", token);
		token = strtok(NULL, "#");
		printf("%40s\n", token);	
	}
	do{
		printf("\nWhat do you want to do?");
		printf("\n1. Check Email");
		printf("\n2. Delete Email");
		printf("\n3. Filter Email");
		printf("\n4. Go Back");
		printf("\n Enter your choice:");
		fgets(seq,2,stdin);
		clean_stdin();
		n = atoi(seq);
		if(n<0 || n>4)
		{
			printf("\n Please choose valid option");
			flag = 1;
		}
		else
		{
			flag = 0;
		}
	}while(flag==1);
	//printf("\nreturning n value %d", n);
	return n;
}

int check_email(char buff[LENGTH])
{
	char * token, seq[2];
	int n , flag;
	token = strtok(buff,"|");
	token = strtok(NULL, "|");
	printf("Username  : %s\n", token);
	token = strtok(NULL, "|");
	printf("Firstname : %s\n", token);
	token = strtok(NULL, "|");
	printf("Lastname  : %s\n", token);
	token = strtok(NULL, "|");
	printf("Subject   : %s\n",  token);
	token = strtok(NULL, "|");
	printf("Body      : %s\n",  token);
	token = strtok(NULL, "|");
	printf("Time      : %s\n", token);
	do{
		printf("\n 1. Reply Email"); 
		printf("\n 2. Forward Email");
		//printf("\n3. Delete Email");
		printf("\n3. Go back");
		printf("\n Enter your choice:");
		fgets(seq,2,stdin);
		clean_stdin();
		n = atoi(seq);
		if(n<0 || n>4)
		{
			printf("\n Please choose valid option");
			flag = 1;
		}
		else
		{
			flag = 0;
		}
	}while(flag==1);
	return n ;
}

char * email_reply()
{
	char * email_reply_content = malloc(2000);
	char email_Body[1024];
	int lines,len;
	printf("\n Enter the content to reply :\n");
	fgets(email_Body,sizeof(email_Body), stdin);
	len = strlen(email_Body)-1;
	if(email_Body[len]== '\n')
	{
		email_Body[len] = '\0';	
	}
	strcpy(email_reply_content, "email_reply|");
	strcat(email_reply_content , email_Body);
	return email_reply_content;
}
char * email_forward()
{
	char * email_forward_content = malloc(200);
	char toRecievers[200];
	int len;
	printf("\nEnter usernames to send email (seperate users using ','):\n");
	fgets(toRecievers,sizeof(toRecievers),stdin);
	len = strlen(toRecievers);
	if(toRecievers[len-1]== '\n')
	{
		toRecievers[len-1] = '\0';
	}

	strcpy(email_forward_content , "email_forward|");
	strcat(email_forward_content, toRecievers);
	return email_forward_content;
}

char * filter_option()
{
	char choice[2], filterBy[50];
	int len,flag;
	char * filter_choice = malloc(5);
	do{
		printf("\nHow do you want to filer your inbox:");
		printf("\n 1. By Username:");
		printf("\n2. By Subject Content");
		printf("\n3. By Date");
		printf("\nEnter your choice:\n");
		fgets(choice,sizeof(choice),stdin);
		clean_stdin();
		len = atoi(choice);
		if(len<0 || len > 3)
		{
			printf("****Please select valid choice****");
			flag =1;
		}
		else 
		{
			flag = 0;			
		}
	}while(flag==1);
	strcpy(filter_choice,choice);
	if(len == 1)
	{
		printf("\n Filter By Username Option");
		printf("\n Enter username by which you want to search your mailbox:");
		fgets(filterBy, sizeof(filterBy), stdin);
		//clean_stdin();		
	}
		if(len == 2)
	{
		printf("\n Filter By Subject Content Option");
		printf("\n Enter subject content by which you want to filter email:");
		fgets(filterBy, sizeof(filterBy), stdin);
		//clean_stdin();		
	}
		if(len == 3)
	{
		printf("\n Filter By Date Option:");
		printf("\n Enter Date or Time by which you want to filter email(Date in Format eg. Mon DD , time in format eg. hh:mm:ss");
		fgets(filterBy, sizeof(filterBy), stdin);
		//clean_stdin();		
	}
	strncpy(filter_choice,choice, strlen(choice)-1);
	strcat(filter_choice,"|");
	strncat(filter_choice, filterBy, strlen(filterBy)-1);
	return filter_choice;
}

char * mail_filter(char buff[LENGTH])
{
	char * mail_filter_num = malloc(100);
	char * token, temp[LENGTH], choice[2];
	int email_seq=0, k=0,i,n,flag,seq;
	strcpy(temp,buff);
	token = strtok(temp,"#");
	while(token!= NULL)
	{
		email_seq++;
		token = strtok(NULL, "#");
	}

	token = strtok(buff,"|");
	token = strtok(NULL,"|");
	if(strcmp(token,"NOOOOO")==0)
	{
		printf("\n*** NO RESULT WITH THE SELECTED FILTER\n");
		strcpy(mail_filter_num,"NOOOOO");
	}
	else
	{	
		system("clear");
		printf("\n  Status   Username  firstname  lastname\tsubject                      time\n");
		printf("-------------------------------------------------------------------------------------------------\n");
		for(i=0;i<email_seq; i ++)
		{
			token = strtok(NULL,"|");
			printf("%d)%s",++k, token);
			token = strtok(NULL, "|");
			printf("%5s", token);
			token = strtok(NULL, "|");
			printf("%12s", token);
			token = strtok(NULL,"|");
			printf("%9s", token);
			token = strtok(NULL, "|");
			printf("%15s", token);
			token = strtok(NULL, "#");
			printf("%40s\n", token);	
		}
		do{
			printf("\n What do you want to do:");
			printf("\n 1. Check Email");
			printf("\n 2. Delete Email");
			printf("\n Enter your choice:");
			memset(choice, '\0', sizeof(choice));
			fgets(choice,sizeof(choice),stdin);
			clean_stdin();
			k = atoi(choice);
			if(k<0 || k>3)
			{
				printf("\n***Please select valid choice***\n");
				flag = 1;
			}
			else
			{
				flag = 0;
			}
		}while(flag==1);
		if(k == 1 )
		{
			printf("\nEnter a seq. no to Check Email:");
			memset(choice, '\0', sizeof(choice));
			fgets(choice,sizeof(choice),stdin);
			//clean_stdin();
			seq = atoi(choice);
			memset(mail_filter_num, '\0', 100);
			strcpy(mail_filter_num, "mail_filter_check|");
			strncat(mail_filter_num, choice, strlen(choice));
		}
		if(k == 2 )
		{
			printf("\nEnter a seq. no to Delete Email:");
			memset(choice, '\0', sizeof(choice));
			fgets(choice,sizeof(choice),stdin);
			//clean_stdin();
			seq = atoi(choice);
			memset(mail_filter_num, '\0', 100);
			strcpy(mail_filter_num, "mail_filter_delete|");	
			strncat(mail_filter_num, choice, strlen(choice));
		}	
	}
	return mail_filter_num;
}
int filter_check_email(char buff[LENGTH])
{
	char * token, seq[2];
	int n , flag;
	token = strtok(buff,"|");
	token = strtok(NULL, "|");
	printf("Username  : %s\n", token);
	token = strtok(NULL, "|");
	printf("Firstname : %s\n", token);
	token = strtok(NULL, "|");
	printf("Lastname  : %s\n", token);
	token = strtok(NULL, "|");
	printf("Subject   : %s\n",  token);
	token = strtok(NULL, "|");
	printf("Body      : %s\n",  token);
	token = strtok(NULL, "|");
	printf("Time      : %s\n", token);
	do{
		printf("\n 1. Reply Email"); 
		printf("\n 2. Forward Email");
		//printf("\n3. Delete Email");
		printf("\n3. Go back");
		printf("\n Enter your choice:");
		memset(seq,'\0', 2);
		clean_stdin();
		fgets(seq,2,stdin);
		clean_stdin();
		/*if(seq[strlen(seq)-1] == '\n');
			seq[strlen(seq)-1] = '\0';*/
		printf("======>seq:%s", seq);
		n = atoi(seq);
		if(n<0 || n>4)
		{
			printf("\n Please choose valid option");
			flag = 1;
		}
		else
		{
			flag = 0;
		}
	}while(flag==1);
	printf("---> n = %d", n);
	return n ;
}


void *backgroundthread()
{
	char send[LENGTH], recc[LENGTH], recc1[LENGTH];
	int n;
	char * token;
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		printf("\n ERROR IN CREATING CLIENT SOCKET\n");
		exit(2);
	}
	if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("\n ERROR IN CONNECTING\n");
		exit(3);
	}
	//printf("HERE");
	sleep(1);
	int new_emails, new_cal;
	pthread_mutex_lock(&lock);
	if(logout_service == 1)
	{
		pthread_mutex_unlock(&lock);
		exit(0);
	}
	memset(send, '\0', sizeof(send));
	strcpy(send, "thread");
	strcat(send,"|");
	strcat(send, current_user);
	//printf("send :%s\n", send);
	 n = write(sock,send,sizeof(send));
	//sleep(1);
	memset(recc, '\0', sizeof(recc));
	n = read(sock , recc , sizeof(recc));
	//new_emails = totalEmailsLatest();
	//new_cal = totalCalReq();
	strcpy(recc1, recc);
	token =strtok(recc1, "|");
	if(token == NULL)
	{	
		printf("exiting here\n");
		pthread_mutex_unlock(&lock);
		return;
	}
	strcpy(recc1, token);
	if(strcmp(recc1,"new_eF") == 0)
	{
				token = strtok(recc, "|");
				token = strtok(NULL, "|");
				new_emails = atoi(token);
				token = strtok(NULL, "|");
				new_cal = atoi(token);
	}
	//printf("==============");
	if(new_emails > total_email_count)
	{
		printf("\n\nNOTIFICATION : YOU HAVE NEW EMAILS IN YOUR MAIL BOX \n\n");
		//usleep(500);
		total_email_count = new_emails; 
	}
	if(new_cal > total_cal_count)
	{
		printf("\n\nNOTIFICATION : YOU HAVE NEW CALENDAR REQUEST IN YOUR MAIL BOX \n\n");
		//usleep(500);
		total_cal_count = new_cal; 
	
	}

	 pthread_mutex_unlock(&lock);
	//printf("++++++++++++++++++++++++++");
}


