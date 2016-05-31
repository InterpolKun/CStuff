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
    int i = 0, d = 0, k;
    char * tmp = s;
    for(i=0; s[i] != '\0' && i < n; ++i){
        if(isdigit(s[i])){
            *tmp = s[i];
            numbers[d] = atoi(tmp);
            if((s[i+1]) != '\0' && (s[i+1]) == '-'){
                cont = 1;
                i++;
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
int include_arr(int * numbers, int n, int size){
    int i = 0;
    for(i = 0; i < size; i++){
        if(numbers[i] != 0 && numbers[i] == n){
            return 1;
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
            if((include_arr(numbers, d, 1024)) == 1){
                strncat(tmp, s+tmp_s, i-tmp_s);
                s_l = strlen(tmp);
                // printf("Stlen %d\n tmp_s = %d\n d = %d\n", s_l, tmp_s, d);
                // tmp[s_l] = '\0';
                // printf("%s\n", tmp);
                k++;
            }
            tmp_s = i;
            d++;
        }
        if(numbers[k] == 0 && cont == 1){
            strncat(tmp, s+tmp_s, (strlen(s)-tmp_s-1));
            break;
        }
        if(s[i+1] == '\0'){
            if((include_arr(numbers, d, 1024)) == 1){
                // strncat(tmp, s+tmp_s, i-tmp_s+2);
                strncat(tmp, s+tmp_s, i-tmp_s);
                s_l = strlen(tmp);
                // printf("Stlen %d\n tmp_s = %d\n d = %d\n", s_l, tmp_s, d);
                // tmp[s_l] = '\0';
                // printf("%s\n", tmp);
                k++;
                break;
            }
        }
    }

    printf("%s\n", tmp);

    return 0;
}

int main(int argc, char ** argv){
    int numbers[1024];
    memset(numbers, 0, sizeof(numbers));
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
    if(argc == 4 || argc == 2){
        printf("Each flag should have an argument\n");
        return -1;
    }

    if(argv[1][1] == 'f'){
        flag = parse_colums(argv[2], numbers, strlen(argv[2]));
    }else if (argv[1][1] == 'd'){
        if(strlen(argv[2])>1){
            printf("Delim should be 1 letter long!\n");
            return -1;
        }
        delim = argv[2][0];
    }else{
        printf("Args should be -d OR -f\n");
        return -1;
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
    // int i = 0;
    // for(i = 0; i < 1024; i++){
    //     printf("%d ",numbers[i]);
    // }
    // printf("\n");
    char input[1024];
    while(fgets(input, 1024, stdin) != NULL){
        parse_string(input, delim, numbers, strlen(input));
    }
    return 0;
}
