# Ash
Ash (pronounced as the word ash, short for arrow shell) is a simple lightweight linux shell written in C
right now it's not yet ready for daily use, although if you are looking for something super lightweight but isn't /bin/sh, this is perfect for you

NOTE: pretty much everything that follows is logs of what I'm working on/what I tried to fix bugs/what I managed to learn about them, this is so it's easier for anyone who wants to commit to this project

# Known Bugs
1. output redirection failed the only test I tried
   
    test command: `echo "is this working?" > test.txt`
   
    expected result: file `test.txt` be created and it's contents be `this is working`
   
    actual result: file `test.txt` was created and it's contentes were `is this working? > test.txt`
   
3. SEGFAULT when pressing tab for completions
   
      backtracing it using GDB indicates that the issue isn't in my code, but rather the readline library, here is the output of backtrace in case it helps anyone

```
(gdb) backtrace
#0  0x00007ffff7ec7ffb in ?? () from /usr/lib/libc.so.6
#1  0x00007ffff7d906c6 in ?? () from /usr/lib/libc.so.6
#2  0x00007ffff7d90621 in ?? () from /usr/lib/libc.so.6
#3  0x00007ffff7d90b66 in qsort_r () from /usr/lib/libc.so.6
#4  0x00007ffff7f55ee9 in remove_duplicate_matches (matches=<optimized out>) at ../complete.c:1270
#5  postprocess_matches (matchesp=matchesp@entry=0x7fffffffb700, matching_filenames=0) at ../complete.c:1471
#6  0x00007ffff7f5fd41 in rl_complete_internal (what_to_do=9) at ../complete.c:2057
#7  0x00007ffff7f57430 in _rl_dispatch_subseq (key=9, map=<optimized out>, got_subseq=0) at ../readline.c:916
#8  0x00007ffff7f579c6 in _rl_dispatch (key=<optimized out>, map=<optimized out>) at ../readline.c:860
#9  0x00007ffff7f58248 in readline_internal_char () at ../readline.c:675
#10 0x00007ffff7f6154e in readline_internal_charloop () at ../readline.c:721
#11 readline_internal () at ../readline.c:733
#12 readline (prompt=prompt@entry=0x5555555560d2 ">>> ") at ../readline.c:387
#13 0x00005555555552d1 in main () at main.c:276
```

# Planned Features
1. tab autocompletions (work in progress)
   pressing tab the first time should display possible completions, for paths it should provide the list of paths from the contents of the current directory, for the commands, it should provide them from places where binaries are usually stored, like /bin directory contents, subsequent tab precess should cycle between the possible completions, all my tests failed, resulting in segfaults that I couldn't trace back or fix, or they didn't work as intended at all, though one test worked perfectly for commands, segfaulted for paths, but I sorta accidentally deleted that one
   
3. syntax highlighting:
   I have no clue how this may even work
