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
// ========================== STUDENT ==========================
class SinhVien : public Person
{
private:
    string lop;
    string gioitinh;
    string birthDay;
    string sdt;
    vector<KetQua> danhSachKetQua;
    double diemTB;
    int teacherId;

public:
    SinhVien() : Person()
    {
        lop = "";
        gioitinh = "";
        birthDay = "";
        sdt = "";
        diemTB = 0.0;
        teacherId = 0;
    }

    SinhVien(int id, string userN, string passW, string ten,
        string lop, string gioitinh, string birthDay, string sdt, double diemTB, int teacherId)
        : Person(id, userN, passW, ten), lop(lop), gioitinh(gioitinh), birthDay(birthDay), sdt(sdt), diemTB(diemTB), teacherId(teacherId)
    {
    }

    bool kiemTraLopDuocThi(const deThi& de) const
    {
        if (de.dsLopDuocThi.empty())
            return true;
        for (const string& l : de.dsLopDuocThi)
        {
            if (l == this->lop)
                return true;
        }
        return false;
    }

    string getRole() const override { return "SinhVien"; }
    string getLop() const { return lop; }
    string getGioiTinh() const { return gioitinh; }
    string getBirthDay() const { return birthDay; }
    string getSdt() const { return sdt; }
    int layMaGV() const { return teacherId; }
    double layDiem() const { return diemTB; }

