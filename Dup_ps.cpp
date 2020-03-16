using namespace std;
class Process
{

  public:
  int socket_file_discripter;/*Socket discriptor for process */

  thread receive_t; /* Each process should maintain two threads. one for local clock and another for receive*/

  int pid; /*Process identifier. Which will be initialized when the process constructor is created.*/

  bool flag = false; /*Used for wait*/

  char color[10],tokenColor[15];

  int C,D,parentNode;
  int minMsg;

  bool hasToken,isLeaf;
  int no_of_children;
  bool reported;


  Process(int id)
      {
          //localclock_t =  thread(&localclock :: IncrementDriftFactor,local_clock);
          receive_t =  thread(&Process :: receive,this);
          pid = id;/*Assign process id to pid variable of the process class*/
      		strcpy(color,"white");
      		parentNode=-1;
          hasToken=false;
          isLeaf=false;
          no_of_children=0;
          reported=false;
          strcpy(tokenColor,"white");
      		receive_t.detach();
      }
      ~Process()
          {
		      close(socket_file_discripter);

	       }


      void send(int source,int destination,char *message)
      {


                        float ff=sleepTime(0.8);
                        sleep(ff);
                        char *tt=timeTaken();
                      /*  struct sockaddr_in peeraddr;  /*Now socket address to the peer to whome the process want to send the message*/
                      /*  peeraddr.sin_family    = AF_INET; /*We use IPV4 protocol*/
                        /*peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local host address here*/
/*

                        if(!strncmp(message,"blue",4))
                        {
                          strcpy(color,"blue");
                          int ff=0;
                          if(hasToken&&(no_of_children==0))
                          {
                          //  printf(" In send function we are trying to send to its parent %d is pid and %d is parentNode\n",pid+1,parentNode+1 );
                            int parent=parentNode;
                            peeraddr.sin_port = htons(parent+DIFF_PORTS);
                            char colorD[10];
                            strcpy(colorD,tokenColor);
                            sprintf(message,"%s ",tokenColor);
                            hasToken=false;
                            reported=true;
                            strcpy(tokenColor,"white");
                            if(pid!=0)
                            {
                            printf("**-- Cell %d sends %s token to Cell %d at %s\n\n",pid+1,colorD,parent+1,tt );
                            strcpy(color,"blue");
                            sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));
                            }
                            else{

                              strcpy(message,"blue");
                              sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));

                            }

                          }
                          else
                          {
                            printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,"blue",destination+1,tt );
                            sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));

                            sleep(1);
                          }
                        }
                        else if(!strncmp(message,"red",3))
                        {
                          if(reported==false){
                          printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,"red",destination+1,tt );
                          peeraddr.sin_port = htons(destination+DIFF_PORTS);
                          sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));
                          }
                          sleep(1);
                        }
                        else
                        {
                          std::cout << " TOken color in send function is %s "<<message << '\n';
                          if(hasToken&&(no_of_children==0))
                          {


                            if(pid==0)
                            {
                                printf("Root node has colro %s and token COlor %s and no.of remo children %d\n",color,tokenColor,no_of_children );
                                if(no_of_children==0) std::cout << " Naveen Kumar kammari call here termination function" << '\n';
                                termination_detected=true;
                            }
                          //  printf(" In send function we are trying to send to its parent %d is pid and %d is parentNode\n",pid+1,parentNode+1 );
                            else{
                            int parent=parentNode;
                            peeraddr.sin_port = htons(parent+DIFF_PORTS);
                            char colorD[10];
                            strcpy(colorD,tokenColor);
                            sprintf(message,"%s ",tokenColor);
                            hasToken=false;
                            reported=true;
                            strcpy(tokenColor,"white");
                            printf("**-- Cell %d sends %s token to Cell %d at %s\n\n",pid+1,colorD,parent+1,tt );
                            sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));
                          }
                        }
                        else{
                          strcpy(message,"blue");
                          sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));
                        }

                        }


                        //sleep(2);

                        */
                        struct sockaddr_in peeraddr;  /*Now socket address to the peer to whome the process want to send the message*/
                        peeraddr.sin_family    = AF_INET; /*We use IPV4 protocol*/
                        peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local host address here*/
                        peeraddr.sin_port = htons(destination+DIFF_PORTS);
                        printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,message,destination+1,tt );
                        sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));
                        if(delayTime&&d_sent==4)
                        {

                          delayTime=false;
                          d_sent=12;
                          chooseRandomBlueProcesses();
                          start_time=clock();

                        }
                        d_sent+=1;
      }



 void receive()
 {
      //if(delayTime) sleep(5);
      while(!termination_detected)
      {
        char *message_received = new char[MAX_BUFFER];  /*Declaring message receive buffer*/
        unsigned int length, n,round_number; /*Variables declaration*/
        char garbage[8];

        struct sockaddr_in peeraddr;  /*socket addresses for  peer*/

        length = sizeof(peeraddr);  /*Storing the size of peeradd in length*/
        peeraddr.sin_family    = AF_INET; /*Using IPV4*/
        peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /*Local host address*/
        n = recvfrom(socket_file_discripter, (char *)message_received, MAX_BUFFER,MSG_WAITALL, ( struct sockaddr *) &peeraddr,&length);
        message_received[n] = '\0';  /*Appending null character at the end of the message*/

        if(n<0) continue;

        if(!strncmp(message_received,"red",3))
        {
            //strcpy(color,"red");
            //
        }
        else if(!strncmp(message_received,"blue",4))
        {
            if(no_of_children==0){
              if(strcmp(color,"red")==0)
              {
                  if(pid==0)
                  {
                        //restart the process
                        restart();
                  }
                  else
                  {
                    char token[10];
                    strcpy(token,"black");
                    send(pid,htons(parentNode+DIFF_PORTS),token);
		                reported = true;
                  }
              }
              else{ // This cell is blue/white and recieved blue
		                if(!reported){
		                    if(parentNode== -1){
		                        termination_detected= true;
		                        continue;
		                    }
		                    send(pid,htons(parentNode+DIFF_PORTS),tokenColor);
                        strcpy(tokenColor,"white");
		                    reported = true;
		                 }
		            }
            }
              strcpy(color,"blue");
        }
        else if(!strncmp(message_received,"reset",5))
        {
                restart();
                int i;
                if(strcmp(color,"blue")==0)
                {
                  char token[10];
                  strcpy(token,"white");
                  send(pid,htons(parentNode+DIFF_PORTS),token);
                }
                char resetToken[10];
                strcpy(resetToken,"reset");
                for(i=0;i<spanningTree.size();i++){
                  printf(" Process %d sends reset token to Cell %d\n",pid+1,spanningTree[pid][i]);
                  send(pid,htons(spanningTree[pid][i]-1+DIFF_PORTS),resetToken);
                }


        }
        else
        {

          no_of_children--;

			    if(strncmp(message_received,"black",5)) strcpy(tokenColor,"black"); //Recieved black_token from child.. Set blackchild flag

			    if(no_of_children==0&& (strcpy(color,"blue")==0)){ // all child are terminated and it is terminated
			        if(parentNode == -1) //This is a root cell
			            if(strcmp(tokenColor,"black")==0) restart(); //If root has black_token , then restart termination detection
			            else termination_detected = true; //Announce Termination
			        else{ //It is internal cell

			            send(pid,htons(parentNode+DIFF_PORTS),tokenColor);
                  strcpy(tokenColor,"white");
			            reported = true;
			        }
			    }

        }




      }
    //  sleep(2);
    //  if(checkTTD()) exit(0);

 }

 void restart(){
        printf("TD restarted at %d \n",pid+1);
        strcpy(tokenColor,"white");
        no_of_children=spanningTree[pid].size()-1;
        reported = false;

        int i;
        char reset[10];
        strcpy(reset,"reset");
        for(i=1;i<spanningTree[pid].size();i++)
        {
                send(pid,htons(spanningTree[pid][i]-1+DIFF_PORTS),reset);
        }

    }

