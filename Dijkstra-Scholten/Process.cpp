
/*
	K. Naveen Kumar
	CS19MTECH11009
	DC Programming Assignment-I
	IIT Hyderabad
*/


/*
  The process class will have send and receive function and other functions that need to be used for computation
  Each process will have all function of Process , which will be executed using threads for each process.

*/
using namespace std;
class Process
{

  public:
  int socket_file_discripter;/*Socket discriptor for process */

  thread receive_t; /* Each process should maintain two threads. one for local clock and another for receive*/

  int pid; /*Process identifier. Which will be initialized when the process constructor is created.*/

  bool flag = false; /*Used for wait*/

  char color[10];

  int C,D,parentNode;
  int minMsg;


  Process(int id)
      {

          receive_t =  thread(&Process :: receive,this); /*Thread for receive function */
          pid = id;/*Assign process id to pid variable of the process class*/
      		strcpy(color,"white"); // Initial color of process

          //Below code shows initial Assignment of each process
      		C=0;D=0;
      		parentNode=id;
          minMsg=2;


      		/* Detaches the thread represented by the object from the calling thread, allowing them to execute independently from each other*/
      		receive_t.detach();
      }


      /*Send function is used to send messages between processes. This function take source, destination and messages
      as arguments and sends the message to destination process using sockets.

      */
      void send(int source,int destination,char *message)
      {
            if(source<n+1&&destination<n+1)
            {
            struct sockaddr_in peeraddr;  /*Now socket address to the peer to whome the process want to send the message*/
            peeraddr.sin_family    = AF_INET; /*We use IPV4 protocol*/
            peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local host address here*/
            peeraddr.sin_port = htons(destination+DIFF_PORTS); /*Port for destination process*/

            char *tt=timeTaken();

            if(!strncmp(message,"ack",3)){printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,"ack",destination+1,tt );}
            else printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,color,destination+1,tt );
            sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));


            if(delayTime&&d_sent==4) // We woke up blue messages after some time. If this is true then we wake up blue processes
            {

              delayTime=false;
              d_sent=12;
              chooseRandomBlueProcesses(); //Choosing random blue processes
              start_time=clock();

            }
            d_sent+=1;
          }
      }


