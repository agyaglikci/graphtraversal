//
//  main.cpp
//  GraphTraversal
//
//  Created by Abdullah Giray Yaglikci on 2/2/15.
//  2015, University of Notre Dame.
//

#include "definitions.h"
#include "Graph.h"

using namespace std;
int32_t log_2(uint64_t number);
pthread_mutex_t mut;

class thread_args
{
public:
    uint32_t proc_num;
    uint32_t num_processors;
    Graph * myGraph;
};

void *gotoNext(void *arg)
{
    thread_args *p=(thread_args *)arg;
    int startIndex = p->proc_num;
    //string log = "";

    Node myNode = *p->myGraph->nodeArray[startIndex];
    
    for (int i = 0 ; i < NUM_OF_LINKS; i++) {
        //log = log+' '+to_string(myNode.nodeIndex);
        myNode = *myNode.getHitPointer();
    }
    //pthread_mutex_lock (&mut);
    //cout << "Thread "<< p->proc_num << " accessed to nodes: " << log << endl;
    //pthread_mutex_unlock (&mut);
    return (NULL);
}

int main(int argc, char* argv[]) {
    //mcsim_skip_instrs_begin();
    
    pthread_t *threads;
    pthread_attr_t pthread_custom_attr;
    thread_args *p;
    if (pthread_mutex_init(&mut, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    
    
    Graph * myGraph = new Graph();

    
    threads=(pthread_t *)malloc(NUM_OF_PROC*sizeof(*threads));
    pthread_attr_init(&pthread_custom_attr);
    
    p=(thread_args *)malloc(sizeof(thread_args)*NUM_OF_PROC);
    /* Start up thread */
    
    //mcsim_skip_instrs_end();
    
    for (int i=0; i<NUM_OF_PROC; i++)
    {
        p[i].proc_num=i;
        p[i].myGraph = myGraph;
        p[i].num_processors = NUM_OF_PROC;
        pthread_create(&threads[i], &pthread_custom_attr, gotoNext, (void *)(p+i));
    }
    
    /* Synchronize the completion of each thread. */
    
    for (int i=0; i<NUM_OF_PROC; i++)
    {
        pthread_join(threads[i],NULL);
    }
    
    free(p);
    pthread_mutex_destroy(&mut);
    
}

int32_t log_2(uint64_t number)
{
    int cumulative = 1;
    int32_t out = 0;
    int done = 0;
    
    while ((cumulative < number) && (!done) && (out < 50)) {
        if (cumulative == number) {
            done = 1;
        } else {
            cumulative = cumulative * 2;
            out ++;
        }
    }
    
    if (cumulative == number) {
        return(out);
    } else {
        return(-1);
    }
}

