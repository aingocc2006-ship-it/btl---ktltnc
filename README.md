#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <map>
#include <windows.h>
#include <commdlg.h>
#include <algorithm>
#include <limits>
#include <cctype>
#include <ctime>
#include <filesystem>
#include <iomanip>

using namespace std;
void SetColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define YELLOW 6
#define WHITE 7

#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

// ========================== SAFE PARSING FUNCTIONS ==========================
int safeStoi(const string& s, int defaultVal = 0) {
    if (s.empty()) return defaultVal;
    try { return stoi(s); }
    catch (...) { return defaultVal; }
}

double safeStod(const string& s, double defaultVal = 0.0) {
    if (s.empty()) return defaultVal;
    try { return stod(s); }
    catch (...) { return defaultVal; }
}

void VeKhungTieuDe(string tieuDe, int mau)
{
    int len = tieuDe.length() + 4;
    SetColor(mau);
    cout << "\n╔";
    for (int i = 0; i < len; i++)
        cout << "═";
    cout << "╗" << endl;
    cout << "║  ";
    SetColor(BRIGHT_WHITE);
    cout << tieuDe;
    SetColor(mau);
    cout << "  ║" << endl;
    cout << "╚";
    for (int i = 0; i < len; i++)
        cout << "═";
    cout << "╝" << endl;
    SetColor(WHITE);
}

//======Tai file======
string taiFile()
{
    OPENFILENAMEW ofn;
    WCHAR szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);

    ofn.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;

    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
    {
        char buffer[260];
        WideCharToMultiByte(CP_UTF8, 0, szFile, -1, buffer, sizeof(buffer), NULL, NULL);
        return string(buffer);
    }
    return "";
}

// ========================== CAU HOI ==========================
struct CauHoi
{
    int id;
    string noiDung;
    string dapAn[4];
    int dapAnDung;
};

//===================== Ham Nhap ==========================
string inputLine(const string& prompt)
{
    string s;
    while (true)
    {
        cout << prompt;
        getline(cin, s);
        if (!s.empty())
            return s;
        cout << "Loi: khong duoc de trong.\n";
    }
}

template <typename T>
T nhapSo(const string& thongBao)
{
    T value;
    while (true)
    {
        cout << thongBao;
        if (cin >> value)
        {
            cin.ignore(1000, '\n');
            return value;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nhap sai, thu lai!\n";
    }
}

time_t parseDateTime(const string& s)
{
    tm tmVal = {};
    int year, month, day, hour, minute;
    if (sscanf(s.c_str(), "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute) == 5)
    {
        tmVal.tm_year = year - 1900;
        tmVal.tm_mon = month - 1;
        tmVal.tm_mday = day;
        tmVal.tm_hour = hour;
        tmVal.tm_min = minute;
        tmVal.tm_sec = 0;
        tmVal.tm_isdst = -1;
        return mktime(&tmVal);
    }
    return 0;
}

string formatDateTime(time_t t)
{
    if (t <= 0)
        return "N/A";
    tm localTime;
    localtime_s(&localTime, &t);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &localTime);
    return string(buffer);
}

string hashPassword(const string& raw)
{
    return raw;
}
// ========================== DE THI ==========================
struct deThi
{
    int id;
    int khoa;
    string tenMon;
    vector<CauHoi> danhSachCauHoi;
    CauHoi ch;
    int thoiGianLamBai;
    int loaiThi;
    int phongThiId;
    time_t batDau;
    time_t ketThuc;
    bool locked;
    vector<string> dsLopDuocThi;
    int soLanOnTap;

    deThi() : id(0), khoa(0), tenMon(""), thoiGianLamBai(0),
        loaiThi(1), phongThiId(0), batDau(0), ketThuc(0),
        locked(false), soLanOnTap(-1) {
    }

    void themCauHoi()
    {
        ch.id = nhapSo<int>("ID: ");
        cout << "Noi dung: ";
        getline(cin, ch.noiDung);
        for (int i = 0; i < 4; i++)
        {
            cout << "Lua chon " << i + 1 << ": ";
            getline(cin, ch.dapAn[i]);
        }
        do
        {
            ch.dapAnDung = nhapSo<int>("Dap an dung (1-4): ");
        } while (ch.dapAnDung < 1 || ch.dapAnDung > 4);
        danhSachCauHoi.push_back(ch);
    }

    void hienThiCauHoi()
    {
        for (auto& ch : danhSachCauHoi)
        {
            cout << ch.id << ". " << ch.noiDung << endl;
        }
    }

    bool isAvailable() const
    {
        if (locked)
            return false;
        if (batDau == 0 || ketThuc == 0)
            return true;
        time_t now = time(0);
        return now >= batDau && now <= ketThuc;
    }

    string getSchedule() const
    {
        return formatDateTime(batDau) + " -> " + formatDateTime(ketThuc);
    }
};

// ========================== CLASS INFO ==========================
struct ClassInfo
{
    int id;
    string className;
    int coVanHocTapId;
    vector<int> danhSachGiangVien;
    int studentCount;
    int teacherId;
    ClassInfo() : id(0), className(""), coVanHocTapId(0), studentCount(0), teacherId(0) {}
};

struct PhongThi
{
    int id;
    string tenPhong;
    int sucChua;
    bool locked;

    PhongThi() : id(0), tenPhong(""), sucChua(0), locked(false) {}
};

// ========================== PERSON ==========================
class Person
{
protected:
    int id;
    string username;
    string password;
    string fullName;

public:
    Person() : id(0), username(""), password(""), fullName("") {}

    Person(int id, const string& username, const string& password, const string& fullName)
        : id(id), username(username), password(password), fullName(fullName)
    {
    }

    virtual ~Person() {}
    virtual string getRole() const = 0;

    virtual bool login(const string& u, const string& p) const
    {
        return u == username && (password == p || password == hashPassword(p));
    }

    int getId() const { return id; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getFullName() const { return fullName; }
    void setId(int newId) { id = newId; }
    void setUsername(const string& newUsername) { username = newUsername; }
    void setPassword(const string& newPassword) { password = newPassword; }
    void setFullName(const string& newFullName) { fullName = newFullName; }
};

