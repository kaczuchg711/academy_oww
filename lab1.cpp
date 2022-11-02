//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//
//void rand_integer(int lower,int upper)
//{
//    FILE *filePtr = fopen("C:\\zzz\\uczelnia\\OWW\\lab1\\randint.txt","w");
//    if(filePtr == NULL)
//    {
//        printf("Error!");
//        exit(1);
//    }
//    int num;
//    int i;
//    for(i=0;i<20;++i)
//    {
//        num = rand()%(upper-lower) + lower;
//        printf("%d\n",num);
//        fprintf(filePtr,"%d\n",num);
//    }
//    fclose(filePtr);
//}
//
//
//void rand_double(double lower, double upper)
//{
//    FILE *filePtr = fopen("C:\\zzz\\uczelnia\\OWW\\lab1\\randdouble.txt","w");
//    if(filePtr == NULL)
//    {
//        printf("Error!");
//        exit(1);
//    }
//    int i;
//    double div;
//    double num;
//    for(i=0;i<20;++i) {
//        div = RAND_MAX / (upper-lower);
//        num = (rand() / div)+lower;
//        printf("%lf\n",num);
//        fprintf(filePtr,"%lf\n",num);
//    }
//
//    fclose(filePtr);
//}
//
//int main() {
//    srand(time(0));
//    rand_integer(-10,20);
//    rand_double(-10,20);
//    return 0;
//}
