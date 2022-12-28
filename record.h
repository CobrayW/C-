// 链表结点
#ifndef RECORD_H
#define RECORD_H
#include <string>
using namespace std;
struct record // 链表结点
{
    string name;
    string address;
    int birth_year;
    string phone_number;
    record *next; // 指向下一个链表结点
};
#endif