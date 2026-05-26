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
// ========================== STUDENT ==========================
class SinhVien : public Person
{
private:
    string lopChuNhiem;       // chỉ 1 lớp chủ nhiệm
    vector<string> monHoc;    // nhiều môn học
    vector<pair<string, double>> diemMon; // diem theo mon hoc
    string name;
    string gioitinh;
    string birthDay;
    string sdt;
    vector<KetQua> danhSachKetQua;
    double diemTB;
    int teacherId;


public:
    SinhVien() : Person()
    {
        lopChuNhiem = "";
        gioitinh = "";
        birthDay = "";
        sdt = "";
        diemTB = 0.0;
        teacherId = 0;
    }

    SinhVien(int id, string userN, string passW, string ten,
        string lopChuNhiem, string gioitinh, string birthDay, string sdt, double diemTB, int teacherId)
        : Person(id, userN, passW, ten), lopChuNhiem(lopChuNhiem), gioitinh(gioitinh), birthDay(birthDay), sdt(sdt), diemTB(diemTB), teacherId(teacherId)
    {
    }

    bool kiemTraLopDuocThi(const deThi& de) const
    {
        if (de.dsLopDuocThi.empty())
            return true;
        for (const string& l : de.dsLopDuocThi)
        {
            if (l == this->lopChuNhiem)
                return true;
        }
        return false;
    }

    // ===== GET/SET ID =====
    int getId() const
    {
        return id;
    }

    void setId(int x)
    {
        id = x;
    }

    // ===== GET/SET NAME =====
    string getName() const
    {
        return name;
    }

    void setName(const string& s)
    {
        name = s;
    }

    // ===== LOP CHU NHIEM =====
    string getLopChuNhiem() const
    {
        return lopChuNhiem;
    }

    void setLopChuNhiem(const string& lop)
    {
        lopChuNhiem = lop;
    }

    // ===== MON HOC =====
    void addMonHoc(const string& mon)
    {
        monHoc.push_back(mon);
    }

    vector<string> getMonHoc() const
    {
        return monHoc;
    }
    

    string getRole() const override { return "SinhVien"; }
    string getLop() const { return lopChuNhiem; }
    string getGioiTinh() const { return gioitinh; }
    string getBirthDay() const { return birthDay; }
    string getSdt() const { return sdt; }
    int layMaGV() const { return teacherId; }
    double layDiem() const { return diemTB; }

    void datDiem(double d) { diemTB = d; }
    void setlopChuNhiem(const string& lop) { this->lopChuNhiem = lop; }
    void setMonHoc(const vector<string>& monHoc) { this->monHoc = monHoc; }
    void addDiemMon(const string& mon, double diem) { diemMon.emplace_back(mon, diem); }
    void setDiemMon(const vector<pair<string, double>>& diemMon) { this->diemMon = diemMon; }
    const vector<pair<string, double>>& getDiemMon() const { return diemMon; }
    double tinhDiemTBTatCaMon() const
    {
        if (diemMon.empty()) return diemTB;
        double tong = 0;
        for (const auto& p : diemMon)
        {
            tong += p.second;
        }
        return round((tong / diemMon.size()) * 100.0) / 100.0;
    }

    double tinhDiemTBChinhThuc() const
    {
        double tong = 0;
        int dem = 0;
        for (const auto& kq : danhSachKetQua)
        {
            if (kq.loaiThi == 1)
            {
                tong += kq.diem;
                dem++;
            }
        }
        if (dem == 0) return 0.0;
        return round((tong / dem) * 100.0) / 100.0;
    }

    string getHocLuc() const
    {
        double tb = tinhDiemTBTatCaMon();
        if (tb >= 8.0) return "Gioi";
        if (tb >= 6.5) return "Kha";
        if (tb >= 5.0) return "Trung binh";
        return "Yeu";
    }
    void setGioiTinh(const string& gioitinh) { this->gioitinh = gioitinh; }
    void setBirthDay(const string& birthDay) { this->birthDay = birthDay; }
    void setSdt(const string& sdt) { this->sdt = sdt; }
    void setDiemTB(double diemTB) { this->diemTB = diemTB; }
    void setTeacherId(int teacherId) { this->teacherId = teacherId; }
    void setDanhSachKetQua(const vector<KetQua>& ds) { danhSachKetQua = ds; }
    const vector<KetQua>& getDanhSachKetQua() const { return danhSachKetQua; }

    void hienThiThongTin() const
    {
        cout << "========== Thong Tin Sinh Vien ==========\n"
            << "[HS] " << fullName << " | ID: " << id << " | Lop: " << lopChuNhiem << "\n"
            << "Tai khoan: " << username << " | Mat khau: (*******)\n"
            << "Gioi tinh: " << gioitinh << " | Ngay sinh: " << birthDay << " | SDT: " << sdt << "\n";

        if (!diemMon.empty())
        {
            cout << "| Diem mon:\n";
            for (const auto& p : diemMon)
            {
                cout << "    " << p.first << ": " << fixed << setprecision(2) << p.second << "\n";
            }
        }
        cout << "| Diem TB tat ca mon: " << fixed << setprecision(2) << tinhDiemTBTatCaMon() << "\n"
             << "| Hoc luc: " << getHocLuc() << " | TeacherID: " << teacherId << endl;
    }

    void xemDanhSachDe(QuanLyThi& thi, int loaiBai)
    {
        SetColor(LIGHT_CYAN);
        cout << (loaiBai == 1 ? "\n--- DANH SACH BAI THI CHINH THUC ---\n" : "\n--- DANH SACH BAI ON TAP ---\n");
        SetColor(WHITE);

        bool coBai = false;

        for (const auto& de : thi.getMaDe())
        {
            if (!kiemTraLopDuocThi(de) || de.loaiThi != loaiBai)
                continue;

            bool daThiChinhThuc = false;
            if (de.loaiThi == 1)
            {
                for (const auto& kq : danhSachKetQua)
                {
                    if (kq.maDe == de.id && kq.loaiThi == 1)
                    {
                        daThiChinhThuc = true;
                        break;
                    }
                }
            }
            if (!daThiChinhThuc)
            {
                string loaiStr = (de.loaiThi == 1) ? "[CHINH THUC (1 lan)]" : "[ON TAP ]";
                string lopInfo;
                if (de.dsLopDuocThi.empty())
                    lopInfo = "ALL";
                else
                {
                    for (size_t i = 0; i < de.dsLopDuocThi.size(); i++)
                    {
                        lopInfo += de.dsLopDuocThi[i];
                        if (i + 1 < de.dsLopDuocThi.size())
                            lopInfo += ", ";
                    }
                }
                cout << "Ten bai thi: " << de.tenBaiThi
                    << " | Mon: " << de.tenMon << " " << loaiStr
                    << " | Thoi gian: " << de.thoiGianLamBai << " phut"
                    << " | Bat dau: " << (de.batDau == 0 ? string("Chua co") : formatDateTime(de.batDau))
                    << " | Ket thuc: " << (de.ketThuc == 0 ? string("Chua co") : formatDateTime(de.ketThuc))
                    << " | Lop: " << lopInfo << "\n";
                coBai = true;
            }
        }
        if (!coBai)
        {
            SetColor(YELLOW);
            cout << (loaiBai == 1 ? "Hien tai khong co bai thi chinh thuc nao!\n" : "Hien tai khong co bai on tap nao!\n");
            SetColor(WHITE);
        }
    }

