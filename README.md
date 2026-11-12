# Double linked list - двусвязный список


## Структура двусвязного списка и его элемента
```c++
struct list_element {
    elem_list_t value;  // значение элемента, для свободных и нулевого значение 0
    uint64_t next;      // номер следующего, для свободного элемента = номер следующего свободного элемента (если его нет, то 0), в нулевом элементе хранится позиция начала списка
    int64_t prev;       // номер предыдущего, для свободного элемента = -1, в нулевом элементе хранится позиция конца списка
};

struct linked_list {
    struct list_element* list;  // массив элементов списка

    uint64_t head;              // начало списка, начальное значение 0
    uint64_t tail;              // конец списка, начальное значение 0
    uint64_t capacity;          // вместимость списка (один элемент зарезервирован под нулевой "зацикливающий" элемент), минимальная вместимость MIN_CAPACITY (4)
    uint64_t free;              // номер первого свободного элемента в массиве
    uint64_t size;              // размер текущего списка
};
```

## Функции
- **void InitLList(struct linked_list* l_list, size_t capacity);** – инициализация списка
- **void LListDumpVerify(struct linked_list* l_list, const char* message);** – функция вызова верификации списка с графическим дампом
- **void DestroyLList(struct linked_list* l_list);** – деструктор списка
- **int64_t InsertElemLList(struct linked_list* l_list, size_t pos, elem_list_t val);** - вставка элемента после позиции pos в массиве, для вставки элемента перед началом списка используется нулевой индекс
- **void RemoveElemLList(struct linked_list* l_list, size_t pos);** – удаление элемента на позиции pos в массиве
- **int64_t get_pos_in_list(struct linked_list* l_list, size_t real_pos);** – поиск индекса в массиве по номеру в списке
- **int LListGraphicDump(struct linked_list* l_list, const char* file_name);** – графический дамп с верификацией

### Особенности
1. 

## Компиляция
gcc d_linked_list.c base.c -o llist

### Запуск
.\llist

## Авторские права
© 2026 [yaroslonix-gif](https://github.com/yaroslonix-gif)