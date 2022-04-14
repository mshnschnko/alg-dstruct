#pragma once
#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False !(True)

typedef struct node {
    int size;      // количество зан€тых ключей
    int key[3];
    struct node* first;   // *first <= key[0];
    struct node* second;  // key[0] <= *second < key[1];
    struct node* third;   // key[1] <= *third < key[2];
    struct node* fourth;  // kye[2] <= *fourth.
    struct node* parent;

}node;

char find(int value, int* key, int size);  // поиск ключа
void swap(int* x, int* y);
void sort2(int* x, int* y);
void sort3(int* x, int* y, int* z);
void sort_keys(int size, int* key); // сортировка ключей
node* create_node(int value); //создание узла, если дерево пустое
node* create_node2(int value, node* first, node* second, node* third, node* fourth, node* parent); //создание узла с параметрами
void insert_key_to_node(int value, node* tree_node); //вставка ключа в узел
void remove_key_from_node(int value, node* tree_node); //удаление ключа из узла
void become_node2(int value, int* key, node* tree_node, node* first_, node* second_);
char is_leaf(node* tree);  // проверка на лист
node* split(node* item); // разделение узла при переполнении
node* insert_node(node* tree, int value); // вставка узла
node* search(node* tree, int value); // поиск элемента
node* search_min(node* tree);// поиск минимального элемента в поддереве
node* merge(node* leaf); // сли€ние
node* redistribute(node* leaf); // перераспеределение ключей в узлах
node* fix(node* leaf); // восстановаление дерева после удалени€
node* remove_node(node* tree, int value); // удалени€ узла
void destroy_tree(node* t); // удаление дерева
void print_tree(node* tree); // печать дерева