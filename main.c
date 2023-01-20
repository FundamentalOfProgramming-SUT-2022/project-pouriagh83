#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
 if(strcmp(command, "pos") == 0)
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
 }
}

void goToDir()
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
        if((nameOfDir[index] == '"' && weHaveSpace == 1) || (nameOfDir[index] == ' ' && weHaveSpace == 0))
        {
        nameOfDir[index] = '\0';
        FILE *check = fopen(nameOfDir, "r");
        if(check == NULL)
        {
        fclose(check);
        printf("The file isn't exited!\n");
        backToMainFolder();
        return;
        }
        else
        {
            fclose(check);
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
           goToDir();
    }
}
}