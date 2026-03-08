/** 
 * @brief Codificacao do modulo do analisador sintatico  
 */

// Inclusao do cabecalho
#include "synt.h"

// Variaveis globais
type_token *lookahead;
extern type_symbol_table_variables global_symbol_table_variables;

type_symbol_table_variables* localTableAtualBoolean;

extern type_symbol_table_func global_symbol_table_Function;

extern type_symbol_table_string symbol_table_string;
extern char output_file_name[MAX_CHAR];
extern FILE *output_file;

/**
 * @brief Verifica se o proximo caracter (a frente) na cadeia eh o esperado
 *
 * @param token_tag (int) codigo do token a ser verificado
 * @return int true/false
 */
int match(int token_tag)
{
    if (lookahead->tag == token_tag)
    {
        lookahead = getToken(); // Pega o proximo token por meio do lexico
        return true;
    }
    // printf("%d, %d", token_tag, lookahead->tag);
    printf("[ERRO] Entrada recebida: %s\n", lookahead->lexema);
    printf("[ERRO] Entrada esperada: %d\n", token_tag);
    return ERROR;
}

/**
 * @brief Regra de derivacao inicial
 */
int program(void)
{
    int ok1, ok2;

    gen_preambule(); // Temporariamente cria um preambulo adicional que permite o uso das funcoes scanf e printf
    ok1 = declarations();
    if((match(BEGIN) == true)){
        gen_preambule_code(); // Chamada do gerador de codigo para escrita do cabecalho da secao de codigo
        ok2 = statements(0, 0);
        
        gen_epilog_code();
        //          gen_data_section(); // Chamada do gerador de codigo para declaracao de variaveis
        //printf("dsadas %d", lookahead->tag);
        if(match(END) == true){
            if(ok1 != ERROR && ok2 != ERROR){
                int aux = func_main();
                gen_data_section(); // Chamada do gerador de codigo para declaracao de variaveis
                return aux;
                if(lookahead->tag!=ENDTOKEN){
                    printf("codigo escrito fora de escopo, Token do lookahead = %d, lexema lookahead = %s\n", lookahead->tag, lookahead->lexema);
                    return ERROR;
                }
                return false;
            }
        }else{
            printf("\nEra esperado um End depois da secao de codigo\n");
            return ERROR;
        }
    }

    //printf("ads %d %d\n", ok1, ok2);
    return ERROR;
}

int func_main(){
    int aux;
    while((aux = Func_code()) == true);

    if(aux == false){
        for (int i = 0; i < global_symbol_table_Function.n_variables; i++)
        {
            if (global_symbol_table_Function.variable[i].flag == 0){
                printf("\nNem todas as funcoes prototipadas foram implementadas\n\n");
                return ERROR;
            }
        }
        return true;
    }
    return ERROR;
}

int Func_code(){
    //type_symbol_table_variables* localVariables = (type_symbol_table_variables*) malloc(sizeof(type_symbol_table_variables));     //malloc melhor --------------------------
    //initSymbolTableVariables(localVariables);
    type_symbol_function_entry* func;
    int var_type;
    char var_name[MAX_CHAR];

    if(global_symbol_table_Function.n_variables > 0 && lookahead->tag != ENDTOKEN){
        var_type = lookahead->tag;
        match(var_type);
        
        if((func = sym_func_find(lookahead->lexema, &global_symbol_table_Function)) != NULL){
            strcpy(var_name, lookahead->lexema);
            if(func->flag == 1){
                printf("Essa função já foi implementada");
                return ERROR;
            }
            if(match(ID)==ERROR){
                printf("\nEra esperado id valido");
                return ERROR;
            }

            if(func->type != var_type){
                printf("\nO tipo da funcao esta errado\n");
                return ERROR;
            }

            // AQUI VAI FAZER CODIGO DE LABEL DE SALTO ASSEMBLY
            gen_label(func->label);

            match(OPEN_PAR);
            type_symbol_table_entry* parametros;
            if((parametros = DeclaracaoFunc(func)) == NULL){
                printf("\nproblema na declaracao da funcao\n");
                return ERROR;
            }
            //printf("\nparametros  %s\n", parametros[1].name);
            if (lookahead->tag == CLOSE_PAR){
            match(CLOSE_PAR);
            if(match(BEGIN) == true){
                if(statements(parametros, func->nparams) != ERROR){
                    if(match(END) == true){
                        func->flag = 1;
                        gen_label_JAL_Function_RA();
                        return true;
                    }
                    printf("\nera esperado um end na funcao");
                    return ERROR;
                }
                printf("\nerro dentro dos steatments da funcao");
                }else{
                    printf("\nera esperado um begin na funcao");
                    return ERROR;
                } 
            }else{
                printf("era esperado begin para inicio da funcao");
                return ERROR;
            }
        }else{
            printf("\nError Area Function\n");
            return ERROR;
        }
        //gen_label_JAL_Function_RA();
        
        // type_symbol_table_variables* aux = &global_symbol_table_variables;
        // while(aux->next != NULL){
        //     aux = aux->next;
        // }
        // aux->next = localVariables;
    } 
    if(lookahead->tag==ENDTOKEN){
        return false;
    }
    return true;
}

