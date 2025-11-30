Cyberbullying Text Analyzer
----------------------------

Installation:
1) Download and extract the zip file to any folder

Requirements
1) MinGW GCC Compiler
2) Visual Studio Code
3) Toxic Words List.txt (provided)
4) Stop Words List.txt (provided)
5) Any .txt file to be analyzed (< 1 MB) 

How to compile and run :
1) Open main.c using VS Code
2) Install and enable the C/C++ extension by Microsoft
3) Install and enable Code Runner extension by Jun Han
4) On the top right corner of the code panel, click the play icon and select "Run C/C++ file" -> "C/C++:gcc.exe build and debug active file"
5) Program will start running
6) Minimize VS Code and open "main.exe"

*Alternative Method:
1) Open main.c using VS Code
2) Open a new powershell terminal
3) Paste the following command: cd "C:\VS Code\New Folder" , where "C:\VS Code\New Folder" is the file directory where your program is stored
4) Compile the program with this command: gcc main.c -o main
5) Run the program with this command: ./main.exe

Configurables:

Toxic Words List.txt and Stop Words List.txt can be opened and configured, however a specific format should be followed.

1) In Toxic Words List.txt, the format is as follows:
   toxic_phrase1
   severity (mild/moderate/severe)

   toxic_phrase2
   severity (mild/moderate/severe)

   toxic_phrase2
   severity (mild/moderate/severe)

2) In Stop Words List.txt, the format is as follows:
   stop_word1
   stop_word2
   stop_word3

**Ensure each word does not end or start with a space, for it will lead to a comparison error.


**IMPORTANT: 
1) All file names provided in the zip file should not be renamed under any circumstance, otherwise reading failure will occur
2) All C header files provided should be included in the main.c source file
3) All required files should be located in the same directory as the program.
   

