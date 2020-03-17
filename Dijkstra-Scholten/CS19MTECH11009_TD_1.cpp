
/*
	K. Naveen Kumar
	CS19MTECH11009
	DC Programming Assignment-I
	IIT Hyderabad
*/

/*
	This file contains the code for main() to start our program execution and create_process_and_socket() function to
	create process and socket for that process which will be used to communicate with other processes through the sockets.
*/



/* Including the headers files that are necessary for our program*/
#include<iostream>
#define MAX_BUFFER 512
#define Max 10000
#define DIFF_PORTS 35000
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <unistd.h>
#include <cmath>
#include<vector>
#include<cmath>
#include <algorithm>
#include <ctime>





class localclock /* We are creating class for local clock here.*/
{
	/* We want make variables and methods of the class accessible from any other snippet of the program.
	   So , to give accesse through entire program we are making them available by declaring as public. The public
	   members are visible to all other classes. Now each process is able to access these members/variables in the program.
	*/

 	public: /* public key word used to declare "public". This is called public access modifier*/

	/* The Timeread() method is used to read the system time and getting the required format for our task. Then this function
		will return the value at the end, which is calculated using the formula given in the Assignment discription.
		This fucntion is invoked by processes (and their threads to read the current time)
	*/
	time_t Timeread() /*This function is not taking any arguments. This function just reads the time*/
	{
		struct timespec start; /* Variable to store the time value*/
		clock_gettime(CLOCK_MONOTONIC, &start); /* For obtaining time with much higher precision(nanoseconds) */
		return ((start.tv_sec*1e9) + start.tv_nsec)*1e-9 ;/* Time is read in the form of seconds and nanoseconds. So we are converting nano seconds into seconds and adding error_factor*driftFactor and we are returning this value*/
	}


};




double start_time,end_time; /*To calculate the Time taken by Termination Detection algorithm*/
using namespace std::chrono;
using namespace std;
int *number_of_messages; /*Used to count no.of messages*/
int *DD;


/*Below are some of the functions declaration , which are used in the program*/
float sleepTime(float);
void printPattern();
bool checkTTD();
bool chechParentNode(int);
void decreaseInitiator();
bool alreadySeen(int);
void callToReject(int);
bool alreadySeen(int);
void makeProcessD(int);
void chooseRandomRedProcesses();
void chooseRandomBlueProcesses();
void callDetermined();
vector<int>findRandomness(int,int);
void sendRedMessage(int,int);
void sendBlueMessage(int,int);
void redNeighbours(int);
void blueNeighbours(int);
char *timeTaken();



int process_init=-1; /*Used to know about the initiator node */
bool delayTime=true;/* used to  introduce blue messages */
int d_sent=0;/* used to  introduce blue messages */

pthread_cond_t conditioned_thread = PTHREAD_COND_INITIALIZER; /*Related to threads*/
pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER; /**/

int process_done = 0; /* Used in the process barrier concept (function)*/


vector<vector<int>> adjacencyMatrix; /* Stores adjacent neighbours information */

/*Below information is used in the computation */
int Ir ,  Ib ,n;
float Wr,Wb, lamda_red, lamda_blue, p , q ;


#include "Process.cpp" /*Including process class file*/


Process **P; /*Used to store processes details*/
Process *initiator_p; /*used to store initiator process details*/





/*	The create_process_and_socket() function is responsible for creating the process and socket for that process.  This
	function takes one argument as pid (which represents process id).
*/


void create_process_and_socket(int pid){

	if(pid<=n){

	/*Creating Constructor for the class ProcessClass. The Constructor will take one argument as process id.
	  As we are using threads, n processes will be created using different constructors in different threads.
	*/

	P[pid]=new Process(pid);
	int status = P[pid]->socket_create(); /* Calling socket creation function in Process Class*/

	P[pid]->process_Synchronization_Barrier(); /* Calling this function for synchronization of processes*/

	if(status<0) /*If socket creation fails*/
	{
		printf("Socket creation is failed\n");
		return ;
	}

	}
}


/*
	This function is used to split the given argumented string based on the delieter. When we read input from the text file , We need to break down
	the line to get required information.


*/


vector<string> split(string str, char delimiter)
{
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal; /*Vector of strings*/
}


