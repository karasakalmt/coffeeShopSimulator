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
    struct customerQueue *customerList;
    customerList = parseInput(argv,&noOfRobots ,&t_maxService,&t_maxArrival);
    createCustomerList(customerList,t_maxService,t_maxArrival);
    int *robotAvailability;//array to see if robots are available
    robotAvailability = (int *) malloc(sizeof(int)*noOfRobots);
    struct customerQueue *c_queue;//struct pointer to keep the list of active customers
    c_queue=initialiseSimulator(robotAvailability,noOfRobots);

    while(c_queue->size!=customerList->size)//the loop that manages the whole simulation
    {
        clock++;
        newCustomer(customerList ,c_queue, &clock);
        if(countItem(c_queue)>0)//when queue is not empty do!
        {
            waitTimeAdding(c_queue);
        }
        displayQueue(clock,c_queue);

    }


    displayCustomers(customerList);

    return 0;
}