type_symbol_table_entry* DeclaracaoFunc(type_symbol_function_entry* func){
    char var_name[MAX_CHAR];
    int i = 0;
    int var_type = lookahead->tag;
    int check;
    type_symbol_table_entry* result = (type_symbol_table_entry*) malloc(func->nparams * sizeof(type_symbol_table_entry));
    
    while (i < func->nparams && (check = match(func->params[i]))){
        strcpy(var_name, lookahead->lexema);
        if(match(ID)==ERROR){
            printf("\nEra esperado id valido");
            return NULL;
        }
        // if(sym_declare(var_name, var_type, 0, localVariables) == NULL){
        //     return ERROR;
        // }
        strcpy(result[i].name, var_name);
        result[i].type = var_type;
        result[i].addr = 0;
        
        i++;
        if(i != func->nparams){
            if(match(COMMA) == ERROR){
                printf("era esperado uma virgula\n");
                return NULL;
            }
        }else if(lookahead->tag == COMMA && func->nparams == i){
            printf("uma virgula nao esperada\n");
            return NULL;
        }
        var_type = lookahead->tag;
    }
        
    
    if(check == false){
        printf("[ERRO DE FUNCAO]Era esperado parametros validos, parametro recebido = %s\n", lookahead->lexema);
        return NULL;
    }
        
    
    return result; 

}

/**
 * @brief Regra de derivacao para declaracoes
 */
int declarations(void)
{
    int ok1;
    while ((ok1 = declaration()) == true); // Laco para processamento continuo das declaracoes
    if(ok1 != ERROR){
        return true;
    }
        return ERROR;
}

/**
 * @brief Regra de derivacao declaracao
 * @return int true/false
 */
int declaration (void) {
    char var_name[MAX_CHAR];
    int var_type;

    //Verifica o tipo da variavel
    var_type = lookahead->tag;
    if ( var_type == INT || var_type == FLOAT || var_type == CHAR || var_type == STRING) { 
        match(var_type);
        strcpy(var_name, lookahead->lexema);

        if(match(ID) == ERROR){
            printf("Era esperado um id nessa declaracao de variavel");
            return ERROR;
        }


        if (lookahead->tag == SEMICOLON){
            //Variable  DECLARATION V
            if(match(SEMICOLON) == ERROR){
                printf("Era esperado um ; nessa declaracao de variavel");
                return ERROR;
            }
            int a = DeclaracaoV(var_name, var_type, &global_symbol_table_variables);
            return a;
        }else if(lookahead->tag == OPEN_PAR){
            //function  DECLARATION F
            match(OPEN_PAR);
            return DeclaracaoF(var_name, var_type);
        }
        else{
            return ERROR;
            //return false;
        }
    } else if (lookahead->tag == ENDTOKEN ||
                lookahead->tag == READ ||
                lookahead->tag == WRITE) {
        //Verifica se fim de arquivo
        printf("[ERROR]Nao foi iniciado o Begin do contexto main para comandos statements ou para que o programa fosse executado\n");
        return ERROR;         
    } else if (lookahead->tag == BEGIN) {
        //Verifica se fim de arquivo
        return false;         
    }else {
        printf ("[ERRO] Tipo desconhecido: %d %s.\n", lookahead->tag, lookahead->lexema);
        return ERROR; 
    }
}



// int DeclaracaoV(char var_name[MAX_CHAR],  int var_type){
//     int ok2;
//     type_symbol_table_entry *search_symbol;
//     search_symbol = sym_find( var_name, &global_symbol_table_variables );
//     if ( search_symbol != NULL) {
//         printf ("[ERRO] Variavel '%s' ja declarada.\n", var_name); 
//         return ERROR;
//     } else {
//         if(sym_declare( var_name, var_type, 0, &global_symbol_table_variables) == NULL){
//             return ERROR;
//         }else{
//             return true;
//         }
//     }
// }

