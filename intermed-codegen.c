#include <stdio.h>
#include <string.h>

int main(){
    char ch;
    char expression[100];
    int idx = 0;
    int lparent_loc[10];
    int lp_count = 0;
    int rparent_loc[10];
    int rp_count = 0;
    char output[100];
    int tmp_count = 1;
    char extracted[100];
    int completed = 0;
    FILE *r_file;
    r_file = fopen("generated/2_ast_output.txt", "r");
    FILE *w_file;
    w_file = fopen("generated/3_intermed.txt", "w");
    while((ch = fgetc(r_file)) != EOF) {
        expression[idx++] = ch;
    }
    idx = 0;
    while(completed == 0){
        ch = expression[idx];
        if(idx == 0 && ch != '('){
            completed = 1;
        }
        else{
            idx++;
            if (ch == '('){
                lparent_loc[lp_count] = idx -1;
                lp_count ++;
            }else if(ch == ')'){
                rparent_loc[rp_count] = idx;
                rp_count ++;
                
                int start = lparent_loc[lp_count - 1];
                int end = rparent_loc[rp_count - 1 ];

                strncpy(extracted, expression + start, end - start);
                extracted[end - start] = '\0';

                char temp[10];
                sprintf(temp, "t%d", tmp_count++);
                
                fprintf(w_file, "%s = %s\n", temp, extracted);

                char rest_of_expression[100];
                strcpy(rest_of_expression, expression + end);
                expression[start]='\0';
                strcat(expression, temp);
                strcat(expression, rest_of_expression);

                idx = 0;
            }
        }
        
    }

    fclose(r_file);
    fclose(w_file);

    return 0;
}