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

    
} if (beginsWith(nextLine, "A.") || beginsWith(nextLine, "B.") || beginsWith(nextLine, "C.") || beginsWith(nextLine, "D.") || beginsWith(nextLine, "Dap an dung:") || beginsWith(nextLine, "[Cau"))
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
