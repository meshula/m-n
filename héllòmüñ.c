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

void banner(const char* msg) {
    printf("\n");
    printf("+--------------------------------------------------+\n");
    printf("| %s", msg);
    for (int i = 0; i < 49 - strlen(msg); i++) {
        printf(" ");
    }
    printf("|\n");
    printf("+--------------------------------------------------+\n");
}

#ifdef _MSC_VER


/// Converts regular std::string to UTF-16 windows string - Windows-only
wchar_t* ArchWindowsUtf8ToUtf16W(const wchar_t* str)
{
    int size = MultiByteToWideChar(CP_UTF8, 0, (LPCCH) str, -1, NULL, 0);
    if (size == 0) return NULL;
    printf("utf16 w required size is %d\n", size);
    wchar_t* new_str = (wchar_t*)malloc(size * sizeof(wchar_t));
    if (new_str == NULL) return NULL;
    if (MultiByteToWideChar(CP_UTF8, 0, (LPCCH) str, -1, new_str, size))
    {
        return new_str;
    }
    free(new_str);
    return NULL;
}
/// Converts regular std::string to UTF-16 windows string - Windows-only
wchar_t* ArchWindowsUtf8ToUtf16A(const char* str)
{
    int size = MultiByteToWideChar(CP_UTF8, 0, (LPCCH) str, -1, NULL, 0);
    if (size == 0) return NULL;
    printf("utf16 a required size is %d\n", size);
    wchar_t* new_str = (wchar_t*)malloc(size * sizeof(wchar_t));
    if (new_str == NULL) return NULL;
    if (MultiByteToWideChar(CP_UTF8, 0, (LPCCH) str, -1, new_str, size))
    {
        return new_str;
    }
    free(new_str);
    return NULL;
}


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

    banner("Size of wchar_t ");
    printf("  size of wchar_t is %d\n", (int) sizeof(wchar_t));

    banner("C umlaut U encodings");
    if (*umlaut_u == 0x00c3 && *(umlaut_u + 1) == 0xbc) {
        printf("wchar_t* umlaut_u = L\"ü\"; is utf8\n");
    }
    else if (*umlaut_u == 0x00fc) {
        printf("wchar_t* umlaut_u = L\"ü\"; is utf16\n");
    }
    else {
        printf("wchar_t* umlaut_u = L\"ü\"; is not utf8 or utf16\n");}
    if (*umlaut_a == 0xc3 && *(umlaut_a + 1) == 0xbc) {
        printf("unsigned char* umlaut_a = \"ü\"; is utf8\n");
    }
    else if (*umlaut_a == 0xfc) {
        printf("unsigned char* umlaut_a = \"ü\"; is utf16\n");
    }
    else {
        printf("unsigned char* umlaut_a = \"ü\"; is not utf8 or utf16\n");
    }
    if (*umlaut_w == 0x00c3 && *(umlaut_w + 1) == 0xbc) {
        printf("wchar_t* umlaut_w = u\"ü\"; is utf8\n");
    }
    else if (*umlaut_w == 0x00fc) {
        printf("wchar_t* umlaut_w = u\"ü\"; is utf16\n");
    }
    else {
        printf("wchar_t* umlaut_w = u\"ü\"; is not utf8 or utf16\n");

    }
    if (*umlaut_W == 0x00c3 && *(umlaut_W + 1) == 0xbc) {
        printf("unsigned int* umlaut_W = U\"ü\"; is utf8\n");
    }
    else if (*umlaut_W == 0x00fc) {
        printf("unsigned int* umlaut_W = U\"ü\"; is utf16\n");
    }
    else {
        printf("unsigned int* umlaut_W = U\"ü\"; is not utf8 or utf16\n");
    }
    if (*umlaut_uu == 0xc3bc) {
        printf("wchar_t* umlaut_uu = L\"\\uc3bc\"; is utf8\n");
    }
    else {
        printf("wchar_t* umlaut_uu = L\"\\uc3bc\"; is not utf8 or utf16\n");
    }
    
    banner("Check uft16 conversions");
    wchar_t* self_file_w = L"héllòmüñ.c";
    char* self_file_a = "héllòmüñ.c";
    wchar_t* self_file_w_u16 = ArchWindowsUtf8ToUtf16W(self_file_w);
    wchar_t* self_file_w_u8 = ArchWindowsUtf8ToUtf16A(self_file_a);

    printf(" L\"héllòmüñ.c\":");
    for (int i = 0; i < wcslen(self_file_w); i++) {
        printf(" %02X", (unsigned char) self_file_w[i]);
    }
    printf("\n as U16:       ");
    for (int i = 0; i < wcslen(self_file_w_u16); i++) {
        printf(" %02X", (unsigned char) self_file_w_u16[i]);
    }
    printf("\n \"héllòmüñ.c\": ");
    for (int i = 0; i < strlen(self_file_a); i++) {
        printf(" %02X", (unsigned char) self_file_a[i]);
    }
    printf("\n as U16:       ");
    for (int i = 0; i < wcslen(self_file_w_u8); i++) {
        printf(" %02X", (unsigned char) self_file_w_u8[i]);
    }
    printf("\n");

    banner("GetFileAttributesW/A");
    DWORD attribs_w = GetFileAttributesW(self_file_w);
    if (attribs_w == INVALID_FILE_ATTRIBUTES) {
        printf("GetFileAttributesW(u8\"%s\") failed: %d\n", self_file_a, GetLastError());
    }
    else {
        printf("GetFileAttributesW succeeded\n");
    }

    DWORD attribs = GetFileAttributesA(self_file_a);
    if (attribs == INVALID_FILE_ATTRIBUTES) {
        printf("GetFileAttributesA(u8\"%s\") failed: %d\n", self_file_a, GetLastError());
    }
    else {
        printf("GetFileAttributesA succeeded\n");
    }

    if (self_file_w_u16 == NULL) {
        printf("ArchWindowsUtf8ToUtf16 failed\n");
    }
    else {
        DWORD attribs_u16 = GetFileAttributesW(self_file_w_u8);
        if (attribs_u16 == INVALID_FILE_ATTRIBUTES) {
            printf("GetFileAttributesW(u16\"%s\") failed: %d\n", self_file_a, GetLastError());
        }
        else {
            printf("GetFileAttributesW(u16\"%s\") succeeded\n", self_file_a);
        }
    }

    banner("FindFirstFileW/A");
    WIN32_FIND_DATAA findfile_data;
    HANDLE find = FindFirstFileA("h*.c", &findfile_data);
    if (find == INVALID_HANDLE_VALUE) {
        printf("FindFirstFileA(\"h*.c\") failed: %d\n", GetLastError());
    }
    else {
        printf("FindFirstFileA(\"h*.c\") succeeded %s\n", findfile_data.cFileName);
        if (!strcmp(self_file_a, findfile_data.cFileName)) {
        }
        else {

        }
    }
    while (FindNextFileA(find, &findfile_data)) {
        printf("FindNextFileA succeeded %s\n", findfile_data.cFileName);
    }

    WIN32_FIND_DATAW findfile_dataw;
    find = FindFirstFileW(L"h*.c", &findfile_dataw);
    if (find == INVALID_HANDLE_VALUE) {
        printf("FindFirstFileW(L\"h*.c\") failed: %d\n", GetLastError());
    }
    else {
        bool match_failed = false;
        printf("FindFirstFileW(L\"h*.c\") succeeded %ls\n", findfile_dataw.cFileName);
        DWORD attribs_w = GetFileAttributesW(findfile_dataw.cFileName);
        if (attribs_w == INVALID_FILE_ATTRIBUTES) {
            printf("  GetFileAttributesW on found path failed: %d\n", GetLastError());
        }
        else {
            printf("  GetFileAttributesW on found path succeeded\n");
        }
    }
    while (FindNextFileW(find, &findfile_dataw)) {
        printf("FindNextFileW succeeded %ls\n", findfile_dataw.cFileName);
    }
    find = FindFirstFileW(self_file_w_u8, &findfile_dataw);
    if (find == INVALID_HANDLE_VALUE) {
        printf("FindFirstFileW(utf16 converted) failed: %d\n", GetLastError());
    }
    else {
        bool match_failed = false;
        printf("FindFirstFileW(utf16 converted) succeeded %ls\n", findfile_dataw.cFileName);
        DWORD attribs_w = GetFileAttributesW(findfile_dataw.cFileName);
        if (attribs_w == INVALID_FILE_ATTRIBUTES) {
            printf("  GetFileAttributesW on found path failed: %d\n", GetLastError());
        }
        else {
            printf("  GetFileAttributesW on found path succeeded\n");
        }
    }
    while (FindNextFileW(find, &findfile_dataw)) {
        printf("FindNextFileW succeeded %ls\n", findfile_dataw.cFileName);
    }

    banner("Finished");
}
#else
void test_conversions() {
    printf("test_conversions not implemented\n");
}
#endif

int main(void) {
    banner("Héllø Mün!");
    set_utf8_locale();
    test_conversions();
    return 0;
}

