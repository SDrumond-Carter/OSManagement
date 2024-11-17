/* Samuel Drumond
 * Spring 2023
 * Embedded Operating System
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<math.h>

/*CPU scheduling global Variables*/
int EXIT = 0;
int INVALID = -1;
int CPU = 1;
int RACE = 2;
int PID = 5;
int resource = 5;
/*Banker's Algorithm global Variables*/
int BANKER = 3;
int PROCESS = 5;
int RESOURCE = 3;
int TRUE = 1;
int FALSE = 0;
/*Memory Menagement global Variables*/
int MEMORY = 4;
int FIRST = 0;
int BEST = 1;
int WORST = 2;
int NEXT = 3;
/*Page Replacement global Variables*/
int PAGE = 5;
int FIFO = 0;
int LRU = 1;
int FRAMES = 4;
/*Disk Scheduling global variable*/
int DISK = 6;
int FCFS = 0;
int SSTF = 1;
int REQUEST = 8;

/*CPU Scheduling Prototype*/
int displayMenu();
void cpuScheduling();
void raceCondition();
void *threadFuncOne(void *arg);
void *threadFuncTwo(void *arg);
void fcfs(int process[], int at[], int bt[]);
void sjf(int process[], int at[], int bt[]);
void displaySchedule(int process[], int at[], int bt[], int wt[], int tat[]);
/*Banker's Algorith prototypes*/
void bankersAlgorithm();
void systemState(int feasible[], int safe[]);
/*Memory Menagement prototypes*/
void memoryManagement();
void displayProcess(int allocation[], int n, int processSize[]);
void worstFit(int blockSize[], int blocks, int processSize[], int processes);
void nextFit(int blockSize[], int blocks, int processSize[], int processes);
void firstFit(int blockSize[], int blocks, int processSize[], int processes);
void bestFit(int blockSize[], int blocks, int processSize[], int processes);
/*Page Replacement prototypes*/
void pageReplacement();
void fifo();
void lru();
int findLRU(int time[]);
void displayPages(int page, int allocation[]);
/*Disk Scheduling prototypes*/
void diskFcfs(int requests[], int head);
void diskSstf(int requests[], int head);
void diskScheduling();


int main()
{
    int userChoice = INVALID;

    while(userChoice == INVALID)
    {
        userChoice = displayMenu(); // Set choice equal to function call displayMenu

        // Decision making logic to determine which OS function to run
        if(userChoice == CPU)
        {
            cpuScheduling(); // Display schedule
        }
        if(userChoice == EXIT)
        {
            exit(0); // Exit the program
        }
        if(userChoice == RACE)
        {
            raceCondition(); //Display race condition
        }
        if(userChoice == BANKER)
        {
            bankersAlgorithm(); //BankersAlgorithm 
        }
        if(userChoice == MEMORY)
        {
            printf("****************** Memory Management ******************\n");
            memoryManagement();
        }
        if(userChoice == PAGE)
        {
            pageReplacement();
        }
        if(userChoice == DISK)
        {
            diskScheduling();
        }
    }
    return 0;
}


// Function definition for displayMenu
int displayMenu()
{
    int userChoice = INVALID;
    while (userChoice == INVALID)
    {
        //print statements to display the menu
        printf("\n****************Operating System Management Menu****************\n");
        printf("\nSelect the OS program to run, enter the number of your selection\n");
        printf("6. Disk Scheduling\n");
        printf("5. Page Replacement\n");
        printf("4. Memory Management\n");
        printf("3. Banker's Algorithm\n");
        printf("2. Race Condition\n");
        printf("1. CPU Scheduling\n");
        printf("0. Exit\n");
        scanf("%d", &userChoice);

        //if input is greater than 6, restart
        if(userChoice > 6)
        {
            userChoice = INVALID;
        }
        //if the input is less than 0, restart
        if(userChoice < 0)
        {
            userChoice = INVALID;
        }

    }
    return userChoice;
}

