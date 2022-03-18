#include <stdlib.h>
#include <stdio.h>

typedef struct node {
	int keys[2];
	int sonsCount;
	node* parent;
	node* sons[3];
} node;

node* root;

node* createnode(void) {
	node* t = NULL;
	t = (node*)malloc(sizeof(node));
	if (!t)
		return NULL;
	t->sonsCount = 0;
	return t;
}

void swap(node* t) {
	int r = t->sons[0];
	t->sons[0] = t->sons[1];
	t->sons[1] = r;
}

void sort2(node* t) {
	if (t->sons[0] > t->sons[1]) swap(t);
}

void sort3(node* t) {
	if (t->sons[0] > t->sons[1]) swap(t);
	if (t->sons[0] > t->sons[2]) swap(t);
	if (t->sons[1] > t->sons[2]) swap(t);
}

void sort(node* t) {
	if (t->sonsCount == 1) return;
	if (t->sonsCount == 2) sort2(t);
	if (t->sonsCount == 3) sort3(t);
}

int maxkey(node* t) {
	int max = t->sons[0];
	for (int i = 0; i < t->sonsCount; ++i)
		if (t->sons[i] > max)
			max = t->sons[i];
	return max;
}

node* search(node* t, int x) {
	while (t->sonsCount != 0) {
		if (t->sonsCount == 2) {
			if (t->keys[0] < x)
				t = t->sons[1];
			else 
				t = t->sons[0];
		}
		else if (t->keys[1] < x)
			t = t->sons[2];
		else if (t->keys[0] < x)
			t = t->sons[1];
		else
			t = t->sons[0];
	}
	return t;
}

void splitparent(node* t) {
	if (t->sonsCount > 3) {
		node* a = createnode();
		a->sons[0] = t->sons[2];
		a->sons[1] = t->sons[3];
		a->keys[0] = t->keys[2];
		t->sons[2]->parent = a;
		t->sons[3]->parent = a;
		t->sonsCount = 2;
		t->sons[2] = NULL;
		t->sons[3] = NULL;
		if (t->parent != NULL) {
			t->parent = a;
			t->sonsCount++;
			sort(t->parent);
			splitparent(t->parent);
		}
		else {
			t = root;
			root->sons[0] = t;
			root->sons[1] = a;
			t->parent = root;
			a->parent = root;
			root->sonsCount = 2;
			sort(root);
		}
	}
}

void updatekeys(node* t) {
	node* a = createnode();
	if (!a)
		return;
	a = t->parent;
	while (a) {
		for (int i = 0; i < a->sonsCount; ++i)
			a->keys[i] = maxkey(a->sons[i]);
		a = a->parent;
	}
}

void insert(node* t, int x) {
	node* n = createnode();
	if (!root) {
		root = n;
		return;
	}
	node* a = createnode();
	if (!a)
		return;
	a = search(root, x);
	if (!a->parent) {
		t = root;
		root->sons[0] = t;
		root->sons[1] = n;
		t->parent = root;
		n->parent = root;
		root->sonsCount = 2;
		sort(root);
	}
	else {
		node* p = createnode();
		if (!p)
			return;
		p = a->parent;
		p->sons[p->sonsCount] = n;
		p->sonsCount++;
		n->parent = p;
		sort(p);
		updatekeys(n);
		splitparent(n);
	}
	updatekeys(n);
}

int lab() {
	root = createnode();
	return 0;
}

int main() {
	lab();
	return 0;
}