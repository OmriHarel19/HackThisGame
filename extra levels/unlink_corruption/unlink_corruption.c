#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_t
{
        struct Node_t* fd;
        struct Node_t* bk;
        char buf[8];
}Node;

void shell()
{
        system("/bin/sh");
}

void myUnlink(Node* P)
// the vulnerable unlink func:
// gets a Node and unlinks it from the nodeList
//given the node B in the list: A <-> B <-> C the unlink of Node b will resault in: A <-> C 
{
        Node* BK;
        Node* FD;

        // current node backward pointer (to Node A)
        BK=P->bk;
        // current node forward pointer (to Node C)
        FD=P->fd;

        //unlink:
        // connecting the forward Node (C) to the backward Node (A)
        FD->bk=BK;
        // connecting the backward Node (A) to the forward Node (C)
        BK->fd=FD;
}
int main(int argc, char* argv[])
{
        printf("creating three nodes!\n");

        malloc(1024);
        Node* A = (Node*)malloc(sizeof(Node));
        Node* B = (Node*)malloc(sizeof(Node));
        Node* C = (Node*)malloc(sizeof(Node));

        // creating a double linked list: A <-> B <-> C
        A->fd = B;
        B->bk = A;
        B->fd = C;
        C->bk = B;

        //leaking stack adr of pointer A on the stack
        printf("here is stack address leak: %p\n", &A);
        //leaking the content of A - the adr of its allocated chunk on the heap
        printf("here is heap address leak: %p\n", A);

        printf("now that you have leaks, get shell!\n");

        // heap overflow here!
        gets(A->buf);

        // exploit this unlink!
        myUnlink(B);
        return 0;
}