#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False !(True)

typedef struct node {
    int size;
    int key[3];
    struct node* first;
    struct node* second;
    struct node* third;
    struct node* fourth;
    struct node* parent;
}node;

char find(int value, int* key, int size);
void swap(int* x, int* y);
void sort2(int* x, int* y);
void sort3(int* x, int* y, int* z);
void sortkeys(int size, int* key);
node* createnode(int value);
node* createnode2(int value, node* first, node* second, node* third, node* fourth, node* parent);
void insertkey(int value, node* treenode);
void removekey(int value, node* treenode);
void becomenode2(int value, int* key, node* treenode, node* first, node* second);
char isleaf(node* tree);
node* split(node* item);
node* insertnode(node* tree, int value);
node* search(node* tree, int value);
node* searchmin(node* tree);
node* merge(node* leaf);
node* redistribute(node* leaf);
node* fix(node* leaf);
node* removenode(node* tree, int value);
void treedestroy(node* t);
void printtree(node* tree, int n);

node* createnode(int value) {
    node* newnode = (node*)malloc(sizeof(node));
    if (!newnode)
        return NULL;
    newnode->first = NULL;
    newnode->second = NULL;
    newnode->third = NULL;
    newnode->fourth = NULL;
    newnode->parent = NULL;
    newnode->key[0] = value;
    newnode->size = 1;
    return newnode;
}

node* createnode2(int value, node* first, node* second, node* third, node* fourth, node* parent) {
    node* newnode = (node*)malloc(sizeof(node));
    if (!newnode)
        return NULL;
    newnode->first = first;
    newnode->second = second;
    newnode->third = third;
    newnode->fourth = fourth;
    newnode->parent = parent;
    newnode->key[0] = value;
    newnode->size = 1;
    return newnode;
}

char find(int value, int* key, int size) {
    for (int i = 0; i < size; ++i)
        if (key[i] == value)
            return True;
    return False;
}

void swap(int* x, int* y) {
    int r = (*x);
    (*x) = (*y);
    (*y) = r;
}

void sort2(int* x, int* y) {
    if (*x > *y)
        swap(x, y);
}

void sort3(int* x, int* y, int* z) {
    if (*x > *y)
        swap(x, y);
    if (*x > *z)
        swap(x, z);
    if (*y > *z)
        swap(y, z);
}

void sortkeys(int size, int* key) {
    if (size == 1)
        return;
    if (size == 2)
        sort2(&key[0], &key[1]);
    if (size == 3)
        sort3(&key[0], &key[1], &key[2]);
}

void insertkey(int value, node* treenode) {
    if (!treenode)
        return;
    treenode->key[treenode->size] = value;
    treenode->size++;
    sortkeys(treenode->size, treenode->key);
}

void removekey(int value, node* treenode) {
    if (!treenode)
        return;
    if (treenode->size >= 1 && treenode->key[0] == value) {
        treenode->key[0] = treenode->key[1];
        treenode->key[1] = treenode->key[2];
        treenode->size--;
    }
    else if (treenode->size == 2 && treenode->key[1] == value) {
        treenode->key[1] = treenode->key[2];
        treenode->size--;
    }
}

void becomenode2(int value, int* key, node* treenode, node* first, node* second) {
    key[0] = value;
    treenode->first = first;
    treenode->second = second;
    treenode->third = NULL;
    treenode->fourth = NULL;
    treenode->parent = NULL;
    treenode->size = 1;
}

char isleaf(node* tree) {
    if (!tree)
        return False;
    if ((!tree->first) && (!tree->second) && (!tree->third))
        return True;
    return False;
}

node* insertnode(node* tree, int value) {
    if (!tree)
        return createnode(value);
    if (search(tree, value))
        return split(tree);
    if (isleaf(tree))
        insertkey(value, tree);
    else if (value <= tree->key[0])
        insertnode(tree->first, value);
    else if ((tree->size == 1) || ((tree->size == 2) && value <= tree->key[1]))
        insertnode(tree->second, value);
    else
        insertnode(tree->third, value);
    return split(tree);
}

