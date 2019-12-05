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

    //arguement errors
    if(argc>5)
    {printf("Too many arguments, expected is 4.\n");return 0;}
    else if(argc<5)
    {printf("Too less arguments, expected is 4.\n");return 0;}

    //input parse process
    int noOfRobots,t_maxService,t_maxArrival;//defines number of robots, maximum service time, and maximum arrival time
    struct customerQueue *customerList;
    customerList = parseInput(argv,&noOfRobots ,&t_maxService,&t_maxArrival);

    //creates main list
    createCustomerList(customerList,t_maxService,t_maxArrival);

    //simulation initialization
    int *robotAvailability,*robotServed;//robotAvailability array to see if robots are available, robotServed to find each robots serve ammount
    robotAvailability = (int *) malloc(sizeof(int)*noOfRobots);
    robotServed = (int *) malloc(sizeof(int)*noOfRobots);
    struct customerQueue *c_queue;//struct pointer to keep the list of active customers
    c_queue=initialiseSimulator(robotAvailability,noOfRobots,robotServed);

    displayCustomers(customerList);

    //the loop that manages the whole simulation
    while(c_queue->size!=customerList->size)
    {
        clock++;
        newCustomer(customerList ,c_queue, clock);
        //dequeue(c_queue);
        serveCustomer(c_queue, robotAvailability, noOfRobots, robotServed);
        if(countItem(c_queue)>0)//when queue is not empty do!
        {
            waitTimeAdding(c_queue);
        }
        displayQueue(clock,c_queue);
    }


    displayCustomers(customerList);

    return 0;
}
