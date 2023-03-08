
# héllòmüñ

This project explores the ins and outs of unicode characters in filenames 
in development workflows

Clone the project into a directory with a unicode character to start the experiment:

```sh
git clone https://github.com/meshula/m-n.git mün
```
On MSVC,

```sh
cl héllòmüñ.c
héllòmüñ.exe
```
various results and diagnostics ensue.

In the following output, we can conclude that only this declaration
```c
unsigned char* umlaut_a = "ü"; is utf8
```
can be converted by windows to utf16 in a form that the win sdk can understand.

C string literals declared with any adornments are not compatible with the win sdk.
Only `umlaut_a` is convertible by `MultiByteToWideChar` such that win sdk functions succeed.

Same for the wide char functions such as wcslen, they expect as input, u16 encoded strings.

```
+--------------------------------------------------+
| Héllø Mün!                                    |
+--------------------------------------------------+

+--------------------------------------------------+
| Size of wchar_t                                  |
+--------------------------------------------------+
  size of wchar_t is 2

+--------------------------------------------------+
| C umlaut U encodings                             |
+--------------------------------------------------+
wchar_t* umlaut_u = L"ü"; is utf8
c3 00 bc 00 00 00  wcslen is 2

unsigned char* umlaut_a = "ü"; is utf8
c3 bc 00 00  wcslen is 8

wchar_t* umlaut_w = u"ü"; is utf8
c3 00 bc 00 00 00  wcslen is 8

unsigned int* umlaut_W = U"ü"; is utf8
wchar_t* umlaut_uu = L"\uc3bc"; is utf8
c3 00 00 00 bc 00 00 00 00 00 00 00  wcslen is 8


+--------------------------------------------------+
| check wcslen(héllòmüñ), should be 8          |
+--------------------------------------------------+
utf16 a required size is 9
hello_u: 12
hello_a: 6
hello_w: 12
hello_W: 1
hello_uu: 8
hello_utf16: 8

+--------------------------------------------------+
| Check uft16 conversions                          |
+--------------------------------------------------+
utf16 a required size is 11
 L"héllòmüñ.c": 68 C3 A9 6C 6C C3 B2 6D C3 BC C3 B1 2E 63
 as U16:        68
 "héllòmüñ.c":  68 C3 A9 6C 6C C3 B2 6D C3 BC C3 B1 2E 63
 as U16:        68 E9 6C 6C F2 6D FC F1 2E 63

+--------------------------------------------------+
| GetFileAttributesW/A                             |
+--------------------------------------------------+
GetFileAttributesW(u8"héllòmüñ.c") failed: 2
GetFileAttributesA(u8"héllòmüñ.c") failed: 2
GetFileAttributesW(u16"héllòmüñ.c") succeeded

+--------------------------------------------------+
| FindFirstFileW/A                                 |
+--------------------------------------------------+
FindFirstFileA("h*.c") succeeded h�ll�m��.c
FindFirstFileW(L"h*.c") succeeded h�ll�m��.c
  GetFileAttributesW on found path succeeded
FindFirstFileW(utf16 converted) succeeded h�ll�m��.c
  GetFileAttributesW on found path succeeded

+--------------------------------------------------+
| Finished                                         |
+--------------------------------------------------+
```

Code for mac and linux to be added once the Windows side is fully explored.

## clang/gcc/msvc

- :butterfly: all compilers can compile the c file, the embedded utf8 string prints correctly
- :butterfly: msvc debugger displays utf8 correctly
- :bug: msvc debugger displays utf16 incorrectly
- :bug: msvc compile/link messages, compiler error windows: unicode not displayed consistently or properly

## cmake

- :butterfly: Unicode cmake project name is fine
- :bug: CMake can't use unicode for target names

```
CMake Error at CMakeLists.txt:3 (add_executable):
  The target name "héllòmün" is reserved or not valid for certain CMake
  features, such as generator expressions, and may result in undefined
  behavior.
```

- :butterfly: unicode filename for c file is fine in cmake


## boost

- :bug: boost build fails when the installation target is a path with a unicode character; every copy fails with a message showing a corrupt path. In the follow example, I tried to build to a directory named üsd-install.

```sh
 copy /b "C:\ï¿½sd-install\src\boost_1_78_0\boost\vmd\detail\recurse\data_equal\data_equal_9.hpp" + this-file-does-not-exist-A698EE7806899E69 "c:\ï¿½sd-install\include\boost-1_78\boost\vmd\detail\recurse\data_equal\data_equal_9.hpp"

...failed common.copy c:\ï¿½sd-install\include\boost-1_78\boost\vmd\detail\recurse\data_equal\data_equal_9.hpp...
common.copy c:\ï¿½sd-install\include\boost-1_78\boost\vmd\detail\recurse\data_equal\data_equal_headers.hpp
The system cannot find the path specified.
```

- :bug: bjam creates a corrupt directory name during the installation.
```sh
ⁿsd-install
```

## git

- :bug: Can't add unicode filename to git on mac

```sh
git add "h\303\251ll\303\262m\303\274\303\261.c"
fatal: pathspec 'h\303\251ll\303\262m\303\274\303\261.c' did not match any files
```

solution:
```sh
git config --global core.precomposeunicode true
git add héllòmüñ.c
git status

Changes to be committed:
...
	new file:   "h\303\251ll\303\262m\303\274\303\261.c"
```

- :bug: Git branches with characters in their names that are illegal in filenames on Windows lead to problems because branch names are used as filenames in the .git directory: https://github.com/git-for-windows/git/issues/2904 It's possible to create and push such a branch on macOs and cause errors on Windows. (Thanks @Simran)

## github

- :bug:  github create mün - repo created as m-n
- :butterfly: héllòmüñ.c appears on github as héllòmüñ.c
- :butterfly: modify héllòmüñ.c locally and push
- :butterfly: clone on mac
- :butterfly: clone on mac without core.precomposeunicode true flag
- :butterfly: clone on windows
- :butterfly: touch file on windows, commit, push
- :butterfly: touch file on linux, commit, push

## perforce

- :butterfly: add héllòmüñ.c to a changelist

## notes

- :books: UTF8 everywhere https://utf8everywhere.org
- :books: OpenAssetIO string encoding https://github.com/OpenAssetIO/OpenAssetIO/blob/main/doc/decisions/DR005-String-encoding.md
- :books: Fixing Unix/Linux/POSIX Filenames: Control Characters (such as Newline), Leading Dashes, and Other Problems, David A. Wheeler, 15 Nov 2020 https://dwheeler.com/essays/fixing-unix-linux-filenames.html