int main()
{


		  number_of_messages=(int *)malloc(sizeof(int)); /*allocating memory */

		  /*
				Below peice of code will read the text file and gets information needed for the computation.
				such as no.of processes, Adjacency information etc.

		  */

          fstream newfile;
          string tp;
          vector<string> sep;
          std::vector<int> neighbours;

          newfile.open("input.txt",ios::in); //open a file to perform read operation using file object
          int i=0, l=0;
          while(getline(newfile, tp))
          {
          sep= split(tp, ' ');

          if(l==0){
          /*Very first line contains information about n,Wr,Ir,WB,Ib, p,q ,lambda_red,lamda_blue etc */
          n=stoi(sep[0]);Wr=stof(sep[1]);Ir=stoi(sep[2]);Wb=stof(sep[3]);Ib=stoi(sep[4]);lamda_red=stof(sep[5]);lamda_blue=stof(sep[6]);
          p=stof(sep[7]);q=stof(sep[8]);
            l=12;
          }

          else{	/*from second line onwards we will have adjacency information. So we read the information and store them in adjacencyMatrix*/

             if(i<n)
             {
                for(int j=0;j<sep.size();j++)
                {
                  int ii=stoi(sep[j]);
                  neighbours.push_back(ii);
                }
                adjacencyMatrix.push_back(neighbours); /*Storing in the adjacency matrix*/
                neighbours.clear();
             }
             i++;
          	}
          }
          newfile.close();  /*Closing the file*/




          P=new Process*[n]; /*N processes objects*/
          thread threads[n]; /* Threads*/

          for(i=0;i<n;i++) /*Creating thread of each process. 'n' threads for 'n' processes*/
            threads[i] =  thread(create_process_and_socket,i); /*thread takes two arguments, 1. routine/function address 2. process id. Each thread will execute create_process_and_socket function. So, that means each process is mainted by the individual threads */

          for(i=0;i<n;i++)
            threads[i].join(); /* Joining the threads so that main will wait until all processes complete their execution*/

          srand((unsigned) time(0));


      	  /*Below snippet of code will create initiator node*/
          initiator_p=new Process(n);
          initiator_p->socket_create();
					strcpy(initiator_p->color,"red");
					initiator_p->C=0;
					initiator_p->D=Ir;
          initiator_p->parentNode=n;

		  DD=(int *)malloc(sizeof(int));
		  *DD=Ir;
		  process_init=n;

		  float ff=sleepTime(Wr); /*Sleep exponential time with avg Wr before introducing the red messages*/
		  sleep(Wr);

          chooseRandomRedProcesses(); /* Choosing some random red  processes */

		  while(initiator_p->D!=0); /*Condition for termination */
		  end_time=clock();/*End clock time */

		  double total=(end_time-start_time)/double(CLOCKS_PER_SEC);
		  sleep(5);
		  std::cout << "\n Time taken by * Dijkstra Termination Detection * Algorithm is: " <<total<<"\n";

		  printPattern(); /*Prints related information after termination dection*/
}



/*
	This function computes the exponential with avg given by the argument and returns the result.

*/
float sleepTime(float a)
{
  double reqSleepTime;
  reqSleepTime = rand() / (double)(RAND_MAX);
  reqSleepTime=-log(1- reqSleepTime) / a;
  return reqSleepTime;
}




/*
	The chooseRandomRedProcesses() chooses some random process between 1 and n. It turns selected processes color to red and
	spreads the red color to its neighbhour.


*/

void chooseRandomRedProcesses()
{
	float t=sleepTime(Wr);
	sleep(2*t);

	std::vector<int> v=findRandomness(Ir,n); /* Finding random processes */


		char *stringTime=timeTaken();/* Time read from the system clock*/

		for(int i=0;i<Ir;i++)
		{
			stringTime=timeTaken();

			strcpy(P[v[i]]->color,"red"); /*Changing color to red */
			P[v[i]]->parentNode=initiator_p->pid; /*Making parent as initiator for dynamic tree construction in Dijkstra algorithm*/
			P[v[i]]->C=1; /* C, D are variables used for Dijkstra-Schelton TD algorithm*/
			P[v[i]]->D=0;
			*number_of_messages+=1; /*Counting the no.of messages */
			printf("Cell %d turns red at %s\n\n",v[i]+1,stringTime );
		}

		/*Below snippet of code will spread/forward red color to chosen processes's neighbours*/
		for(int i=0;i<v.size();i++)
		{
				int r=v[i];
				int adj=adjacencyMatrix[r].size();
				for(int j=1;j<adj;j++)
				{
					int in=adjacencyMatrix[r][j]-1;/*Adjacent node*/
					sendRedMessage(r,in); /*Sending red message to adjacent node*/
				}
		}

}




/*
	The chooseRandomBlueProcesses() chooses some  random process between 1 and n. It turns selected processes color to blue and
	spreads the blue color to its neighbhour.
*/

void chooseRandomBlueProcesses()
{
	float t=sleepTime(Wb);
	sleep(2*t);
	std::vector<int> v=findRandomness(Ib,n); /*Selecting random processes */

	char *stringTime=timeTaken();

		for(int i=0;i<Ib;i++)
		{
			stringTime=timeTaken();/*System time read*/

			if(P[v[i]]->D==0)
			{
				strcpy(P[v[i]]->color,"blue");
				printf("Cell %d turns blue at %s\n\n",v[i]+1,stringTime );
			}
			else
			{
					if((v[i]+1)!=n)
					{
						v[i]=v[i]+1;
						i--;
					}
			}
		}


	/* Below snippet of code will forward blue message to selected process's neighbours */

		for(int i=0;i<v.size();i++)
		{
				int r=v[i];
				int adj=adjacencyMatrix[r].size(); /**/
				for(int j=1;j<adj;j++)
				{

					int in=adjacencyMatrix[r][j]-1; /*neighbours node*/
					sendBlueMessage(r,in);
				}
		}

}


