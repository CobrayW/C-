#ifndef ADDRESS_H_
#define ADDRESS_H_
#include <string>
using namespace std;
void run_program();        // 运行整个程序
void display_commands();   // 显示菜单
int get_menu_selection();  // 获取输入的菜单选项值并进行错误处理，返回正确的菜单选项值
string get_name();         // 获取人员的姓名并返回
string get_address();      // 获取人员的地址并返回
int get_birth_year();      // 获取人员的出生年份并返回
string get_phone_number(); // 获取人员的电话号码并返回
string get_file_name();    // 获取输入作为要打开的文件名
#endif