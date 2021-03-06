//os.cpp : Defines the entry point for the console application.
//Whitney Angelica Andrews
//COP 4610 CPU Scheudluer FCFS SJF MLFQ

#include "stdafx.h"
#include<iostream>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

class info {
public:
	info(vector<int> v);
	int timeTR; //turn around time
	int timeW; //waiting time
	int timeR; //response time
	int index; //index to current prog
	int totalburst; //the total burst time
	bool done = false; //flag when process is done
	bool start = false; //flag when process begins
	vector<int> P; // current program

};
//initializes content of info class
info::info(vector<int>v) {
	totalburst = timeTR = timeW = 0;
	timeR = -1;
	P = v;
}
void vprint(const vector<int>&A); //prints a vector of ints
bool pempty(vector<info> p); //check if the vector of class is empty, to let know if all processes are done
void FCFS(const vector<info> &V); //First Come Fist Serve
void SJF(const vector<info> &V); //Shortest Job First
void MLFQ(const vector<info> &V); //MultiLevel Feedback Queue
void vprint(const vector<int>&A); //prints vector for testing
vector<info> runio(vector<info> &io); //runs io, decrements and returns vector of any that hit 0
int smallest(const vector<info> &ready); //returns smallest index in ready q
void main() {

	vector<info> Progs;
	//intializes program bursts to a vector of ints
	vector<int> P2[8];
	P2[0] = { 4, 24, 5, 73, 3, 31, 5, 27, 4, 33, 6, 43, 4, 64, 5, 19, 2 };
	P2[1] = { 18, 31, 19, 35, 11, 42, 18, 43, 19, 47, 18, 43, 17, 51, 19, 32, 10 };
	P2[2] = { 6, 18, 4, 21, 7, 19, 4, 16, 5, 29, 7, 21, 8, 22, 6, 24, 5 };
	P2[3] = { 17, 42, 19, 55, 20, 54, 17, 52, 15, 67, 12, 72, 15, 66, 14 };
	P2[4] = { 5, 81, 4, 82, 5, 71, 3, 61, 5, 62, 4, 51, 3, 77, 4, 61, 3, 42, 5 }; 
	P2[5] = { 10, 35, 12, 41, 14, 33, 11, 32, 15, 41, 13, 29, 11 };
	P2[6] = { 21, 51, 23, 53, 24, 61, 22, 31, 21, 43, 20 };
	P2[7] = { 11, 52, 14, 42, 15, 31, 17, 21, 16, 43, 12, 31, 13, 32, 15 };

	//initilizes the vector to the class
	for (int i = 0; i < 8; i++)
	{
		Progs.push_back(P2[i]);
		Progs[i].index = i;
	}

	FCFS(Progs);
	SJF(Progs);
	MLFQ(Progs);

}
void FCFS(const vector<info> &V) {

	int totalRunT = 0; // totaltime
	vector<info> temp = V; //temporary vector so that the orignal vector will not be changed
	vector<info> running; // process running only one at a time
	vector<info> ready; // ready queue
	vector<info> io; // io processes
	totalRunT = 0; //  reseting run time for each algorithm

	int ResponseTime = 0; // counts response time
	vector<info> ioindex; //keep track of indexes in io
	double totalCPUBurst; //the total cpu bursts for wait time
	double totalCPUused; //total cpu used
	int currindex; //keeps track of current process
	totalCPUBurst = totalCPUused = 0; //sets the values to zero for counting

	// calculating total cpu burs, total wait time and initial response times
	//initializes ready queue with all eight processes at start
	for (size_t i = 0; i < temp.size(); i++) 
	{
		//calculating total cpu burst
		for (size_t j = 0; j < temp[i].P.size(); j += 2) 
		{
			totalCPUBurst += temp[i].P[j];
		}
		// calculating total bursts for wait time
		for (size_t j = 0; j < temp[i].P.size(); j++)
		{
			temp[i].totalburst += temp[i].P[j]; 
		}
		// calculate initial response times
		temp[i].timeR = ResponseTime;
		ResponseTime += temp[i].P.front();
		// initializing ready queue at start
		ready.push_back(temp[i]);
		temp[i].P.erase(temp[i].P.begin());
	}
	cout << "*********First Come First Serve (FCFS) Dynamic Execution*********" << endl << endl;
	//main control loop [until all processes are empty]
	while (!pempty(temp) || !ready.empty() || !io.empty() || !running.empty()) { 
		//if the running is empty, then load the next process in ready
		if (running.empty()) {
			if (!ready.empty()) {

				running.push_back(ready.front());
				ready.erase(ready.begin());

				if (!running.empty())
				{
				cout << "Running Burst: " << running.front().P.front() << endl;
				}
				else
				{
				cout << "Running Burst: Empty" << endl;
				}
				if (!ready.empty()) {
				cout << "Ready List: ";
				for (size_t i = 0; i < ready.size(); i++)
				{
				cout << ready[i].P.front() << " ";
				}
				cout << endl;
				}
				else {
				cout << "Ready List: Empty" << endl;
				}
				if (!io.empty()) {
				cout << "IO Burst: ";
				for (size_t i = 0; i < io.size(); i++)
				{
				cout << io[i].P.front() << " ";
				}
				cout << endl;
				}
				else {
				cout << "IO Burst: Empty" << endl;
				}
				cout << "Run Time: " << totalRunT << endl;
				cout << "------------------------------" << endl;

				running.front().P.front()--;

			}
		}
		else {
			//decrement running if there is an item currently in it
			if (running.front().P.front() > 0) {
				running.front().P.front()--;
			}
			//get the index of the process running, if it is empty clock the important times otherwise run the io burst of the current process
			//set the next item in the ready list to running
			else {

				currindex = running.front().index;
				running.pop_back();

				if (temp[currindex].P.empty()) {
					temp[currindex].timeTR = totalRunT;
					temp[currindex].done = true;
					cout << "Process " << currindex + 1 << " Finished: " << totalRunT << endl;
				}
				if (!temp[currindex].P.empty()) {

					io.push_back(temp[currindex]);
					temp[currindex].P.erase(temp[currindex].P.begin());
				}

				if (!ready.empty()) {

					running.push_back(ready.front());
					ready.erase(ready.begin());
					if (!running.empty())
					{
					cout << "Running Burst: " << running.front().P.front() << endl;
					}
					else
					{
					cout << "Running Burst: Empty" << endl;
					}
					if (!ready.empty()) {
					cout << "Ready List: ";
					for (size_t i = 0; i < ready.size(); i++)
					{
					cout << ready[i].P.front() << " ";
					}
					cout << endl;
					}
					else {
					cout << "Ready List: Empty" << endl;
					}
					if (!io.empty()) {
					cout << "IO Burst: ";
					for (size_t i = 0; i < io.size(); i++)
					{
					cout << io[i].P.front() << " ";
					}
					cout << endl;
					}
					else {
					cout << "IO Burst: Empty" << endl;
					}
					cout << "Run Time: " << totalRunT << endl;
					cout << "------------------------------" << endl;
					running.front().P.front()--;

				}


			}
		}

		// decrements all processes in io by 1, returns index of processes that are 0 or less
		// and removes any process from io that reaches 0 or less
		if (!io.empty()) { 
			ioindex = runio(io);
		}
		//if the io indexes are empty and the process is not yet done, then the CPU process is loaded to the ready list
		if (!ioindex.empty()) {

			for (size_t i = 0; i < ioindex.size(); i++)
			{
				if (!temp[ioindex[i].index].P.empty()) {
					ready.push_back(temp[ioindex[i].index]);
					temp[ioindex[i].index].P.erase(temp[ioindex[i].index].P.begin());
				}
			}

		}
		totalRunT++;
	}
	totalRunT--;
	
	//calculates waiting time based on turn around time and the total cpu burst required for each program
	for (size_t i = 0; i < temp.size(); i++) { 
		temp[i].timeW = temp[i].timeTR - temp[i].totalburst;
	}
	//prints out final result
	cout << "*********First Come First Serve (FCFS) Results*********" << endl;
	for (size_t i = 0; i < temp.size(); i++) {
		cout << "P " << i + 1 << " "
			<< " Tw: " << temp[i].timeW
			<< " Ttr: " << temp[i].timeTR
			<< " Tr: " << temp[i].timeR << endl;
	}
	//calculates the averages
	double wtavg, Ttravg, Travg;
	wtavg = Ttravg = Travg = 0;
	for (size_t i = 0; i < temp.size(); i++)
	{
		wtavg += temp[i].timeW;
		Ttravg += temp[i].timeTR;
		Travg += temp[i].timeR;

	}
	wtavg = wtavg / temp.size();
	Ttravg = Ttravg / temp.size();
	Travg = Travg / temp.size();
	totalCPUused = totalCPUBurst / totalRunT;
	//prints out the averages
	cout << "***********************************************" << endl;
	cout << "AVGTW: " << wtavg << " AVGTtr: " << Ttravg << " AVGTr: " << Travg << endl << endl;
	cout << "Total CPU Used: " << totalCPUused * 100 << "%" << endl;
	cout << "***********************************************" << endl;
	cout << "***********************************************" << endl << endl;
}

