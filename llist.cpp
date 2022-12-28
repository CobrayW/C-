// 链表类llist的函数定义
#include <iostream>
#include <fstream>
#include <string>
#include "record.h"
#include "llist.h"
#include "mis.h"
using namespace std;

llist::llist(string file)
{
    int records_read = 0; // 指示文件是否正常读取
    start = NULL;
    file_name = file;

    records_read = read_file();
    // 根据文件读取情况打印消息
    if (records_read == -1)
    {
        cout << "未找到文件\"" << file_name << "\". 因此, 没有记录被添加.\n";
        cout << "程序将创建文件\"" << file_name << "\"用于存储数据.\n";
    }
    else if (records_read == 0)
    {
        cout << "已找到文件\"" << file_name << "\", 但文件为空. 因此, 没有记录被添加.\n";
    }
    else
    {
        cout << "已找到文件\"" << file_name << "\", 已添加" << records_read << "条数据到数据库.\n";
    }
    cout << "--------------------\n";
}

llist::~llist()
{
    write_file();
    delete_all_records();
}

int llist::read_file()
{
    ifstream read_file;
    read_file.open(file_name.c_str()); // c_str()将string对象转化为C-风格字符串
    record *temp = NULL;               // 存储每次读取数据的临时指针（当前指针）
    record *previous = NULL;           // 存储当前指针的上一个指针
    int record_count = 0;              // 记录数量
    string blank_line;

    if (!read_file.is_open())
    {
        read_file.close();
        return -1;
    }

    do // read_file.eof() = true 时停止
    {
        temp = new record;
        getline(read_file, temp->name);
        getline(read_file, temp->address, '$'); // 截断字符$，读取到$并丢弃$
        read_file >> temp->birth_year;          // cin类对象可以忽略有效输入前的空白字符
        getline(read_file, blank_line);         // 清除换行符
        getline(read_file, temp->phone_number);
        getline(read_file, blank_line); // 清除空行
        ++record_count;
        if (previous == NULL) // 只有首次才执行该条件对应的语句块
        {
            temp->next = NULL;
            start = temp; // start与temp指向同一个存储地址，不能使用delete删除
            previous = start;
        }
        else // 此后执行该语句块
        {
            previous->next = temp;
            temp->next = NULL;
            previous = temp; // 这样在下一轮循环中，previous将指向当前指针(下一轮循环的temp)的上一个指针
        }
    } while (!read_file.eof());
    read_file.close();
    return record_count;
}

void llist::write_file()
{
    ofstream write_file;
    write_file.open(file_name.c_str());
    record *index = start;
    int record_count = 0; // 记录数量

    if (start == NULL)
    {
        cout << "数据库为空, 因此没有记录被写入文件\"" << file_name << "\".\n";
        write_file.close();
    }

    do
    {
        write_file << index->name << endl;
        write_file << index->address << "$\n";
        write_file << index->birth_year << endl;
        if (index->next != NULL)
            write_file << index->phone_number << endl
                       << endl; // 尚未到达链表末尾时打印两行换行符，以避免读取出错
        else
            write_file << index->phone_number;
        index = index->next;
        ++record_count;
    } while (index != NULL);

    cout << "已将" << record_count << "条记录写入文件\"" << file_name << "\".\n";
    write_file.close();
}

// 函数重载2，递归进行链表反转，reverse_llist(start)
static int node_number = 0; // 标识当前的链表结点序号
void llist::reverse_llist(record *pointer)
{
    node_number++;
    if (pointer->next == NULL)
        start = pointer;
    else
    {
        reverse_llist(pointer->next); // 下面语句暂不执行，进入递归
        pointer->next->next = pointer;
        if (--node_number == 1) // 当pointer为反转前的起始结点时
        {
            pointer->next = NULL;
            node_number = 0;
        }
    }
}

void llist::sort_by_year(record *pointer)
{
    int length = 1; // 存储链表长度
    record *temp_start = new record;
    record *temp;
    temp_start->next = pointer; // 添加一个空白的链表起始结点，以便于冒泡排序算法的实现

    while (pointer->next != NULL) // 计算链表长度
    {
        length++;
        pointer = pointer->next;
    }

    for (int i = 1; i < length; i++) // 进行length-1趟冒泡
    {
        int count = 0;     // 记录这一趟冒泡进行的结点比较次数
        bool flag = false; // 标记这一趟冒泡是否进行了结点位置交换操作
        record *p = temp_start;
        while (count < (length - i)) // 每趟冒泡
        {
            if (p->next->birth_year > p->next->next->birth_year)
            {
                flag = true; // 标记这一趟冒泡进行了结点位置交换操作
                // 进行结点位置交换
                temp = p->next;
                p->next = p->next->next;
                temp->next = p->next->next;
                p->next->next = temp;
            }
            count++;
            p = p->next; // 比较下一对结点
        }
        // 这一趟冒泡没有进行结点位置交换操作，说明链表已经有序
        if (flag == false)
            break;
    }
    start = temp_start->next; // 更新链表实际开始结点
    delete temp_start;
}

void llist::delete_all_records()
{
    record *temp;
    record *index = start;

    if (start != NULL)
    {
        while (index != NULL)
        {
            temp = index;
            index = index->next;
            delete temp;
        }
    }
}

