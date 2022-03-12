#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS //VS2022 requires this sometimes.
#include <Windows.h> //system("pause"), system("cls")
#else
//UNIX includes here
#endif
#include <stdio.h> //For console IO and file IO.
#include <stdlib.h> //For system("clear")
#include <math.h> //For pow()

//This program will be tasked with cypering and decyphering the Caesar chyper.
//The Caesar cypher takes a phrase and off-sets each letter by a key K.
//We will de-cypher a cyphered phrase using the ci-squared algorithm. We create a frequency map for each letter and we compare with the english frequency map.
//The lowest ci-square will be the key offset.
//This program will also provide an UI and menu for more actions.

void pause()
{
    //Cross-platform way of pausing the program. This is required by me since im developing on a linux system.
    #ifdef _WIN32
        system("pause");
    #else
        printf("Press any key to continue...\n");
        system("read");
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

void caesar_cypher_f(char* file_name, int key, int* err)
{
    //Algoritm to cypher a file and return its cyphered contents on the terminal.
    *err = 0;
    FILE* fptr = fopen(file_name,"r");
    if(fptr == NULL)
    {
        //Check if the file exists
        *err = -1;
        return;
    }
    while(!feof(fptr))
    {   
        //Here is the cyphering "per se"
        char w[51] = {'\0'};
        fscanf(fptr,"%s",w);
        for(int i = 0;w[i] != '\0'; i++)
        {
            int up = 0;
            if(w[i] >= 65 && w[i] <= 90)
            {
                //Check for upper case and if true bring it to lower case for ease of use
                up = 1;
                w[i] += 32;
            }
            if(w[i] >= 97 && w[i] <= 122)
            {
                //Cyphering here
                int k = w[i] - 97;
                k = (k + key) % 26;
                if(k < 0)
                {
                    k += 26;
                }
                w[i] = k + 97;
                if(up == 1)
                {
                    //If its upper case then bring it back to upper case
                    w[i] -= 32;
                }
            }
        }
        //Print the word
        printf("%s ",w);
    }
    //Clean up
    printf("\n");
    fclose(fptr);
}

void caesar_cypher_s(char* string, int key, int* err)
{
    //Because I dont want to reinvent the wheel, I will use the file cyphering algorithm.
    //Putting the string in a temp file.
    char temp_file_path[] = {"temp.txt"};
    FILE* fptr = fopen(temp_file_path,"w");
    fputs(string, fptr);
    fclose(fptr);
    //Calling the file cyphering function with the temp file as parameter.
    caesar_cypher_f(temp_file_path,key, err);
    //Error handling
    if(*err != 0)
    {
        return;
    }
    if(!remove(temp_file_path) == 0)
    {
        //If temp cant be removed then call the error.
        *err = -2;
        return;
    }
}

void shift(int *arr)
{
    int temp = arr[sizeof(arr)];
    for (int i = 24; i > 0; i++)
    {
        arr[i] = arr[i - 1];
    }
    arr[0] = temp;
}

float ci_square(float* ref_arr, float* cyp_arr)
{
    float s = 0.0f;
    for(int i = 0; i < 26; i++)
    {
        s+= (float)(pow(cyp_arr[i] - ref_arr[i],2))/ref_arr[i];
    }
    return s;
}

void caesar_decypher_f(char* ref_file, char* file, int* err)
{
    //Get reference table.
    float reference_table[26] = {0.0f};
    FILE* fref = fopen(ref_file, "r");
    if(fref == NULL)
    {
        *err = -1;
        return;
    }
    for(int i = 0; i < 26; i++)
    {
        fscanf(fref,"%f",&reference_table[i]);
    }
    fclose(fref);
    //Create reference table for the cyphered file.
    int cyphered[26] = {0}, tot = 0;
    FILE* fcip = fopen(file,"r");
    if(fcip == NULL)
    {
        *err = -1;
        return;
    }
    while(!feof(fcip))
    {
        char w[51] = {'\0'};
        fscanf(fcip,"%s",w);
        for(int i = 0; w[i] != '\0'; i++)
        {
            if(w[i] >= 65 && w[i] <= 90)
            {
                //Check for upper case and if true bring it to lower case for ease of use
                w[i] += 32;
            }
            if(w[i] >= 97 && w[i] <= 122)
            {
                cyphered[w[i]-97]++;
                tot++;
            }
        }
    }
    float cyphered_table[26] = {0.0f};
    for(int i = 0; i < 26; i++)
    {
        cyphered_table[i] = (float)(100*cyphered[i])/tot;
    }
    //Recursively shift and call the ci-square function on the 2 tables.
    int best_shift = 0;
    float min_ci_sq = 0.0;
    for(int i = 0; i < 25; i++)
    {
        float ci = ci_square(reference_table, cyphered_table);
        if(min_ci_sq == 0.0)
        {
            min_ci_sq = ci;
            best_shift = i;
        }
        else if (min_ci_sq > ci)
        {
            min_ci_sq = ci;
            best_shift = i;
        }
    }
    caesar_cypher_f(file, best_shift,err);
}

int main()
{
    //Main loop and UI handler.
    int a = 1;
    while(a == 1)
    {
        clear();
        printf("1. Cypher a string\n");
        printf("2. Cypher a file\n");
        printf("3. De-cypher a string(SUCCESS IS NOT GUARANTEED)\n");
        printf("4. De-cypher a file(SUCCESS IS NOT GUARANTEED)\n");
        printf("0. Exit\n");
        int opt = -1,err_code = 0;
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
                char buffer[2];
                fgets(buffer,sizeof(buffer),stdin);
                fgets(s,sizeof(s),stdin);
                caesar_cypher_s(s,key, &err_code);
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
                printf("Enter the file name: ");
                fscanf(stdin," %s",s);
                caesar_cypher_f(s,key, &err_code);
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
                scanf("%s",s);
                printf("Enter the reference file name: ");
                scanf("%s",ref);
                caesar_decypher_f(ref, s, &err_code);
                break;
            }
            default:
            {
                printf("Incorrect choice\n");
            }
        }
        //Error code handler
        switch(err_code)
        {
            case 0: //No error
                break;
            case -1: //File not opened
                printf("Error -1: inexistent file. Check for the file if it exists and check for spelling errors when inserting the file name in the program\n");
                break;
            case -2: //Unable to delete the temp file
                printf("Error -2: unable to delete temp file. Check if you have administrator privileges running this program or contact the system administrator\n");
                break;
        }
        pause();
    }
    return 0;
}