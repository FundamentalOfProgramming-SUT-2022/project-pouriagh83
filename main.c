#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <windows.h>
#include <conio.h>
#include <dirent.h>
void arman();
int armanFind;
int lineNUmberGrep;
int isCommanValid;
int numberOfDirectoryWeGoInto = 0;
char giveGrepString[10000];
int grepOption;
int armanOffOrOn;
int armanGrep;
int armanForPrintGrep;
char armanString[10000];
void backToMainFolder()
{
for(int i = 0; i < numberOfDirectoryWeGoInto; i++)
{
chdir("..");
}
numberOfDirectoryWeGoInto = 0;
}

void makePervious(char *nameOfFile)
{
    char name[10000];
    for(int i = 0;;i++)
    {
    strcpy(name, "POF1");
    for(int j = 0; j < i; j++){
    strcat(name, "1");}
    strcat(name, "pervious");
    strcat(name, nameOfFile);
    FILE *check = fopen(name, "r");
    if(check == NULL)
    {
        fclose(check);
        break;
    }
    else
    {
        fclose(check);
        memset(name, 0, 10000);
    }
    }
    FILE *perviousVersion = fopen(name, "w");
    FILE *now = fopen(nameOfFile, "r");
    char a = getc(now);
    while(a != EOF)
    {
        fputc(a, perviousVersion);
        a = getc(now);
    }
    fclose(now);
    fclose(perviousVersion);
    DWORD attributes = GetFileAttributes(name);
    SetFileAttributes(name, attributes + FILE_ATTRIBUTE_HIDDEN);
}

void createFile(char *nameOfFile)
{
    FILE *check = fopen(nameOfFile, "r");
   if(check == NULL)
   {
    fclose(check);
    FILE *makeFile = fopen(nameOfFile, "w");
    fclose(makeFile);
   }
   else
   {
      printf("The file has already been created!\n");
      fclose(check);
   }
   isCommanValid = 1;
}

void createDir()
{
    numberOfDirectoryWeGoInto = 0;
    struct stat st = {0};
    char nameOfDir[1000];
    getchar();
    if(getchar() == '"')
       getchar();
    int index = 0;
    while(index != -1)
    {
    memset(nameOfDir, 0, 1000);
    while((nameOfDir[index] = getchar()) != '/')
    {
        if(nameOfDir[index] == '"')
        {
        index++;
        nameOfDir[index] = getchar();
        }
        if(nameOfDir[index] == '\n')
        {
        nameOfDir[index] = '\0';
        if(nameOfDir[index - 1] == '"')
           nameOfDir[index - 1] = '\0';
        createFile(nameOfDir);
        index = -1;
        break;
        }
        index++;
    }
    if(index != -1)
    {
        numberOfDirectoryWeGoInto++;
     nameOfDir[index] = '\0';
        if(stat(nameOfDir, &st) == -1)
           mkdir(nameOfDir);
        chdir(nameOfDir);
        index = 0;
    }
    }
    backToMainFolder();
}

void setStringInSpecified(char *fileName)
{
 char *inputedString = malloc(1000000 * sizeof(char));
 if(armanOffOrOn == 0){
 int index = 0, weHaveSpace = 0;
 while(1)
 {
    scanf("%c", &inputedString[index]);
    if(index == 0 && inputedString[index] == '"')
    {
       weHaveSpace = 1;
       continue;
    }
    if(inputedString[index] == '\\')
    {
        inputedString[index] = getchar();
        if(inputedString[index] == 'n')
            inputedString[index] = '\n';
        else if(inputedString[index] == '"')
           inputedString[index] = '"';
        else if(inputedString[index] == '*')
           inputedString[index] = '*';
        else if(inputedString[index] == '\\')
        {
            char trash;
            if((trash = getchar()) == 'n')
            {
                index++;
                inputedString[index] = 'n';
            }
            else
            {
               inputedString[index] = '\\';
               index++;
               inputedString[index] = trash;
            }
        }
    }
    else if(inputedString[index] == ' ' && weHaveSpace == 0)
    {
       inputedString[index] = '\0';
       break;
    }
    else if(inputedString[index] == '"' && weHaveSpace == 1)
    {
       inputedString[index] = '\0';
       break;
    }
    index++;
 }
    }
    if(armanOffOrOn) strcat(inputedString, armanString);
 char command[100];
 scanf("%s", command);
 if(strcmp(command, "--pos") == 0)
 {
    makePervious(fileName);
    getchar();
    int lineNumber, charNumber;
    scanf("%d", &lineNumber);
    getchar();
    scanf("%d", &charNumber);
    FILE *openForWrite = fopen(fileName, "r");
    FILE *tmpFile = fopen("tmp.txt", "w");
    int whichLine = 1, whichChar = 0;
    char temp = 'a';
    while(whichLine != lineNumber)
    {
        temp = getc(openForWrite);
        if(temp != EOF)
            putc(temp, tmpFile);
        else
           putc('\n', tmpFile);
        if(temp == '\n' || temp == EOF)
           whichLine++;
    }
    while(whichChar != charNumber)
    {
        temp = getc(openForWrite);
        if(temp == EOF)
           putc(' ', tmpFile);
        else
           putc(temp, tmpFile);
        whichChar++;
    }
    fputs(inputedString, tmpFile);
    while(temp != EOF)
    {
        temp = getc(openForWrite);
        if(temp != EOF)
        putc(temp, tmpFile);
    }
    fclose(openForWrite);
    fclose(tmpFile);
    remove(fileName);
    rename("tmp.txt", fileName);
    isCommanValid = 1;
 }
}
void catFile(char *nameOfFile)
{
    FILE *openToRead = fopen(nameOfFile, "r");
    char character;
    while(character != EOF)
    {
    character = getc(openToRead);
    if(character == EOF)
       break;
    if(armanOffOrOn == 0)
       printf("%c", character);
    else
       strncat(armanString, &character, 1);
    isCommanValid = 1;
    }
    fclose(openToRead);
}

void removeStr(char *nameOfFile)
{
int lineNumber, charNumber, numberOfChar;
char backOrForward, command[20];
scanf("%d", &lineNumber);getchar();scanf("%d", &charNumber);
scanf("%s", command);
if(strcmp(command, "-size") == 0)
{
scanf("%d", &numberOfChar);
getchar();getchar();
backOrForward = getchar();
char character;
if(backOrForward == 'b' || backOrForward == 'f')
{
    makePervious(nameOfFile);
FILE *openToRead = fopen(nameOfFile, "r");
FILE *tempFile = fopen("tmp.txt", "w");
int whichLineWeStand = 1, whichCharWeStand = 0;
    if(backOrForward == 'b')
    {
    int sizeMalloc = 10;
    char *saveChar = malloc(sizeMalloc * sizeof(char));
    int countCharInFile = 0;
    int index = 0;
    while(whichLineWeStand != lineNumber)
    {
        if(countCharInFile + 1 == sizeMalloc){sizeMalloc += 2; realloc(saveChar, sizeMalloc);}
        saveChar[index] = getc(openToRead);
        countCharInFile++;
        if(saveChar[index] == '\n')
            whichLineWeStand++;
        index++;
    }
    while(charNumber != whichCharWeStand)
    {
        if(countCharInFile + 1 == sizeMalloc){sizeMalloc += 2; realloc(saveChar, sizeMalloc);}
        saveChar[index] = getc(openToRead);
        countCharInFile++;
        whichCharWeStand++;
        index++;
    }
    for(int i = 0;i < countCharInFile - numberOfChar;i++)
    {
        fputc(saveChar[i], tempFile);
    }
    char save;
    while(save != EOF)
    {
        save = getc(openToRead);
        if(save != EOF)
            putc(save, tempFile);
    }
    isCommanValid = 1;
    }
    else if(backOrForward == 'f')
    {
    while(whichLineWeStand != lineNumber)
          {
          character = getc(openToRead);
          putc(character, tempFile);
          if(character == '\n')
             whichLineWeStand++;
          }
          while(whichCharWeStand != charNumber + 1)
          {
            character = getc(openToRead);
            putc(character, tempFile);
            whichCharWeStand++;
          }
          for(int i = 0; i < numberOfChar; i++)
          {
            character = getc(openToRead);
          }
          while(1)
          {
            character = getc(openToRead);
            if(character == EOF)
               break;
            putc(character, tempFile);
          }
          isCommanValid = 1;
          }
          fclose(openToRead);
          fclose(tempFile);
          remove(nameOfFile);
          rename("tmp.txt", nameOfFile);
}
}
}

