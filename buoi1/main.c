#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "libfdr/jrb.h"
#include "libfdr/jval.h"

JRB j_ignore, j_take;

void get_ignore(char*);
void get_word(char*);
void edit(char*, int);
void reverse(char*, int, int);


int main(int argc, char* argv[]) {

    get_ignore(argv[2]);
    get_word(argv[1]);
   
    jrb_free_tree(j_ignore);
    jrb_free_tree(j_take);
    return 0;
}



void get_ignore(char* file){
    
    FILE *f = fopen(file,"r");
    if (f == NULL) {
        printf("Can't open file.");
        exit(-1);
    }
    
    j_ignore = make_jrb();
    char word[50];

    while( !feof(f) ) {
        fscanf(f,"%s",word);
        if(feof(f)) break;
        jrb_insert_str(j_ignore, strdup(word), (Jval){.s = NULL});
    }

    fclose(f);
}

void get_word(char* file) {
    FILE *f = fopen(file,"r");
    if (f == NULL) {
        printf("Can't open file.");
        exit(-1);
    }

    j_take = make_jrb();

    int flag = 0, line = 1, index = 0;
    char get[300], word[50];
    
    while( !feof(f) ) {
        fgets(get,300,f);
        if(feof(f)) break;
        
        for(int i=0; i< strlen(get); i++) {
            
            if ( get[i] == '.' && !isalnum(get[i+1]) )
                flag = 1;
            if ( isalpha(get[i]) ){
                if ( isupper(get[i]) && flag == 0 ){
                    while ( isalpha(get[i]) )
                        i++;
                    i--;
                }
                else if (isupper(get[i]) && flag ){
                        word[index++] = tolower(get[i]);
                    flag = 0;
                }
                else
                    word[index++] = get[i];
            }
            else {
                word[index] = 0;
                
                char set = '1', get[10];
                if ( jrb_find_str(j_ignore, word) == NULL) {
                    JRB find;
                    find = jrb_find_str(j_take,word);
                    if ( find == NULL ){
                        sprintf(get,"%c,%d",set,line);
                        jrb_insert_str(j_take, strdup(word), (Jval){.s = strdup(get)});
                    }    
                    else edit(find->val.s,line);
                }

                word[0] = 0;
                index = 0;
            }
        
        }
        line++;
    }

    for(JRB ptr = jrb_first(j_take); ptr != jrb_nil(j_take); ptr = jrb_next(ptr))
        printf("%s %s\n",ptr->key.s,ptr->val.s);

}

void edit(char* string, int line) {
    char s1[10], s2[30], s3[10];
    int i = 0, j = 0;
    while ( string[i] != ',') {
        s1[i]=string[i];
        i++;
    }
    s1[i]=0;
    while ( string[i] != 0) {
        s2[j]=string[i];
        i++;
        j++;
    }
    s2[j]=0;
    i = strlen(s2)-1;
    j = 0;
    while ( s2[i] != ',') {
        s3[j] = s2[i];
        i--;
        j++;  
    }
    s2[i+1]=0;
    s3[j]=0;

    int x = atoi(s1);
    reverse(s3,0,strlen(s3)-1);
    int y = atoi(s3);

    if ( y == line )
        sprintf(string,"%d%s%s",x+1,s2,s3);
    else 
        sprintf(string,"%d%s%s,%d",x+1,s2,s3,line);
}

void reverse(char *string, int begin, int end){
    char c;
    if (begin >= end)
        return;
    c = *(string+begin);
    *(string+begin) = *(string+end);
    *(string+end) = c;
    reverse(string, begin+1, end-1);
}