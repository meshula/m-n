
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

- :books: Fixing Unix/Linux/POSIX Filenames: Control Characters (such as Newline), Leading Dashes, and Other Problems, David A. Wheeler, 15 Nov 2020 https://dwheeler.com/essays/fixing-unix-linux-filenames.html