node* split(node* item) {
    if (!item)
        return NULL;
    if (item->size < 3)
        return item;
    node* x = createnode2(item->key[0], item->first, item->second, NULL, NULL, item->parent);
    if (!x)
        return NULL;
    node* y = createnode2(item->key[2], item->third, item->fourth, NULL, NULL, item->parent);
    if (!y) {
        free(x);
        return NULL;
    }
    if (x->first)
        x->first->parent = x;
    if (x->second)
        x->second->parent = x;
    if (y->first)
        y->first->parent = y;
    if (y->second)
        y->second->parent = y;
    if (item->parent) {
        insertkey(item->key[1], item->parent);
        if (item->parent->first == item)
            item->parent->first = NULL;
        else if (item->parent->second == item)
            item->parent->second = NULL;
        else if (item->parent->third == item)
            item->parent->third = NULL;
        if (!item->parent->first) {
            item->parent->fourth = item->parent->third;
            item->parent->third = item->parent->second;
            item->parent->second = y;
            item->parent->first = x;
        }
        else if (!item->parent->second) {
            item->parent->fourth = item->parent->third;
            item->parent->third = y;
            item->parent->second = x;
        }
        else {
            item->parent->fourth = y;
            item->parent->third = x;
        }
        node* tmp = item->parent;
        free(item);
        return tmp;
    }
    else {
        x->parent = item;
        y->parent = item;
        becomenode2(item->key[1], item->key, item, x, y);
        return item;
    }
}

node* search(node* tree, int value) {
    if (!tree)
        return NULL;
    if (find(value, tree->key, tree->size))
        return tree;
    else if (value < tree->key[0])
        return search(tree->first, value);
    else if ((tree->size == 2) && (value < tree->key[1]) || (tree->size == 1))
        return search(tree->second, value);
    else if (tree->size == 2)
        return search(tree->third, value);
}

node* searchmin(node* tree) {
    if (!tree)
        return tree;
    if (!tree->first)
        return tree;
    else
        return searchmin(tree->first);
}

node* removenode(node* tree, int value) {
    if (!tree)
        return NULL;
    node* item = search(tree, value);
    if (!item)
        return tree;
    node* min = NULL;
    if (item->key[0] == value)
        min = searchmin(item->second);
    else
        min = searchmin(item->third);
    if (min) {
        int* z = (value == item->key[0] ? &(item->key[0]) : &(item->key[1]));
        swap(z, &min->key[0]);
        item = min;
    }
    removekey(value, item);
    return fix(item);
}

node* fix(node* leaf) {
    if (!leaf)
        return NULL;
    if (leaf->size == 0 && !leaf->parent) {
        free(leaf);
        return NULL;
    }
    if (leaf->size != 0) {
        if (leaf->parent)
            return fix(leaf->parent);
        else
            return leaf;
    }
    node* parent = leaf->parent;
    if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2)
        leaf = redistribute(leaf);
    else if (parent->size == 2 && parent->third->size == 2)
        leaf = redistribute(leaf);
    else
        leaf = merge(leaf);
    return fix(leaf);
}

node* merge(node* leaf) {
    if (!leaf)
        return NULL;
    node* parent = leaf->parent;
    if (parent->first == leaf) {
        insertkey(parent->key[0], parent->second);
        parent->second->third = parent->second->second;
        parent->second->second = parent->second->first;
        if (leaf->first)
            parent->second->first = leaf->first;
        else if
            (leaf->second) parent->second->first = leaf->second;
        if (parent->second->first)
            parent->second->first->parent = parent->second;
        removekey(parent->key[0], parent);
        free(parent->first);
        parent->first = NULL;
    }
    else if (parent->second == leaf) {
        insertkey(parent->key[0], parent->first);
        if (leaf->first)
            parent->first->third = leaf->first;
        else if (leaf->second)
            parent->first->third = leaf->second;
        if (parent->first->third)
            parent->first->third->parent = parent->first;
        removekey(parent->key[0], parent);
        free(parent->second);
        parent->second = NULL;
    }
    if (!parent->parent) {
        node* tmp = NULL;
        if (parent->first)
            tmp = parent->first;
        else
            tmp = parent->second;
        tmp->parent = NULL;
        free(parent);
        return tmp;
    }
    return parent;
}

