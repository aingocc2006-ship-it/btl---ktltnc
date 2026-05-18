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
    const vector<deThi>& getMaDe() const { return maDe; }
    QuanLyThi()
    {
        maDe = {};
        tenMon = "Chua dat ten";
    }

    void thietLapDeThi(int teacherId, string monGiangVien, const vector<ClassInfo>& classes, const vector<PhongThi>& examRooms)
    {
        do
        {
            de.loaiThi = nhapSo<int>("Loai bai thi (1. Chinh thuc | 2. On tap): ");
        } while (de.loaiThi != 1 && de.loaiThi != 2);

        de.tenMon = monGiangVien;
        do {
            de.id = nhapSo<int>("Ma de: ");
            bool trung = false;
            for (const auto& d : maDe)
            {
                if (d.id == de.id)
                {
                    trung = true;
                    break;
                }
            }
            if (trung)
            {
                SetColor(RED);
                cout << "Ma de da ton tai! Nhap lai.\n";
                SetColor(WHITE);
            }
            else
                break;
        } while (true);

        de.thoiGianLamBai = nhapSo<int>("Thoi gian lam bai (giay): ");

        if (de.loaiThi == 1)
        {
            if (examRooms.empty())
            {
                SetColor(RED);
                cout << "Chua co phong thi nao! Admin can tao phong thi truoc.\n";
                SetColor(WHITE);
                return;
            }
            bool validRoom = false;
            do
            {
                de.phongThiId = nhapSo<int>("Nhap ID phong thi: ");
                for (const auto& r : examRooms)
                {
                    if (r.id == de.phongThiId)
                    {
                        validRoom = true;
                        break;
                    }
                }
                if (!validRoom)
                {
                    SetColor(RED);
                    cout << "Phong thi ID " << de.phongThiId << " khong ton tai! Nhap lai.\n";
                    SetColor(WHITE);
                }
            } while (!validRoom);
        }
        else
        {
            de.phongThiId = 0;
            de.batDau = 0;
            de.ketThuc = 0;

            cout << "Gioi han so lan on tap? (0 = khong gioi han): ";
            int soLan = nhapSo<int>("");
            de.soLanOnTap = (soLan <= 0) ? -1 : soLan;

            if (de.soLanOnTap == -1)
            {
                SetColor(LIGHT_GREEN);
                cout << "De on tap khong gioi han so lan.\n";
                SetColor(WHITE);
            }
            else
            {
                SetColor(LIGHT_YELLOW);
                cout << "De on tap gioi han " << de.soLanOnTap << " lan.\n";
                SetColor(WHITE);
            }
        }

        if (de.loaiThi == 1)
        {
            do
            {
                string batDau = inputLine("Nhap ngay gio bat dau (YYYY-MM-DD HH:MM): ");
                de.batDau = parseDateTime(batDau);
                if (de.batDau == 0)
                    cout << "Dinh dang khong hop le. Thu lai!\n";
            } while (de.batDau == 0);

            do
            {
                string ketThuc = inputLine("Nhap ngay gio ket thuc (YYYY-MM-DD HH:MM): ");
                de.ketThuc = parseDateTime(ketThuc);
                if (de.ketThuc == 0 || de.ketThuc <= de.batDau)
                {
                    cout << "Khong hop le hoac ket thuc truoc bat dau. Thu lai!\n";
                    de.ketThuc = 0;
                }
            } while (de.ketThuc == 0);
        }
        else
        {
            de.batDau = 0;
            de.ketThuc = 0;
        }

        de.dsLopDuocThi.clear();
        SetColor(LIGHT_CYAN);
        cout << "\nDang tu dong phan quyen thi cho cac lop cung chuyen nganh...\n";
        SetColor(WHITE);

        bool hasClass = false;

        for (const auto& c : classes)
        {
            bool duocGiangDay = false;
            for (int gvId : c.danhSachGiangVien)
            {
                if (gvId == teacherId)
                {
                    duocGiangDay = true;
                    break;
                }
            }
            if (!duocGiangDay && !monGiangVien.empty() && !c.className.empty())
            {
                if (c.className.find(monGiangVien) == 0 ||
                    c.className.rfind(monGiangVien, 0) == 0)
                {
                    duocGiangDay = true;
                }
            }

            if (duocGiangDay)
            {
                de.dsLopDuocThi.push_back(c.className);
                SetColor(GREEN);
                cout << " + Da cap quyen cho lop: " << c.className << "\n";
                SetColor(WHITE);
                hasClass = true;
            }
        }

        if (!hasClass)
        {
            SetColor(YELLOW);
            cout << "Khong tim thay lop nao phu trach. De thi se mo cho toan truong (ALL).\n";
            SetColor(WHITE);
            de.dsLopDuocThi.clear();
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
        int idDe = nhapSo<int>("Nhap Ma De can khoa/mo: ");
        for (auto& d : maDe)
        {
            if (d.id == idDe)
            {
                d.locked = !d.locked;
                cout << "De thi " << d.id << " da " << (d.locked ? "duoc khoa" : "duoc mo") << ".\n";
                saveDeThi();
                return;
            }
        }
        cout << "Khong tim thay de thi!\n";
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
        for (const auto& d : maDe)
        {
            cout << "Ma de: " << d.id
                << " | Mon: " << d.tenMon
                << " | Phong: " << d.phongThiId
                << " | Lich: " << d.getSchedule()
                << " | Trang thai: " << (d.locked ? "Khoa" : (d.isAvailable() ? "Dang mo" : "Chua den gio/da ket thuc"))
                << "\n";
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
        int idDe = nhapSo<int>("Nhap Ma De can sua: ");
        for (auto& d : maDe)
        {
            if (d.id == idDe)
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
        int idDe = nhapSo<int>("Nhap Ma De can xoa cau hoi: ");
        for (auto& d : maDe)
        {
            if (d.id == idDe)
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
            cout << "1. Tao de thi moi\n2. Sua cau hoi trong de\n3. Xoa cau hoi trong de\n4. Xem danh sach de thi\n5. Khoa/Mo de thi\n6. Xem lich thi\n0. Quay lai\n";
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
                    string soLan = "";
                    if (d.loaiThi == 2)
                    {
                        soLan = (d.soLanOnTap == -1)
                            ? " | So lan: Khong gioi han"
                            : " | So lan: " + to_string(d.soLanOnTap);
                    }
                    cout << "Ma de: " << d.id
                        << " | Mon: " << d.tenMon
                        << " " << loai
                        << soLan
                        << " | Thoi gian: " << d.thoiGianLamBai << "s"
                        << " | Trang thai: " << (d.locked ? "Khoa" : "Mo")
                        << endl;
                }
                break;
            case 5:
                khoaMoDeThi();
                break;
            case 6:
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
        fout << de.id << '|' << de.tenMon << '|' << de.thoiGianLamBai << '|'
            << de.loaiThi << '|' << de.danhSachCauHoi.size()
            << '|' << de.phongThiId << '|' << formatDateTime(de.batDau)
            << '|' << formatDateTime(de.ketThuc) << '|' << (de.locked ? 1 : 0)
            << '|' << de.soLanOnTap << '|';

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
    string line;
    while (getline(fin, line))
    {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> tokens; string token;
        while (getline(ss, token, '|')) tokens.push_back(token);

        if (tokens.size() < 7) continue;

        deThi de;
        de.id = safeStoi(tokens[0]);
        de.tenMon = tokens[1];
        de.thoiGianLamBai = safeStoi(tokens[2]);
        de.loaiThi = safeStoi(tokens[3]);
        int soCau = safeStoi(tokens[4]);
        de.phongThiId = safeStoi(tokens[5]);

        string dsLopStr = "";

        if (tokens.size() >= 11) {
            de.batDau = tokens[6].empty() ? 0 : parseDateTime(tokens[6]);
            de.ketThuc = tokens[7].empty() ? 0 : parseDateTime(tokens[7]);
            de.locked = (tokens[8] == "1");
            de.soLanOnTap = safeStoi(tokens[9], -1);
            dsLopStr = tokens[10];
        }
        else if (tokens.size() == 10) {
            de.batDau = tokens[6].empty() ? 0 : parseDateTime(tokens[6]);
            de.ketThuc = tokens[7].empty() ? 0 : parseDateTime(tokens[7]);
            de.locked = (tokens[8] == "1");
            de.soLanOnTap = -1;
            dsLopStr = tokens[9];
        }
        else {
            de.batDau = 0; de.ketThuc = 0; de.locked = false; de.soLanOnTap = -1;
            if (tokens.size() > 6) dsLopStr = tokens[6];
        }

        if (dsLopStr != "ALL" && !dsLopStr.empty())
        {
            stringstream ssLop(dsLopStr);
            string tenLop;
            while (getline(ssLop, tenLop, ',')) de.dsLopDuocThi.push_back(tenLop);
        }

        for (int i = 0; i < soCau; i++)
        {
            if (!getline(fin, line)) break;
            if (!line.empty() && line.back() == '\r') line.pop_back();
            stringstream s2(line);
            vector<string> chTk; string c;
            while (getline(s2, c, '|')) chTk.push_back(c);
            if (chTk.size() < 7) continue;

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
    fin.close();
}// ========================== QUAN LY THI ==========================
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
    const vector<deThi>& getMaDe() const { return maDe; }
    QuanLyThi()
    {
        maDe = {};
        tenMon = "Chua dat ten";
    }

    void thietLapDeThi(int teacherId, string monGiangVien, const vector<ClassInfo>& classes, const vector<PhongThi>& examRooms)
    {
        do
        {
            de.loaiThi = nhapSo<int>("Loai bai thi (1. Chinh thuc | 2. On tap): ");
        } while (de.loaiThi != 1 && de.loaiThi != 2);

        de.tenMon = monGiangVien;
        do {
            de.id = nhapSo<int>("Ma de: ");
            bool trung = false;
            for (const auto& d : maDe)
            {
                if (d.id == de.id)
                {
                    trung = true;
                    break;
                }
            }
            if (trung)
            {
                SetColor(RED);
                cout << "Ma de da ton tai! Nhap lai.\n";
                SetColor(WHITE);
            }
            else
                break;
        } while (true);

        de.thoiGianLamBai = nhapSo<int>("Thoi gian lam bai (giay): ");

        if (de.loaiThi == 1)
        {
            if (examRooms.empty())
            {
                SetColor(RED);
                cout << "Chua co phong thi nao! Admin can tao phong thi truoc.\n";
                SetColor(WHITE);
                return;
            }
            bool validRoom = false;
            do
            {
                de.phongThiId = nhapSo<int>("Nhap ID phong thi: ");
                for (const auto& r : examRooms)
                {
                    if (r.id == de.phongThiId)
                    {
                        validRoom = true;
                        break;
                    }
                }
                if (!validRoom)
                {
                    SetColor(RED);
                    cout << "Phong thi ID " << de.phongThiId << " khong ton tai! Nhap lai.\n";
                    SetColor(WHITE);
                }
            } while (!validRoom);
        }
        else
        {
            de.phongThiId = 0;
            de.batDau = 0;
            de.ketThuc = 0;

            cout << "Gioi han so lan on tap? (0 = khong gioi han): ";
            int soLan = nhapSo<int>("");
            de.soLanOnTap = (soLan <= 0) ? -1 : soLan;

            if (de.soLanOnTap == -1)
            {
                SetColor(LIGHT_GREEN);
                cout << "De on tap khong gioi han so lan.\n";
                SetColor(WHITE);
            }
            else
            {
                SetColor(LIGHT_YELLOW);
                cout << "De on tap gioi han " << de.soLanOnTap << " lan.\n";
                SetColor(WHITE);
            }
        }

        if (de.loaiThi == 1)
        {
            do
            {
                string batDau = inputLine("Nhap ngay gio bat dau (YYYY-MM-DD HH:MM): ");
                de.batDau = parseDateTime(batDau);
                if (de.batDau == 0)
                    cout << "Dinh dang khong hop le. Thu lai!\n";
            } while (de.batDau == 0);

            do
            {
                string ketThuc = inputLine("Nhap ngay gio ket thuc (YYYY-MM-DD HH:MM): ");
                de.ketThuc = parseDateTime(ketThuc);
                if (de.ketThuc == 0 || de.ketThuc <= de.batDau)
                {
                    cout << "Khong hop le hoac ket thuc truoc bat dau. Thu lai!\n";
                    de.ketThuc = 0;
                }
            } while (de.ketThuc == 0);
        }
        else
        {
            de.batDau = 0;
            de.ketThuc = 0;
        }

        de.dsLopDuocThi.clear();
        SetColor(LIGHT_CYAN);
        cout << "\nDang tu dong phan quyen thi cho cac lop cung chuyen nganh...\n";
        SetColor(WHITE);

        bool hasClass = false;

        for (const auto& c : classes)
        {
            bool duocGiangDay = false;
            for (int gvId : c.danhSachGiangVien)
            {
                if (gvId == teacherId)
                {
                    duocGiangDay = true;
                    break;
                }
            }
            if (!duocGiangDay && !monGiangVien.empty() && !c.className.empty())
            {
                if (c.className.find(monGiangVien) == 0 ||
                    c.className.rfind(monGiangVien, 0) == 0)
                {
                    duocGiangDay = true;
                }
            }

            if (duocGiangDay)
            {
                de.dsLopDuocThi.push_back(c.className);
                SetColor(GREEN);
                cout << " + Da cap quyen cho lop: " << c.className << "\n";
                SetColor(WHITE);
                hasClass = true;
            }
        }

        if (!hasClass)
        {
            SetColor(YELLOW);
            cout << "Khong tim thay lop nao phu trach. De thi se mo cho toan truong (ALL).\n";
            SetColor(WHITE);
            de.dsLopDuocThi.clear();
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
        int idDe = nhapSo<int>("Nhap Ma De can khoa/mo: ");
        for (auto& d : maDe)
        {
            if (d.id == idDe)
            {
                d.locked = !d.locked;
                cout << "De thi " << d.id << " da " << (d.locked ? "duoc khoa" : "duoc mo") << ".\n";
                saveDeThi();
                return;
            }
        }
        cout << "Khong tim thay de thi!\n";
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
        for (const auto& d : maDe)
        {
            cout << "Ma de: " << d.id
                << " | Mon: " << d.tenMon
                << " | Phong: " << d.phongThiId
                << " | Lich: " << d.getSchedule()
                << " | Trang thai: " << (d.locked ? "Khoa" : (d.isAvailable() ? "Dang mo" : "Chua den gio/da ket thuc"))
                << "\n";
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
        int idDe = nhapSo<int>("Nhap Ma De can sua: ");
        for (auto& d : maDe)
        {
            if (d.id == idDe)
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
        int idDe = nhapSo<int>("Nhap Ma De can xoa cau hoi: ");
        for (auto& d : maDe)
        {
            if (d.id == idDe)
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
            cout << "1. Tao de thi moi\n2. Sua cau hoi trong de\n3. Xoa cau hoi trong de\n4. Xem danh sach de thi\n5. Khoa/Mo de thi\n6. Xem lich thi\n0. Quay lai\n";
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
                    string soLan = "";
                    if (d.loaiThi == 2)
                    {
                        soLan = (d.soLanOnTap == -1)
                            ? " | So lan: Khong gioi han"
                            : " | So lan: " + to_string(d.soLanOnTap);
                    }
                    cout << "Ma de: " << d.id
                        << " | Mon: " << d.tenMon
                        << " " << loai
                        << soLan
                        << " | Thoi gian: " << d.thoiGianLamBai << "s"
                        << " | Trang thai: " << (d.locked ? "Khoa" : "Mo")
                        << endl;
                }
                break;
            case 5:
                khoaMoDeThi();
                break;
            case 6:
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
        fout << de.id << '|' << de.tenMon << '|' << de.thoiGianLamBai << '|'
            << de.loaiThi << '|' << de.danhSachCauHoi.size()
            << '|' << de.phongThiId << '|' << formatDateTime(de.batDau)
            << '|' << formatDateTime(de.ketThuc) << '|' << (de.locked ? 1 : 0)
            << '|' << de.soLanOnTap << '|';

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
    string line;
    while (getline(fin, line))
    {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> tokens; string token;
        while (getline(ss, token, '|')) tokens.push_back(token);

        if (tokens.size() < 7) continue;

        deThi de;
        de.id = safeStoi(tokens[0]);
        de.tenMon = tokens[1];
        de.thoiGianLamBai = safeStoi(tokens[2]);
        de.loaiThi = safeStoi(tokens[3]);
        int soCau = safeStoi(tokens[4]);
        de.phongThiId = safeStoi(tokens[5]);

        string dsLopStr = "";

        if (tokens.size() >= 11) {
            de.batDau = tokens[6].empty() ? 0 : parseDateTime(tokens[6]);
            de.ketThuc = tokens[7].empty() ? 0 : parseDateTime(tokens[7]);
            de.locked = (tokens[8] == "1");
            de.soLanOnTap = safeStoi(tokens[9], -1);
            dsLopStr = tokens[10];
        }
        else if (tokens.size() == 10) {
            de.batDau = tokens[6].empty() ? 0 : parseDateTime(tokens[6]);
            de.ketThuc = tokens[7].empty() ? 0 : parseDateTime(tokens[7]);
            de.locked = (tokens[8] == "1");
            de.soLanOnTap = -1;
            dsLopStr = tokens[9];
        }
        else {
            de.batDau = 0; de.ketThuc = 0; de.locked = false; de.soLanOnTap = -1;
            if (tokens.size() > 6) dsLopStr = tokens[6];
        }

        if (dsLopStr != "ALL" && !dsLopStr.empty())
        {
            stringstream ssLop(dsLopStr);
            string tenLop;
            while (getline(ssLop, tenLop, ',')) de.dsLopDuocThi.push_back(tenLop);
        }

        for (int i = 0; i < soCau; i++)
        {
            if (!getline(fin, line)) break;
            if (!line.empty() && line.back() == '\r') line.pop_back();
            stringstream s2(line);
            vector<string> chTk; string c;
            while (getline(s2, c, '|')) chTk.push_back(c);
            if (chTk.size() < 7) continue;

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
    fin.close();

    
}
