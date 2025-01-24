// Unbalanced binary search tree header file - Lab 4, Fall 2024

// These will have to change if data in node is more than just an int.
typedef int Key;
typedef Key Item;
#define key(A) (A)
#define less(A, B) (key(A) < key(B))
#define eq(A, B) (key(A) == key(B))

typedef struct STnode* link;

struct STnode 
{
Item item;  // Data for this node
link l, r;  // left & right links
char tombstone; // 0 = live key, 1 = dead key
int N;      // subtree size (counts only live nodes)
};

extern Item NULLitem;

void STinit();          // Initialize tree with just a sentinel

Item STsearch(Key v);   // Find node for a key

Item STselect(int k);   // Treat tree as flattened into an ordered array

int STinvSelect(Key v); // Inverse of STselect

void STinsert(Item item);     // Insert an item.  No uniqueness check

void STdelete(Key v);         // Delete item for a key, if present.

int verifyBSTproperties();    // Ensure that tree isn't damaged

void STprintTree();           // Dumps out tree

int getLive();  // Number of nodes in tree with active keys.

int getDead();  // Number of nodes in tree with tombstone set.

int getRecycled();  // Number of elements available from last removeDead()

int getHeight();  // Longest path to sentinel

void removeDead();  // Rebuild tree and put tombstoned elements in recycling list.