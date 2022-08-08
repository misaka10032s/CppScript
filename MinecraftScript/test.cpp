#include<stdio.h>
#include<stdlib.h>

void bubble_sort(int list[5]){
    int i=0, j=0, tmp;
    while(j < 4){
        if(list[i] > list[i+1]){
            tmp = list[i];
            list[i] = list[i+1];
            list[i+1] = tmp;
        }
        else{
            i++;
            if(i >= 4){
                j++;
                i = 0;
            }
        }
    }
}

int main()
{
    int a, b, XD[5][5];
    printf("DATE: ");
    scanf("%d",&a);
    srand(a);
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            XD[i][j]=rand()%101;
            printf("%2d ",XD[i][j]);
        }
        printf("\n");
    }
    printf("mode: ");
    scanf("%d",&b);

    for(int i=0; i<5; i++) bubble_sort(XD[i]);

    for(int i=0; i<5; i++) {
        for(int j=0; j<5; j++){
            printf("%d ", XD[i][j]);
        }
        printf("\n");
    }
    system("pause");
    return 0;
}