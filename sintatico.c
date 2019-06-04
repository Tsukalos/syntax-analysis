#include <stdio.h>
#include <string.h>
#include "lexico.h"
#include <stdlib.h>

char current_lex[30];

int bloco();
int comando();
int comando_sem_rotulo();
int comando_composto();
int fator();
int lista_de_expressoes();
int termo();
int expressao();
int variavel();

int cmplex(char l[20])
{
    return !strcmp(current_lex, l);
}

int next()
{
    current_lex[0] = 0;
    analisador(current_lex);
    printf("%s ", current_lex);
}

int error(const char s[])
{
    printf("\nErro no analisador sintatico na linha %d, em %s()\n", LINE, s);
    exit(1);
}

int relacao()
{
    if (!(cmplex("c_equal") || cmplex("c_greater") || cmplex("c_lesser") || cmplex("c_greater_eq") || cmplex("c_lesser_eq") || cmplex("unequal")))
        return 0;
    return 1;
}

int tipo()
{
    if (!(cmplex("c_int") || cmplex("c_bool")))
        error(__func__);
}

int lista_id()
{
    while (cmplex("id"))
    {
        next();
        if (!cmplex("c_sepa"))
        {
            break;
        }
        next();
    }
}

int secao_param_formais()
{
}

int param_formais()
{
    if (cmplex("c_scope_o"))
    {
        next();
        while (cmplex("c_var") || cmplex("id"))
        {
            if (cmplex("c_var"))
            {
                next();
                lista_id();
            }
            else
            {
                lista_id();
            }
            if (!cmplex("c_double_point"))
                error(__func__);
            next();
            if (!cmplex("id"))
                error(__func__);
            next();
            if (!cmplex("c_sepa"))
                error(__func__);
            next();
        }
        if (!cmplex("c_scope"))
            error(__func__);
        next();
    }
}

int declara_procedure()
{
    next();
    if (!cmplex("id"))
        error(__func__);
    next();
    param_formais();
    if (!cmplex("c_endexp"))
        error(__func__);
    bloco();
}

int declara_funcao()
{
    next();
    if (!cmplex("id"))
        error(__func__);
    next();
    param_formais();
    if (!cmplex("c_double_point"))
        error(__func__);
    next();
    if (!cmplex("id"))
        error(__func__);
    next();
    if (!cmplex("c_endexp"))
        error(__func__);
    bloco();
}

int parte_declara_sub_rot()
{
    while (cmplex("c_procedure") || cmplex("c_function"))
    {
        if (cmplex("c_procedure"))
        {
            declara_procedure();
        }
        else
        {
            declara_funcao();
        }
        next();
        if (!cmplex("c_endexp"))
            error(__func__);
        next();
    }
}

int declara_vars()
{
    lista_id();

    if (!cmplex("c_double_point"))
        error(__func__);
    next();
    tipo();
}

int parte_declara_vars()
{
    if (!cmplex("c_var"))
        return 0;
    next();
    while (cmplex("id"))
    {
        declara_vars();
        next();
        if (!cmplex("c_endexp"))
            break;
        next();
    }
}

int lista_de_expressoes()
{
    next();
    while (expressao())
    {
        if (!cmplex("c_sepa"))
        {
            break;
        }
        next();
    }
}

int fator()
{
    if (cmplex("id"))
    {
        variavel();
        next();
        if (cmplex("c_scope_o"))
        {
            lista_de_expressoes();
            if (cmplex("c_scope"))
            {
                next();
            }
            else
            {
                error(__func__);
            }
        }
    }
    else
    {
        if (cmplex("c_num"))
        {
            next();
        }
        else
        {
            if (cmplex("c_scope_o"))
            {
                next();
                expressao();
                if (!cmplex("c_scope"))
                    error(__func__);
                next();
            }
            else
            {
                error(__func__);
            }
        }
    }
}

int termo()
{
    fator();
    // next();
    while (cmplex("c_times") || cmplex("c_div") || cmplex("c_and"))
    {
        next();
        fator();
        // next();
    }
}

int expressao_simples()
{
    if (cmplex("c_plus") || cmplex("c_minus"))
    {
        next();
    }
    termo();
    // next();
    while (cmplex("c_plus") || cmplex("c_minus") || cmplex("c_or"))
    {
        next();
        termo();
    }
}

int expressao()
{
    expressao_simples();
    // next();
    if (relacao())
    {
        next();
        expressao_simples();
    }
    return 1;
}

int variavel()
{
    // Não há nada aqui
}

int atribuicao()
{
    variavel();
    next();
    expressao();
}

int chamada_procedimento()
{
    if (!cmplex("c_scope_o"))
        return 0;
    lista_de_expressoes();
    if (!cmplex("c_scope"))
    {
        error(__func__);
    }
}

int comando_condicional()
{
    expressao();
    if (!cmplex("c_then"))
        error(__func__);
    next();
    comando_sem_rotulo();
    next();
    if (cmplex("c_else"))
    {
        next();
        comando_sem_rotulo();
    }
}

int comando_repetitivo()
{
    expressao();
    if (!cmplex("c_do"))
        error(__func__);
    next();
    comando_sem_rotulo();
}

int comando_sem_rotulo()
{
    if (cmplex("id"))
    {
        next();
        if (cmplex("c_atrib"))
        {
            atribuicao();
        }
        else
        {
            chamada_procedimento();
            if(!cmplex("c_endexp"))
                next();
        }
    }
    else
    {
        if (cmplex("c_if"))
        {
            next();
            comando_condicional();
        }
        else
        {
            if (cmplex("c_while"))
            {
                next();
                comando_repetitivo();
            }
            else
            {
                comando_composto();
            }
        }
    }
}

int comando()
{
    // if (cmplex("c_num"))
    // {
    //     next();
    //     if (!cmplex("c_double_point"))
    //         error(__func__);
    //     next();
    // }
    comando_sem_rotulo();
    if (!cmplex("c_endexp"))
    {
        return 0;
    }
    next();
    return 1;
}

int comando_composto()
{
    if (!cmplex("c_begin"))
        return 1;
    next();
    while (comando())
        ;
    if (!cmplex("c_end"))
        error(__func__);
    return 0;
}

int bloco()
{
    next();
    parte_declara_vars();
    parte_declara_sub_rot();
    if (comando_composto())
        error(__func__);
}

int programa()
{
    next();
    if (cmplex("c_program"))
    {
        next();
        if (cmplex("id"))
        {
            bloco();
            next();
            if(!cmplex("c_end")) error(__func__);
            next();
            if(!cmplex("c_point")) error(__func__);
        }
        else
        {
            error(__func__);
        }
    }
    else
    {
        error(__func__);
    }
    return 1;
}

int main()
{
    f = fopen("input.txt", "r");
    if(f == NULL)
        f = stdin;
    if (!feof(f))
    {
        current_lex[0] = 0;

        programa();
    }
}