    void xemThongTinDe(QuanLyThi& thi, int loaiBai)
    {
        string tenBai = inputLine("Nhap ten bai thi can xem: ");
        for (auto& de : thi.getMaDe())
        {
            if (de.tenBaiThi == tenBai && de.loaiThi == loaiBai)
            {
                if (!kiemTraLopDuocThi(de))
                {
                    cout << "Ban khong co quyen xem thong tin de thi nay!\n";
                    return;
                }
                cout << "\n===== THONG TIN CHI TIET =====\n";
                cout << "Ten bai thi: " << de.tenBaiThi << endl;
                cout << "Mon: " << de.tenMon << endl;
                cout << "So cau hoi: " << de.danhSachCauHoi.size() << endl;
                cout << "Thoi gian: " << de.thoiGianLamBai << " phut\n";
                cout << "Trang thai: " << (de.locked ? "Dang khoa" : "Dang mo") << endl;
                if (de.loaiThi == 1)
                    cout << "Lich thi: " << de.getSchedule() << endl;
                else
                    cout << "So lan lam toi da: " << (de.soLanOnTap == -1 ? "Khong gioi han" : to_string(de.soLanOnTap)) << endl;
                return;
            }
        }
        cout << "Khong tim thay bai thi nay trong danh sach!\n";
    }

    double traLoiCauHoi(vector<CauHoi> danhSachCauHoi, time_t start, int thoiGian) const
{
    int n;
    double diemThi = 0;

    timerRunning = true;

    thread timerThread(hienThiDongHo, start, thoiGian);

    for (size_t idx = 0; idx < danhSachCauHoi.size(); ++idx)
    {
        const auto& ch = danhSachCauHoi[idx];

        int remaining = thoiGian - (int)difftime(time(0), start);

        if (remaining <= 0)
        {
            cout << "\n\nHet gio lam bai!\n";
            break;
        }

        cout << "\n\n";
        cout << "=====================================\n";

        SetColor(LIGHT_CYAN);
        cout << "Cau " << idx + 1 << ". ";
        SetColor(WHITE);

        cout << ch.noiDung << "\n\n";

        for (int i = 0; i < 4; i++)
        {
            cout << i + 1 << ". " << ch.dapAn[i] << "\n";
        }

        cout << "\n";

        do
        {
            n = nhapSo<int>("Dap an ban chon (1-4): ");
        }
        while (n < 1 || n > 4);

        if (n == ch.dapAnDung)
        {
            diemThi += 10.0 / danhSachCauHoi.size();
        }
    }

    timerRunning = false;

    if (timerThread.joinable())
        timerThread.join();

    return diemThi;
}

    bool verifyStudentInfo()
    {
        SetColor(LIGHT_CYAN);
        cout << "\n===== XAC THUC THONG TIN =====\n";
        SetColor(WHITE);
        
        int nhapID = nhapSo<int>("Nhap ID sinh vien: ");
        string nhapLop = inputLine("Nhap lop: ");
        
        if (nhapID != this->id)
        {
            SetColor(RED);
            cout << "ID khong dung!\n";
            SetColor(WHITE);
            return false;
        }
        
        if (nhapLop != this->lopChuNhiem)
        {
            SetColor(RED);
            cout << "Lop khong dung!\n";
            SetColor(WHITE);
            return false;
        }
        
        SetColor(GREEN);
        cout << "Xac thuc thanh cong! Chao " << this->fullName << "\n";
        SetColor(WHITE);
        return true;
    }

    void lamBai(QuanLyThi& thi, int loaiBai, function<void()> onChinhThucSubmit = nullptr)
    {
        if (!verifyStudentInfo()) {
            return;
        }
        
        cout << "\n===== CHON BAI THI =====\n";
        xemDanhSachDe(thi, loaiBai);
        
        string tenBaiThi = inputLine("Nhap ten bai thi de chon: ");
        deThi* deHienTai = nullptr;
        for (auto& de : thi.getMaDe())
        {
            if (de.tenBaiThi == tenBaiThi && de.loaiThi == loaiBai && kiemTraLopDuocThi(de))
            {
                deHienTai = const_cast<deThi*>(&de);
                break;
            }
        }

        if (deHienTai == nullptr || deHienTai->danhSachCauHoi.empty())
        {
            SetColor(RED);
            cout << "De khong hop le hoac chua co cau hoi!\n";
            SetColor(WHITE);
            return;
        }
        if (!kiemTraLopDuocThi(*deHienTai))
        {
            SetColor(RED);
            cout << "Ban khong thuoc danh sach lop duoc phep lam bai nay!\n";
            SetColor(WHITE);
            return;
        }
        if (deHienTai->locked)
        {
            SetColor(RED);
            cout << "Bai dang bi khoa!\n";
            SetColor(WHITE);
            return;
        }
        if (loaiBai == 1 && !deHienTai->isAvailable())
        {
            SetColor(YELLOW);
            if (deHienTai->batDau != 0 && time(0) < deHienTai->batDau)
            {
                cout << "Chua den gio. Lich thi: " << deHienTai->getSchedule() << "\n";
            }
            else if (deHienTai->ketThuc != 0 && time(0) > deHienTai->ketThuc)
            {
                cout << "Da ket thuc. Lich thi: " << deHienTai->getSchedule() << "\n";
            }
            else
            {
                cout << "Bai thi hien tai chua duoc mo. Lich thi: " << deHienTai->getSchedule() << "\n";
            }
            SetColor(WHITE);
            return;
        }

        int soLanDaThi = 0;
        for (const auto& kq : danhSachKetQua)
        {
            if (kq.maDe == deHienTai->id)
                soLanDaThi++;
        }

        if (loaiBai == 1)
        {
            if (soLanDaThi >= 1)
            {
                SetColor(RED);
                cout << "Bai thi Chinh thuc chi duoc lam 1 lan. Ban da nop bai roi!\n";
                SetColor(WHITE);
                return;
            }
            SetColor(LIGHT_GREEN);
            cout << "Bat dau lam bai thi chinh thuc.\n";
            SetColor(WHITE);
        }
        else
        {
            if (deHienTai->soLanOnTap != -1 && soLanDaThi >= deHienTai->soLanOnTap)
            {
                SetColor(RED);
                cout << "Ban da lam bai on tap nay " << soLanDaThi << "/"
                    << deHienTai->soLanOnTap << " lan. Het luot!\n";
                SetColor(WHITE);
                return;
            }

            SetColor(LIGHT_CYAN);
            if (deHienTai->soLanOnTap == -1)
                cout << "Lan lam thu " << soLanDaThi + 1 << " (Khong gioi han)\n";
            else
                cout << "Lan lam thu " << soLanDaThi + 1
                << "/" << deHienTai->soLanOnTap << "\n";
            SetColor(WHITE);
        }

        cout << "\n=== BAT DAU LAM BAI: " << deHienTai->tenBaiThi << " ===\n";
        SetColor(WHITE);
        time_t start = time(0);

        // Random câu hỏi nếu cần
        vector<CauHoi> cauHoiLam = deHienTai->danhSachCauHoi;
        if (deHienTai->randomDe) {
            cauHoiLam = shuffleCauHoi(cauHoiLam);
            SetColor(LIGHT_YELLOW);
            cout << "Cau hoi da duoc sap xep ngau nhien.\n";
            SetColor(WHITE);
        }

        double diemThi = traLoiCauHoi(cauHoiLam, start, deHienTai->thoiGianLamBai * 60);

        int thoiGianThucTe = (int)difftime(time(0), start);
        if (thoiGianThucTe == 0) thoiGianThucTe = 1;

        SetColor(GREEN);
        cout << "=> Nop bai thanh cong! Diem: " << fixed << setprecision(1) << diemThi 
             << "/10\n";
        SetColor(WHITE);

        KetQua kq;
        kq.idSV = this->id;
        kq.tenSV = this->fullName;
        kq.diem = diemThi;
        kq.mon = deHienTai->tenMon;
        kq.thoiGian = thoiGianThucTe;
        kq.maDe = deHienTai->id;
        kq.loaiThi = deHienTai->loaiThi;
        this->danhSachKetQua.push_back(kq);

        // Tự động lưu ranking.txt ngay sau khi nộp bài thi chính thức
        if (loaiBai == 1 && onChinhThucSubmit)
            onChinhThucSubmit();

        if (loaiBai == 2)
        {
            SetColor(LIGHT_CYAN);
            cout << "\n===== DAP AN =====\n";
            for (const auto& ch : deHienTai->danhSachCauHoi)
            {
                cout << ch.id << ". Dap an dung: " << ch.dapAnDung << ". " << ch.dapAn[ch.dapAnDung - 1] << "\n";
            }
            SetColor(WHITE);
        }
    }

