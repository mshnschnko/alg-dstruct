#include "23tree.h"

node* create_node(int value) {
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node) {
        new_node->first = NULL;
        new_node->second = NULL;
        new_node->third = NULL;
        new_node->fourth = NULL;
        new_node->parent = NULL;
        new_node->key[0] = value;
        new_node->size = 1;
        return new_node;
    }
    return NULL;
}

node* create_node2(int value, node* first, node* second, node* third, node* fourth, node* parent) {
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node) {
        new_node->first = first;
        new_node->second = second;
        new_node->third = third;
        new_node->fourth = fourth;
        new_node->parent = parent;
        new_node->key[0] = value;
        new_node->size = 1;
        return new_node;
    }
    return NULL;
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

void sort_keys(int size, int* key) { // сортировка ключей в вершинах
    if (size == 1)
        return;
    if (size == 2)
        sort2(&key[0], &key[1]);
    if (size == 3)
        sort3(&key[0], &key[1], &key[2]);
}

void insert_key_to_node(int value, node* tree_node) {  // вставка ключа в узел
    if (tree_node == NULL)
        return;
    tree_node->key[tree_node->size] = value;
    tree_node->size++;
    sort_keys(tree_node->size, tree_node->key);
}

void remove_key_from_node(int value, node* tree_node) {// удаление ключа из узла
    if (tree_node == NULL)
        return;
    if (tree_node->size >= 1 && tree_node->key[0] == value) {
        tree_node->key[0] = tree_node->key[1];
        tree_node->key[1] = tree_node->key[2];
        tree_node->size--;
    }
    else if (tree_node->size == 2 && tree_node->key[1] == value) {
        tree_node->key[1] = tree_node->key[2];
        tree_node->size--;
    }
}

void become_node2(int value, int* key, node* tree_node, node* first_, node* second_) {
    key[0] = value;
    tree_node->first = first_;
    tree_node->second = second_;
    tree_node->third = NULL;
    tree_node->fourth = NULL;
    tree_node->parent = NULL;
    tree_node->size = 1;
}

char is_leaf(node* tree) { // проверка на лист
    if (tree == NULL)
        return False;
    if ((tree->first == NULL) && (tree->second == NULL) && (tree->third == NULL))
        return True;
    return False;
}

node* insert_node(node* tree, int value) {// вставка узла
    if (tree == NULL)
        return create_node(value); // если дерево пусто, то создаем первую 2-3-вершину (корень)
    if (search(tree, value))
        return split(tree);
    if (is_leaf(tree))
        insert_key_to_node(value, tree);
    else if (value <= tree->key[0])
        insert_node(tree->first, value);
    else if ((tree->size == 1) || ((tree->size == 2) && value <= tree->key[1]))
        insert_node(tree->second, value);
    else
        insert_node(tree->third, value);
    return split(tree);
}

