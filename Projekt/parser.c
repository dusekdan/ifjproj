////////////////////Roman Jaška
////////////////////rozrobené
#include "parser.h"

//tNodePtr globalTS;
tNodePtr localTS;//ukazatel na localnu tabulku
//tNodePtr adresa = 0;
bool searchGlobal = true;//ci ukladam do globalnej tabulky
int  searchType = 0;//rozhoduje medzi V a F v nt type

char *remember=NULL;//Zapamatanie nazvu funkcie aby sa jej neskor mohol dat typ
bool shouldirecall=false;//Ci uz sa ma pouzit remember
//char *latestfunction=NULL;
int fwddeclarations=0;//pocet fwd
bool nextmustbebody=false;//ci uz nemoze byt fwd
bool debug=false;
char * tempremem=NULL;

int buildemin()
{
    //tNodePtr newnode;
    tData    newsymbol = malloc(sizeof(struct tData));
    if (newsymbol != NULL)
    {
        newsymbol->name = "length";
        newsymbol->type = 5;//funkcia integrova
        newsymbol->argCount = 1;
        insertSymbol(&rootTS, "Flength", newsymbol);
    }else return 1;
    newsymbol=NULL;
             //newnode;
             newsymbol = malloc(sizeof(struct tData));
    if (newsymbol != NULL)
    {
        newsymbol->name = "copy";
        newsymbol->type = 7;//funkcia stringova
        newsymbol->argCount = 3;
        insertSymbol(&rootTS, "Fcopy", newsymbol);
    }else return 1;
    newsymbol=NULL;
             //newnode;
             newsymbol = malloc(sizeof(struct tData));
    if (newsymbol != NULL)
    {
        newsymbol->name = "find";
        newsymbol->type = 5;//funkcia intefgrova
        newsymbol->argCount = 2;
        insertSymbol(&rootTS, "Ffind", newsymbol);
    }else return 1;
    newsymbol=NULL;
             //newnode;
             newsymbol = malloc(sizeof(struct tData));
    if (newsymbol != NULL)
    {
        newsymbol->name = "sort";
        newsymbol->type = 7;//funkcia stringova
        newsymbol->argCount = 1;
        insertSymbol(&rootTS, "Fsort", newsymbol);
    }else return 1;
    return 0;
}

void gib_tok (token tok)
{
    int i=0;
    char *m;    
    scanf("%d",&i);
    if (i==t_var_id || i == t_fun_id)
    {
        scanf("%ms",&m);
        tok->val_str=m;
    }
    tok->type=i;
}

void terminalis (int terminal, token tok)
{
    switch (terminal)
    {
        case 1: printf("var\n");break;
        case 2: printf(": ");break;
        case 3: printf(";\n");break;
        case 4: printf("( ");break;
        case 5: printf(") ");break;
        case 6: printf("function ");break;
        case 7: printf("forward ");break;
        case 8: printf("begin\n");break;
        case 9: printf("end\n");break;
        case 10: printf(".\n");break;
        case 11: printf(", ");break;
        case 12: printf(":= ");break;
        case 13: printf("if ");break;
        case 14: printf("then\n");break;
        case 15: printf("else\n");break;
        case 16: printf("while ");break;
        case 17: printf("do ");break;
        case 18: printf("readln ");break;
        case 19: printf("write ");break;
        case 20: printf("%s ",tok->val_str);break;
        case 21: printf("expr ");break;
        case 22: printf("%s ",tok->val_str);break;
        case 23: printf("term ");break;
        //case 24: printf("param ");break;
        //case 25: printf("read_id ");break;
        case 26: printf("integer ");break;
        case 27: printf("real ");break;
        case 28: printf("string ");break;
        case 29: printf("boolean ");break;
        case 30: printf("$\n");break;
        default: printf("error input=%d\n", terminal);
    }
}

void match (token tok, int terminal)
{
    if (terminal!=30)
    {
        gib_tok(tok);
        terminalis(terminal, tok);
    }
    else printf("done\n");
}


