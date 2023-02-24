
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