/*
  This function is responsible for the receiving a message from the sender.
  We perform some computations after receiving the message.

*/
 void receive()
 {
      while(true)
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

        float o=sleepTime(1.5);
        sleep(o);

        if(!strncmp(message_received,"red",3)) // If received message is red
        {
          float tq=sleepTime(Wr);
          sleep(tq*2);
            if(C>0) // This condition is used to send signal message to neighbours
            {
                  strcpy(color,"red"); // We send red message to neighbhours only if C>0
                  char *ss=timeTaken();
                  printf("Cell %d receives red message from Cell %d at %s\n\n",pid+1,ntohs(peeraddr.sin_port)-DIFF_PORTS+1,ss);
                  printf("Cell %d turns red at %s\n\n",pid+1,ss);
                  if(pid!=process_init) redNeighbours(pid);
            }
            else if(C==0&&D==0) // If new node receives red message then we change its color and C
            {

                int pd=ntohs(peeraddr.sin_port)-DIFF_PORTS;
                if(!chechParentNode(pd))
                {
                strcpy(color,"red");
                C=1;
                D=0;
                parentNode=ntohs(peeraddr.sin_port)-DIFF_PORTS;
                showRedMessage(pid,ntohs(peeraddr.sin_port)-DIFF_PORTS); // Displays message received at which time
                makeProcessD(ntohs(peeraddr.sin_port)-DIFF_PORTS); //Increments sender's D
                redNeighbours(pid); //forward red message to neighours
              }
            }
        }
        else if(!strncmp(message_received,"blue",4)) // If received message is blue
        {

          strcpy(color,"blue");
          if(pid==process_init) //If initiator receive blue message
          {


            decreaseInitiator(); //We decrement initiators D
            *DD=*DD-1;
            continue;
          }
            if(C==0&&D==0) //If node with C==0 and D==0 receives blue msg, it just forwards it to neighbhours
            {
                C=0;
                D=0;
                parentNode=pid;

                strcpy(color,"blue");
                showBlueMessage(pid,ntohs(peeraddr.sin_port)-DIFF_PORTS);
                if(minMsg!=0) blueNeighbours(pid);
            }
            else if(C==1&&D==0)// If node's C==1 and D==0 receive blue msg then it should send sck msg to its parent
            {
                *number_of_messages+=1;
                strcpy(color,"blue");
                char message[10];
                strcpy(message,"ack");

                if (chechParentNode(pid)) continue;
                int dd=parentNode;
                parentNode=pid;
                C=0;
                D=0;

                struct sockaddr_in peeraddr;  /*Now socket address to the peer to whome the process want to send the message*/
                peeraddr.sin_family    = AF_INET; /*We use IPV4 protocol*/
                peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local host address here*/
                peeraddr.sin_port = htons(dd+DIFF_PORTS);
                char *tt=timeTaken();
                if(!strncmp(message,"ack",3)) printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,"ack",dd+1,tt );
                else printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,color,dd+1,tt );
                sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));
                //Sending ack msg to parent here
            }
            else
            {
                  //Spread this message to it's neighbours
                  strcpy(color,"blue");
                  blueNeighbours(pid);
            }

        }
        else if(!strncmp(message_received,"ack",3)) //If process receives ack msg
        {

              strcpy(color,"blue");
              if(pid==process_init) //If process is initiator
              {

                decreaseInitiator(); //Decrements D of initiator
                *DD=*DD-1;

                continue;
              }
              if(C==1&&D>0) //We just decrese the value of D by 1, if we have this condition
              {
                printf("Cell %d gets blue message from Cell %d\n\n",pid+1,ntohs(peeraddr.sin_port)-DIFF_PORTS+1 );
                 D=D-1;


                if(D==0) //After decreaseing the D value if it becomes 0 then we send ack msg to its parent
                {
                  *number_of_messages+=1;
                  char message[10];
                  strcpy(message,"ack");

                  int dd=parentNode; //we send ack msg to parent
                  parentNode=pid;
                  C=0;
                  D=0;
                  struct sockaddr_in peeraddr;  /*Now socket address to the peer to whome the process want to send the message*/
                  peeraddr.sin_family    = AF_INET; /*We use IPV4 protocol*/
                  peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local host address here*/
                  peeraddr.sin_port = htons(dd+DIFF_PORTS);
                  char *tt=timeTaken();
                  if(!strncmp(message,"ack",3)) printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,"blue",dd+1,tt );
                  else printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,color,dd+1,tt );
                  sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));


                }
                else blueNeighbours(pid);
              }
              else if(D==0)
              {
                  if(C==0){ //If D==0 and C==0 we just forward blue msg
                  parentNode=pid;
                  strcpy(color,"blue");
                  showBlueMessage(pid,ntohs(peeraddr.sin_port)-DIFF_PORTS);

                  if(minMsg!=0) blueNeighbours(pid);
                  }
                  else if(C==1) //D==0 and C==1 then send ack msg to its parent
                  {
                      *number_of_messages+=1;
                      int dd=parentNode;
                      parentNode=pid;

                      C=0;
                      D=0;
                      strcpy(color,"blue");
                      char message[100];
                      strcpy(message,"ack");
                      //char *ttt=timeTaken();
                      struct sockaddr_in peeraddr;  /*Now socket address to the peer to whome the process want to send the message*/
                      peeraddr.sin_family    = AF_INET; /*We use IPV4 protocol*/
                      peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local host address here*/
                      peeraddr.sin_port = htons(dd+DIFF_PORTS);
                      char *tt=timeTaken();
                      if(!strncmp(message,"ack",3)) printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,"blue",dd+1,tt );
                      else printf("Cell %d sends %s message to Cell %d at %s\n\n",pid+1,color,dd+1,tt );
                      sendto(socket_file_discripter, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &peeraddr,sizeof(peeraddr));

                  }

              }

        }
        else
        {

          printf(" Other message than blue , red and ack\n");

        }




      }

      if(checkTTD()) exit(0);

 }


/*
  This function displays a message which says receiving of red message and process turns to red

*/
void showRedMessage(int s,int d)
{
  char *tt=timeTaken();
  printf("Cell %d receives red message from Cell %d at %s\n\n",s+1,d+1,tt );
  tt=timeTaken();
  printf("Cell %d turns  red at %s\n\n",s+1,tt);


}

/*
  This function displays a message which says receiving of blue message and process turns to blue

*/
void showBlueMessage(int s,int d)
{
  char *tt=timeTaken();
  printf("Cell %d receives blue message from Cell %d at %s\n\n",s+1,d+1,tt );
  tt=timeTaken();
  printf("Cell %d turns  blue at %s\n\n",s+1,tt);
}



/*
  The below snippet of line shows the barrier synchronization of the process. We use this synchronization because
  process after creating the socket, it has to wait untill all other processes to create their sockets. So that they
  can communicate with each other. If we don't use this type synchronization, then process will send/receive messages to
  the processes which haven't created their sockets(No means of communication then). So, we use this barrier synchronizations.
*/

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

  //This function shows C and D values of process
  void showDC()
  {
    printf("Process %d has C=%d and D=%d values\n",pid+1,C,D );
  }




  /*
		This socket_create() function creates socket for process.
		The port assigned to the socket is obtained from the process id.(i.e DIFF_PORT+processID)
		Each process will invoke this function to create socket for that process.
	*/
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
