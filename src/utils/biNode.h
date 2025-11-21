typedef struct biNode biNode;

void push(int, biNode**);
int valueAt(int, biNode*);
int pop(biNode**);
int popValue(int, biNode**);
int popAt(int, biNode**);
void printList(biNode*);
void clear(biNode**);
