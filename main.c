#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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
        else if(inputedString[index] == '\\')
        {
            char trash;
            if(trash = getchar() == 'n')
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
        else
        {
           index++;
           inputedString[index] = inputedString[index - 1];
           inputedString[index - 1] = '\\';
           
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
    char saveChar[10000];
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

void goToDir(char *checkCommand)
{
int weHaveSpace = 0;
struct stat st = {0};
char nameOfDir[1000];
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
        if((nameOfDir[index] == '"' && weHaveSpace == 1) || (nameOfDir[index] == ' ' && weHaveSpace == 0) || (nameOfDir[index] == '\n' && !strcmp(checkCommand, "cat")))
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
             removeStr(nameOfDir);
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
    if(isCommanValid == 0)
    {
        while(getchar() != '\n');
        printf("Invalid Command\n");
    }
    
}
}