#include "d_linked_list.h"

int main() {
    START_LOGG;
    LOGG("<pre>\n");
    _mkdir(DIR_NAME);

    struct linked_list l_list = {};
    
    InitLList(&l_list, 10);
    InsertElemLList(&l_list, 0, 10);
    InsertElemLList(&l_list, 1, 20);
    InsertElemLList(&l_list, 2, 30);
    InsertElemLList(&l_list, 1, 15);
    RemoveElemLList(&l_list, 2);
    InsertElemLList(&l_list, 1, 20);
    LListDump(&l_list, "");

    STOP_LOGG;
    return 0;
}

void InitLList(struct linked_list* l_list, size_t capacity) {
    if (capacity == 0)
        capacity = 1;
    
    l_list->list = (struct list_element*)calloc(capacity, sizeof(l_list->list[0]));

    l_list->head = 0;
    l_list->tail = 0;
    l_list->capacity = capacity;
    l_list->free = 1;
    l_list->size = 0;

    l_list->list[0].next = 0;
    l_list->list[0].prev = 0;
    l_list->list[0].value = 0;

    for (size_t i = 1; i < capacity; i++) {
        l_list->list[i].next = i + 1;
        l_list->list[i].prev = -1;
    }
    l_list->list[capacity - 1].next = 0;
}

void DestroyLList(struct linked_list* l_list) {
    free(l_list->list);

    l_list->head = 0;
    l_list->tail = 0;
}

int64_t GetFreePos(struct linked_list* l_list) {
    int64_t free_pos = l_list->free;
    if (free_pos == 0) return 0;
    l_list->free = l_list->list[free_pos].next;

    return free_pos;
}

void InsertElemLList(struct linked_list* l_list, size_t pos, elem_list_t val) {
    LLIST_DUMP(l_list, "BEFORE INSERT\n");
    if (pos >= l_list->capacity || (pos != 0 && l_list->list[pos].prev == -1))
        return;

    if (l_list->size + 1 >= l_list->capacity) {
        size_t old_cap = l_list->capacity;
        size_t new_cap = 2 * l_list->capacity;
        struct list_element* list = (struct list_element*)_recalloc(l_list->list, new_cap, sizeof(l_list->list[0]));

        if (list == NULL) {
            return; // ошибка
        }

        l_list->list = list;
        
        for (size_t i = old_cap; i < new_cap; i++) {
            l_list->list[i].next = i + 1;
            l_list->list[i].prev = -1;
            l_list->list[i].value = 0;
        }
        l_list->list[new_cap - 1].next = l_list->free;
        l_list->free = old_cap;
        l_list->capacity = new_cap;
    }

    int64_t free_pos = GetFreePos(l_list);

    if (free_pos == 0) {
        return;
    }

    l_list->list[free_pos].value = val;    

    size_t next_pos = l_list->list[pos].next;

    l_list->list[free_pos].prev = pos;
    l_list->list[free_pos].next = next_pos;

    if (pos != 0)
        l_list->list[pos].next = free_pos;

    if (next_pos != 0)
        l_list->list[next_pos].prev = free_pos;

    if (l_list->size == 0 || pos == l_list->tail)
        l_list->tail = free_pos;

    if (l_list->size == 0)
        l_list->head = free_pos;

    l_list->size++;
    LLIST_DUMP(l_list, "AFTER INSERT\n");
}

void RemoveElemLList(struct linked_list* l_list, size_t pos) {
    if (l_list->size == 0 || pos == 0 || pos >= l_list->capacity || l_list->list[pos].prev == -1)
        return;
    
    size_t next_pos = l_list->list[pos].next;
    size_t prev_pos = l_list->list[pos].prev;

    if (pos == l_list->head) {
        l_list->head = next_pos;
        if (next_pos != 0)
            l_list->list[next_pos].prev = 0;
    } else {
        l_list->list[prev_pos].next = next_pos;
    }

    if (pos == l_list->tail) {
        l_list->tail = prev_pos;
        if (prev_pos != 0)
            l_list->list[prev_pos].next = 0;
    } else {
        l_list->list[next_pos].prev = prev_pos;
    }

    if (l_list->size == 1) {
        l_list->head = 0;
        l_list->tail = 0;
    }

    l_list->list[pos].value = 0;
    l_list->list[pos].prev = -1;
    l_list->list[pos].next = l_list->free;
    l_list->free = pos;

    l_list->size--;
}

int64_t get_pos_in_list(struct linked_list* l_list, size_t real_pos) {
    size_t current_pos = l_list->head;

    for (int i = 0; i < real_pos; i++) {
        if (current_pos == l_list->tail)
            return -1;
        
        current_pos = l_list->list[current_pos].next;
    }

    return current_pos;
}

void SetDumpNode(FILE* dump_out, const size_t index, const struct list_element* node, const char* color, const char* fillcolor) {
    fprintf(dump_out, "  NODE%zu[color = \"%s\", fillcolor = \"%s\", label=\" node = %zu | next = %zd | prev = %zd | val = %lld\"];\n", 
            index, color, fillcolor, index, node->next, node->prev, node->value);
}

