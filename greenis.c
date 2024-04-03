#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PORT 7379

typedef struct node Node;
char *temp="-"; //I use this to set the value that the server sends when a GET request goes well

struct node{
    char *key;
    char *value;
    time_t time_creation;
    int ttl;
    Node *next;
};

void exit_with_error(const char * msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

Node * createNode() {
    Node *n=malloc(sizeof(Node));
    n->key=NULL;
    n->value=NULL;
    n->next=NULL;
    return n;
}

void printdat(Node* n){
    while(n!=NULL) {
        printf("key: %s\n", n->key);
        printf("val: %s\n", n->value);
        n=n->next;
    }
}

int get_value(char* key, Node *n) {
    Node *t=n;
    while (t!=NULL) {
        if (t->key!=NULL){
            if (strcmp(t->key, key)==0){
                if (t->ttl==-1){ 
                    temp=t->value;
                    return 2;
                }
                else {
                    if (time(NULL)-t->time_creation<=t->ttl){ //< or <=? 
                        temp=t->value;
                        return 2;
                    }
                    else
                        return 0;
                }
            }
        }
        t=t->next;
    }
    return 0;
}

void set_value(char *key, char *value, int ttl, Node *n) {
    Node *t=n;
    if (t->next==NULL){
        Node *new=createNode();
        new->key=malloc(sizeof(key));
        strcpy(new->key, key);
        new->value=malloc(sizeof(value));
        strcpy(new->value, value);
        new->next=NULL;
        new->time_creation=time(NULL);
        new->ttl=ttl;
        t->next=new;
    }    
    else  { 
        set_value(key, value, ttl, t->next);
    }
}

int interprete(char *input, Node *n) { //works on the string sent by the client and returns 1, 0, 2 depending on the result of getter and setter
    if (strstr(input, "CLIENT"))    //CONNECTION request
        return 1;
    else if (strstr(input, "SET")){  //SET request
    char *t=input;
        char *tok=strtok(t, "$");
        int i=1; 
        char *k;
        char *v;
        char *ttl="\n-1";
        while (tok) {
            if (i==3)
                k=tok;
            else if (i==4)
                v=tok;
            else if (i==6)
                ttl=tok;
            i++;
            tok=strtok(NULL, "$");
        }
        //k+=3;
        while (isalpha(*k)==0) {
            k++;
        }
        int lenk=strlen(k)-1; 
        char *key=malloc(lenk);
        memset(key, 0, sizeof(key));
        for (int i=0; i<lenk-1; i++) {
            key[i]=*k;
            k++;
        }
        key[lenk]='\0';
        //v+=3;
        while (isalpha(*v)==0) {
            v++;
        }
        int lenv=strlen(v)-1;
        char *value=malloc(lenv);
        memset(value, 0, sizeof(value));
        for (int i=0; i<lenv-1; i++) {
            value[i]=*v;
            v++;
        }
        value[lenv]='\0';
        //ttl+=3; //è il ttl se c'è senno è -1
        while (*ttl!='\n') { //to handle cases like $10 or $100
            ttl++;
        }
        ttl++;
        int tt=atoi(ttl);
        set_value(key, value, tt, n);
        if (key!=NULL)
            free(key);
        if (value!=NULL)
            free(value);
        return 1;
    }
    else if (strstr(input, "GET")){  //GET request
        char *key=strrchr(input, '$');
        //key+=4;
        while (isalpha(*key)==0){ //to handle cases like $10 or $100
            key++;
        }
        int len=strlen(key)-1;
        char *k=malloc(len);
        memset(k, 0, sizeof(k));
        
        for (int i=0; i<len-1; i++) {
            k[i]=*key;
            key++;
        }
        k[len]='\0';
        int ret=get_value(k, n);
        if (k!=NULL)
            free(k);
        return ret;
    }
    else {
        return 0;
    }
}

void server_tcp(Node *n, int sem) {
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};
    char* ok=malloc(sizeof(5));
    ok[0]='+';
    ok[1]='O';
    ok[2]='K';
    ok[3]='\r';
    ok[4]='\n';
    char *notok=malloc(sizeof(5));
    notok[0]='$';
    notok[1]='-';
    notok[2]='1';
    notok[3]='\r';
    notok[4]='\n';

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        exit_with_error("socket failed");
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        exit_with_error("setsockopt");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        exit_with_error("bind failed");
    }
    
    if (listen(server_fd, 3) < 0){
        exit_with_error("listen");
    }
    while (1){
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0){
            exit_with_error("accept");
        }
        struct sembuf sem_op={0, 1, 0};
        int s=semop(sem, &sem_op, 1);
        if (s==-1)
            exit_with_error("semop error");
        pid_t p=fork();
        if (p==-1){
            exit_with_error("fork error");
        }
        else if(p==0){
            struct sembuf sem_op={0, -1, 0};
            s=semop(sem, &sem_op, 1);
            if (s==-1)
                exit_with_error("semop error");
            while(1){
                valread = read(new_socket, buffer, 1024 - 1); 
                printf("comando ricevuto: %s\n", buffer);
                int res=interprete(buffer, n);
                if (res==1) {
                    send(new_socket, ok, strlen(ok), 0);
                    printf("Reply message sent: %s\n", ok);
                }
                else if (res==2) {
                    int len=strlen(temp);
                    char *r=malloc(6+len);
                    memset(r, 0, sizeof(r));
                    r[0]='$';
                    int x=0;
                    if (len>=10){ //max lenght is 99 characters
                        x++;
                        char str[3];
                        sprintf(str, "%d", len);
                        //printf("%s\n", str);
                        r[1]=str[0];
                        r[2]=str[1];
                    }
                    else {                
                        r[1]=len+48;
                    }
                    r[2+x]='\r';
                    r[3+x]='\n';
                    //printf("@@@@@%s\n",temp);
                    for (int i=0; i<len; i++) {
                        r[i+4+x]=temp[i];
                    }
                    r[len+4+x]='\r';
                    r[len+5+x]='\n';
                    send(new_socket, r, strlen(r), 0);
                    printf("Reply message sent: %s\n", r);
                    if (r!=NULL)
                        free(r);
                        temp="-"; //maybe useless
                }
                else{
                    send(new_socket, notok, strlen(notok), 0);
                    printf("Reply message sent: %s\n", notok);
                }
                memset(buffer, 0, sizeof(buffer)); 
                //printdat(n);
            }
            close(new_socket);
            close(server_fd);
            sem_op.sem_op=1;
            s=semop(sem, &sem_op, 1);
            if (s==-1)
                exit_with_error("semop error");
        }
    }
    if (ok!=NULL)
        free(ok);
    if (notok!=NULL)
        free(notok);
}

void freeNode(Node *n) {
    if (n->key!=NULL)
        free(n->key);
    if (n->value!=NULL)
        free(n->value);
    freeNode(n->next);
    if ((n!=NULL))
        free(n);
}

int main(int argc, const char * argv[]) {
    //semaphore initialization
    int sem=semget(IPC_PRIVATE, 1, 0600);
    if (sem==-1)   
        exit_with_error("semget error");
    union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    };
    union semun sem_val={.val=0};
    int x=semctl(sem, 0, SETVAL, sem_val);
    if (x==-1)
        exit_with_error("semctl error");
    Node *n=createNode();
    server_tcp(n, sem);
    freeNode(n);
    return 0;
}