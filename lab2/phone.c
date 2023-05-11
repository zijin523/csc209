#include <stdio.h>

int main(){
  char phone[11];
  int n;
  scanf("%s %d",phone,&n);

  if(n < -1 || n > 9){
    printf("ERROR\n" );
    return 1;
  }

  if(n == -1){
    printf("%s\n", phone);
  } else if(0<=n && n<=9){
    printf("%c\n", phone[n]);
  }
  return 0;
}