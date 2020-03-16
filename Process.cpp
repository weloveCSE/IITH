using namespace std;
class Process
{

public:
  int socket_file_discripter; /*Socket discriptor for process */

  thread receive_t; /* Each process should maintain two threads. one for local clock and another for receive*/

  int pid; /*Process identifier. Which will be initialized when the process constructor is created.*/

  bool flag = false; /*Used for wait*/

  char color[10], tokenColor[15];

  int C, D, parentNode;
  int minMsg;

  bool hasToken, isLeaf;
  int no_of_children;
  bool reported, tokenSeen;

  Process(int id)
  {
    //localclock_t =  thread(&localclock :: IncrementDriftFactor,local_clock);
    receive_t = thread(&Process ::receive, this);
    pid = id; /*Assign process id to pid variable of the process class*/
    strcpy(color, "white");
    parentNode = -1;
    hasToken = false;
    isLeaf = false;
    no_of_children = 0;
    reported = false;
    tokenSeen = false;
    strcpy(tokenColor, "white");
    receive_t.detach();
  }
  ~Process()
  {
    close(socket_file_discripter);
  }

  void send(int source, int destination, char *message)
  {

    if(checkBlackToken())
    {
      reinitiateTD();
      exit(0);
    }

    float ff = sleepTime(0.8);
    sleep(ff);
    char *tt = timeTaken();

    struct sockaddr_in peeraddr;                       /*Now socket address to the peer to whome the process want to send the message*/
    peeraddr.sin_family = AF_INET;                     /*We use IPV4 protocol*/
    peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local host address here*/
    peeraddr.sin_port = htons(destination + DIFF_PORTS);
    printf("Cell %d sends %s message to Cell %d at %s\n\n", pid + 1, message, destination + 1, tt);
    if (!reported && hasToken&&(strcmp(color,"red")==0))
    {
      *number_of_messages += 1;
      hasToken=true;
      strcpy(tokenColor, "black");
      //printf("Process %d has turned its token color to %s\n", pid + 1, tokenColor);
    }
    sendto(socket_file_discripter, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&peeraddr, sizeof(peeraddr));

    if (delayTime && d_sent == 4)
    {

      delayTime = false;
      d_sent = 12;
      chooseRandomBlueProcesses();
      start_time = clock();
    }
    d_sent += 1;
  }

