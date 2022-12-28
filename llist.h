// llist链表类的头文件
#ifndef LLIST_H
#define LLIST_H
#include <string>
#include "record.h"
using namespace std;
class llist // 链表类
{
private:
    record *start;    // 链表起始结点的指针，默认为NULL
    string file_name; // 用于读取数据并存储数据的文件
    int read_file();
    void write_file();
    void reverse_llist(record *);
    void delete_all_records();
    void sort_by_year(record *);

public:
    llist(string);
    ~llist();
    void add_record(string, string, int, string);
    void print_record(string);
    void modify_record(string, string, int, string);
    void print_all_records();
    void delete_record(string);
    void reverse_llist();
    void sort_by_year();
};
#endif