// Ethan Johnson
// gcc lab4fall24.c lab4.c -std=c99

#include <stdlib.h>
#include <stdio.h>
#include "bst.h"

// Globals
link z, head;               // Pointers to sentinel and root
Item NULLitem = (-9999999); // Data for sentinel
link recycleList = NULL;    // Head of recycle list for reusing nodes

link NEW(Item item, link l, link r, int N, char tombstone) {
    link x;
    if (recycleList != NULL) {
        // Reuse node from recycle list
        x = recycleList;
        recycleList = recycleList->r;
    } else {
        x = malloc(sizeof *x);
    }
    x->item = item;
    x->l = l;
    x->r = r;
    x->N = N;
    x->tombstone = tombstone;
    return x;
}

void STinit() {
    head = (z = NEW(NULLitem, 0, 0, 0, 0));
    recycleList = NULL;
}

Item searchR(link h, Key v) {
    Key t;
    if (h == z) return NULLitem;
    t = key(h->item);
    if (eq(v, t)) {
        if (h->tombstone) return NULLitem;
        return h->item;
    }
    if (less(v, t)) return searchR(h->l, v);
    return searchR(h->r, v);
}

Item STsearch(Key v) {
    return searchR(head, v);
}

void fixN(link h) {
    int liveCount = 0;
    if (h != z) {
        liveCount = (h->tombstone) ? 0 : 1;
        h->N = h->l->N + h->r->N + liveCount;
    }
}

link STinsertR(link h, Item item) {
    Key v = key(item);
    if (h == z) return NEW(item, z, z, 1, 0);
    
    if (eq(v, key(h->item))) {
        if (h->tombstone) {
            h->tombstone = 0;
        }
        h->item = item;
    }
    else if (less(v, key(h->item)))
        h->l = STinsertR(h->l, item);
    else
        h->r = STinsertR(h->r, item);
    
    fixN(h);
    return h;
}

void STinsert(Item item) {
    head = STinsertR(head, item);
}

void STdelete(Key v) {
    link h = head;
    link parent = NULL;
    link path[1000];  // Stack to store path to node
    int pathLen = 0;
    
    // Find the node and store path
    while (h != z) {
        path[pathLen++] = h;
        if (eq(v, key(h->item))) {
            if (!h->tombstone) {
                h->tombstone = 1;
                // Update N counts for all nodes in path from node to root
                for (int i = pathLen - 1; i >= 0; i--) {
                    fixN(path[i]);
                }
            }
            return;
        }
        h = less(v, key(h->item)) ? h->l : h->r;
    }
}

Item selectR(link h, int i) {
    int r, leftCount;
    
    if (h == z) return NULLitem;
    
    leftCount = h->l->N;
    r = leftCount + (!h->tombstone ? 1 : 0);
    
    if (i <= leftCount)
        return selectR(h->l, i);
    else if (i == r && !h->tombstone)
        return h->item;
    else
        return selectR(h->r, i - r);
}

Item STselect(int k) {
    if (k < 1 || k > getLive()) {
        printf("Range error in STselect()\n");
        return NULLitem;
    }
    return selectR(head, k);
}

int invSelectR(link h, Key v) {
    Key t;
    int work;
    
    if (h == z) return -1;
    
    t = key(h->item);
    if (eq(v, t)) {
        if (h->tombstone) return -1;
        return h->l->N + 1;
    }
    if (less(v, t))
        return invSelectR(h->l, v);
    
    work = invSelectR(h->r, v);
    if (work == -1) return -1;
    return h->l->N + (!h->tombstone ? 1 : 0) + work;
}

int STinvSelect(Key v) {
    return invSelectR(head, v);
}

int getHeightR(link h) {
    int leftHeight, rightHeight;
    if (h == z) return 0;
    
    leftHeight = getHeightR(h->l);
    rightHeight = getHeightR(h->r);
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int getHeight() {
    return getHeightR(head);
}

void collectNodes(link h, link* liveList, link* deadList) {
    if (h == z) return;
    
    link rightChild = h->r;  // Store right child before modification
    collectNodes(rightChild, liveList, deadList);
    
    if (h->tombstone) {
        h->r = *deadList;
        *deadList = h;
    } else {
        h->r = *liveList;
        *liveList = h;
    }
    
    collectNodes(h->l, liveList, deadList);
}

link rebuildTree(link* nodeList, int n) {
    link root;
    link left;
    int leftSize;
    
    if (n == 0) return z;
    
    leftSize = n/2;
    
    left = rebuildTree(nodeList, leftSize);
    root = *nodeList;
    *nodeList = root->r;
    root->l = left;
    root->r = rebuildTree(nodeList, n - leftSize - 1);
    
    fixN(root);
    return root;
}

void removeDead() {
    link liveList = NULL;
    
    collectNodes(head, &liveList, &recycleList);
    
    head = rebuildTree(&liveList, getLive());
}

void printTree(link h, int depth) {
    int i;
    if (h == z) return;
    
    printTree(h->r, depth + 1);
    for (i = 0; i < depth; i++)
        printf("     ");
    if (h->tombstone)
        printf("(%d) %d\n", key(h->item), h->N);
    else
        printf("%d %d\n", key(h->item), h->N);
    printTree(h->l, depth + 1);
}

void STprintTree() {
    printTree(head, 0);
}

int checkInorder(link h, Key* lastKey) {
    if (h == z) return 1;
    
    if (!checkInorder(h->l, lastKey)) return 0;
    
    if (*lastKey != NULLitem && less(h->item, *lastKey)) {
        printf("Inorder violation\n");
        return 0;
    }
    *lastKey = key(h->item);
    
    return checkInorder(h->r, lastKey);
}

int checkN(link h) {
    int liveCount;
    if (h == z) return 1;
    
    if (!checkN(h->l) || !checkN(h->r)) return 0;
    
    liveCount = (h->tombstone) ? 0 : 1;
    if (h->N != h->l->N + h->r->N + liveCount) {
        printf("Count error at key %d\n", key(h->item));
        return 0;
    }
    
    return 1;
}

int verifyBSTproperties() {
    Key lastKey = NULLitem;
    return checkInorder(head, &lastKey) && checkN(head);
}

int getLive() {
    return head->N;
}

int getDead() {
    int count = 0;
    link h = head;
    link stack[1000];  // Assuming tree won't be deeper than 1000 nodes
    int top = 0;
    
    while (h != z || top > 0) {
        if (h != z) {
            if (h->tombstone) count++;
            stack[top++] = h;
            h = h->l;
        } else {
            h = stack[--top];
            h = h->r;
        }
    }
    return count;
}

int getRecycled() {
    int count = 0;
    link current = recycleList;
    while (current != NULL) {
        count++;
        current = current->r;
    }
    return count;
}