int DeclaracaoV(char var_name[MAX_CHAR],  int var_type, type_symbol_table_variables*TableVariables){
    int ok2;
    type_symbol_table_entry *search_symbol;
    search_symbol = sym_find( var_name, TableVariables );
    if ( search_symbol != NULL) {
        printf ("[ERRO] Variavel '%s' ja declarada.\n", var_name); 
        return ERROR;
    } else {
        if(sym_declare( var_name, var_type, 0, TableVariables) == NULL){
            return ERROR;
        }else{
            return true;
        }
    }
}

int DeclaracaoF(char var_name[MAX_CHAR],  int var_type){
    //type_symbol_table_entry *search_symbol;
    // int ok1, ok2;
    //Verifica o tipo da variavel
    int tipoFunc = var_type;

    var_type = lookahead->tag;
    int params[MAX_PARAMS];
    int i = 0;

    if(sym_func_find(var_name, &global_symbol_table_Function) == NULL){
        if (lookahead->tag == CLOSE_PAR){
            match(CLOSE_PAR);
            if(match(SEMICOLON)){
                    char label[MAX_CHAR];
                    gen_label_FuncName(label);
                    sym_func_declare(var_name, tipoFunc, 0, params, label, &global_symbol_table_Function);
                    return true;
                }else{
                    return ERROR;
                }
            return true;
        }
        while ((var_type == INT || var_type == FLOAT || var_type == CHAR || var_type == STRING)){
            
            match(var_type);
            params[i] = var_type; 
            //printf("lexema %d ", lookahead->tag);
            if (lookahead->tag == CLOSE_PAR){
                match(CLOSE_PAR);
                if(match(SEMICOLON)){
                    char label[MAX_CHAR];
                    gen_label_FuncName(label);
                    sym_func_declare(var_name, tipoFunc, i+1, params, label, &global_symbol_table_Function);
                    return true;
                }else{
                    return ERROR;
                }
            }else if(lookahead->tag == COMMA){
                match(COMMA);
            }else{
                printf("[ERRO DE FUNCAO] A funcao nao foi fechada corretamente\n");
                return ERROR;
                
            }
            var_type = lookahead->tag;
            i++;
        }
        
    }else{
        printf("funcao ja declarada\n");
    }
    printf("[ERRO DE FUNCAO]Era esperado parametros validos, parametro recebido = %s\n", lookahead->lexema);
    return ERROR; 

}



/**
 * @brief Regra de derivacao para comandos
 */
int statements(type_symbol_table_entry* parametros, int quant)
{   
    type_symbol_table_variables* localVariables = (type_symbol_table_variables*) malloc(sizeof(type_symbol_table_variables));     //malloc melhor --------------------------
    initSymbolTableVariables(localVariables);
    
    if(parametros != 0){
        int i = 0;
        while (i < quant){
            if(sym_declare(parametros[i].name, parametros[i].type, 0, localVariables) == NULL){
                printf("error ao declarar variavel;\n");
                return ERROR;
            }
            //printf("\n%s\n", parametros[i].name);
            i++;
        }
    }

    int ok1;
    while ((ok1 = statement(localVariables)) == true); // processa enquanto houver comandos

    type_symbol_table_variables* aux = &global_symbol_table_variables;
    if(localVariables->n_variables > 0){
        while(aux->next != NULL){
        aux = aux->next;
    }
    aux->next = localVariables;

    }
  
    if(ok1 != ERROR){
        return ok1;
    }
        return ERROR;
}

int assignment(char var_name[MAX_CHAR], type_symbol_table_variables* localVariables)
{
    type_symbol_table_entry *search_symbol;      
    search_symbol = sym_find(var_name, localVariables);

    if (search_symbol != NULL)
    {
        E();
        gen_assignment(var_name, search_symbol->type);
        return true;

    }else{
        printf("[ERRO] Variável não declarada: %s\n", var_name);
        return ERROR;
    }
}


/**
 * @brief Regra de derivacao que processa os comandos
 *
 * @return int true/false
 */