    void hienThiDanhSachKetQua(const QuanLyThi& thi) const
    {
        cout << "\n===== LICH SU LAM BAI =====\n";
        if (danhSachKetQua.empty())
        {
            cout << "Chua co ket qua nao!\n";
            return;
        }
        for (const auto& kq : danhSachKetQua)
        {
            string loai = (kq.loaiThi == 1) ? "[Chinh thuc]" : "[On tap]";
            string tenBaiThi = "";
            const deThi* de = thi.timDeThi(kq.maDe);
            if (de)
            {
                tenBaiThi = de->tenBaiThi;
            }
            cout << "Mon: " << kq.mon << " | Ten bai thi: " << tenBaiThi << " | Diem: " << kq.diem << "/10 | Thoi gian: " << kq.thoiGian << "s " << loai << "\n";
        }
    }

    void thongKeDiemTB(const QuanLyThi&)
    {
        if (danhSachKetQua.empty())
        {
            cout << "\nBan chua co diem bai thi nao!\n";
            return;
        }
        
        map<string, vector<double>> diemTheoMon;
        for (const auto& kq : danhSachKetQua)
        {
            if (kq.loaiThi == 1)
            {
                diemTheoMon[kq.mon].push_back(kq.diem);
            }
        }
        
        cout << "\n===== THONG KE DIEM THEO MON =====\n";
        if (diemTheoMon.empty())
        {
            cout << "Ban chua co diem bai thi Chinh thuc nao!\n";
        }
        else
        {
            double tongTatCa = 0;
            int demTatCa = 0;
            for (const auto& [mon, diem] : diemTheoMon)
            {
                double tong = 0;
                for (double d : diem)
                    tong += d;
                double tb = tong / diem.size();
                cout << "Mon: " << mon << " | So bai: " << diem.size() << " | Diem TB: " << fixed << setprecision(2) << tb << "\n";
                tongTatCa += tong;
                demTatCa += diem.size();
            }
            cout << "\nDiem TB tich luy (chinh thuc): " << fixed << setprecision(2) << (tongTatCa / demTatCa) << "\n";
        }
    }

    void xemBangXepHang(QuanLyThi& thi, const vector<SinhVien>& dsSinhVien)
    {
        cout << "\n===== BANG XEP HANG (CHINH THUC) =====\n";
        cout << "1. Xep hang toan truong\n";
        cout << "2. Xep hang theo lop\n";
        cout << "3. Xep hang theo ten bai thi\n";
        int choice = nhapSo<int>("Chon kieu xep hang: ");

        if (choice == 1)
        {
            SetColor(CYAN);
            cout << "\n===== BANG XEP HANG TOAN TRUONG (CHINH THUC) =====\n";
            SetColor(WHITE);
            struct HSScore { string fullName; double score; };
            vector<HSScore> bxh;

            for (const auto& hs : dsSinhVien)
            {
                double tong = 0;
                int dem = 0;
                for (const auto& kq : hs.getDanhSachKetQua())
                {
                    if (kq.loaiThi == 1)
                    {
                        tong += kq.diem;
                        dem++;
                    }
                }
                if (dem > 0)
                    bxh.push_back({ hs.getFullName(), tong / dem });
            }

            if (bxh.empty())
            {
                SetColor(YELLOW);
                cout << "Chua co du lieu de xep hang!\n";
                SetColor(WHITE);
                return;
            }

            sort(bxh.begin(), bxh.end(), [](const HSScore& a, const HSScore& b) { return a.score > b.score; });
            for (int i = 0; i < (int)bxh.size(); i++)
            {
                if (bxh[i].fullName == this->fullName) SetColor(LIGHT_GREEN);
                cout << i + 1 << ". Ho ten: " << bxh[i].fullName << " | Diem TB: " << fixed << setprecision(2) << bxh[i].score << '\n';
                SetColor(WHITE);
            }
        }
        else if (choice == 2)
        {
            string tenLop = inputLine("Nhap ten lop can xem bang xep hang: ");
            SetColor(CYAN);
            cout << "\n===== BANG XEP HANG LOP: " << tenLop << " (CHINH THUC) =====\n";
            SetColor(WHITE);

            struct HSScore { string fullName; string className; double score; };
            vector<HSScore> bxh;

            for (const auto& hs : dsSinhVien)
            {
                if (hs.getLop() != tenLop)
                    continue;
                double tong = 0;
                int dem = 0;
                for (const auto& kq : hs.getDanhSachKetQua())
                {
                    if (kq.loaiThi == 1)
                    {
                        tong += kq.diem;
                        dem++;
                    }
                }
                if (dem > 0)
                    bxh.push_back({ hs.getFullName(), hs.getLop(), tong / dem });
            }

            if (bxh.empty())
            {
                SetColor(YELLOW);
                cout << "Chua co sinh vien nao trong lop " << tenLop << " co diem thi chinh thuc!\n";
                SetColor(WHITE);
                return;
            }

            sort(bxh.begin(), bxh.end(), [](const HSScore& a, const HSScore& b) { return a.score > b.score; });
            for (int i = 0; i < (int)bxh.size(); i++)
            {
                cout << i + 1 << ". Ho ten: " << bxh[i].fullName << " | Lop: " << bxh[i].className << " | Diem TB: " << fixed << setprecision(2) << bxh[i].score << '\n';
            }
        }
        else if (choice == 3)
        {
            xemBangXepHangTheoBaiThi(thi, dsSinhVien);
        }
        else
        {
            SetColor(RED);
            cout << "Lua chon khong hop le!\n";
            SetColor(WHITE);
        }
    }

