
#include <pthread.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
struct args{
double first;
double n;
};
double value=1;
pthread_mutex_t lock;
void* pi_wallis_thread(void *input)
{   

    double size=(((struct args*)input)->n)-((struct args*)input)->first;
    size++;
    double first=((struct args*)input)->first;
    printf("Thread #%ld size=%2.0f first=%2.0f\n",pthread_self(),size,first);   
    
    double l=1;
    double m=1;
    double result=1;
    
    for (double i=first;i<=((struct args*)input)->n;i++)
   {
        l=(4*i*i);
        m=((2*i)-1)*((2*i)+1);
        result*=(l/m);
   }
    pthread_mutex_lock(&lock);
    value*=result;
    pthread_mutex_unlock(&lock);
    printf("Thread #%ld prod=%f\n",pthread_self(),result);
    
}
double pi_wallis(double n)
{
    double l=1;
    double m=1;
    double value1=1;
   for (double i=1;i<=n;i++)
   {
        l=(4*i*i);
        m=((2*i)-1)*((2*i)+1);
        value1 *=(l/m);
       
   }
   return value1*2;
}
int main(int argc, char **argv){
if(argc !=3)
{
    fprintf(stderr,"invalid number of arguments\n");
    return 1;
}
char *end;
char *end1;
double n_Wallis=strtol(argv[1],&end1,10);
int number_of_threads=strtol(argv[2],&end,10);
long long range=n_Wallis/number_of_threads;
clock_t start,end2;
double time;
if(*end !='\0'|| *end1 !='\0')
{
    fprintf(stderr,"argument is not a number\n");
    return 1;
}
pthread_t threads[number_of_threads];
struct args arguments[number_of_threads];
for (int i=0; i<number_of_threads;i++)
{   
    if(i==number_of_threads-1)
    {
        arguments[i].first=1+(i*range);
        arguments[i].n=n_Wallis;
    }
    else
    {
        arguments[i].first=1+(i*range);
        arguments[i].n=range+(i*range);
    }

}
start=clock();
for(int i=0; i<number_of_threads;i++)
{
    pthread_create(threads+i,NULL,pi_wallis_thread,(void*)&arguments[i]);
}
for (int i=0; i<number_of_threads;i++)
{
    long retval;
    pthread_join(threads[i],(void*)&retval);

}
end2=clock();
time=(double)(end2-start)/CLOCKS_PER_SEC;
printf("with threads PI=%f time=%2.8f\n",value*2,time);
clock_t start1, end3;
double time1;
start1=clock();
double value1=pi_wallis(n_Wallis);
end3=clock();
time1=(double)(end3-start1)/CLOCKS_PER_SEC;
printf("without threads PI=%f time=%2.8f\n",value1,time1);
return 0;
}