  void receive()
  {
    //if(delayTime) sleep(5);
    while (1)
    {
      char *message_received = new char[MAX_BUFFER]; /*Declaring message receive buffer*/
      unsigned int length, n, round_number;          /*Variables declaration*/
      char garbage[8];

      struct sockaddr_in peeraddr; /*socket addresses for  peer*/

      length = sizeof(peeraddr);                         /*Storing the size of peeradd in length*/
      peeraddr.sin_family = AF_INET;                     /*Using IPV4*/
      peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /*Local host address*/
      n = recvfrom(socket_file_discripter, (char *)message_received, MAX_BUFFER, MSG_WAITALL, (struct sockaddr *)&peeraddr, &length);
      message_received[n] = '\0'; /*Appending null character at the end of the message*/

      if (n < 0)
        continue;

      if (!strncmp(message_received, "red", 3))
      {
        float tq = sleepTime(lamda_red);
        sleep(tq);

        if (!reported)
        {
          if (strcmp(color, "red") != 0)
          {
            strcpy(color, "red");
            int sender = ntohs(peeraddr.sin_port) - DIFF_PORTS;

            // setAllConditionsOfSender(sender);
            char *tt = timeTaken();
            printf("Cell %d received red message from Cell %d at %s\n\n", pid + 1, sender + 1, tt);
            tt = timeTaken();
            printf("Cell %d turns red at %s\n\n", pid + 1, tt);
            redNeighbours(pid);
          }
          
        }
        else
        {
          strcpy(tokenColor, "white");
          blueNeighbours(pid);
        }
      }
      else if (!strncmp(message_received, "blue", 4))
      {

        

        strcpy(color, "blue");


        if(areChildrenTerminated(0)){

          setRootNode();

          terminationDetected();
          exit(0);
        }

        float tq = sleepTime(lamda_blue);
        sleep(tq);

        int sender = ntohs(peeraddr.sin_port) - DIFF_PORTS;

        char *tt = timeTaken();

        printf("Cell %d received blue message from Cell %d at %s\n\n", pid + 1, sender + 1, tt);
        tt = timeTaken();
        printf("Cell %d turns blue at %s\n\n", pid + 1, tt);
        strcpy(color, "blue");
        //callPrint();

        if (hasToken)
        {
          if (no_of_children == 0)
          {
            if (pid == 0)
            {
              if (strcmp(tokenColor, "black") == 0)
              {
                //std::cout << " Need to call re-initiated" << '\n';
                process_done = 0;
                reinitiateTD();
                return;
              }
              terminationDetected();
              return;
            }

            char token[100];
            strcpy(token, tokenColor);
            int parent = parentNode;
            hasToken = false;
            strcpy(tokenColor, "white"); // We don't know about the previous color of the token
            
            makeParentToken(parent);
            //Directly send token to parentNode
            struct sockaddr_in peeraddr;                       /*Now socket address to the peer to whome the process want to send the message*/
            peeraddr.sin_family = AF_INET;                     /*We use IPV4 protocol*/
            peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local host address here*/
            peeraddr.sin_port = htons(parent + DIFF_PORTS);
            char *tt = timeTaken();
            reported = true;
            tokenSeen = false;
            printf("Cell %d sends %s token  to Cell %d at %s\n\n", pid + 1, token, parent + 1, tt);
            //  send(pid,parentNode,token);
            sendto(socket_file_discripter, (const char *)token, strlen(token), MSG_CONFIRM, (const struct sockaddr *)&peeraddr, sizeof(peeraddr));
            //while(tokenSeen==false);
          }

          else
          {
            //printf("Process %d has token but no. of children are %d\n",pid+1,no_of_children );
            if(areChildrenTerminated(pid))
            {
                strcpy(color,"blue");
                strcpy(tokenColor,"white");
                hasToken=false;
                no_of_children=0;
            }
            blueNeighbours(pid);
          }

          //  blueNeighbours(pid);
        }

        //sleep(1);
        else
        {
          blueNeighbours(pid);
        }
      }
      else if (!strncmp(message_received, "black", 5))
      {
        *number_of_messages += 1;
        hasToken=true;
        if (pid == 0)
        {
          //cout << " Restart the process and exit\n";
          process_done = 0;
          reinitiateTD();
          exit(0);
        }

        strcpy(tokenColor, "black");

        int sender = ntohs(peeraddr.sin_port) - DIFF_PORTS;
        // changeTokenSeen(sender);
        char *tt = timeTaken();
        printf("Cell %d received %s token from Cell %d at %s\n", pid + 1, message_received, sender + 1, tt);

        if (hasToken == false)
          hasToken = true; //Because we have received the token from one of its child
       // std::cout << pid+1<< " Before child "<<no_of_children  << '\n';
        if (no_of_children > 0)
          no_of_children -= 1;
        //std::cout <<pid+1<< " Affore child "<<no_of_children  << '\n';

        if (pid!=0&&no_of_children == 0 )
        {
          char token[100];
          strcpy(token, tokenColor);

          int parent = parentNode;
          hasToken = false;
          tokenSeen = false;

          strcpy(tokenColor, "white"); // We don't know about the previous color of the token
           makeParentToken(parent);
          //Directly send token to parentNode
          struct sockaddr_in peeraddr;                       /*Now socket address to the peer to whome the process want to send the message*/
          peeraddr.sin_family = AF_INET;                     /*We use IPV4 protocol*/
          peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local host address here*/
          peeraddr.sin_port = htons(parent + DIFF_PORTS);
          char *tt = timeTaken();
          printf("Cell %d sends %s token  to Cell %d at %s\n\n", pid + 1, token, parent + 1, tt);
          sendto(socket_file_discripter, (const char *)token, strlen(token), MSG_CONFIRM, (const struct sockaddr *)&peeraddr, sizeof(peeraddr));
          //while(tokenSeen==false);
        }
        else
        {
            blueNeighbours(pid);
        }

        //  blueNeighbours(pid);
      }
      else if (!strncmp(message_received, "white", 5))
      {
        *number_of_messages += 1;
        hasToken=true;
        if (pid == 0 && no_of_children == 0)
        {
          cout << " termination_detected the process and exit - white\n";
          process_done = 0;
          terminationDetected();
          return;
        }

        int sender = ntohs(peeraddr.sin_port) - DIFF_PORTS;
        changeTokenSeen(sender);
        char *tt = timeTaken();
        printf("Cell %d received %s token from Cell %d at %s\n", pid + 1, message_received, sender + 1, tt);

        if (hasToken == false)
          hasToken = true; //Because we have received the token from one of its child
        ////std::cout << pid+1<< " Before child "<<no_of_children  << '\n';
        if (no_of_children > 0)
          no_of_children -= 1;
        //std::cout << pid+1<< " aFTER child "<<no_of_children  << '\n';
        //if(strcmp(tokenColor,"black")==0)

        if (pid!=0&&no_of_children == 0)
        {
          char token[100];
          strcpy(token, tokenColor);

          int parent = parentNode;

          hasToken = false;
          tokenSeen = false;
          strcpy(tokenColor, "white"); // We don't know about the previous color of the token
          makeParentToken(parent);
          //Directly send token to parentNode
          struct sockaddr_in peeraddr;                       /*Now socket address to the peer to whome the process want to send the message*/
          peeraddr.sin_family = AF_INET;                     /*We use IPV4 protocol*/
          peeraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local host address here*/
          peeraddr.sin_port = htons(parent + DIFF_PORTS);
          char *tt = timeTaken();
          printf("Cell %d sends %s token  to Cell %d at %s\n\n", pid + 1, token, parent + 1, tt);
          sendto(socket_file_discripter, (const char *)token, strlen(token), MSG_CONFIRM, (const struct sockaddr *)&peeraddr, sizeof(peeraddr));
          //while(tokenSeen==false);
        }
      }
      else
      {

        if(areChildrenTerminated(pid))
        {
            strcpy(color,"blue");
            strcpy(tokenColor,"white");
            hasToken=false;
            no_of_children=0;
        }
        //  blueNeighbours(pid);
      }
    }
    //  sleep(2);
    //  if(checkTTD()) exit(0);
  }

