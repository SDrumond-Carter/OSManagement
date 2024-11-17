Embedded Operating System

This project is a simulation of an embedded operating system implemented in C. It includes core operating system functionalities such as CPU scheduling, Banker's algorithm, memory management, page replacement, and disk scheduling. The goal is to demonstrate these OS concepts in a modular and easy-to-understand manner.

Features:

*CPU Scheduling: Simulates scheduling algorithms such as First-Come-First-Serve (FCFS), Shortest Job Next (SJN), Round Robin (RR), and Priority Scheduling.

*Banker's Algorithm: Implements the Banker's algorithm to ensure safe resource allocation and avoid deadlocks.

*Memory Management: Includes dynamic memory allocation and deallocation, segmentation, and paging simulations.

*Page Replacement: Implements page replacement algorithms such as FIFO, Least Recently Used (LRU), and Optimal Page Replacement.

*Disk Scheduling: Simulates disk scheduling techniques like FCFS, Shortest Seek Time First (SSTF), SCAN, and C-SCAN.

compile: gcc OSManagement.c
run: ./a.out OSManagemnt.c
*make sure you are in the directory of the file
