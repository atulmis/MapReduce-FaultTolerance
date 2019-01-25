
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define ECHOMAX 255     /* Longest string to echo */

const int MAX_WORDS_PER_FILE = 5; // maximum words for split

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

	char* getSplitFilename(int count)
	 {
		char *splitName = "split_";
		char splitCount = '0' + count;
		char *fExt = ".txt";
		char *newName = (char *) malloc(strlen(splitName) + 2 + strlen(fExt));   //one for extra char, one for trailing zero
		
		strcpy(newName, splitName);
 		newName[strlen(splitName)] = splitCount;
 		newName[strlen(splitName)+1] = '\0';
		strcat(newName, fExt);
		
		return newName;

	}

	int splitfile()
	{
    		FILE *fpIn;
    		char buf[1000];

    		fpIn =fopen("input.txt","r");
    		if (!fpIn)
        		return 1;

		 int splitCount = 1;   
	//Prepare split file handle
		 FILE *fpOut;
                char *fNameSplit = getSplitFilename(splitCount);
                fpOut = fopen(fNameSplit, "w+");
    
                char temp;
                char *string = malloc(sizeof(char));
                int charCount = 0;
                int wordCount = 0;

	 while ((temp = fgetc(fpIn)) != EOF)
	{
		 if (temp == '\n' || temp == ' ' || temp == '?' || temp == ',' || temp == '.' || temp == '\"' ||
		 temp == '!' || temp == '@' || temp == '~' || temp == '#' || temp == '$' || temp == '%' ||
		 temp == '%' || temp == '^' || temp == '&' || temp == '*' || temp == '(' || temp == ')' ||
		 temp == '-' || temp == '_' || temp == '{' || temp == '}' || temp == '[' || temp == ']' ||
		 temp == '<' || temp == '>' || temp == '\r' || temp == '/' || temp == '\'' || temp == '`'
		 ){
			 if (charCount > 0) 
			{
				 string[strlen(string)] = '\0';
				 printf("[%s]-->\n",string);
				 wordCount ++;
				
				fprintf(fpOut, "%s\n", string);
				if (wordCount % MAX_WORDS_PER_FILE == 0) 
				{
					splitCount++;
				//close the file handle and re-open to next split file
		                    fclose(fpOut);
		                    free(fNameSplit);
                		    fNameSplit = getSplitFilename(splitCount);
				fpOut = fopen(fNameSplit, "w+");


				}

		        charCount = 0;
                	free(string);
			string = calloc(0, sizeof(char));
			}
	 }
		else
		{
			 if (charCount == 0)
			{
				free(string);
				string = calloc(0, sizeof(char));
			}

			string[charCount] = temp;
			string[strlen(string)] = '\0';     //add the null-termination character '\0'
			 charCount ++;		
		}
	
      }
           free(string);
 	   free(fNameSplit);
           fclose(fpIn);
           fclose(fpOut);

	return(splitCount);

	}

int main(int argc, char *argv[])
{
    int sock;                        /* Socket */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int cliAddrLen;         /* Length of incoming message */
    char echoBuffer[ECHOMAX];        /* Buffer for echo string */
    unsigned short echoServPort;     /* Server port */
    int recvMsgSize;                 /* Size of received message */

   int mapTasksCount = 0;     //number of map tasks to be allocated across the mappers

    if (argc != 2)         /* Test for correct number of parameters */
    {
        fprintf(stderr,"Usage:  %s <UDP SERVER PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");

  
	mapTasksCount = splitfile();
	printf("\nMap Tasks Count: %d\n", mapTasksCount);

	int i;

 
    for (;;) // Run forever 
    {
        // Set the size of the in-out parameter
        cliAddrLen = sizeof(echoClntAddr);

        // Block until receive message from a client 
        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
            DieWithError("recvfrom() failed");

	echoBuffer[ recvMsgSize ] = '\0';

        printf("Server handling client %s\n", inet_ntoa( echoClntAddr.sin_addr ));
        printf("Server receives string: %s\n", echoBuffer );

	if(strcmp(echoBuffer,"Map")==0)
	{
			
		sprintf(echoBuffer,"%d", mapTasksCount);		

	if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != strlen(echoBuffer))
            DieWithError("sendto() sent a different number of bytes than expected");

	}

	if(strcmp(echoBuffer,"Reduce")==0)
	{
			
		sprintf(echoBuffer,"%d", mapTasksCount);		

	if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != strlen(echoBuffer))
            DieWithError("sendto() sent a different number of bytes than expected");

	}

}
}
