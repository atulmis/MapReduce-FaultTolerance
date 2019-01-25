
#include <time.h>
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define ECHOMAX 255     /* Longest string to echo */

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

	char* getMappedFilename(int count)
	 {
		char *mapName = "mapped_";
		char splitCount = '0' + count;
		char *fExt = ".txt";
		char *newName = (char *) malloc(strlen(mapName) + 2 + strlen(fExt));   //one for extra char, one for trailing zero
		
		strcpy(newName, mapName);
 		newName[strlen(mapName)] = splitCount;
 		newName[strlen(mapName)+1] = '\0';
		strcat(newName, fExt);
		
		return newName;

	}


void analyzeWordsCount(char *fName, int count) 
{
    FILE *fpIn;
    fpIn = fopen(fName, "r");
    
    char temp;
    int charCount = 0;
    char *string = calloc(0, sizeof(char));
    
    char *prevString = calloc(0, sizeof(char));
    int matchCount = 1;    //initialize the matchCount to 1 cause this is basic count value for each word exist in the input file

    FILE *fpOut;
    char *fNameCombinedOutput = getMappedFilename(count);
    fpOut = fopen(fNameCombinedOutput, "w+");
    
    while ((temp = fgetc(fpIn)) != EOF) 
	{
	if (temp == '\n') 
	{
	 if (charCount > 0) 
	  {
		charCount = 0;
		//do the comparison here
		if (strlen(prevString) <= 0)    //the first record, prevString is empty 
		{
		string[strlen(string)] = '\0';   //add the null-termination character '\0'
		printf("the first record [%s]-->\n",string);
		
		//copy the the current string to previous string, destroy the current string
		strcpy(prevString,string);
		prevString[strlen(prevString)] = '\0';    //add the null-termination character '\0'
		}
		else
		{
		/* compare the current string with previous string
 		*  if matches, increment the match count by 1, then proceed next iteration
 		*  if not matches, write the output associated match count to output file, then reset match count to 1 */
		
		if (strcmp(prevString, string) != 0)    // No match is found
		  {
			prevString[strlen(prevString)] = '\0';    //add the null-termination character '\0'
			printf("no more match found for [%s] --> %d\n", prevString, matchCount);

                	fprintf(fpOut, "%s,%d\n", prevString, matchCount);
                      
                	matchCount = 1;
                        
			string[strlen(string)] = '\0';               //add the null-termination character '\0'
			strcpy(prevString,string);
			prevString[strlen(prevString)] = '\0';      // add the null termination character '\0'

		   } else 
		   {
                        matchCount++;
                        printf("a previous match is found for [%s]-->\n",string);
                   }
		}		

		free(string);
                string = calloc(0, sizeof(char));
           }
	}
	else 
	{
            if (charCount == 0) {
                free(string);
                string = calloc(0, sizeof(char));
            }
            string[charCount] = temp;
            charCount ++;
        }

	}   //while loop closes


	/* Avoid the last record being omitted, just use the matchCount to determine a matching or not  */
	if (matchCount <= 1)    //no match is found
	{
		 printf("no more match found for [%s] --> %d\n", prevString, matchCount);
	        fprintf(fpOut, "%s,%d\n", prevString, matchCount);
	}
	else 
	{
        printf("a previous match is found for [%s]-->\n",string);
        fprintf(fpOut, "%s,%d\n", string, matchCount);
	}

    free(string);
    free(prevString);
    free(fNameCombinedOutput);
    fclose(fpIn);
    fclose(fpOut);
}


