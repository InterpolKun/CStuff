#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// cut for xinu with -d -f flags
// cut -f n-        выводит столбцы с n и до конца строки
// cut -f a         выводит столбец a
// cut -f a,b...    выводит перечисленные столбцы
// cut -f a,b... -d "pattern"   выводит перечисленные столбцы по pattern.
// к примеру cut -f 1,2 -d ":" ==> a:b:c выводит a:b
// Разделитель -- только 1 символ
int cont = 0; //0 -- без опции "-", 1 -- с опцией "-"

int parse_colums(char * s, int * numbers, int n){
    int i = 0, d = 0;
    char * tmp = s;
    for(i=0; s[i] != '\0' && i < n; ++i){
        if(isdigit(s[i])){
            *tmp = s[i];
            numbers[d] = atoi(tmp);
            if((s[i+1]) != '\0' && (s[i+1]) == '-'){
                cont = 1;
                return 0;
            }
            d++;
        }else if(s[i] == ','){
            continue;
        }else{
            printf("cut: wrong field value <%c>\n", s[i]);
            return -1;
        }
    }
    return 0;
}

int parse_string(char * s, char delim, int * numbers, int length){
    int i = 0, d = 1, k = 0, s_l, tmp_s = 0;
    char tmp[1024];
    tmp[0] = 0;
    // printf("Len %d\n", length);
    for(i = 0; s[i] != '\0' && i < length; i++){
        if(s[i] == delim){
            if(numbers[k] == d){
                strncat(tmp, s+tmp_s, i-tmp_s+1);
                s_l = strlen(tmp);
                // printf("Stlen %d\n tmp_s = %d\n d = %d\n", s_l, tmp_s, d);
                // tmp[s_l] = '\0';
                // printf("%s\n", tmp);
                k++;
            }
            d++;
            tmp_s = i+1;
        }
        if(numbers[k] == 0 && cont == 1){
            strncat(tmp, s+tmp_s, (strlen(s)-tmp_s));
            break;
        }
        if(s[i+1] == '\0'){
            if(numbers[k] == d){
                strncat(tmp, s+tmp_s, i-tmp_s+1);
                s_l = strlen(tmp);
                // printf("Stlen %d\n tmp_s = %d\n d = %d\n", s_l, tmp_s, d);
                // tmp[s_l] = '\0';
                // printf("%s\n", tmp);
                k++;
            }
        }
    }
    printf("%s\n", tmp);
    return 0;
}

int main(int argc, char ** argv){
    int numbers[1024];
    int flag;
    char delim = '\t';
    if(argc < 2){
        printf("No arguments\n");
        return 0;
    }
    // if(argc > 4){
    //     printf("argv[0]: %s\n", argv[0]);
    //     printf("argv[1]: %s\n", argv[1]);
    //     printf("argv[2]: %s\n", argv[2]);
    //     printf("argv[3]: %s\n", argv[3]);
    //     printf("argv[4]: %s\n", argv[4]);
    // }else{
    //     return 0;
    // }

    if(argv[1][1] == 'f'){
        flag = parse_colums(argv[2], numbers, strlen(argv[2]));
    }
    if(argv[1][1] == 'd'){
        if(strlen(argv[2])>1){
            printf("Delim should be 1 letter long!\n");
            return -1;
        }
        delim = argv[2][0];
    }

    if(argc>3){
        if(argv[3][1] == 'd'){
            if(strlen(argv[4])>1){
                printf("Delim should be 1 letter long!\n");
                return -1;
            }
            delim = argv[4][0];
        }
        if(argv[3][1] == 'f'){
            flag = parse_colums(argv[4], numbers, strlen(argv[4]));
        }
    }

    if(flag == -1){
        printf("Error: parse_columns\n");
        return -1;
    }
    for(int i = 0; i < 5; i++){
        printf("%d ", numbers[i]);
    }
    printf("\n%d\n", cont);
    printf("%c\n", delim);
    char input[1024];
    while(fgets(input, 1024, stdin) != NULL){
        parse_string(input, delim, numbers, strlen(input));
    }
    return 0;
}