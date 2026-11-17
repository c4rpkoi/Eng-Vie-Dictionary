#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tu dien tra cuu Anh - Viet */

#define MAX_TU    50     /* do dai toi da cua tu tieng Anh */
#define MAX_NGHIA 100    /* do dai toi da cua nghia tieng Viet */
#define MAX_VIDU  150    /* do dai toi da cua cau vi du */
#define TEN_FILE  "tudien.txt"

/* mot tu trong tu dien - la 1 nut cua danh sach lien ket */
struct TuVung {
    char Tu[MAX_TU];
    char Nghia[MAX_NGHIA];
    char ViDu[MAX_VIDU];
    struct TuVung *Next;
};

typedef struct TuVung TuVung;

/* xoa ky tu Enter ma fgets de lai o cuoi chuoi */
void XoaEnter(char *chuoi) {
    int i = 0;
    while (chuoi[i] != '\0' && chuoi[i] != '\n') {
        i++;
    }
    chuoi[i] = '\0';
}

void NhapChuoi(char *chuoi, int gioiHan) {
    fgets(chuoi, gioiHan, stdin);
    XoaEnter(chuoi);
}

/* kiem tra chuoi co phai toan chu so khong (tra ve 1 neu dung) */
int LaSo(char *chuoi) {
    int i = 0;
    if (chuoi[0] == '\0') {
        return 0;
    }
    while (chuoi[i] != '\0') {
        if (chuoi[i] < '0' || chuoi[i] > '9') {
            return 0;
        }
        i++;
    }
    return 1;
}

/* dung lai man hinh, cho nguoi dung bam Enter roi moi ve menu */
void ChoBamEnter(void) {
    printf("\nBam Enter de ve menu...");
    getchar();
}

TuVung* TaoTuMoi(const char *tu, const char *nghia, const char *viDu) {
    TuVung *tuMoi = (TuVung *) malloc(sizeof(TuVung));
    if (tuMoi == NULL) {
        printf("Loi: khong du bo nho!\n");
        exit(1);
    }
    strcpy(tuMoi->Tu, tu);
    strcpy(tuMoi->Nghia, nghia);
    strcpy(tuMoi->ViDu, viDu);
    tuMoi->Next = NULL;
    return tuMoi;
}

/* tim tu bang de quy: dung thi tra ve, sai thi de quy xuong nut sau */
TuVung* TimTu(TuVung *dau, const char *tu) {
    if (dau == NULL) {
        return NULL;
    }
    if (strcmp(tu, dau->Tu) == 0) {
        return dau;
    }
    return TimTu(dau->Next, tu);
}

/* chen 1 tu moi vao dung vi tri de danh sach luon sap xep theo a-z (de quy) */
TuVung* ThemTu(TuVung *dau, TuVung *tuMoi) {
    if (dau == NULL || strcmp(tuMoi->Tu, dau->Tu) < 0) {
        tuMoi->Next = dau;
        return tuMoi;
    }
    dau->Next = ThemTu(dau->Next, tuMoi);
    return dau;
}

/* tim nut can xoa bang vong lap, noi lai danh sach roi free nut do */
TuVung* XoaTu(TuVung *dau, const char *tu, int *daXoa) {
    TuVung *truoc = NULL;
    TuVung *hienTai = dau;

    *daXoa = 0;
    while (hienTai != NULL && strcmp(hienTai->Tu, tu) != 0) {
        truoc = hienTai;
        hienTai = hienTai->Next;
    }

    if (hienTai == NULL) {
        return dau;
    }

    if (truoc == NULL) {
        dau = hienTai->Next;
    } else {
        truoc->Next = hienTai->Next;
    }

    free(hienTai);
    *daXoa = 1;
    return dau;
}

void InTatCaTu(TuVung *dau) {
    if (dau == NULL) {
        printf("Tu dien dang rong.\n");
        return;
    }
    int stt = 1;
    TuVung *p = dau;
    while (p != NULL) {
        printf("%d. %s\n", stt, p->Tu);
        printf("   Nghia : %s\n", p->Nghia);
        printf("   Vi du : %s\n", p->ViDu);
        p = p->Next;
        stt++;
    }
}

void GiaiPhongDanhSach(TuVung *dau) {
    TuVung *p = dau;
    while (p != NULL) {
        TuVung *ke = p->Next;
        free(p);
        p = ke;
    }
}

/* moi tu ghi 3 dong trong file: tu / nghia / vi du */
void GhiFile(TuVung *dau) {
    FILE *f = fopen(TEN_FILE, "w");
    if (f == NULL) {
        printf("Khong mo duoc file de ghi!\n");
        return;
    }
    TuVung *p = dau;
    while (p != NULL) {
        fprintf(f, "%s\n%s\n%s\n", p->Tu, p->Nghia, p->ViDu);
        p = p->Next;
    }
    fclose(f);
    printf("Da luu tu dien vao file %s\n", TEN_FILE);
}

/* doc lai file 3-dong-1-tu, chua co file thi coi nhu tu dien rong */
TuVung* DocFile(TuVung *dau) {
    FILE *f = fopen(TEN_FILE, "r");
    if (f == NULL) {
        printf("Chua co file du lieu, bat dau voi tu dien rong.\n");
        return dau;
    }

    char tu[MAX_TU], nghia[MAX_NGHIA], viDu[MAX_VIDU];
    int soTu = 0;

    while (fgets(tu, MAX_TU, f) != NULL) {
        XoaEnter(tu);

        fgets(nghia, MAX_NGHIA, f);
        XoaEnter(nghia);

        fgets(viDu, MAX_VIDU, f);
        XoaEnter(viDu);

        TuVung *tuMoi = TaoTuMoi(tu, nghia, viDu);
        dau = ThemTu(dau, tuMoi);
        soTu++;
    }

    fclose(f);
    printf("Da doc %d tu tu file %s\n", soTu, TEN_FILE);
    return dau;
}

