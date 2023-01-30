#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <windows.h>
#include <conio.h>
#include <dirent.h>
int isCommanValid;
int numberOfDirectoryWeGoInto = 0;

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
    char name[50];
    strcpy(name, "pervious");
    strcat(name, nameOfFile);
    remove(name);
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
 char command[100];
 scanf("%s", command);
 if(strcmp(command, "--pos") == 0)
 {
    getchar();
    int lineNumber, charNumber;
    scanf("%d", &lineNumber);
    getchar();
    scanf("%d", &charNumber);
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
    printf("%c", character);
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
FILE *openToRead = fopen(nameOfFile, "r");
FILE *tempFile = fopen("tmp.txt", "w");
int whichLineWeStand = 1, whichCharWeStand = 0;
    if(backOrForward == 'b')
    {
    char *saveChar = malloc(100000000 * sizeof(char));
    int countCharInFile = 0;
    int index = 0;
    while(whichLineWeStand != lineNumber)
    {
        saveChar[index] = getc(openToRead);
        countCharInFile++;
        if(saveChar[index] == '\n')
            whichLineWeStand++;
        index++;
    }
    while(charNumber != whichCharWeStand)
    {
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
    char *saveChar = malloc(100000000 * sizeof(char));
    int countCharInFile = 0;
    int index = 0;
    while(whichLineWeStand != lineNumber)
    {
        saveChar[index] = getc(openToRead);
        countCharInFile++;
        if(saveChar[index] == '\n')
            whichLineWeStand++;
        index++;
    }
    while(charNumber != whichCharWeStand)
    {
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
char namePervious[50];
strcpy(namePervious, "pervious");
strcat(namePervious, nameOfFile);
remove(nameOfFile);
DWORD attributes = GetFileAttributes(namePervious);
SetFileAttributes(namePervious, attributes + FILE_ATTRIBUTE_HIDDEN);
rename(namePervious, nameOfFile);
isCommanValid = 1;
}

void autoIndent(char *nameOfFile)
{
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
        if(prevent == '}')
           fputc('\n', temp);
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
            printf("========== #%d ==========\n", linenumber);
            printf("%s\n", line1);
            printf("%s\n", line2);
            break;
        }
    }
    if(length1 > length2)
    {
        for(int i = linenumber; i < length2; i++)
        {
            fgets(line1, sizeof(line1), file1);
        }
        printf("<<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<<\n", length2 + 1, length1);
        char a = getc(file1);
        while(a != EOF)
        {
        printf("%c", a);
        a = getc(file1);
        }
        printf("\n");
    }
    else if(length2 > length1)
    {
        for(int i = linenumber; i < length1; i++)
        {
            fgets(line2, sizeof(line2), file2);
        }
        printf(">>>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>>>\n", length1 + 1, length2);
        char a = getc(file2);
        while(a != EOF)
        {
            printf("%c", a);
            a = getc(file2);
        }
        printf("\n");
    }
}

int findFunction(char* nameOfFile, char* string, int count, int at, int byword, int all, int starIndex)
{
if(at == 0 && all == 0 && count == 0 && byword == 0)
{
   if(starIndex == -1)
   {
      FILE* openToRead = fopen(nameOfFile, "r");
      int index = 0;
      char a;
      a = getc(openToRead);
      while(a != EOF)
      {
        if(a == string[0])
        {
            int output = index;
            int check = 1;
            for(int i = 1; i < strlen(string); i++)
            {
                a = getc(openToRead);
                index++;
                if(a != string[i])
                {
                   fseek(openToRead, output + 1, SEEK_SET);
                   index = output;
                   check = 0;
                   break;
                }
            }
            if(check){
              fclose(openToRead);
              return output;
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
    if(starIndex == strlen(string) - 1)
    {
        string[strlen(string) - 1] = '\0';
        return findFunction(nameOfFile, string, 0, 0, 0, 0, -1);
    }
    else if(starIndex == 0)
    {
        for(int i = 0; i < strlen(string) - 1; i++)
        {
            string[i] = string[i + 1];
        }
        string[strlen(string) - 1] = '\0';
        int x = findFunction(nameOfFile, string, 0, 0, 0, 0, -1);
        FILE* openToRead = fopen(nameOfFile, "r");
        for(int i = 0; i < x; i++)
        {
            fseek(openToRead, x - i, SEEK_SET);
            char a = getc(openToRead);
            if(a == ' ' || a == '\0' || a == '\n')
            {
               fclose(openToRead);
               return x - i + 1;
            }
            fclose(openToRead);
            return 0;
        }
    }
    else
    {
        FILE* openToRead = fopen(nameOfFile, "r");
        if(string[starIndex + 1] == ' ')
        {
            int index = 0, check = 1;
            int output;
            char a = getc(openToRead);
            while(a != EOF)
            {
                check = 1;
                if(a == string[0])
                {
                    output = index;
                    for(int i = 1; i < starIndex; i++)
                    {
                     a = getc(openToRead);
                     index++;
                     if(a != string[i])
                     {
                      fseek(openToRead, output + 1, SEEK_SET);
                      index = output;
                      check = 0;
                      break;
                     }
                    }
                    if(check)
                    {
                    a = getc(openToRead);
                    while(a != EOF && a != ' ' && a != '\0')
                    {
                        a = getc(openToRead);
                    }
                    a = getc(openToRead);
                    if(string[starIndex + 2] == a)
                    {
                    for(int i = starIndex + 3; i < strlen(string); i++)
                    {
                        a = getc(openToRead);
                        if(a != string[i])
                        {
                            fseek(openToRead, output + 1, SEEK_SET);
                            index = output;
                            check = 0;
                            break;
                        }
                    }
                    if(check)
                    {
                       fclose(openToRead);
                       return output;
                    }
                    }
                    fseek(openToRead, output + 1, SEEK_SET);
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
            char a = getc(openToRead);
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
                    index++;back++;
                    if(a != string[i])
                    {
                        check = 0;
                        index = output;
                        back = output;
                        fseek(openToRead, output + 1, SEEK_SET);
                        break;
                    }
                 }
                 if(check)
                 {
                    a = getc(openToRead);
                    back++;
                    if(a == ' ' || a == '\0' || a == EOF || a == '\n')
                    {
                        check = 0;
                        index = output;
                        back = output;
                        fseek(openToRead, output + 1, SEEK_SET);
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
                            if(a != string[i])
                            {
                                fseek(openToRead,back + 1 , SEEK_SET);
                                check = 0;
                            }
                        }
                        if(check)
                        {
                        fclose(openToRead);
                        return output;
                        }
                     }
                     a = getc(openToRead);
                     back++;
                    }
                 }
                 fseek(openToRead, output + 1, SEEK_SET);
                }
                a = getc(openToRead);
                index++;
                back++;
            }
            fclose(openToRead);
            return -1;
        }
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
     if(entry -> d_name[0] != '.' && (entry -> d_name[0] != 'p' || entry -> d_name[1] != 'e' || entry -> d_name[2] != 'r' || entry -> d_name[3] != 'v' || entry -> d_name[4] != 'i' || entry -> d_name[5] != 'o' || entry -> d_name[6] != 'u' || entry -> d_name[7] != 's'))
     {
        for(int i = 0; i < firstDepth - depth; i++){printf("|  ");}
        printf("|--%s", entry -> d_name);
        if(entry -> d_type == DT_DIR)
        {
            printf(":\n");
            char nameOfDir[100];
            strcpy(nameOfDir, nameOfFileOrDir);
            strcat(nameOfDir, "/");
            strcat(nameOfDir, entry -> d_name);
            tree(depth - 1, nameOfDir, firstDepth);
        }
        else 
           printf("\n");
     }
}
closedir(dir);
}

FILE* file1;
void goToDir(char *checkCommand)
{
int weHaveSpace = 0;
struct stat st = {0};
char nameOfDir[1000];
if(strcmp(checkCommand, "compare1"))
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
        if((nameOfDir[index] == '"' && weHaveSpace == 1) || (nameOfDir[index] == ' ' && weHaveSpace == 0) || (nameOfDir[index] == '\n' && (!strcmp(checkCommand, "cat") || !strcmp(checkCommand, "undo") || !strcmp(checkCommand, "auto") || !strcmp(checkCommand, "compare1"))))
        {
        nameOfDir[index] = '\0';
        FILE *check = fopen(nameOfDir, "r");
        if(check == NULL)
        {
        fclose(check);
        printf("The file isn't exited!\n");
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
        char secondCommand[100];
        scanf("%s", secondCommand);
        if(strcmp(secondCommand, "--str") == 0)
        {
            getchar();
            makePervious(nameOfDir);
            setStringInSpecified(nameOfDir);
            backToMainFolder();
            return;
        }
        }
        else if(strcmp(checkCommand, "cat") == 0)
        {
         catFile(nameOfDir);
         printf("\n");
         backToMainFolder();
         return;
        }
        else if(strcmp(checkCommand, "removestr") == 0)
        {
          char posCommnad[100];
          scanf("%s", posCommnad);
          if(strcmp(posCommnad, "--pos") == 0)
          {
            makePervious(nameOfDir);
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
         makePervious(nameOfDir);
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
           makePervious(nameOfDir);
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
            makePervious(nameOfDir);
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
            return;
        }
        else if(!strcmp(checkCommand, "find"))
        {
            int cout = 0, at = 0, byword = 0, all = 0, option = 0, star = -1;
            char command[10000];
            scanf("%s",command);
            if(!strcmp(command, "--str"))
            {
            getchar();
            char *inputedString = malloc(1000000 * sizeof(char));
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
            if(getchar() == '\n')
               break;
            }
            }
            if((at && all) || (at && cout) || (all && cout) || (cout && byword))
                 printf("Invalid combination");
            else if(all == 0)
                printf("%d\n", findFunction(nameOfDir, inputedString, cout, at, byword, all, star));
            else
            {
             /*****************************************************************************************************/
            }
            isCommanValid = 1;
            }
            backToMainFolder();
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
       printf("The directory isn't exited!\n");
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

int main()
{
char commands[1000];
while(1)
{
isCommanValid = 0;
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
    else if(!strcmp(commands, "tree"))
    {
        int depth;
        scanf("%d", &depth);
        isCommanValid = 1;
        if(depth < -1)
           printf("Invalid depth\n");
        else if(depth > -1)
        {
           printf("root:\n");
           tree(depth, "./root", depth);
        }
    }
    if(isCommanValid == 0)
    {
        while(getchar() != '\n');
        printf("Invalid Command\n");
    }
}
}