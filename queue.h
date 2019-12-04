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

struct customerQueue* parseInput(char* argv[], int *noOfRobots, int *t_maxService, int *t_maxArrival)
{//this function parses the information from argv and returns thegi list and pointers
    cqueue list;
    list = (struct customerQueue*) malloc(sizeof(struct customerQueue));
    list->size=atoi(argv[1]);
    *noOfRobots=atoi(argv[2]);
    *t_maxArrival=atoi(argv[3]);
    *t_maxService=atoi(argv[4]);
    return list;
}

void createCustomerList(struct customerQueue *customerList, int t_maxService, int t_maxArrival)
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

struct customerQueue * initialiseSimulator(int *robotAvailability, int noOfRobots)
{
    for(int count=0;count<noOfRobots;count++)
    {
        robotAvailability[count]=1;
    }
    cqueue c_queue;//to simulate current queue
    c_queue=(struct customerQueue *) malloc(sizeof(struct customerQueue));
    c_queue->front=NULL;
    c_queue->rear=NULL;
    c_queue->size=0;
    return c_queue;
}

void newCustomer(struct customerQueue *customerList ,struct customerQueue *c_queue, int *clock)//Because of i already have ordered the queue in createCustomerList function with their arrival time and priorities, this function only will add the customers to the c_queue
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
        c_queue->front = tmp;
        c_queue->rear = tmp;
        tmp->queuenext=NULL;
        c_queue->size++;
        *clock=tmp->t_arrival;
    }
    if(clock==tmp->next->t_arrival)
    {
        if(c_queue->front!=NULL)
        {
            tmp->queuenext = tmp->next;
            tmp = tmp->queuenext;
            c_queue->size++;
        }

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

void displayCustomers(struct customerQueue *customerList)
{
    pcustomer tmp;
    tmp=customerList->front;
    for(int i=0;i<customerList->size;i++)
    {
        tmp=tmp->next;
        printf("%d %d\n",tmp->type,tmp->t_arrival);
    }
}

void displayQueue(int clock,struct customerQueue *c_queue)
{
    pcustomer tmp;
    tmp=c_queue->front;
    printf("\n%d item\n%d. stage\n\n",c_queue->size,clock);
    while(tmp->queuenext!=NULL)
    {
        tmp=tmp->queuenext;
        printf("%d %d\n",tmp->type,tmp->t_arrival);

    }
}