int statement(type_symbol_table_variables*localVariables)
{
    char lexeme_of_id[MAX_CHAR];
    type_symbol_table_entry *search_symbol;
    type_symbol_table_string_entry *gen_string;
    int ok1, ok2, type;
    char string_value[MAX_CHAR];

    if (lookahead->tag == READ)
    {
        match(READ);
        strcpy(lexeme_of_id, lookahead->lexema);
        ok1 = match(ID);
        search_symbol = sym_find(lexeme_of_id, localVariables);
        if (search_symbol != NULL)
        {
            type = search_symbol->type;
            gen_read(lexeme_of_id, type);
            ok2 = match(SEMICOLON);
            return ok1 && ok2;
        }
        search_symbol = sym_find(lexeme_of_id, &global_symbol_table_variables);
        if (search_symbol != NULL)
        {
            type = search_symbol->type;
            gen_read(lexeme_of_id, type);
            ok2 = match(SEMICOLON);
            return ok1 && ok2;
        }
        else
        {
            printf("[ERRO] Simbolo desconhecido (Variavel nao declarada): %s\n", lexeme_of_id);
            return ERROR;
        }
    }
    else if (lookahead->tag == WRITE)
    {
        match(WRITE);
        if (lookahead->tag == STRING)
        {
            strcpy(string_value, lookahead->lexema);
            // TODO: verificar se o valor da constante ja foi incluida na tabela de simbolos
            gen_string = sym_string_declare(string_value);
            match(STRING);
            if (gen_string != NULL)
            {
                strcpy(lexeme_of_id, gen_string->name);
                gen_write(lexeme_of_id, STRING);
                match(SEMICOLON);
            }
        }
        else if (lookahead->tag == ID)
        {
            strcpy(lexeme_of_id, lookahead->lexema);
            match(ID);
            search_symbol = sym_find(lexeme_of_id, localVariables);
            if (search_symbol != NULL)
            {
                type = search_symbol->type;
                gen_write(lexeme_of_id, type);
                match(SEMICOLON);
                return true;
            }
            search_symbol = sym_find(lexeme_of_id, &global_symbol_table_variables);
            if (search_symbol != NULL)
            {
                type = search_symbol->type;
                gen_write(lexeme_of_id, type);
                match(SEMICOLON);
                return true;
            }
            else
            {
                printf("[ERRO] Simbolo desconhecido (Variavel nao declarada): %s\n", lexeme_of_id);
                return ERROR;
            }
        }
    }
    else if (lookahead->tag == IF) {
        char label_else[MAX_CHAR];
        char label_end[MAX_CHAR];
        gen_label_name(label_else);
        gen_label_name(label_end);

        match(IF);
        match(OPEN_PAR);
        localTableAtualBoolean = localVariables;
        B(); //Expressao booleana
        gen_cond_jump(label_else);
        match(CLOSE_PAR);
        
        match(BEGIN);
        statements(0, 0);
        //printf("\n -----passou end   %d\n", lookahead->tag);
        match(END);
        //printf("\n -----passou end   %d\n", lookahead->tag);
        gen_incond_jump(label_end);
        gen_label(label_else);
        //printf("passou end");
        //Verifica se ocorre um ELSE
        if (lookahead->tag == ELSE) {
            match(ELSE);
            match(BEGIN);
            statements(0, 0);
            match(END);
        }
        gen_label(label_end);
        return 1;
    }
    else if (lookahead->tag == WHILE)
    {
        char label_begin[MAX_CHAR];
        int test;
        char label_end[MAX_CHAR];
        gen_label_name(label_begin);
        gen_label_name(label_end);

        match(WHILE);
        match(OPEN_PAR);
        gen_label(label_begin);
        localTableAtualBoolean = localVariables;
        B(); // Expressao booleana
        gen_cond_jump(label_end);
        match(CLOSE_PAR);
        match(BEGIN);
        // statement(0, 0);
        statements(0, 0);
        match(END);
        gen_incond_jump(label_begin);
        gen_label(label_end);
        return true;
    }
    else if (lookahead->tag == ID)
    {
        char var_name[MAX_CHAR];
        strcpy(var_name, lookahead->lexema);

        match(ID);
        if(lookahead->tag == ASSIGN){
            match(ASSIGN);
            int result = assignment(var_name, localVariables);
            match(SEMICOLON);
            return result;
        }
        else if(lookahead->tag == OPEN_PAR){
            int a,b;
            match(OPEN_PAR);
            int result = fun_call_cmd(var_name, localVariables);
            a = match(CLOSE_PAR);
            b = match(SEMICOLON);
            if(a == ERROR || b == ERROR || result == ERROR){
                return ERROR;
            }
            return result;
        }
        return ERROR;
    }
    else if (lookahead->tag == INT || lookahead->tag == FLOAT || lookahead->tag == CHAR || lookahead->tag == STRING){
        int var_type = lookahead->tag;
        match(lookahead->tag);
        char var_name[MAX_CHAR];
        strcpy(var_name, lookahead->lexema);
        if(match(ID) == ERROR){
            printf("era esperado um id\n");
            return ERROR;
        }
        if(match(SEMICOLON) == ERROR){
            printf("era esperado um ;\n");
            return ERROR;
        }
        return DeclaracaoV(var_name, var_type, localVariables);
    }
    else if (lookahead->tag == END)
    {
        return false;
    }
    else if (lookahead->tag == ENDTOKEN)
    {
        printf("[ERROR]Begin do contexto main nao foi fechado com End\n");
        
        return ERROR;
    }
    else
    {
        printf("[ERRO] Comando desconhecido.\nTag=%d; Lexema=%s\n", lookahead->tag, lookahead->lexema);
        return ERROR;
    }
}



