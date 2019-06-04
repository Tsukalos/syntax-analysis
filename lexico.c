#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexico.h"

char prox;
int idcode = 0;
int numcode = 0;
int current_line = 1;
FILE *f;

void proximo()
{
    if(f == NULL){
        f = stdin;
    }
    prox = getc(f);
    if (feof(f))
        exit(0);

    if (prox == 10)
    {
        current_line += 1;
    }
}

void erro()
{
    printf("\nErro no analisador lexico na linha %d\n", current_line);
    exit(1);
}

int is_special()
{
    return (prox == '.' || prox == ',' || prox == ';' || prox == '(' || prox == ')' || prox == ':' || prox == '=' || prox == '<' || prox == '>' || prox == '+' || prox == '-' || prox == '*');
}

int is_keyword(char atomo[])
{
    return !strcmp(atomo, "integer") || !strcmp(atomo, "program") || !strcmp(atomo, "var") || !strcmp(atomo, "procedure") || !strcmp(atomo, "function") || !strcmp(atomo, "begin") || !strcmp(atomo, "end") || !strcmp(atomo, "if") || !strcmp(atomo, "then") || !strcmp(atomo, "else") || !strcmp(atomo, "while") || !strcmp(atomo, "do") || !strcmp(atomo, "or") || !strcmp(atomo, "and") || !strcmp(atomo, "div") || !strcmp(atomo, "not");
}

int is_letter()
{
    return (prox >= 64 && prox <= 90) || (prox >= 97 && prox <= 122);
}

int is_digit()
{
    return (prox >= 48 && prox <= 57);
}

int is_letter_or_digit()
{
    return is_letter() || is_digit();
}

char *codigo(char s[])
{
    if (!strcmp(s, ":="))
        return "c_atrib";
    if (!strcmp(s, ","))
        return "c_sepa";
    if (!strcmp(s, ";"))
        return "c_endexp";
    if (!strcmp(s, "="))
        return "c_equal";
    if (!strcmp(s, "("))
        return "c_scope_o";
    if (!strcmp(s, ")"))
        return "c_scope";
    if (!strcmp(s, ">"))
        return "c_greater";
    if (!strcmp(s, "<"))
        return "c_lesser";
    if (!strcmp(s, "<="))
        return "c_lesser_eq";
    if (!strcmp(s, ">="))
        return "c_greater_eq";
    if (!strcmp(s, "<>"))
        return "c_unequal";
    if (!strcmp(s, "+"))
        return "c_plus";
    if (!strcmp(s, "-"))
        return "c_minus";
    if (!strcmp(s, "*"))
        return "c_times";
    if (!strcmp(s, "."))
        return "c_point";
    if (!strcmp(s, ":"))
        return "c_double_point";
    if (!strcmp(s, "program"))
        return "c_program";
    if (!strcmp(s, "var"))
        return "c_var";
    if (!strcmp(s, "procedure"))
        return "c_procedure";
    if (!strcmp(s, "function"))
        return "c_function";
    if (!strcmp(s, "begin"))
        return "c_begin";
    if (!strcmp(s, "end"))
        return "c_end";
    if (!strcmp(s, "if"))
        return "c_if";
    if (!strcmp(s, "then"))
        return "c_then";
    if (!strcmp(s, "else"))
        return "c_else";
    if (!strcmp(s, "while"))
        return "c_while";
    if (!strcmp(s, "do"))
        return "c_do";
    if (!strcmp(s, "or"))
        return "c_or";
    if (!strcmp(s, "and"))
        return "c_and";
    if (!strcmp(s, "div"))
        return "c_div";
    if (!strcmp(s, "not"))
        return "c_not";
    if (!strcmp(s, "integer"))
        return "c_int";
    if (!strcmp(s, "boolean"))
        return "c_bool";
    return NULL;
}

void analisador(char simbolo[])
{
    //simbolo[0] = 0;
    char atomo[50] = "";
    char c = 0;
    char str[10] = "";
    char s = 0;
    while (prox <= 32)
        proximo();

    if (is_special())
    {
        s = prox;
        proximo();
        switch (s)
        {
        case ':':
            if (prox == '=')
            {
                strcpy(atomo, ":=");
                proximo();
            }
            else
            {
                strcat(atomo, &s);
            }
            break;
        case '<':
            if (prox == '=')
            {
                strcpy(atomo, "<=");
                proximo();
            }
            else
            {
                strcat(atomo, &s);
            }
            break;
        case '>':
            if (prox == '=')
            {
                strcpy(atomo, ">=");
                proximo();
            }
            else
            {
                strcat(atomo, &s);
            }
            break;
        default:
            strcat(atomo, &s);
            break;
        }

        strcpy(simbolo,codigo(atomo));
    }
    else if (is_letter())
    {
        do
        {
            c = tolower(prox);
            strcat(atomo, &c);
            proximo();
        } while (is_letter_or_digit());
        if (is_keyword(atomo))
        {
            strcpy(simbolo,codigo(atomo));
        }
        else
        {
            strcat(simbolo, "id");
            // sprintf(str, "%d",idcode);
            // idcode +=1;
            // strcat(simbolo,str);
        }
    }
    else if (is_digit())
    {
        do
        {
            c = prox;
            strcat(atomo, &c);
            proximo();
        } while (is_digit());
        if (is_letter())
        {
            erro();
        }
        strcpy(simbolo,"c_num");
        // sprintf(str,"%d",numcode);
        // numcode +=1;
        // strcat(simbolo, str);
    }
    else
    {
        erro();
    }
    //printf("%s ", simbolo);
}