node* redistribute(node* leaf) {
    if (!leaf)
        return NULL;
    node* parent = leaf->parent;
    node* first = parent->first;
    node* second = parent->second;
    node* third = parent->third;
    if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2)) {
        if (first == leaf) {
            parent->first = parent->second;
            parent->second = parent->third;
            parent->third = NULL;
            insertkey(parent->key[0], parent->first);
            parent->first->third = parent->first->second;
            parent->first->second = parent->first->first;
            if (leaf->first)
                parent->first->first = leaf->first;
            else if
                (leaf->second) parent->first->first = leaf->second;
            if (parent->first->first)
                parent->first->first->parent = parent->first;
            removekey(parent->key[0], parent);
        }
        else if (second == leaf) {
            insertkey(parent->key[0], first);
            removekey(parent->key[0], parent);
            if (leaf->first)
                first->third = leaf->first;
            else if (leaf->second)
                first->third = leaf->second;
            if (first->third)
                first->third->parent = first;
            parent->second = parent->third;
            parent->third = NULL;
            free(second);
        }
        else if (third == leaf) {
            insertkey(parent->key[1], second);
            parent->third = NULL;
            removekey(parent->key[1], parent);
            if (leaf->first)
                second->third = leaf->first;
            else if (leaf->second)
                second->third = leaf->second;
            if (second->third)
                second->third->parent = second;
            free(third);
        }
    }
    else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2))) {
        if (third == leaf) {
            if (leaf->first) {
                leaf->second = leaf->first;
                leaf->first = NULL;
            }
            insertkey(parent->key[1], leaf);
            if (second->size == 2) {
                parent->key[1] = second->key[1];
                removekey(second->key[1], second);
                leaf->first = second->third;
                second->third = NULL;
                if (leaf->first)
                    leaf->first->parent = leaf;
            }
            else if (first->size == 2) {
                parent->key[1] = second->key[0];
                leaf->first = second->second;
                second->second = second->first;
                if (leaf->first)
                    leaf->first->parent = leaf;
                second->key[0] = parent->key[0];
                parent->key[0] = first->key[1];
                removekey(first->key[1], first);
                second->first = first->third;
                if (second->first)
                    second->first->parent = second;
                first->third = NULL;
            }
        }
        else if (second == leaf) {
            if (third->size == 2) {
                if (!leaf->first) {
                    leaf->first = leaf->second;
                    leaf->second = NULL;
                }
                insertkey(parent->key[1], second);
                parent->key[1] = third->key[0];
                removekey(third->key[0], third);
                second->second = third->first;
                if (second->second)
                    second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = NULL;
            }
            else if (first->size == 2) {
                if (!leaf->second) {
                    leaf->second = leaf->first;
                    leaf->first = NULL;
                }
                insertkey(parent->key[0], second);
                parent->key[0] = first->key[1];
                removekey(first->key[1], first);
                second->first = first->third;
                if (second->first)
                    second->first->parent = second;
                first->third = NULL;
            }
        }
        else if (first == leaf) {
            if (!leaf->first) {
                leaf->first = leaf->second;
                leaf->second = NULL;
            }
            insertkey(parent->key[0], first);
            if (second->size == 2) {
                parent->key[0] = second->key[0];
                removekey(second->key[0], second);
                first->second = second->first;
                if (first->second)
                    first->second->parent = first;
                second->first = second->second;
                second->second = second->third;
                second->third = NULL;
            }
            else if (third->size == 2) {
                parent->key[0] = second->key[0];
                second->key[0] = parent->key[1];
                parent->key[1] = third->key[0];
                removekey(third->key[0], third);
                first->second = second->first;
                if (first->second)
                    first->second->parent = first;
                second->first = second->second;
                second->second = third->first;
                if (second->second)
                    second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = NULL;
            }
        }
    }
    else if (parent->size == 1) {
        insertkey(parent->key[0], leaf);
        if (first == leaf && second->size == 2) {
            parent->key[0] = second->key[0];
            removekey(second->key[0], second);
            if (!leaf->first)
                leaf->first = leaf->second;
            leaf->second = second->first;
            second->first = second->second;
            second->second = second->third;
            second->third = NULL;
            if (leaf->second)
                leaf->second->parent = leaf;
        }
        else if (second == leaf && first->size == 2) {
            parent->key[0] = first->key[1];
            removekey(first->key[1], first);
            if (!leaf->second)
                leaf->second = leaf->first;
            leaf->first = first->third;
            first->third = NULL;
            if (leaf->first)
                leaf->first->parent = leaf;
        }
    }
    return parent;
}

void printtree(node* tree, int n) {
    int i;
    if (!tree)
        return;
    printtree(tree->first, n + 2);
    for (i = 0; i < n; i++)
        printf("  ");
    printf("%d", tree->key[0]);
    printf("\n");
    printtree(tree->second, n + 2);
    for (int i = 0; i < n; i++)
        printf("  ");
    if (tree->size == 2)
        printf("%d", tree->key[1]);
    printf("\n");
    printtree(tree->third, n + 2);
}

void treedestroy(node* tree) {
    if (!tree)
        return;
    treedestroy(tree->first);
    treedestroy(tree->second);
    treedestroy(tree->third);
    free(tree);
}

int lab() {
    node* t = NULL;
    char command[16] = " ", ch;
    int key;
    while (fgets(command, 16, stdin)) {
        sscanf(command, "%c%i", &ch, &key);
        switch (ch) {
        case 'a':
            t = insertnode(t, key);
            break;
        case 'r':
            t = removenode(t, key);
            break;
        case 'f':
            if (search(t, key))
                fprintf(stdout, "yes\n");
            else
                fprintf(stdout, "no\n");
            break;
        case 'p':
            printtree(t, 1);
            fprintf(stdout, "\n");
            break;
        default:
            treedestroy(t);
            return 0;
        }
    }
    treedestroy(t);
    return 0;
}

int main() {
    lab();
    return 0;
}