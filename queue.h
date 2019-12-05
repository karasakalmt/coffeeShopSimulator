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