void llist::add_record(string input_name, string input_address, int input_birth_year, string input_phone_number)
{
    record *temp = NULL;
    record *index = start;
    record *previous = NULL;
    temp = new record;
    temp->name = input_name;
    temp->address = input_address;
    temp->birth_year = input_birth_year;
    temp->phone_number = input_phone_number;
    while (index != NULL) // 使previous指向当前链表的最后一个结点
    {
        previous = index;
        index = index->next;
    }
    if (previous == NULL) // 当前无记录的情况
    {
        temp->next = start;
        start = temp;
    }
    else // 当前有记录的情况
    {
        previous->next = temp;
        temp->next = index;
    }
    cout << "--------------------\n";
    cout << "记录已添加.\n";
    cout << "--------------------\n";
}

void llist::print_record(string input_name)
{
    record *index = start;
    int records_printed = 0; // 标识共找到了几个记录
    cout << "--------------------\n";
    if (start == NULL)
    {
        cout << "数据库为空, 因此没有姓名为\"" << input_name << "\"的记录.\n";
        cout << "--------------------\n";
    }
    while (index != NULL) // 在链表中逐个比对
    {
        if (input_name == index->name)
        {
            ++records_printed;
            cout << "====================\n";
            cout << "已找到记录 #" << records_printed << " :\n";
            cout << "姓名: " << index->name << "\n";
            cout << "地址: " << index->address << '\n';
            cout << "出生年份: " << index->birth_year << '\n';
            cout << "电话号码: " << index->phone_number << '\n';
        }
        index = index->next;
    }

    if (records_printed == 0)
    {
        cout << "未发现姓名为\"" << input_name << "\"的记录.\n";
    }
    else
    {
        cout << "--------------------\n";
        cout << "共打印了" << records_printed << "条姓名为\"" << input_name << "\"的记录.\n";
    }
    cout << "--------------------\n";
}

void llist::modify_record(string input_name, string input_address, int input_year, string input_phone_number)
{
    record *index = start;
    int records_modified = 0;
    cout << "--------------------\n";
    if (start == NULL)
    {
        cout << "数据库为空, 因此没有姓名为\"" << input_name << "\"的记录.\n";
        cout << "--------------------\n";
    }
    while (index != NULL) // 在链表中逐个比对并修改
    {
        if (input_name == index->name)
        {
            index->address = input_address;
            index->birth_year = input_year;
            index->phone_number = input_phone_number;
            ++records_modified;
        }
        index = index->next;
    }
    if (records_modified == 0)
    {
        cout << "未发现姓名为\"" << input_name << "\"的记录.\n";
    }
    else
    {
        cout << "共修改了" << records_modified << "条姓名为\"" << input_name << "\"的记录.\n";
    }
    cout << "--------------------\n";
}

void llist::print_all_records()
{
    record *index = start;
    int record_count = 0;
    cout << "--------------------\n";
    if (start == NULL)
    {
        cout << "数据库为空, 因此没有记录被打印.\n";
        cout << "--------------------\n";
        return;
    }
    while (index != NULL)
    {
        record_count++;
        cout << "====================\n";
        cout << "记录 #" << record_count << '\n';
        cout << "姓名: " << index->name << '\n';
        cout << "地址: " << index->address << '\n';
        cout << "出生年份: " << index->birth_year << '\n';
        cout << "电话号码: " << index->phone_number << '\n';
        index = index->next;
    }
    cout << "--------------------\n";
    cout << "共打印了" << record_count << "条记录.\n";
    cout << "--------------------\n";
    return;
}

void llist::delete_record(string input_name)
{
    record *temp = NULL;
    record *index = start;
    record *previous = NULL; // 要保证previous始终是index的上一个结点
    int records_deleted = 0;
    cout << "--------------------\n";
    if (start == NULL)
    {
        cout << "数据库为空, 因此无姓名为\"" << input_name << "\"的记录.\n";
        cout << "--------------------\n";
    }
    while (index != NULL)
    {
        if (index->name == input_name)
        {
            temp = index;
            if (index == start) // 如果是起始结点
            {
                index = index->next;
                start = index; // 重设起点
            }
            else // 重设前后链表结点
            {
                index = index->next;
                previous->next = index;
            }
            delete temp;
            ++records_deleted;
        }
        else // 使previous始终是index的上一个结点
        {
            previous = index;
            index = index->next;
        }
    }
    if (records_deleted == 0)
    {
        cout << "未发现姓名为\"" << input_name << "\"的记录.\n";
    }
    else
    {
        cout << "共删除了" << records_deleted << "条姓名为\"" << input_name << "\"的记录.\n";
    }
    cout << "--------------------\n";
}

void llist::reverse_llist() // 函数重载1，函数作用是输出文字说明
{
    cout << "--------------------\n";
    if (start == NULL)
    {
        cout << "数据库为空, 因此没有记录被反转.\n";
    }
    else if (start->next == NULL)
    {
        cout << "数据库中仅有一个数据, 无效反转.\n";
    }
    else
    {
        reverse_llist(start);
        cout << "已反转所有记录.\n";
    }
    cout << "--------------------\n";
    return;
}

void llist::sort_by_year()
{
    cout << "--------------------\n";
    if (start == NULL)
    {
        cout << "数据库为空, 无法重排顺序.\n";
    }
    else if (start->next == NULL)
    {
        cout << "数据库中仅有一个数据, 无法重排顺序.\n";
    }
    else
    {
        sort_by_year(start);
        cout << "已按年龄由大到小重排所有记录.\n";
    }
    cout << "--------------------\n";
    return;
}