    void datDiem(double d) { diemTB = d; }
    void setLop(const string& lop) { this->lop = lop; }
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
            << "[HS] " << fullName << " | Lop: " << lop << " | Gioi tinh: " << gioitinh
            << " | Ngay sinh: " << birthDay << " | SDT: " << sdt << " | Diem TB: " << diemTB
            << " | TeacherID: " << teacherId << endl;
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
                string loaiStr = (de.loaiThi == 1) ? "[CHINH THUC (1 lan)]" : "[ON TAP (n lan)]";
                cout << "Ma de: " << de.id << " | Mon: " << de.tenMon << " " << loaiStr << " | Thoi gian: " << de.thoiGianLamBai << "s\n";
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
        int maDe = nhapSo<int>("Nhap ma de can xem: ");
        for (auto& de : thi.getMaDe())
        {
            if (de.id == maDe && de.loaiThi == loaiBai)
            {
                cout << "\n===== THONG TIN CHI TIET =====\n";
                cout << "Mon: " << de.tenMon << endl;
                cout << "So cau hoi: " << de.danhSachCauHoi.size() << endl;
                cout << "Thoi gian: " << de.thoiGianLamBai << " giay\n";
                cout << "Trang thai: " << (de.locked ? "Dang khoa" : "Dang mo") << endl;
                if (de.loaiThi == 1)
                    cout << "Lich thi: " << de.getSchedule() << endl;
                else
                    cout << "So lan lam toi da: " << (de.soLanOnTap == -1 ? "Khong gioi han" : to_string(de.soLanOnTap)) << endl;
                return;
            }
        }
        cout << "Khong tim thay ma de nay trong danh sach!\n";
    }

    double traLoiCauHoi(vector<CauHoi> danhSachCauHoi, time_t start, int thoiGian) const
    {
        int n;
        double diemThi = 0;
        for (auto& ch : danhSachCauHoi)
        {
            time_t now = time(0);
            if (difftime(now, start) >= thoiGian)
            {
                cout << "Het gio lam bai!\n";
                break;
            }
            cout << "Cau " << ch.id << ". " << ch.noiDung << endl;
            for (int i = 0; i < 4; i++)
            {
                cout << i + 1 << ". " << ch.dapAn[i] << endl;
            }
            n = nhapSo<int>("Dap an ban chon: ");
            if (n == ch.dapAnDung)
            {
                diemThi += 10.0 / danhSachCauHoi.size();
            }
        }
        return diemThi;
    }

    void lamBai(QuanLyThi& thi, int loaiBai)
    {
        int chonDe = nhapSo<int>("Nhap Ma De muon lam: ");
        deThi* deHienTai = nullptr;
        for (auto& de : thi.getMaDe())
        {
            if (de.id == chonDe && de.loaiThi == loaiBai)
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
            cout << "Chua den gio hoac da qua han. Lich thi: " << deHienTai->getSchedule() << "\n";
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

        cout << "\n=== BAT DAU LAM BAI: " << deHienTai->tenMon << " ===\n";
        SetColor(WHITE);
        time_t start = time(0);

        double diemThi = traLoiCauHoi(deHienTai->danhSachCauHoi, start, deHienTai->thoiGianLamBai);

        int thoiGianThucTe = (int)difftime(time(0), start);
        if (thoiGianThucTe == 0) thoiGianThucTe = 1;

        SetColor(GREEN);
        cout << "=> Nop bai thanh cong! Diem: " << diemThi << " | Thoi gian: " << thoiGianThucTe << "s\n";
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

    void hienThiDanhSachKetQua() const
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
            cout << "Ma de: " << kq.maDe << " " << loai << " | Mon: " << kq.mon << " | Diem: " << kq.diem << " | Thoi gian: " << kq.thoiGian << "s\n";
        }
    }

    void thongKeDiemTB()
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
        cout << "\n===== THONG KE DIEM =====\n";
        if (dem == 0)
        {
            cout << "Ban chua co diem bai thi Chinh thuc nao!\n";
        }
        else
        {
            cout << "Tong so bai da thi (Chinh thuc): " << dem << "\n";
            cout << "Diem TB tich luy: " << tong / dem << "\n";
        }
    }

    void xemBangXepHang(const vector<SinhVien>& dsSinhVien)
    {
        SetColor(CYAN);
        cout << "\n===== BANG XEP HANG TOAN TRUONG (CHINH THUC) =====\n";
        SetColor(WHITE);
        struct HSScore {
            string fullName;
            double score;
        };
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

        for (int i = 0; i < bxh.size(); i++)
        {
            if (bxh[i].fullName == this->fullName) SetColor(LIGHT_GREEN);
            cout << i + 1 << ". Ho ten: " << bxh[i].fullName << " | Diem TB: " << bxh[i].score << '\n';
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
        return to_string(id) + "|" + username + "|" + password + "|" + fullName + "|" + lop + "|" + to_string(diemTB) + "|" + to_string(teacherId);
    }

    void menu(QuanLyThi& thi, const vector<SinhVien>& dsSinhVienToanTruong)
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
            cout << "║  2. Xem danh sach bai thi (Chinh thuc)       ║\n";
            cout << "║  3. Xem thong tin bai thi (Chinh thuc)       ║\n";
            cout << "║  4. Lam bai thi (Chinh thuc)                 ║\n";
            cout << "║  5. Xem lich su thi                          ║\n";
            cout << "║  6. Thong ke diem trung binh                 ║\n";
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
            case 4: lamBai(thi, 1); break;
            case 5: hienThiDanhSachKetQua(); break;
            case 6: thongKeDiemTB(); break;
            case 7: xemBangXepHang(dsSinhVienToanTruong); break;
            case 8: xemDanhSachDe(thi, 2); break;
            case 9: xemThongTinDe(thi, 2); break;
            case 10: lamBai(thi, 2); break;
            case 11: datLaiMatKhau(); break;
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

// ========================== TEACHER ==========================
class GiangVien : public Person
{
private:
    string mon;

public:
    void setMon(const string& m) { mon = m; }
    string getMon() const { return mon; }

    GiangVien() : Person() {};
    GiangVien(int id, string u, string p, string ten, string mon) : Person(id, u, p, ten), mon(mon) {}

    string getRole() const override
    {
        return "GiangVien";
    }

    void hienThiThongTin() const
    {
        SetColor(LIGHT_YELLOW);
        cout << "[GV] " << fullName << " | Mon: " << mon << endl;

        SetColor(WHITE);
    }

    string chuyenThanhChuoiFile() const
    {
        return to_string(id) + "|" + username + "|" + password + "|" + fullName + "|" + mon;
    }

    void datLaiMatKhau()
    {
        string mk;

        SetColor(LIGHT_CYAN);
        cout << "Nhap MK moi: ";

        SetColor(WHITE);
        getline(cin, mk);

        password = mk;

        SetColor(LIGHT_GREEN);
        cout << "Doi mat khau thanh cong!\n";

        SetColor(WHITE);
    }
    void xemSinhVien(const vector<SinhVien>& students)
    {
        SetColor(LIGHT_CYAN);
        cout << "\n--- sinh vien LOP CHU NHIEM ---\n";
        SetColor(WHITE);
        for (const auto& hs : students)
        {
            if (hs.layMaGV() == this->id)
                hs.hienThiThongTin();
        }
    }

    void xemKetQuaThi(const vector<SinhVien>& students)
    {
        SetColor(LIGHT_CYAN);
        cout << "\n===== KET QUA THI LOP CHU NHIEM =====\n";

        for (const auto& hs : students)
        {
            if (hs.layMaGV() == this->id)
            {

                SetColor(LIGHT_YELLOW);
                cout << "\nSinh vien: " << hs.getFullName() << endl;

                for (const auto& kq : hs.getDanhSachKetQua())
                {

                    SetColor(LIGHT_GREEN);
                    cout << "Mon: " << kq.mon
                        << " | Diem: " << kq.diem
                        << " | Thoi gian: " << kq.thoiGian << "s\n";
                }
            }
        }

        SetColor(WHITE);
    }

    void sapXepSinhVienTheoDiem(vector<SinhVien>& students)
    {
        vector<SinhVien> ds;
        for (auto hs : students)
        {
            if (hs.layMaGV() == this->id)
            {
                ds.push_back(hs);
            }
        }
        sort(ds.begin(), ds.end(), [](SinhVien a, SinhVien b)
            { return a.layDiem() > b.layDiem(); });
        SetColor(CYAN);
        cout << "\n===== SAP XEP sinh vien THEO DIEM =====\n";
        SetColor(WHITE);
        for (auto& hs : ds)
        {
            cout << hs.getFullName() << " | Diem TB: " << hs.layDiem() << endl;
        }
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

    void thongKeTheoMon(const vector<SinhVien>& students)
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

        for (const auto& hs : students)
        {
            if (hs.layMaGV() == this->id)
            {
                double maxDiem = -1;
                for (const auto& kq : hs.getDanhSachKetQua())
                {
                    if (kq.mon == monTK && kq.loaiThi == 1)
                        maxDiem = max(maxDiem, kq.diem);
                }
                if (maxDiem >= 0)
                    tk.push_back({ hs.getFullName(), maxDiem });
            }
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
            cout << "║  6. Sap xep sinh vien theo diem              ║\n";
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
                xemSinhVien(students);
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
                thongKeTheoMon(students);
                SetColor(WHITE);
                break;

            case 5:
                SetColor(LIGHT_GREEN);
                xemKetQuaThi(students);
                SetColor(WHITE);
                break;

            case 6:
                SetColor(YELLOW);
                sapXepSinhVienTheoDiem(students);
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

// ========================== RANK ITEM ==========================
struct RankItem
{
    int id;
    string fullName;
    double score;
};

