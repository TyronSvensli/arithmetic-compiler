#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char expression[100];
int register_index = 0;
int tmp_using_register[8] = {0};
FILE *w_file;

char* set_var_register(int register_idx){
    switch (register_idx)
    {
    case 0:
        return "r8";
        break;
    case 1:
        return "r9";
        break;
    case 2:
        return "r10";
        break;
    case 3:
        return "r11";
        break;
    case 4:
        return "r12";
        break;
    case 5:
        return "r13";
        break;
    case 6:
        return "r14";
        break;
    case 7:
        return "r15";
        break;
    default:
        exit(1);
    }
}

void print_asm(char op, char *var1, char *var2){
    switch (op)
    {
    case '+':
        fprintf(w_file, "add %s, %s\n",var1,var2);
        break;
    case '-':
        fprintf(w_file, "sub %s, %s\n",var1,var2);
        break;
    case '*':
        fprintf(w_file, "imul %s, %s\n",var1,var2);
        break;
    case '/':
        fprintf(w_file, "mov rax, %s\n",var1);
        fprintf(w_file, "mov rbx, %s\n",var2);
        fprintf(w_file, "div rbx\n");
        fprintf(w_file, "mov %s, rax\n",var1);
        break;
    default:
        exit(1);
    }
}


void treat_line() {
    char tx[10];
    char op;
    char var1[10], var2[10];
    int tmp_nr;
    char value_holder[10];

    if (sscanf(expression, "%s = (%s %c %[^)])", tx, var1, &op, var2) == 4) {

        register_index = 0;
        if (var1[0] == 't' && var2[0] == 't')
        {
            sscanf(var2, "t%d", &tmp_nr);
            while (tmp_using_register[register_index] != tmp_nr)
            {
                register_index ++;
            }
            char *var2 = set_var_register(register_index);
            tmp_using_register[register_index] = 0;

            register_index = 0;
            sscanf(var1, "t%d", &tmp_nr);
            while (tmp_using_register[register_index] != tmp_nr)
            {
                register_index ++;
            }
            char *var1 = set_var_register(register_index);
            sscanf(tx, "t%d", &tmp_nr);
            tmp_using_register[register_index] = tmp_nr;

            print_asm(op, var1, var2);
        }
        else if (var1[0] == 't'){
            sscanf(var1, "t%d", &tmp_nr);
            while (tmp_using_register[register_index] != tmp_nr)
            {
                register_index ++;
            }
            char *var1 = set_var_register(register_index);
            sscanf(tx, "t%d", &tmp_nr);
            tmp_using_register[register_index] = tmp_nr;

            print_asm(op, var1, var2);
        }
        else if (var2[0] == 't'){
            while (tmp_using_register[register_index] != 0){
                register_index ++;
            }
            strcpy(value_holder, var1);
            char *var1 = set_var_register(register_index);
            fprintf(w_file, "mov %s, %s\n",var1,value_holder);
            
            register_index = 0;
            sscanf(var2, "t%d", &tmp_nr);
            while (tmp_using_register[register_index] != tmp_nr)
            {
                register_index ++;
            }
            char *var2 = set_var_register(register_index);
            sscanf(tx, "t%d", &tmp_nr);
            tmp_using_register[register_index] = tmp_nr;

            print_asm(op, var1, var2);
            
            fprintf(w_file, "mov %s, %s\n",var2,var1);
        }
        else{
            while (tmp_using_register[register_index] != 0){
                register_index ++;
            }
            strcpy(value_holder, var1);
            char *var1 = set_var_register(register_index);
            fprintf(w_file, "mov %s, %s\n",var1,value_holder);

            sscanf(tx, "t%d", &tmp_nr);
            tmp_using_register[register_index] = tmp_nr;
            
            print_asm(op, var1, var2);
        }

    }
}


int main(void){
    char ch;
    int idx = 0;

    FILE *file;
    file = fopen("generated/3_intermed.txt", "r");

    w_file = fopen("generated/4_ASM.s", "w");

    char asm_header[] = "section .data\nbuffer db 20 dup(0)\nsection .text\nglobal _start\n_start:\n\n";
    fprintf(w_file, asm_header);

    while((ch = fgetc(file)) != EOF){
        expression[idx++] = ch;
        if(ch == '\n'){
            treat_line();
            memset(expression, 0, sizeof(expression));
            idx = 0;
        }
    }
    char asm_convert_and_print_answer[] = "\nmov rcx, buffer + 19\nmov rbx, 10\ntest r8, r8\njnz convert_loop\ndec rcx\nmov byte [rcx], '0'\njmp print_result\n\nconvert_loop:\nxor rdx, rdx\nmov rax, r8\ndiv rbx\nmov r8, rax\nadd dl, '0'\ndec rcx\nmov [rcx], dl\ntest r8, r8\njnz convert_loop\n\nprint_result:\nmov rax, 1\nmov rdi, 1\nlea rsi, [rcx]\nlea rdx, [buffer + 20]\nsub rdx, rcx\nsyscall\n";
    char asm_exit[] = "\nmov rax, 60\nxor rdi, rdi\nsyscall";
    fprintf(w_file, asm_convert_and_print_answer);
    fprintf(w_file, asm_exit);

    fclose(file);
    fclose(w_file);
    return 0;
}
