#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
    struct stat st = {0};
    char nameOfDir[1000];
    getchar();
    if(getchar() == '"')
       getchar();
    int index = 0, numberOfDirectoryWeMade = 0;
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
        numberOfDirectoryWeMade++;
     nameOfDir[index] = '\0';
        if(stat(nameOfDir, &st) == -1)
           mkdir(nameOfDir);
        chdir(nameOfDir);
        index = 0;
    }
    }
    for(int i = 0; i < numberOfDirectoryWeMade; i++)
    {
        chdir("..");
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
}
}