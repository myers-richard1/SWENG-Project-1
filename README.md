# SWENG-Project-1
CSUBatch Readme

This is Richard and Harrison's Group Project 1, CSUBatch.

Configuration:
GCC is required to compile this program. Otherwise, no project-specific configuration is necessary.

Installation:
Simply run make in this directory to compile all necessary executables.

Usage:
The program runs in 2 modes. The first is user mode.
User mode:
    To run in user mode, type
    ./app.out

    This will launch the application in user mode, with additional instructions for each command.
    For convenience, the commands are:
    run <job> <time> <priority>: submit a job named <job>, execution time is <time>, priority is <priority>
    list: display the job status.
    "fcfs: change the scheduling policy to first come first serve.
    sjf: change the scheduling policy to shortest job first.
    priority: change the scheduling policy to priority.
    test <benchmark> <policy> <num_of_jobs> <priority_levels> <min_CPU_time> <max_CPU_time>
    quit: exit CSUbatch

The second mode is test mode, which runs the unit tests.
Test mode:
    To run in test mode, type
    ./app.out test

    This will launch the application in test mode, testing the core functions and printing the result.