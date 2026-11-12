#include "base.h"

typedef long long elem_list_t;

const char DIR_NAME[] = {'d', 'u', 'm', 'p', __TIME__[0], __TIME__[1], __TIME__[3], __TIME__[4], __TIME__[6], __TIME__[7], '\0'}; // snprintf

#define LLIST_DEBUG_ON

#define HAVE_ERROR 1
#define MIN_CAPACITY 4

#ifdef LLIST_DEBUG_ON
    #define LLIST_DUMP_VERIFY(l_list, message) LListDumpVerify(l_list, message);
    #define LLIST_ASSERT(l_list) { \
        HARD_ASSERT(l_list != NULL, NULL_LLIST); \
        HARD_ASSERT(l_list->list != NULL, NULL_LIST); \
    }
#else
    #define LLIST_DUMP_VERIFY(l_list, message)
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
    uint64_t next;
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

int64_t InsertElemLList(struct linked_list* l_list, size_t pos, elem_list_t val);

void RemoveElemLList(struct linked_list* l_list, size_t pos);

int64_t get_pos_in_list(struct linked_list* l_list, size_t real_pos);


//========================================================= Верификация и дамп =========================================================//
void LListDumpVerify(struct linked_list* l_list, const char* message);

int LListGraphicDump(struct linked_list* l_list, const char* file_name);

void _LListBasePartVerify(struct linked_list* l_list, FILE* dump_out, int* have_err);

void _DumpCheckLListElems(struct linked_list* l_list, FILE* dump_out, int* have_err);

void _DumpCheckLListFreeElems(struct linked_list* l_list, FILE* dump_out, int* have_err);


int CreateDir(const char* dir_name);