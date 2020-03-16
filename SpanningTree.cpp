#include <iostream>
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
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>

class localclock
{
public:
	time_t Timeread() /*This function is not taking any arguments. This function just reads the time*/
	{
		struct timespec start;								  /* Variable to store the time value*/
		clock_gettime(CLOCK_MONOTONIC, &start);				  /* For obtaining time with much higher precision(nanoseconds) */
		return ((start.tv_sec * 1e9) + start.tv_nsec) * 1e-9; /* Time is read in the form of seconds and nanoseconds. So we are converting nano seconds into seconds and adding error_factor*driftFactor and we are returning this value*/
	}
};



void setRootNode();
bool checkBlackToken();
bool _flag=true;
void changeTokenSeen(int);
bool allBlue();
bool checkTD();
void makeParentToken(int);
bool termination_detected = false;
double start_time, end_time;
using namespace std::chrono;
using namespace std;
int *number_of_messages;
int non_leaves;
int *DD;
void setSenderTokens(int);
bool areChildrenTerminated(int);
void displayDetails();
void reinitiateTD();
void callPrint();
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
vector<int> findRandomness(int, int);
void sendRedMessage(int, int);
void sendBlueMessage(int, int);
void redNeighbours(int);
void blueNeighbours(int);
char *timeTaken();
bool terminationDetected();
void setAllConditionsOfSender(int);
int process_init = -1;
//int *socket_file_discripter;
bool delayTime = true;
int d_sent = 0;
pthread_cond_t conditioned_thread = PTHREAD_COND_INITIALIZER; /**/
pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;			  /**/
int process_done = 0;

vector<vector<int>> adjacencyMatrix, spanningTree;
int Ir, Ib, n;
float Wr, Wb, lamda_red, lamda_blue, p, q;

#include "Process.cpp"

Process **P;
Process *initiator_p;
void create_process_and_socket(int pid)
{

	if (pid <= n)
	{
		P[pid] = new Process(pid);
		int status = P[pid]->socket_create();

		P[pid]->process_Synchronization_Barrier();

		if (status < 0)
		{
			printf("Socket creation is failed\n");
			return;
		}
	}
}

vector<string> split(string str, char delimiter)
{
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter))
	{
		internal.push_back(tok);
	}

	return internal;
}

