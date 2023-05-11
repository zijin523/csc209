#include <stdio.h>

int main(){
  char phone[11];
  int n;
  int flag = 0;
  scanf("%s",phone);

  while(scanf("%d",&n) != -1){
    if(n < -1 || n > 9){
        printf("ERROR\n" );
        flag = 1;
    }
    else if(n == -1){
        printf("%s\n", phone);
    }
    else if(0<=n && n<=9){
        printf("%c\n", phone[n]);
    }
  }
  return flag;
}