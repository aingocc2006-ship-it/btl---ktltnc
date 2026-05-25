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

