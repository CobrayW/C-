// 函数定义
#include <iostream>
#include <string>
#include "record.h"
#include "llist.h"
#include "MIS.h"
using namespace std;

void run_program()
{
    llist records(get_file_name()); // 使用构造函数初始化对象
    string input_name, input_address, input_phone;
    int input_year, menu_selection;

    display_commands();
    do
    {
        menu_selection = get_menu_selection();
        switch (menu_selection)
        {
        case 1: // 添加新记录
            cout << "--------------------\n";
            cout << "添加一个新记录\n";
            input_name = get_name();
            input_address = get_address();
            input_year = get_birth_year();
            input_phone = get_phone_number();
            records.add_record(input_name, input_address, input_year, input_phone);
            break;
        case 2: // 打印指定姓名的记录
            cout << "--------------------\n";
            cout << "打印指定姓名的记录\n";
            input_name = get_name();
            records.print_record(input_name);
            break;
        case 3: // 修改指定姓名的记录
            cout << "--------------------\n";
            cout << "修改指定姓名的记录\n";
            input_name = get_name();
            input_address = get_address();
            input_year = get_birth_year();
            input_phone = get_phone_number();
            records.modify_record(input_name, input_address, input_year, input_phone);
            break;
        case 4: // 打印所有的记录
            cout << "--------------------\n";
            cout << "打印记录中...\n";
            records.print_all_records();
            break;
        case 5: // 删除指定姓名的记录
            cout << "--------------------\n";
            cout << "删除指定姓名的记录\n";
            input_name = get_name();
            records.delete_record(input_name);
            break;
        case 6: // 反转所有记录的顺序
            cout << "--------------------\n";
            cout << "反转记录顺序中...\n";
            records.reverse_llist();
            break;
        case 7:
            cout << "--------------------\n";
            cout << "重排记录中...\n";
            records.sort_by_year();
            break;
        }
        if (menu_selection != 8)
            display_commands();
    } while (menu_selection != 8);
}

void display_commands()
{
    cout << "选项菜单:\n";
    cout << "\t1: 添加新记录.\n";
    cout << "\t2: 打印指定姓名的记录.\n";
    cout << "\t3: 修改指定姓名的记录.\n";
    cout << "\t4: 打印所有记录.\n";
    cout << "\t5: 删除指定姓名的记录.\n";
    cout << "\t6: 反转所有记录的顺序.\n";
    cout << "\t7: 按年龄由大到小重排所有记录.\n";
    cout << "\t8: 退出程序.\n";
    cout << "请输入选项值: ";
}

int get_menu_selection() // 获取输入的菜单选项值并进行错误处理，返回正确的菜单选项值
{
    int selection; // 存储输入的菜单选项值
    char ch;       // 用于接受下一个输入进行判断
    cin >> selection;

    while ((ch = cin.get()) != '\n' || selection < 1 || selection > 8) // 当输入失败或选项越界时
    {
        if (cin.fail() || ch != '\n')
        {
            cout << "输入应为整数, 请重新输入: ";
            cin.clear(); // 重置输入
            while (cin.get() != '\n')
                ; // 清除错误输入，直至遇到换行符
        }
        else if (selection < 1 || selection > 8)
        {
            cout << "无对应选项, 请重新输入.\n";
            display_commands();
        }
        cin >> selection;
    }
    return selection;
}

string get_name() // 获取人员的姓名并返回
{
    string name;
    cout << "请输入人员的姓名: ";
    getline(cin, name);
    return name;
}

string get_address() // 获取人员的地址并返回
{
    string address, blank_line;
    cout << "请输入人员的地址(以\"$\"标识输入结束):\n";
    getline(cin, address, '$');
    getline(cin, blank_line); // 清除输入流中的$和换行符
    return address;
}

int get_birth_year() // 获取人员的出生年份并返回
{
    int birth_year;
    cout << "请输入人员的出生年份: ";
    cin >> birth_year;

    while (cin.get() != '\n' || cin.fail())
    {
        cout << "输入应为整数, 请重新输入: ";
        cin.clear();
        while (cin.get() != '\n')
            ; // 清除错误输入，直至遇到换行符
        cin >> birth_year;
    }
    return birth_year;
}

string get_phone_number() // 获取人员的电话号码并返回
{
    string phone_number;
    cout << "请输入联系人的电话号码: ";
    getline(cin, phone_number);
    return phone_number;
}

string get_file_name() // 获取输入作为要打开的文件名
{
    string file_name;
    cout << "请输入文件名(文件扩展名\".txt\"将自动添加): ";
    getline(cin, file_name);
    file_name += ".txt";
    return file_name;
}