// Function definition for cpuScheduling
void cpuScheduling()
{
    // Declare and initialize arrays for process IDs, arrival times, and burst times
    int process[] = {1, 2, 3, 4, 5};
    int arrvTime[] = {0, 2, 4, 6, 7};
    int burstTime[] = {8, 5, 10, 2, 3};

    // Call function fcfs passing arguments
    fcfs(process, arrvTime, burstTime);

    // Call function sjf passing arguments
    sjf(process, arrvTime, burstTime);
}

// Function definition for fcfs
void fcfs(int process[], int at[], int bt[])
{
    // Declare arrays for wait times and turnaround times
    int wt[PID];
    int tat[PID];

    // Initialize wait time for the first process to 0
    wt[0] = 0;

    // Calculate wait times for each process
    for (int i = 1; i < PID; i++)
    {
        wt[i] = wt[i - 1] + bt[i - 1];
    }

    // Calculate turnaround times for each process
    for (int i = 0; i < PID; i++)
    {
        tat[i] = bt[i] + wt[i];
    }

    // Call function displaySchedule
    printf("****************FCFS***************\n");
    displaySchedule(process, at, bt, wt, tat);
}

// Function definition for sjf
void sjf(int process[], int at[], int bt[])
{
    // Declare arrays for wait times and turnaround times
    int wt[PID];
    int tat[PID];

    // Declare variables for sorting
    int idx;
    int temp;

    // Nested loop to sort the process data based on burst time
    for (int i = 0; i < PID; i++)
    {
        int idx = i;
        for (int j = i + 1; j < PID; j++)
        {
            if (bt[j] < bt[idx])
            {
                idx = j;
            }
        }

        // Swap burst times
        int temp = bt[i];
        bt[i] = bt[idx];
        bt[idx] = temp;

        // Swap process IDs
        temp = process[i];
        process[i] = process[idx];
        process[idx] = temp;

        // Swap arrival times
        temp = at[i];
        at[i] = at[idx];
        at[idx] = temp;
    }

    // Calculate wait times and turnaround times
    wt[0] = 0; // First process has 0 wait time
    tat[0] = bt[0]; // First process has turnaround time equal to its burst time

    // Calculate wait times and turnaround times for remaining processes
    for (int i = 1; i < PID; i++)
    {
        wt[i] = wt[i - 1] + bt[i - 1]; // Wait time for the current process
        tat[i] = wt[i] + bt[i]; // Turnaround time for the current process
    }

    // Call displaySchedule function
    printf("****************SJF****************\n");
    displaySchedule(process, at, bt, wt, tat);
}

//Function Definition for threadFuncOne
void *threadFuncOne(void *arg)
{
    // Declare variables
    int threadId = 1;
    int fOne = resource;

    // Output the initial value of the shared resource
    printf("Thread 1 reads the value of shared resource as %d\n", fOne);

    // Increment local variable fOne
    fOne++;

    // Output the updated value of fOne
    printf("Local update by Thread 1: %d\n", fOne);

    // Simulate preemption by sleeping for one second
    sleep(1);

    // Update the shared resource value
    resource = fOne;

    // Output the updated value of the shared resource
    printf("Value of shared resource updated by Thread 1 is: %d\n", resource);

    // Exit the thread
    pthread_exit(NULL);
}

//Function Definition for threadFunctwo
void *threadFuncTwo(void *arg)
    {
    // Declare variables
    int threadId = 2;
    int fTwo = resource;

    // Output the initial value of the shared resource
    printf("Thread 2 reads resource as %d\n", fTwo);

    // Decrement local variable fTwo
    fTwo--;

    // Output the updated value of fTwo
    printf("Local update by Thread 2: %d\n", fTwo);

    // Simulate preemption by sleeping for one second
    sleep(1);

    // Update the shared resource value
    resource = fTwo;

    // Output the updated value of the shared resource
    printf("Value of shared resource updated by Thread 2 is: %d\n", resource);

    // Exit the thread
    pthread_exit(NULL);
}

