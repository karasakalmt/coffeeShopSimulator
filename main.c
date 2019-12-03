/*
 * Name & Surname: Mete Karasakal
 * Student ID:2315380
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

int main(int argc,char* argv[]) {
    srand(time(NULL));//seeds the random
    int clock=0;//main clock for all the code
    if(argc>5)
    {printf("Too many arguments, expected is 4.\n");return 0;}
    else if(argc<5)
    {printf("Too less arguments, expected is 4.\n");return 0;}
    int noOfRobots,t_maxService,t_maxArrival;//defines number of robots, maximum service time, and maximum arrival time
    struct listRecord *customerList;
    customerList = parseInput(argv,&noOfRobots ,&t_maxService,&t_maxArrival);
    createCustomerList(customerList,t_maxService,t_maxArrival);
    int *robotAvailability;
    struct customerQueue *c_queue;
    c_queue=initialiseSimulator(robotAvailability,noOfRobots);

    displayCustomers(customerList);

    return 0;
}
