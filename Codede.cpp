#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <commdlg.h>
#include <algorithm>
#include <limits>
#include <cctype>
#include <ctime>
#include <filesystem>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

#define BLACK         0
#define BLUE          1
#define GREEN         2
#define CYAN          3
#define RED           4
#define MAGENTA       5
#define YELLOW        6
#define WHITE         7

#define LIGHT_BLUE    9
#define LIGHT_GREEN   10
#define LIGHT_CYAN    11
#define LIGHT_RED     12
#define LIGHT_MAGENTA 13
#define LIGHT_YELLOW  14
#define BRIGHT_WHITE  15

void VeKhungTieuDe(string tieuDe, int mau) {
    int len = tieuDe.length() + 4;
    SetColor(mau);
    cout << "\n╔";
    for (int i = 0; i < len; i++) cout << "═";
    cout << "╗" << endl;
    cout << "║  ";
    SetColor(BRIGHT_WHITE);
    cout << tieuDe;
    SetColor(mau);
    cout << "  ║" << endl;
    cout << "╚";
    for (int i = 0; i < len; i++) cout << "═";
    cout << "╝" << endl;
    SetColor(WHITE);
}
//======Tai file======
string taiFile() {
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

    if (GetOpenFileNameW(&ofn)) {
        // Convert wide string to narrow string
        char buffer[260];
        WideCharToMultiByte(CP_UTF8, 0, szFile, -1, buffer, sizeof(buffer), NULL, NULL);
        return string(buffer);
    }
    return "";
}

// ========================== CAU HOI ==========================
struct CauHoi {
    int id;
    string noiDung;
    string dapAn[4];
    int dapAnDung;
};

//===================== Ham Nhap ==========================
string inputLine(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        if (!s.empty()) return s;
        cout << "Loi: khong duoc de trong.\n";
    }
}