void SJF(const vector<info> &V) {
	vector<info> temp = V; //temporary vector so that the orignal vector will not be changed
	vector<info> running; //process running only one at a time
	vector<info> ready; //ready queue
	vector<info> io; //io processes
	int totalRunT = 0; // total run time

	int ResponseTime = 0; // counts response time
	vector<info> ioindex; //keep track of indexes in io
	double totalCPUBurst; //the total cpu bursts for wait time
	double totalCPUused; //total cpu used
	int currindex; //keeps track of current process
	totalCPUBurst = totalCPUused = 0; //sets the values to zero for counting


	// calculating total cpu burst, total wait time and initial response times
	//initializes ready queue with all eight processes at start
	for (size_t i = 0; i < temp.size(); i++)
	{
		//calculating total cpu burst
		for (size_t j = 0; j < temp[i].P.size(); j += 2)
		{
			totalCPUBurst += temp[i].P[j];
		}
		// calculating total bursts for wait time
		for (size_t j = 0; j < temp[i].P.size(); j++)
		{
			temp[i].totalburst += temp[i].P[j];
		}
		// calculate initial response times
		temp[i].timeR = ResponseTime;
		ResponseTime += temp[i].P.front();
		// initializing ready queue at start
		ready.push_back(temp[i]);
		temp[i].P.erase(temp[i].P.begin());
	}

	// main control loop [until all processes are empty]
	cout << "*********Shortest Job First (SJF) Dynamic Execution*********" << endl << endl;
	while (!pempty(temp) || !ready.empty() || !io.empty() || !running.empty()) { 
		//if the running is empty, then load the next smallest process in ready
		if (running.empty()) {
			if (!ready.empty()) {
				if (ready[smallest(ready)].start == false) {
					temp[ready[smallest(ready)].index].start = true;
					temp[ready[smallest(ready)].index].timeR = totalRunT;
				}
				running.push_back(ready[smallest(ready)]);
				ready.erase(ready.begin() + smallest(ready));

				if (!running.empty())
				{
					cout << "Running Burst: " << running.front().P.front() << endl;
				}
				else
				{
					cout << "Running Burst: Empty" << endl;
				}
				if (!ready.empty()) {
					cout << "Ready List: ";
					for (size_t i = 0; i < ready.size(); i++)
					{
						cout << ready[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "Ready List: Empty" << endl;
				}
				if (!io.empty()) {
					cout << "IO Burst: ";
					for (size_t i = 0; i < io.size(); i++)
					{
						cout << io[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "IO Burst: Empty" << endl;
				}
				cout << "Run Time: " << totalRunT << endl;
				cout << "------------------------------" << endl;

				running.front().P.front()--;

			}
		}
		if (!running.empty()) {
			//decrement running if there is an item currently in it
			if (running.front().P.front() > 0) {
				running.front().P.front()--;
			}
			//get the index of the process running, if it is empty clock the important times otherwise run the io burst of the current process
			//set the next item in the ready list to running
			else {

				currindex = running.front().index;
				running.pop_back();

				if (temp[currindex].P.empty()) {
					temp[currindex].timeTR = totalRunT;
					temp[currindex].done = true;
					cout << "Process " << currindex + 1 << " Finished: " << totalRunT << endl;
				}
				if (!temp[currindex].P.empty()) {

					io.push_back(temp[currindex]);
					temp[currindex].P.erase(temp[currindex].P.begin());
				}

				if (!ready.empty()) {

					if (ready[smallest(ready)].start == false) {
						temp[ready[smallest(ready)].index].start = true;
						temp[ready[smallest(ready)].index].timeR = totalRunT;
					}

					running.push_back(ready[smallest(ready)]);
					ready.erase(ready.begin() + smallest(ready));
					if (!running.empty())
					{
						cout << "Running Burst: " << running.front().P.front() << endl;
					}
					else
					{
						cout << "Running Burst: Empty" << endl;
					}
					if (!ready.empty()) {
						cout << "Ready List: ";
						for (size_t i = 0; i < ready.size(); i++)
						{
							cout << ready[i].P.front() << " ";
						}
						cout << endl;
					}
					else {
						cout << "Ready List: Empty" << endl;
					}
					if (!io.empty()) {
						cout << "IO Burst: ";
						for (size_t i = 0; i < io.size(); i++)
						{
							cout << io[i].P.front() << " ";
						}
						cout << endl;
					}
					else {
						cout << "IO Burst: Empty" << endl;
					}
					cout << "Run Time: " << totalRunT << endl;
					cout << "------------------------------" << endl;
					running.front().P.front()--;

				}
			}
		}
		// decrements all processes in io by 1, returns index of processes that are 0 or less
		// and removes any process from io that reaches 0 or less
		if (!io.empty()) {
			ioindex = runio(io);
		}
		//if the io indexes are empty and the process is not yet done, then the CPU process is loaded to the ready list
		if (!ioindex.empty()) {

			for (size_t i = 0; i < ioindex.size(); i++)
			{
				if (!temp[ioindex[i].index].P.empty()) {
					ready.push_back(temp[ioindex[i].index]);
					temp[ioindex[i].index].P.erase(temp[ioindex[i].index].P.begin());
				}
			}

		}

		totalRunT++;
	}

	//calculates waiting time based on turn around time and the total cpu burst required for each program
	for (size_t i = 0; i < temp.size(); i++) {

		temp[i].timeW = temp[i].timeTR - temp[i].totalburst;
	}

	//prints out final result
	cout << "*********Shortest Job First (SJF) Results*********" << endl;
	for (size_t i = 0; i < temp.size(); i++) {
		cout << "P " << i + 1 << " "
			<< " Tw: " << temp[i].timeW
			<< " Ttr: " << temp[i].timeTR
			<< " Tr: " << temp[i].timeR << endl;
	}
	//calculates the averages
	double wtavg, Ttravg, Travg;
	wtavg = Ttravg = Travg = 0;
	for (size_t i = 0; i < temp.size(); i++)
	{
		wtavg += temp[i].timeW;
		Ttravg += temp[i].timeTR;
		Travg += temp[i].timeR;

	}
	wtavg = wtavg / temp.size();
	Ttravg = Ttravg / temp.size();
	Travg = Travg / temp.size();
	totalCPUused = totalCPUBurst / totalRunT;
	//prints out the averages
	cout << "***********************************************" << endl;
	cout << "AVGTW: " << wtavg << " AVGTtr: " << Ttravg << " AVGTr: " << Travg << endl << endl;
	cout << "Total CPU Used: " << totalCPUused * 100 << "%" << endl;
	cout << "***********************************************" << endl;
	cout << "***********************************************" << endl << endl;

}
void MLFQ(const vector<info> &V) {

	vector<info> temp = V; //temporary vector so that the orignal vector will not be changed
	vector<info> running; // process running
	vector<info> RR1; // Round Robin queue 1
	vector<info> RR2; // Round Robin queue 2
	vector<info> FCFSQ; //First Come First Serve, final queue
	vector<info> io; // io processes
	int totalRunT = 0; // total run time
	int currentQ = 0; // 0 -> RR1, 1->RR2, 2->FCFSQ, keeps track of the current queue


	int ResponseTime = 0; // counts response time
	vector<info> ioindex; //keep track of indexes in io
	double totalCPUBurst; //the total cpu bursts for wait time
	double totalCPUused; //total cpu used
	int currindex; //keeps track of current process
	totalCPUBurst = totalCPUused = 0; //sets the values to zero for counting

	int tq1 = 6, tq2 = 11; //  time quantum 1 and 2 for Round Robin queues
	int rr1tq, rr2tq; // used to keep track of time quantam state (counter)
	rr1tq = rr2tq = 0; // presets the quantum states to zero
	
	cout << "*********MultiLevel Feedback Queue (MLFQ) Dynamic Execution*********" << endl;
	//calculating total cpu burst, total cpu burst and initializes the round robin queue one
	for (size_t i = 0; i < temp.size(); i++)
	{
		// calculating total bursts for wait time
		for (size_t j = 0; j < temp[i].P.size(); j++)
		{
			temp[i].totalburst += temp[i].P[j]; 
		}
		// calculates total cpu burst
		for (size_t j = 0; j < temp[i].P.size(); j += 2) 
		{
			totalCPUBurst += temp[i].P[j];
		}
		// initializing RR1 queue at start
		RR1.push_back(temp[i]);
		temp[i].P.erase(temp[i].P.begin());
	}
	//sets response time to zero for first processes
	temp[0].timeR = 0;
	// calculating response time per process
	for (size_t i = 1; i < RR1.size(); i++) 
	{
		if (RR1[i - 1].P.front() <= tq1) {
			temp[i].timeR = temp[i - 1].timeR + RR1[i - 1].P.front();
		}
		else {
			temp[i].timeR = temp[i - 1].timeR + tq1;
		}

	}

	// main control loop [until all processes are empty]
	while (!RR1.empty() || !io.empty() || !RR2.empty() || !FCFSQ.empty() || !running.empty()) { 
		//if the running is empty, then load the next process from the respective queue with RR1 having highest priority
		if (running.empty())
		{
			if (!RR1.empty()) {
				running.push_back(RR1.front());
				RR1.erase(RR1.begin());
				if (!running.empty())
				{
					cout << "Running Burst: " << running.front().P.front() << endl;
				}
				else
				{
					cout << "Running Burst: Empty" << endl;
				}
				if (!RR1.empty()) {
					cout << "RR1 Ready Queue: ";
					for (size_t i = 0; i < RR1.size(); i++)
					{
						cout << RR1[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "RR1 Ready Queue: Empty" << endl;
				}
				if (!RR2.empty()) {
					cout << "RR2 Ready Queue: ";
					for (size_t i = 0; i < RR2.size(); i++)
					{
						cout << RR2[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "RR2 Ready Queue: Empty" << endl;
				}
				if (!FCFSQ.empty()) {
					cout << "FCFS Queue Ready Queue: ";
					for (size_t i = 0; i < FCFSQ.size(); i++)
					{
						cout << FCFSQ[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "FCFS Queue Ready Queue: Empty" << endl;
				}
				if (!io.empty()) {
					cout << "IO Burst: ";
					for (size_t i = 0; i < io.size(); i++)
					{
						cout << io[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "IO Burst: Empty" << endl;
				}
				cout << "Run Time: " << totalRunT << endl;
				cout << "------------------------------" << endl;
				running.front().P.front()--;
				rr1tq++;
				currentQ = 0;
			}
			else if (!RR2.empty()) {
				running.push_back(RR2.front());
				RR2.erase(RR2.begin());
				if (!running.empty())
				{
					cout << "Running Burst: " << running.front().P.front() << endl;
				}
				else
				{
					cout << "Running Burst: Empty" << endl;
				}
				if (!RR1.empty()) {
					cout << "RR1 Ready Queue: ";
					for (size_t i = 0; i < RR1.size(); i++)
					{
						cout << RR1[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "RR1 Ready Queue: Empty" << endl;
				}
				if (!RR2.empty()) {
					cout << "RR2 Ready Queue: ";
					for (size_t i = 0; i < RR2.size(); i++)
					{
						cout << RR2[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "RR2 Ready Queue: Empty" << endl;
				}
				if (!FCFSQ.empty()) {
					cout << "FCFS Queue Ready Queue: ";
					for (size_t i = 0; i < FCFSQ.size(); i++)
					{
						cout << FCFSQ[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "FCFS Queue Ready Queue: Empty" << endl;
				}
				if (!io.empty()) {
					cout << "IO Burst: ";
					for (size_t i = 0; i < io.size(); i++)
					{
						cout << io[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "IO Burst: Empty" << endl;
				}
				cout << "Run Time: " << totalRunT << endl;
				cout << "------------------------------" << endl;
				running.front().P.front()--;
				rr2tq++;
				currentQ = 1;
			}
			else if (!FCFSQ.empty()) {
				running.push_back(FCFSQ.front());
				FCFSQ.erase(FCFSQ.begin());
				if (!running.empty())
				{
					cout << "Running Burst: " << running.front().P.front() << endl;
				}
				else
				{
					cout << "Running Burst: Empty" << endl;
				}
				if (!RR1.empty()) {
					cout << "RR1 Ready Queue: ";
					for (size_t i = 0; i < RR1.size(); i++)
					{
						cout << RR1[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "RR1 Ready Queue: Empty" << endl;
				}
				if (!RR2.empty()) {
					cout << "RR2 Ready Queue: ";
					for (size_t i = 0; i < RR2.size(); i++)
					{
						cout << RR2[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "RR2 Ready Queue: empty" << endl;
				}
				if (!FCFSQ.empty()) {
					cout << "FCFS Queue Ready Queue: ";
					for (size_t i = 0; i < FCFSQ.size(); i++)
					{
						cout << FCFSQ[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "FCFS Queue Ready Queue: empty" << endl;
				}
				if (!io.empty()) {
					cout << "IO Burst: ";
					for (size_t i = 0; i < io.size(); i++)
					{
						cout << io[i].P.front() << " ";
					}
					cout << endl;
				}
				else {
					cout << "IO Burst: Empty" << endl;
				}
				cout << "Run Time: " << totalRunT << endl;
				cout << "------------------------------" << endl;
				running.front().P.front()--;
				currentQ = 2;
			}
		}

		else {
			//decrement the running processes for as long as it is less than the given time quantum for the respective process
			if (running.front().P.front() > 0 && rr1tq < tq1 && rr2tq < tq2) {
				running.front().P.front()--;
				if (currentQ == 0) {
					rr1tq++;
				}
				else if (currentQ == 1) {
					rr2tq++;
				}
			}
			//when the front of the running is zero
			//get the index of the process running, if it is empty clock the important times otherwise run the io burst of the current process
			//set the next item in the highest priority available queue list to running
			else {
				//resets time quantum counter
				rr1tq = rr2tq = 0;
				if (running.front().P.front() == 0) {

					currindex = running.front().index;
					//if the current process is done and the done flag has not been set, clock the run time for turn around time 
					//and set the done flag to true
					if (temp[currindex].P.empty() && temp[currindex].done == false) {
						temp[currindex].timeTR = totalRunT;
						temp[currindex].done = true;
						cout << endl << "Process " << currindex + 1 << " Finished: " << totalRunT << endl;
					}
					//if the current index is not empty the get the io burst to io queue
					if (!temp[currindex].P.empty()) {
						io.push_back(temp[currindex]);
						temp[currindex].P.erase(temp[currindex].P.begin());
					}
					running.pop_back();
				}
				else if (currentQ == 0) {
					RR2.push_back(running.front());
					running.pop_back();
				}
				else if (currentQ == 1) {
					FCFSQ.push_back(running.front());
					running.pop_back();
				}
				//if the running is empty, set the next highest priority process to run
				if (running.empty())
				{
					if (!RR1.empty()) {

						running.push_back(RR1.front());
						RR1.erase(RR1.begin());
						if (!running.empty())
						{
							cout << "Running Burst: " << running.front().P.front() << endl;
						}
						else
						{
							cout << "Running Burst: Empty" << endl;
						}
						if (!RR1.empty()) {
							cout << "RR1 Ready Queue: ";
							for (size_t i = 0; i < RR1.size(); i++)
							{
								cout << RR1[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "RR1 Ready Queue: Empty" << endl;
						}
						if (!RR2.empty()) {
							cout << "RR2 Ready Queue: ";
							for (size_t i = 0; i < RR2.size(); i++)
							{
								cout << RR2[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "RR2 Ready Queue: Empty" << endl;
						}
						if (!FCFSQ.empty()) {
							cout << "FCFS Queue Ready Queue: ";
							for (size_t i = 0; i < FCFSQ.size(); i++)
							{
								cout << FCFSQ[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "FCFS Queue Ready Queue: Empty" << endl;
						}
						if (!io.empty()) {
							cout << "IO Burst: ";
							for (size_t i = 0; i < io.size(); i++)
							{
								cout << io[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "IO Burst: Empty" << endl;
						}
						cout << "Run Time: " << totalRunT << endl;
						cout << "-------------------------------" << endl;
						running.front().P.front()--;
						rr1tq++;
						currentQ = 0;

					}
					else if (!RR2.empty()) {
						running.push_back(RR2.front());
						RR2.erase(RR2.begin());
						if (!running.empty())
						{
							cout << "Running Burst: " << running.front().P.front() << endl;
						}
						else
						{
							cout << "Running Burst: Empty" << endl;
						}
						if (!RR1.empty()) {
							cout << "RR1 Ready Queue: ";
							for (size_t i = 0; i < RR1.size(); i++)
							{
								cout << RR1[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "RR1 Ready Queue: Empty" << endl;
						}
						if (!RR2.empty()) {
							cout << "RR2 Ready Queue: ";
							for (size_t i = 0; i < RR2.size(); i++)
							{
								cout << RR2[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "RR2 Ready Queue: Empty" << endl;
						}
						if (!FCFSQ.empty()) {
							cout << "FCFS Queue Ready Queue: ";
							for (size_t i = 0; i < FCFSQ.size(); i++)
							{
								cout << FCFSQ[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "FCFS Queue Ready Queue: Empty" << endl;
						}
						if (!io.empty()) {
							cout << "IO Burst: ";
							for (size_t i = 0; i < io.size(); i++)
							{
								cout << io[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "IO Burst: Empty" << endl;
						}
						cout << "Run Time: " << totalRunT << endl;
						cout << "------------------------------" << endl;
						running.front().P.front()--;
						rr2tq++;
						currentQ = 1;
					}
					else if (!FCFSQ.empty()) {
						running.push_back(FCFSQ.front());
						FCFSQ.erase(FCFSQ.begin());
						if (!running.empty())
						{
							cout << "Running Burst: " << running.front().P.front() << endl;
						}
						else
						{
							cout << "Running Burst: Empty" << endl;
						}
						if (!RR1.empty()) {
							cout << "RR1 Ready Queue: ";
							for (size_t i = 0; i < RR1.size(); i++)
							{
								cout << RR1[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "RR1 Ready Queue: Empty" << endl;
						}
						if (!RR2.empty()) {
							cout << "RR2 Ready Queue: ";
							for (size_t i = 0; i < RR2.size(); i++)
							{
								cout << RR2[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "RR2 Ready Queue: Empty" << endl;
						}
						if (!FCFSQ.empty()) {
							cout << "FCFS Queue Ready Queue: ";
							for (size_t i = 0; i < FCFSQ.size(); i++)
							{
								cout << FCFSQ[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "FCFS Queue Ready Queue: Empty" << endl;
						}
						if (!io.empty()) {
							cout << "IO Burst: ";
							for (size_t i = 0; i < io.size(); i++)
							{
								cout << io[i].P.front() << " ";
							}
							cout << endl;
						}
						else {
							cout << "IO Burst: Empty" << endl;
						}
						cout << "Run Time: " << totalRunT << endl;
						cout << "------------------------------" << endl;
						running.front().P.front()--;
						currentQ = 2;
					}
				}
				
			}

		}

		// decrements all processes in io by 1, returns index of processes that are 0 or less
		// and removes any process from io that reaches 0 or less
		if (!io.empty()) {
			ioindex = runio(io);
		}
		//if the io indexes are empty and the process is not yet done, then the CPU process is loaded to the ready list
		if (!ioindex.empty()) {

			for (size_t i = 0; i < ioindex.size(); i++)
			{
				if (!temp[ioindex[i].index].P.empty()) {
					RR1.push_back(temp[ioindex[i].index]);
					temp[ioindex[i].index].P.erase(temp[ioindex[i].index].P.begin());
				}
			}

		}

		totalRunT++;
	}

	totalRunT--;
	//calculates waiting time based on turn around time and the total cpu burst required for each program
	for (size_t i = 0; i < temp.size(); i++) {

		temp[i].timeW = temp[i].timeTR - temp[i].totalburst;

	}
	//prints out final result
	cout << "*********MultiLevel Feedback Queue (MLFQ) Results*********" << endl;
	for (size_t i = 0; i < temp.size(); i++) {
		cout << "P " << i + 1 << " "
			<< " Tw: " << temp[i].timeW
			<< " Ttr: " << temp[i].timeTR
			<< " Tr: " << temp[i].timeR << endl;
	}
	//calculates the averages
	double wtavg, Ttravg, Travg;
	wtavg = Ttravg = Travg = 0;
	for (size_t i = 0; i < temp.size(); i++)
	{
		wtavg += temp[i].timeW;
		Ttravg += temp[i].timeTR;
		Travg += temp[i].timeR;

	}
	wtavg = wtavg / temp.size();
	Ttravg = Ttravg / temp.size();
	Travg = Travg / temp.size();
	totalCPUused = totalCPUBurst / totalRunT;
	//prints out the averages
	cout << "***********************************************" << endl;
	cout << "AVGTW: " << wtavg << " AVGTtr: " << Ttravg << " AVGTr: " << Travg << endl << endl;
	cout << "Total CPU Used: " << totalCPUused * 100 << "%" << endl;
	cout << "***********************************************" << endl;
	cout << "***********************************************" << endl << endl;
}

bool pempty(const vector<info> p) {

	for (size_t i = 0; i < p.size(); i++)
	{
		if (!p[i].P.empty()) {
			return false;
		}
	}

	return true;

}

void vprint(const vector<int>&A) { 
	for (size_t i = 0; i < A.size(); i++)
	{
		cout << A[i];
		cout << " ";

	}
	cout << endl;
}

vector<info> runio(vector<info> &io) {

	vector<info> index;
	for (size_t i = 0; i < io.size(); i++)
	{
		io[i].P.front()--;
	}
	for (size_t i = 0; i < io.size(); i++)
	{
		if (io[i].P.front() < 1) {

			index.push_back(io[i]);
			io.erase(io.begin() + i);
			return index;
		}
	}
	return index;
}

int smallest(const vector<info> &ready) {
	int index = 0;

	for (size_t i = 1; i < ready.size(); i++)
	{
		if (ready[i].P.front() < ready[index].P.front()) {
			index = i;
		}
	}

	return index;
}