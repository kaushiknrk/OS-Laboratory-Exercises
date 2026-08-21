#include<stdio.h>
#include<unistd.h>
int isprime(int n){
   if(n<=1)
      return 0;
   int i;
   for( i=2;i*i<=n;i++){
      if(n%i==0)
	 return 0;
   }
   return 1;
}
int main(){
    int n,p1,i,j;
    printf("Enter the no of element:");
    scanf("%d",&n);
    int arr[n];
    printf("Enter the array element:");
    for(i=0;i<n;i++)
       scanf("%d",&arr[i]);
    p1=fork();
    if(p1==0){
       printf("PRIME NUMBERS\n");
       for( j=0;j<n;j++){
	  if(isprime(arr[j]))
	     printf("%d\n",arr[j]);
       }
    }
    else{
       wait(NULL);
       int a=0;
       int b=1;
       for(i=0;i<n;i++){
	  printf("%d\t",a);
	  int next = a+b;
	  a=b;
	  b=next;
       }
       printf("\n");
    }
    return 0;
}