void copyStr(char *nameOfFile)
{
char command[20];
scanf("%s", command);
if(strcmp(command, "--pos") == 0)
{
    remove("U:/BasicProgramming/Project/Codes/clipboard.txt");
    FILE *clipboard = fopen("U:/BasicProgramming/Project/Codes/clipboard.txt", "w");
    int lineNumber, charNumber;
    scanf("%d", &lineNumber);
    getchar();
    scanf("%d", &charNumber);
    scanf("%s", command);
    if(strcmp(command, "-size") == 0)
    {
        int numberOfCharacterToCopy;
        scanf("%d", &numberOfCharacterToCopy);
        getchar();getchar();
        char backOrForward = getchar();
        int whichLineWeStand = 1, whichCharWeStand = 0;
        FILE *openToRead = fopen(nameOfFile, "r");
        if(backOrForward == 'f')
        {
            while(lineNumber != whichLineWeStand)
            {
             if(getc(openToRead) == '\n')
                whichLineWeStand++;
            }
            while(charNumber != whichCharWeStand)
            {
                getc(openToRead);
                whichCharWeStand++;
            }
            getc(openToRead);
            for(int i = 0; i < numberOfCharacterToCopy; i++){
                char a = getc(openToRead);
                putc(a, clipboard);
            }
            isCommanValid = 1;
        }
        else if(backOrForward == 'b')
        {
            int howManyCharWeSaved = 0;
            while(lineNumber != whichLineWeStand)
            {
                char a = getc(openToRead);
                fputc(a, clipboard);
                if(a == '\n')
                   whichLineWeStand++;
                howManyCharWeSaved++;
            }
            while(charNumber != whichCharWeStand)
            {
                char a = getc(openToRead);
                fputc(a, clipboard);
                whichCharWeStand++;
                howManyCharWeSaved++;
            }
            fseek(clipboard, 0, SEEK_SET);
            for(int i = 0; i < howManyCharWeSaved - numberOfCharacterToCopy; i++)
            {
                fputc(EOF, clipboard);
            }
            isCommanValid = 1;
        }
        fclose(clipboard);
        fclose(openToRead);
        DWORD attributes = GetFileAttributes("U:/BasicProgramming/Project/Codes/clipboard.txt");
        SetFileAttributes("U:/BasicProgramming/Project/Codes/clipboard.txt", attributes + FILE_ATTRIBUTE_HIDDEN);
    }
}
}

 void cutStr(char *nameOfFile)
 {
char command[20];
scanf("%s", command);
if(strcmp(command, "--pos") == 0)
{
    int lineNumber, charNumber;
    scanf("%d", &lineNumber);
    getchar();
    scanf("%d", &charNumber);
    scanf("%s", command);
    if(strcmp(command, "-size") == 0)
    {
        makePervious(nameOfFile);
        remove("U:/BasicProgramming/Project/Codes/clipboard.txt");
        int numberOfCharacterToCopy;
        scanf("%d", &numberOfCharacterToCopy);
        getchar();getchar();
        char backOrForward = getchar();
        int whichLineWeStand = 1, whichCharWeStand = 0;
        FILE *openToRead = fopen(nameOfFile, "r");
        FILE *clipboard = fopen("U:/BasicProgramming/Project/Codes/clipboard.txt", "w");
        if(backOrForward == 'f')
        {
            while(lineNumber != whichLineWeStand)
            {
             if(getc(openToRead) == '\n')
                whichLineWeStand++;
            }
            while(charNumber != whichCharWeStand)
            {
                getc(openToRead);
                whichCharWeStand++;
            }
            getc(openToRead);
            for(int i = 0; i < numberOfCharacterToCopy; i++){
                char a = getc(openToRead);
                putc(a, clipboard);
            }
        }
        else if(backOrForward == 'b')
        {
            int howManyCharWeSaved = 0;
            while(lineNumber != whichLineWeStand)
            {
                char a = getc(openToRead);
                putc(a, clipboard);
                if(a == '\n')
                   whichLineWeStand++;
                howManyCharWeSaved++;
            }
            while(charNumber != whichCharWeStand)
            {
                char a = getc(openToRead);
                putc(a, clipboard);
                whichCharWeStand++;
                howManyCharWeSaved++;
            }
            fseek(clipboard, 0, SEEK_SET);
            for(int i = 0; i < howManyCharWeSaved - numberOfCharacterToCopy; i++)
            {
                fputc(EOF, clipboard);
            }
        }
        fseek(openToRead, 0, SEEK_SET);
        whichCharWeStand = 0;
        whichLineWeStand = 1;
        FILE *tempFile = fopen("tmp.txt", "w");
if(backOrForward == 'b')
    {
        int sizeMalloc = 10;
    char *saveChar = malloc(sizeMalloc * sizeof(char));
    int countCharInFile = 0;
    int index = 0;
    while(whichLineWeStand != lineNumber)
    {
        if(countCharInFile + 1 == sizeMalloc){sizeMalloc += 2; realloc(saveChar, sizeMalloc);}
        saveChar[index] = getc(openToRead);
        countCharInFile++;
        if(saveChar[index] == '\n')
            whichLineWeStand++;
        index++;
    }
    while(charNumber != whichCharWeStand)
    {
        if(countCharInFile + 1 == sizeMalloc){sizeMalloc += 2; realloc(saveChar, sizeMalloc);}
        saveChar[index] = getc(openToRead);
        countCharInFile++;
        whichCharWeStand++;
        index++;
    }
    for(int i = 0;i < countCharInFile - numberOfCharacterToCopy;i++)
    {
        fputc(saveChar[i], tempFile);
    }
    char save;
    while(save != EOF)
    {
        save = getc(openToRead);
        if(save != EOF)
            putc(save, tempFile);
    }
    isCommanValid = 1;
    }
    else if(backOrForward == 'f')
    {
        char character;
    while(whichLineWeStand != lineNumber)
          {
          character = getc(openToRead);
          putc(character, tempFile);
          if(character == '\n')
             whichLineWeStand++;
          }
          while(whichCharWeStand != charNumber + 1)
          {
            character = getc(openToRead);
            putc(character, tempFile);
            whichCharWeStand++;
          }
          for(int i = 0; i < numberOfCharacterToCopy; i++)
          {
            character = getc(openToRead);
          }
          while(1)
          {
            character = getc(openToRead);
            if(character == EOF)
               break;
            putc(character, tempFile);
          }
          isCommanValid = 1;
          }
          fclose(clipboard);
          fclose(openToRead);
          fclose(tempFile);
          remove(nameOfFile);
          rename("tmp.txt", nameOfFile);
          DWORD attributes = GetFileAttributes("U:/BasicProgramming/Project/Codes/clipboard.txt");
          SetFileAttributes("U:/BasicProgramming/Project/Codes/clipboard.txt", attributes + FILE_ATTRIBUTE_HIDDEN);
}
}
}

