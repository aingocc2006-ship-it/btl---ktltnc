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
#include <regex>
#include <cmath>
#include <chrono>
#include <thread>
#include <atomic>
#include <random>
#include <chrono>
#include <set>
#include <unordered_set>
#include <functional>

using namespace std;

static atomic<bool> examTimerRunning(false);
static atomic<int> examTimeRemaining(0);
static COORD examTimerCoord = {0, 0};

void SetColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void printTimerLine(int seconds)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(hConsole, &info))
        return;

    COORD currentPos = info.dwCursorPosition;
    examTimerCoord.X = 0;
    SetConsoleCursorPosition(hConsole, examTimerCoord);
    int remaining = max(0, seconds);
    int hours = remaining / 3600;
    int minutes = (remaining % 3600) / 60;
    int secs = remaining % 60;
    SetColor(12);
    cout << "Thoi gian con lai: " << setfill('0') << setw(2) << hours << ":"
         << setw(2) << minutes << ":" << setw(2) << secs << "   ";
    SetColor(7);
    cout.flush();
    SetConsoleCursorPosition(hConsole, currentPos);
}

void startExamTimer(int seconds)
{
    examTimerRunning = true;
    examTimeRemaining = seconds;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(hConsole, &info))
    {
        examTimerCoord = {0, info.dwCursorPosition.Y};
    }
    printTimerLine(seconds);
    cout << "\n";
    thread([]()
    {
        while (examTimerRunning && examTimeRemaining > 0)
        {
            this_thread::sleep_for(chrono::seconds(1));
            examTimeRemaining--;
            if (!examTimerRunning) break;
            printTimerLine(examTimeRemaining.load());
        }
        if (examTimerRunning)
        {
            printTimerLine(0);
            cout << "\n";
        }
    }).detach();
}

void stopExamTimer()
{
    examTimerRunning = false;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(hConsole, &info))
    {
        COORD currentPos = info.dwCursorPosition;
        SetConsoleCursorPosition(hConsole, examTimerCoord);
        cout << string(40, ' ');
        cout.flush();
        SetConsoleCursorPosition(hConsole, currentPos);
    }
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