    void xemBangXepHangTheoBaiThi(const QuanLyThi& thi, const vector<SinhVien>& dsSinhVien)
    {
        string tenBaiThi = inputLine("Nhap ten bai thi can xem xep hang: ");
        const deThi* de = nullptr;
        const auto& maDe = thi.getMaDe();
        for (const auto& d : maDe)
        {
            if (d.tenBaiThi == tenBaiThi)
            {
                de = &d;
                break;
            }
        }
        
        if (!de) {
            SetColor(RED);
            cout << "Ten bai thi khong ton tai!\n";
            SetColor(WHITE);
            return;
        }

        SetColor(CYAN);
        cout << "\n===== BANG XEP HANG BAI THI: " << de->tenBaiThi << " =====\n";
        SetColor(WHITE);

        struct HSScore {
            string fullName;
            double score;
        };
        vector<HSScore> bxh;

        for (const auto& hs : dsSinhVien)
        {
            if (!hs.kiemTraLopDuocThi(*de)) continue;
            
            for (const auto& kq : hs.getDanhSachKetQua())
            {
                if (kq.maDe == de->id && kq.loaiThi == 1)
                {
                    bxh.push_back({ hs.getFullName(), kq.diem });
                    break;
                }
            }
        }

        if (bxh.empty())
        {
            SetColor(YELLOW);
            cout << "Chua co ai lam bai thi nay!\n";
            SetColor(WHITE);
            return;
        }

        sort(bxh.begin(), bxh.end(), [](const HSScore& a, const HSScore& b) { return a.score > b.score; });

        for (int i = 0; i < (int)bxh.size(); i++)
        {
            if (bxh[i].fullName == this->fullName) SetColor(LIGHT_GREEN);
            cout << i + 1 << ". Ho ten: " << bxh[i].fullName << " | Diem: " << fixed << setprecision(2) << bxh[i].score << '\n';
            SetColor(WHITE);
        }
    }

    void datLaiMatKhau()
    {
        string mk;
        cout << "Nhap mat khau moi: ";
        getline(cin, mk);
        password = mk;
        SetColor(GREEN);
        cout << "Doi mat khau thanh cong!\n";
        SetColor(WHITE);
    }

    string chuyenThanhChuoiFile() const
    {
        return to_string(id) + "|" + username + "|" + password + "|" + fullName + "|" + lopChuNhiem + "|" + to_string(diemTB) + "|" + to_string(teacherId);
    }

    void menu(QuanLyThi& thi, const vector<SinhVien>& dsSinhVienToanTruong, function<void()> onChinhThucSubmit = nullptr)
    {
        int choice;
        do
        {
            SetColor(CYAN);
            cout << "\n╔══════════════════════════════════════════════╗\n";
            cout << "║               MENU SINH VIEN                 ║\n";
            cout << "╠══════════════════════════════════════════════╣\n";

            SetColor(YELLOW);
            cout << "║  1. Xem thong tin ca nhan                    ║\n";
            cout << "║  2. Xem danh sach bai thi                    ║\n";
            cout << "║  3. Xem thong tin bai thi                    ║\n";
            cout << "║  4. Lam bai thi                              ║\n";
            cout << "║  5. Xem lich su thi                          ║\n";
            cout << "║  6. Thong ke diem trung binh cua bai thi     ║\n";
            cout << "║  7. Xem bang xep hang                        ║\n";
            cout << "║  8. Xem danh sach bai on tap                 ║\n";
            cout << "║  9. Xem thong tin bai on tap                 ║\n";
            cout << "║ 10. Lam bai on tap                           ║\n";
            cout << "║ 11. Doi mat khau                             ║\n";
            cout << "║  0. Dang xuat                                ║\n";

            SetColor(CYAN);
            cout << "╚══════════════════════════════════════════════╝\n";

            SetColor(GREEN);
            choice = nhapSo<int>("Chon chuc nang: ");

            SetColor(WHITE);
            switch (choice)
            {
            case 1: hienThiThongTin(); break;
            case 2: xemDanhSachDe(thi, 1); break;
            case 3: xemThongTinDe(thi, 1); break;
            case 4: lamBai(thi, 1, onChinhThucSubmit); break;
            case 5: hienThiDanhSachKetQua(thi); break;
            case 6: thongKeDiemTB(thi); break;
            case 7: xemBangXepHang(thi, dsSinhVienToanTruong); break;
            case 8: xemDanhSachDe(thi, 2); break;
            case 9: xemThongTinDe(thi, 2); break;
            case 10: lamBai(thi, 2); break;
            case 11:
                datLaiMatKhau();
                break;
            case 0:
                SetColor(GREEN);
                cout << "Dang xuat thanh cong!\n";
                SetColor(WHITE);
                break;
            default:
                SetColor(RED);
                cout << "Lua chon khong hop le!\n";
                SetColor(WHITE);
            }
        } while (choice != 0);
    }
};
// ========================== RANK ITEM ==========================
struct RankItem
{
    int id;
    string fullName;
    double score;
};