void MenuThem(TuVung **dau) {
    char tu[MAX_TU], nghia[MAX_NGHIA], viDu[MAX_VIDU];

    printf("Nhap tu tieng Anh: ");
    NhapChuoi(tu, MAX_TU);
    if (tu[0] == '\0') {
        printf("Tu khong duoc de trong!\n");
        return;
    }

    if (TimTu(*dau, tu) != NULL) {
        printf("Tu \"%s\" da co trong tu dien!\n", tu);
        return;
    }

    printf("Nhap nghia tieng Viet: ");
    NhapChuoi(nghia, MAX_NGHIA);
    if (nghia[0] == '\0') {
        printf("Nghia khong duoc de trong!\n");
        return;
    }

    printf("Nhap cau vi du (co the de trong): ");
    NhapChuoi(viDu, MAX_VIDU);

    TuVung *tuMoi = TaoTuMoi(tu, nghia, viDu);
    *dau = ThemTu(*dau, tuMoi);
    printf("Da them tu \"%s\" vao tu dien.\n", tu);
}

void MenuTraCuu(TuVung *dau) {
    char tu[MAX_TU];
    printf("Nhap tu can tra cuu: ");
    NhapChuoi(tu, MAX_TU);

    TuVung *ketQua = TimTu(dau, tu);
    if (ketQua == NULL) {
        printf("Khong tim thay tu \"%s\" trong tu dien.\n", tu);
    } else {
        printf("Tu    : %s\n", ketQua->Tu);
        printf("Nghia : %s\n", ketQua->Nghia);
        printf("Vi du : %s\n", ketQua->ViDu);
    }
}

void MenuSua(TuVung *dau) {
    char tu[MAX_TU];
    printf("Nhap tu can sua: ");
    NhapChuoi(tu, MAX_TU);
    if (tu[0] == '\0') {
        printf("Tu khong duoc de trong!\n");
        return;
    }

    TuVung *ketQua = TimTu(dau, tu);
    if (ketQua == NULL) {
        printf("Khong tim thay tu \"%s\" trong tu dien.\n", tu);
        return;
    }

    printf("Tu \"%s\" hien co:\n", ketQua->Tu);
    printf("   Nghia : %s\n", ketQua->Nghia);
    printf("   Vi du : %s\n", ketQua->ViDu);

    char nghiaMoi[MAX_NGHIA], viDuMoi[MAX_VIDU];

    /* de trong thi giu nguyen, khong ghi de bang chuoi rong */
    printf("Nhap nghia moi (Enter de giu nguyen): ");
    NhapChuoi(nghiaMoi, MAX_NGHIA);
    if (nghiaMoi[0] != '\0') {
        strcpy(ketQua->Nghia, nghiaMoi);
    }

    printf("Nhap vi du moi (Enter de giu nguyen): ");
    NhapChuoi(viDuMoi, MAX_VIDU);
    if (viDuMoi[0] != '\0') {
        strcpy(ketQua->ViDu, viDuMoi);
    }

    printf("Da cap nhat tu \"%s\".\n", ketQua->Tu);
}

void MenuXoa(TuVung **dau) {
    char tu[MAX_TU];
    printf("Nhap tu can xoa: ");
    NhapChuoi(tu, MAX_TU);
    if (tu[0] == '\0') {
        printf("Tu khong duoc de trong!\n");
        return;
    }

    int daXoa;
    *dau = XoaTu(*dau, tu, &daXoa);

    if (daXoa) {
        printf("Da xoa tu \"%s\".\n", tu);
    } else {
        printf("Khong tim thay tu \"%s\" trong tu dien.\n", tu);
    }
}

void InMenu(void) {
    printf("\n--- TU DIEN TRA CUU ANH - VIET ---\n");
    printf("1. Them tu moi\n");
    printf("2. Sua nghia / vi du cua mot tu\n");
    printf("3. Xoa mot tu\n");
    printf("4. Tra cuu mot tu\n");
    printf("5. Hien thi toan bo tu dien\n");
    printf("6. Luu tu dien ra file ngay\n");
    printf("0. Thoat\n");
    printf("Chon chuc nang: ");
}

int main(void) {
    TuVung *dau = NULL;
    dau = DocFile(dau);
    ChoBamEnter();   /* cho doc xong thong bao da doc file truoc khi xoa man hinh */

    char dongChon[10];
    int chon;

    do {
        system("cls");
        InMenu();
        NhapChuoi(dongChon, 10);

        /* go chu hoac de trong thi coi la chon sai, khong de atoi tra ve 0 (= Thoat) */
        if (LaSo(dongChon)) {
            chon = atoi(dongChon);
        } else {
            chon = -1;
        }

        switch (chon) {
            case 1:
                MenuThem(&dau);
                ChoBamEnter();
                break;
            case 2:
                MenuSua(dau);
                ChoBamEnter();
                break;
            case 3:
                MenuXoa(&dau);
                ChoBamEnter();
                break;
            case 4:
                MenuTraCuu(dau);
                ChoBamEnter();
                break;
            case 5:
                InTatCaTu(dau);
                ChoBamEnter();
                break;
            case 6:
                GhiFile(dau);
                ChoBamEnter();
                break;
            case 0:
                GhiFile(dau);   /* luu truoc khi thoat */
                printf("Tam biet!\n");
                break;
            default:
                printf("Ban chon sai roi, vui long thu lai!\n");
                ChoBamEnter();
        }
    } while (chon != 0);

    GiaiPhongDanhSach(dau);
    return 0;
}