void nt_program (token tok)
{
    gib_tok (tok);
    /////////////////////////////////////////////////////////////////////////////RULE 1
    if (tok->type == t_var || tok->type == t_function || tok->type == t_begin)
    {   
        if (tok->type == t_var)
        {
            nt_var_def_block (tok);
        }
        if (tok->type == t_function)
        {
            nt_fun_def_list (tok);
        }
        if (tok->type == t_begin)
        {
            nt_main (tok);
            if (fwddeclarations!=0) printf("nebola dodelkarovana fwd funkcia\n");
        }
    }
    else
        printf("syntax error in nt_program\n");
}

void nt_var_def_block (token tok)
{
    if (tok->type == t_var || tok->type == t_function || tok->type == t_begin)
    {   //printf("zacina vardefblock\n");
        ////////////////////////////////////////////////////////////////////////RULE 2
        if (tok->type == t_var)
        {
            match(tok, t_var);
            //printf("ide vardef\n");
            nt_var_def(tok);
            //printf("vardef von\n");
            nt_var_def_list(tok);
            //printf("╔═══════════════════╗\n║vardefblock skoncil║\n╚═══════════════════╝\n");
        }
        //////////////////////////////////////////////////////////////////////////RULE3
        else
            return;
    }
    else
        printf("syntax error in nt_var_def_block\n");
}

void nt_var_def_list (token tok)
{
    if (tok->type == t_function || tok->type == t_begin || tok->type == t_var_id)
    {

        /////////////////////////////////////////////////////////////////////////RULE4
        if (tok->type == t_var_id)
        {
            //printf("idem vardef kamko a inak na adrese  \n");

            nt_var_def(tok);
            //printf("tak ja idem aj list\n");
            nt_var_def_list(tok);
        }
        /////////////////////////////////////////////////////////////////////////RULE5
        else
            return;
    }
    else
        printf("syntax error in nt_var_def_list\n");
}

void nt_var_def (token tok)
{
    ///////////////////////////////////////////////////////////////////////////RULE6
    
    if (tok->type == t_var_id)
    {
        ////////////////////////////////////////////SPRACOVANIE PREMENNEJ

        char * key = malloc(sizeof(char)*(strlen(tok->val_str)+1));
        memset(key, 0, strlen(key));
        strcat (key, "V");
        strcat (key, tok->val_str);
        //printf("searchglobal is %d\n",searchGlobal );
        if (searchGlobal==true)
            if (searchSymbol(&rootTS, key)==0)
            {
                tData newsymbol = malloc(sizeof(struct tData));
                if (newsymbol != NULL)
                {
                    newsymbol->name = tok->val_str;
                    newsymbol->type = t_var_id;
                    insertSymbol(&rootTS, key, newsymbol);//printf("╔═══════════════════╗\n║▼ulozene do rootTS▼║\n╚═══════════════════╝\n");
                    match (tok, t_var_id);
                }
                else
                    printf("dojebal sa malloc\n");
            }
            else
                printf("premenna uz existuje\n");
        else
            if (searchSymbol(&localTS, key)==0)
            {
                tData newsymbol = malloc(sizeof(struct tData));
                if (newsymbol != NULL)
                {
                    newsymbol->name = tok->val_str;
                    newsymbol->type = t_var_id;
                    insertSymbol(&localTS, key, newsymbol);//printf("╔════════════════════╗\n║▼ulozene do localTS▼║\n╚════════════════════╝\n");
                    match (tok, t_var_id);
                }
                else
                    printf("dojebal sa malloc\n");
            }
            else
                printf("premenna uz existuje\n");
        ///////////////////////////////////////////////Premenna ulozena
        match (tok, t_colon);
        searchType=1;
        nt_type (tok);
        searchType=0;
        match (tok, t_semicolon);
        free(key);

    }
    else
        printf("syntax error in nt_var_def\n");

}