int fun_call_cmd(char var_name[MAX_CHAR],  type_symbol_table_variables*localVariables){
    type_symbol_function_entry* aux;
    type_symbol_table_entry *auxVar;
    if((aux = sym_func_find(var_name, &global_symbol_table_Function)) != NULL){

        if(aux->nparams == 0){
            gen_label_JAL_Function(aux->label);
            return true;
        }
        int i = 0;
        while (i < aux->nparams)
        {   
            //printf("\ntestes %d    %d\n", aux->params[i], lookahead->tag);
            if(lookahead->tag == ID){
                if(((auxVar = sym_find(lookahead->lexema, &global_symbol_table_variables)) != NULL) || (auxVar = sym_find(lookahead->lexema, localVariables)) != NULL){
                    if(auxVar->type == aux->params[i]){
                        match(lookahead->tag);
                        if(i != aux->nparams-1){                //ATENÇAOOOOOO -----------------------------------------------
                           match(COMMA); 
                        }
                    }
                }else{
                    printf("[ERROR] Variavel passada por parametro nao existente\n");
                    return ERROR;
                }

            }else if (aux->params[i] == lookahead->tag){
                match(lookahead->tag);
                if(i != aux->nparams -1){
                    match(COMMA); 
                }
            }else{
                printf("[ERROR] PARAMETROS DA FUNCAO MAL DECLARADOS\n");
                return ERROR;
            }
            i++;
        }
        
    }else{
        printf("[Error] voce esta chamando uma funcao q n existe\n");
        return ERROR;
    }
    gen_label_JAL_Function(aux->label);
    return true;
}


/**
 * @brief Regra de derivação que analiza expressoes booleanas
 *        no padrao 'id op_rel expr'
 *
 */
int B() {
    int operator;
    char lexema_of_id[MAX_CHAR];
    strcpy(lexema_of_id, lookahead->lexema);
    //printf("\n%d esperavase -> A\n", lookahead->tag);
    //tste
    int a1 = E();
    //gen_id(lexema_of_id);
    if(a1 == true){
        if (boolOperator(&operator) ) {
            if(E()){
                gen_bool(operator);
                //printf("fundsc\n");
                return true;
            }       
        // } else {
        //     return false;
        // }
        }
    }
    
    printf("\n[Error] Expressao MAL FORMADA\n");
    return ERROR;
}

int boolOperator(int *operator) {
    int lookahead_tag;
    lookahead_tag = lookahead->tag;
    *operator = -1;
    //printf("\n%d esperavase -> \n", lookahead_tag);
    if (lookahead_tag == EQUAL ||
        lookahead_tag == NE ||
        lookahead_tag == LT ||
        lookahead_tag == GT ||
        lookahead_tag == LE ||
        lookahead_tag == GE) {
            *operator = lookahead_tag;
            match(lookahead_tag);
            return true;
    } else {
        printf("[ERRO] Operador relacional experado.\n");
        return false;
    }
}

//------
/* Funções que representam a gramatica que reconhece expressoes aritmeticas */
/* Elaborada nas primeiras aulas */
int E() {
    if(lookahead->tag == ID){
        int a1;      
        if ((sym_find(lookahead->lexema, localTableAtualBoolean)!= NULL)) {
            a1 = match(ID);
            return a1;
        }else if((sym_find(lookahead->lexema, &global_symbol_table_variables)!= NULL)){
            a1 = match(ID);
            return a1;
        } else {
            printf("\n[ERRO] Simbolo desconhecido (Variavel nao declarada): %s\n", lookahead->lexema);
            return false;
        }
        //printf("chegou aq %d", a1);
        
    }else{
        int b1, b2;
        b1 = T();
        if (b1) 
            b2 = ER();
        return b1 && b2;
    }

}