void mergeWordsCount(char *fName, int count) 
{
    FILE *fpIn, *fpOut;
    fpOut = fopen(fName, "w+");
    
    char temp;
    int charCount = 0;
    char *string = calloc(0, sizeof(char));
  
    const char *fNameMapped[] = {"mapped_1.txt","mapped2.txt","mapped_3.txt"};
  
    char *prevString = calloc(0, sizeof(char));
    int matchCount = 1;     //initialize the matchCount to 1 cause this is basic count value for each word exist in the input file

    int i;
    int mapCount=1;
    	for(i=0; i<count; i++)
	{
		
		fpIn = fopen(fNameMapped[i], "r");
		
	while ((temp = fgetc(fpIn)) != EOF) 
	{
	if (temp == '\n') 
	{
	 if (charCount > 0) 
	  {
		charCount = 0;
		//do the comparison here
		if (strlen(prevString) <= 0)    //the first record, prevString is empty 
		{
		string[strlen(string)] = '\0';   //add the null-termination character '\0'
		printf("the first record [%s]-->\n",string);
		//copy the the current string to previous string, destroy the current string
		strcpy(prevString,string);
		prevString[strlen(prevString)] = '\0';    //add the null-termination character '\0'
		}
		else
		{
		/* compare the current string with previous string
 		*  if matches, increment the match count by 1, then proceed next iteration
 		*  if not matches, write the output associated match count to output file, then reset match count to 1 */
		
		  if (strcmp(prevString, string) != 0)    // No match is found
		  {
			prevString[strlen(prevString)] = '\0';    //add the null-termination character '\0'
			printf("no more match found for [%s] --> %d\n", prevString, matchCount);

                	fprintf(fpOut, "%s,%d\n", prevString, matchCount);
                      
                	matchCount = 1;
                        
			string[strlen(string)] = '\0';               //add the null-termination character '\0'
			strcpy(prevString,string);
			prevString[strlen(prevString)] = '\0';      // add the null termination character '\0'
		  } 
		  else 
		  {
                        matchCount++;
                        printf("a previous match is found for [%s]-->\n",string);
                  }
		}		

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
            charCount ++;
        }

	}      //while loop closes  
 
	free(string);
        free(prevString);
        fclose(fpIn);
	fclose(fpOut);
		
	mapCount++;

	}
	
}
	

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned short echoServPort;     /* Echo server port */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char *servIP;                    /* IP address of server */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[ECHOMAX+1];      /* Buffer for receiving echoed string */
    int echoStringLen;               /* Length of string to echo */
    int respStringLen;               /* Length of received response */
    int i, mapTasksCount;
    int failFlag=0;
    char *workerJob;		     /* Map (or) Reduce */
    char *mode;                   /* Re-execution or Replication */

    if (argc < 5)    /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage: %s <Server IP address> <Echo Port>\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];           /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]);  /* Second arg: Use given port, if any */
    workerJob = argv[3];
    mode = argv[4];
	printf( "Arguments passed: server IP %s, port %d\n", servIP, echoServPort );
        printf( "Worker identified as %s and running in %s mode\n\n", workerJob, mode );
        
    /* Create a datagram/UDP socket */

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */

    memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
    echoServAddr.sin_port   = htons(echoServPort);     /* Server port */


		strcpy(echoBuffer, workerJob);
    	/* Send the struct to the server */
    	if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))
       		DieWithError("sendto() sent a different number of bytes than expected");
  
	 	printf( "Client identified as %s and sent to server\n", echoBuffer );	

    	/* Recv a response */

    	fromSize = sizeof(fromAddr);

    	if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &fromAddr, &fromSize)) > ECHOMAX )
        	DieWithError("recvfrom() failed");

		echoBuffer[ respStringLen] = '\0';

   		if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
   		{
       		fprintf(stderr,"Error: received a packet from unknown source.\n");
       		exit(1);
   		}
		
		mapTasksCount = atoi(echoBuffer);

	if ( strcmp(workerJob,"Map")==0 )    //worker is Mapper1
	{
		printf("Client received mapTasksCount from server: %d\n\n", mapTasksCount );    

		if ( strcmp(mode,"Re-execution")==0 )           //Re-execution mode of operation for Map1
		{
			printf("Map1 operating in Re-execution mode. \n");

			int splitCount=1;
			for(i=0; i<(mapTasksCount/2); i++)
			{
				char *fNameSplit = getSplitFilename(splitCount);
				analyzeWordsCount(fNameSplit,splitCount);
				splitCount++;
			}

		}

		srand(time(0));
		int r = rand() % 100;                        /*Random Number Generation*/
//		int r = 61;                             /* to debug failure scenarios */
                printf( "Random number:%d\n", r );

                if(r<=60) 	
                {
			printf("Mapper alive. Go ahead.\n");
              	}
 		else
		 {
			printf("Mapper dead. STOP.\n");
			failFlag=1;
		 }	

	if (strcmp(mode,"Replication")==0) 	       //Replication mode of operation for Map1
	{
		printf("Map1 operating in Replication mode. \n ");	

		if (failFlag==0)
		{
			printf("Map1 task executing from Primary.\n");

			int splitCount=1;
			for(i=0; i<(mapTasksCount/2); i++)
			{	
			char *fNameSplit = getSplitFilename(splitCount);
			analyzeWordsCount(fNameSplit,splitCount);
			splitCount++;
			}

		}

		if (failFlag==1)
		{
			printf("Map1 task executing from Backup.\n");

			int splitCount=1;
			for(i=0; i<(mapTasksCount/2); i++)
			{
			char *fNameSplit = getSplitFilename(splitCount);
			analyzeWordsCount(fNameSplit,splitCount);
			splitCount++;
			}
			failFlag = 0;       // Backup recovers the failure
		printf("Backup recovered failure.\n");
		}

	}

	printf("***Map1 job complete***\n\n");
	}

	if ( strcmp(workerJob,"Map")==0 )    //worker is Mapper2
	{

	if (failFlag==1)
		{
			int splitCount=1;
			for(i=0; i<(mapTasksCount); i++)
			{
		
			char *fNameSplit = getSplitFilename(splitCount);
			analyzeWordsCount(fNameSplit,splitCount);
			splitCount++;

			}

		}

		else 
		{
			int splitCount= (mapTasksCount/2)+1 ;
			for(i=(mapTasksCount/2); i<mapTasksCount; i++)
			{
		
			char *fNameSplit = getSplitFilename(splitCount);
			analyzeWordsCount(fNameSplit,splitCount);
			splitCount++;
			}
		}

	printf("***Map2 job complete***\n\n");
	}

	if ( strcmp(workerJob,"Reduce")==0 )    //worker is Reducer
	{
		printf("Client received reduceTasksCount from server: %d\n\n", mapTasksCount);

		//generate final output file
		char *reduceName = "reduced_output";
		char *fExt = ".txt";
		char *newName = (char *) malloc(strlen(reduceName) + 1 + strlen(fExt));   //one for trailing zero
		strcpy(newName, reduceName);
		newName[strlen(reduceName)] = '\0';
		strcat(newName, fExt);
	
		mergeWordsCount(newName,mapTasksCount);
	printf("***Reduce job complete***");
	
	}


    close(sock);
    exit(0);
}