 /* void showRedMessage(int s, int d)
  {
    char *tt = timeTaken();
    printf("Cell %d receives red message from Cell %d at %s\n\n", s + 1, d + 1, tt);
    tt = timeTaken();
    printf("Cell %d turns  red at %s\n\n", s + 1, tt);
  }
  void showBlueMessage(int s, int d)
  {
    char *tt = timeTaken();
    printf("Cell %d receives blue message from Cell %d at %s\n\n", s + 1, d + 1, tt);
    tt = timeTaken();
    printf("Cell %d turns  blue at %s\n\n", s + 1, tt);
  } */
  void process_Synchronization_Barrier()
  {
    pthread_mutex_lock(&locker); /*all the processes should bind to socket before they start communicating each other*/
    process_done++;              /*Each process increments it if they bind the socket*/

    if (process_done == n) /*If all the processes are done with binding of the sockets then we release the lock and wake up the threads*/
    {
      for (int i = 1; i <= process_done; i++)     /*Here waking up the threads*/
        pthread_cond_signal(&conditioned_thread); /*This concept is used from operating systems subject for synchronization*/
    }

    else /*If not all processes completed the binding then wait untill they do */
    {
      pthread_cond_wait(&conditioned_thread, &locker); /*Waiting */
    }

    pthread_mutex_unlock(&locker); /*Do this after all processes completes the binding of socekts*/
    process_done = 0;
  }


  int socket_create()
  {
    int PORT = DIFF_PORTS + pid; /* Making port number with process id. Here DIFF_PORTS=35000 as predefined */

    //  printf("Process %d is Port with %d\n",pid+1,PORT );
    struct sockaddr_in myaddr; /* Socket address variable*/

    /*
			In below if condition we used socket() function to create socket and the returned discriptor is stored in the
			socket_file_discripter. If socket() function returns the value less than 0 then socket is not created. i.e error
			occured while creating the socket.
		*/
    if ((socket_file_discripter = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
      printf("Socket is not created for process %d \n", pid + 1);
      return -1;
    }

    /*
			After creating the socket we need to bind it with address and port number to get identified.
			Below snippet of code is doing this task.
		*/
    myaddr.sin_family = AF_INET;         // IPv4
    myaddr.sin_addr.s_addr = INADDR_ANY; /*Addressss*/
    myaddr.sin_port = htons(PORT);       /*Port number*/

    /*
			We use bind() function to bind the address and port number
		*/
    int status = bind(socket_file_discripter, (const struct sockaddr *)&myaddr, sizeof(myaddr));

    if (status < 0) /*If returned value from the bind() is negative then we can conclude that error occured during the binding process*/
    {
      printf("Binding error id occurred for process %d \n", myaddr.sin_port - DIFF_PORTS);
      perror("bind failed");
      exit(EXIT_FAILURE);
      return -1; //failed to associate particular port number with this process....
    }

    /*
			If we successfully bind the address then we can declare that socket creation for the process is completed successfully.
		*/
    if (pid != n)
      printf("Socket Created Successfully for process %d \n", pid + 1);
    ;
    //cout<<color<<endl;

    return 1;
  }
  void reset()
  {

    strcpy(color, "white");
    strcpy(tokenColor, "white");
    hasToken = false;
    reported = false;
    tokenSeen = false;
    no_of_children = 0;

    if (isLeaf)
    {
      no_of_children = spanningTree[pid].size() - 1;
      hasToken = true;
    }
    printf("Process %d is re-initiated\n", pid + 1);
  }
};
