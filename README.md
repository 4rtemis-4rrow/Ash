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
   
     I failed to trace it back using gdb, I am still trying to find it's source, but this is more advanced than my current skillset would allow

      after some tinkering with gdb, I managed to trace back the segfault to the FindMathces function, specifically in line 168, as of writing this, I need to go do something, I shall continue debugging when I'm free

      I did find a bug in the implementation of the autocompletion, however it wasn't related to the segfault, the segfault is happening at the call of strstr, I checked that it's arguments were indeed of the correct type, and that they were null terminated

# Planned Features
1. tab autocompletions (work in progress)
   pressing tab the first time should display possible completions, for paths it should provide the list of paths from the contents of the current directory, for the commands, it should provide them from places where binaries are usually stored, like /bin directory contents, subsequent tab precess should cycle between the possible completions, all my tests failed, resulting in segfaults that I couldn't trace back or fix, or they didn't work as intended at all, though one test worked perfectly for commands, segfaulted for paths, but I sorta accidentally deleted that one
   
3. syntax highlighting:
   I have no clue how this may even work