// ========================== TEACHER ==========================
// ========================== EXPORT RANKING TO EXCEL (free function) ==========================
void exportRankingVectorToExcel(const vector<RankItem>& ranking)
{
    if (ranking.empty()) return;

    auto xmlEscape = [](const string& s) -> string {
        string out;
        for (char c : s) {
            if      (c == '&')  out += "&amp;";
            else if (c == '<')  out += "&lt;";
            else if (c == '>')  out += "&gt;";
            else if (c == '"')  out += "&quot;";
            else if (c == '\'') out += "&apos;";
            else                out += c;
        }
        return out;
    };

    vector<string> sharedStr = { "STT", "ID Sinh Vien", "Ho Ten", "Diem Trung Binh", "Xep Loai" };
    auto getIdx = [&](const string& val) -> int {
        for (int i = 0; i < (int)sharedStr.size(); i++)
            if (sharedStr[i] == val) return i;
        sharedStr.push_back(val);
        return (int)sharedStr.size() - 1;
    };
    for (const auto& r : ranking) getIdx(r.fullName);

    stringstream sheet;
    sheet << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n"
          << "<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\"><sheetData>";
    sheet << "<row r=\"1\">";
    const char* hdrs[] = { "STT", "ID Sinh Vien", "Ho Ten", "Diem Trung Binh", "Xep Loai" };
    for (int h = 0; h < 5; h++)
        sheet << "<c r=\"" << (char)('A'+h) << "1\" t=\"s\" s=\"1\"><v>" << getIdx(hdrs[h]) << "</v></c>";
    sheet << "</row>";

    for (int i = 0; i < (int)ranking.size(); i++) {
        int row = i + 2;
        const RankItem& r = ranking[i];
        string xepLoai;
        if      (r.score >= 8.5) xepLoai = "Xuat sac";
        else if (r.score >= 8.0) xepLoai = "Gioi";
        else if (r.score >= 6.5) xepLoai = "Kha";
        else if (r.score >= 5.0) xepLoai = "Trung binh";
        else                     xepLoai = "Yeu";
        getIdx(xepLoai);
        sheet << "<row r=\"" << row << "\">"
              << "<c r=\"A" << row << "\" s=\"2\"><v>" << (i+1) << "</v></c>"
              << "<c r=\"B" << row << "\" s=\"2\"><v>" << r.id << "</v></c>"
              << "<c r=\"C" << row << "\" t=\"s\" s=\"2\"><v>" << getIdx(r.fullName) << "</v></c>"
              << "<c r=\"D" << row << "\" s=\"3\"><v>" << fixed << setprecision(2) << r.score << "</v></c>"
              << "<c r=\"E" << row << "\" t=\"s\" s=\"2\"><v>" << getIdx(xepLoai) << "</v></c>"
              << "</row>";
    }
    sheet << "</sheetData></worksheet>";

    stringstream sst;
    sst << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n"
        << "<sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\""
        << " count=\"" << sharedStr.size() << "\" uniqueCount=\"" << sharedStr.size() << "\">";
    for (const auto& s : sharedStr) sst << "<si><t>" << xmlEscape(s) << "</t></si>";
    sst << "</sst>";

    const string styles =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n"
        "<styleSheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">"
        "<fonts><font><sz val=\"11\"/><name val=\"Arial\"/></font>"
        "<font><b/><sz val=\"11\"/><name val=\"Arial\"/><color rgb=\"FFFFFFFF\"/></font></fonts>"
        "<fills><fill><patternFill patternType=\"none\"/></fill>"
        "<fill><patternFill patternType=\"gray125\"/></fill>"
        "<fill><patternFill patternType=\"solid\"><fgColor rgb=\"FF1F4E79\"/></patternFill></fill>"
        "<fill><patternFill patternType=\"solid\"><fgColor rgb=\"FFD9E1F2\"/></patternFill></fill></fills>"
        "<borders><border><left/><right/><top/><bottom/><diagonal/></border>"
        "<border><left style=\"thin\"><color rgb=\"FF000000\"/></left>"
        "<right style=\"thin\"><color rgb=\"FF000000\"/></right>"
        "<top style=\"thin\"><color rgb=\"FF000000\"/></top>"
        "<bottom style=\"thin\"><color rgb=\"FF000000\"/></bottom><diagonal/></border></borders>"
        "<cellStyleXfs count=\"1\"><xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\"/></cellStyleXfs>"
        "<cellXfs>"
        "<xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\" xfId=\"0\"/>"
        "<xf numFmtId=\"0\" fontId=\"1\" fillId=\"2\" borderId=\"1\" xfId=\"0\" applyFont=\"1\" applyFill=\"1\" applyBorder=\"1\" applyAlignment=\"1\"><alignment horizontal=\"center\"/></xf>"
        "<xf numFmtId=\"0\" fontId=\"0\" fillId=\"3\" borderId=\"1\" xfId=\"0\" applyFill=\"1\" applyBorder=\"1\" applyAlignment=\"1\"><alignment horizontal=\"center\"/></xf>"
        "<xf numFmtId=\"2\" fontId=\"0\" fillId=\"3\" borderId=\"1\" xfId=\"0\" applyNumberFormat=\"1\" applyFill=\"1\" applyBorder=\"1\" applyAlignment=\"1\"><alignment horizontal=\"center\"/></xf>"
        "</cellXfs></styleSheet>";
    const string workbook =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n"
        "<workbook xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\""
        " xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">"
        "<sheets><sheet name=\"Bang Xep Hang\" sheetId=\"1\" r:id=\"rId1\"/></sheets></workbook>";
    const string wbRels =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
        "<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet\" Target=\"worksheets/sheet1.xml\"/>"
        "<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings\" Target=\"sharedStrings.xml\"/>"
        "<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>"
        "</Relationships>";
    const string pkgRels =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
        "<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"xl/workbook.xml\"/>"
        "</Relationships>";
    const string contentTypes =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n"
        "<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">"
        "<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>"
        "<Default Extension=\"xml\" ContentType=\"application/xml\"/>"
        "<Override PartName=\"/xl/workbook.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml\"/>"
        "<Override PartName=\"/xl/worksheets/sheet1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml\"/>"
        "<Override PartName=\"/xl/sharedStrings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml\"/>"
        "<Override PartName=\"/xl/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml\"/>"
        "</Types>";

    const string tmp = "xlsx_tmp_bao_cao";
    filesystem::remove_all(tmp);
    filesystem::create_directories(tmp + "/xl/worksheets");
    filesystem::create_directories(tmp + "/xl/_rels");
    filesystem::create_directories(tmp + "/_rels");

    auto wf = [](const string& path, const string& content) {
        ofstream f(path, ios::binary); f << content;
    };
    wf(tmp + "/[Content_Types].xml", contentTypes);
    wf(tmp + "/_rels/.rels", pkgRels);
    wf(tmp + "/xl/workbook.xml", workbook);
    wf(tmp + "/xl/_rels/workbook.xml.rels", wbRels);
    wf(tmp + "/xl/worksheets/sheet1.xml", sheet.str());
    wf(tmp + "/xl/sharedStrings.xml", sst.str());
    wf(tmp + "/xl/styles.xml", styles);

    filesystem::remove("bao_cao_thi.xlsx");
    string cmd = "powershell -NoProfile -Command \"Add-Type -Assembly System.IO.Compression.FileSystem; "
                 "[System.IO.Compression.ZipFile]::CreateFromDirectory('"
                 + tmp + "', 'bao_cao_thi.xlsx')\"";
    int ret = system(cmd.c_str());
    filesystem::remove_all(tmp);

    if (ret == 0 && filesystem::exists("bao_cao_thi.xlsx")) {
        SetColor(LIGHT_GREEN);
        cout << "Da cap nhat bao_cao_thi.xlsx\n";
    } else {
        SetColor(YELLOW);
        cout << "Khong the tao file Excel (kiem tra PowerShell).\n";
    }
    SetColor(WHITE);
}