//Function to create threads and handle the race condition
void raceCondition(void)
    {
    pthread_t threadOne;
    pthread_t threadTwo;

    // Create threadOne
    pthread_create(&threadOne, NULL, threadFuncOne, NULL);

    // Create threadTwo
    pthread_create(&threadTwo, NULL, threadFuncTwo, NULL);

    // Wait for threadOne to finish
    pthread_join(threadOne, NULL);

    // Wait for threadTwo to finish
    pthread_join(threadTwo, NULL);

    // Output the final value of the shared resource
    printf("Value of the shared resource is %d\n", resource);
}

// Function definition for displaySchedule
void displaySchedule(int process[], int at[], int bt[], int wt[], int tat[])
    {
    // Declare variables for total wait times, total turnaround times, average wait times, and average turnaround times
    int totWt = 0;
    int totTat = 0;
    float avgWt = 0;
    float avgTat = 0;

    // Iterate through each process
    printf("PID\tAT\tBT\tWT\tTAT\n");
    printf("---\t---\t---\t---\t---\t\n");
    for (int i = 0; i < PID; i++)
    {
        // Update total wait times and total turnaround times
        totWt += wt[i];
        totTat += tat[i];
        // Display process data
        printf("%d\t%d\t%d\t%d\t%d", process[i], at[i], bt[i], wt[i], tat[i]);
        printf("\n");
    }

    // Calculate average wait time and average turnaround time
    avgWt = (float)totWt / PID;
    avgTat = (float)totTat / PID;

    // Display average wait time and average turnaround time
    printf("Average Waiting Time: %.2f\n", avgWt);
    printf("Average Turnaround Time: %.2f\n", avgTat);
    printf("\n");
}

void bankersAlgorithm()
{
    // Declare and initialize the allocation array
    int allocation[5][3] = {
        {0, 0, 2},
        {3, 0, 2},
        {0, 1, 0},
        {2, 1, 1},
        {2, 0, 0}
    };

    // Declare and initialize the maximum demand array
    int maxDemand[5][3] = {
        {4, 3, 3},
        {9, 0, 2},
        {7, 5, 3},
        {2, 2, 2},
        {3, 2, 2}
    };

    // Declare and initialize the available resources array
    int available[3] = {2, 4, 6};

    // Declare the needed allocations array
    int need[5][3];

    // Declare the feasible resource allocation array
    int feasible[5] = {0};

    // Declare the safe resource allocation sequence array
    int safe[5];

    // Initialize safeIdx
    int safeIdx = 0;

    // Iterate for the number of processes
    for (int p = 0; p < PROCESS; p++)
    {
        // Set feasible[p] to FALSE
        feasible[p] = 0;

        // Calculate the needed resources
        for (int r = 0; r < RESOURCE; r++)
    {
            need[p][r] = maxDemand[p][r] - allocation[p][r];
        }
    }

    // Initialize isUnsafe
    int isUnsafe = 0;

    for (int p = 0; p < PROCESS; p++)
    {
        // If feasible[p] is FALSE
        if (feasible[p] == 0)
    {
            // Reset isUnsafe
            isUnsafe = 0;

            // Iterate for the number of resources
            for (int r = 0; r < RESOURCE; r++)
    {
                // If need[p][r] is greater than available[r]
                if (need[p][r] > available[r])
    {
                    isUnsafe = 1; // Set isUnsafe to TRUE
                    break; // Break out of the loop
                }
            }

            // If isUnsafe is FALSE
            if (isUnsafe == 0)
    {
                // Save the safe state for the process
                safe[safeIdx++] = p;

                // Release resources for other processes
                for (int r = 0; r < RESOURCE; r++)
    {
                    available[r] += allocation[p][r];
                }

                // Set feasible[p] to TRUE
                feasible[p] = 1;

                // Check system state
                systemState(feasible, safe);
            }
        }
    }
    void systemState(int feasible[], int safe[]);
}