int main()
{

	number_of_messages = (int *)malloc(sizeof(int));
	fstream newfile;
	string tp;
	vector<string> sep;
	std::vector<int> neighbours;

	newfile.open("data.txt", ios::in); //open a file to perform read operation using file object
	int i = 0, m = 0, l = 0;
	while (getline(newfile, tp))
	{

		sep = split(tp, ' ');
		//std::cout << tp << '\n';

		if (l == 0)
		{
			n = stoi(sep[0]);
			Wr = stof(sep[1]);
			Ir = stoi(sep[2]);
			Wb = stof(sep[3]);
			Ib = stoi(sep[4]);
			lamda_red = stof(sep[5]);
			lamda_blue = stof(sep[6]);
			p = stof(sep[7]);
			q = stof(sep[8]);
			l = 12;
		}
		else
		{
			if (i < n)
			{
				for (int j = 0; j < sep.size(); j++)
				{
					int ii = stoi(sep[j]);
					//  if(initiator==-1) initiator=ii-1;
					neighbours.push_back(ii);
				}
				adjacencyMatrix.push_back(neighbours);
				neighbours.clear();
				i++;
			}
			else
			{
				//std::cout << "\n/* message */ "<<sep.size() << " --\n";
				for (int j = 0; j < sep.size(); j++)
				{
					int ii = stoi(sep[j]);
					// std::cout << "Naveen "<<ii << '\n';
					neighbours.push_back(ii);
				}
				spanningTree.push_back(neighbours);
				neighbours.clear();
				m++;
			}
		}
	}

	newfile.close();

	P = new Process *[n];
	thread threads[n];

	for (i = 0; i < n; i++) /*Creating thread of each process. 'n' threads for 'n' processes*/
		threads[i] = thread(create_process_and_socket, i);

	for (i = 0; i < n; i++)
		threads[i].join();

	for (int i = 0; i < n; i++)
	{
		if (spanningTree[i].size() == 1)
		{
			P[i]->isLeaf = true;
			P[i]->hasToken = true;
			strcpy(P[i]->color, "white");
			strcpy(P[i]->tokenColor, "white");
			P[i]->no_of_children = 0;
		}
		else
		{
			P[i]->no_of_children = spanningTree[i].size() - 1;
			for (int j = 1; j < spanningTree[i].size(); j++)
				if (P[spanningTree[i][j] - 1]->parentNode == -1)
					P[spanningTree[i][j] - 1]->parentNode = i;
		}
	}

	/*  printf("Pid\tsocket id\tcolor\tisLeaf\thasToken\ttokenColor\tNo_of children\tParent Node\n" );
          for(int i=0;i<n;i++)
            cout<<P[i]->pid+1 <<"\t"<<P[i]->socket_file_discripter<<"\t"<<P[i]->color<<"\t"<<P[i]->isLeaf<<"\t"<<P[i]->hasToken<<"\t"<<P[i]->tokenColor<<"\t"<<P[i]->no_of_children<<"\t"<<P[i]->parentNode+1<<"\t"<<endl;
*/

	/*
          for(int i=0;i<n;i++)
            cout<<P[i]->pid+1 <<"\t"<<P[i]->color<<"\t"<<P[i]->no_of_children<<"\t"<<P[i]->tokenColor<<endl;*/
	srand((unsigned)time(0));
	//sleep(3);

	std::cout << '\n';
	for (int i = 0; i < n; i++)
		cout << P[i]->pid + 1 << "\t" << P[i]->color << "\t" << P[i]->no_of_children << "\t" << P[i]->tokenColor << "\t" << P[i]->hasToken << endl;
	std::cout << '\n';
	chooseRandomRedProcesses();

	std::cout << '\n';
	/*  for(int i=0;i<n;i++)
            cout<<P[i]->pid+1 <<"\t"<<P[i]->color<<"\t"<<P[i]->no_of_children<<endl;
      /*    char message[100];
          strcpy(message,"blue 1");
          strcpy(P[1]->color,"blue");
          char *ss=timeTaken();
          printf("Cell 2 turns blue at %s \n",ss);
          P[1]->send(1,7,message);
          sleep(5);
          std::cout << "/* message */
	;
	sleep(3);
	//  chooseRandomBlueProcesses();

	while (_flag);

	//for (int i = 0; i < n; i++)
	//	cout << P[i]->pid + 1 << "\t" << P[i]->color << "\t" << P[i]->no_of_children << "\t" << P[i]->tokenColor << "\t" << P[i]->hasToken << endl;
	return 0;
	//while(1);
}

float sleepTime(float a)
{
	double reqSleepTime;
	reqSleepTime = rand() / (double)(RAND_MAX);
	reqSleepTime = -log(1 - reqSleepTime) / a;
	return reqSleepTime;
}
void chooseRandomRedProcesses()
{
	float t = sleepTime(lamda_red);
	sleep(2 * t);
	std::vector<int> v = findRandomness(Ir, n);

	localclock local_clock;
	struct tm *time_information; /* Structure containing a calendar date and time broken down into its components*/
	time_t system_time;			 /* time type variable, which stores the local_clock time from the tableValues*/
	time_information;			 /*Converts to local time*/
	char *stringTime;			 /*Convert tm structure to string*/

	for (int i = 0; i < Ir; i++)
	{ //std::cout << "Came into Random processes " << '\n';
		system_time = local_clock.Timeread();
		time(&system_time);
		time_information = localtime(&system_time);
		stringTime = asctime(time_information);
		stringTime[19] = '\0';
		stringTime = &stringTime[11];
		//std::cout << v[i]+1 <<" Process "<< '\n';
		strcpy(P[v[i]]->color, "red");
		if (P[v[i]]->hasToken)
			strcpy(P[v[i]]->tokenColor, "black"); // We can make it as red/back
		*number_of_messages += 1;
		printf("Cell %d turns %s at %s\n\n", v[i] + 1, P[v[i]]->color, stringTime);
	}

	for (int i = 0; i < v.size(); i++)
	{
		int r = v[i];
		//	std::cout << r << '\n';
		int adj = adjacencyMatrix[r].size(); // CHoose random processes here
		int n_size = ceil(adj * p);
		std::vector<int> vv = findRandomness(n_size, adj);
		for (int j = 0; j < n_size; j++)
		{
			//printf("Cell %d is neighbour of Cell %d\n",adjacencyMatrix[r][j],r+1);
			int in = adjacencyMatrix[r][vv[j]] - 1;
			sendRedMessage(r, in);
		}
	}
	//printf("Ended here chooseRandomRedProcesses\n" );
}