template <typename T>
T nhapSo(const string& thongBao) {
    T value;
    while (true) {
        cout << thongBao;
        if (cin >> value) {
            cin.ignore(1000, '\n');
            return value;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nhap sai, thu lai!\n";
    }
}

// ========================== DE THI ==========================
struct deThi {
    int id;
    int khoa;
    string tenMon;
    vector<CauHoi> danhSachCauHoi;
    CauHoi ch;
    int thoiGianLamBai;
    int loaiThi;

    deThi() : id(0), khoa(0), tenMon(""), thoiGianLamBai(0), loaiThi(1) {}

    void themCauHoi() {
        ch.id = nhapSo<int>("ID: ");
        cout << "Noi dung: ";
        getline(cin, ch.noiDung);
        for (int i = 0; i < 4; i++) {
            cout << "Lua chon " << i + 1 << ": ";
            getline(cin, ch.dapAn[i]);
        }
        do {
            ch.dapAnDung = nhapSo<int>("Dap an dung (1-4): ");
        } while (ch.dapAnDung < 1 || ch.dapAnDung > 4);
        danhSachCauHoi.push_back(ch);
    }

    void hienThiCauHoi() {
        for (auto& ch : danhSachCauHoi) {
            cout << ch.id << ". " << ch.noiDung << endl;
        }
    }
};

// ========================== KET QUA ==========================
struct KetQua {
    int idSV;
    string tenSV;
    string mon;
    double diem;
    int thoiGian;
    int maDe;
    int loaiThi;

    string getMon() const {
        return mon;
    }
};

// ========================== CLASS INFO ==========================
struct ClassInfo {
    int id;
    string className;
    int teacherId;      // 0 = chua gan
    int studentCount;
};

// ========================== PERSON ==========================
class Person {
protected:
    int id;
    string username;
    string password;
    string fullName;

public:
    Person() : id(0), username(""), password(""), fullName("") {}

    Person(int id, const string& username, const string& password, const string& fullName)
        : id(id), username(username), password(password), fullName(fullName) {
    }

    virtual ~Person() {}
    virtual string getRole() const = 0;

    virtual bool login(const string& u, const string& p) const {
        return u == username && p == password;
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
class Admin : public Person {
public:
    Admin() : Person(1, "admin", "CoDungXinhDep", "System Admin") {}

    Admin(int id, string u, string p, string ten) : Person(id, u, p, ten) {}

    string getRole() const override {
        return "Admin";
    }

    void hienThiThongTin() const {
        cout << "[ADMIN] " << fullName << endl;
    }

    string chuyenThanhChuoiFile() const {
        return to_string(id) + "|" + username + "|" + password + "|" + fullName;
    }
};

// ========================== QUAN LY THI ==========================
class QuanLyThi {
private:
    string tenMon;
    deThi de;
    vector<KetQua> danhSachKetQua;
    vector<deThi> maDe;
public:
    void saveDeThi() const;
    void loadDeThi();
    vector<deThi>& getMaDe() { return maDe; }
    QuanLyThi() { maDe = {}; tenMon = "Chua dat ten"; }

    void thietLapDeThi() {
        de.id = nhapSo<int>("Ma de: ");
        cout << "Ten mon: ";
        getline(cin, de.tenMon);
        de.thoiGianLamBai = nhapSo<int>("Thoi gian lam bai (giay): ");
        do {
            de.loaiThi = nhapSo<int>("Loai bai thi (1. Chinh thuc | 2. On tap): ");
        } while (de.loaiThi != 1 && de.loaiThi != 2);

        int m = nhapSo<int>("Nhap so cau hoi cua de: ");
        de.danhSachCauHoi.clear();
        for (int i = 0; i < m; i++) {
            SetColor(LIGHT_YELLOW);
            cout << "\n--- Nhap cau hoi thu " << i + 1 << " ---" << endl;
            SetColor(RED);
            de.themCauHoi();
        }
        maDe.push_back(de);
        saveDeThi();
        SetColor(LIGHT_GREEN);
        cout << "Tao de thi thanh cong!\n";
        SetColor(BLUE);
    }

    void suaCauHoi() {
        int idDe = nhapSo<int>("Nhap Ma De can sua: ");
        for (auto& d : maDe) {
            if (d.id == idDe) {
                d.hienThiCauHoi();
                if (d.danhSachCauHoi.empty()) {
                    SetColor(RED);
                    cout << "De chua co cau hoi!\n";
                    SetColor(WHITE);
                    return;
                }
                int idCH = nhapSo<int>("Nhap ID cau hoi can sua: ");
                for (auto& ch : d.danhSachCauHoi) {
                    if (ch.id == idCH) {
                        cout << "Noi dung moi: "; getline(cin, ch.noiDung);
                        for (int i = 0; i < 4; i++) {
                            cout << "Lua chon " << i + 1 << ": "; getline(cin, ch.dapAn[i]);
                        }
                        do {
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

    void xoaCauHoi() {
        int idDe = nhapSo<int>("Nhap Ma De can xoa cau hoi: ");
        for (auto& d : maDe) {
            if (d.id == idDe) {
                d.hienThiCauHoi();
                int idCH = nhapSo<int>("Nhap ID cau hoi can xoa: ");
                auto it = remove_if(d.danhSachCauHoi.begin(), d.danhSachCauHoi.end(), [idCH](const CauHoi& c) { return c.id == idCH; });
                if (it != d.danhSachCauHoi.end()) {
                    d.danhSachCauHoi.erase(it, d.danhSachCauHoi.end());
                    SetColor(GREEN);
                    cout << "Xoa cau hoi thanh cong!\n";
                    SetColor(WHITE);
                }
                else {
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
    void menu() {
        int ch;
        do {
            VeKhungTieuDe("QUAN LY DE THI", YELLOW);
            cout << "1. Tao de thi moi\n2. Sua cau hoi trong de\n3. Xoa cau hoi trong de\n4. Xem danh sach de thi\n0. Quay lai\n";
            ch = nhapSo<int>("Chon: ");
            switch (ch) {
            case 1: thietLapDeThi(); break;
            case 2: suaCauHoi(); break;
            case 3: xoaCauHoi(); break;
            case 4:
                for (const auto& d : maDe) {
                    string loai = (d.loaiThi == 1) ? "[Chinh thuc]" : "[On tap]";
                    SetColor(CYAN);

                    cout << "Ma de: " << d.id
                        << " | Mon: " << d.tenMon
                        << " " << loai
                        << " | Thoi gian: "
                        << d.thoiGianLamBai << "s"
                        << endl;

                    SetColor(WHITE);
                }
                break;
            }
        } while (ch != 0);
    }
};

void QuanLyThi::saveDeThi() const {
    cout << "Dang luu tai: "
        << filesystem::current_path() << endl;
    ofstream fout("dethi.txt");
    for (const auto& de : maDe) {
        fout << de.id << '|' << de.tenMon << '|' << de.thoiGianLamBai << '|' << de.loaiThi << '|' << de.danhSachCauHoi.size() << '\n';
        for (const auto& ch : de.danhSachCauHoi) {
            fout << ch.id << '|' << ch.noiDung << '|' << ch.dapAn[0] << '|' << ch.dapAn[1] << '|' << ch.dapAn[2] << '|' << ch.dapAn[3] << '|' << ch.dapAnDung << '\n';
        }
    }
    fout.close();
}

void QuanLyThi::loadDeThi() {
    maDe.clear();
    ifstream fin("dethi.txt");
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        deThi de;
        string idStr, tgStr, loaiStr, soCauStr;

        getline(ss, idStr, '|');
        getline(ss, de.tenMon, '|');
        getline(ss, tgStr, '|');
        getline(ss, loaiStr, '|');
        getline(ss, soCauStr);

        de.id = stoi(idStr);
        de.thoiGianLamBai = stoi(tgStr);
        de.loaiThi = stoi(loaiStr);
        int soCau = stoi(soCauStr);

        for (int i = 0; i < soCau; i++) {
            getline(fin, line);
            stringstream s2(line);
            CauHoi ch;
            string idCH, dapAnDungStr;

            getline(s2, idCH, '|');
            getline(s2, ch.noiDung, '|');
            getline(s2, ch.dapAn[0], '|');
            getline(s2, ch.dapAn[1], '|');
            getline(s2, ch.dapAn[2], '|');
            getline(s2, ch.dapAn[3], '|');
            getline(s2, dapAnDungStr);

            ch.id = stoi(idCH);
            ch.dapAnDung = stoi(dapAnDungStr);
            de.danhSachCauHoi.push_back(ch);
        }
        maDe.push_back(de);
    }
    fin.close();
}

// ========================== STUDENT ==========================
class SinhVien : public Person {
private:
    string lop;
    string gioitinh;
    string birthDay;
    string sdt;
    vector<KetQua> danhSachKetQua;
    double diemTB;
    int teacherId;

public:
    SinhVien() : Person() {
        lop = ""; gioitinh = ""; birthDay = ""; sdt = ""; diemTB = 0.0; teacherId = 0;
    }

    SinhVien(int id, string userN, string passW, string ten,
        string lop, string gioitinh, string birthDay, string sdt, double diemTB, int teacherId)
        : Person(id, userN, passW, ten), lop(lop), gioitinh(gioitinh), birthDay(birthDay), sdt(sdt), diemTB(diemTB), teacherId(teacherId) {
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

    void hienThiThongTin() const {
        cout << "==========Thong Tin Sinh Vien===========\n"
            << "[HS] " << fullName << " | Lop: " << lop << " | Gioi tinh: " << gioitinh
            << " | Birth: " << birthDay << " | SDT: " << sdt << " | Diem TB: " << diemTB
            << " | TeacherID: " << teacherId << endl;
    }

    void thongKeDiemTB() {
        double tong = 0;
        int dem = 0;
        for (const auto& kq : danhSachKetQua) {
            tong += kq.diem;
            dem++;
        }
        if (dem == 0) {
            cout << "Chua co diem!\n";
            return;
        }
        cout << "Diem trung binh: " << tong / dem << endl;
    }

    void xemDanhSachOnTap(QuanLyThi& thi) {
        cout << "\n===== DANH SACH DE ON TAP =====\n";
        for (const auto& de : thi.getMaDe()) {
            if (de.loaiThi == 2) {
                cout << "Ma de: " << de.id << " | Mon: " << de.tenMon
                    << " | Thoi gian: " << de.thoiGianLamBai << " giay\n";
            }
        }
    }

    void datLaiMatKhau() {
        string mk;
        cout << "Nhap MK moi: ";
        getline(cin, mk);
        password = mk;
    }

    double traLoiCauHoi(vector<CauHoi> danhSachCauHoi, time_t start, int thoiGian) const {
        int n;
        double diemThi = 0;
        for (auto& ch : danhSachCauHoi) {
            time_t now = time(0);
            if (difftime(now, start) >= thoiGian) {
                cout << "Het gio lam bai!\n";
                break;
            }
            cout << ch.id << ". " << ch.noiDung << endl;
            for (int i = 0;i < 4;i++) {
                cout << i + 1 << ". " << ch.dapAn[i] << endl;
            }
            n = nhapSo<int>("Dap an ban chon: ");
            if (n == ch.dapAnDung) {
                diemThi += 10.0 / danhSachCauHoi.size();
            }
        }
        return diemThi;
    }

    void xemDanhSachDe(QuanLyThi& thi) {
        SetColor(LIGHT_CYAN);
        cout << "--- DANH SACH BAI THI CO THE LAM ---\n";
        SetColor(WHITE);

        bool coBai = false;

        for (const auto& de : thi.getMaDe()) {
            bool thiChinhThucRoi = false;
            if (de.loaiThi == 1) {
                for (const auto& kq : danhSachKetQua) {
                    if (kq.maDe == de.id && kq.loaiThi == 1) { thiChinhThucRoi = true; break; }
                }
            }
            if (!thiChinhThucRoi) {
                string loaiStr = (de.loaiThi == 1) ? "[CHINH THUC (1 lan)]" : "[ON TAP (n lan)]";
                cout << "Ma de: " << de.id << " | Mon: " << de.tenMon << " " << loaiStr << " | Thoi gian: " << de.thoiGianLamBai << "s\n";
                coBai = true;
            }
        }
        if (!coBai) {
            SetColor(YELLOW);
            cout << "Ban da lam het de Chinh thuc hien co!\n";
            SetColor(WHITE);
        }
    }

    void lamBaiThi(QuanLyThi& thi) {
        int chonDe = nhapSo<int>("Nhap Ma De muon thi: ");

        deThi* deHienTai = nullptr;

        for (auto& de : thi.getMaDe()) {
            if (de.id == chonDe) {
                deHienTai = &de;
                break;
            }
        }

        if (deHienTai == nullptr || deHienTai->danhSachCauHoi.empty()) {
            SetColor(RED);
            cout << "De khong hop le!\n";
            SetColor(WHITE);
            return;
        }

        if (deHienTai->loaiThi == 1) {
            for (const auto& kq : danhSachKetQua) {

                if (kq.maDe == deHienTai->id && kq.loaiThi == 1) {
                    SetColor(RED);
                    cout << "De thi Chinh thuc chi duoc lam 1 lan. Ban da thi roi!\n";
                    SetColor(WHITE);
                    return;
                }
            }
        }
        SetColor(CYAN);

        cout << "\n=== BAT DAU LAM BAI: "
            << deHienTai->tenMon
            << " ===\n";
        SetColor(WHITE);
        time_t start = time(0);

        double diemThi = traLoiCauHoi(
            deHienTai->danhSachCauHoi,
            start,
            deHienTai->thoiGianLamBai
        );

        int thoiGianThucTe =
            (int)difftime(time(0), start);
        if (thoiGianThucTe == 0)
            thoiGianThucTe = 1;
        SetColor(GREEN);
        cout << "=> Nop bai thanh cong! Diem: "
            << diemThi
            << " | Thoi gian: "
            << thoiGianThucTe
            << "s\n";

        SetColor(WHITE);
        KetQua kq;
        kq.idSV = this->id; kq.tenSV = this->fullName; kq.diem = diemThi;
        kq.mon = deHienTai->tenMon; kq.thoiGian = thoiGianThucTe;
        kq.maDe = deHienTai->id; kq.loaiThi = deHienTai->loaiThi;
        this->danhSachKetQua.push_back(kq);
    }

    void xemThongTinDeThi(QuanLyThi& thi) {
        int maDe = nhapSo<int>("Nhap ma de: ");
        for (auto& de : thi.getMaDe()) {
            if (de.id == maDe) {
                cout << "\n===== THONG TIN DE THI =====\n";
                cout << "Mon: " << de.tenMon << endl;
                cout << "So cau hoi: " << de.danhSachCauHoi.size() << endl;
                cout << "Thoi gian: " << de.thoiGianLamBai << " giay\n";
                cout << "Loai thi: ";
                if (de.loaiThi == 1) cout << "Chinh thuc\n";
                else cout << "On tap\n";
                return;
            }
        }
        cout << "Khong tim thay de.\n";
    }

    void xemMucTieu() {
        VeKhungTieuDe("MUC TIEU PHAN DAU", GREEN);
        double tong = 0; int dem = 0;
        for (const auto& kq : danhSachKetQua) {
            if (kq.loaiThi == 1) { tong += kq.diem; dem++; }
        }
        double dtb = (dem == 0) ? 0 : (tong / dem);
        SetColor(BRIGHT_WHITE);
        cout << "Diem TB Tich Luy (Chinh thuc): "
            << dtb
            << "\n";

        SetColor(WHITE);

        if (dtb >= 8.0) {

            SetColor(GREEN);

            cout << "=> Xuat sac! Ban dat muc Gioi.\n";

            SetColor(WHITE);
        }
        else if (dtb >= 6.5) {

            SetColor(YELLOW);

            cout << "=> Kha. De dat Gioi can them: "
                << 8.0 - dtb
                << " diem.\n";

            SetColor(WHITE);
        }
        else {

            SetColor(RED);

            cout << "=> Yeu. De dat Kha can them: "
                << 6.5 - dtb
                << " diem.\n";

            SetColor(WHITE);
        }
    }
    void hienThiDanhSachKetQua(const vector<KetQua>& ds) const {
        if (ds.empty()) {
            cout << "Chua co ket qua nao!\n";
            return;
        }
        for (const auto& kq : ds) {
            cout << "Ten SV: " << kq.tenSV << " | Mon: " << kq.mon << " | Diem: " << kq.diem << " | Thoi gian: " << kq.thoiGian << " giay" << endl;
        }
    }

    string chuyenThanhChuoiFile() const {
        return to_string(id) + "|" + username + "|" + password + "|" + fullName + "|" + lop + "|" + to_string(diemTB) + "|" + to_string(teacherId);
    }

    void menu(QuanLyThi& thi) {
        int choice;
        do {
            SetColor(CYAN);
            cout << "╔══════════════════════════════════════════════╗\n";
            cout << "║               MENU SINH VIEN                ║\n";
            cout << "╠══════════════════════════════════════════════╣\n";

            SetColor(YELLOW);
            cout << "║  1. Xem thong tin ca nhan                   ║\n";
            cout << "║  2. Xem danh sach bai thi                   ║\n";
            cout << "║  3. Xem thong tin bai thi                   ║\n";
            cout << "║  4. Lam bai thi                             ║\n";
            cout << "║  5. Xem lich su thi                         ║\n";
            cout << "║  6. Thong ke diem trung binh                ║\n";
            cout << "║  7. Xem danh sach bai on tap                ║\n";
            cout << "║  8. Doi mat khau                            ║\n";
            cout << "║  0. Dang xuat                               ║\n";

            SetColor(CYAN);
            cout << "╚══════════════════════════════════════════════╝\n";

            SetColor(GREEN);
            choice = nhapSo<int>("Chon chuc nang: ");

            SetColor(WHITE);
            switch (choice) {
            case 1:
                SetColor(LIGHT_CYAN);
                hienThiThongTin();
                SetColor(WHITE);
                break;

            case 2:
                SetColor(LIGHT_BLUE);
                xemDanhSachDe(thi);
                SetColor(WHITE);
                break;

            case 3:
                SetColor(CYAN);
                xemThongTinDeThi(thi);
                SetColor(WHITE);
                break;

            case 4:
                SetColor(LIGHT_GREEN);
                lamBaiThi(thi);
                SetColor(WHITE);
                break;

            case 5:
                SetColor(YELLOW);
                hienThiDanhSachKetQua(danhSachKetQua);
                SetColor(WHITE);
                break;

            case 6:
                SetColor(LIGHT_MAGENTA);
                thongKeDiemTB();
                SetColor(WHITE);
                break;

            case 7:
                SetColor(LIGHT_YELLOW);
                xemDanhSachOnTap(thi);
                SetColor(WHITE);
                break;

            case 8:
                datLaiMatKhau();

                SetColor(GREEN);
                cout << "Doi mat khau thanh cong!\n";
                SetColor(WHITE);

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

// ========================== TEACHER ==========================
class GiangVien : public Person {
private:
    string mon;

public:
    void setMon(const string& m) { mon = m; }
    string getMon() const { return mon; }

    GiangVien() : Person() {};
    GiangVien(int id, string u, string p, string ten, string mon) : Person(id, u, p, ten), mon(mon) {}

    string getRole() const override {
        return "GiangVien";
    }

    void hienThiThongTin() const {
        SetColor(LIGHT_YELLOW);
        cout << "[GV] " << fullName << " | Mon: " << mon << endl;

        SetColor(WHITE);
    }

    string chuyenThanhChuoiFile() const {
        return to_string(id) + "|" + username + "|" + password + "|" + fullName + "|" + mon;
    }

    void datLaiMatKhau() {
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
    void xemSinhVien(const vector<SinhVien>& students) {
        SetColor(LIGHT_CYAN);
        cout << "\n--- sinh vien LOP CHU NHIEM ---\n";
        SetColor(WHITE);    for (const auto& hs : students) {
            if (hs.layMaGV() == this->id) hs.hienThiThongTin();
        }
    }

    void xemKetQuaThi(const vector<SinhVien>& students) {
        SetColor(LIGHT_CYAN);
        cout << "\n===== KET QUA THI LOP CHU NHIEM =====\n";

        for (const auto& hs : students) {
            if (hs.layMaGV() == this->id) {

                SetColor(LIGHT_YELLOW);
                cout << "\nSinh vien: " << hs.getFullName() << endl;

                for (const auto& kq : hs.getDanhSachKetQua()) {

                    SetColor(LIGHT_GREEN);
                    cout << "Mon: " << kq.mon
                        << " | Diem: " << kq.diem
                        << " | Thoi gian: " << kq.thoiGian << "s\n";
                }
            }
        }

        SetColor(WHITE);
    }

    void sapXepSinhVienTheoDiem(vector<SinhVien>& students) {
        vector<SinhVien> ds;
        for (auto hs : students) {
            if (hs.layMaGV() == this->id) {
                ds.push_back(hs);
            }
        }
        sort(ds.begin(), ds.end(), [](SinhVien a, SinhVien b) { return a.layDiem() > b.layDiem(); });
        SetColor(CYAN);
        cout << "\n===== SAP XEP sinh vien THEO DIEM =====\n";
        SetColor(WHITE);        for (auto& hs : ds) { cout << hs.getFullName() << " | Diem TB: " << hs.layDiem() << endl; }
    }

    void thongKe(const vector<SinhVien>& students) {
        double tong = 0; int dem = 0;
        for (const auto& hs : students) {
            if (hs.layMaGV() == this->id) { tong += hs.layDiem(); dem++; }
        }
        if (dem) cout << "Diem TB: " << tong / dem << endl;
        else cout << "Khong co du lieu!\n";
    }

    void thongKeTheoMon(const vector<SinhVien>& students) {
        string monTK = inputLine("Nhap ten mon muon thong ke: ");
        SetColor(CYAN);

        cout << "\n--- THONG KE DIEM MON "
            << monTK
            << " (CHINH THUC) ---\n";

        SetColor(WHITE);
        struct HSData { string ten; double diem; };
        vector<HSData> tk;

        for (const auto& hs : students) {
            if (hs.layMaGV() == this->id) {
                double maxDiem = -1;
                for (const auto& kq : hs.getDanhSachKetQua()) {
                    if (kq.mon == monTK && kq.loaiThi == 1) maxDiem = max(maxDiem, kq.diem);
                }
                if (maxDiem >= 0) tk.push_back({ hs.getFullName(), maxDiem });
            }
        }

        if (tk.empty()) {
            SetColor(YELLOW);
            cout << "Chua co du lieu thi Chinh thuc mon nay!\n";
            SetColor(WHITE);
        }
        else {
            sort(tk.begin(), tk.end(), [](const HSData& a, const HSData& b) { return a.diem > b.diem; });
            for (const auto& data : tk) cout << "HS: " << data.ten << " | Diem mon: " << data.diem << "\n";
        }
    }

    void menu(vector<SinhVien>& students, QuanLyThi& thi) {
        int choice;
        do {
            SetColor(CYAN);
            cout << "╔══════════════════════════════════════════════╗\n";
            cout << "║               MENU GIANG VIEN                ║\n";
            cout << "╠══════════════════════════════════════════════╣\n";

            SetColor(YELLOW);
            cout << "║  1. Xem danh sach sinh vien                  ║\n";
            cout << "║  2. Thong ke diem trung binh                ║\n";
            cout << "║  3. Quan ly de thi                          ║\n";
            cout << "║  4. Thong ke theo mon                       ║\n";
            cout << "║  5. Xem ket qua thi                         ║\n";
            cout << "║  6. Sap xep sinh vien theo diem              ║\n";
            cout << "║  7. Doi mat khau                            ║\n";
            cout << "║  0. Dang xuat                               ║\n";

            SetColor(CYAN);
            cout << "╚══════════════════════════════════════════════╝\n";

            SetColor(GREEN);
            choice = nhapSo<int>("Chon chuc nang: ");

            SetColor(WHITE);

            switch (choice) {
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
                thi.menu();
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
class QuanLyNguoiDung {
private:
    vector<Person*> users;

public:
    void them(Person* p) { users.push_back(p); }

    Person* dangNhap() {
        string u, p;
        cout << "User: "; cin >> u;
        cout << "Pass: "; cin >> p;
        cin.ignore();

        for (auto user : users) {
            if (user->getUsername() == u && user->getPassword() == p) {
                return user;
            }
        }
        return NULL;
    }

    vector<Person*>& layDanhSach() { return users; }
};

// ========================== RANK ITEM ==========================
struct RankItem {
    int id;
    string fullName;
    double score;
};

// ========================== ADMIN SYSTEM ==========================
class AdminSystem {
private:
    Admin adminAccount;
    vector<GiangVien> teachers;
    vector<SinhVien> students;
    vector<ClassInfo> classes;
    vector<RankItem> ranking;
    int totalStudents;

public:
    void sortRanking() {
        sort(ranking.begin(), ranking.end(), [](const RankItem& a, const RankItem& b) {
            if (a.score != b.score) return a.score > b.score;
            return a.fullName < b.fullName;
            });
    }

    AdminSystem() : totalStudents(0) { sortRanking(); }

    // ==================== VALIDATION ====================
    bool isValidName(const string& s) const {
        if (s.empty()) return false;
        for (char c : s) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!(isalnum(uc) || isspace(uc))) { return false; }
        }
        return true;
    }

    bool isValidUsername(const string& s) const {
        if (s.empty()) return false;
        for (char c : s) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!(isalnum(uc) || c == '_')) { return false; }
        }
        return true;
    }

    bool isValidPassword(const string& s) const {
        return s.length() >= 6;
    }

    bool teacherIdExists(int id) const {
        for (const auto& t : teachers) {
            if (t.getId() == id) return true;
        }
        return false;
    }

    bool studentIdExists(int id) const {
        for (const auto& hs : students) {
            if (hs.getId() == id) return true;
        }
        return false;
    }

    bool classIdExists(int id) const {
        for (const auto& c : classes) {
            if (c.id == id) return true;
        }
        return false;
    }

    bool usernameExists(const string& username) const {
        if (username == "admin") return true;
        for (const auto& t : teachers) {
            if (t.getUsername() == username) return true;
        }
        for (const auto& hs : students) {
            if (hs.getUsername() == username) return true;
        }
        return false;
    }

    int findTeacherIndexById(int id) const {
        for (int i = 0; i < (int)teachers.size(); i++) {
            if (teachers[i].getId() == id) return i;
        }
        return -1;
    }

    int findStudentIndexById(int id) const {
        for (int i = 0; i < (int)students.size(); i++) {
            if (students[i].getId() == id) return i;
        }
        return -1;
    }

    int findClassIndexById(int id) const {
        for (int i = 0; i < (int)classes.size(); i++) {
            if (classes[i].id == id) return i;
        }
        return -1;
    }

    bool teacherHasAssignedClass(int teacherId) const {
        for (const auto& c : classes) {
            if (c.teacherId == teacherId) return true;
        }
        return false;
    }

    void saveKetQua() const {
        ofstream fout("ketqua.txt");
        for (const auto& hs : students) {
            for (const auto& kq : hs.getDanhSachKetQua()) {
                fout << kq.idSV << '|' << kq.tenSV << '|' << kq.mon << '|' << kq.diem << '|' << kq.thoiGian << '|' << kq.maDe << '|' << kq.loaiThi << '\n';
            }
        }
        fout.close();
    }

    void loadKetQua() {
        ifstream fin("ketqua.txt");
        if (!fin) return;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idSVStr, tenSV, mon, diemStr, thoiGianStr, maDeStr, loaiThiStr;

            getline(ss, idSVStr, '|'); getline(ss, tenSV, '|'); getline(ss, mon, '|'); getline(ss, diemStr, '|'); getline(ss, thoiGianStr, '|'); getline(ss, maDeStr, '|'); getline(ss, loaiThiStr);

            KetQua kq;
            kq.idSV = stoi(idSVStr); kq.tenSV = tenSV; kq.mon = mon; kq.diem = stod(diemStr); kq.thoiGian = stoi(thoiGianStr); kq.maDe = stoi(maDeStr); kq.loaiThi = stoi(loaiThiStr);

            for (auto& hs : students) {
                if (hs.getId() == kq.idSV) {
                    vector<KetQua> ds = hs.getDanhSachKetQua();
                    ds.push_back(kq);
                    hs.setDanhSachKetQua(ds);
                    break;
                }
            }
        }
        fin.close();
    }

    void recalcTotalStudents() {
        totalStudents = 0;
        for (const auto& c : classes) {
            totalStudents += c.studentCount;
        }
    }

    // ==================== LOGIN ====================
    bool loginAdmin() {
        SetColor(MAGENTA);

        cout << "╔════════════════════════════════════╗\n";
        cout << "║         DANG NHAP ADMIN           ║\n";
        cout << "╚════════════════════════════════════╝\n";

        string user, pass;

        SetColor(CYAN);
        cout << "Username: ";
        SetColor(WHITE);
        getline(cin, user);

        SetColor(CYAN);
        cout << "Password: ";
        SetColor(WHITE);
        getline(cin, pass);

        if (adminAccount.login(user, pass)) {
            SetColor(GREEN);
            cout << "\nDang nhap thanh cong.\n";

            SetColor(WHITE);
            return true;
        }

        SetColor(RED);
        cout << "\nSai username hoac password.\n";

        SetColor(WHITE);
        return false;
    }

    //==================== STUDENT CRUD =====================
    void addStudent() {
        cout << "\n===== THEM SINH VIEN =====\n";
        SinhVien hs;
        hs.setId(nhapSo<int>("Nhap ID sinh vien: "));

        if (studentIdExists(hs.getId())) { cout << "ID sinh vien da ton tai.\n"; return; }

        hs.setUsername(inputLine("Nhap username: "));
        if (!isValidUsername(hs.getUsername())) { cout << "Username khong hop le. Chi gom chu, so, dau _.\n"; return; }
        if (usernameExists(hs.getUsername())) { cout << "Username da ton tai.\n"; return; }

        hs.setPassword(inputLine("Nhap password: "));
        if (!isValidPassword(hs.getPassword())) { cout << "Password khong hop le. Toi thieu 6 ky tu.\n"; return; }

        hs.setFullName(inputLine("Nhap ho ten: "));
        if (!isValidName(hs.getFullName())) { cout << "Ho ten khong hop le.\n"; return; }

        hs.setLop(inputLine("Nhap lop: "));
        hs.setGioiTinh(inputLine("Nhap gioi tinh: "));
        hs.setBirthDay(inputLine("Nhap ngay sinh: "));
        hs.setSdt(inputLine("Nhap so dien thoai: "));
        hs.setDiemTB(nhapSo<double>("Nhap diem trung binh: "));

        int teacherId = nhapSo<int>("Nhap ID giang vien phu trach: ");
        if (!teacherIdExists(teacherId)) { cout << "ID giang vien khong ton tai.\n"; return; }

        hs.setTeacherId(teacherId);
        students.push_back(hs);
        cout << "Them sinh vien thanh cong.\n";
    }

    void viewStudents() const {
        cout << "\n===== DANH SACH SINH VIEN =====\n";
        if (students.empty()) { cout << "Chua co sinh vien nao.\n"; return; }
        for (int i = 0; i < (int)students.size(); i++) {
            cout << "STT: " << i + 1 << " | ID: " << students[i].getId() << " | Username: " << students[i].getUsername() << " | Full name: " << students[i].getFullName() << '\n';
        }
    }

    void editStudent() {
        cout << "\n===== SUA SINH VIEN =====\n";
        int id = nhapSo<int>("Nhap ID sinh vien can sua: ");
        int idx = findStudentIndexById(id);
        if (idx == -1) { cout << "Khong tim thay sinh vien.\n"; return; }

        cout << "1. Sua username\n2. Sua password\n3. Sua ho ten\n4. Sua tat ca\n";
        int choice = nhapSo<int>("Chon: ");

        if (choice == 1 || choice == 4) {
            string newUsername = inputLine("Nhap username moi: ");
            if (!isValidUsername(newUsername)) { cout << "Username khong hop le.\n"; return; }
            if (newUsername != students[idx].getUsername() && usernameExists(newUsername)) { cout << "Username da ton tai.\n"; return; }
            students[idx].setUsername(newUsername);
        }

        if (choice == 2 || choice == 4) {
            string newPassword = inputLine("Nhap password moi: ");
            if (!isValidPassword(newPassword)) { cout << "Password khong hop le.\n"; return; }
            students[idx].setPassword(newPassword);
        }

        if (choice == 3 || choice == 4) {
            string newFullName = inputLine("Nhap ho ten moi: ");
            if (!isValidName(newFullName)) { cout << "Ho ten khong hop le.\n"; return; }
            students[idx].setFullName(newFullName);
        }

        cout << "1. Sua username\n2. Sua password\n3. Sua ho ten\n4. Sua lop\n5. Sua gioi tinh\n6. Sua ngay sinh\n7. Sua so dien thoai\n8. Sua diem trung binh\n9. Sua giang vien phu trach\n10. Sua tat ca\n";
        int chHS = nhapSo<int>("Chon: ");

        if (choice == 1 || chHS == 10) {
            string newUsername = inputLine("Nhap username moi: ");
            if (!isValidUsername(newUsername)) { cout << "Username khong hop le.\n"; return; }
            if (newUsername != students[idx].getUsername() && usernameExists(newUsername)) { cout << "Username da ton tai.\n"; return; }
            students[idx].setUsername(newUsername);
        }
        if (chHS == 2 || chHS == 10) {
            string newPassword = inputLine("Nhap password moi: ");
            if (!isValidPassword(newPassword)) { cout << "Password khong hop le.\n"; return; }
            students[idx].setPassword(newPassword);
        }
        if (chHS == 3 || chHS == 10) {
            string newFullName = inputLine("Nhap ho ten moi: ");
            if (!isValidName(newFullName)) { cout << "Ho ten khong hop le.\n"; return; }
            students[idx].setFullName(newFullName);
        }
        if (chHS == 4 || chHS == 10) {
            string newClass = inputLine("Nhap lop moi: ");
            students[idx].setLop(newClass);
        }
        if (chHS == 5 || chHS == 10) {
            string newGender = inputLine("Nhap gioi tinh moi: ");
            students[idx].setGioiTinh(newGender);
        }
        if (chHS == 6 || chHS == 10) {
            string newBirthDay = inputLine("Nhap ngay sinh moi: ");
            students[idx].setBirthDay(newBirthDay);
        }
        if (chHS == 7 || chHS == 10) {
            string newPhone = inputLine("Nhap so dien thoai moi: ");
            students[idx].setSdt(newPhone);
        }
        if (chHS == 8 || chHS == 10) {
            double newDiemTB = nhapSo<double>("Nhap diem trung binh moi: ");
            students[idx].setDiemTB(newDiemTB);
        }
        if (chHS == 9 || chHS == 10) {
            int newTeacherId = nhapSo<int>("Nhap ID giang vien moi: ");
            if (!teacherIdExists(newTeacherId)) { cout << "ID giang vien khong ton tai.\n"; return; }
            students[idx].setTeacherId(newTeacherId);
        }
        cout << "Sua sinh vien thanh cong.\n";
    }

    void deleteStudent() {
        cout << "\n===== XOA SINH VIEN =====\n";
        int id = nhapSo<int>("Nhap ID sinh vien can xoa: ");
        int idx = findStudentIndexById(id);
        if (idx == -1) { cout << "Khong tim thay sinh vien.\n"; return; }
        students.erase(students.begin() + idx);
        cout << "Xoa sinh vien thanh cong.\n";
    }

    vector<SinhVien>& getStudents() { return students; }

    // ==================== TEACHER CRUD ====================
    void addTeacher() {
        cout << "\n===== THEM giang vien =====\n";
        GiangVien t;
        t.setId(nhapSo<int>("Nhap ID giang vien: "));
        if (teacherIdExists(t.getId())) { cout << "ID giang vien da ton tai.\n"; return; }

        t.setUsername(inputLine("Nhap username: "));
        if (!isValidUsername(t.getUsername())) { cout << "Username khong hop le.\n"; return; }
        if (usernameExists(t.getUsername())) { cout << "Username da ton tai.\n"; return; }

        t.setPassword(inputLine("Nhap password: "));
        if (!isValidPassword(t.getPassword())) { cout << "Password phai >= 6 ky tu.\n"; return; }

        t.setFullName(inputLine("Nhap ho ten: "));
        string mon = inputLine("Nhap mon day: ");
        t.setMon(mon);

        teachers.push_back(t);
        cout << "Them giang vien thanh cong.\n";
    }

    void viewTeachers() const {
        cout << "\n===== DANH SACH GIANG VIEN =====\n";
        if (teachers.empty()) { cout << "Chua co giang vien nao.\n"; return; }
        for (int i = 0; i < teachers.size(); i++) {
            cout << "STT: " << i + 1 << " | ID: " << teachers[i].getId() << " | Username: " << teachers[i].getUsername() << " | Full Name: " << teachers[i].getFullName() << " | Mon: " << teachers[i].getMon() << '\n';
        }
    }

    void editTeacher() {
        cout << "\n===== SUA GIANG VIEN =====\n";
        int id = nhapSo<int>("Nhap ID giang vien can sua: ");
        int idx = findTeacherIndexById(id);
        if (idx == -1) { cout << "Khong tim thay giang vien.\n"; return; }

        cout << "1. Sua username\n2. Sua password\n3. Sua ho ten\n4. Sua tat ca\n";
        int choice = nhapSo<int>("Chon: ");

        if (choice == 1 || choice == 4) {
            string newUsername = inputLine("Nhap username moi: ");
            if (!isValidUsername(newUsername)) { cout << "Username khong hop le.\n"; return; }
            if (newUsername != teachers[idx].getUsername() && usernameExists(newUsername)) { cout << "Username da ton tai.\n"; return; }
            teachers[idx].setUsername(newUsername);
        }

        if (choice == 2 || choice == 4) {
            string newPassword = inputLine("Nhap password moi: ");
            if (!isValidPassword(newPassword)) { cout << "Password khong hop le.\n"; return; }
            teachers[idx].setPassword(newPassword);
        }

        if (choice == 3 || choice == 4) {
            string newFullName = inputLine("Nhap ho ten moi: ");
            if (!isValidName(newFullName)) { cout << "Ho ten khong hop le.\n"; return; }
            teachers[idx].setFullName(newFullName);
        }

        cout << "Sua giang vien thanh cong.\n";
    }

    void deleteTeacher() {
        cout << "\n===== XOA GIANG VIEN =====\n";
        int id = nhapSo<int>("Nhap ID giang vien can xoa: ");
        int idx = findTeacherIndexById(id);
        if (idx == -1) { cout << "Khong tim thay giang vien.\n"; return; }
        if (teacherHasAssignedClass(id)) { cout << "Khong the xoa. giang vien dang duoc gan cho lop.\n"; return; }

        teachers.erase(teachers.begin() + idx);
        cout << "Xoa giang vien thanh cong.\n";
    }

    vector<GiangVien>& getTeachers() { return teachers; }

    // ==================== CLASS CRUD ====================
    void addClass() {
        cout << "\n===== THEM LOP =====\n";
        ClassInfo c;
        c.id = nhapSo<int>("Nhap ID lop: ");
        if (classIdExists(c.id)) { cout << "ID lop da ton tai.\n"; return; }

        c.className = inputLine("Nhap ten lop: ");
        if (c.className.empty()) { cout << "Ten lop khong duoc rong.\n"; return; }

        c.teacherId = nhapSo<int>("Nhap ID giang vien phu trach (0 neu chua gan): ");
        if (c.teacherId != 0 && !teacherIdExists(c.teacherId)) { cout << "Giang vien khong ton tai.\n"; return; }

        c.studentCount = nhapSo<int>("Nhap so luong sinh vien: ");
        if (c.studentCount < 0) { cout << "So luong sinh vien khong hop le.\n"; return; }

        classes.push_back(c);
        recalcTotalStudents();
        cout << "Them lop thanh cong.\n";
    }

    void viewClasses() const {
        cout << "\n===== DANH SACH LOP =====\n";
        if (classes.empty()) { cout << "Chua co lop nao.\n"; return; }
        for (int i = 0; i < (int)classes.size(); i++) {
            cout << "STT: " << i + 1 << " | ID lop: " << classes[i].id << " | Ten lop: " << classes[i].className << " | Teacher ID: " << classes[i].teacherId << " | So sinh vien: " << classes[i].studentCount << '\n';
        }
    }

    void editClass() {
        cout << "\n===== SUA LOP =====\n";
        int id = nhapSo<int>("Nhap ID lop can sua: ");
        int idx = findClassIndexById(id);
        if (idx == -1) { cout << "Khong tim thay lop.\n"; return; }

        cout << "1. Sua ten lop\n2. Sua giang vien phu trach\n3. Sua so luong sinh vien\n4. Sua tat ca\n";
        int choice = nhapSo<int>("Chon: ");

        if (choice == 1 || choice == 4) {
            string newClassName = inputLine("Nhap ten lop moi: ");
            classes[idx].className = newClassName;
        }

        if (choice == 2 || choice == 4) {
            int newTeacherId = nhapSo<int>("Nhap ID giang vien moi (0 neu bo gan): ");
            if (newTeacherId != 0 && !teacherIdExists(newTeacherId)) { cout << "giang vien khong ton tai.\n"; return; }
            classes[idx].teacherId = newTeacherId;
        }

        if (choice == 3 || choice == 4) {
            int newStudentCount = nhapSo<int>("Nhap so luong sinh vien moi: ");
            if (newStudentCount < 0) { cout << "So luong sinh vien khong hop le.\n"; return; }
            classes[idx].studentCount = newStudentCount;
            recalcTotalStudents();
        }

        cout << "Sua lop thanh cong.\n";
    }

    void deleteClass() {
        cout << "\n===== XOA LOP =====\n";
        int id = nhapSo<int>("Nhap ID lop can xoa: ");
        int idx = findClassIndexById(id);
        if (idx == -1) { cout << "Khong tim thay lop.\n"; return; }

        classes.erase(classes.begin() + idx);
        recalcTotalStudents();
        cout << "Xoa lop thanh cong.\n";
    }

    void assignClassToTeacher() {
        cout << "\n===== GAN LOP CHO GIANG VIEN =====\n";
        int classId = nhapSo<int>("Nhap ID lop: ");
        int teacherId = nhapSo<int>("Nhap ID giang vien: ");

        int cidx = findClassIndexById(classId);
        int tidx = findTeacherIndexById(teacherId);

        if (cidx == -1) { cout << "Lop khong ton tai.\n"; return; }
        if (tidx == -1) { cout << "Giang vien khong ton tai.\n"; return; }

        classes[cidx].teacherId = teacherId;
        cout << "Gan lop cho giang vien thanh cong.\n";
    }
    // ==================== STATISTICS ====================
    void countUsers() const {
        cout << "\n===== SO LUONG NGUOI DUNG =====\n";
        cout << "So giang vien: " << teachers.size() << '\n';
        cout << "So sinh vien: " << students.size() << '\n';
    }

    void viewSystemStatistics() const {
        cout << "\n===== THONG KE TOAN HE THONG =====\n";
        cout << "So giang vien: " << teachers.size() << '\n';
        cout << "So lop: " << classes.size() << '\n';
        cout << "Tong so sinh vien: " << totalStudents << '\n';
        cout << "So muc trong bang xep hang: " << ranking.size() << '\n';

        int assignedClassCount = 0;
        for (const auto& c : classes) {
            if (c.teacherId != 0) assignedClassCount++;
        }
        cout << "So lop da gan giang vien: " << assignedClassCount << '\n';
    }

    void viewRanking() {
        SetColor(CYAN);
        cout << "\n===== BANG XEP HANG TOAN TRUONG (CHINH THUC) =====\n";
        SetColor(WHITE);
        struct HSScore { string fullName; double score; };
        vector<HSScore> bxh;

        for (const auto& hs : students) {
            double tong = 0; int dem = 0;
            for (const auto& kq : hs.getDanhSachKetQua()) {
                if (kq.loaiThi == 1) { tong += kq.diem; dem++; }
            }
            if (dem > 0) bxh.push_back({ hs.getFullName(), tong / dem });
        }

        if (bxh.empty()) {
            SetColor(YELLOW);
            cout << "Chua co du lieu!\n";
            SetColor(WHITE);
            return;
        }
        sort(bxh.begin(), bxh.end(), [](const HSScore& a, const HSScore& b) { return a.score > b.score; });
        for (int i = 0; i < bxh.size(); i++) {
            cout << i + 1 << ". Ho ten: " << bxh[i].fullName << " | Diem TB: " << bxh[i].score << '\n';
        }
    }

    // ==================== SAVE / LOAD ====================
    void saveTeachers() const {
        ofstream fout("GiangVien.txt");
        for (const auto& t : teachers) {
            fout << t.getId() << '|' << t.getUsername() << '|' << t.getPassword() << '|' << t.getFullName() << '|' << t.getMon() << '\n';
        }
        fout.close();
    }

    void saveClasses() const {
        ofstream fout("lophoc.txt");
        for (const auto& c : classes) {
            fout << c.id << '|' << c.className << '|' << c.teacherId << '|' << c.studentCount << '\n';
        }
    }

    void saveRanking() const {
        ofstream fout("ranking.txt");
        for (const auto& r : ranking) { fout << r.id << '|' << r.fullName << '|' << r.score << '\n'; }
    }

    void saveStudents() const {
        ofstream fout("students.txt");
        for (const auto& s : students) {
            fout << s.getId() << '|' << s.getUsername() << '|' << s.getPassword() << '|' << s.getFullName() << '|' << s.getLop() << '|' << s.getGioiTinh() << '|' << s.getBirthDay() << '|' << s.getSdt() << '|' << s.layDiem() << '|' << s.layMaGV() << '\n';
        }
    }

    void saveData() const {
        saveStudents();
        saveTeachers();
        saveClasses();
        saveRanking();
        saveKetQua();
        cout << "Luu du lieu thanh cong.\n";
    }

    void loadStudents(const string& fileName) {
        students.clear();
        ifstream fin(fileName);
        if (!fin) { cout << "Khong mo duoc file sinh vien!\n"; return; }

        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            stringstream ss(line);

            string idStr, username, password, fullName, mon;
            string lop, gioitinh, birthDay, sdt;
            string diemTBStr, teacherIdStr;

            getline(ss, idStr, '|'); getline(ss, username, '|'); getline(ss, password, '|'); getline(ss, fullName, '|'); getline(ss, lop, '|'); getline(ss, gioitinh, '|'); getline(ss, birthDay, '|'); getline(ss, sdt, '|'); getline(ss, diemTBStr, '|'); getline(ss, teacherIdStr);

            SinhVien s;
            s.setId(stoi(idStr)); s.setUsername(username); s.setPassword(password); s.setFullName(fullName); s.setLop(lop); s.setGioiTinh(gioitinh); s.setBirthDay(birthDay); s.setSdt(sdt);

            if (diemTBStr.empty()) diemTBStr = "0";
            if (teacherIdStr.empty()) teacherIdStr = "0";

            s.setDiemTB(stod(diemTBStr));
            s.setTeacherId(stoi(teacherIdStr));
            students.push_back(s);
        }
        fin.close();
        cout << "Tai danh sach sinh vien thanh cong.\n";
    }

    void loadTeachers(const string& fileName) {
        teachers.clear();
        ifstream fin(fileName);
        if (!fin) { cout << "Khong mo duoc file giang vien!\n"; return; }

        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, username, password, fullName, mon;

            getline(ss, idStr, '|'); getline(ss, username, '|'); getline(ss, password, '|'); getline(ss, fullName, '|'); getline(ss, mon);

            GiangVien t;
            t.setId(stoi(idStr)); t.setUsername(username); t.setPassword(password); t.setFullName(fullName); t.setMon(mon);
            teachers.push_back(t);
        }
        fin.close();
        cout << "Tai danh sach giang vien thanh cong.\n";
    }

    void loadClasses(const string& fileName) {
        classes.clear();
        ifstream fin(fileName);
        if (!fin) return;

        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, className, teacherIdStr, studentCountStr;

            getline(ss, idStr, '|'); getline(ss, className, '|'); getline(ss, teacherIdStr, '|'); getline(ss, studentCountStr);

            ClassInfo c;
            c.id = stoi(idStr); c.className = className; c.teacherId = stoi(teacherIdStr); c.studentCount = stoi(studentCountStr);
            classes.push_back(c);
        }
        recalcTotalStudents();
    }

    void loadRanking() {
        ranking.clear();
        ifstream fin("xepHang.txt");
        if (!fin) return;

        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, fullName, scoreStr;

            getline(ss, idStr, '|'); getline(ss, fullName, '|'); getline(ss, scoreStr);

            RankItem r;
            r.id = stoi(idStr); r.fullName = fullName; r.score = stod(scoreStr);
            ranking.push_back(r);
        }
        sortRanking();
    }

    void loadData() {
        cout << "Chon file sinh vien...\n"; string studentFile = taiFile();
        if (!studentFile.empty()) { loadStudents(studentFile); }
        else { cout << "Khong chon file sinh vien!\n"; }

        cout << "Chon file giang vien...\n"; string teacherFile = taiFile();
        if (!teacherFile.empty()) { loadTeachers(teacherFile); }
        else { cout << "Khong chon file giang vien!\n"; }

        cout << "Chon file lop hoc...\n"; string classFile = taiFile();
        if (!classFile.empty()) { loadClasses(classFile); }
        else { cout << "Khong chon file lop hoc!\n"; return; }
        loadRanking();
        loadKetQua();
        recalcTotalStudents();
        cout << "Tai du lieu thanh cong.\n";
    }

    // ==================== MENU ====================
    void adminMenu(QuanLyThi& thi) {
        int choice;
        do {
            SetColor(LIGHT_CYAN);

            cout << "\n╔══════════════════════════════════════════════════════╗\n";

            SetColor(LIGHT_MAGENTA);
            cout << "║                 MENU ADMIN TESTPRO                   ║\n";

            SetColor(LIGHT_CYAN);
            cout << "╠══════════════════════════════════════════════════════╣\n";

            SetColor(LIGHT_YELLOW);
            cout << "║ [1]  Them giang vien                                  ║\n";
            cout << "║ [2]  Xoa giang vien                                   ║\n";
            cout << "║ [3]  Sua giang vien                                   ║\n";
            cout << "║ [4]  Xem danh sach giang vien                         ║\n";

            SetColor(LIGHT_YELLOW);
            cout << "╠══════════════════════════════════════════════════════╣\n";

            SetColor(LIGHT_YELLOW);
            cout << "║ [5]  Them lop                                        ║\n";
            cout << "║ [6]  Xoa lop                                         ║\n";
            cout << "║ [7]  Sua lop                                         ║\n";
            cout << "║ [8]  Xem danh sach lop                               ║\n";

            SetColor(LIGHT_YELLOW);
            cout << "╠══════════════════════════════════════════════════════╣\n";

            SetColor(LIGHT_YELLOW);
            
            cout << "║ [9]  Gan lop cho giang vien                           ║\n";
            cout << "║ [10] Xem thong ke toan he thong                      ║\n";
            cout << "║ [11] Thong ke so luong sinh vien / giang vien          ║\n";

            SetColor(LIGHT_YELLOW);
           
            cout << "╠══════════════════════════════════════════════════════╣\n";

            SetColor(LIGHT_YELLOW);
            
            cout << "║ [12] Luu du lieu                                     ║\n";
            cout << "║ [13] Tai du lieu                                     ║\n";

            SetColor(LIGHT_YELLOW);
          
            cout << "║ [0]  Thoat                                           ║\n";

            SetColor(LIGHT_CYAN);
            cout << "╚══════════════════════════════════════════════════════╝\n";

            SetColor(WHITE);

            choice = nhapSo<int>("Chon chuc nang: ");

            switch (choice) {
            case 1: addTeacher(); break;
            case 2: deleteTeacher(); break;
            case 3: editTeacher(); break;
            case 4: viewTeachers(); break;
            case 5:
            {
                addClass();
                int l;
                do {
                    cout << "\n================ Quan ly sinh vien ================\n";
                    cout << "1. Them sinh vien\n";
                    cout << "2. Xoa sinh vien\n";
                    cout << "3. Sua sinh vien\n";
                    cout << "4. Xem danh sach sinh vien\n";
                    cout << "0. Thoat\n";

                    l = nhapSo<int>("Chon chuc nang: ");

                    switch (l) {
                    case 1: addStudent(); break;
                    case 2: deleteStudent(); break;
                    case 3: editStudent(); break;
                    case 4: viewStudents(); break;
                    case 0: break;

                    default:
                        cout << "Lua chon khong hop le.\n";
                    }

                } while (l != 0);
            }
            break;

            case 6: deleteClass(); break;
            case 7: editClass(); break;
            case 8: viewClasses(); break;
            case 9: assignClassToTeacher(); break;
            case 10: viewSystemStatistics(); break;
            case 11: countUsers(); break;
     

            case 12:
                saveData();
                thi.saveDeThi();
                break;

            case 13:
                loadData();
                break;
            case 0: cout << "Thoat chuong trinh.\n"; break;
            default: cout << "Lua chon khong hop le.\n";
            }
        } while (choice != 0);
    }
};

void drawLine(int color) {
    SetColor(color);
    cout << "====================================================\n";
    SetColor(WHITE);
}

//==================== MAIN ====================
int main() {
    SetConsoleOutputCP(CP_UTF8);

    AdminSystem adminSystem;
    QuanLyNguoiDung ql;
    QuanLyThi thi;

    adminSystem.loadStudents("sinhvien.txt");
    adminSystem.loadTeachers("giangvien.txt");
    adminSystem.loadClasses("lophoc.txt");

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

    do {

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

        if (vaiTro == 1) {

            if (adminSystem.loginAdmin()) {
                adminSystem.adminMenu(thi);
            }

        }
        else if (vaiTro == 2) {

            ql.layDanhSach().clear();

            for (auto& t : adminSystem.getTeachers()) {
                ql.them(&t);
            }

            Person* user = ql.dangNhap();

            if (user != nullptr) {

                SetColor(GREEN);
                cout << "Dang nhap thanh cong!\n";
                SetColor(WHITE);

                GiangVien* gv = dynamic_cast<GiangVien*>(user);

                if (gv != nullptr) {
                    gv->menu(adminSystem.getStudents(), thi);
                }
            }
            else {

                SetColor(RED);
                cout << "Dang nhap that bai!\n";
                SetColor(WHITE);
            }
        }
        else if (vaiTro == 3) {

            ql.layDanhSach().clear();

            for (auto& hs : adminSystem.getStudents()) {
                ql.them(&hs);
            }

            Person* user = ql.dangNhap();

            if (user != nullptr) {

                SetColor(GREEN);
                cout << "Dang nhap thanh cong!\n";
                SetColor(WHITE);

                SinhVien* hs = dynamic_cast<SinhVien*>(user);

                if (hs != nullptr) {
                    hs->menu(thi);
                }
            }
            else {

                SetColor(RED);
                cout << "Dang nhap that bai!\n";
                SetColor(WHITE);
            }
        }

    } while (vaiTro != 0);

    return 0;
}



  
