#include <stdio.h>
#include <stdbool.h>

#ifdef _MSC_VER
#include <Windows.h>
#endif

void set_utf8_locale(void) {
#ifdef _MSC_VER
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#else
    setlocale(LC_ALL, "en_US.UTF-8");
#endif
}

#ifdef _MSC_VER
void test_conversions() {

    // as you will see below, wchar_t L strings are utf8, each character padded to 16 bits
    // note that L strings are implementation-defined value and size with no standard encoding
    // unsigned char* strings are 8 bit utf8 characters
    // lower case u strings are utf8, with each character padded to 16 bits
    // upper case u strings are utf8, with each character padded to 32 bits
    // In the description above, note that the padding is not per glyph.
    // For example, one might expect that the glyph "ü" would be encoded as 0xc3 0xbc,
    // but it is actually encoded as 0x00 0xc3 0x00 0xbc.
    // There appears to be no way to specify a utf16 string where each glyph occupies two bytes,
    // in a clear text manner. The only way appears to be use the "\u" escape sequence.
    
    wchar_t* umlaut_u = L"ü";
    unsigned char* umlaut_a = "ü";
    wchar_t* umlaut_w = u"ü";
    unsigned int* umlaut_W = U"ü";
    wchar_t* umlaut_uu = L"\uc3bc";

    printf("mark1\n");
    printf("  size of wchar_t is %d\n", (int) sizeof(wchar_t));
    if (*umlaut_u == 0x00fc) {
        printf("Umlaut U encoded as an L string is utf16: should be 0x00fc, is: %x\n", *umlaut_u);
        printf("  second character is %x\n", *(umlaut_u + 1));
    }
    else {
        printf("Umlaut U encoded as an L string is not utf16: should be 0x00fc, is: %x\n", *umlaut_u);
        printf("  second character is %x\n", *(umlaut_u + 1));
    }
    if (*umlaut_a == 0xc3 && *(umlaut_a + 1) == 0xbc) {
        printf("Umlaut A encoded as an char string is utf8: should be 0xc3, is %x\n", *umlaut_a);
        printf(" second character is %x\n", *(umlaut_a + 1));
    }
    else {
        printf("Umlaut A encoded as an char string is not utf8: should be 0xc3, is %x\n", *umlaut_a);
        printf(" second character is %x\n", *(umlaut_a + 1));
    }
    if (*umlaut_w == 0x00fc) {
        printf("Umlaut w encoded as an u8 string is utf16: should be 0x00fc, is: %x\n", *umlaut_w);
        printf("  second character is %x\n", *(umlaut_w + 1));
    }
    else {
        printf("Umlaut w encoded as an u8 string is not utf16: should be 0x00fc, is: %x\n", *umlaut_w);
        printf("  second character is %x\n", *(umlaut_w + 1));
    }
    if (*umlaut_W == 0x00fc) {
        printf("Umlaut w encoded as an U32 string is utf16: should be 0x00fc, is: %x\n", *umlaut_W);
        printf("  second character is %x\n", *(umlaut_W + 1));
    }
    else {
        printf("Umlaut w encoded as an U32 string is not utf16: should be 0x00fc, is: %x\n", *umlaut_W);
        printf("  second character is %x\n", *(umlaut_W + 1));
    }
    if (*umlaut_uu == 0xc3bc) {
        printf("Umlaut uu encoded as an L string is utf16: should be 0xc3bc, is: %x\n", *umlaut_uu);
    }
    else {
        printf("Umlaut uu encoded as an L string is not utf16: should be 0xc3bc, is: %x\n", *umlaut_uu);
    }

    wchar_t* self_file_w = L"héllòmüñ.c";
    DWORD attribs_w = GetFileAttributesW(self_file_w);
    if (attribs_w == INVALID_FILE_ATTRIBUTES) {
        printf("GetFileAttributesW failed: %d\n", GetLastError());
    }
    else {
        printf("GetFileAttributesW succeeded\n");
    }

    char* self_file_a = "héllòmüñ.c";
    DWORD attribs = GetFileAttributesA(self_file_a);
    if (attribs == INVALID_FILE_ATTRIBUTES) {
        printf("GetFileAttributesA failed: %d\n", GetLastError());
    }
    else {
        printf("GetFileAttributesA succeeded\n");
    }

    WIN32_FIND_DATAA findfile_data;
    HANDLE find = FindFirstFileA("h*.c", &findfile_data);
    if (find == INVALID_HANDLE_VALUE) {
        printf("FindFirstFileA failed: %d\n", GetLastError());
    }
    else {
        printf("FindFirstFileA succeeded %s\n", findfile_data.cFileName);
        if (!strcmp(self_file_a, findfile_data.cFileName)) {
            printf("FindFirstFileA filename matches char* string %s\n", self_file_a);
        }
        else {
            printf("FindFirstFileA filename doesn't match char* string %s\n", self_file_a);
            printf("     Wn U8\n");
            for (int i = 0; i < strlen(findfile_data.cFileName); i++) {
                printf("  %d: %x %x\n", i, (unsigned char) findfile_data.cFileName[i], (unsigned char) self_file_a[i]);
            }
        }
    }
    while (FindNextFileA(find, &findfile_data)) {
        printf("FindNextFileA succeeded %s\n", findfile_data.cFileName);
    }

    WIN32_FIND_DATAW findfile_dataw;
    find = FindFirstFileW(L"h*.c", &findfile_dataw);
    if (find == INVALID_HANDLE_VALUE) {
        printf("FindFirstFileW failed: %d\n", GetLastError());
    }
    else {
        bool match_failed = false;
        printf("FindFirstFileW succeeded %ls\n", findfile_dataw.cFileName);
        if (!wcscmp(self_file_w, findfile_dataw.cFileName)) {
            printf("FindFirstFileW filename matches char* string %ls\n", self_file_w);
        }
        else {
            printf("FindFirstFileW filename doesn't match wchar* string %ls\n", self_file_w);
            printf("     Wn U8w\n");
            for (int i = 0; i < wcslen(findfile_dataw.cFileName); i++) {
                printf("  %d: %x %x\n", i, (unsigned char) findfile_dataw.cFileName[i], (unsigned char) self_file_w[i]);
            }
        }
        DWORD attribs_w = GetFileAttributesW(findfile_dataw.cFileName);
        if (attribs_w == INVALID_FILE_ATTRIBUTES) {
            printf("GetFileAttributesW on found path failed: %d\n", GetLastError());
        }
        else {
            printf("GetFileAttributesW on found path succeeded\n");
        }
    }
    while (FindNextFileW(find, &findfile_dataw)) {
        printf("FindNextFileW succeeded %ls\n", findfile_dataw.cFileName);
    }

    printf("mark3\n");
}
#else
void test_conversions() {
    printf("test_conversions not implemented\n");
}
#endif

int main(void) {
    set_utf8_locale();
    test_conversions();
    printf("Héllø Mün!\n");
    return 0;
}