void nt_fun_def_list (token tok)
{
    if (tok->type == t_function || tok->type == t_begin)
    {
        //////////////////////////////////////////////////////////////////RULE7
        if (tok->type == t_function)
        {

            match(tok,t_function);

            char * key = malloc(sizeof(char)*(strlen(tok->val_str)+1));
            memset(key, 0, strlen(key));
            strcat (key, "F");
            strcat (key, tok->val_str);
            //printf("\n╔═══════════════╗\n║Hladany key je:║%s\n╚═══════════════╝\n",key);
            

            ////////////////////////////////////////////SPRACOVANIE FID
            //Kontrola či funkcia už je v tabulke alebo nie
            //funkcie sa vydy hladaju v rootTS
            tNodePtr hledam  = searchSymbol(&rootTS, key);//printf("%d\n",&hledam );
            if (hledam == 0)
            //////////////////////////////////////////////IDEME UKLADAT
            {
                remember=tok->val_str;//odpanatanie nayvu funkcie aby sme jej vedeli neskor dat typ
                tData    newsymbol = malloc(sizeof(struct tData));
                if (newsymbol != NULL)
                {
                    newsymbol->name = tok->val_str;
                    newsymbol->type = t_fun_id;
                    insertSymbol(&rootTS, key, newsymbol);//printf("\n╔═══════════════════╗\n║▼ulozene do rootTS▼║\n╚═══════════════════╝\n");
                    hledam  = searchSymbol(&rootTS, key);//do hledam dam novu
                                    //teraz treba este vytvorit rovnomennu lokalnu premennu;
                    //bool temp=searchGlobal;
                


                tData    newsymbol1 = malloc(sizeof(struct tData));
                if (newsymbol1 != NULL)
                {
                    newsymbol1->name = tok->val_str;
                    newsymbol1->type = t_var_id;
                    char * key1 = malloc(sizeof(char)*(strlen(tok->val_str)+1));
                    memset(key1, 0, strlen(key1));
                    strcat (key1, "V");
                    strcat (key1, tok->val_str);
                    /*printf("\nimplicitna premenna %s ok\n",*/insertSymbol(&localTS, key1, newsymbol1)/*->key)*/;
                }






                    match(tok,t_fun_id);
                }
                else printf("dojebal sa malloc\n");
            }
            else //funkcia uz bola definovana takze musi mat typ prideleny cize iny ako funid
            {
                if (hledam->data->type >= 9 && hledam->data->type <=12)
                {
                    remember=tok->val_str;
                    //printf("tokvalstr %s\n",remember );
                    //printf("REMEM %s\n",remember );
                    //searchType=4;
                    if (debug==true)printf("\n----------funkcia %s mala fwd a bol to %d\n",hledam->data->name, hledam->data->type);
                    nextmustbebody=true;
                    match(tok,t_fun_id);
                }
            }
            match(tok,t_l_parrent);

            nt_param_list(tok);
            //printf("\n╔═══════════════════╗\n║Parametre nacitane ║\n╚═══════════════════╝\n");

            match(tok,t_r_parrent);
            match(tok,t_colon);
            shouldirecall=true;//skoncili paprametre treba priradit typ funkcii
            //printf("pretype remember%s\n",remember );
            searchType=2;
            nt_type(tok);//teraz ma funkcia priradeny typ 5-8
            searchType=0;//printf("type ok\n");
            match(tok,t_semicolon);

            int fwdbefore=fwddeclarations;//
            tempremem=hledam->key;
            //printf("pojde body\n");
            nt_fun_body(tok);
            if (fwdbefore!=fwddeclarations) //ak sa ymenil pocet fwd tak toto je fwd deklaracia
                {
                    hledam->data->type+=4; //novy stav 9-12
                }
            else (hledam->data->type+=8);

            match(tok,t_semicolon);

            nt_fun_def_list(tok);
        }
        //////////////////////////////////////////////////////////////////////RULE8
        else
            return;
    }
    else
        printf("synerror in nt_fun_def_list\n");
}