/*
	This findRandomness() function's computation will choose some random processes between given range(1 to n_adj_size).
	This function will produce no.of random processes as given size (size_subset).
	This function will return vector contain randomly selected processes.

*/

vector<int>findRandomness(int size_subset,int n_adj_size) //returns subset of indices of size size_subset range between 0 to n_adj_size
{
	//size_subset is no.of indices
	//n_adj_size is no.of neighbours

	int i=0,flag=1;
	vector<int>v;
	while(i<size_subset)
	{
		int index=rand()%n_adj_size; /* Generating random numbers*/
		if(index!=0){
		if(i==0){
				v.push_back(index);
		}
		else{
				flag=0;
				do{
					flag=0;

				if(find(v.begin(), v.end(), index) != v.end()&&index!=0) // Generated number should not be present in previously generated numbers
				{
					flag=1;
					index=rand()%n_adj_size;
				}
				else
				{
						v.push_back(index);
						flag=0;
				}
				}while(flag);
		}
		i++;

		}
	}

	return v;
}



/*
	This function takes two arguments and sends red message to destination process.
*/

void sendRedMessage(int s,int d) // Taking two argument as source and destination
{

  if(s!=d&&s<n+1&&d<n+1)
    {
      char message[MAX_BUFFER];
      sprintf(message,"%s %d\n","red",s	);
      if(P[s]->C>0)
			{

				P[s]->send(s,d,message); //Calling send() function
			}
    }
}



/*
	This function takes two arguments and sends blue message to destination process.
*/
void sendBlueMessage(int s,int d)
{

  if(s!=d&&s<n+1&&d<n+1)
    {
      char message[MAX_BUFFER];
      sprintf(message,"%s %d\n","blue",s	);
      P[s]->send(s,d,message); //Calling send() function in Process class
    }
}



/*
	This function will read the time from the local system and then truncate the taken time to get required representation of the time.
	This function will return stringTime

*/

char * timeTaken()
{
    localclock local_clock;
    struct tm * time_information; /* Structure containing a calendar date and time broken down into its components*/
    time_t system_time;/* time type variable, which stores the local_clock time from the tableValues*/
    time_information; /*Converts to local time*/
    char *stringTime;/*Convert tm structure to string*/
    system_time=local_clock.Timeread();
    time ( &system_time );
    time_information = localtime ( &system_time );
    stringTime= asctime(time_information);
    stringTime[19]='\0';
    stringTime=&stringTime[11];

    return stringTime;

}




/*
	This redNeighbours() function takes one argument and then sends red message to neighbours of the process that has taken as argument.
*/
void redNeighbours(int s)
{
	int n_size=ceil(adjacencyMatrix[s].size()*p);// p% of adjacent neighbours

	std::vector<int> v=findRandomness(n_size,adjacencyMatrix[s].size()); // Finding the random neighbours

	for(int i=0;i<v.size();i++)
	{
		float tt=sleepTime(Wr);
		sleep(tt);
		if(s<n+1&&adjacencyMatrix[s][v[i]]-1<n+1) sendRedMessage(s,adjacencyMatrix[s][v[i]]-1); // Sending message to neighbours
	}

}


/*
	This blueNeighbours() function takes one argument and then sends blue message to neighbours of the process that has taken as argument.
*/
void blueNeighbours(int s)
{
	int n_size=ceil(adjacencyMatrix[s].size()*q);// q% of adjacent neighbours

	std::vector<int> v=findRandomness(n_size,adjacencyMatrix[s].size()); // Finding random neighours
	for(int i=0;i<v.size();i++)
	{
		float tt=sleepTime(Wb);
		sleep(tt);
		if(s<n+1&&adjacencyMatrix[s][v[i]]-1<n+1)  sendBlueMessage(s,adjacencyMatrix[s][v[i]]-1); //sending blue message to neighbours
	}
}


/* Increments D of process , this function takes one argument as pid of process*/
void makeProcessD(int s)
{
	if(s!=process_init) P[s]->D+=1;
}

/* When ever initiator gets ack message then we use this function to decrement D of initiator*/
void decreaseInitiator()
{
	//printf("Came to decreaseInitiator function %d is D and *D=%d\tcount= %d\n",initiator_p->D,*DD,*number_of_messages );
	if(initiator_p->D>0) initiator_p->D-=1;
}

/*This function is used to check whether the initiator D is empty or not*/
bool checkTTD()
{
	if(initiator_p->D==0)
	{

		return true;
	}
	return false;
}

/*This function is used to check whether the parent node is itself or not*/
bool chechParentNode(int id)
{
	if(P[id]->parentNode==id) return true;
	return false;
}


/*After termination detection , we use this function to print the table which shows the details of the processes*/
void printPattern()
{
	printf("\npid\tcolor\tC\tD\tparentNode\n");
	for(int i=0;i<n;i++)
	{
		printf("%d\t%s\t%d\t%d\t%d\n\n",P[i]->pid+1,P[i]->color,P[i]->C,P[i]->D,P[i]->parentNode+1);
	}

	printf("%d is number of messages sent to detect Termination\n\n",*number_of_messages );

}