class GiangVien : public Person
{
private:
    string mon;
    string teacherRole;  // comma-separated roles: "BoMon,CoVan" or "BoMon" or "CoVan"
    string advisorClass;

    // Parse roles from comma-separated string
    vector<string> parseRoles() const {
        vector<string> roles;
        stringstream ss(teacherRole);
        string role;
        while (getline(ss, role, ',')) {
            if (!role.empty()) {
                roles.push_back(role);
            }
        }
        return roles;
    }

public:
    void setMon(const string& m) { mon = m; }
    string getMon() const { return mon; }
    void setTeacherRole(const string& r) { teacherRole = r; }
    string getTeacherRole() const { return teacherRole; }
    void setAdvisorClass(const string& c) { advisorClass = c; }
    string getAdvisorClass() const { return advisorClass; }
    
    bool isAdvisor() const {
        vector<string> roles = parseRoles();
        for (const auto& r : roles) {
            if (r == "CoVan" || r == "CoVanHocTap") return true;
        }
        return false;
    }
    
    bool isSubjectTeacher() const {
        vector<string> roles = parseRoles();
        for (const auto& r : roles) {
            if (r == "BoMon") return true;
        }
        return false;
    }
    
    bool hasRole(const string& role) const {
        vector<string> roles = parseRoles();
        for (const auto& r : roles) {
            if (r == role) return true;
        }
        return false;
    }

    void normalizeTeacherRole() {
        bool hasBoMon = false;
        bool hasCoVan = false;
        vector<string> roles = parseRoles();
        for (const auto& r : roles) {
            if (r == "BoMon") hasBoMon = true;
            if (r == "CoVan" || r == "CoVanHocTap") hasCoVan = true;
        }
        if (!hasBoMon && hasCoVan)
            hasBoMon = true;

        teacherRole.clear();
        teacherRole = "BoMon";
        if (hasCoVan)
            teacherRole += ",CoVan";
    }
    
    string getTeacherRoleLabel() const {
        vector<string> roles = parseRoles();
        string label = "";
        for (size_t i = 0; i < roles.size(); i++) {
            if (roles[i] == "BoMon") label += (label.empty() ? "BoMon" : ",BoMon");
            else if (roles[i] == "CoVan" || roles[i] == "CoVanHocTap") {
                label += (label.empty() ? "CoVan" : ",CoVan");
            }
        }
        return label.empty() ? "BoMon" : label;
    }

    GiangVien() : Person(), teacherRole("BoMon"), advisorClass("") {};
    GiangVien(int id, string u, string p, string ten, string mon, string role = "BoMon")
        : Person(id, u, p, ten), mon(mon), teacherRole(role), advisorClass("") {}

    string getRole() const override
    {
        return "GiangVien";
    }

    void hienThiThongTin() const
    {
        SetColor(LIGHT_YELLOW);
        string roleDisplay = "[";
        if (isSubjectTeacher()) roleDisplay += "GVBM";
        if (isSubjectTeacher() && isAdvisor()) roleDisplay += "+";
        if (isAdvisor()) roleDisplay += "CVHT";
        roleDisplay += "]";
        
        cout << roleDisplay << " " << fullName
             << " | Mon: " << mon
             << " | Vai tro: " << getTeacherRoleLabel();
        if (!advisorClass.empty())
            cout << " | Lop chu nhiem: " << advisorClass;
        cout << endl;

        SetColor(WHITE);
    }

    string chuyenThanhChuoiFile() const
    {
        return to_string(id) + "|" + username + "|" + password + "|" + fullName + "|" + mon + "|" + getTeacherRoleLabel() + "|" + advisorClass;
    }

    void datLaiMatKhau()
    {
        string mkCu = inputLine("Nhap mat khau cu: ");
        if (mkCu != this->password)
        {
            SetColor(RED);
            cout << "Mat khau cu khong dung!\n";
            SetColor(WHITE);
            return;
        }
        string mkMoi = inputLine("Nhap mat khau moi: ");
        string mkMoiXacNhan = inputLine("Xac nhan mat khau moi: ");
        if (mkMoi != mkMoiXacNhan)
        {
            SetColor(RED);
            cout << "Xac nhan mat khau khong dung!\n";
            SetColor(WHITE);
            return;
        }
        this->password = mkMoi;
        SetColor(GREEN);
        cout << "Thay doi mat khau thanh cong!\n";
        SetColor(WHITE);
    }
    void xemSinhVien(const vector<SinhVien>& students, const vector<ClassInfo>& classes)
    {
        SetColor(LIGHT_CYAN);
        cout << "\n--- Danh sach sinh vien giang vien day ---\n";
        SetColor(WHITE);

        // Get classes this teacher is involved with (advisor or subject teacher)
        vector<string> classNames = layDanhSachLopDay(classes);

        for (const auto& hs : students)
        {
            bool show = false;
            if (hs.layMaGV() == this->id) show = true;
            if (!show && !classNames.empty() && find(classNames.begin(), classNames.end(), hs.getLop()) != classNames.end()) show = true;

            if (show) hs.hienThiThongTin();
        }
    }

    vector<string> layDanhSachLopDay(const vector<ClassInfo>& classes) const
    {
        vector<string> result;
        for (const auto& c : classes)
        {
            if (c.coVanHocTapId == id || find(c.danhSachGiangVien.begin(), c.danhSachGiangVien.end(), id) != c.danhSachGiangVien.end())
            {
                result.push_back(c.className);
            }
        }
        sort(result.begin(), result.end());
        result.erase(unique(result.begin(), result.end()), result.end());
        return result;
    }

