# Ash
Ash (pronounced as the word ash, short for arrow shell) is a simple lightweight linux shell written in C
right now it's not yet ready for daily use, although if you are looking for something super lightweight but isn't /bin/sh, this is perfect for you

# Known Bugs
output redirection failed the only test I tried
  test command: `echo "is this working?" > test.txt`
  expected result: file `test.txt` be created and it's contents be `this is working`
  actual result: file `test.txt` was created and it's contentes were `"this is working" > test.txt`

# Planned Features
1. tab autocompletions (work in progress)
   pressing tab the first time should display possible completions, for paths it should provide the list of paths from the contents of the current directory, for the commands, it should provide them from places where binaries are usually stored, like /bin directory contents, subsequent tab precess should cycle between the possible completions, all my tests failed, resulting in segfaults that I couldn't trace back or fix, or they didn't work as intended at all, though one test worked perfectly for commands, segfaulted for paths, but I sorta accidentally deleted that one
2. syntax highlighting:
   I have no clue how this may even work