void systemState(int feasible[], int safe[])
    {
    // Declare variable to track safety
    int isSafe = 1; // Assume the system is safe initially

    // Iterate over the processes
    for (int i = 0; i < PROCESS; i++)
    {
        // If the process is not feasible
        if (feasible[i] == 0)
    {
            isSafe = 0; // System is not safe
            printf("The operating system is not safe\n");
            break; // Break out of the loop
        }
    }

    // If the system is safe, print the safe sequence
    if (isSafe == 1)
    {
        printf("Safe resource allocation to process sequence\n");
        for (int i = 0; i < PROCESS; i++)
    {
            printf(" P%d ->", safe[i]);
        }
        printf("\n");
    }
}

// Function memoryManagement
void memoryManagement()
{
    // Arrays to store block sizes and process sizes
    int blockSize[] = {70, 20, 45, 65, 40, 80};
    int processSize[] = {15, 35, 25, 45, 60, 20};

    // Variables to store number of blocks and processes
    int blocks = 6; // Number of blocks
    int processes = 6; // Number of processes

    // Looping variable for memory management algorithms
    int algorithm;
    for (algorithm = 0; algorithm < 4; algorithm++)
    {
        // Decision making based on algorithm value
        if(algorithm == FIRST)
        {
            firstFit(blockSize, blocks, processSize, processes);
        }
        if(algorithm == BEST)
        {
            bestFit(blockSize, blocks, processSize, processes);
        }
        if(algorithm == WORST)
        {
            worstFit(blockSize, blocks, processSize, processes);
        }
        if(algorithm == NEXT)
        {
            nextFit(blockSize, blocks, processSize, processes);
        }
    }
}

// Function nextFit
void nextFit(int blockSize[], int blocks, int processSize[], int processes)
{
    // Array to store block allocation for each process
    int allocation[processes];

    // Variable to store block allocation for a process
    int id = 0;
    memset(allocation, INVALID, sizeof(allocation));

    // Loop through the number of processes
    for (int i = 0; i < processes; i++)
    {
        // Loop while id is less than the number of blocks
        while (id < blocks)
    {
            // If current block size is greater than or equal to current process size
            if (blockSize[id] >= processSize[i])
    {
                // Update allocation array
                allocation[i] = id;
                // Reduce available memory of current block
                blockSize[id] -= processSize[i];
                break; // Exit inner loop
            }
            // Update id to next block
            id = (id + 1) % blocks;
        }
    }

    // Display process allocation
    printf("****************** Next Fit ******************\n");
    displayProcess(allocation, processes, processSize);
}

// Function firstFit
void firstFit(int blockSize[], int blocks, int processSize[], int processes)
{
    // Array to store block allocation for each process
    int allocation[processes];

    // Initialize allocation array with INVALID (-1)
    memset(allocation, INVALID, sizeof(allocation));

    // Loop through the number of processes
    for (int i = 0; i < processes; i++)
    {
        // Loop through the number of blocks
        for (int j = 0; j < blocks; j++)
    {
            // If current block size is greater than or equal to current process size
            if (blockSize[j] >= processSize[i])
    {
                // Update allocation array
                allocation[i] = j;
                // Reduce available memory of current block
                blockSize[j] -= processSize[i];
                break; // Exit inner loop
            }
        }
    }

    // Display process allocation
    printf("****************** First Fit ******************\n");
    displayProcess(allocation, processes, processSize);
}

// Function bestFit
void bestFit(int blockSize[], int blocks, int processSize[], int processes)
{
    // Array to store block allocation for each process
    int allocation[processes];

    // Initialize allocation array with INVALID (-1)
    memset(allocation, INVALID, sizeof(allocation));

    // Loop through the number of processes
    for (int i = 0; i < processes; i++)
    {
        // Variable to store the current best fit value
        int bestIdx = INVALID;

        // Loop through the number of blocks
        for (int j = 0; j < blocks; j++)
    {
            // If current block size is greater than or equal to current process size
            if (blockSize[j] >= processSize[i])
    {
                // If bestIdx is INVALID or current block size is smaller than previous best fit
                if (bestIdx == INVALID || blockSize[j] < blockSize[bestIdx])
    {
                    bestIdx = j;
                }
            }
        }

        // If bestIdx is not INVALID
        if (bestIdx != INVALID)
    {
            // Update allocation array
            allocation[i] = bestIdx;
            // Reduce available memory of current block
            blockSize[bestIdx] -= processSize[i];
        }
    }

    // Display process allocation
    printf("****************** Best Fit ******************\n");
    displayProcess(allocation, processes, processSize);
}