int ER() {
    if (lookahead->tag == '+')  { 
        int b1, b2;
        match('+');
        b1 = T();
        genAdd();
        if (b1) 
            b2 = ER();
        return b1 && b2;
    } else if (lookahead->tag == '-') {
        int b1, b2;
        match('-');
        b1 = T();
        genSub();
        if (b1)
            b2 = ER();
        return b1 && b2;      
    } else if (lookahead -> tag == ')') {
        return true;
    } else if (lookahead -> tag == ENDTOKEN) {
        return true;
    } else if (lookahead -> tag == '*') {
        return true;
    } else if (lookahead -> tag == '/') {
        return true;
    } else {
        return true;
    }
}

int T() {
    int b1, b2;
    b1 = F();
    if (b1)
        b2 = TR();
        //printf("dsad %d", b2);
    return b1 && b2;
}

int TR() {
    if (lookahead->tag == '*') { 
        int b1, b2;
        match('*');
        b1 = F();
        genMult();
        if (b1)
            b2 = TR();
        return b1 && b2;
    } else if (lookahead -> tag == '/') {
        int b1, b2;
        match('/');
        b1 = F();
        genDiv();
        if (b1)
            b2 = TR();
        return b1 && b2;
    } else if (lookahead->tag == ')') {
        return true;
    } else if (lookahead->tag == ENDTOKEN){ //EOF
        return true;
    } else if (lookahead->tag == '+') {
        return true;
    } else if (lookahead->tag == '-') {
        return true;
    } else {
        return true;
    }
}

int F() {
    if (lookahead->tag == '(') {
        int b1,b2;
        match('(');
        b1 = E();
        if (b1)
            b2 = match(')');
        return b1 && b2;
    } else if (lookahead->tag == INT || lookahead->tag == FLOAT) {
        int b1;
        char lexema[MAX_TOKEN];
        strcpy(lexema, lookahead->lexema);
        b1 = match(lookahead->tag); //substituir 'id' por NUM
        genNum(lexema);
        return b1;
    } else {
        return false;
    }
}
/*fim da importação */
//------

//--------------------- MAIN -----------------------

/**
 * @brief Funcao principal (main) do compilador
 *
 * @return int
 */
int main(int argc, char *argv[])
{

    // Inicializa a tabela de simbolo global
    initSymbolTableVariables(&global_symbol_table_variables);
    initSymbolTableString();

    //global_symbol_table_Function.n_variables = 0;

    // Verifica a passagem de parametro
    if (argc != 2)
    {
        printf("[ERRO]\n\tÉ necessário informar um arquivo de entrada (código) como parâmetro.\n\n");
        exit(EXIT_FAILURE);
    }

    initLex(argv[1]);       // Carrega codigo
    lookahead = getToken(); // Inicializacao do lookahead

    // Abre o arquivo de saida
    strcpy(output_file_name, argv[1]);
    strcat(output_file_name, ".asm");
    output_file = fopen(output_file_name, "w+");

    int aux;
    aux = program(); // Chamada da derivacao/funcao inicial da gramatica



    if(aux != true){
        printf("\n Compilacao Mal Sucedida\n");
        return 0;
    }else{
        printf("--------imprecao da TSF-----------");
        printf("\nTK == Token");
        for(int i = 0; i < global_symbol_table_Function.n_variables; i++){
            printf("\nTSF, Linha%d -> %d(tipoTk) %s(nome) %d(parametros) %s(label)", i, global_symbol_table_Function.variable[i].type, global_symbol_table_Function.variable[i].name, global_symbol_table_Function.variable[i].nparams,
            global_symbol_table_Function.variable[i].label);
    }}
    

    type_symbol_table_variables* aux3 = &global_symbol_table_variables;
    int h = 1;
    while (aux3 != NULL)
    {
        int j = 0;
        printf("\n____Tabela %d \n", h);
        while (j < aux3->n_variables)
        {
            /* code */
            printf(" %s   %s \n", aux3->variable[j].name, global_symbol_table_Function.variable[2].label);
            j++;
        }
        // printf("\n");
        h++;
        aux3= aux3->next;
    }
    printf("\nCompilacao completa\n");

    fclose(output_file);
    return 1;
}