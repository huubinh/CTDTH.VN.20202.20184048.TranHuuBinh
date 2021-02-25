#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "libfdr/jrb.h"
#include "libfdr/jval.h"

JRB j_ignore, j_take;

int jset_insert_str(JRB tree, char *key, Jval val);
void get_word(char* file);
void get_ignore(char* file);
void edit(char* string, int line);
void reverse(char*, int, int);
int check(char*);
int main(int argc, char* argv[]) {
//    char t[]= "hello";
//    reverse(t,0,strlen(t)-1);
//    printf("%s",t);

    get_ignore(argv[2]);
    get_word(argv[1]);
   
    for(JRB ptr = jrb_first(j_take); ptr != jrb_nil(j_take); ptr = jrb_next(ptr))
        printf("%s %s\t",ptr->key.s,ptr->val.s);

    jrb_free_tree(j_ignore);
    jrb_free_tree(j_take);
    return 0;
}


int jset_insert_str(JRB tree, char *key, Jval val) {
    if ( jrb_find_str(tree,key) == NULL ){
        jrb_insert_str(tree,strdup(key),val);
        return 1;
    }
    return 0;
}

void get_word(char* file) {
    FILE *f = fopen(file,"r");
    if (f == NULL) {
        printf("Can't open file.");
        exit(-1);
    }

    j_take = make_jrb();
    
    char c;
    char word[50];
    int index = 0, line = 1, flag = 0;
    
    do {
        c = (char)fgetc(f);
        
        if ( c == '\n' )
            line++;
        if ( c == '.' ) {}
        /*    if ( (char)fgetc(f) == ' ')
                flag = 1;
        else if ( isupper(c) ) {
            if (flag == 0)
                while ( isalpha((char)fgetc(f)) );
            else {
                flag = 0;
                word[index++] = c;
                word[index] = 0;
            }
        }*/
        if ( isalpha(c) ) {
            word[index++] = c;
        }
        else {
            word[index] = 0;
            if (check(word)) {
                for(int i = 0; word[i]; i++)
                    word[i] = tolower(word[i]);
                char set = '1',*get;
                if ( jrb_find_str(j_ignore, word) == NULL ) {
                    JRB find;
                    find = jrb_find_str(j_take,word);
                    if ( find == NULL ){
                        sprintf(get,"%c,%d",set,line);
                        jrb_insert_str(j_take, strdup(word), (Jval){.s = strdup(get)});
                    }    
                    else edit(find->val.s,line);
                }
            }
            word[0] = 0;
            index = 0;
        }
    } while (c != EOF);

    fclose(f);
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
    //printf("\n%s %s %s\n", string, s1, s2);
    i = strlen(s2)-1;
    j = 0;
    while ( s2[i] != ',') {
    //    printf("\n%c %c\n", s3[j], s2[i]);
        s3[j] = s2[i];
        i--;
        j++;  
    }
    s2[i+1]=0;
    s3[j]=0;

    int x = atoi(s1);
    //reverse(s3,0,strlen(s3)-1);
    int y = atoi(s3);

     printf("\n%s %s %s %s %d %d %d\n", s1, s2, s3, string, line, x+1, y);

     if ( y == line )
         sprintf(string,"%d%s%s",x+1,s2,s3);
     else 
         sprintf(string,"%d%s%s,%d",x+1,s2,s3,line);
    
    printf("\n%s\n", string);
}

void reverse(char *x, int begin, int end){
    char c;
    if (begin >= end)
        return;
    c = *(x+begin);
    *(x+begin) = *(x+end);
    *(x+end) = c;
    reverse(x, ++begin, --end);
}

int check (char* string) {
    for (int i=0 ; string[i]; i++)
        if (!isalpha(string[i]))
            return 0;
    return 1;
}