#include "base.h"

typedef long long elem_list_t;

const char DIR_NAME[] = {'d', 'u', 'm', 'p', __TIME__[0], __TIME__[1], __TIME__[3], __TIME__[4], __TIME__[6], __TIME__[7], '\0'};

#define LLIST_DEBUG_ON

#define LLIST_VERIFY(l_list) { \
    enum LLIST_errs err = LListVerify(l_list); \
    if (err != LLIST_OK) { \
        LOGG("LList ERROR code: %d\n", err); \
        abort(); \
    } \
}

#ifdef LLIST_DEBUG_ON
    #define LLIST_DUMP(l_list, message) LListDump(l_list, message);
    #define LLIST_ASSERT(l_list) { \
        HARD_ASSERT(l_list != NULL, NULL_LLIST); \
        HARD_ASSERT(l_list->list != NULL, NULL_LIST); \
    }
#else
    #define LLIST_DUMP
#endif

enum LLIST_errs {
    LLIST_OK = 0x0,
    NULL_LLIST = 0x1,
    NULL_LIST = 0x2,
    HEAD_OUT_OF_LIST = 0x3,
    TAIL_OUT_OF_LIST = 0x4,
    SIZE_OUT_OF_CAPACITY = 0x5,
    VERIFY_VISITED_FAIL = 0x6,
    CYCLE_OR_OUT_OF_LIST = 0x7,
    LINK_ERROR = 0x8,
    SIZE_ERROR = 0x9,
};

/////////////////////////////////////////////////////////// STRUCT ///////////////////////////////////////////////////////////
struct list_element {
    elem_list_t value;
    int64_t next;
    int64_t prev;
};

struct linked_list {
    struct list_element* list;

    uint64_t head;
    uint64_t tail;
    uint64_t capacity;
    uint64_t free;
    uint64_t size;
};

/////////////////////////////////////////////////////////// ПРОТОТИПЫ ФУНКЦИЙ ///////////////////////////////////////////////////////////
void InitLList(struct linked_list* l_list, size_t capacity);

void DestroyLList(struct linked_list* l_list);

void InsertElemLList(struct linked_list* l_list, size_t pos, elem_list_t val);

void RemoveElemLList(struct linked_list* l_list, size_t pos);

int64_t get_pos_in_list(struct linked_list* l_list, size_t real_pos);

void LListDump(struct linked_list* l_list, const char* message);

void LListGraphicDump(struct linked_list* l_list, const char* file_name);

enum LLIST_errs LListVerify(const struct linked_list* l_list);