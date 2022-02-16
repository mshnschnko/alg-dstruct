#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int key;
    unsigned char height;
    struct node* left;
    struct node* right;
} node;

unsigned char height(node* p) {
    return p ? p->height : 0;
}

int bfactor(node* p) {
    return height(p->right)-height(p->left);
}

void fixheight(node* p) {
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

node* rotateright(node* p) {
    node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

node* rotateleft(node* q) {
    node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

node* balance(node* p) {
    fixheight(p);
    if(bfactor(p) == 2)
    {
        if(bfactor(p->right) < 0)
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if(bfactor(p) == -2)
    {
        if(bfactor(p->left) > 0)
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p;
}

node* insert(node* p, int k) {
    if(!p) {
        p = (node*)malloc(sizeof (node));
        if(!p)
            return NULL;
        else {
            p->key = k;
            p->left = NULL;
            p->right = NULL;
            p->height = 1;
            return p;
        }
    }
    if(k < p->key)
        p->left = insert(p->left, k);
    else
        p->right = insert(p->right, k);
    return balance(p);
}

node* findmin(node* p) {
    return p->left ? findmin(p->left) : p;
}

node* removemin(node* p) {
    if(p->left == 0)
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

node* delete(node* p, int k) {
    if(!p)
        return 0;
    if(k < p->key)
        p->left = delete(p->left,k);
    else if(k > p->key)
        p->right = delete(p->right,k);
    else
    {
        node* q = p->left;
        node* r = p->right;
        free(p);
        if(!r)
            return q;
        node* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}

void search(node* p, int k) {
    if(!p) {
        fprintf(stdout, "no\n");
        return;
    }
    if(k == p->key) {
        fprintf(stdout, "yes\n");
        return;
    }
    if(k < p->key)
        search(p->left, k);
    else
        search(p->right, k);
}

void treedestroy(node* p) {
    if(p) {
        treedestroy(p->left);
        treedestroy(p->right);
        free(p);
    }
}

int lab() {
    node* p = NULL;
    char command[16] = " ", ch;
    int key;
    while(fgets(command, 16, stdin)) {
        sscanf(command, "%c%i", &ch, &key);
        switch (ch) {
            case 'a':
                p = insert(p, key);
                break;
            case 'r':
                p = delete(p, key);
                break;
            case 'f':
                search(p, key);
                break;
            default:
                treedestroy(p);
                return 0;
        }
    }
    treedestroy(p);
    return 0;
}

int main(void) {
    lab();
    return 0;
}