void nt_fun_body (token tok)
{
    if (tok->type == t_forward || tok->type == t_begin || tok->type == t_var)
    {
        //////////////////////////////////////////////////////////////////RULE9 +++treba zaistit ze neskor bude definovana
        if (tok->type == t_forward && nextmustbebody==false)//forward a zaroven nebol fwd
        {
            fwddeclarations++;//zvysil sa pocet funkcii cakajucich na fwd
            //printf("found forward\n");
            match(tok,t_forward);
            //printf("nasiel som fwd\n");
        }
        //////////////////////////////////////////////////////////////////////RULE10
        else
        {   




            tNodePtr hledam = searchSymbol(&rootTS, tempremem);
            if (nextmustbebody==true)
                hledam->data->type+=4;//hotovokonec 13-16
            if (debug==true)printf("type je %d\n",hledam->data->type );
            tempremem=NULL;


            if (nextmustbebody==true)//fwd funkcia bola dodelkarovana
            {
                fwddeclarations--;
                nextmustbebody=false;
            }
            searchGlobal = false;
            nt_var_def_block (tok);
            //searchGlobal = true;
            

            nt_body (tok);
            disposeTable(&localTS);
            //printf("skoncilo funbody\n");
            
        }
    }
    else
        printf("synerror in nt_fun_def_list\n");
}

void nt_body (token tok)
{
    ////////////////////////////////////////////////////////////////////////////RULE11
    if (tok->type == t_begin)
    {

        match(tok,t_begin);
        nt_stmt_list(tok);
        match(tok,t_end);
    }
    else printf("syn error in nt_body\n");
}

void nt_main (token tok)
{
    ///////////////////////////////////////////////////////////////////////////////RULE12
    if (tok->type == t_begin)
    {
        nt_body(tok);
        match(tok,t_period);
        match(tok,t_dollar);
    }
    else printf("syn error in nt_main\n");
}

void nt_stmt_list (token tok)
{
    if (tok->type == t_begin || tok->type == t_end    || tok->type == t_if    ||
        tok->type == t_while || tok->type == t_readln || tok->type == t_write ||
        tok->type == t_var_id)
    {
        ////////////////////////////////////////////////////////////////////////////RULE14
        if (tok->type == t_end)
        {
            return;
        }
        ////////////////////////////////////////////////////////////////////////////RULE13
        else
        {
            nt_stmt(tok);
            nt_stmt_more(tok);
        }

    }
    else
        printf("syn error in nt_stmt_list\n");
}

void nt_stmt_more (token tok)
{
    if (tok->type == t_semicolon || tok->type == t_end)
    {
        //////////////////////////////////////////////////////////////////////////////////RULE15
        if (tok->type == t_semicolon)
        {
            match(tok,t_semicolon);
            nt_stmt_list(tok);
        }
        //////////////////////////////////////////////////////////////////////////////////RULE16
        else
            return;
    }
}

