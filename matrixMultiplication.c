#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>

struct Matrix{
int row;
int column;
};

int row1,row2,column1,column2;
int i,j,position;
int **A;
int **B;
int **result1;
int **result2;
clock_t start,end;
double elapsedT1,elapsedT2;

//function that reads 2 matrices from the input file that will be multiplied then allocates memory for the arrays in which the matrices will be stored and for the array of results
void readFile()
{
  FILE* f;
  f=fopen("100.txt","r");
  if(f!= NULL)
{ while(!feof(f)){ 
 fscanf(f,"%d %d",&row1,&column1);
fscanf(f,"\n");


 A=(int**)malloc(row1*sizeof(int*));
 for(i=0;i<row1;i++)
{
  A[i]=(int*)malloc(column1*sizeof(int));
}
  for(i=0;i<row1;i++){
   for(j=0;j<column1;j++)
    fscanf(f,"%d ",&A[i][j]);
    fscanf(f,"\n"); 
}

 fscanf(f,"%d %d",&row2,&column2);
 fscanf(f,"\n");
 B=(int**)malloc(row2*sizeof(int*));
 for(i=0;i<row2;i++)
{
  B[i]=(int*)malloc(column2*sizeof(int));
}
 
  for(i=0;i<row2;i++){
   for(j=0;j<column2;j++)
    fscanf(f,"%d ",&B[i][j]);
    fscanf(f,"\n"); 
}}

//to store memory for the results 
result1=(int**)malloc(row1*sizeof(int*));
 for(i=0;i<row1;i++)
 { result1[i]=(int*)malloc(column2*sizeof(int));
 }
result2=(int**)malloc(row1*sizeof(int*));
 for(i=0;i<row1;i++)
  {result2[i]=(int*)malloc(column2*sizeof(int));
  } 
 fclose(f);
}}



//function that writes the matrix result and the time elapsed to the output file
void writetoFile(int **array1,int **array2)
{
 FILE* f;
 f=fopen("output.txt","w");
if(f!=NULL)
{
 fprintf(f,"Result of computing output of each element on a thread:\n");
 for(i=0;i<row1;i++){
  for(j=0;j<column2;j++)
   fprintf(f,"%d\t",array1[i][j]); 
   fprintf(f,"\n"); }
  fprintf(f,"Elapsed time= %lf\n\n",elapsedT1); 

  fprintf(f,"Result of computing output of each row on a thread:\n");
 for(i=0;i<row1;i++){
  for(j=0;j<column2;j++)
   fprintf(f,"%d\t",array2[i][j]); 
   fprintf(f,"\n"); }
  fprintf(f,"Elapsed time= %lf\n\n\n",elapsedT2); 
}}


//function that multiplies the 2 input matrices and puts the result back in the output matrix
//each element in output matrix is calculated on a thread
void *case1(void *a)
{    
     struct Matrix *x=a;
     for(position=0;position<row2;position++)
      result1[x->row][x->column]+=A[x->row][position]*B[position][x->column];
}

//function that multiplies the 2 input matrices and puts the result back in the output matrix
//each row in output matrix is calculated on a thread
void *case2(void *a)
{
     struct Matrix *x=a;
     for(j=0;j<column1;j++)
      for(position=0;position<row2;position++)
       result2[x->row][j]+=A[x->row][position]*B[position][j];
}


//this function creates a thread and calls case1 function and sends the needed parameters to it and calculates the time taken for multiplication done by this method
void elementbyelement()
{
printf("Row:%d\n",row1);
printf("Column:%d\n",column2);

//pthread_t threads1[row1][column2];

pthread_t **threads1=(pthread_t **)malloc(row1* sizeof(pthread_t *));
for(i=0;i<row1;i++)
{threads1[i]=(pthread_t *)malloc(column2* sizeof(pthread_t));  }


start=clock();
for(i=0;i<row1;i++)
  for(j=0;j<column2;j++)
   {struct Matrix *result3=malloc(sizeof(struct Matrix));
    result3->row=i;
    result3->column=j;
    pthread_create(&threads1[i][j],NULL,case1,(void*)result3);

   }

for(i=0;i<row1;i++){
  for(j=0;j<column2;j++){
  pthread_join(threads1[i][j],NULL);
}}

end=clock()-start;
elapsedT1=((double)end)/CLOCKS_PER_SEC;

printf("First method\n");
printf("%lf\n",elapsedT1);
 /* for(i=0;i<row1;i++)
    { for(j=0;j<column2;j++)
       printf("%d ",result1[i][j]);
       printf("\n");
     }*/
}

//this function creates a thread and calls case2 function and sends the needed parameters to it and calculates the time taken for multiplication done by this method
void rowbyrow()
{
//pthread_t threads2[row1];
pthread_t *threads2=(pthread_t *)malloc(row1 *sizeof(pthread_t));

start=clock();

for(i=0;i<row1;i++)
{ struct Matrix *result4=malloc(sizeof(struct Matrix));
  result4->row=i;
  pthread_create(&threads2[i],NULL,case2,(void*)result4);
//  printf("index:%d\n",i);
}

 for(i=0;i<row1;i++)
{pthread_join(threads2[i],NULL);
}

end=clock()-start;
elapsedT2=((double)end)/CLOCKS_PER_SEC;


printf("Second method\n");
printf("%lf\n",elapsedT2);

 /* for(i=0;i<row1;i++){
     for(j=0;j<column2;j++)
     
       printf("%d ",result2[i][j]);
       printf("\n");
     } */   
}

int main(){

readFile();

//makes sure that row2 and column1 are equal for the matrix multiplication to be calculated

if(column1==row2)
{
elementbyelement(); 
rowbyrow();
writetoFile(result1,result2);
}

else printf("Multiplication cannot be done.");

return 0;
}