    void xemKetQuaThi(const vector<SinhVien>& students, const vector<ClassInfo>& classes, const QuanLyThi& thi)
    {
        SetColor(LIGHT_CYAN);
        cout << "\n===== KET QUA THI =====\n";

        vector<string> classNames = layDanhSachLopDay(classes);
        bool hasAdvisorClass = false;
        for (const auto& c : classes)
        {
            if (c.coVanHocTapId == id)
            {
                hasAdvisorClass = true;
                break;
            }
        }
        bool isAdvisorRole = isAdvisor() || hasAdvisorClass;

        if (isAdvisorRole)
        {
            cout << "--- Giao vien co van hoc tap xem ket qua toan lop ---\n";
            if (classNames.empty())
            {
                cout << "Chua co lop duoc phan cong cho giao vien nay.\n";
            }
            for (const auto& hs : students)
            {
                if (!classNames.empty() && find(classNames.begin(), classNames.end(), hs.getLop()) == classNames.end())
                    continue;

                SetColor(LIGHT_YELLOW);
                cout << "\nSinh vien: " << hs.getFullName() << " | Lop: " << hs.getLop() << endl;

                for (const auto& kq : hs.getDanhSachKetQua())
                {
                    string tenBaiThi = "";
                    const deThi* de = thi.timDeThi(kq.maDe);
                    if (de)
                        tenBaiThi = de->tenBaiThi;
                    
                    SetColor(LIGHT_GREEN);
                    cout << "Mon: " << kq.mon << " | Ten bai thi: " << tenBaiThi
                        << " | Diem: " << kq.diem
                        << " | Thoi gian: " << kq.thoiGian << "s\n";
                }
            }
        }
        else
        {
            cout << "--- Giao vien bo mon xem ket qua mon " << mon << " ---\n";
            if (classNames.empty())
            {
                cout << "Chua duoc phan cong lop. Se hien thi ket qua mon cua tat ca sinh vien.\n";
            }
            for (const auto& hs : students)
            {
                if (!classNames.empty() && find(classNames.begin(), classNames.end(), hs.getLop()) == classNames.end())
                    continue;

                bool any = false;
                for (const auto& kq : hs.getDanhSachKetQua())
                {
                    if (kq.mon == mon && kq.loaiThi == 1)
                    {
                        if (!any)
                        {
                            SetColor(LIGHT_YELLOW);
                            cout << "\nSinh vien: " << hs.getFullName() << " | Lop: " << hs.getLop() << endl;
                            any = true;
                        }

                        SetColor(LIGHT_GREEN);
                        cout << "Mon: " << kq.mon
                            << " | Diem: " << kq.diem
                            << " | Thoi gian: " << kq.thoiGian << "s\n";
                    }
                }
            }
        }

        SetColor(WHITE);
    }



    void sapXepSinhVienTheoDiem(vector<SinhVien>& students, const QuanLyThi&)
    {
        struct HSScore
        {
            int id;
            string hoTen;
            string lop;
            double diem;
        };
        vector<HSScore> ds;

        for (const auto& hs : students)
        {
            double tong = 0;
            int dem = 0;
            for (const auto& kq : hs.getDanhSachKetQua())
            {
                if (kq.loaiThi == 1)
                {
                    tong += kq.diem;
                    dem++;
                }
            }
            if (dem > 0)
            {
                ds.push_back({ hs.getId(), hs.getFullName(), hs.getLop(), round((tong / dem) * 100.0) / 100.0 });
            }
        }

        if (ds.empty())
        {
            SetColor(YELLOW);
            cout << "Chua co sinh vien nao co diem thi chinh thuc!\n";
            SetColor(WHITE);
            return;
        }

        sort(ds.begin(), ds.end(), [](const HSScore& a, const HSScore& b)
        { return a.diem > b.diem; });
        
        SetColor(CYAN);
        cout << "\n===== BANG XEP HANG TOAN TRUONG (CHINH THUC) =====\n";
        SetColor(WHITE);
        for (int i = 0; i < (int)ds.size(); i++)
        {
            cout << i + 1 << ". " << ds[i].hoTen << " | Lop: " << ds[i].lop << " | Diem: " << fixed << setprecision(2) << ds[i].diem << "\n";
        }

        ofstream fout("ranking.txt");
        for (const auto& item : ds)
        {
            fout << item.id << '|' << item.hoTen << '|' << fixed << setprecision(2) << item.diem << '\n';
        }
        fout.close();
        SetColor(GREEN);
        cout << "\nDa luu bang xep hang toan truong vao ranking.txt\n";
        SetColor(WHITE);

        // Tự động xuất Excel bao_cao_thi.xlsx
        vector<RankItem> tmpRanking;
        for (const auto& item : ds)
            tmpRanking.push_back({ item.id, item.hoTen, item.diem });
        exportRankingVectorToExcel(tmpRanking);
    }

    void thongKe(const vector<SinhVien>& students)
    {
        double tong = 0;
        int dem = 0;
        for (const auto& hs : students)
        {
            if (hs.layMaGV() == this->id)
            {
                tong += hs.layDiem();
                dem++;
            }
        }
        if (dem)
            cout << "Diem TB: " << tong / dem << endl;
        else
            cout << "Khong co du lieu!\n";
    }

    void thongKeTheoMon(const vector<SinhVien>& students, const vector<ClassInfo>& classes)
    {
        string monTK = inputLine("Nhap ten mon muon thong ke: ");
        SetColor(CYAN);

        cout << "\n--- THONG KE DIEM MON "
            << monTK
            << " (CHINH THUC) ---\n";

        SetColor(WHITE);
        struct HSData
        {
            string ten;
            double diem;
        };
        vector<HSData> tk;
        vector<string> classNames = layDanhSachLopDay(classes);
        bool isAdvisorRole = isAdvisor();

        if (!isAdvisorRole && monTK != mon)
        {
            SetColor(RED);
            cout << "Chi giao vien bo mon co the xem diem mon: " << mon << "\n";
            SetColor(WHITE);
            return;
        }

        if (classNames.empty())
        {
            SetColor(YELLOW);
            cout << "Giao vien nay chua duoc phan cong lop nao.\n";
            SetColor(WHITE);
            return;
        }

        for (const auto& hs : students)
        {
            if (find(classNames.begin(), classNames.end(), hs.getLop()) == classNames.end())
                continue;

            double tongDiem = 0;
            int demDiem = 0;
            for (const auto& kq : hs.getDanhSachKetQua())
            {
                if (kq.mon == monTK && kq.loaiThi == 1)
                {
                    tongDiem += kq.diem;
                    demDiem++;
                }
            }
            if (demDiem > 0)
                tk.push_back({ hs.getFullName(), tongDiem / demDiem });
        }

        if (tk.empty())
        {
            SetColor(YELLOW);
            cout << "Chua co du lieu thi Chinh thuc mon nay!\n";
            SetColor(WHITE);
        }
        else
        {
            sort(tk.begin(), tk.end(), [](const HSData& a, const HSData& b)
                { return a.diem > b.diem; });
            for (const auto& data : tk)
                cout << "HS: " << data.ten << " | Diem mon: " << data.diem << "\n";
        }
    }

