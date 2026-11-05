#include "d_linked_list.h"

int main() {
    START_LOGG;
    LOGG("<pre>\n");
    LOGG("<head><style>table {border-collapse: collapse;background-color: #e6f2ff;font-family: Arial, sans-serif;} th, td {border: 1px solid #b3d7ff;padding: 12px; text-align: center;}th {background-color: #cbdff7; font-weight: bold;}</style></head>\n");
    HARD_VERIFY(_mkdir(DIR_NAME) != -1, CANT_MAKE_DIR);

    struct linked_list l_list = {};
    
    InitLList(&l_list, 10);
    InsertElemLList(&l_list, 0, 10);
    InsertElemLList(&l_list, 0, 11);
    InsertElemLList(&l_list, 1, 20);
    InsertElemLList(&l_list, 2, 30);
    InsertElemLList(&l_list, 1, 15);
    RemoveElemLList(&l_list, 2);
    InsertElemLList(&l_list, 1, 20);
    l_list.list[3].prev = 7;
    InsertElemLList(&l_list, 4, 60);
    InsertElemLList(&l_list, 4, 60);
    InsertElemLList(&l_list, 4, 60);
    InsertElemLList(&l_list, 4, 60);
    InsertElemLList(&l_list, 4, 60);
    InsertElemLList(&l_list, 4, 60);

    STOP_LOGG;
    return 0;
}

void InitLList(struct linked_list* l_list, size_t capacity) {
    HARD_ASSERT(l_list != NULL, NULL_LLIST);

    if (capacity < MIN_CAPACITY)
        capacity = MIN_CAPACITY;
    
    l_list->list = (struct list_element*)calloc(capacity, sizeof(l_list->list[0]));
    HARD_VERIFY(l_list->list != NULL, NULL_ERROR);

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
    HARD_ASSERT(l_list != NULL, NULL_ERROR);

    free(l_list->list);

    l_list->head = 0;
    l_list->tail = 0;
    l_list->capacity = 0;
    l_list->free = 0;
    l_list->size = 0;
}

int64_t GetFreePos(struct linked_list* l_list) {
    HARD_ASSERT(l_list != NULL, NULL_LLIST);
    HARD_ASSERT(l_list->list != NULL, NULL_LIST);

    int64_t free_pos = l_list->free;
    if (free_pos == 0) return 0;
    l_list->free = l_list->list[free_pos].next;

    return free_pos;
}

void InsertElemLList(struct linked_list* l_list, size_t pos, elem_list_t val) {
    LLIST_DUMP_VERIFY(l_list, "BEFORE INSERT\n");

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

    if (pos == 0 && l_list->size > 0) {
        l_list->list[l_list->head].prev = free_pos;
        l_list->list[free_pos].next = l_list->head;
        l_list->head = free_pos;
    }

    if (pos != 0)
        l_list->list[pos].next = free_pos;

    if (next_pos != 0)
        l_list->list[next_pos].prev = free_pos;

    if (l_list->size == 0 || pos == l_list->tail)
        l_list->tail = free_pos;

    if (l_list->size == 0)
        l_list->head = free_pos;

    l_list->size++;
    LLIST_DUMP_VERIFY(l_list, "AFTER INSERT\n");
}

void RemoveElemLList(struct linked_list* l_list, size_t pos) {
    LLIST_DUMP_VERIFY(l_list, "BEFORE REMOVE\n");
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
    LLIST_DUMP_VERIFY(l_list, "AFTER REMOVE\n");
}

int64_t get_pos_in_list(struct linked_list* l_list, size_t real_pos) {
    LLIST_DUMP_VERIFY(l_list, "VIEW Get Free Pos\n");

    if (real_pos >= l_list->size) {
        LOGG("ELEM OUT OF LLIST");
        return -1;
    }
    size_t current_pos = l_list->head;

    for (int i = 0; i < real_pos; i++) {        
        current_pos = l_list->list[current_pos].next;
    }

    return current_pos;
}

