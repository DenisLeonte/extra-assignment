#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS //VS2022 requires this sometimes.
#include <Windows.h> //system("pause"), system("cls")
#endif
#include <stdio.h> //For console IO and file IO.
#include <stdlib.h> //For system("clr")

//This program will be tasked with cypering and decyphering the Caesar chyper.
//The Caesar cypher takes a phrase and off-sets each letter by a key K.
//We will de-cypher a cyphered phrase using the ci-squared algorithm. We create a frequency map for each letter and we compare with the english frequency map.
//The lowest ci-square will be the key offset.
//This program will also provide an UI and menu for more actions.

void pause()
{
    //Cross-platform way of pausing the program. This is required by me since im developing on a linux system
    #ifdef _WIN32
        system("pause");
    #else
        int c = getchar();
        c+=2; //Just to do anything with it
    #endif
}

void clear()
{
    //Cross-platform way of clearing the program terminal.
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main()
{
    int a = 1;
    while(a == 1)
    {
        clear();
        printf("1. Cypher a string\n");
        printf("2. Cypher a file\n");
        printf("3. De-cypher a string(SUCCESS IS NOT GUARANTEED)\n");
        printf("4. De-cypher a file(SUCCESS IS NOT GUARANTEED)\n");
        printf("0. Exit\n");
        int opt = -1;
        scanf("%d",&opt);
        clear();
        switch(opt)
        {
            case 0:
                a = 0;
                printf("See you again\n");
                break;
            case 1:
            {
                int key = 0;
                char s[1001] = {'\0'};
                printf("Please be aware that the key must be between -25 and 25. Any more or less than 25 will be looped and results wont be as predictable.\nThe string limit is 1000 characters\n");
                printf("Enter the key: ");
                scanf("%d",&key);
                key %= 26;
                printf("Enter the string: ");
                fgets(s,sizeof(s),stdin);
                //caesar_cyper_s(s,key);
                break;
            }
            case 2:
            {
                int key = 0;
                char s[1001] = {'\0'};
                printf("Please be aware that the key must be between -25 and 25. Any more or less than 25 will be looped and results wont be as predictable.\nThe file name limit is 1000 characters\n");
                printf("Enter the key: ");
                scanf("%d",&key);
                key %= 26;
                printf("Enter the string: ");
                fgets(s,sizeof(s),stdin);
                //caesar_cyper_f(s,key);
                break;
            }
            case 3:
            {
                char s[1001] = {'\0'}, ref[1001] = {'\0'};
                printf("The string limit is 1000 characters\n");
                printf("Enter the cyphered string: ");
                fgets(s,sizeof(s),stdin);
                printf("Enter the reference file name: ");
                fgets(ref, sizeof(ref),stdin);
                //caesar_decypher_s(s, ref);
                break;
            }
            case 4:
            {
                char s[1001] = {'\0'}, ref[1001] = {'\0'};
                printf("The file name limit is 1000 characters\n");
                printf("Enter the cyphered file name: ");
                fgets(s,sizeof(s),stdin);
                printf("Enter the reference file name: ");
                fgets(ref, sizeof(ref),stdin);
                //caesar_decypher_f(s, ref);
                break;
            }
            default:
            {
                printf("Incorrect choice\n");
            }
        }
        pause();
    }
    return 0;
}