    void menu(vector<SinhVien>& students, const vector<ClassInfo>& classes, const vector<PhongThi>& examRooms, QuanLyThi& thi)
    {
        int choice;
        do
        {
            SetColor(CYAN);
            cout << "╔══════════════════════════════════════════════╗\n";
            cout << "║               MENU GIANG VIEN                ║\n";
            cout << "╠══════════════════════════════════════════════╣\n";

            SetColor(YELLOW);
            cout << "║  1. Xem danh sach sinh vien                  ║\n";
            cout << "║  2. Thong ke diem trung binh                 ║\n";
            cout << "║  3. Quan ly de thi                           ║\n";
            cout << "║  4. Thong ke theo mon                        ║\n";
            cout << "║  5. Xem ket qua thi                          ║\n";
            cout << "║  6. Xem bang xep hang toan truong           ║\n";
            cout << "║  7. Doi mat khau                             ║\n";
            cout << "║  0. Dang xuat                                ║\n";

            SetColor(CYAN);
            cout << "╚══════════════════════════════════════════════╝\n";

            SetColor(GREEN);
            choice = nhapSo<int>("Chon chuc nang: ");

            SetColor(WHITE);

            switch (choice)
            {
            case 1:
                SetColor(LIGHT_CYAN);
                xemSinhVien(students, classes);
                SetColor(WHITE);
                break;

            case 2:
                SetColor(LIGHT_YELLOW);
                thongKe(students);
                SetColor(WHITE);
                break;

            case 3:
                SetColor(LIGHT_MAGENTA);
                thi.menu(this->id, this->mon, classes, examRooms);
                SetColor(WHITE);
                break;

            case 4:
                SetColor(LIGHT_BLUE);
                thongKeTheoMon(students, classes);
                SetColor(WHITE);
                break;

            case 5:
                SetColor(LIGHT_GREEN);
                xemKetQuaThi(students, classes, thi);
                SetColor(WHITE);
                break;

            case 6:
                SetColor(YELLOW);
                sapXepSinhVienTheoDiem(students, thi);
                SetColor(WHITE);
                break;

            case 7:
                datLaiMatKhau();
                SetColor(GREEN);
                cout << "Doi mat khau thanh cong!\n";
                SetColor(WHITE);
                break;

            case 0:
                SetColor(LIGHT_RED);
                cout << "Dang xuat thanh cong!\n";
                SetColor(WHITE);
                break;

            default:
                SetColor(RED);
                cout << "Lua chon khong hop le!\n";
                SetColor(WHITE);
            }

        } while (choice != 0);
    }
};

// ========================== QUAN LY USER ==========================
class QuanLyNguoiDung
{
private:
    vector<Person*> users;

public:
    void them(Person* p) { users.push_back(p); }

    Person* dangNhap()
    {
        string u, p;
        cout << "User: ";
        cin >> u;
        cout << "Pass: ";
        cin >> p;
        cin.ignore();

        for (auto user : users)
        {
            if (user->getUsername() == u && user->getPassword() == p)
            {
                return user;
            }
        }
        return NULL;
    }

    vector<Person*>& layDanhSach() { return users; }
};
//==================== MAIN ====================
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    // Adjust working directory if running from output folder
    string dataPath = ".";
    if (!filesystem::exists("sinhvien_data.txt") && !filesystem::exists("giangvien.txt")) {
        if (filesystem::exists("../sinhvien_data.txt") || filesystem::exists("../giangvien.txt")) {
            dataPath = "..";
        }
    }

    AdminSystem adminSystem;
    QuanLyNguoiDung ql;
    QuanLyThi thi;

    if (filesystem::exists(dataPath + "/sinhvien_data.txt"))
        adminSystem.loadStudents(dataPath + "/sinhvien_data.txt");
    else if (filesystem::exists(dataPath + "/sinhvien_report.txt"))
        adminSystem.loadStudentsFromReadableFormat(dataPath + "/sinhvien_report.txt");

    adminSystem.loadTeachers(dataPath + "/giangvien.txt");
    adminSystem.loadClasses(dataPath + "/lophoc.txt");
    adminSystem.loadRooms(dataPath + "/phongthi.txt");
    adminSystem.loadRanking();
    adminSystem.loadKetQua();

    thi.loadDeThi();

    int vaiTro;

    SetColor(LIGHT_CYAN);

    cout << R"(

  /\_/\     ████████╗███████╗███████╗████████╗██████╗ ██████╗  ██████╗ 
 ( >.< )    ╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝██╔══██╗██╔══██╗██╔═══██╗
  > ^ <        ██║   █████╗  ███████╗   ██║   ██████╔╝██████╔╝██║   ██║
 /     \       ██║   ██╔══╝  ╚════██║   ██║   ██╔═══╝ ██╔══██╗██║   ██║
               ██║   ███████╗███████║   ██║   ██║     ██║  ██║╚██████╔╝ 
               ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚═╝     ╚═╝  ╚═╝ ╚═════╝ 

)" << endl;

    do
    {
        drawLine(11);
        SetColor(LIGHT_GREEN);
        cout << "        HE THONG QUAN LY THI TRAC NGHIEM\n";
        drawLine(11);
        SetColor(LIGHT_BLUE);
        cout << "        [1] DANG NHAP ADMIN\n";
        SetColor(LIGHT_YELLOW);
        cout << "        [2] DANG NHAP GIANG VIEN\n";
        SetColor(LIGHT_MAGENTA);
        cout << "        [3] DANG NHAP SINH VIEN\n";
        SetColor(LIGHT_RED);
        cout << "        [0] THOAT CHUONG TRINH\n";
        SetColor(WHITE);

        vaiTro = nhapSo<int>("\nNhap lua chon: ");

        if (vaiTro == 1)
        {
            if (adminSystem.loginAdmin())
            {
                adminSystem.adminMenu(thi);
                adminSystem.saveData();
                thi.saveDeThi();
            }
        }
        else if (vaiTro == 2)
        {
            ql.layDanhSach().clear();
            for (auto& t : adminSystem.getTeachers())
            {
                ql.them(&t);
            }

            Person* user = ql.dangNhap();
            if (user != nullptr)
            {
                SetColor(GREEN);
                cout << "Dang nhap thanh cong!\n";
                SetColor(WHITE);

                GiangVien* gv = dynamic_cast<GiangVien*>(user);
                if (gv != nullptr)
                {
                    gv->menu(adminSystem.getStudents(), adminSystem.getClasses(), adminSystem.getRooms(), thi);
                    adminSystem.saveData();
                    thi.saveDeThi();
                }
            }
            else
            {
                SetColor(RED);
                cout << "Dang nhap that bai!\n";
                SetColor(WHITE);
            }
        }
        else if (vaiTro == 3)
        {
            ql.layDanhSach().clear();
            for (auto& hs : adminSystem.getStudents())
            {
                ql.them(&hs);
            }

            Person* user = ql.dangNhap();
            if (user != nullptr)
            {
                SetColor(GREEN);
                cout << "Dang nhap thanh cong!\n";
                SetColor(WHITE);

                SinhVien* hs = dynamic_cast<SinhVien*>(user);
                if (hs != nullptr)
                {
                    hs->menu(thi, adminSystem.getStudents(), [&adminSystem]() {
                        adminSystem.rebuildAndSaveRanking();
                    });
                    adminSystem.saveData();
                    thi.saveDeThi();
                }
            }
            else
            {
                SetColor(RED);
                cout << "Dang nhap that bai!\n";
                SetColor(WHITE);
            }
        }
    } while (vaiTro != 0);

    return 0;
}
