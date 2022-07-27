#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <pthread.h>

#include <stdbool.h>

void * sumPartOfAraay(void * arg);
char * cleanStringAndSaveVar(char * str1);
void splitByPlus(char * str3);
void makeThreads(char * str, int index);
int powerOfx(int x, int pow);
bool thereIsX(char * str);

int totalSum;
int numOfX;
int countThreads;
pthread_t threads[3];

int main() {
    while (1) {
        countThreads = 0;
        totalSum = 0;
        char str[510];
        printf("PLEASE ENTER POLYNOM AND VAL\n");
        fgets(str, 510, stdin);
        if (strcmp(str, "done\n") == 0) {
            return 0;
        }
        char * strOfPolynom = cleanStringAndSaveVar(str);

        splitByPlus(str);

        int * res;
        for (int z = 0; z < countThreads; z++) { //get the sum from the threads and make them free
            pthread_join(threads[z], (void ** ) & res);
            totalSum += * res; //sum the results from the threads
            free(res);
        }
        printf("the result is : %d\n", totalSum);
    }
}

char * cleanStringAndSaveVar(char * str1) {
    char tempStr[50];
    int i;
    int j;
    int k = 0;
    bool psik = false;
    for (i = 0; i < strlen(str1); i++) {
        if (str1[i] == ',') {
            psik = true;
            if (str1[i - 1] == ' ' || str1[i + 1] != ' ') {
                printf("this syntax is not allowed\n");
                exit(1);
            }
            for (j = i + 1; j < strlen(str1); j++) {
                tempStr[k] = str1[j]; //copy the part after the char ',' in str1
                k++;
            }
            numOfX = atoi(tempStr); //save the var that we put instead 'x'
            str1[i] = '\0';
            return str1; //return the polynom without the part of ", number"
        }
    }
    if (psik == false) {
        printf("this syntax is not allowed\n");
        exit(1);
    }
}

void splitByPlus(char * str3) {
    char * token;
    int i = 0;
    token = strtok(str3, "+");

    while (token != NULL) {
        if (thereIsX(token) == false) {
            token = strtok(NULL, "+"); //split the polynom by plus
        } else {
            makeThreads(token, i); //send the token(part of the polynom) to make thread for this token
            token = strtok(NULL, "+");
            i++;
        }
    }
}

void makeThreads(char * str, int index) { //creat the threads
    int status;
    status = pthread_create( & threads[index], NULL, sumPartOfAraay, (void * ) str);
    countThreads++; //number of threads
    if (status != 0) {
        fputs("pthread create fail in main", stderr);
        exit(1);
    }
}

void * sumPartOfAraay(void * arg) {
    int * sum = (int * ) malloc(sizeof(int)); //sum for threads return
    * sum = 0;
    int positionOfx;
    int positionOfPow;
    bool thereIsPow = false;
    char parameter[20];
    char * str = (char * ) arg; //casting for the input of the function from void* to char*
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == 'x') { //check if there is x in str
            positionOfx = i; //the position of x ins str is index i
        } else if (str[i] == '^') {
            positionOfPow = i; //the position of ^ ins str is index i
            thereIsPow = true;
        }
    }
    int par;
    int p;
    int po;
    if (positionOfx == 0) { //for example x^2
        par = 1;
    } else {
        for (int j = 0; j < positionOfx - 1; j++) {
            parameter[j] = str[j]; //copy the parameter of x
        }
        par = atoi(parameter); //make the parameter to convert to int
    }
    if (thereIsPow == false) {
        p = 1;
    } else {
        p = str[positionOfPow + 1] - '0'; //cast and save the power as int
    }
    if (p == 0) {
        po = 1;
    } else {
        po = powerOfx(numOfX, p); //calculate x in power of p
    }

    * sum = par * po; //calculate all the part of the big polynom
    return sum;
}

int powerOfx(int x, int pow) { //calculate x in power of number
    int xAfterPower = x;
    for (int i = 0; i < pow - 1; i++) {
        xAfterPower *= x;
    }
    return xAfterPower;
}

bool thereIsX(char * str) {
    for (int i = 0; i < strlen(str); i++) { //check if there is 'x' in str
        if (str[i] == 'x') {
            return true;
        }
    }
    totalSum += atoi(str); //add the free number in the polynom to the total sum
    return false;
}