void nt_stmt (token tok)
{
    if (tok->type == t_begin  || tok->type == t_var_id || tok->type == t_if    ||
        tok->type == t_while  || tok->type == t_readln || tok->type == t_write)
    {
        tNodePtr hledam;char * key = malloc(sizeof(char)*(strlen(tok->val_str)+1));
        switch (tok->type)
        {
            ////////////////////////////////////////////////////////////////////////////////RULE17
            case 8:         nt_body (tok);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE18
            case 20:        //////////////////////////////////////Spracovanie premennej
                            
                            memset(key, 0, strlen(key));
                            strcat (key, "V");
                            strcat (key, tok->val_str);
                            //printf("key je %s\n", key);
                            //printf("searchglobal je %d\n",searchGlobal);

                            if (searchGlobal==true)
                            {
                                //printf("hladam v global\n");
                                hledam=searchSymbol(&rootTS, key);
                            }
                            
                            else
                            {
                                //printf("hladam v local\n");
                                hledam=searchSymbol(&localTS, key);
                                if (hledam==0)
                                    hledam=searchSymbol(&rootTS, key);
                            }

                            if (hledam!=0)
                                if (hledam->data->type==t_var_id || (hledam->data->type >= 1 && hledam->data->type <= 4))
                                {
                                    //printf("ano je to varid\n");
                                    match (tok,t_var_id);
                                }
                                //else if (hledam->data->type==t_fun_id || (hledam->data->type >= 5 && hledam->data->type <= 8) || (hledam->data->type >= 13 && hledam->data->type <= 16))//toto reaguje len na typove premenne )
                                //{
                                 //   match(tok,t_fun_id);
                                //}
                                else
                                {
                                    printf("nedefnovana premenna\n");
                                    printf("type bol %d\n",hledam->data->type );
                                    return;
                                }
                            match     (tok,t_assign);
                            nt_assign (tok);
                            break;                
            ////////////////////////////////////////////////////////////////////////////////RULE19               
            case 13:        match   (tok,t_if);
                            match   (tok,t_expr);
                            match   (tok,t_then);
                            nt_body (tok);
                            match   (tok,t_else);
                            nt_body (tok);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE20
            case 16:        match   (tok,t_while);
                            match   (tok,t_expr);
                            match   (tok,t_do);
                            nt_body (tok);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE21
            case 18:        match (tok,t_readln);
                            match (tok,t_l_parrent);
                            ////////////////////////////////////////////SPRACOVANIE PREMENNEJ
                            //Kontrola či premenná už je v tabulke
                            char * key = malloc(sizeof(char)*(strlen(tok->val_str)+1));
                            memset(key, 0, strlen(key));
                            strcat (key, "V");
                            strcat (key, tok->val_str);
                            if (searchGlobal==false)
                                {
                                    hledam=searchSymbol(&localTS, key);
                                    if (hledam==0)
                                        hledam=searchSymbol(&rootTS, key);
                                }
                            else
                                hledam=searchSymbol(&rootTS, key);

                            if (hledam!=0)
                                
                                if (hledam->data->type==t_var_id || (hledam->data->type >= 1 && hledam->data->type <= 4))//toto reaguje len na typove premenne 
                                {//printf("%d\n", hledam->data->type );
                                    match (tok,t_var_id);
                                }
                                else
                                {
                                    printf("nedefnovana premenna pre radln\n");
                                    return;
                                }                     
                            match (tok,t_r_parrent);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE22
            case 19:        match        (tok,t_write);
                            match        (tok,t_l_parrent);
                            nt_term_list (tok);
                            match        (tok,t_r_parrent);
                            break;
        }
    }    
    else
        printf("syntax error in nt_stmt\n");
}

void nt_assign (token tok)
{
    if (tok->type == t_expr || tok->type == t_fun_id)
    {
        ///////////////////////////////////////////////////////////////////////RULE23
        if (tok->type == t_expr)
        {
            match(tok,t_expr);
        }
        ////////////////////////////////////////////////////////////////////////RULE24
        else
        {
            char * key = malloc(sizeof(char)*(strlen(tok->val_str)+1));
            memset(key, 0, strlen(key));
            strcat (key, "F");
            strcat (key, tok->val_str);
            if (searchSymbol(&rootTS, key)==0)
            {
                printf("error funkcia nieje definovana\n");
            }

            match(tok,t_fun_id);
            match(tok,t_l_parrent);
            nt_term_list(tok);
            match(tok,t_r_parrent);
        }
    }
    else printf("syn error in nt_assign\n");
}

void nt_term_list (token tok)
{
    if (tok->type == t_term || tok->type == t_r_parrent)
    {
        /////////////////////////////////////////////////////////////////////RULE26
        if (tok->type == t_term)
        {
            match(tok,t_term);
            nt_term_more(tok);
        }
        /////////////////////////////////////////////////////////////////////RULE25
        else
            return;
    }
    else
        printf("syn error in nt_term_list\n");
}

void nt_term_more (token tok)
{
    if (tok->type == t_comma || tok->type == t_r_parrent)
    {
        ////////////////////////////////////////////////////////////////////////RULE27
        if (tok->type == t_comma)
        {
            match(tok,t_comma);
            nt_term_list(tok);
        }
        ////////////////////////////////////////////////////////////////////////RULE28
        else
            return;
    }
    else
        printf("syn error in nt_term_more\n");
}

