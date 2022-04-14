#pragma once
#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False !(True)

typedef struct node {
    int size;      // ���������� ������� ������
    int key[3];
    struct node* first;   // *first <= key[0];
    struct node* second;  // key[0] <= *second < key[1];
    struct node* third;   // key[1] <= *third < key[2];
    struct node* fourth;  // kye[2] <= *fourth.
    struct node* parent;

}node;

char find(int value, int* key, int size);  // ����� �����
void swap(int* x, int* y);
void sort2(int* x, int* y);
void sort3(int* x, int* y, int* z);
void sort_keys(int size, int* key); // ���������� ������
node* create_node(int value); //�������� ����, ���� ������ ������
node* create_node2(int value, node* first, node* second, node* third, node* fourth, node* parent); //�������� ���� � �����������
void insert_key_to_node(int value, node* tree_node); //������� ����� � ����
void remove_key_from_node(int value, node* tree_node); //�������� ����� �� ����
void become_node2(int value, int* key, node* tree_node, node* first_, node* second_);
char is_leaf(node* tree);  // �������� �� ����
node* split(node* item); // ���������� ���� ��� ������������
node* insert_node(node* tree, int value); // ������� ����
node* search(node* tree, int value); // ����� ��������
node* search_min(node* tree);// ����� ������������ �������� � ���������
node* merge(node* leaf); // �������
node* redistribute(node* leaf); // ������������������ ������ � �����
node* fix(node* leaf); // ��������������� ������ ����� ��������
node* remove_node(node* tree, int value); // �������� ����
void destroy_tree(node* t); // �������� ������
void print_tree(node* tree); // ������ ������