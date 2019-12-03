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

struct listRecord
{
    pcustomer head;
    pcustomer tail;
    int size;
};

struct customerQueue{
    pcustomer front;
    pcustomer rear;
};
typedef struct customerQueue *cqueue;// defined to use it easier in future usage

struct listRecord* parseInput(char* argv[],int *noOfRobots,int *t_maxService,int *t_maxArrival)
{//this function parses the information from argv and returns thegi list and pointers
    struct listRecord* list;
    list = (struct listRecord*) malloc(sizeof(struct listRecord));
    list->size=atoi(argv[1]);
    *noOfRobots=atoi(argv[2]);
    *t_maxArrival=atoi(argv[3]);
    *t_maxService=atoi(argv[4]);
    return list;
}

void createCustomerList(struct listRecord *customerList,int t_maxService,int t_maxArrival)
{
    pcustomer newCustomer,tmp;
    int rand_num,inner_count;// rand num to use in loop
    newCustomer=(struct customer*) malloc(sizeof(struct customer));//I have made a dummy node.
    customerList->head=newCustomer;
    customerList->tail=newCustomer;
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
        tmp=customerList->head;
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
                    customerList->tail = newCustomer;
                    break;
                }
                tmp = tmp->next;
                inner_count++;
                }

            }
    }
}

struct customerQueue * initialiseSimulator(int *robotAvailability,int noOfRobots)
{
    for(int count=0;count<noOfRobots;count++)
    {
        robotAvailability[count]=1;
    }
    cqueue cust_q;
    cust_q=(struct customerQueue *) malloc(sizeof(struct customerQueue));
    return cust_q;
}

void newCustomer()//Because of i already have ordered the queue in createCustomerList function with their arrival time and priorities, this function only will add the customers to the c_queue
{

}

void displayCustomers(struct listRecord *customerList)
{
    pcustomer tmp;
    tmp=customerList->head;
    for(int i=0;i<customerList->size;i++)
    {
        tmp=tmp->next;
        printf("%d %d\n",tmp->type,tmp->t_arrival);
    }
}