void nt_param (token tok)
{
    if (tok->type == t_var_id)
    {   
        char * key = malloc(sizeof(char)*(strlen(tok->val_str)+1));
        memset(key, 0, strlen(key));
        strcat (key, "P");
        strcat (key, tok->val_str);
        ////////////////////////////////////////////SPRACOVANIE Parametru
        if (nextmustbebody==false)//ak sa idu nove ukladat
        {
            tData    newsymbol = malloc(sizeof(struct tData));
            if (newsymbol != NULL)
            {
                
                newsymbol->name = tok->val_str;
                newsymbol->type = t_var_id;
                insertSymbol(&localTS, key, newsymbol);
                //printf("\n╔════════════════════╗\n║▼ulozene do localTS▼║\n╚════════════════════╝\n");
                //printf("\n--------------------parameter %s bol vytvorena\n", tok->val_str);
                match (tok, t_var_id);

            }
            else printf("dojebal sa malloc\n");
        }
        else // ak sa len kontroluju z minulej fwd hlavicky
        {
            tNodePtr hledam = searchSymbol(&localTS, key);
            if (hledam==0) printf("Error hlavicky sa lisia ty kokot\n");
            match (tok, t_var_id);
        }
        ///////////////////////////////////////////////Premenna ulozena




        match(tok,t_colon);
        searchType=3;
        nt_type(tok);
        searchType=0;
        //printf("type fajn\n");

    }
    else
        printf("synerror in nt_param\n");
}

void nt_param_list (token tok)
{
    //printf("nt_param_list dostal %d\n",tok->type );
    if (tok->type == t_r_parrent || tok->type == t_var_id)
    {
        ////////////////////////////////////////////////////////////////////////RULE29
        if (tok->type == t_r_parrent)
        {
            return;
        }
        ////////////////////////////////////////////////////////////////////////RULE30
        else
        {
            nt_param(tok);
            nt_param_more(tok);
        }
    }
    else
        printf("synerror in nt_param_list\n");
}

void nt_param_more (token tok)
{
    if (tok->type == t_r_parrent || tok->type == t_comma)
    {
        ////////////////////////////////////////////////////////////////////////RULE32
        if (tok->type == t_r_parrent)
        {
            return;
        }
        ////////////////////////////////////////////////////////////////////////RULE31
        else
        {
            match(tok,t_comma);
            nt_param_list(tok);
        }

    }
    else
        printf("synerror in nt_param_more\n");
}

