#ifndef LEX_H_   /* Include guard */
#define LEX_H_
#define LINE current_line
FILE *f;
int current_line;
void proximo();
void erro();
void analisador(char simbolo[]);

#endif // LEX_H_