void pasteStr(char *fileName)
 {
    int lineNumber, charNumber;
    scanf("%d", &lineNumber);
    getchar();
    scanf("%d", &charNumber);
    makePervious(fileName);
    FILE *clipboard = fopen("U:/BasicProgramming/Project/Codes/clipboard.txt", "r");
    FILE *openForWrite = fopen(fileName, "r");
    FILE *tmpFile = fopen("tmp.txt", "w");
    int whichLine = 1, whichChar = 0;
    char temp;
   while(whichLine != lineNumber)
    {
        temp = getc(openForWrite);
        if(temp != EOF)
            putc(temp, tmpFile);
        else
           putc('\n', tmpFile);
        if(temp == '\n' || temp == EOF)
           whichLine++;
    }
    while(whichChar != charNumber)
    {
        temp = getc(openForWrite);
        if(temp == EOF)
           putc(' ', tmpFile);
        else
           putc(temp, tmpFile);
        whichChar++;
    }
    char a = getc(clipboard);
    while(a == EOF)
    {
        a = getc(clipboard);
    }
    putc(a, tmpFile);
    a = getc(clipboard);
    while(a != EOF)
    {
        fputc(a, tmpFile);
        a = getc(clipboard);
    }
    while(temp != EOF)
    {
        temp = getc(openForWrite);
        if(temp != EOF)
        putc(temp, tmpFile);
    }
    fclose(openForWrite);
    fclose(tmpFile);
    fclose(clipboard);
    remove(fileName);
    rename("tmp.txt", fileName);
    isCommanValid = 1;
 }

void undo(char *nameOfFile)
{
char namePervious[10000];
for(int i = 0;; i++)
{
strcpy(namePervious, "POF1");
for(int j = 0; j < i; j++){strcat(namePervious, "1");}
strcat(namePervious, "pervious");
strcat(namePervious, nameOfFile);
FILE *check = fopen(namePervious, "r");
if(check == NULL)
{
    if(i == 0) return;
 fclose(check);
 memset(namePervious, 0, 10000);
 strcpy(namePervious, "POF1");
 for(int j = 1; j < i; j++){strcat(namePervious, "1");}
 strcat(namePervious, "pervious");
 strcat(namePervious, nameOfFile);
 break;
}
else
{
fclose(check);
memset(namePervious, 0, 10000);
}
}
remove(nameOfFile);
DWORD attributes = GetFileAttributes(namePervious);
SetFileAttributes(namePervious, attributes + FILE_ATTRIBUTE_HIDDEN);
rename(namePervious, nameOfFile);
isCommanValid = 1;
}

