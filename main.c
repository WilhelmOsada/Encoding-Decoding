#include <stdio.h>
#include <stdlib.h>
#include <string.h>
union char_union
{
    unsigned char c;
    struct {
        unsigned char c1: 1;
        unsigned char c2: 1;
        unsigned char c3: 1;
        unsigned char c4: 1;
        unsigned char c5: 1;
        unsigned char c6: 1;
        unsigned char c7: 1;
        unsigned char c8: 1;
    }bity;
};
int decode(const char * filename, char *txt, int size)
{
    if(filename == NULL || txt == NULL || size < 1)
    {
        return 1;
    }
    int k = 0;
    union char_union wczytany_char;
    int array[8];
    FILE *f = fopen(filename,"r");
    if(!f)
    {
        return 2;
    }
    while (!feof(f) && k < size)
    {

        for(int i = 0 ; i < 8 ; i++)
        {
            if(!fscanf(f,"%d",array+i) || (feof(f) && i!=0))
            {
                fclose(f);
                return 3;
            }
        }
        wczytany_char.bity.c1 = *(array+7)%2;
        wczytany_char.bity.c2 = *(array+6)%2;
        wczytany_char.bity.c3 = *(array+5)%2;
        wczytany_char.bity.c4 = *(array+4)%2;
        wczytany_char.bity.c5 = *(array+3)%2;
        wczytany_char.bity.c6 = *(array+2)%2;
        wczytany_char.bity.c7 = *(array+1)%2;
        wczytany_char.bity.c8 = *(array)%2;
        *(txt+k)=(signed char)wczytany_char.c;
        k++;
    }
    if(k<size && k>0)
    {
        *(txt + k - 1) = '\0';
    }
    else
    {
        *(txt + size - 1) = '\0';
    }
    fclose(f);
    return 0;
}
int encode(const char *input, char *txt, const char *output)
{
    if(input == NULL || output == NULL || txt == NULL)
    {
        return 1;
    }
    FILE *fin = fopen(input,"r");
    if(!fin)
    {
        return 2;
    }
    FILE *fout = fopen(output,"w");
    if(!fout)
    {
        fclose(fin);
        return 4;
    }
    int size_text = strlen(txt);
    int size = 0;
    unsigned char tab[500001];
    int temp = 0;
    for(;size <= 500000;size++)
    {
        if(!fscanf(fin,"%d",&temp))
        {
            fclose(fin);
            fclose(fout);
            return 3;
        }
        *(tab+size) = (char)temp;
        if(feof(fin))
        {
            break;
        }
    }
    if(size_text>size/8)
    {
        fclose(fin);
        fclose(fout);
        return 3;
    }
    int czysieudalo = 0;
    union char_union char_temp;
    union char_union int_temp;
    for(int j = 0 ; j<size+1 ; j++)
    {
        int_temp.c = *(tab+j);
        int_temp.bity.c1 = 0;
        *(tab+j) = int_temp.c;
    }
    for(int i = 0 ; i*8+7<size ; i++)
    {
        if(i==size_text)
        {
            czysieudalo = 1;
            break;
        }
        char_temp.c = *(txt+i);

        int_temp.c = *(tab+i*8);
        int_temp.bity.c1 = char_temp.bity.c8;
        *(tab+i*8) = int_temp.c;

        int_temp.c = *(tab+i*8+1);
        int_temp.bity.c1 = char_temp.bity.c7;
        *(tab+i*8+1) = int_temp.c;

        int_temp.c = *(tab+i*8+2);
        int_temp.bity.c1 = char_temp.bity.c6;
        *(tab+i*8+2) = int_temp.c;

        int_temp.c = *(tab+i*8+3);
        int_temp.bity.c1 = char_temp.bity.c5;
        *(tab+i*8+3) = int_temp.c;

        int_temp.c = *(tab+i*8+4);
        int_temp.bity.c1 = char_temp.bity.c4;
        *(tab+i*8+4) = int_temp.c;

        int_temp.c = *(tab+i*8+5);
        int_temp.bity.c1 = char_temp.bity.c3;
        *(tab+i*8+5) = int_temp.c;

        int_temp.c = *(tab+i*8+6);
        int_temp.bity.c1 = char_temp.bity.c2;
        *(tab+i*8+6) = int_temp.c;

        int_temp.c = *(tab+i*8+7);
        int_temp.bity.c1 = char_temp.bity.c1;
        *(tab+i*8+7) = int_temp.c;
    }
    fseek(fin,0,SEEK_SET);
    char przerwa;
    for(int i = 0 ; i < size ; i++)
    {
        fprintf(fout,"%d",*(tab+i));
        do {
            przerwa = fgetc(fin);
        }while(przerwa != ' ' && przerwa != '\n');
        do {
            fprintf(fout,"%c",przerwa);
            przerwa = fgetc(fin);
        }while (przerwa == ' ' || przerwa == '\n');
    }
    fclose(fin);
    fclose(fout);
    if(czysieudalo == 0)
    {
        return 3;
    }
    return 0;
}
int main() {
    char txt[1001];
    char input[31];
    char input2[30];
    printf("Do you want to encode (E/e) or decode (D/d) a message?");
    char answer = getchar();
    while(getchar()!='\n');
    if (answer == 'e' || answer == 'E') {
        printf("Enter a message to be encoded:");
        scanf("%1000[^\n]s", txt);
        while(getchar()!='\n');
        printf("Enter input file name:");
        if (!scanf("%30s", input)) {
            printf("Incorrect input");
            return 1;
        }
        while(getchar()!='\n');
        printf("Enter output file name:");
        if (!scanf("%29s", input2)) {
            printf("Incorrect input");
            return 1;
        }
        while(getchar()!='\n');
        int result = encode(input, txt, input2);
        if (result == 4) {
            printf("Couldn't create file");
            return 5;
        }
        if (result == 3) {
            printf("File corrupted");
            return 6;
        }
        if (result == 2) {
            printf("Couldn't open file");
            return 4;
        }
        if (result == 1) {
            printf("Incorrect input");
            return 1;
        }
        printf("File saved");
    }
    else if (answer == 'd' || answer == 'D') {
        printf("Enter input file name:");
        if (!scanf("%30s", input)) {
            printf("Incorrect input");
            return 1;
        }
        while(getchar()!='\n');
        int result = decode(input, txt, 1000);
        if (result == 4) {
            printf("Couldn't create file Couldn't create file");
            return 5;
        }
        if (result == 3) {
            printf("File corrupted");
            return 6;
        }
        if (result == 2) {
            printf("Couldn't open file");
            return 4;
        }
        if (result == 1) {
            printf("Incorrect input");
            return 1;
        }
        printf("%s", txt);
    }
    else {
        printf("Incorrect input data");
        return 1;
    }
    return 0;
}