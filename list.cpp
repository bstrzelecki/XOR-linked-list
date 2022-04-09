#include <cstring>
#include <cstdio>
typedef struct node {
    int data;
    node* ptr; // XOR of next and previous pointer
} ListNode ;

typedef struct {
    ListNode* start = nullptr;
    ListNode* end = nullptr;
    ListNode* actual = nullptr;
    ListNode* next = nullptr;
    ListNode* previous = nullptr;
} List;

ListNode* getPointer(ListNode* left, ListNode* right){
    return (ListNode*)((uintptr_t)left^(uintptr_t)right);
}

// Resets empty list
void clear(List* list) {
    list->start = nullptr;
    list->end = nullptr;
    list->actual = nullptr;
    list->previous = nullptr;
    list->next = nullptr;
}

void printList(ListNode* head){
    ListNode* current = head;
    ListNode* previous = nullptr;
    while(current != nullptr){
        printf("%d ", current->data);
        ListNode* tmp = getPointer(previous, current->ptr);
        previous = current;
        current = tmp;
    }
    printf("\n");
}

void pushBegin(List* list, int value){
    ListNode* first = new ListNode();
    first->data = value;
    first->ptr = getPointer(nullptr, list->start);
    if(list->start != nullptr){
        list->start->ptr = getPointer(first, getPointer(nullptr, list->start->ptr));
    }
    list->start = first;
    if(list->actual == nullptr) {
        list->actual = first;
    }else if(list->previous == nullptr){
        list->previous = first;
    }
    if(list->end == nullptr) list->end = first;
}

void deleteBegin(List* list){
    if(list->start == nullptr)return;
    ListNode* node = list->start;
    ListNode* next = getPointer(nullptr, node->ptr);
    if(next == nullptr)
    {
        delete node;
        clear(list);
        return;
    }
    ListNode* next2 = getPointer(node, next->ptr);
    if(node == list->previous)list->previous = nullptr;
    next->ptr = getPointer(nullptr, next2);
    list->start = next;
    if(node == list->actual){
        list->next = nullptr;
        list->actual = list->end;
        list->previous = getPointer(list->actual->ptr, list->next);
    }
    delete node;

}

// Memory cleanup
void dispose(List* list){
    while(list->start != nullptr){
        deleteBegin(list);
    }
}


void pushBack(List* list, int value){
    ListNode* last = new ListNode();
    last->data = value;
    last->ptr = getPointer(list->end, nullptr);
    if(list->end != nullptr){
        list->end->ptr = getPointer(last, getPointer(nullptr, list->end->ptr));
    }
    list->end = last;
    if(list->actual == nullptr) {
        list->actual = last;
    }else if(list->next == nullptr){
        list->next = last;
    }
    if(list->start == nullptr) list->start = last;
}

void deleteEnd(List* list){
    if(list->end == nullptr)return;
    ListNode* node = list->end;
    ListNode* prev = getPointer(node->ptr, nullptr);
    if(prev == nullptr)
    {
        delete node;
        clear(list);
        return;
    }
    ListNode* prev2 = getPointer(node, prev->ptr);
    if(node == list->next) list->next = nullptr;
    if(node == list->actual){
        list->actual = list->previous;
        list->previous = prev2;
    }
    delete node;
    prev->ptr = getPointer(nullptr, prev2);
    list->end = prev;

}

// Move actual to next node
void moveNext(List* list){
    if(list->start == nullptr)return;
    ListNode* node = list->next;
    if(node == nullptr) {
        list->previous = nullptr;
        list->actual = list->start;
        list->next = getPointer(list->previous, list->actual->ptr);
        return;
    }
    list->previous = list->actual;
    list->next = getPointer(list->previous, list->next->ptr);
    list->actual = node;
}

void deleteActual(List* list){

    if(list->previous == nullptr){
        deleteBegin(list);
        return;
    }
    if(list->next == nullptr){
        deleteEnd(list);
        return;
    }
    ListNode* prev2 = getPointer(list->previous->ptr, list->actual);
    ListNode* next2 = getPointer(list->actual, list->next->ptr);

    list->next->ptr = getPointer(list->previous, next2);
    list->previous->ptr = getPointer(prev2, list->next);

    delete list->actual;
    list->actual = list->previous;
    list->previous = prev2;
}

// Move actual to previous node
void movePrev(List* list){
    if(list->end == nullptr) return;
    ListNode* node = list->previous;
    if(node == nullptr) {
        list->next = nullptr;
        list->actual = list->end;
        list->previous = getPointer(list->next, list->actual->ptr);
        return;
    }
    list->next = list->actual;
    list->previous = getPointer(list->previous->ptr, list->next);
    list->actual = node;
}

void deleteValue(List* list, int value){
    ListNode* current = list->start;
    ListNode* previous = nullptr;
    while(current != nullptr){

        ListNode* tmp = getPointer(previous, current->ptr);
        if(current->data == value){
            if(previous == nullptr){
                deleteBegin(list);
                current = list->start;
                continue;
            }
            if(tmp == nullptr){
                deleteEnd(list);
                return;
            }
            ListNode* prev2 = getPointer(previous->ptr, current);
            ListNode* next2 = getPointer(current, tmp->ptr);
            tmp->ptr = getPointer(previous, next2);
            previous->ptr = getPointer(prev2, tmp);

            delete current;
        }else{
            previous = current;
        }
        current = tmp;

    }
}

void pushActual(List* list, int value){
    if(list->previous == nullptr) {
        pushBegin(list, value);
        return;
    }

    ListNode* added = new ListNode();
    added->data = value;
    added->ptr = getPointer(list->previous, list->actual);
    ListNode* prev2 = getPointer(list->previous->ptr, list->actual);
    list->previous->ptr = getPointer(prev2, added);
    list->actual->ptr = getPointer(added, list->next);
    list->previous = added;
}

void printActual(const List* list) {
    if(list->actual == nullptr){
        printf("NULL\n");
    }else
        printf("%d\n", list->actual->data);
}

void resolveInput(const char* input, List* list) {
    if(strcmp("ADD_BEG", input) == 0){
        int n;
        scanf("%i", &n);
        pushBegin(list, n);
    } else if(strcmp("ADD_END",input) == 0){
        int n;
        scanf("%i", &n);
        pushBack(list,n);
    } else if(strcmp("ACTUAL",input) == 0){
        printActual(list);
    } else if(strcmp("NEXT",input) == 0){
        moveNext(list);
        printActual(list);
    } else if(strcmp("PREV",input) == 0){
        movePrev(list);
        printActual(list);
    } else if(strcmp("DEL_BEG",input) == 0){
        deleteBegin(list);
    } else if(strcmp("DEL_END",input) == 0){
        deleteEnd(list);
    } else if(strcmp("DEL_ACT",input) == 0){
        deleteActual(list);
    } else if(strcmp("PRINT_FORWARD",input) == 0){
        if(list->start == nullptr)
            printf("NULL\n");
        else
            printList(list->start);
    } else if(strcmp("PRINT_BACKWARD",input) == 0){
        if(list->end == nullptr)
            printf("NULL\n");
        else
            printList(list->end);
    } else if(strcmp("DEL_VAL",input) == 0){
        int n;
        scanf("%i", &n);
        deleteValue(list,n);
    } else if(strcmp("ADD_ACT",input) == 0) {
        int n;
        scanf("%i", &n);
        pushActual(list,n);
    } else {
        printf("%25s\n", input);
    }
}

int main() {

    char input[25];
    List* list = new List();
    while(scanf("%24s", input) != EOF){
        resolveInput(input, list);
    }

    dispose(list);
    delete list;

    return 0;
}