void chooseRandomBlueProcesses()
{
	float t = sleepTime(lamda_blue);
	sleep(2 * t);
	std::vector<int> v = findRandomness(Ib, n);

	for (int i = 0; i < v.size(); i++)
		std::cout << v[i] << '\t';
	cout << endl;

	localclock local_clock;
	struct tm *time_information; /* Structure containing a calendar date and time broken down into its components*/
	time_t system_time;			 /* time type variable, which stores the local_clock time from the tableValues*/
	time_information;			 /*Converts to local time*/
	char *stringTime;			 /*Convert tm structure to string*/

	for (int i = 0; i < Ib; i++)
	{
		system_time = local_clock.Timeread();
		time(&system_time);
		time_information = localtime(&system_time);
		stringTime = asctime(time_information);
		stringTime[19] = '\0';
		stringTime = &stringTime[11];

		strcpy(P[v[i]]->color, "blue");
		printf("Cell %d turns blue at %s\n\n", v[i] + 1, stringTime);
	}
	//	std::cout << "Random vector size\n"<<v.size() << '\n';
	for (int i = 0; i < v.size(); i++)
	{
		int r = v[i];
		//	std::cout << r << '\n';
		int adj = adjacencyMatrix[r].size();
		for (int j = 1; j < adj; j++)
		{
			//printf("Cell %d is neighbour of Cell %d\n",adjacencyMatrix[r][j],r+1);
			int in = adjacencyMatrix[r][j] - 1;
			sendBlueMessage(r, in);
		}
	}
	//printf("Ended here chooseRandomRedProcesses\n" );
}

vector<int> findRandomness(int size_subset, int n_adj_size) //returns subset of indices of size size_subset range between 0 to n_adj_size
{
	//size_subset is no.of indices
	//n_adj_size is no.of neighbours

	int i = 0, flag = 1;
	vector<int> v;
	while (i < size_subset)
	{
		int index = rand() % n_adj_size;
		if (index != 0)
		{
			if (i == 0)
			{
				v.push_back(index);
			}
			else
			{
				flag = 0;
				do
				{
					flag = 0;
					//int index=rand()%n_adj_size;
					if (find(v.begin(), v.end(), index) != v.end() && index != 0)
					{
						flag = 1;
						index = rand() % n_adj_size;
					}
					else
					{
						v.push_back(index);
						flag = 0;
					}
				} while (flag);
			}
			i++;
		}
	}

	return v;
}

void sendRedMessage(int s, int d)
{

	if (s != d)
	{
		char message[MAX_BUFFER];
		sprintf(message, "%s ", "red");

		P[s]->send(s, d, message);
	}
}

void sendBlueMessage(int s, int d)
{

	if (s != d)
	{
		char message[MAX_BUFFER];
		sprintf(message, "%s ", "blue");
		P[s]->send(s, d, message);
	}
}
char *timeTaken()
{
	localclock local_clock;
	struct tm *time_information; /* Structure containing a calendar date and time broken down into its components*/
	time_t system_time;			 /* time type variable, which stores the local_clock time from the tableValues*/
	time_information;			 /*Converts to local time*/
	char *stringTime;			 /*Convert tm structure to string*/
	system_time = local_clock.Timeread();
	time(&system_time);
	time_information = localtime(&system_time);
	stringTime = asctime(time_information);
	stringTime[19] = '\0';
	stringTime = &stringTime[11];

	return stringTime;
}
void redNeighbours(int s)
{
	int n_size = ceil(adjacencyMatrix[s].size() * p);
	std::vector<int> v = findRandomness(n_size, adjacencyMatrix[s].size());
	for (int i = 0; i < v.size(); i++)
	{
		float tt = sleepTime(lamda_red);
		sleep(tt);
		sendRedMessage(s, adjacencyMatrix[s][v[i]] - 1);
	}
}
void blueNeighbours(int s)
{
	int n_size = ceil(adjacencyMatrix[s].size());
	std::vector<int> v = findRandomness(n_size, adjacencyMatrix[s].size());
	for (int i = 0; i < v.size(); i++)
	{
		float tt = sleepTime(lamda_blue);
		sleep(tt);
		sendBlueMessage(s, adjacencyMatrix[s][v[i]] - 1);
	}
}