string trimString(const string& s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool beginsWith(const string& s, const string& prefix)
{
    return s.rfind(prefix, 0) == 0;
}

string toUpperCopy(const string& s)
{
    string result = s;
    for (char& c : result)
        c = toupper((unsigned char)c);
    return result;
}

string extractFieldValue(const string& line, const string& prefix)
{
    if (!beginsWith(line, prefix))
        return "";
    return trimString(line.substr(prefix.size()));
}

bool parseYesNo(const string& s)
{
    string value = toUpperCopy(trimString(s));
    return value == "CO" || value == "YES" || value == "TRUE" || value == "1" || value == "OPEN";
}

int parseAnswerLetter(const string& s)
{
    string value = trimString(s);
    if (value.empty())
        return 0;
    char c = toupper((unsigned char)value[0]);
    if (c >= 'A' && c <= 'D')
        return c - 'A' + 1;
    return safeStoi(value);
}

int extractNumericSuffix(const string& s)
{
    string digits;
    for (char c : s)
    {
        if (isdigit((unsigned char)c))
            digits.push_back(c);
    }
    if (digits.empty())
        return 0;
    return safeStoi(digits);
}

vector<string> parseListSection(const vector<string>& lines, size_t startIndex)
{
    vector<string> values;
    for (size_t i = startIndex + 1; i < lines.size(); ++i)
    {
        string trimmed = trimString(lines[i]);
        if (trimmed.empty())
            break;
        if (trimmed.size() >= 1 && trimmed[0] == '-')
        {
            string item = trimString(trimmed.substr(1));
            if (!item.empty())
                values.push_back(item);
        }
        else
            break;
    }
    return values;
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

atomic<bool> timerRunning(false);

void hienThiDongHo(time_t start, int thoiGian)
{
    while (timerRunning)
    {
        int remaining = thoiGian - (int)difftime(time(0), start);

        int hours = remaining / 3600;
        int minutes = (remaining % 3600) / 60;
        int secs = remaining % 60;

        // Lưu vị trí con trỏ hiện tại
        cout << "\033[s";

        // Đưa con trỏ lên đầu màn hình
        cout << "\033[H";

        SetColor(LIGHT_RED);

        cout << "Thoi gian con lai: "
             << setfill('0') << setw(2) << hours << ":"
             << setw(2) << minutes << ":"
             << setw(2) << secs << "     ";

        SetColor(WHITE);

        // Khôi phục vị trí cũ
        cout << "\033[u";

        cout.flush();

        this_thread::sleep_for(chrono::seconds(1));
    }
}

string diemMonToString(const vector<pair<string, double>>& diemMon)
{
    stringstream ss;
    for (size_t i = 0; i < diemMon.size(); i++)
    {
        ss << diemMon[i].first << ":" << fixed << setprecision(2) << diemMon[i].second;
        if (i + 1 < diemMon.size()) ss << ",";
    }
    return ss.str();
}

vector<pair<string, double>> parseDiemMonString(const string& s)
{
    vector<pair<string, double>> result;
    if (s.empty()) return result;

    stringstream ss(s);
    string item;
    while (getline(ss, item, ','))
    {
        size_t pos = item.find(':');
        if (pos == string::npos) continue;
        string mon = item.substr(0, pos);
        string diemStr = item.substr(pos + 1);
        double diem = safeStod(diemStr, 0.0);
        if (!mon.empty()) result.emplace_back(mon, diem);
    }
    return result;
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

// ========================== HAM TIEN ICH ==========================
// Shuffle câu hỏi
vector<CauHoi> shuffleCauHoi(const vector<CauHoi>& danhSach) {
    vector<CauHoi> result = danhSach;
    unsigned seed = (unsigned)chrono::system_clock::now()
                    .time_since_epoch()
                    .count();
    shuffle(result.begin(), result.end(),
            default_random_engine(seed));
    return result;
}

// Shuffle đáp án của một câu hỏi


// Đếm ngược thời gian
void countdownTimer(int seconds) {
    while (seconds > 0) {
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        int secs = seconds % 60;
        
        cout << "\r";
        SetColor(LIGHT_RED);
        cout << "Thoi gian con lai: " << setfill('0') 
             << setw(2) << hours << ":" 
             << setw(2) << minutes << ":" 
             << setw(2) << secs << " ";
        SetColor(WHITE);
        cout.flush();
        
        Sleep(1000);
        seconds--;
    }
    SetColor(LIGHT_RED);
    cout << "\n*** HET GIO LAM BAI ***\n";
    SetColor(WHITE);
}

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
    if (sscanf(s.c_str(), "%d/%d/%d %d:%d", &day, &month, &year, &hour, &minute) == 5)
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
        return "";
    tm localTime;
    localtime_s(&localTime, &t);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", &localTime);
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
    string tenBaiThi;           // Tên bài thi
    vector<CauHoi> danhSachCauHoi;
    CauHoi ch;
    int thoiGianLamBai;
    int loaiThi;
    int phongThiId;
    time_t batDau;
    time_t ketThuc;
    bool locked;
    bool randomDe;              // Ngẫu nhiên câu hỏi
    bool shuffleAns;            // Ngẫu nhiên đáp án
    vector<string> dsLopDuocThi;
    int soLanOnTap;

    deThi() : id(0), khoa(0), tenMon(""), tenBaiThi(""), thoiGianLamBai(0),
        loaiThi(1), phongThiId(0), batDau(0), ketThuc(0),
        locked(false), randomDe(false), shuffleAns(false), soLanOnTap(-1) {
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
        string start = (batDau <= 0 ? string("Chua co") : formatDateTime(batDau));
        string end = (ketThuc <= 0 ? string("Chua co") : formatDateTime(ketThuc));
        return start + " -> " + end;
    }
};

// ========================== KET QUA ==========================
struct KetQua
{
    int idSV;
    string tenSV;
    string mon;
    double diem;
    int thoiGian;      // thời gian thực tế (s)
    int maDe;
    int loaiThi;
    time_t startTime;  // thời gian bắt đầu làm bài (server-side)
    time_t submitTime; // thời gian nộp bài
    string receiptId;  // id nộp bài (unique)
    string loginIP;    // IP đăng nhập
    int soLanLam;      // Số lần đã làm
    string getMon() const { return mon; }
};

// ========================== CLASS INFO ==========================
struct ClassInfo
{
    string id;
    string className;

    int coVanHocTapId;

    vector<int> danhSachGiangVien;
    vector<int> students;

    int studentCount;
    ClassInfo() : id(""), className(""), coVanHocTapId(0), studentCount(0) {}
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
// ========================== ADMIN ==========================
class Admin : public Person
{
public:
    Admin() : Person(1, "admin", "admin", "System Admin") {}

    Admin(int id, string u, string p, string ten) : Person(id, u, p, ten) {}

    string getRole() const override
    {
        return "Admin";
    }

    void hienThiThongTin() const
    {
        cout << "[ADMIN] " << fullName << endl;
    }

    string chuyenThanhChuoiFile() const
    {
        return to_string(id) + "|" + username + "|" + password + "|" + fullName;
    }
};
// ========================== QUAN LY THI ==========================
class QuanLyThi
{
private:
    string tenMon;
    deThi de;
    vector<KetQua> danhSachKetQua;
    vector<deThi> maDe;

public:
    void saveDeThi() const;
    void loadDeThi();
    vector<deThi>& getMaDe() { return maDe; }
    const vector<deThi>& getMaDe() const { return maDe; }
    QuanLyThi()
    {
        maDe = {};
        tenMon = "Chua dat ten";
    }

    void thietLapDeThi(int teacherId, string monGiangVien, const vector<ClassInfo>& classes, const vector<PhongThi>& examRooms)
    {
        (void)teacherId;
        (void)examRooms;
        do
        {
            de.loaiThi = nhapSo<int>("Loai bai thi (1. Chinh thuc | 2. On tap): ");
        } while (de.loaiThi != 1 && de.loaiThi != 2);

        de.tenMon = monGiangVien;
        
        // Thêm tên bài thi và kiểm tra trùng tên
        while (true)
        {
            de.tenBaiThi = inputLine("Nhap ten bai thi: ");
            bool exists = false;
            string newNameUpper = toUpperCopy(trimString(de.tenBaiThi));
            for (const auto& d : maDe)
            {
                if (toUpperCopy(trimString(d.tenBaiThi)) == newNameUpper)
                {
                    exists = true;
                    break;
                }
            }
            if (!exists)
                break;
            cout << "Loi: ten bai thi da ton tai. Vui long nhap lai.\n";
        }
        
        int nextId = 1;
        for (const auto& d : maDe)
            nextId = max(nextId, d.id + 1);
        de.id = nextId;

        de.thoiGianLamBai = nhapSo<int>("Thoi gian lam bai (phut): ");
        
        // Tùy chọn random đề
        int chooseRandom = nhapSo<int>("Random cau hoi? (1. Co | 0. Khong): ");
        de.randomDe = (chooseRandom == 1);
        

        de.phongThiId = 0;

        if (de.loaiThi == 2)
        {
            de.batDau = 0;
            de.ketThuc = 0;
            de.soLanOnTap = -1; // Default: unlimited practice attempts
        }
        else
        {
            de.soLanOnTap = 1; // Default: official exams can only be taken once
            do
            {
                string batDau = inputLine("Nhap ngay gio bat dau (dd/MM/yyyy HH:mm): ");
                de.batDau = parseDateTime(batDau);
                if (de.batDau == 0)
                    cout << "Dinh dang khong hop le. Thu lai!\n";
            } while (de.batDau == 0);

            do
            {
                string ketThuc = inputLine("Nhap ngay gio ket thuc (dd/MM/yyyy HH:mm): ");
                de.ketThuc = parseDateTime(ketThuc);
                if (de.ketThuc == 0 || de.ketThuc <= de.batDau)
                {
                    cout << "Khong hop le hoac ket thuc truoc bat dau. Thu lai!\n";
                    de.ketThuc = 0;
                }
            } while (de.ketThuc == 0);
        }

        de.dsLopDuocThi.clear();
        if (classes.empty())
        {
            SetColor(YELLOW);
            cout << "Chua co lop nao duoc dang ky. De thi se mo cho toan truong (ALL).\n";
            SetColor(WHITE);
        }
        else
        {
            cout << "\n===== DANH SACH LOP HIEN CO =====\n";
            for (const auto& c : classes)
            {
                cout << "ID " << c.id << " | " << c.className << "\n";
            }
            cout << "\nNhap ID cac lop duoc thi, cach nhau boi dau ',' (0 = tat ca lop). Ban co the nhap ID so hoac Ten lop: ";
            string line = inputLine("");
            stringstream ss(line);
            string token;
            vector<string> selectedTokens;
            bool allClasses = false;

            while (getline(ss, token, ','))
            {
                token = trimString(token);
                if (token.empty()) continue;
                if (token == "0")
                {
                    allClasses = true;
                    break;
                }
                selectedTokens.push_back(token);
            }

            if (allClasses)
            {
                SetColor(LIGHT_GREEN);
                cout << "De thi se mo cho tat ca lop.\n";
                SetColor(WHITE);
            }
            else
            {
                for (const auto& c : classes)
                {
                    for (const auto& sel : selectedTokens)
                    {
                        if (sel == c.id || sel == c.className)
                        {
                            de.dsLopDuocThi.push_back(c.className);
                            break;
                        }
                        int selNum = safeStoi(sel, INT_MIN);
                        if (selNum != INT_MIN && to_string(selNum) == c.id)
                        {
                            de.dsLopDuocThi.push_back(c.className);
                            break;
                        }
                    }
                }

                // Remove duplicates and normalize list
                sort(de.dsLopDuocThi.begin(), de.dsLopDuocThi.end());
                de.dsLopDuocThi.erase(unique(de.dsLopDuocThi.begin(), de.dsLopDuocThi.end()), de.dsLopDuocThi.end());

                if (de.dsLopDuocThi.empty())
                {
                    SetColor(YELLOW);
                    cout << "Khong co lop hop le duoc chon. De thi se mo cho toan truong (ALL).\n";
                    SetColor(WHITE);
                }
                else
                {
                    SetColor(GREEN);
                    cout << "De thi da duoc mo cho cac lop sau:\n";
                    for (const auto& lop : de.dsLopDuocThi)
                    {
                        cout << " - " << lop << "\n";
                    }
                    SetColor(WHITE);
                }
            }
        }

        de.locked = false;
        int m = nhapSo<int>("Nhap so cau hoi cua de: ");
        de.danhSachCauHoi.clear();
        for (int i = 0; i < m; i++)
        {
            SetColor(LIGHT_YELLOW);
            cout << "\n--- Nhap cau hoi thu " << i + 1 << " ---\n";
            SetColor(WHITE);
            de.themCauHoi();
        }

        maDe.push_back(de);
        saveDeThi();
        SetColor(LIGHT_GREEN);
        cout << "Tao de thi thanh cong!\n";
        SetColor(WHITE);
    }

    void khoaMoDeThi()
    {
        string tenBaiThi = inputLine("Nhap ten bai thi can thao tac: ");
        for (auto& d : maDe)
        {
            if (d.tenBaiThi == tenBaiThi)
            {
                cout << "1. Khoa de thi\n";
                cout << "2. Mo de thi va nhap lich thi\n";
                int action = nhapSo<int>("Chon: ");
                if (action == 1)
                {
                    d.locked = true;
                    cout << "De thi " << d.tenBaiThi << " da duoc khoa.\n";
                }
                else if (action == 2)
                {
                    string batDau;
                    string ketThuc;
                    time_t startTime = 0;
                    time_t endTime = 0;
                    do
                    {
                        batDau = inputLine("Nhap ngay gio bat dau (dd/MM/yyyy HH:mm): ");
                        startTime = parseDateTime(batDau);
                        if (startTime == 0)
                        {
                            cout << "Ngay gio khong hop le. Thu lai!\n";
                            continue;
                        }
                        ketThuc = inputLine("Nhap ngay gio ket thuc (dd/MM/yyyy HH:mm): ");
                        endTime = parseDateTime(ketThuc);
                        if (endTime == 0 || endTime <= startTime)
                        {
                            cout << "Khong hop le hoac ket thuc truoc bat dau. Thu lai!\n";
                            continue;
                        }
                        break;
                    } while (true);
                    d.batDau = startTime;
                    d.ketThuc = endTime;
                    d.locked = false;
                    cout << "De thi " << d.tenBaiThi << " da duoc mo va cap nhat lich thi.\n";
                }
                else
                {
                    cout << "Lua chon khong hop le.\n";
                    return;
                }
                saveDeThi();
                return;
            }
        }
        cout << "Khong tim thay de thi!\n";
    }

    void xoaDe()
    {
        string tenBaiThi = inputLine("Nhap ten bai thi can xoa: ");
        for (size_t i = 0; i < maDe.size(); i++)
        {
            if (maDe[i].tenBaiThi == tenBaiThi)
            {
                maDe.erase(maDe.begin() + i);
                saveDeThi();
                cout << "Xoa de thi '" << tenBaiThi << "' thanh cong!\n";
                return;
            }
        }
        cout << "Khong tim thay de thi voi ten nay!\n";
    }

    void xemLichThi()
    {
        if (maDe.empty())
        {
            cout << "Chua co de thi de hien thi lich.\n";
            return;
        }
        SetColor(CYAN);
        cout << "\n===== LICH THI =====\n";
        SetColor(WHITE);
        time_t now = time(0);
        for (const auto& d : maDe)
        {
            string status;
            if (d.locked)
            {
                status = "Khoa";
            }
            else if (d.batDau != 0 && now < d.batDau)
            {
                status = "Chua den gio";
            }
            else if (d.ketThuc != 0 && now > d.ketThuc)
            {
                status = "Da ket thuc";
            }
            else
            {
                status = "Dang mo";
            }

            string lopInfo;
            if (d.dsLopDuocThi.empty())
                lopInfo = "ALL";
            else
            {
                for (size_t i = 0; i < d.dsLopDuocThi.size(); i++)
                {
                    lopInfo += d.dsLopDuocThi[i];
                    if (i + 1 < d.dsLopDuocThi.size())
                        lopInfo += ", ";
                }
            }

            cout << "Ten bai thi: " << d.tenBaiThi
                << " | Mon: " << d.tenMon;
            if (d.phongThiId != 0)
            {
                cout << " | Phong: " << d.phongThiId;
            }
            cout << " | Bat dau: " << (d.batDau == 0 ? string("Chua co") : formatDateTime(d.batDau))
                << " | Ket thuc: " << (d.ketThuc == 0 ? string("Chua co") : formatDateTime(d.ketThuc))
                << " | Lop: " << lopInfo
                << " | Trang thai: " << status
                << "\n";
        }
    }

    void clearRoomAssignments(int roomId)
    {
        for (auto& d : maDe)
        {
            if (d.phongThiId == roomId)
            {
                d.phongThiId = 0;
            }
        }
    }

    deThi* timDeThi(int id)
    {
        for (auto& d : maDe)
        {
            if (d.id == id)
                return &d;
        }
        return nullptr;
    }

    const deThi* timDeThi(int id) const
    {
        for (const auto& d : maDe)
        {
            if (d.id == id)
                return &d;
        }
        return nullptr;
    }

    void suaCauHoi()
    {
        string tenBaiThi = inputLine("Nhap ten bai thi can sua: ");
        for (auto& d : maDe)
        {
            if (d.tenBaiThi == tenBaiThi)
            {
                d.hienThiCauHoi();
                if (d.danhSachCauHoi.empty())
                {
                    SetColor(RED);
                    cout << "De chua co cau hoi!\n";
                    SetColor(WHITE);
                    return;
                }
                int idCH = nhapSo<int>("Nhap ID cau hoi can sua: ");
                for (auto& ch : d.danhSachCauHoi)
                {
                    if (ch.id == idCH)
                    {
                        cout << "Noi dung moi: ";
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

                        SetColor(LIGHT_GREEN);
                        cout << "Sua cau hoi thanh cong!\n";
                        SetColor(YELLOW);
                        return;
                    }
                }
                SetColor(LIGHT_RED);
                cout << "Khong tim thay cau hoi!\n";
                SetColor(RED);
                return;
            }
        }
        SetColor(LIGHT_RED);
        cout << "Khong tim thay de thi!\n";
        SetColor(WHITE);
    }

    void xoaCauHoi()
    {
        string tenBaiThi = inputLine("Nhap ten bai thi can xoa cau hoi: ");
        for (auto& d : maDe)
        {
            if (d.tenBaiThi == tenBaiThi)
            {
                d.hienThiCauHoi();
                int idCH = nhapSo<int>("Nhap ID cau hoi can xoa: ");
                auto it = remove_if(d.danhSachCauHoi.begin(), d.danhSachCauHoi.end(), [idCH](const CauHoi& c)
                    { return c.id == idCH; });
                if (it != d.danhSachCauHoi.end())
                {
                    d.danhSachCauHoi.erase(it, d.danhSachCauHoi.end());
                    SetColor(GREEN);
                    cout << "Xoa cau hoi thanh cong!\n";
                    SetColor(WHITE);
                }
                else
                {
                    SetColor(RED);
                    cout << "Khong tim thay cau hoi!\n";
                    SetColor(WHITE);
                }
                return;
            }
        }

        SetColor(RED);
        cout << "Khong tim thay de thi!\n";
        SetColor(WHITE);
    }

    void menu(int teacherId, string monGiangVien, const vector<ClassInfo>& classes, const vector<PhongThi>& examRooms)
    {
        int ch;
        do
        {
            VeKhungTieuDe("QUAN LY DE THI", YELLOW);
            cout << "1. Tao de thi moi\n2. Sua cau hoi trong de\n3. Xoa cau hoi trong de\n4. Xem danh sach de thi\n5. Xoa de thi\n6. Khoa/Mo de thi\n7. Xem lich thi\n0. Quay lai\n";
            ch = nhapSo<int>("Chon: ");
            switch (ch)
            {
            case 1:
                thietLapDeThi(teacherId, monGiangVien, classes, examRooms);
                break;
            case 2:
                suaCauHoi();
                break;
            case 3:
                xoaCauHoi();
                break;
            case 4:
                for (const auto& d : maDe)
                {
                    string loai = (d.loaiThi == 1) ? "[Chinh thuc]" : "[On tap]";
                    string randomStr = d.randomDe ? " | Tro cau: Co" : " | Tro cau: Khong";
                    string shuffleStr = d.shuffleAns ? " | Xao DA: Co" : " | Xao DA: Khong";
                    cout << "Ten bai thi: " << d.tenBaiThi
                        << " | Mon: " << d.tenMon
                        << " " << loai
                        << randomStr
                        << shuffleStr
                        << " | Trang thai: " << (d.locked ? "Khoa" : "Mo")
                        << endl;
                }
                break;
            case 5:
                xoaDe();
                break;
            case 6:
                khoaMoDeThi();
                break;
            case 7:
                xemLichThi();
                break;
            }
        } while (ch != 0);
    }
};

void QuanLyThi::saveDeThi() const
{
    ofstream fout("dethi.txt");
    for (const auto& de : maDe)
    {
        fout << de.id << '|' << de.tenMon << '|' << de.tenBaiThi << '|' << de.thoiGianLamBai << '|'
            << de.loaiThi << '|' << de.danhSachCauHoi.size()
            << '|' << de.phongThiId << '|' << (de.batDau > 0 ? formatDateTime(de.batDau) : string(""))
            << '|' << (de.ketThuc > 0 ? formatDateTime(de.ketThuc) : string("")) << '|' << (de.locked ? 1 : 0)
            << '|' << de.soLanOnTap << '|' << (de.randomDe ? 1 : 0)
            << '|' << (de.shuffleAns ? 1 : 0) << '|';

        for (size_t i = 0; i < de.dsLopDuocThi.size(); i++)
        {
            fout << de.dsLopDuocThi[i] << (i < de.dsLopDuocThi.size() - 1 ? "," : "");
        }
        if (de.dsLopDuocThi.empty())
            fout << "ALL";
        fout << '\n';

        for (const auto& ch : de.danhSachCauHoi)
        {
            fout << ch.id << '|' << ch.noiDung << '|' << ch.dapAn[0] << '|' << ch.dapAn[1] << '|' << ch.dapAn[2] << '|' << ch.dapAn[3] << '|' << ch.dapAnDung << '\n';
        }
    }
    fout.close();
}

void QuanLyThi::loadDeThi()
{
    maDe.clear();
    ifstream fin("dethi.txt");
    if (!fin) return;

    vector<string> lines;
    string line;
    while (getline(fin, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        lines.push_back(line);
    }
    fin.close();

    if (lines.empty())
        return;

    bool isMachineFormat = false;
    for (const auto& l : lines)
    {
        string trimmed = trimString(l);
        if (trimmed.empty())
            continue;
        if (trimmed.find('|') != string::npos)
            isMachineFormat = true;
        break;
    }

    if (isMachineFormat)
    {
        for (size_t idx = 0; idx < lines.size(); ++idx)
        {
            string current = lines[idx];
            if (current.empty())
                continue;

            stringstream ss(current);
            vector<string> tokens;
            string token;
            while (getline(ss, token, '|'))
                tokens.push_back(token);

            if (tokens.size() < 8)
                continue;

            deThi de;
            de.id = safeStoi(tokens[0]);
            de.tenMon = tokens[1];
            de.tenBaiThi = tokens[2];
            de.thoiGianLamBai = safeStoi(tokens[3]);
            de.loaiThi = safeStoi(tokens[4]);
            int soCau = safeStoi(tokens[5]);
            de.phongThiId = safeStoi(tokens[6]);

            string dsLopStr = "";
            if (tokens.size() >= 13)
            {
                de.batDau = parseDateTime(tokens[7]);
                de.ketThuc = parseDateTime(tokens[8]);
                de.locked = (tokens[9] == "1");
                de.soLanOnTap = safeStoi(tokens[10], -1);
                de.randomDe = (tokens[11] == "1");
                de.shuffleAns = (tokens[12] == "1");
                dsLopStr = (tokens.size() > 13 ? tokens[13] : "");
            }
            else if (tokens.size() >= 12)
            {
                de.batDau = tokens[7].empty() ? 0 : parseDateTime(tokens[7]);
                de.ketThuc = tokens[8].empty() ? 0 : parseDateTime(tokens[8]);
                de.locked = (tokens[9] == "1");
                de.soLanOnTap = safeStoi(tokens[10], -1);
                de.randomDe = (tokens[11] == "1");
                de.shuffleAns = false;
                if (tokens.size() > 12)
                    dsLopStr = tokens[12];
            }
            else
            {
                de.batDau = 0;
                de.ketThuc = 0;
                de.locked = false;
                de.soLanOnTap = -1;
                de.randomDe = false;
                de.shuffleAns = false;
                if (tokens.size() > 7)
                    dsLopStr = tokens[7];
            }

            if (dsLopStr != "ALL" && !dsLopStr.empty())
            {
                stringstream ssLop(dsLopStr);
                string tenLop;
                while (getline(ssLop, tenLop, ','))
                {
                    tenLop = trimString(tenLop);
                    if (!tenLop.empty())
                        de.dsLopDuocThi.push_back(tenLop);
                }
            }

            for (int i = 0; i < soCau; i++)
            {
                if (idx + 1 >= lines.size())
                    break;
                current = lines[++idx];
                if (!current.empty() && current.back() == '\r')
                    current.pop_back();

                stringstream s2(current);
                vector<string> chTk;
                string c;
                while (getline(s2, c, '|'))
                    chTk.push_back(c);
                if (chTk.size() < 7)
                    continue;

                CauHoi ch;
                ch.id = safeStoi(chTk[0]);
                ch.noiDung = chTk[1];
                ch.dapAn[0] = chTk[2];
                ch.dapAn[1] = chTk[3];
                ch.dapAn[2] = chTk[4];
                ch.dapAn[3] = chTk[5];
                ch.dapAnDung = safeStoi(chTk[6]);
                de.danhSachCauHoi.push_back(ch);
            }
            maDe.push_back(de);
        }
        return;
    }

    deThi readableDe;
    readableDe.id = 1;
    readableDe.tenMon = "";
    readableDe.tenBaiThi = "";
    readableDe.thoiGianLamBai = 0;
    readableDe.loaiThi = 1;
    readableDe.phongThiId = 0;
    readableDe.batDau = 0;
    readableDe.ketThuc = 0;
    readableDe.locked = false;
    readableDe.soLanOnTap = -1;
    readableDe.randomDe = false;
    readableDe.shuffleAns = false;
    readableDe.dsLopDuocThi.clear();
    readableDe.danhSachCauHoi.clear();

    size_t nextQuestionId = 1;
    bool foundAnyHeader = false;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        string trimmed = trimString(lines[i]);
        if (trimmed.empty())
            continue;

        if (beginsWith(trimmed, "Ma de thi:"))
        {
            string idText = extractFieldValue(trimmed, "Ma de thi:");
            int idValue = extractNumericSuffix(idText);
            if (idValue <= 0)
                idValue = 1;
            readableDe.id = idValue;
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "Ten bai thi:"))
        {
            readableDe.tenBaiThi = extractFieldValue(trimmed, "Ten bai thi:");
            readableDe.tenMon = readableDe.tenBaiThi;
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "TeacherID:"))
        {
            readableDe.phongThiId = safeStoi(extractFieldValue(trimmed, "TeacherID:"), 0);
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "Lop duoc thi:"))
        {
            readableDe.dsLopDuocThi = parseListSection(lines, i);
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "So cau hoi:"))
        {
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "Thoi gian lam bai:"))
        {
            string value = extractFieldValue(trimmed, "Thoi gian lam bai:");
            size_t pos = value.find("phut");
            if (pos != string::npos)
                value = trimString(value.substr(0, pos));
            readableDe.thoiGianLamBai = safeStoi(value, 0);
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "Ngay mo de:"))
        {
            string value = extractFieldValue(trimmed, "Ngay mo de:");
            if (value.empty() && i + 1 < lines.size())
                value = trimString(lines[i + 1]);
            readableDe.batDau = value.empty() ? 0 : parseDateTime(value);
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "Ngay dong de:"))
        {
            string value = extractFieldValue(trimmed, "Ngay dong de:");
            if (value.empty() && i + 1 < lines.size())
                value = trimString(lines[i + 1]);
            readableDe.ketThuc = value.empty() ? 0 : parseDateTime(value);
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "Random cau hoi:"))
        {
            readableDe.randomDe = parseYesNo(extractFieldValue(trimmed, "Random cau hoi:"));
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "Shuffle dap an:"))
        {
            readableDe.shuffleAns = parseYesNo(extractFieldValue(trimmed, "Shuffle dap an:"));
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "Cho phep thi lai:"))
        {
            readableDe.soLanOnTap = parseYesNo(extractFieldValue(trimmed, "Cho phep thi lai:")) ? 1 : -1;
            foundAnyHeader = true;
        }
        else if (beginsWith(trimmed, "Trang thai:"))
        {
            string value = extractFieldValue(trimmed, "Trang thai:");
            if (value.empty() && i + 1 < lines.size())
                value = trimString(lines[i + 1]);
            readableDe.locked = !parseYesNo(value);
            foundAnyHeader = true;
        }
    }

    for (size_t idx = 0; idx < lines.size(); ++idx)
    {
        string trimmed = trimString(lines[idx]);
        if (!beginsWith(trimmed, "[Cau"))
            continue;

        size_t cursor = idx + 1;
        while (cursor < lines.size() && trimString(lines[cursor]).empty())
            ++cursor;

        if (cursor >= lines.size())
            break;

        string questionText = trimString(lines[cursor++]);
        while (cursor < lines.size())
        {
            string nextLine = trimString(lines[cursor]);
            if (nextLine.empty())
            {
                ++cursor;
                continue;
            }
            if (beginsWith(nextLine, "A.") || beginsWith(nextLine, "B.") || beginsWith(nextLine, "C.") || beginsWith(nextLine, "D.") || beginsWith(nextLine, "Dap an dung:") || beginsWith(nextLine, "[Cau"))
                break;
            questionText += " " + nextLine;
            ++cursor;
        }

        string options[4] = {"", "", "", ""};
        while (cursor < lines.size())
        {
            string nextLine = trimString(lines[cursor]);
            if (nextLine.empty())
            {
                ++cursor;
                continue;
            }
            if (beginsWith(nextLine, "A."))
            {
                options[0] = trimString(nextLine.substr(2));
                ++cursor;
                continue;
            }
            if (beginsWith(nextLine, "B."))
            {
                options[1] = trimString(nextLine.substr(2));
                ++cursor;
                continue;
            }
            if (beginsWith(nextLine, "C."))
            {
                options[2] = trimString(nextLine.substr(2));
                ++cursor;
                continue;
            }
            if (beginsWith(nextLine, "D."))
            {
                options[3] = trimString(nextLine.substr(2));
                ++cursor;
                continue;
            }
            break;
        }

        int answerIndex = 0;
        while (cursor < lines.size())
        {
            string nextLine = trimString(lines[cursor]);
            if (nextLine.empty())
            {
                ++cursor;
                continue;
            }
            if (beginsWith(nextLine, "Dap an dung:"))
            {
                answerIndex = parseAnswerLetter(extractFieldValue(nextLine, "Dap an dung:"));
                ++cursor;
                break;
            }
            if (beginsWith(nextLine, "[Cau"))
                break;
            ++cursor;
        }

        CauHoi ch;
        ch.id = static_cast<int>(nextQuestionId++);
        ch.noiDung = questionText;
        ch.dapAn[0] = options[0];
        ch.dapAn[1] = options[1];
        ch.dapAn[2] = options[2];
        ch.dapAn[3] = options[3];
        ch.dapAnDung = answerIndex;
        readableDe.danhSachCauHoi.push_back(ch);
    }

    if (foundAnyHeader && !readableDe.danhSachCauHoi.empty())
    {
        if (readableDe.tenMon.empty())
            readableDe.tenMon = readableDe.tenBaiThi;
        maDe.push_back(readableDe);
    }
}
