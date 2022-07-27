#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/shm.h>

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <stdbool.h>

#include <unistd.h>

#include <wait.h>

int * sumPartOfAraay(char * arg);
char * cleanStringAndSaveVar(char * str1);
void splitByPlus(char * str3);
int powerOfx(int x, int pow);
bool thereIsX(char * str);
void makeForks(char * str);

int totalSum;
int numOfX;
int * shm_ptr;
int shm_id;
int numOfFork;
int shareMemIndex;

int main() {
    totalSum = 0;
    numOfFork = 0;
    shareMemIndex = 0;
    key_t key;
    if ((key = ftok("/", 'y')) == -1) {
        perror("ftok() failed");
        exit(EXIT_FAILURE);
    }

    shm_id = shmget(key, 6 * sizeof(int), IPC_CREAT | IPC_EXCL | 0600); //the max number for x in power 3
    if (shm_id == -1) {
        perror("failed to get memory.");
        exit(EXIT_FAILURE);
    }

    shm_ptr = (int * ) shmat(shm_id, NULL, 0);
    if (shm_ptr == NULL) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        totalSum = 0;
        numOfFork = 0;
        shareMemIndex = 0;
        char str[510];
        printf("PLEASE ENTER POLYNOM AND VAL\n");
        fgets(str, 510, stdin);
        if (strcmp(str, "done\n") == 0) {
            shmdt(shm_ptr);
            shmctl(shm_id, 0, NULL);
            return 0;
        }
        char * strOfPolynom = cleanStringAndSaveVar(str);
        splitByPlus(strOfPolynom);

        for (int z = 0; z < numOfFork; z++) { //wait for all sons
            wait(NULL);
        }
        for (int i = 0; i < numOfFork; i++) { //sum the result of each son
            totalSum += shm_ptr[i];
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
    token = strtok(str3, "+");
    while (token != NULL) {
        if (thereIsX(token) == false) {
            token = strtok(NULL, "+");
        } else {
            makeForks(token);
            numOfFork++;
            token = strtok(NULL, "+");
            shareMemIndex++;
        }
    }
}

void makeForks(char * str) {
    pid_t pid = fork();
    if (fork < 0) {
        perror("failed");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        sumPartOfAraay(str);
        exit(0);
    }
}

int * sumPartOfAraay(char * str) {
    int * sum = (int * ) malloc(sizeof(int)); //sum for forks return
    * sum = 0;
    int positionOfx;
    int positionOfPow;
    bool thereIsPow = false;
    char parameter[20];
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == 'x') { //there is x in str
            positionOfx = i; //the position of x ins str is index i
        } else if (str[i] == '^') {
            positionOfPow = i; //the position of ^ ins str is index i
            thereIsPow = true;
        }
    }
    int par;
    int p;
    int po;
    if (positionOfx == 0) { //for exmple x^2
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
    if (p == 0) { // for exmple x^0
        po = 1; //the sum of x in power of 0 is 1
    } else {
        po = powerOfx(numOfX, p); //calculate x in power of p
    }
    * sum = par * po; //calculate all the part of the big polynom
    shm_ptr[shareMemIndex] = * sum;
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