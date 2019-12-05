/*
 * Name & Surname: Mete Karasakal
 * Student ID:2315380
 */
struct customer
{
    int type;
    int coffee_type;
    int coffee_size;
    int t_arrival;//time for arrival
    int t_service;//time for service
    int total_wait;// this is for see how much customer waited
    struct customer *next;
    struct customer *queuenext;//to keep track for using in queue
};
typedef struct customer *pcustomer;//defined pcustomer as pointer to easy usage after

struct customerQueue//this is for all the queue
{
    pcustomer front;
    pcustomer rear;
    int size;
};

typedef struct customerQueue *cqueue;// defined to use it easier in future usage

//until the other comment line there are function prototypes will be shown

struct customerQueue* parseInput(char* argv[], int *, int *, int *);
void createCustomerList(struct customerQueue *, int , int);
struct customerQueue * initialiseSimulator(int *, int, int *);
void newCustomer(struct customerQueue * ,struct customerQueue *, int );
int countItem(struct customerQueue *);
struct customer* dequeue(struct customerQueue *);
void waitTimeAdding(struct customerQueue *);
//will be changed
void displayCustomers(struct customerQueue *);
void displayQueue(int ,struct customerQueue *);

//function prototypes are done


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
        tmp=tmp->next;
    }
    if(c_queue->front==NULL)
    {
        for(;count<=c_queue->size;count++)//travers to last item added
        {
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
    pcustomer tmp;
    tmp=c_queue->front;
    while(tmp!=NULL)
    {
        tmp->total_wait++;
        tmp=tmp->queuenext;
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

void displayCustomers(struct customerQueue *customerList)//Displays the customerList(main queue).I will leave the functions in main if you need to see the procces you can uncomment them and see the priority of the customer and arrival time of the customer
{
    pcustomer tmp;
    tmp=customerList->front;
    printf("\n\n");
    for(int i=0;i<customerList->size;i++)
    {
        tmp=tmp->next;
        printf("%d %d\n",tmp->type,tmp->t_arrival);
    }
}

void displayQueue(int clock,struct customerQueue *c_queue)//Displays the c_queue(current queue).I will leave the functions in main if you need to see the procces you can uncomment them and see the priority of the customer and arrival time of the customer
{
    pcustomer tmp;
    tmp=c_queue->front;
    printf("\n%d item\n%d. minute\n\n",c_queue->size,clock);
    for(int i=0;i<c_queue->size;i++)
    {
        if(tmp!=NULL) {
            printf("%d %d\n", tmp->type, tmp->t_arrival);
            tmp = tmp->queuenext;
        }
    }

}