void LListDump(struct linked_list* l_list, const char* message) {
    LOGG(message);
    time_t now_time = 0;
    struct tm* time_info = NULL;

    time(&now_time);
    time_info = localtime(&now_time);
    clock_t tik_now = clock();

    char file_name[200] = "";
    strftime(file_name, sizeof(file_name), "dump%d_%m_%Y_%H_%M_%S_", time_info);
    snprintf(file_name + strlen(file_name), 200, "%d", tik_now);
    LListGraphicDump(l_list, file_name);
    LOGG("<img src=\"%s\\%s.png\">\n", DIR_NAME, file_name);
}

void LListGraphicDump(struct linked_list* l_list, const char* file_name) {
    char file_dot[200] = "";
    sprintf(file_dot, "%s\\%s.dot", DIR_NAME, file_name);

    FILE* dump_out = fopen(file_dot, "w");

    fprintf(dump_out, "digraph {\n");
    fprintf(dump_out, "  rankdir = LR;\n  node[shape = \"Mrecord\", style = filled, fontsize = 12];\n");
    fprintf(dump_out, "  graph[splines = ortho, nodesep = 0.5];\n");
    fprintf(dump_out, "  edge[radius = 5];\n");
    
    SetDumpNode(dump_out, 0, &(l_list->list[0]), "#000000", "#B0B0B0");
    for (size_t i = 1; i < l_list->capacity; i++) {           
        if (l_list->list[i].prev == -1)
            SetDumpNode(dump_out, i, &(l_list->list[i]), "#404040", "#E0E0E0");
        else
            SetDumpNode(dump_out, i, &(l_list->list[i]), "#008000", "#C0FFC0");
    }
    fprintf(dump_out, "  HEAD[color = \"#000080\", fillcolor = \"#C0C0FF\", label=\"HEAD\"];\n");
    fprintf(dump_out, "  TAIL[color = \"#808000\", fillcolor = \"#FFFFC0\", label=\"TAIL\"];\n");

    fprintf(dump_out, "  edge[color = \"white\", weight = 1000, dir = both];\n");
    fprintf(dump_out, "  NODE0");
    for (size_t i = 1; i < l_list->capacity; i++) {
        fprintf(dump_out, "->NODE%zu", i);
    }
    fprintf(dump_out, ";\n");

    if (l_list->size > 0) {
        fprintf(dump_out, "  edge[color = \"black\", weight = 10];\n");
        size_t current_pos = l_list->head;
        fprintf(dump_out, "  NODE%zu", current_pos);
        int i = 0;

        while (true) {
            if (current_pos == l_list->tail)
                break;
            
            current_pos = l_list->list[current_pos].next;
            fprintf(dump_out, "->NODE%zu", current_pos);
        }
        fprintf(dump_out, ";\n");

        fprintf(dump_out, "  edge[color = \"#000080\", weight = 5, dir = forward];\n");
        fprintf(dump_out, "  { rank=same; NODE%zu; HEAD; }\n", l_list->head);
        fprintf(dump_out, "  HEAD->NODE%zu;\n", l_list->head);
        fprintf(dump_out, "  edge[color = \"#808000\", weight = 5, dir = forward];\n");
        fprintf(dump_out, "  { rank=same; NODE%zu; TAIL; }\n", l_list->tail);
        fprintf(dump_out, "  TAIL->NODE%zu;\n", l_list->tail);
    }

    fprintf(dump_out, "}\n");

    fclose(dump_out);
    char sys_cmd[200] = "";
    sprintf(sys_cmd, "\"C:\\Program Files\\Graphviz\\bin\\dot.exe\" -Tpng %s\\%s.dot -o %s\\%s.png", DIR_NAME, file_name, DIR_NAME, file_name);
    system(sys_cmd);
}

enum LLIST_errs LListVerify(const struct linked_list* l_list) {
    if (l_list == NULL)    return NULL_LLIST;
    if (l_list->list == NULL)    return NULL_LIST;
    if (l_list->size >= l_list->capacity)    return SIZE_OUT_OF_CAPACITY;

    if (l_list->size == 0 && l_list->head == 0 && l_list->tail == 0) {
        return LLIST_OK;
    }

    if (l_list->head == 0 || l_list->head >= l_list->capacity)    return HEAD_OUT_OF_LIST;
    if (l_list->tail == 0 || l_list->tail >= l_list->capacity)    return TAIL_OUT_OF_LIST;

    int* visited = (int*)calloc(l_list->capacity, sizeof(int));
    if (visited == NULL) return VERIFY_VISITED_FAIL;

    size_t current_pos = l_list->head;
    uint64_t cur_sz = 0;
    
    while (current_pos != 0 && current_pos != l_list->tail) {   
        if (current_pos >= l_list->capacity || visited[current_pos] != 0)   {
            free(visited);
            return CYCLE_OR_OUT_OF_LIST;
        }
        visited[current_pos] = 1;
        cur_sz++;

        int64_t next_pos = l_list->list[current_pos].next;
        if (next_pos != 0 && next_pos < l_list->capacity && l_list->list[next_pos].prev != current_pos) {
            free(visited);
            return LINK_ERROR;
        }
        current_pos = next_pos;
    }

    if (current_pos == l_list->tail) {
        visited[current_pos] = 1;
        cur_sz++;
    } else {
        free(visited);
        return LINK_ERROR;
    }

    free(visited);

    if (cur_sz != l_list->size)
        return SIZE_ERROR;
    
    return LLIST_OK;
}