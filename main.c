/*
 * Name & Surname: Mete Karasakal
 * Student ID:2315380
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

//until the other comment line there are function prototypes will be shown

struct customerQueue* parseInput(char* argv[], int *, int *, int *);
void createCustomerList(struct customerQueue *, int , int);
struct customerQueue * initialiseSimulator(int *, int, int *);
void newCustomer(struct customerQueue * ,struct customerQueue *, int );
int countItem(struct customerQueue *);
struct customer* dequeue(struct customerQueue *);
void waitTimeAdding(struct customerQueue *);
int isRobotsAvailable(int *, int );
void serveCustomer(struct customerQueue *, int *, int , int *);
int mainLoopController(struct customerQueue *, struct customerQueue *, int *, int);
void reportStatistics(struct customerQueue *,int *, int , int);

void displayCustomers(struct customerQueue *);
void displayQueue(int ,struct customerQueue *);

//function prototypes are done

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

    //displayCustomers(customerList);//shows customer list(optional usage)

    //the loop that manages the whole simulation
    while(mainLoopController(c_queue,customerList,robotAvailability,noOfRobots))
    {
        clock++;
        newCustomer(customerList ,c_queue, clock);
        serveCustomer(c_queue, robotAvailability, noOfRobots, robotServed);
        waitTimeAdding(c_queue);
        //displayQueue(clock,c_queue);//can be used to see the procces inside main loop(optional usage)
    }


    //displayCustomers(customerList);//can be used to see customer list(optional usage)
    reportStatistics(customerList,robotServed,noOfRobots,clock);

    return 0;
}



//Functions begins at that line, all the function has a comment line at the beginning that shows that what function does
struct customerQueue* parseInput(char* argv[], int *noOfRobots, int *t_maxService, int *t_maxArrival)//splits the input and distributes it
{//this function parses the information from argv and returns the list and pointers
    cqueue list;
    list = (struct customerQueue*) malloc(sizeof(struct customerQueue));
    list->size=atoi(argv[1]);
    *noOfRobots=atoi(argv[2]);
    *t_maxArrival=atoi(argv[3]);
    *t_maxService=atoi(argv[4]);
    return list;
}

void createCustomerList(struct customerQueue *customerList, int t_maxService, int t_maxArrival)//Create main list with their membership priority and arrival time
{
    pcustomer newCustomer,tmp;
    int rand_num,inner_count;// rand num to use in loop
    newCustomer=(struct customer*) malloc(sizeof(struct customer));//I have made a dummy node.
    customerList->front=newCustomer;
    customerList->rear=newCustomer;
    newCustomer->type=3;
    newCustomer->t_arrival=t_maxArrival+1;
    newCustomer->t_service=t_maxService+1;
    newCustomer->next=NULL;
    for(int count=0;count<customerList->size;count++)//whole loop initializes customer list
    {
        newCustomer=(struct customer*) malloc(sizeof(struct customer));
        newCustomer->coffee_size=rand()%3;// 0 --> tall, 1-->grande, 2--> venti
        newCustomer->coffee_type=rand()%4;// 0 --> espresso, 1 --> americano, 2 --> latte, 3 --> cappuccino
        newCustomer->type=rand()%3;// 0 --> Platinium, 1 --> Gold, 2 --> Silver
        newCustomer->total_wait=0;
        rand_num=0;
        while(rand_num==0)
            rand_num=rand()%t_maxArrival;
        newCustomer->t_arrival=rand_num;
        rand_num=0;
        while(rand_num==0)
            rand_num=rand()%t_maxService;
        newCustomer->t_service=rand_num;
        tmp=customerList->front;
        inner_count=0;
        if(count==0)
        {
            tmp->next=newCustomer;
            newCustomer->next=NULL;
        }
        else {
            while (inner_count < count) {//this loop checks priority and orders the queue in respects of arrival time and priority of membership

                if (tmp->next->t_arrival >= newCustomer->t_arrival) {
                    if ((newCustomer->type <= tmp->next->type && tmp->next->t_arrival == newCustomer->t_arrival)||(tmp->next->t_arrival > newCustomer->t_arrival)) {
                        newCustomer->next = tmp->next;
                        tmp->next = newCustomer;
                    }
                    else {
                        newCustomer->next = tmp->next->next;
                        tmp->next->next = newCustomer;
                    }
                    break;
                }
                else if (tmp->next->next == NULL && tmp->next->t_arrival < newCustomer->t_arrival) {
                    tmp->next->next = newCustomer;
                    newCustomer->next = NULL;
                    customerList->rear = newCustomer;
                    break;
                }
                tmp = tmp->next;
                inner_count++;
            }

        }
    }
}

struct customerQueue * initialiseSimulator(int *robotAvailability, int noOfRobots, int *robotServed)//starts the simulation by initializing robots and initializing a empty queue for current list tracking in main loop
{
    for(int count=0;count<noOfRobots;count++)
    {
        robotAvailability[count]=0;
        robotServed[count]=0;
    }
    cqueue c_queue;//to simulate current queue
    c_queue=(struct customerQueue *) malloc(sizeof(struct customerQueue));
    c_queue->front=NULL;
    c_queue->rear=NULL;
    c_queue->size=0;
    return c_queue;
}

void newCustomer(struct customerQueue *customerList ,struct customerQueue *c_queue, int clock)//Because of i already have ordered the queue in createCustomerList function with their arrival time and priorities, this function only will add the customers to the c_queue in respect of clock
{
    pcustomer tmp;
    tmp=customerList->front->next;
    int count;
    for(count=1;count<c_queue->size;count++)//travers to last item added
    {
        if(tmp->next==NULL)
            break;
        tmp=tmp->next;
    }
    if(c_queue->front==NULL)
    {
        for(;count<=c_queue->size;count++)//travers to last item added
        {
            if(tmp->next==NULL)
                break;
            tmp=tmp->next;
        }
        if(tmp->t_arrival==clock) {
            c_queue->front = tmp;
            c_queue->rear = tmp;
            tmp->queuenext = NULL;
            c_queue->size++;
        }
    }
    if(tmp->next!= NULL)
        if(clock==tmp->next->t_arrival)
        {
            if(c_queue->front!=NULL)
            {
                tmp->queuenext = tmp->next;
                tmp = tmp->queuenext;
                c_queue->size++;
            }
            if(tmp->next!= NULL)
                while(tmp->t_arrival==tmp->next->t_arrival)
                {
                    tmp->queuenext = tmp->next;
                    tmp = tmp->queuenext;
                    c_queue->size++;
                    if (tmp->next==NULL)
                        break;
                }
            c_queue->rear = tmp;
            tmp->queuenext=NULL;
        }
}

int countItem(struct customerQueue *c_queue)//counts the items in current queue to use in main and dequeue function
{
    pcustomer tmp;
    int count=0;
    tmp=c_queue->front;
    if(tmp==NULL)
        return count;
    do
    {
        tmp=tmp->queuenext;
        count++;
    }
    while(tmp!=NULL);
    return count;
}

struct customer* dequeue(struct customerQueue *c_queue)//simply dequeues
{
    pcustomer tmp;

    if (countItem(c_queue)==0)
        return NULL;

    tmp=c_queue->front;

    if(tmp->queuenext==NULL)
    {
        c_queue->front=NULL;
        c_queue->rear=NULL;
    }
    else
    {
        c_queue->front=tmp->queuenext;
    }
    return tmp;
}

void waitTimeAdding(struct customerQueue *c_queue)//adds time to customer how much they waited before they go servecustomer
{
    if(countItem(c_queue)!=0) {
        pcustomer tmp;
        tmp = c_queue->front;
        while (tmp != NULL) {
            tmp->total_wait++;
            tmp = tmp->queuenext;
        }
    }
}

int isRobotsAvailable(int *robotAvailability, int noOfRobots)//returns robots index if it is available otherwise will return -1
{
    for(int count=0;count<noOfRobots;count++)
    {
        if(robotAvailability[count]==0)
            return count;
    }
    return -1;
}

void serveCustomer(struct customerQueue *c_queue, int *robotAvailability, int noOfRobots, int *robotServed)//dequeues customer and serves them
{
    int index=isRobotsAvailable(robotAvailability,noOfRobots);
    if(index!=-1 && c_queue->front!=NULL)
    {
        pcustomer serving;
        while (index!=-1 && c_queue->front!=NULL) {
            serving = dequeue(c_queue);
            serving->total_wait += serving->t_service;
            robotAvailability[index]+=serving->t_service;
            index=isRobotsAvailable(robotAvailability,noOfRobots);
        }
    }
    for(int count=0;count<noOfRobots;count++)
    {
        if(robotAvailability[count]==0)
            continue;
        else
        {
            robotAvailability[count]--;
            if(robotAvailability[count]==0)
                robotServed[count]++;
        }
    }
}

int mainLoopController(struct customerQueue *c_queue, struct customerQueue *customerList, int *robotAvailability, int noOfRobots)//if any of the robots busy or all the custmer not added yet dont end the loop
{
    if(c_queue->size!=customerList->size)
        return 1;
    if(c_queue->front!=NULL && c_queue->size==customerList->size)
        return 1;
    for(int count=0;count<noOfRobots;count++)
    {
        if(robotAvailability[count]!=0)
            return 1;
    }
    return 0;
}

void reportStatistics(struct customerQueue *customerList,int *robotServed, int noOfRobots, int clock)//gives statistics of customers (whole output)
{
    int averageWait=0,maxWait,popularCoffee,*coffeeChosen;
    coffeeChosen=(int *) malloc(sizeof(int)*4);
    pcustomer tmp;//customer pointer to travers
    tmp=customerList->front->next;
    printf("\n\nNumber of robots: %d",noOfRobots);
    printf("\nNumber of customers: %d\n\n",customerList->size);
    printf("The number of customers for each robot:");
    for(int count=0;count<noOfRobots;count++)
    {
        printf("\nRobot %d ID served: %d people",count,robotServed[count]);
    }
    printf("\n\nCompletion time: %d",clock);

    maxWait=tmp->total_wait;
    while(tmp!=NULL)
    {
        averageWait+=tmp->total_wait;
        if(tmp->total_wait>maxWait)
            maxWait=tmp->total_wait;
        popularCoffee=tmp->coffee_type;
        coffeeChosen[popularCoffee]++;
        tmp=tmp->next;
    }


    averageWait/=customerList->size;
    printf("\nAverage time spent in the queue: %d",averageWait);
    printf("\nMaximum waiting time: %d",maxWait);

    popularCoffee=0;
    for(int count=0;count<4;count++)
    {
        if(coffeeChosen[popularCoffee]<coffeeChosen[count])
            popularCoffee=count;
    }
    switch (popularCoffee){//0 --> espresso, 1 --> americano, 2 --> latte, 3 --> cappuccino
        case 0:
            printf("\nMost popular coffee type: Espresso");
            break;
        case 1:
            printf("\nMost popular coffee type: Americano");
            break;
        case 2:
            printf("\nMost popular coffee type: Latte");
            break;
        case 3:
            printf("\nMost popular coffee type: Cappuccino");
            break;
        default:
            break;
    }

}


//Functions to see simulation clearer
void displayCustomers(struct customerQueue *customerList)//Displays the customerList(main queue).I will leave the functions in main if you need to see the procces you can uncomment them and see the priority of the customer and arrival time of the customer
{
    pcustomer tmp;
    tmp=customerList->front;
    printf("\n\n");
    for(int i=0;i<customerList->size;i++)
    {
        tmp=tmp->next;
        printf("Customer Type:%d Arrival Time:%d Service Time:%d Total Wait:%d Coffee type:%d\n", tmp->type, tmp->t_arrival, tmp->t_service, tmp->total_wait, tmp->coffee_type);
    }
}

void displayQueue(int clock,struct customerQueue *c_queue)//Displays the c_queue(current queue).I will leave the functions in main if you need to see the procces you can uncomment them and see the priority of the customer, arrival time of the customer and service time
{
    pcustomer tmp;
    tmp=c_queue->front;
    printf("\n%d item\n%d. minute\n\n",c_queue->size,clock);
    for(int i=0;i<c_queue->size;i++)
    {
        if(tmp!=NULL) {
            printf("Customer Type:%d Arrival Time:%d Service Time:%d\n", tmp->type, tmp->t_arrival, tmp->t_service);
            tmp = tmp->queuenext;
        }
    }

}