void nt_type (token tok)
{

    if (tok->type == t_integer || tok->type == t_real ||
        tok->type == t_string  || tok->type == t_boolean)
    {
        char * whatamilookingfor;
        if (remember != NULL && shouldirecall==true) ///test ci uz som von z parametrov
            {
                whatamilookingfor=malloc(sizeof(char)*(strlen(remember)));
                whatamilookingfor=remember;
                remember=NULL;
                //printf("\n\nI remember %s\n",whatamilookingfor);
            }
        else
            {
                whatamilookingfor=malloc(sizeof(char)*(strlen(tok->val_str)));
                whatamilookingfor=tok->val_str;
            }
                //printf("WHATAMILOOKINGFOR: %s\n",whatamilookingfor );
                char * key = malloc(sizeof(char)*(strlen(whatamilookingfor))+1);
                memset(key, 0, strlen(key));
                //printf("SEARCHTYPE JE %d\n",searchType );
                switch (searchType)
                {
                    case 1: strcat (key, "V");
                            break;
                    case 2: strcat (key, "F");
                            searchGlobal=true;
                            break;
                    case 3: strcat (key, "P");
                            searchGlobal=false;
                            break;
                    /*case 4: searchGlobal=true;
                            break;*/
                    default:printf("kokot\n");
                            exit(1);
                }
                strcat (key, whatamilookingfor);
                free (whatamilookingfor);
            //printf("som vonku %s\n",key);
        /////////////////////////////////////////////////////Rozlisenie ci idem hladat lokalnu ci globalnu
        //printf(" hladat bude key: %s a searchGlobal je %d\n",key,searchGlobal);
        tNodePtr hledam;//=malloc(sizeof(struct tUzel));

        if (searchGlobal==true)
        {   
            hledam=searchSymbol(&rootTS,  key);
            //printf("hladam v global\n");
        }
        else
        { 
            hledam=searchSymbol(&localTS, key);
        }
        if (hledam==0){printf("nenasiel som\n");exit(1);}
        //printf("NASIEL SOM %s a jeho type je %d\n",hledam->key,hledam->data->type );
        ///////////////////////////1234 su premenne typu i r s b
        
        switch (tok->type)
        {
            /////////////////////////////////////////////////////////////RULE33
            case 26:    match(tok,t_integer);

                        if (nextmustbebody==false)
                        {   
                            if (hledam->data->type==t_var_id)
                                {
                                    hledam->data->type=1;
                                }
                            else
                                if (hledam->data->type==t_fun_id)
                                {   
                                    hledam->data->type=5;
                                    shouldirecall=false;
                                    remember=NULL;
                                }
                        }
                        else //len test
                        {
                                if (hledam->data->type!=1 && hledam->data->type!=9)
                                {

                                    printf("error nesedi integer dostal som %d\n",hledam->data->type);
                                }
                        }
                        break;
            /////////////////////////////////////////////////////////////RULE34
            case 27:    match(tok,t_real);
                        if (nextmustbebody==false)
                        {
                        if (hledam->data->type==t_var_id)
                            hledam->data->type=2;
                        else
                            if (hledam->data->type==t_fun_id)
                                {
                                    hledam->data->type=6;
                                    shouldirecall=false;
                                    remember=NULL;
                                }
                        }
                        else //len test
                        {
                                if (hledam->data->type!=2)
                                {
                                    printf("error nesedi real dostal som %d\n",hledam->data->type);
                                }
                        }
                        break;
            /////////////////////////////////////////////////////////////RULE35
            case 28:    match(tok,t_string);
                        if (nextmustbebody==false)
                        {
                        if (hledam->data->type==t_var_id)
                            hledam->data->type=3;
                        else
                            if (hledam->data->type==t_fun_id)
                                {
                                    hledam->data->type=7;
                                    shouldirecall=false;
                                    remember=NULL;
                                }
                        }
                        else //len test
                        {
                                if (hledam->data->type!=3)
                                {
                                    printf("error nesedi string dostal som %d\n",hledam->data->type);
                                }
                        }
                        break;
            /////////////////////////////////////////////////////////////RULE36
            case 29:    match(tok,t_boolean);
                        if (nextmustbebody==false)
                        {
                        if (hledam->data->type==t_var_id)
                            hledam->data->type=4;
                        else
                            if (hledam->data->type==t_fun_id)
                                {
                                    hledam->data->type=8;
                                    shouldirecall=false;
                                    remember=NULL;
                                }
                        }
                        else //len test
                        {
                                if (hledam->data->type!=4)
                                {
                                    printf("error nesedi bool dostal som %d\n",hledam->data->type);
                                }
                        }
                        break;
        }
        if (debug==true)
            if (nextmustbebody==false)
                printf("\n--objektu %s bol priradeny typ %d \n",hledam->data->name, hledam->data->type );
            else 
                printf("\n-----------typy sedeli\n");
        //printf("typy fajn\n");
    free(key);
    }   
    else
        printf("syntax error in nt_type\n");

}

void startTable () ///////////////////////////////////////////////Funkcia na rozbehanie tabulky symbolov
{
    init(&rootTS);
    /*Vetvenie stromu na globalnu a lokalnu polovicu*/
    init(&localTS);
    printf("╔═══════════════════╗\n║rootTS:     %d║\n║localTS:    %d║\n╚═══════════════════╝\n",&rootTS,&localTS);

    if (buildemin()==0) //////////////////////////////////////////vlozenie vstavanych funkcii
        {
            //  if (debug==true)
   //             printf("BUILTIN FUNCTIONS OK\n");
     //           tNodePtr hledam = searchSymbol(&rootTS, "Fsort");
 //               printf("Hledam ma adresu %s\n", hledam);
        }
    else printf("BUILTIN FUNCTIONS FUCKED UP\n");

}

int main(int argc, char const *argv[])
{
    if (argc!=1 && strcmp(argv[1],"-d")==0)debug=true;
    startTable();
    token tok=malloc(sizeof(struct token));
    if (tok!=0)
    {
        nt_program (tok);
    }
    else
    {
        printf("tokenkokot\n");
        return 1;
    }
    disposeTable(&rootTS);
    return 0;
}