void showRedMessage(int s,int d)
{
  char *tt=timeTaken();
  printf("Cell %d receives red message from Cell %d at %s\n\n",s+1,d+1,tt );
  tt=timeTaken();
  printf("Cell %d turns  red at %s\n\n",s+1,tt);


}
void showBlueMessage(int s,int d)
{
  char *tt=timeTaken();
  printf("Cell %d receives blue message from Cell %d at %s\n\n",s+1,d+1,tt );
  tt=timeTaken();
  printf("Cell %d turns  blue at %s\n\n",s+1,tt);
}
 void process_Synchronization_Barrier()
	{
		pthread_mutex_lock(&locker);/*all the processes should bind to socket before they start communicating each other*/
		process_done ++; /*Each process increments it if they bind the socket*/

		if(process_done == n) /*If all the processes are done with binding of the sockets then we release the lock and wake up the threads*/
		{
			for(int i=1;i<=process_done;i++) /*Here waking up the threads*/
				pthread_cond_signal(&conditioned_thread);  /*This concept is used from operating systems subject for synchronization*/
		}

		else /*If not all processes completed the binding then wait untill they do */
		{
		pthread_cond_wait(&conditioned_thread, &locker);  /*Waiting */
		}

		pthread_mutex_unlock(&locker);/*Do this after all processes completes the binding of socekts*/
		process_done = 0;
	}


  void showDC()
  {
    printf("Process %d has C=%d and D=%d values\n",pid+1,C,D );
  }
  int socket_create()
	{
		int  PORT = DIFF_PORTS+ pid; /* Making port number with process id. Here DIFF_PORTS=35000 as predefined */

  //  printf("Process %d is Port with %d\n",pid+1,PORT );
    struct sockaddr_in myaddr; /* Socket address variable*/


		/*
			In below if condition we used socket() function to create socket and the returned discriptor is stored in the
			socket_file_discripter. If socket() function returns the value less than 0 then socket is not created. i.e error
			occured while creating the socket.
		*/
		if ( (socket_file_discripter = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 )
		{
			printf("Socket is not created for process %d \n",pid+1);
			return -1;
		}



		/*
			After creating the socket we need to bind it with address and port number to get identified.
			Below snippet of code is doing this task.
		*/
		myaddr.sin_family    = AF_INET; // IPv4
		myaddr.sin_addr.s_addr = INADDR_ANY; /*Addressss*/
		myaddr.sin_port = htons(PORT);  /*Port number*/


		/*
			We use bind() function to bind the address and port number
		*/
		int status = bind(socket_file_discripter, (const struct sockaddr *)&myaddr,sizeof(myaddr))	;

		if (status < 0 ) /*If returned value from the bind() is negative then we can conclude that error occured during the binding process*/
		{
			printf("Binding error id occurred for process %d \n", myaddr.sin_port-DIFF_PORTS);
			perror("bind failed");
			exit(EXIT_FAILURE);
			return -1; //failed to associate particular port number with this process....
		}

		/*
			If we successfully bind the address then we can declare that socket creation for the process is completed successfully.
		*/
	if(pid!=n)	printf("Socket Created Successfully for process %d \n", pid+1);;
		//cout<<color<<endl;

		return 1;
	}

};