// Function worstFit
void worstFit(int blockSize[], int blocks, int processSize[], int processes)
{
    // Array to store block allocation for each process
    int allocation[processes];

    // Initialize allocation array with INVALID (-1)
    memset(allocation, INVALID, sizeof(allocation));

    // Loop through the number of processes
    for (int i = 0; i < processes; i++)
    {
        // Variable to store the current worst fit value
        int wstIdx = INVALID;

        // Loop through the number of blocks
        for (int j = 0; j < blocks; j++)
    {
            // If current block size is greater than or equal to current process size
            if (blockSize[j] >= processSize[i])
    {
                // If wstIdx is INVALID or current block size is larger than previous worst fit
                if (wstIdx == INVALID)
                {
                    wstIdx = j;
                }
                if (blockSize[j] > blockSize[wstIdx])
                {
                    wstIdx = j;
                }
            }
        }

        if (wstIdx != INVALID)
    {

            allocation[i] = wstIdx;
            blockSize[wstIdx] -= processSize[i];
        }
    }

    // Display process allocation
    printf("****************** Worst Fit ******************\n");
    displayProcess(allocation, processes, processSize);
}

// Function displayProcess
void displayProcess(int allocation[], int processes, int processSize[])
    {
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < processes; i++)
    {
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if (allocation[i] != -1)
    {
            printf("%d\n", allocation[i]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

//Assignment 7

// Function to perform page replacement using FIFO or LRU algorithm
void pageReplacement()
{
    printf("****************** Page Replacement ******************\n\n");
    int algorithm;
    // Looping construct to choose between FIFO and LRU algorithms
    for (algorithm = 0; algorithm < 2; algorithm++)
    {
        if (algorithm == FIFO)
        {
            printf("****************** Fisrt In First Out ******************\n\n");
            fifo(); // Call FIFO function
        }
        if (algorithm == LRU)
        {
            printf("****************** Least Recently Used ******************\n\n");
            lru(); // Call LRU function
        }
    }
}

// Function Fifo
void fifo()
{
    printf("\tPage\tFrame 1\tFrame 2\tFrame 3\tFrame 4\n");
    
    int pageRequests[] = {2, 3, 8, 4, 5, 6, 5, 7, 1, 8, 3, 1, 4, 2, 6};
    int allocation[FRAMES];
    int present = 0;
    int pageFaults = 0;
    int pages = 15;

    memset(allocation, INVALID, sizeof(allocation));

    for (int i = 0; i < pages; i++)
    {
        present = 0;
        for (int j = 0; j < FRAMES; j++)
        {
            if (pageRequests[i] == allocation[j])
            {
                present = 1;
                pageFaults--;
                break;
            }
        }

        pageFaults++;

        if (pageFaults <= FRAMES && present == 0)
        {
            allocation[pageFaults - 1] = pageRequests[i];
        }
        if (present == 0)
        {
            allocation[(pageFaults - 1) % FRAMES] = pageRequests[i];
        }

        displayPages(pageRequests[i], allocation);
    }

    printf("\nTotal Page Faults: %d\n\n", pageFaults);
}

// Function LRU
void lru()
{
    printf("\tPage\tFrame 1\tFrame 2\tFrame 3\tFrame 4\n");
    
    int pageRequests[] = {2, 3, 8, 4, 5, 6, 5, 7, 1, 8, 3, 1, 4, 2, 6};
    int pageFaults = 0;
    int allocation[FRAMES];
    int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);
    int counter = 0;
    int time[10] = {0};
    int flag1, flag2, position;

    memset(allocation, INVALID, sizeof(allocation));

    for (int i = 0; i < pages; i++)
    {
        flag1 = 0;
        flag2 = 0;
        
        for (int j = 0; j < FRAMES; j++)
    {
            if (pageRequests[i] == allocation[j])
    {
                counter++;
                time[j] = counter;
                flag1 = 1;
                flag2 = 1;
                break;
            }
        }

        if (flag1 == 0)
    {
            for (int j = 0; j < FRAMES; j++)
    {
                if (allocation[j] == INVALID)
    {
                    counter++;
                    pageFaults++;
                    allocation[j] = pageRequests[i];
                    time[j] = counter;
                    flag2 = 1;
                    break;
                }
            }
        }

        if (flag2 == 0)
    {
            position = findLRU(time);
            counter++;
            pageFaults++;
            allocation[position] = pageRequests[i];
            time[position] = counter;
        }

        displayPages(pageRequests[i], allocation);
    }

    printf("\nTotal Page Faults: %d\n", pageFaults);
}

// Function LRU
int findLRU(int time[])
{
    int position = 0;
    int minimum = time[0];

    for (int i = 1; i < FRAMES; i++)
    {
        if (time[i] < minimum)
    {
            minimum = time[i];
            position = i;
        }
    }

    return position;
}

// Function to Display Pages
void displayPages(int page, int allocation[])
{
    printf("\t%d\t", page);
    for (int i = 0; i < FRAMES; i++)
    {
        if (allocation[i] == INVALID)
        {
            printf("-\t");
        } else {
            printf("%d\t", allocation[i]);
        }
    }
    printf("\n");
}

//Function for Disk Scheduling
void diskScheduling()
{
    // Declare and initialize disk requests array
    int requests[] = {146, 89, 24, 70, 102, 13, 51, 134};
    int numRequests = REQUEST;
    int head = 50;

    printf("****************** Disk Scheduling ******************\n\n");
    for (int algorithm = 0; algorithm < 2; algorithm++)
    {
        // Decision making logic based on the value of the looping variable (algorithm)
        if(algorithm == FCFS)
        {
            printf("****************** FCFS ******************\n\n");
            diskFcfs(requests, head);  
        }
        if(algorithm == SSTF)
        {
            printf("****************** SSTF ******************\n\n");
            diskSstf(requests, head);
        }
    }
}

//function FCFS
void diskFcfs(int requests[], int head)
{
    // Declare variables
    int seek = 0;
    int track = 0;
    int distance = 0;
    int start = head;
    printf("Seek Sequence: ");
    printf("%d ", head);

    // Iterate through the requests
    for (int i = 0; i < REQUEST; i++)
    {
        track = requests[i];
        distance = abs(head - track);
        seek += distance;
        head = track;
    }

    // Display seek sequence and total seek time
    for (int i = 0; i < REQUEST; i++)
    {
        printf("-> %d ", requests[i]);
    }
    printf("\nTotal Seek Operations: %d\n\n", seek);
}

//Function SSTF
void diskSstf(int requests[], int head)
{
    // Declare variables
    int sequence[REQUEST];
    int distance[REQUEST];
    int seek = 0;
    int start = head;
    int minVal = 0;
    int minValIdx = 0;
    int seqIdx = 0;

    // Iterate through the number of REQUESTS
    printf("Seek Sequence: ");
    printf("%d ", head);
    for (int i = 0; i < REQUEST; i++)
    {
        // Calculate the distance between requests
        for (int j = 0; j < REQUEST; j++)
        {
            distance[j] = abs(head - requests[j]);
        }

        // Find the minimum value and its index
        minVal = distance[0];
        minValIdx = 0;
        for (int j = 1; j < REQUEST; j++)
        {
            if (minVal > distance[j])
            {
                minVal = distance[j];
                minValIdx = j;
            }
        }

        // Update the sequence array
        sequence[seqIdx] = requests[minValIdx];
        seqIdx++;
        head = requests[minValIdx];
        requests[minValIdx] = 999;
        seek += abs(start - sequence[seqIdx - 1]);

        printf("-> %d ", sequence[seqIdx - 1]);
    }
    printf("\nTotal Seek Operations: %d\n", seek);
}