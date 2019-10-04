#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

typedef struct LinkedListNode {
    int x;
    int y;
    int hasNext;
    struct LinkedListNode* next;
}Node;
Node* addNode(int x, int y, Node* head);
void printList(Node* head);
int takeStep(int numSteps, size_t width, size_t height,  char grid[width][height], Node* head);
void printGrid(size_t width, size_t height,char grid[width][height]);
int runSimulation(double probability, size_t width, size_t height);
double getSimulationAvg(int numSimulations, double probability, size_t width, size_t height);
void printData(size_t numProbs, double averages[numProbs], double probs[numProbs]);
double gettime();
int main(int argc, char* argv[]){
    double tic = gettime();

    size_t width = 30;
    size_t height = 40;
    int numTrials = 1000;
    double probSpacing = 0.01;
    size_t numProbs = (size_t)(3.0/probSpacing);

    double steps[numProbs];
    double probs[numProbs];
    for(int x=0;x<numProbs;x++){
        steps[x] = 0.0;
        probs[x] = 0.0;
    }
    srand(10000);
    int index = 0;

    
    for(double p = 0.0;p<=1.0000;p=p+probSpacing/(1+2*steps[index-1])){
        steps[index] = getSimulationAvg((int)(numTrials*(1+2*steps[index-1])),p,width,height)/width;
        probs[index] = p;
        printf("%f:%f\n",p,steps[index]);
        index++;
    }

    double toc = gettime();
    double runtime = toc - tic;
    printf("%lf\n",runtime);

    printData(numProbs, steps, probs);
    
}
double getSimulationAvg(int numSimulations, double probability, size_t width, size_t height){
    double average = 0;
    for(int x=0;x<numSimulations;x++){
        int steps = runSimulation(probability, width, height);
        average = (((double)x)*average + ((double)steps))/((double)(x+1));
        //printf(",%d",steps);
    }
    return average;
}
int runSimulation(double probability, size_t width, size_t height){
    char grid[width][height];
    Node* head = (Node*)malloc(sizeof(Node));
    head -> x = -1;
    head -> y = -1;
    head -> hasNext = 0;
    for(int x=0;x<(int)width;x++){
        for(int y=0;y<(int)height;y++){
            if(x==0){
                grid[x][y] = 'X';
                head = addNode(x,y,head);
            }
            else{
                if(rand()%10000<probability*10000)
                    grid[x][y] = 'T';
                else
                    grid[x][y] = '.';
            }
        }
    }
    int numSteps = takeStep(0,width,height,grid,head);
    //printf("%d\n",numSteps);
    return numSteps;
}
int takeStep(int numSteps, size_t width, size_t height, char grid[width][height], Node* head){
    //usleep(100000);
    //system("clear");
    //printList(head);
    //printGrid(width,height,grid);
    
    
    Node* curr = head;
    while(curr -> hasNext == 1){
        int x = curr -> next -> x;
        int y = curr -> next -> y;
        int nodeAdded = 0;
        //printf("Started handling (%d, %d)\n",x,y);
        grid[x][y] = '.';
        if(curr -> next -> hasNext)
            curr -> next = curr -> next -> next;
        else
            curr -> hasNext = 0;
        //printf("Deleted (%d, %d)\n",x,y);
        if(x+1<width){
            if(grid[x+1][y] == 84){
                grid[x+1][y] = 'X';
                head = addNode(x+1,y,head);
                nodeAdded = 1;
            }
        }
        if(x>0){
            if(grid[x-1][y] == 84){
                grid[x-1][y] = 'X';
                head = addNode(x-1,y,head);
                nodeAdded = 1;
            }
        }
        if(y+1<height){
            if(grid[x][y+1] == 84){
                grid[x][y+1] = 'X';
                head = addNode(x,y+1,head);
                nodeAdded = 1;
            }
        }
        if(y>0){
            if(grid[x][y-1] == 84){
                grid[x][y-1] = 'X';
                head = addNode(x,y-1,head);
                nodeAdded = 1;
            }
        }
        //printf("Filled nodes surrounding (%d, %d)\n",x,y);
        if(curr == head && nodeAdded == 1)
            curr = curr -> next;
        //printf("Finished handling (%d, %d)\n",x,y);
        //printf("List is now: ");
        //printList(head);
        //printf("\n");
    }
    if(head -> hasNext == 1)
        return takeStep(numSteps+1,width,height,grid, head);
    else
        return numSteps;
}
void printGrid(size_t width, size_t height, char grid[width][height]){
    printf("\n");
    for(int y=0;y<(int)height;y++){
        for(int x=0;x<(int)width;x++){
            printf("%c",grid[x][y]);
        }
        printf("\n");
    }
}
void printData(size_t numProbs, double averages[numProbs], double probs[numProbs]){
    FILE* out = fopen("out_1sim.txt", "w");
    int x=1;
    fprintf(out,"%lf %lf\n",probs[0],averages[0]);
    while(probs[x]>0.0 && x<numProbs){
        fprintf(out,"%lf %lf\n",probs[x],averages[x]);
        x = x + 1;
    }
}
double gettime(){
    double t;
    struct timeval* ptr = (struct timeval*)malloc(sizeof(struct timeval));
    gettimeofday(ptr, NULL); // second argument is time zone... NULL
    t = ptr->tv_sec * 1000000.0 + ptr->tv_usec;
    free(ptr);
    return t/1000000.0;
}
Node* addNode(int x0, int y0, Node* head){
    Node* nextNode = (Node*)malloc(sizeof(Node));
    nextNode -> x = x0;
    nextNode -> y = y0;
    if(head -> hasNext == 1){
        nextNode -> next = head -> next;
        nextNode -> hasNext = 1;
    }
    else
        nextNode -> hasNext = 0;
    head -> next = nextNode;
    head -> hasNext = 1;
    return head;
}
void printList(Node* head){
    Node* curr = head;
    while(curr -> hasNext == 1){
        printf("(%d, %d, %d)",curr -> next -> x,curr -> next -> y, curr -> next -> hasNext);
        if(curr -> next -> hasNext == 1)
            printf(", ");
        curr = curr -> next;
    }
}