node* split(node* item) {
    if (item == NULL)
        return NULL;
    if (item->size < 3)
        return item;
    node* x = create_node2(item->key[0], item->first, item->second, NULL, NULL, item->parent); // Создаем две новые вершины
    node* y = create_node2(item->key[2], item->third, item->fourth, NULL, NULL, item->parent);
    if (x->first)
        x->first->parent = x;
    if (x->second)
        x->second->parent = x;
    if (y->first)
        y->first->parent = y;
    if (y->second)
        y->second->parent = y;
    if (item->parent != NULL) {
        insert_key_to_node(item->key[1], item->parent);
        if (item->parent->first == item)
            item->parent->first = NULL;
        else if (item->parent->second == item)
            item->parent->second = NULL;
        else if (item->parent->third == item)
            item->parent->third = NULL;
        if (item->parent->first == NULL) {
            item->parent->fourth = item->parent->third;
            item->parent->third = item->parent->second;
            item->parent->second = y;
            item->parent->first = x;
        }
        else if (item->parent->second == NULL) {
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
        become_node2(item->key[1], item->key, item, x, y);
        return item;
    }
}

node* search(node* tree, int value) { // поиск ключа
    if (tree == NULL)
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

node* search_min(node* tree) {
    if (tree == NULL)
        return tree;
    if ((tree->first) == NULL)
        return tree;
    else
        return search_min(tree->first);
}

node* remove_node(node* tree, int value) {
    if (tree == NULL)
        return NULL;
    node* item = search(tree, value);
    if (item == NULL)
        return tree;
    node* min = NULL;
    if (item->key[0] == value)
        min = search_min(item->second); // эквивалентный ключ
    else
        min = search_min(item->third);
    if (min != NULL) { // меняем ключи местами
        int* z = (value == item->key[0] ? &(item->key[0]) : &(item->key[1]));
        swap(z, &min->key[0]);
        item = min; // перемещаем указатель на лист
    }
    remove_key_from_node(value, item); // удаляем требуемый ключ из листа
    return fix(item);
}

node* fix(node* leaf) {
    if (leaf == NULL)
        return NULL;
    if (leaf->size == 0 && leaf->parent == NULL) { // единственный ключ
        free(leaf);
        return NULL;
    }
    if (leaf->size != 0) { // два ключа
        if (leaf->parent)
            return fix(leaf->parent);
        else
            return leaf;
    }
    node* parent = leaf->parent;
    if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2)
        leaf = redistribute(leaf); // достаточно перераспределить ключи
    else if (parent->size == 2 && parent->third->size == 2)
        leaf = redistribute(leaf); // аналогично
    else
        leaf = merge(leaf);
    return fix(leaf);
}

node* merge(node* leaf) {
    if (leaf == NULL)
        return NULL;
    node* parent = leaf->parent;
    if (parent->first == leaf) {
        insert_key_to_node(parent->key[0], parent->second);
        parent->second->third = parent->second->second;
        parent->second->second = parent->second->first;
        if (leaf->first != NULL)
            parent->second->first = leaf->first;
        else if
            (leaf->second != NULL) parent->second->first = leaf->second;
        if (parent->second->first != NULL)
            parent->second->first->parent = parent->second;
        remove_key_from_node(parent->key[0], parent);
        free(parent->first);
        parent->first = NULL;
    }
    else if (parent->second == leaf) {
        insert_key_to_node(parent->key[0], parent->first);
        if (leaf->first != NULL)
            parent->first->third = leaf->first;
        else if (leaf->second != NULL)
            parent->first->third = leaf->second;
        if (parent->first->third != NULL)
            parent->first->third->parent = parent->first;
        remove_key_from_node(parent->key[0], parent);
        free(parent->second);
        parent->second = NULL;
    }
    if (parent->parent == NULL) {
        node* tmp = NULL;
        if (parent->first != NULL)
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
    if (leaf == NULL)
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
            insert_key_to_node(parent->key[0], parent->first);
            parent->first->third = parent->first->second;
            parent->first->second = parent->first->first;
            if (leaf->first != NULL)
                parent->first->first = leaf->first;
            else if
                (leaf->second != NULL) parent->first->first = leaf->second;
            if (parent->first->first != NULL)
                parent->first->first->parent = parent->first;
            remove_key_from_node(parent->key[0], parent);//delete first;
        }
        else if (second == leaf) {
            insert_key_to_node(parent->key[0], first);
            remove_key_from_node(parent->key[0], parent);
            if (leaf->first != NULL)
                first->third = leaf->first;
            else if (leaf->second != NULL)
                first->third = leaf->second;
            if (first->third != NULL)
                first->third->parent = first;
            parent->second = parent->third;
            parent->third = NULL;
            free(second);
        }
        else if (third == leaf) {
            insert_key_to_node(parent->key[1], second);
            parent->third = NULL;
            remove_key_from_node(parent->key[1], parent);
            if (leaf->first != NULL)
                second->third = leaf->first;
            else if (leaf->second != NULL)
                second->third = leaf->second;
            if (second->third != NULL)
                second->third->parent = second;
            free(third);
        }
    }
    else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2))) {
        if (third == leaf) {
            if (leaf->first != NULL) {
                leaf->second = leaf->first;
                leaf->first = NULL;
            }
            insert_key_to_node(parent->key[1], leaf);
            if (second->size == 2) {
                parent->key[1] = second->key[1];
                remove_key_from_node(second->key[1], second);
                leaf->first = second->third;
                second->third = NULL;
                if (leaf->first != NULL)
                    leaf->first->parent = leaf;
            }
            else if (first->size == 2) {
                parent->key[1] = second->key[0];
                leaf->first = second->second;
                second->second = second->first;
                if (leaf->first != NULL)
                    leaf->first->parent = leaf;
                second->key[0] = parent->key[0];
                parent->key[0] = first->key[1];
                remove_key_from_node(first->key[1], first);
                second->first = first->third;
                if (second->first != NULL)
                    second->first->parent = second;
                first->third = NULL;
            }
        }
        else if (second == leaf) {
            if (third->size == 2) {
                if (leaf->first == NULL) {
                    leaf->first = leaf->second;
                    leaf->second = NULL;
                }
                insert_key_to_node(parent->key[1], second);
                parent->key[1] = third->key[0];
                remove_key_from_node(third->key[0], third);
                second->second = third->first;
                if (second->second != NULL)
                    second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = NULL;
            }
            else if (first->size == 2) {
                if (leaf->second == NULL) {
                    leaf->second = leaf->first;
                    leaf->first = NULL;
                }
                insert_key_to_node(parent->key[0], second);
                parent->key[0] = first->key[1];
                remove_key_from_node(first->key[1], first);
                second->first = first->third;
                if (second->first != NULL)
                    second->first->parent = second;
                first->third = NULL;
            }
        }
        else if (first == leaf) {
            if (leaf->first == NULL) {
                leaf->first = leaf->second;
                leaf->second = NULL;
            }
            insert_key_to_node(parent->key[0], first);
            if (second->size == 2) {
                parent->key[0] = second->key[0];
                remove_key_from_node(second->key[0], second);
                first->second = second->first;
                if (first->second != NULL)
                    first->second->parent = first;
                second->first = second->second;
                second->second = second->third;
                second->third = NULL;
            }
            else if (third->size == 2) {
                parent->key[0] = second->key[0];
                second->key[0] = parent->key[1];
                parent->key[1] = third->key[0];
                remove_key_from_node(third->key[0], third);
                first->second = second->first;
                if (first->second != NULL)
                    first->second->parent = first;
                second->first = second->second;
                second->second = third->first;
                if (second->second != NULL)
                    second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = NULL;
            }
        }
    }
    else if (parent->size == 1) {
        insert_key_to_node(parent->key[0], leaf);
        if (first == leaf && second->size == 2) {
            parent->key[0] = second->key[0];
            remove_key_from_node(second->key[0], second);
            if (leaf->first == NULL)
                leaf->first = leaf->second;
            leaf->second = second->first;
            second->first = second->second;
            second->second = second->third;
            second->third = NULL;
            if (leaf->second != NULL)
                leaf->second->parent = leaf;
        }
        else if (second == leaf && first->size == 2) {
            parent->key[0] = first->key[1];
            remove_key_from_node(first->key[1], first);
            if (leaf->second == NULL)
                leaf->second = leaf->first;
            leaf->first = first->third;
            first->third = NULL;
            if (leaf->first != NULL)
                leaf->first->parent = leaf;
        }
    }
    return parent;
}

void print_tree(node* tree) {
    if (tree == NULL)
        return;
    print_tree(tree->first);
    for (int i = 0; i < tree->size; i++) {
        printf("%d ", tree->key[i]);
    }
    print_tree(tree->second);
    print_tree(tree->third);
}

void destroy_tree(node* tree) {
    if (tree == NULL)
        return;
    destroy_tree(tree->first);
    destroy_tree(tree->second);
    destroy_tree(tree->third);
    free(tree);
}