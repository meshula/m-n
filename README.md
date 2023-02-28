
# Readme

- cc can compile the c file, the embedded utf8 string prints correctly

- Unicode cmake project name is fine

- CMake can't use unicode for target names

```
CMake Error at CMakeLists.txt:3 (add_executable):
  The target name "héllòmün" is reserved or not valid for certain CMake
  features, such as generator expressions, and may result in undefined
  behavior.
```

- not demonstrated here, but, boost build fails when the installation target is a path with a unicode character; every copy fails with a message showing a corrupt path, e.g.

```
 copy /b "C:\XXX\ï¿½sd-install-2023-02-24\src\boost_1_78_0\boost\vmd\detail\recurse\data_equal\data_equal_9.hpp" + this-file-does-not-exist-A698EE7806899E69 "c:\XXX\ï¿½sd-install-2023-02-24\include\boost-1_78\boost\vmd\detail\recurse\data_equal\data_equal_9.hpp"

...failed common.copy c:\XXX\ï¿½sd-install-2023-02-24\include\boost-1_78\boost\vmd\detail\recurse\data_equal\data_equal_9.hpp...
common.copy c:\XXX\ï¿½sd-install-2023-02-24\include\boost-1_78\boost\vmd\detail\recurse\data_equal\data_equal_headers.hpp
The system cannot find the path specified.
```

Also, bjam creates a corrupt directory name during the installation.
```
ⁿsd-install-2023-02-24
```

- unicode filename for c file is fine in cmake

Can't add unicode filename to git

```
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

- github create mün
repo created as m-n

héllòmüñ.c appears on github as héllòmüñ.c

- modify héllòmüñ.c locally and push

success

- clone on mac

success

- clone on windows

success

- touch file on windows, commit, push

success

- touch file on linux, commit, push

success

- pull on mac without core.precomposeunicode true flag

success