void SetDumpNode(FILE* dump_out, const size_t index, const struct list_element* node, const char* color, const char* fillcolor) {
    HARD_ASSERT(dump_out != NULL, NULL_ERROR);
    HARD_ASSERT(node != NULL, NULL_ERROR);
    HARD_ASSERT(color != NULL, NULL_ERROR);
    HARD_ASSERT(fillcolor != NULL, NULL_ERROR);

    fprintf(dump_out, "  NODE%zu[color = \"%s\", fillcolor = \"%s\", label=\" node = %zu | next = %zd | prev = %zd | val = %lld\"];\n", 
            index, color, fillcolor, index, node->next, node->prev, node->value);
}

void LListDumpVerify(struct linked_list* l_list, const char* message) {
    HARD_ASSERT(message != NULL, NULL_ERROR);

    LOGG(message);
    time_t now_time = 0;
    struct tm* time_info = NULL;

    time(&now_time);
    time_info = localtime(&now_time);
    clock_t tik_now = clock();

    char file_name[200] = "";
    strftime(file_name, sizeof(file_name), "dump%d_%m_%Y_%H_%M_%S_", time_info);
    snprintf(file_name + strlen(file_name), 200, "%d", tik_now);
    int have_err = LListGraphicDump(l_list, file_name);
    LOGG("<img src=\"%s\\%s.png\">\n", DIR_NAME, file_name);

    if (have_err == HAVE_ERROR) {
        abort();
    }
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

int LListGraphicDump(struct linked_list* l_list, const char* file_name) {
    if (l_list == NULL) {
        LOGG("NULL_LLIST: LINKED LIST is NULL\n");
        return HAVE_ERROR;
    }

    if (l_list->list == NULL) {
        LOGG("NULL_LIST: LIST is NULL\n");
        return HAVE_ERROR;
    }

    int have_err = 0;

    LOGG("<table border=\"1\">\n");
    LOGG("<tr><td>head</td><td>%llu</td></tr>\n", l_list->head);
    LOGG("<tr><td>tail</td><td>%llu</td></tr>\n", l_list->tail);
    LOGG("<tr><td>size</td><td>%llu</td></tr>\n", l_list->size);
    LOGG("<tr><td>capacity</td><td>%llu</td></tr>\n", l_list->capacity);
    LOGG("<tr><td>free</td><td>%llu</td></tr>\n", l_list->free);
    LOGG("</table>\n");

    if (l_list->size >= l_list->capacity) {
        LOGG("WARNING: size >= l_list->capacity\n");
        have_err = HAVE_ERROR;
    }
    
    char file_dot[200] = "";
    sprintf(file_dot, "%s\\%s.dot", DIR_NAME, file_name);

    FILE* dump_out = fopen(file_dot, "w");
    HARD_VERIFY(dump_out != NULL, CANT_OPEN_FILE);

    fprintf(dump_out, "digraph {\n");
    fprintf(dump_out, "  rankdir = LR;\n  node[shape = \"Mrecord\", style = filled, fontsize = 12];\n");
    fprintf(dump_out, "  graph[splines = ortho, nodesep = 0.5];\n");
    fprintf(dump_out, "  edge[radius = 5];\n");
    
    _LListBasePartVerify(l_list, dump_out, &have_err);

    if (l_list->size > 0) {
        _DumpCheckLListElems(l_list, dump_out, &have_err);
    }

    if (l_list->free != 0) {
        _DumpCheckLListFreeElems(l_list, dump_out, &have_err);
    }

    fprintf(dump_out, "  edge[color = \"#36799D\", weight = 5, dir = forward];\n");
    fprintf(dump_out, "  { rank=same; NODE%zu; FREE; }\n", l_list->free);
    fprintf(dump_out, "  FREE->NODE%zu;\n", l_list->free);

    fprintf(dump_out, "}\n");

    fclose(dump_out);

    char sys_cmd[200] = "";
    sprintf(sys_cmd, "\"C:\\Program Files\\Graphviz\\bin\\dot.exe\" -Tpng %s\\%s.dot -o %s\\%s.png", DIR_NAME, file_name, DIR_NAME, file_name);
    system(sys_cmd);

    return have_err;
}

void _LListBasePartVerify(struct linked_list* l_list, FILE* dump_out, int* have_err) {
    SetDumpNode(dump_out, 0, &(l_list->list[0]), "#000000", "#B0B0B0");
    for (size_t i = 1; i < l_list->capacity; i++) {           
        if (l_list->list[i].prev == -1)
            SetDumpNode(dump_out, i, &(l_list->list[i]), "#404040", "#E0E0E0");
        else
            SetDumpNode(dump_out, i, &(l_list->list[i]), "#008000", "#C0FFC0");
    }

    if (l_list->head >= l_list->capacity) {
        LOGG("HEAD_OUT_OF_LIST\n");
        fprintf(dump_out, "  HEAD[color = \"#800000\", fillcolor = \"#FFC0C0\", label=\"HEAD\"];\n");
        *have_err = HAVE_ERROR;
    } else {
        fprintf(dump_out, "  HEAD[color = \"#000080\", fillcolor = \"#C0C0FF\", label=\"HEAD\"];\n");
    }

    if (l_list->tail >= l_list->capacity) {
        LOGG("HEAD_OUT_OF_LIST\n");
        fprintf(dump_out, "  TAIL[color = \"#800000\", fillcolor = \"#FFC0C0\", label=\"TAIL\"];\n");
        *have_err = HAVE_ERROR;
    } else {
        fprintf(dump_out, "  TAIL[color = \"#808000\", fillcolor = \"#FFFFC0\", label=\"TAIL\"];\n");
    }

    if (l_list->free <= l_list->capacity) {
        fprintf(dump_out, "  FREE[color = \"#36799D\", fillcolor = \"#87CEFA\", label=\"FREE\"];\n");
    } else {
        LOGG("FREE_OUT_OF_LIST\n");
        fprintf(dump_out, "  FREE[color = \"#800000\", fillcolor = \"#FFC0C0\", label=\"FREE\"];\n");
        *have_err = HAVE_ERROR;
    }

    fprintf(dump_out, "  edge[color = \"white\", weight = 1000, dir = both];\n");
    fprintf(dump_out, "  NODE0");
    for (size_t i = 1; i < l_list->capacity; i++) {
        fprintf(dump_out, "->NODE%zu", i);
    }
    fprintf(dump_out, ";\n");
}

void _DumpCheckLListElems(struct linked_list* l_list, FILE* dump_out, int* have_err) {

    fprintf(dump_out, "  edge[color = \"black\", weight = 10];\n");
    size_t current_pos = l_list->head;
    fprintf(dump_out, "  NODE%zu", current_pos);

    int* visited = (int*)calloc(l_list->capacity, sizeof(int));
    if (visited == NULL) {
        LOGG("CAN'T MAKE VISITED IN DumpCheckLListElems\n");
        *have_err = HAVE_ERROR;
        return;
    }

    int i = 0;
    uint64_t cur_sz = 0;

    while (current_pos != 0 && current_pos != l_list->tail) {
        if (current_pos >= l_list->capacity || visited[current_pos] != 0)   {
            LOGG("CYCLE_OR_OUT_OF_LIST: %llu\n", current_pos);
            visited[current_pos] = -1;
            *have_err = HAVE_ERROR;
            break;
        }

        visited[current_pos] = 1;
        cur_sz++;
        
        int64_t next_pos = l_list->list[current_pos].next;
        if (next_pos != 0 && next_pos < l_list->capacity && l_list->list[next_pos].prev != current_pos) {
            visited[next_pos] = -1;
            LOGG("LINK_ERROR\n");
            *have_err = HAVE_ERROR;
            break;
        }

        current_pos = next_pos;
        fprintf(dump_out, "->NODE%zu", current_pos);
    }

    if (current_pos == l_list->tail && l_list->list[current_pos].next == 0) {
        visited[current_pos] = 1;
        cur_sz++;
    } else {
        LOGG("LINK_ERROR_TAIL\n");
        visited[current_pos] = -1;
    }

    if (cur_sz != l_list->size)
        LOGG("SIZE_ERROR\n");

    fprintf(dump_out, ";\n");

    for (size_t i = 1; i < l_list->capacity; i++) {
        if (visited[i] == -1) {
            fprintf(dump_out, "  NODE%zu[color = \"#800000\", fillcolor = \"#FFC0C0\"]", i);
            continue;
        }

        if (visited[i] == 1)
            continue;
        
        if (l_list->list[i].prev != -1) {
            fprintf(dump_out, "  NODE%zu[color = \"#800000\", fillcolor = \"#FFC0C0\"]", i);
            *have_err = HAVE_ERROR;
        }
    }

    free(visited);

    fprintf(dump_out, "  edge[color = \"#000080\", weight = 5, dir = forward];\n");
    fprintf(dump_out, "  { rank=same; NODE%zu; HEAD; }\n", l_list->head);
    fprintf(dump_out, "  HEAD->NODE%zu;\n", l_list->head);

    fprintf(dump_out, "  edge[color = \"#808000\", weight = 5, dir = forward];\n");
    fprintf(dump_out, "  { rank=same; NODE%zu; TAIL; }\n", l_list->tail);
    fprintf(dump_out, "  TAIL->NODE%zu;\n", l_list->tail);
}

void _DumpCheckLListFreeElems(struct linked_list* l_list, FILE* dump_out, int* have_err) {

    fprintf(dump_out, "  edge[color = \"#C0C0FF\", weight = 10];\n");
    size_t current_pos = l_list->free;
    fprintf(dump_out, "  NODE%zu", current_pos);

    int* visited = (int*)calloc(l_list->capacity, sizeof(int));
    if (visited == NULL) {
        LOGG("CAN'T MAKE VISITED IN DumpCheckLListElems\n");
        *have_err = HAVE_ERROR;
        return;
    }

    int i = 0;
    uint64_t cur_sz = 0;

    while (current_pos != 0) {
        if (current_pos >= l_list->capacity || visited[current_pos] != 0)   {
            LOGG("CYCLE_OR_OUT_OF_FREE_LIST: %llu\n", current_pos);
            visited[current_pos] = -1;
            *have_err = HAVE_ERROR;
            break;
        }

        visited[current_pos] = 1;
        cur_sz++;
        
        int64_t next_pos = l_list->list[current_pos].next;
        if ((next_pos != 0 && next_pos < l_list->capacity && l_list->list[next_pos].prev == -1) || (next_pos == 0)) {
            current_pos = next_pos;
            fprintf(dump_out, "->NODE%zu", current_pos);
            continue;
        }

        visited[next_pos] = -1;
        LOGG("LINK_FREE_ERROR\n");
        *have_err = HAVE_ERROR;
        break;        
    }

    if (current_pos == 0 && l_list->list[0].next == 0 && l_list->list[0].prev == 0 && l_list->list[0].value == 0) {
        visited[current_pos] = 1;
        cur_sz++;
    } else {
        LOGG("LINK_ERROR_TAIL\n");
        visited[current_pos] = -1;
    }

    if (cur_sz != (l_list->capacity - l_list->size))
        LOGG("SIZE_ERROR\n");

    fprintf(dump_out, ";\n");

    for (size_t i = 1; i < l_list->capacity; i++) {
        if (visited[i] == -1) {
            fprintf(dump_out, "  NODE%zu[color = \"#800000\", fillcolor = \"#FFC0C0\"]", i);
            continue;
        }

        if (visited[i] == 1)
            continue;
    }

    free(visited);
}