void autoIndent(char *nameOfFile)
{
    makePervious(nameOfFile);
    FILE *opentoRead = fopen("t.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    int numberOfBraces = 0, numberOfSpace = 0;
    char prevent = '\0';
    char a = getc(opentoRead);
    while(a != EOF)
    {
    if(a == '{')
    {
        if(prevent == '\n')
           for(int i = 0; i < 4 * numberOfBraces; i++){fputc(' ', temp);}
        if(prevent != ' ' && prevent != SEEK_SET && prevent != '\n')
           fputc(' ', temp);
        if(prevent == '}'){
           fputc('\n', temp);for(int i = 0; i < 4 * numberOfBraces; i++){fputc(' ', temp);}}
        numberOfBraces++;
        fputc(a, temp);
        fputc('\n', temp);
        prevent = '\n';
    }
    else if(a == '}')
    {
        numberOfBraces--;
        if(prevent != '\n')
        fputc('\n', temp);
        for(int i = 0; i < 4*numberOfBraces; i++){fputc(' ', temp);}
        fputc(a, temp);
        prevent = '}';
    }
    else if(a == ' ')
    {
        char b;
        if(prevent == '}')
        {
            fputc('\n', temp);
            prevent = '\n';
            continue;
        }
        if(prevent == '\n' || prevent == '{')
        {
            while((b = getc(opentoRead)) == ' ');
            a = b;
            continue;
        }
     numberOfSpace++;
     while((b = getc(opentoRead)) == ' '){numberOfSpace++;}
     if(b == '{')
     {
        a = b;
        numberOfSpace = 0;
        continue;
     }
     else if(b == '}')
     {
        a = b;
        numberOfSpace = 0;
        continue;
     }
     else if(prevent == '\n')
     {
        for(int i = 0; i < 4 * numberOfBraces; i++){fputc(' ', temp);}
        for(int i = 0; i < numberOfSpace; i++){fputc(' ', temp);}
        a = b;
        prevent = ' ';
        numberOfSpace = 0;
        continue;
     }
     else
     {
        for(int i = 0; i < numberOfSpace; i++){fputc(' ', temp);}
        a = b;
        prevent = ' ';
        numberOfSpace = 0;
        continue;
     }
     numberOfSpace = 0;
    }
    else
    {
        if(prevent == '}')
           fputc('\n', temp);
        if(prevent == '}' || prevent == '\n')
        {
            for(int i = 0; i < 4 * numberOfBraces; i++){fputc(' ', temp);}
        }
        fputc(a, temp);
        prevent = a;
    }
    a = getc(opentoRead);
    }
    fclose(temp);
    fclose(opentoRead);
    remove(nameOfFile);
    rename("temp.txt", nameOfFile);
}

void compare(FILE* file1, FILE* file2)
{
    int linenumber = 0;
    char line1[10000], line2[10000];
    int length1 = 1, length2 = 1;
    char a = fgetc(file1);
    while(a != EOF)
    {
        if(a == '\n')
           length1++;
        a = fgetc(file1);
    }
    a = fgetc(file2);
    while(a != EOF)
    {
        if(a == '\n')
           length2++;
        a = fgetc(file2);
    }
    fseek(file1, 0, SEEK_SET);
    fseek(file2, 0, SEEK_SET);
    while(1)
    {
        linenumber++;
        fgets(line1, sizeof(line1), file1);
        fgets(line2 ,sizeof(line2), file2);
        if(strcmp(line1, line2))
        {
            if(armanOffOrOn == 0){
            printf("========== #%d ==========\n", linenumber);
            printf("%s\n", line1);
            printf("%s\n", line2);}
            else
            {
                char x[100];
                sprintf(x, "%d", linenumber);
                strcat(armanString, "========== #");
                strcat(armanString, x);
                strcat(armanString,  "==========\n");
                strcat(armanString, line1);
                strcat(armanString, "\n");
                strcat(armanString, line2);
                strcat(armanString, "\n");
            }
            break;
        }
    }
    if(length1 > length2)
    {
        for(int i = linenumber; i < length2; i++)
        {
            fgets(line1, sizeof(line1), file1);
        }
        if(armanOffOrOn == 0) printf("<<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<<\n", length2 + 1, length1);
        else
        {
            char a[1000];
            sprintf(a, "<<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<<\n", length2 + 1, length1);
            strcat(armanString, a);
        } 
        char a = getc(file1);
        while(a != EOF)
        {
        if(armanOffOrOn == 0) printf("%c", a);
        else strncat(armanString, &a, 1);
        a = getc(file1);
        }
        if(armanOffOrOn == 0) printf("\n");
        else strcat(armanString, "\n");
    }
    else if(length2 > length1)
    {
        for(int i = linenumber; i < length1; i++)
        {
            fgets(line2, sizeof(line2), file2);
        }
        if(armanOffOrOn == 0) printf(">>>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>>>\n", length1 + 1, length2);
        else
        {
            char x[1000];
            sprintf(x, ">>>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>>>\n", length1 + 1, length2);
            strcat(armanString, x);
        }
        char a = getc(file2);
        while(a != EOF)
        {
            if(armanOffOrOn == 0) printf("%c", a);
            else strncat(armanString, &a, 1);
            a = getc(file2);
        }
        if(armanOffOrOn == 0) printf("\n");
    }
}

int findFunction(char* nameOfFile, char* string, int count, int at, int byword, int all, int starIndex, int weWannaIndex)
{
if(at == 0 && all == 0 && count == 0 && byword == 0)
{
   if(starIndex == -1)
   {
      FILE* openToRead = fopen(nameOfFile, "r");
      int weHaveEnter = 0;
      int index = 0;
      char a;
      a = getc(openToRead);
      if(a == '\n') weHaveEnter++;
      while(a != EOF)
      {
        if(a == string[0])
        {
            int output = index;
            int check = 1;
            for(int i = 1; i < strlen(string); i++)
            {
                a = getc(openToRead);
                if(a == '\n') weHaveEnter++;
                index++;
                if(a != string[i])
                {
                   fseek(openToRead, output + 1 + weHaveEnter, SEEK_SET);
                   index = output;
                   check = 0;
                   break;
                }
            }
            if(check && weWannaIndex){
                fclose(openToRead);
                return index;
            }
            else if(check){
              fclose(openToRead);
              return output;
            }
        }
        a = getc(openToRead);
        if(a == '\n') weHaveEnter++;
        index++;
      }
      fclose(openToRead);
      return -1;
   }
   else
   {
    if(starIndex == strlen(string) - 1)
    {
        string[strlen(string) - 1] = '\0';
        if(weWannaIndex)
        {
        int x = findFunction(nameOfFile, string, 0, 0, 0, 0, -1, 1);
        FILE *openToFind = fopen(nameOfFile, "r");
        fseek(openToFind, x, SEEK_SET);
        char a = getc(openToFind);int here = 0;
        while(a != ' ' && a != EOF && a != '\n' && a != '\0')
        {
            here++;
            a = getc(openToFind);
        }
        fclose(openToFind);
        return x + here - 1;
        }
        return findFunction(nameOfFile, string, 0, 0, 0, 0, -1, 0);
    }
    else if(starIndex == 0)
    {
        for(int i = 0; i < strlen(string) - 1; i++)
        {
            string[i] = string[i + 1];
        }
        string[strlen(string) - 1] = '\0';
        if(weWannaIndex)
           return findFunction(nameOfFile, string, 0, 0, 0, 0, -1, 1);
        int x = findFunction(nameOfFile, string, 0, 0, 0, 0, -1, 0);
        FILE* openToRead = fopen(nameOfFile, "r");
        for(int i = 0; i <= x; i++)
        {
            fseek(openToRead, x - i, SEEK_SET);
            char a = getc(openToRead);
            if(a == ' ' || a == '\0' || a == '\n')
            {
               fclose(openToRead);
               return x - i + 1;
            }
        }
        fclose(openToRead);
        if(x == -1) return -1;
        return 0;
    }
    else
    {
        FILE* openToRead = fopen(nameOfFile, "r");
        int weHaveEnter = 0;
        if(string[starIndex + 1] == ' ')
        {
            int index = 0, check = 1;
            int output;
            char a = getc(openToRead);
            if(a == '\n') weHaveEnter++;
            while(a != EOF)
            {
                check = 1;
                if(a == string[0])
                {
                    output = index;
                    for(int i = 1; i < starIndex; i++)
                    {
                     a = getc(openToRead);
                     if(a == '\n') weHaveEnter++;
                     index++;
                     if(a != string[i])
                     {
                      fseek(openToRead, output + 1 + weHaveEnter, SEEK_SET);
                      index = output;
                      check = 0;
                      break;
                     }
                    }
                    if(check)
                    {
                    a = getc(openToRead);
                    if(a == '\n') weHaveEnter++;
                    index++;
                    while(a != EOF && a != ' ' && a != '\0')
                    {
                        a = getc(openToRead);
                        index++;
                    }
                    a = getc(openToRead);
                    if(a == '\n') weHaveEnter++;
                    index++;
                    if(string[starIndex + 2] == a)
                    {
                    for(int i = starIndex + 3; i < strlen(string); i++)
                    {
                        a = getc(openToRead);
                        if(a == '\n') weHaveEnter++;
                        index++;
                        if(a != string[i])
                        {
                            fseek(openToRead, output + 1 + weHaveEnter, SEEK_SET);
                            index = output;
                            check = 0;
                            break;
                        }
                    }
                    if(check && weWannaIndex)
                    {
                        fclose(openToRead);
                        return index;
                    }
                    else if(check)
                    {
                       fclose(openToRead);
                       return output;
                    }
                    }
                    fseek(openToRead, output + 1 + weHaveEnter, SEEK_SET);
                    }
                }
                a = getc(openToRead);
                index++;
            }
            fclose(openToRead);
            return -1;
        }
        else
        {
            int weHaveEnter = 0;
            char a = getc(openToRead);
            if(a == '\n') weHaveEnter++;
            int index = 0, check = 1, output, back = 0;
            while(a != EOF)
            {
                check = 1;
                if(a == string[0])
                {
                    output = index;
                 for(int i = 1; i < starIndex; i++)
                 {
                    a = getc(openToRead);
                    if(a == '\n') weHaveEnter++;
                    index++;back++;
                    if(a != string[i])
                    {
                        check = 0;
                        index = output;
                        back = output;
                        fseek(openToRead, output + 1 + weHaveEnter, SEEK_SET);
                        break;
                    }
                 }
                 if(check)
                 {
                    a = getc(openToRead);
                    if(a == '\n') weHaveEnter++;
                    back++;index++;
                    if(a == ' ' || a == '\0' || a == EOF || a == '\n')
                    {
                        check = 0;
                        index = output;
                        back = output;
                        fseek(openToRead, output + 1 + weHaveEnter, SEEK_SET);
                        break;
                    }
                    while(a != ' ' && a != '\0' && a != EOF && a != '\n')
                    {
                     check = 1;
                     if(a == string[starIndex + 1])
                     {
                        for(int i = starIndex + 2; i < strlen(string); i++)
                        {
                            a = getc(openToRead);
                            if(a == '\n') weHaveEnter++;
                            index++;
                            if(a != string[i])
                            {
                                index = back;
                                fseek(openToRead,back + 1 + weHaveEnter, SEEK_SET);
                                check = 0;
                                break;
                            }
                        }
                        if(check && weWannaIndex)
                        {
                            fclose(openToRead);
                            return index;
                        }
                        if(check)
                        {
                        fclose(openToRead);
                        return output;
                        }
                     }
                     a = getc(openToRead);
                     if(a == '\n') weHaveEnter++;
                     index++;
                     back++;
                    }
                 }
                 index = output;
                 back = output;
                 fseek(openToRead, output + 1 + weHaveEnter, SEEK_SET);
                }
                a = getc(openToRead);
                if(a == '\n') weHaveEnter++;
                index++;
                back++;
            }
            fclose(openToRead);
               return -1;
        }
    }
   }
}
else if(at == 0 && all == 0 && byword == 0 && count == 1)
{
int i;char string2[10000];
strcpy(string2, string);
for(i = 1;;i++)
{
    int x = findFunction(nameOfFile, string, 0, i, 0, 0, starIndex, 0);
    strcpy(string, string2);
    if(x == -1) break;
}
return i - 1;
}

else if(at && all == 0 && byword == 0 && count == 0)
{
    char string2[10000];
    strcpy(string2, string);
    int y = 0, x, weHaveEnter = 0;
    for(int i = 1; i < at; i++)
    {
     FILE* openToCopy = fopen(nameOfFile, "r");
     FILE* temp = fopen("tmp.txt", "w");
     fseek(openToCopy, y, SEEK_SET);
     weHaveEnter = 0;
     char a = getc(openToCopy);
     while(a != EOF)
     {
       putc(a, temp);
       a = getc(openToCopy);
       if(a == '\n') weHaveEnter++;
     }
     fclose(temp);
     fclose(openToCopy);
     x = findFunction("tmp.txt", string, 0, 0, 0, 0, starIndex, 1);
     y += x + 1;
     if(x == -1)
     {
        remove("tmp.txt");
        return -1;
     }
     strcpy(string, string2);
    }
    remove("tmp.txt");
    FILE* openToCopy = fopen(nameOfFile, "r");
    FILE* temp = fopen("tmp.txt", "w");
    fseek(openToCopy, y, SEEK_SET);
     char a = getc(openToCopy);
     while(a != EOF)
     {
       putc(a, temp);
       a = getc(openToCopy);
     }
     fclose(temp);
     fclose(openToCopy);
    if(weWannaIndex)
        x = findFunction("tmp.txt", string, 0, 0, 0, 0, starIndex, 1);
    else
        x = findFunction("tmp.txt", string, 0, 0, 0, 0, starIndex, 0);
    remove("tmp.txt");
    if(x == -1) return x;
    return x + y;
}

else if(at == 0 && all == 0 && byword == 1 && count == 0)
{
   int x = findFunction(nameOfFile, string, 0, 0, 0, 0, starIndex, 0);
   if(x == -1)
      return x;
    FILE* openToRead = fopen(nameOfFile, "r");
    int numberOfSpace = 0;
   for(int i = 0; i < x; i++)
   {
     char a = fgetc(openToRead);
     if(a == ' ' || a == '\n')
        numberOfSpace++;
   }
   fclose(openToRead);
   return numberOfSpace + 1;
}

else if(at == 0 && all == 1 && byword == 0 && count == 0)
{
    char string2[10000];
    strcpy(string2, string);
    for(int i = 0;; i++)
    {
        int x = findFunction(nameOfFile, string, 0, i + 1, 0, 0, starIndex, 0);
        strcpy(string, string2);
        if(x == -1)
        {
           if(armanOffOrOn == 0) printf("\n");
           return 0;
        }
           if(i > 0){
           if(armanOffOrOn == 0) printf(" ,");
           else strcat(armanString, " ,");
           }
       if(armanOffOrOn == 0) printf("%d", x);
       else{char a[100]; sprintf(a, "%d", x);
       strcat(armanString, a);}
    }
}

else if(at && all == 0 && byword == 1 && count == 0)
{
    char string2[10000];
    strcpy(string2, string);
    int x = findFunction(nameOfFile, string, 0, at, 0, 0, starIndex, 0);
    strcpy(string, string2);
    if(x == -1)
      return x;
    FILE* openToRead = fopen(nameOfFile, "r");
    int numberOfSpace = 0;
    for(int i = 0; i < x; i++)
    {
     char a = fgetc(openToRead);
     if(a == ' ' || a == '\n')
        numberOfSpace++;
    }
    fclose(openToRead);
    return numberOfSpace + 1;
}

else if(at == 0 && all == 1 && byword == 1 && count == 0)
{
    for(int i = 1;; i++)
    {
    int x = findFunction(nameOfFile, string, 0, i, 1, 0, starIndex, 0);
    if(x == -1 && i == 1)
     {
     if(armanOffOrOn == 0) printf("-1\n");
     else strcat(armanString, "-1");
     return -1;
     }
    if(x == -1)
    {
       if(armanOffOrOn == 0) printf("\n");
        return 0;
    }
    if(i > 1)
    {
       if(armanOffOrOn == 0) printf(", ");
       else strcat(armanString, ", ");
    }
    if(armanOffOrOn == 0) printf("%d", x);
    else
    {
        char a[100];
        sprintf(a, "%d", x);
        strcat(armanString, a);
    }
    }
}

}
void tree(int depth, char *nameOfFileOrDir, int firstDepth)
{
if(depth == 0)
   return;
struct dirent *entry;
DIR *dir = opendir(nameOfFileOrDir);
if(dir == NULL)
   return;
while((entry = readdir(dir)) != NULL)
{
     if(entry -> d_name[0] != '.' && (entry -> d_name[0] != 'P' || entry -> d_name[1] != 'O' || entry -> d_name[2] != 'F' || entry -> d_name[3] != '1'))
     {
        for(int i = 0; i < firstDepth - depth; i++)
        { 
            if(armanOffOrOn == 0)
               printf("|  ");
            else
               strcat(armanString, "|  ");
        }
        if(armanOffOrOn == 0)
           printf("|--%s", entry -> d_name);
        else
        {
           strcat(armanString, "|--");
           strcat(armanString, entry ->d_name);
        }
        if(entry -> d_type == DT_DIR)
        {
            if(armanOffOrOn == 0)
               printf(":\n");
            else
               strcat(armanString, ":\n");
            char nameOfDir[100];
            strcpy(nameOfDir, nameOfFileOrDir);
            strcat(nameOfDir, "/");
            strcat(nameOfDir, entry -> d_name);
            tree(depth - 1, nameOfDir, firstDepth);
        }
        else
        {
            if(armanOffOrOn == 0)
               printf("\n");
            else
               strcat(armanString, "\n");
        }
        
     }
}
closedir(dir);
}
void replaceString(char *nameOfFile)
{
   char command[1000];
   scanf("%s", command);
   if(!strcmp(command, "--str1"))
   {
            getchar();
            char *str1 = malloc(1000000 * sizeof(char));
            int index = 0, weHaveSpace = 0, star = -1;
            while(1)
            {
            scanf("%c", &str1[index]);
            if(str1[index] == '*')
               star = index;
            if(index == 0 && str1[index] == '"')
             {
             weHaveSpace = 1;
             continue;
             }
            if(str1[index] == '\\')
            {
            str1[index] = getchar();
            if(str1[index] == 'n')
               str1[index] = '\n';
            else if(str1[index] == '"')
               str1[index] = '"';
            else if(str1[index] == '*')
               str1[index] = '*';
            else if(str1[index] == '\\')
            {
            char trash;
            if((trash = getchar()) == 'n')
            {
                index++;
                str1[index] = 'n';
            }
            else
            {
               str1[index] = '\\';
               index++;
               str1[index] = trash;
            }
            }
            }
            else if(str1[index] == ' ' && weHaveSpace == 0)
            {
            str1[index] = '\0';
            break;
            }
            else if(str1[index] == '"' && weHaveSpace == 1)
            {
            str1[index] = '\0';
            break;
            }
            index++;
            }
    scanf("%s", command);
    if(!strcmp(command, "--str2"))
    {
            getchar();
            char *str2 = malloc(1000000 * sizeof(char));
            index = 0, weHaveSpace = 0;
            int option = 0;
            while(1)
            {
            scanf("%c", &str2[index]);
            if(index == 0 && str2[index] == '"')
             {
             weHaveSpace = 1;
             continue;
             }
            if(str2[index] == '\\')
            {
            str1[index] = getchar();
            if(str2[index] == 'n')
               str2[index] = '\n';
            else if(str2[index] == '"')
               str2[index] = '"';
            else if(str2[index] == '*')
               str2[index] = '*';
            else if(str2[index] == '\\')
            {
            char trash;
            if((trash = getchar()) == 'n')
            {
                index++;
                str2[index] = 'n';
            }
            else
            {
               str2[index] = '\\';
               index++;
               str2[index] = trash;
            }
            }
            }
            else if(str2[index] == ' ' && weHaveSpace == 0)
            {
            str2[index] = '\0';
            option = 1;
            break;
            }
            else if(str2[index] == '"' && weHaveSpace == 1)
            {
            str2[index] = '\0';
            if(getchar() == ' ')
               option = 1;
            break;
            }
            else if(str2[index] == '\n')
            {
                str2[index] = '\0';
                option = 0;
                break;
            }
            index++;
            }
            int at = 0, all = 0;
            if(option)
            {
                while(1){
                scanf("%s", command);
                if(!strcmp(command, "-at"))
                    scanf("%d", &at);
                else if(!strcmp(command, "-all"))
                   all = 1;
                if(getchar() == '\n') break;
                }
                if(at && all) {printf("Invalid option\n"); return;}
            }
            isCommanValid = 1;
            makePervious(nameOfFile);
            if(all == 0)
            {
                if(at == 0) at = 1;
                char hey[10000];
                strcpy(hey, str1);
                int startIndex = findFunction(nameOfFile, str1, 0, at, 0, 0, star, 0);
                int endIndex = findFunction(nameOfFile, hey, 0, at, 0, 0, star, 1);
                if(startIndex == -1) {printf("String not found!\n");return;}
                FILE *openToRead = fopen(nameOfFile, "r");
                FILE *temp = fopen("tmp.txt", "w");
                for(int i = 0; i < startIndex; i++)
                {
                 char a = fgetc(openToRead);
                 fputc(a, temp);
                }
                for(int i = 0; i < strlen(str2); i++)
                {
                    fputc(str2[i], temp);
                }
                for(int i = 0; i <= endIndex - startIndex; i++)
                {
                    getc(openToRead);
                }
                for(int i = 0;;i++)
                {
                    char a = fgetc(openToRead);
                    if(a == EOF)
                       break;
                    fputc(a, temp);
                }
                fclose(openToRead);
                fclose(temp);
                remove(nameOfFile);
                rename("tmp.txt", nameOfFile);
                return;
            }
            else if(all)
            {
             while(1)
             {
                char hey[10000];
                strcpy(hey, str1);
               int startIndex = findFunction(nameOfFile, str1, 0, 1, 0, 0, star, 0);
               int endIndex = findFunction(nameOfFile, hey, 0, 1, 0, 0, star, 1);
               if(startIndex == -1) return;
                FILE *openToRead = fopen(nameOfFile, "r");
                FILE *temp = fopen("tmp.txt", "w");
                for(int i = 0; i < startIndex; i++)
                {
                 char a = fgetc(openToRead);
                 fputc(a, temp);
                }
                for(int i = 0; i < strlen(str2); i++)
                {
                    fputc(str2[i], temp);
                }
                for(int i = 0; i <= endIndex - startIndex; i++)
                {
                    fgetc(openToRead);
                }
                for(int i = 0;;i++)
                {
                    char a = fgetc(openToRead);
                    if(a == EOF)
                       break;
                    fputc(a, temp);
                }
                fclose(openToRead);
                fclose(temp);
                remove(nameOfFile);
                rename("tmp.txt", nameOfFile);
             }
            }
    }
   }
}

char* grepString()
{
            getchar();
            char *inputedString = malloc(1000000 * sizeof(char));
            int index = 0, weHaveSpace = 0;
            while(1)
            {
            scanf("%c", &inputedString[index]);
            if(index == 0 && inputedString[index] == '"')
             {
             weHaveSpace = 1;
             continue;
             }
            if(inputedString[index] == '\\')
            {
            inputedString[index] = getchar();
            if(inputedString[index] == 'n')
               inputedString[index] = '\n';
            else if(inputedString[index] == '"')
               inputedString[index] = '"';
            else if(inputedString[index] == '*')
               inputedString[index] = '*';
            else if(inputedString[index] == '\\')
            {
            char trash;
            if((trash = getchar()) == 'n')
            {
                index++;
                inputedString[index] = 'n';
            }
            else
            {
               inputedString[index] = '\\';
               index++;
               inputedString[index] = trash;
            }
            }
            }
            else if(inputedString[index] == ' ' && weHaveSpace == 0)
            {
            inputedString[index] = '\0';
            break;
            }
            else if(inputedString[index] == '"' && weHaveSpace == 1)
            {
            inputedString[index] = '\0';
            break;
            }
            index++;
            }
            return inputedString;
}
char saveGrep[100000];
void grep(char* nameOfFile)
{
    int previous = -1;
    isCommanValid = 1;
    if(armanGrep) strcpy(giveGrepString, armanString);
 if(grepOption == -1)
 {
  for(int i = 1;;i++)
  {
  int x = findFunction(nameOfFile, giveGrepString, 0, i, 0, 0, -1, 0);
  FILE *check = fopen(nameOfFile, "r");
  fclose(check);
  if(x == -1) break;
  FILE* openToPrint = fopen(nameOfFile, "r");
  int j;
  for(j = 0; j <= x; j++)
  {
   fseek(openToPrint, x - j, SEEK_SET);
   if(x - j != 0)
   if(getc(openToPrint) == '\n') break;
  }
  if(j == x + 1) j = x;
  if(x - j == previous) {continue; fclose(openToPrint);}
  previous = x - j;
  char why[10000];
  sprintf(why, "%s:   ", nameOfFile);
  strcat(saveGrep, why);
  char a = getc(openToPrint);
  while(a != '\n' && a != EOF)
  {
   strncat(saveGrep, &a, 1);
   a = getc(openToPrint);
  }
  strcat(saveGrep, "\n");
  fclose(openToPrint);
  }
 }
 else if(grepOption == 0)
 {
  int x = findFunction(nameOfFile, giveGrepString, 0, 0, 0, 0, -1, 0);
  if(x == -1) return;;
  FILE* openToPrint = fopen(nameOfFile, "r");
  int j;
  for(j = 0; j <= x; j++)
  {
   fseek(openToPrint, x - j, SEEK_SET);
   if(x - j != 0)
   if(getc(openToPrint) == '\n') break;
  }
  char why[10000];
  sprintf(why, "%s", nameOfFile);
  strcat(saveGrep, why);
  char a = getc(openToPrint);
  strcat(saveGrep, "\n");
  fclose(openToPrint);
 }
 else if(grepOption == 1)
 {
  for(int i = 1;;i++)
  {
  int x = findFunction(nameOfFile, giveGrepString, 0, i, 0, 0, -1, 0);
  if(x == -1) break;
  FILE* openToPrint = fopen(nameOfFile, "r");
  int j;
  for(j = 0; j <= x; j++)
  {
   fseek(openToPrint, x - j, SEEK_SET);
   if(x - j != 0)
   if(getc(openToPrint) == '\n') break;
  }
  if(j == x + 1) j = x;
  if(x - j == previous) {continue; fclose(openToPrint);}
  previous = x - j;
  char a = getc(openToPrint);
  lineNUmberGrep++;
  fclose(openToPrint);
  }
 }
}

int checkArman()
{
    char command[100];
    scanf("%s", command);
    if(!strcmp(command, "=D"))
       return 1;
    return 0;
}

FILE* file1;
void goToDir(char *checkCommand)
{
    int weHaveAnotherLoc = 0, armanForCommands = 0, option = 0;
int weHaveSpace = 0;
struct stat st = {0};
char nameOfDir[1000];
if(strcmp(checkCommand, "compare1") && strcmp(checkCommand, "grepDir"))
    getchar();
if(getchar() == '"'){
   getchar();
   weHaveSpace = 1;}
int index = 0;
while(index != -1)
{
    memset(nameOfDir, 0, 1000);
    while((nameOfDir[index] = getchar()) != '/')
    {
        if(index == 0 && nameOfDir[0] == 'D' && !strcmp(checkCommand, "grepDir")) {armanOffOrOn = 1;armanForPrintGrep = 1; arman(); return;}
        if(!strcmp(checkCommand, "grepDir") && ((nameOfDir[index] == '"' && weHaveSpace == 1 && getchar() == ' ') || (nameOfDir[index] == ' ' && weHaveSpace == 0)))
           weHaveAnotherLoc = 1;
        if(!strcmp(checkCommand, "cat") && ((nameOfDir[index] == '"' && weHaveSpace == 1 && getchar() == ' ') || (nameOfDir[index] == ' ' && weHaveSpace == 0)))
        {
            armanForCommands = checkArman();
            armanOffOrOn = armanForCommands;
        }
        else if(!strcmp(checkCommand, "compare1") && ((nameOfDir[index] == '"' && weHaveSpace == 1 && getchar() == ' ') || (nameOfDir[index] == ' ' && weHaveSpace == 0)))
        {
            armanForCommands = checkArman();
            armanOffOrOn = armanForCommands;
        }
        if(armanForCommands || weHaveAnotherLoc || (nameOfDir[index] == '"' && weHaveSpace == 1) || (nameOfDir[index] == ' ' && weHaveSpace == 0) || (nameOfDir[index] == '\n' && (!strcmp(checkCommand, "cat") || !strcmp(checkCommand, "undo") || !strcmp(checkCommand, "auto") || !strcmp(checkCommand, "compare1") || !strcmp(checkCommand, "grepDir") || !strcmp(checkCommand, "find"))))
        {
            if(!strcmp(checkCommand, "find") && nameOfDir[index] == '\n') option = 0;
            else if(!strcmp(checkCommand, "find") && armanFind && nameOfDir[index] == ' ') option = 1;
        nameOfDir[index] = '\0';
        FILE *check = fopen(nameOfDir, "r");
        if(check == NULL)
        {
        fclose(check);
        printf("The file doesn't exist!\n");
        while(getchar() != '\n');
        isCommanValid = 1;
        backToMainFolder();
        return;
        }
        else
        {
            fclose(check);
        if(strcmp(checkCommand, "insertstr") == 0)
        {
        if(armanOffOrOn == 0){
        char secondCommand[100];
        scanf("%s", secondCommand);
        if(strcmp(secondCommand, "--str") == 0)
        {
            getchar();
            setStringInSpecified(nameOfDir);
            backToMainFolder();
            return;
        }}
        else
        {
            setStringInSpecified(nameOfDir);
            backToMainFolder();
        }
        }
        else if(strcmp(checkCommand, "cat") == 0)
        {
         catFile(nameOfDir);
         if(armanOffOrOn == 0) printf("\n");
         backToMainFolder();
         if(armanOffOrOn) arman();
         return;
        }
        else if(strcmp(checkCommand, "removestr") == 0)
        {
          char posCommnad[100];
          scanf("%s", posCommnad);
          if(strcmp(posCommnad, "--pos") == 0)
          {
             removeStr(nameOfDir);
          }
          backToMainFolder();
          return;
        }
        else if(strcmp(checkCommand, "copy") == 0)
        {
        copyStr(nameOfDir);
        backToMainFolder();
        return;
        }
        else if(strcmp(checkCommand, "cut") == 0)
        {
         cutStr(nameOfDir);
         backToMainFolder();
         return;
        }
        else if(strcmp(checkCommand, "paste") == 0)
        {
            char posCommnad[100];
          scanf("%s", posCommnad);
          if(strcmp(posCommnad, "--pos") == 0)
          {
           pasteStr(nameOfDir);
          }
          backToMainFolder();
          return;
        }
        else if(!strcmp(checkCommand, "undo"))
        {
            undo(nameOfDir);
            backToMainFolder();
            return;
        }
        else if(!strcmp(checkCommand, "auto"))
        {
            autoIndent(nameOfDir);
            backToMainFolder();
            isCommanValid = 1;
            return;
        }
        else if(!strcmp(checkCommand, "compare"))
        {
            file1 = fopen(nameOfDir, "r");
            backToMainFolder();
            goToDir("compare1");
            return;
        }
        else if(!strcmp(checkCommand, "compare1"))
        {
            FILE* file2 = fopen(nameOfDir, "r");
            compare(file1, file2);
            fclose(file1);
            fclose(file2);
            backToMainFolder();
            isCommanValid = 1;
            if(armanOffOrOn) arman();
            return;
        }
        else if(!strcmp(checkCommand, "find"))
        {
            int cout = 0, at = 0, byword = 0, all = 0, star = -1;
            char command[10000];
            char *inputedString = malloc(1000000 * sizeof(char));
            if(armanFind == 0)
            {
            scanf("%s",command);
            if(!strcmp(command, "--str"))
            {
            getchar();
            int index = 0, weHaveSpace = 0;
            while(1)
            {
            scanf("%c", &inputedString[index]);
            if(inputedString[index] == '*')
               star = index;
            if(index == 0 && inputedString[index] == '"')
             {
             weHaveSpace = 1;
             continue;
             }
            if(inputedString[index] == '\\')
            {
            inputedString[index] = getchar();
            if(inputedString[index] == 'n')
               inputedString[index] = '\n';
            else if(inputedString[index] == '"')
               inputedString[index] = '"';
            else if(inputedString[index] == '*')
               inputedString[index] = '*';
            else if(inputedString[index] == '\\')
            {
            char trash;
            if((trash = getchar()) == 'n')
            {
                index++;
                inputedString[index] = 'n';
            }
            else
            {
               inputedString[index] = '\\';
               index++;
               inputedString[index] = trash;
            }
            }
            }
            else if(inputedString[index] == ' ' && weHaveSpace == 0)
            {
            inputedString[index] = '\0';
            option = 1;
            break;
            }
            else if(inputedString[index] == '"' && weHaveSpace == 1)
            {
            inputedString[index] = '\0';
            if(getchar() == ' ')
               option = 1;
            break;
            }
            else if(inputedString[index] == '\n')
            {
                inputedString[index] = '\0';
                option = 0;
                break;
            }
            index++;
            }
            }
            }
            if(armanFind)
              strcpy(inputedString, armanString);
            if(option == 0){cout = 0; at = 0; byword = 0; all = 0;}
            else
            {
            while(1){
            scanf("%s", command);
            if(!strcmp(command, "-at"))
                scanf("%d", &at);
            else if(!strcmp(command, "-byword"))
               byword = 1;
            else if(!strcmp(command, "-all"))
               all = 1;
            else if(!strcmp(command, "-count"))
               cout = 1;
            else if(!strcmp(command, "=D"))
            {
                armanOffOrOn = 1;
                break;
            }
            if(getchar() == '\n')
               break;
            }
            }
            int x;
            if((at && all) || (at && cout) || (all && cout) || (cout && byword)){
                 if(armanOffOrOn == 0) printf("Invalid combination\n");
                 else strcat(armanString,"Invalid combination\n" );
                 }
            else if(all == 1 && byword == 1){ x = findFunction(nameOfDir, inputedString, cout, at, byword, all, star, 0);}
            else if(at && byword == 1){
               if(armanOffOrOn == 0) printf("%d\n", findFunction(nameOfDir, inputedString, cout, at, byword, all, star, 0));
               else sprintf(armanString, "%d", findFunction(nameOfDir, inputedString, cout, at, byword, all, star, 0));
               }
            else if(all == 0){
               if(armanOffOrOn == 0) printf("%d\n", findFunction(nameOfDir, inputedString, cout, at, byword, all, star, 0));
               else sprintf(armanString, "%d", findFunction(nameOfDir, inputedString, cout, at, byword, all, star, 0));
               }
            else{x = findFunction(nameOfDir, inputedString, cout, at, byword, all, star, 0);}
            isCommanValid = 1;
            backToMainFolder();
            if(armanOffOrOn) arman();
            return;
        }
        else if(!strcmp(checkCommand, "replace"))
        {
            replaceString(nameOfDir);
            backToMainFolder();
            return;
        }
        else if(!strcmp(checkCommand, "grepDir"))
        {
         grep(nameOfDir);
         backToMainFolder();
         if(weHaveAnotherLoc)
         {
            weHaveAnotherLoc = 0;
            goToDir("grepDir");
         }
         return;
        }
        }
        index = -1;
        break;
        }
        index++;
    }
    if(index != -1)
    {
    nameOfDir[index] = '\0';
    if(stat(nameOfDir, &st) == -1)
       {
       printf("The directory doesn't exist!\n");
       while(getchar() != '\n');
       isCommanValid = 1;
       backToMainFolder();
       return;
       }
    else
    {
        chdir(nameOfDir);
        index = 0;
        numberOfDirectoryWeGoInto++;
    }
    }
}
}

void arman()
{
    if(armanForPrintGrep) strcpy(armanString, saveGrep);
char command[100];
   scanf("%s", command);
if(!strcmp(command, "insertstr"))
{
    scanf("%s", command);
    if(!strcmp(command, "--file"))
       goToDir("insertstr");
}
else if(!strcmp(command, "find"))
{
    scanf("%s", command);
    if(!strcmp(command, "--file"))
    {
       armanOffOrOn = 0;
       armanFind = 1;
       goToDir("find");
    }
}
else if(!strcmp(command, "grep"))
{
     lineNUmberGrep = 0;
        grepOption = -1;//0 = c  1 = l
        scanf("%s", command);
        if(!strcmp(command, "-c")){
           grepOption = 0;scanf("%s", command);}
        else if(!strcmp(command, "-l")){
           grepOption = 1;scanf("%s", command);}
        if(!strcmp(command, "--file"))
        {
            armanGrep = 1;
            armanOffOrOn = 0;
            getchar();
            memset(saveGrep, 0, 100000);
            armanString[strlen(armanString) - 1] = '\0';
            goToDir("grepDir");
            printf("%s", saveGrep);
            if(grepOption == 1)
               printf("%d\n", lineNUmberGrep);
        }
        
}
}

int main()
{
char commands[1000];
while(1)
{
isCommanValid = 0;armanOffOrOn = 0;armanFind = 0, armanGrep = 0, armanForPrintGrep = 0;
memset(armanString, 0, 10000);
scanf("%s", commands);
    if(strcmp(commands, "createfile") == 0)
    {
        scanf("%s", commands);
        if(strcmp(commands, "--file") == 0)
           createDir();
    }
    else if(strcmp(commands, "insertstr") == 0)
    {
        scanf("%s", commands);
        if(strcmp(commands, "--file") == 0)
           goToDir("insertstr");
    }
    else if (strcmp(commands, "cat") == 0)
    {
        scanf("%s", commands);
        if(strcmp(commands, "--file") == 0)
           goToDir("cat");
    }
    else if(strcmp(commands, "removestr") == 0)
    {
        scanf("%s", commands);
        if(strcmp(commands, "--file") == 0)
           goToDir("removestr");
    }
    else if(strcmp(commands, "copystr") == 0)
    {
        scanf("%s", commands);
        if(strcmp(commands, "--file") == 0)
            goToDir("copy");
    }
    else if(strcmp(commands, "cutstr") == 0)
    {
        scanf("%s", commands);
        if(strcmp(commands, "--file") == 0)
           goToDir("cut");
    }
    else if(strcmp(commands, "pastestr") == 0)
    {
        scanf("%s", commands);
        if(strcmp(commands, "--file") == 0)
           goToDir("paste");
    }
    else if(!strcmp(commands, "undo"))
    {
        scanf("%s", commands);
        if(!strcmp(commands, "--file"))
           goToDir("undo");
    }
    else if(!strcmp(commands, "auto-indent"))
    {
        isCommanValid = 1;
        goToDir("auto");
    }
    else if(!strcmp(commands, "compare"))
    {
        goToDir("compare");
    }
    else if(!strcmp(commands, "find"))
    {
        scanf("%s", commands);
        if(!strcmp(commands, "--file"))
        {
            goToDir("find");
        }
    }
    else if(!strcmp(commands, "replace"))
    {
        scanf("%s", commands);
        if(!strcmp(commands, "--file"))
        {
            goToDir("replace");
        }
    }
    else if(!strcmp(commands, "grep"))
    {
        memset(saveGrep, 0, 100000);
        lineNUmberGrep = 0;
        grepOption = -1;//0 = c  1 = l
        scanf("%s", commands);
        if(!strcmp(commands, "-c")){
           grepOption = 0;scanf("%s", commands);}
        else if(!strcmp(commands, "-l")){
           grepOption = 1;scanf("%s", commands);}
        if(!strcmp(commands, "--str"))
        {
        strcpy(giveGrepString, grepString());
        scanf("%s", commands);
        if(!strcmp(commands, "--file"))
        {
            getchar();
            goToDir("grepDir");
            if(armanForPrintGrep == 0)
            {
            printf("%s", saveGrep);
            if(grepOption == 1)
               printf("%d\n", lineNUmberGrep);
            }
        }
        }
    }
    else if(!strcmp(commands, "tree"))
    {
        int depth;
        scanf("%d", &depth);
        isCommanValid = 1;
        if(depth < -1)
           printf("Invalid depth\n");
        else if(depth > -1)
        {
           if(getchar() == ' ')
           {
            scanf("%s", commands);
            if(!strcmp(commands, "=D"))
                armanOffOrOn = 1;
           }
           if(armanOffOrOn) strcpy(armanString, "root:\n");
           else
              printf("root:\n");
           tree(depth, "./root", depth);
           if(armanOffOrOn) {
              arman();
              }
        }
    }
    if(isCommanValid == 0)
    {
        while(getchar() != '\n');
        printf("Invalid Command\n");
    }
}
}