void makeProcessD(int s)
{
	if (s != process_init)
		P[s]->D += 1;
}

void decreaseInitiator()
{
	//printf("Came to decreaseInitiator function %d is D and *D=%d\tcount= %d\n",initiator_p->D,*DD,*number_of_messages );
	if (initiator_p->D > 0)
		initiator_p->D -= 1;
}
bool checkTTD()
{
	if (initiator_p->D == 0)
	{
		std::cout << "Terminated" << '\n';
		return true;
	}
	return false;
}
bool chechParentNode(int id)
{
	if (P[id]->parentNode == id)
		return true;
	return false;
}

void printPattern()
{
	printf("\npid\tcolor\tC\tD\tparentNode\n");
	for (int i = 0; i < n; i++)
	{
		printf("%d\t%s\t%d\t%d\t%d\n\n", P[i]->pid + 1, P[i]->color, P[i]->C, P[i]->D, P[i]->parentNode + 1);
	}

	printf("%d is number of messages sent to detect Termination\n\n", *number_of_messages);
}

void setAllConditionsOfSender(int s)
{
	strcpy(P[s]->tokenColor, "black");
}
void reinitiateTD()
{

	for (int i = 0; i < n; i++)
	{
		P[i]->reset();

		P[i]->process_Synchronization_Barrier();
	}

	//callPrint();

	delayTime = true;
	d_sent = 0;
	cout << endl;
	chooseRandomRedProcesses();
}

bool allBlue()
{
	for (int i = 1; i < n; i++)
	{
		if (strcmp(P[i]->color, "blue") != 0 || (strcmp(P[i]->tokenColor, "black") == 0))
			return false;
	}
	return true;
}
bool checkTD()
{
	if (P[0]->no_of_children != 0)
		return true;
	else if (strcpy(P[0]->tokenColor, "black") == 0)
		return true;
	return false;
}

void callPrint()
{
	printf("Pid\tsocket id\tcolor\tisLeaf\thasToken\ttokenColor\tNo_of children\tParent Node\n");
	for (int i = 0; i < n; i++)
		cout << P[i]->pid + 1 << "\t" << P[i]->socket_file_discripter << "\t\t" << P[i]->color << "\t" << P[i]->isLeaf << "\t" << P[i]->hasToken << "\t\t" << P[i]->tokenColor << "\t\t" << P[i]->no_of_children << "\t\t" << P[i]->parentNode + 1 << "\t" << endl;
}

void setSenderTokens(int s)
{
	strcpy(P[s]->tokenColor, "white");
	P[s]->hasToken = false;
}

void displayDetails()
{
	std::cout << " ended here" << '\n';
	exit(0);
}

void changeTokenSeen(int sender)
{
	P[sender]->tokenSeen = true;
}

bool areChildrenTerminated(int s)
{
	if(P[s]->isLeaf) return false;

	bool flag=true;
	std::vector<char> v;
	for(int i=0;i<spanningTree[s].size();i++) v.push_back(spanningTree[s][i]);
	for(int i=1;i<v.size();i++)
	{
			if(P[v[i]-1]->no_of_children!=0||(strcmp(P[v[i]-1]->color,"blue")!=0)||(strcmp(P[v[i]-1]->tokenColor,"black")==0)) return false;
	}
	return true;
}
bool terminationDetected(){
	sleep(10);
	callPrint();
	exit(0);
	return true;
}

void makeParentToken(int s)
{
	P[s]->hasToken=true;
}
bool checkBlackToken()
{
	if(strcmp(P[0]->tokenColor,"black")==0) return true;
	return false;
}
void setRootNode()
{

		strcpy(P[0]->color,"blue");
         P[0]->no_of_children=0;
          P[0]->hasToken=true;
}