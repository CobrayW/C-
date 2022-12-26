#include <string>
#include <iostream>
#include <fstream>
using namespace std;
struct record // 链表节点
{
    string name;
    string address;
    int birth_year;
    string phone_number;
    record *next; // 指向下一个链表节点
};

class llist // 链表类
{
private:
    record *start;    // 链表起始节点的指针，默认为NULL
    string file_name; // 用于读取数据并存储数据的文件，默认为"save.txt"
    int read_file();
    void write_file();
    void reverse_llist(record *);
    void delete_all_records();

public:
    llist(string file = "save");
    ~llist();
    int add_record(string, string, int, string);
    int print_record(string);
    int modify_record(string, string, string);
    void print_all_records();
    int delete_record(string);
    void reverse_llist();
};
void run_program();        // 运行整个程序
void display_commands();   // 显示菜单
int get_menu_selection();  // 获取输入的菜单选项值并进行错误处理，返回正确的菜单选项值
string get_name();         // 获取人员的姓名并返回
string get_address();      // 获取人员的地址并返回
int get_birth_year();      // 获取人员的出生年份并返回
string get_phone_number(); // 获取人员的电话号码并返回
string get_file_name();    // 获取输入作为要打开的文件名

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
        getline(read_file, temp->address, '$'); // 截断字符$
        read_file >> temp->birth_year;
        getline(read_file, blank_line);
        getline(read_file, temp->phone_number);
        getline(read_file, blank_line);
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
        cout << "数据库为空, 因此没有记录被写入到文件\"" << file_name << "\"中.\n";
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

    cout << "已将" << record_count << "条记录写入到文件\"" << file_name << "\"中.\n";
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
        (pointer->next)->next = pointer;
        if (--node_number == 1) // 当pointer为反转前的起始节点时
            pointer->next = NULL;
    }
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

int llist::add_record(string input_name, string input_address, int input_birth_year, string input_phone_number)
{
    record *temp = NULL;
    record *index = start;
    record *previous = NULL;
    temp = new record;
    temp->name = input_name;
    temp->address = input_address;
    temp->birth_year = input_birth_year;
    temp->phone_number = input_phone_number;
    while (index != NULL) // 使previous指向当前链表的最后一个节点
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
    return 1;
}

int llist::print_record(string input_name)
{
    record *index = start;
    int records_printed = 0; // 标识共找到了几个记录
    cout << "--------------------\n";
    if (start == NULL)
    {
        cout << "数据库为空, 因此没有姓名为\"" << input_name << "\"的记录.\n";
        cout << "--------------------\n";
        return -1;
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
        cout << "共打印了" << records_printed << "条姓名为\"" << input_name << "\"的记录.\n";
    }
    cout << "--------------------\n";
    return 1;
}

int llist::modify_record(string input_name, string input_address, string input_phone_number)
{
    record *index = start;
    int records_modified = 0;
    cout << "--------------------\n";
    if (start == NULL)
    {
        cout << "数据库为空, 因此没有姓名为\"" << input_name << "\"的记录.\n";
        cout << "--------------------\n";
        return -1;
    }
    while (index != NULL) // 在链表中逐个比对并修改
    {
        if (input_name == index->name)
        {
            index->address = input_address;
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
    return 1;
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
    cout << "共打印了" << record_count << "条记录.\n";
    cout << "--------------------\n";
    return;
}

int llist::delete_record(string input_name)
{
    struct record *temp = NULL;
    struct record *index = start;
    struct record *previous = NULL; // 要保证previous始终是index的上一个节点
    int records_deleted = 0;
    cout << "--------------------\n";
    if (start == NULL)
    {
        cout << "数据库为空, 因此无姓名为\"" << input_name << "\"的记录.\n";
        cout << "--------------------\n";
        return -1;
    }
    while (index != NULL)
    {
        if (index->name == input_name)
        {
            temp = index;
            if (index == start) // 如果是起始节点
            {
                index = index->next;
                start = index;
            }
            else // 重设前后链表节点
            {
                index = index->next;
                previous->next = index;
            }
            delete temp;
            ++records_deleted;
        }
        else // 使previous始终是index的上一个节点
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
    return 1;
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

int main()
{
    using namespace std;
    cout << "********************\n";
    cout << "欢迎使用人员信息管理系统!\n";
    cout << "********************\n";
    run_program();
    cout << "********************\n";
    cout << "程序已终止\n";
    cout << "********************\n";
    return 0;
}

void run_program()
{
    llist records(get_file_name()); // 使用构造函数初始化对象
    string input_name, input_address, input_phone, blank_line;
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
            getline(cin, blank_line);
            input_phone = get_phone_number();
            records.modify_record(input_name, input_address, input_phone);
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
        }
        if (menu_selection != 7)
            display_commands();
    } while (menu_selection != 7);
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
    cout << "\t7: 退出程序.\n";
    cout << "请输入选项值: ";
}

int get_menu_selection() // 获取输入的菜单选项值并进行错误处理，返回正确的菜单选项值
{
    int selection; // 存储输入的菜单选项值
    char ch;       // 用于接受下一个输入进行判断
    cin >> selection;

    while ((ch = cin.get()) != '\n' || selection < 1 || selection > 7) // 当输入失败或选项越界时
    {
        if (cin.fail() || ch != '\n')
        {
            cout << "输入应为整数, 请重新输入: ";
            cin.clear(); // 重置输入
            while (cin.get() != '\n')
                ; // 清除错误输入，直至遇到换行符
        }
        else if (selection < 1 || selection > 7)
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
    string address;
    cout << "请输入人员的地址(以\"$\"标识输入结束):\n";
    getline(cin, address, '$');
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