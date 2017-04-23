/**
 * Projeto Teatro
 * Grupo: Natalia Lopes, Nelson William, Larissa Chyevena e Sofia Moraes
 * Algoritmos: Fundamentos e Estruturas de Dados - 2016/2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100 //Limite de 100 sessoes
#define MAXF 50 //Limite de 50 pessoas na fila de espera

#define TOTAL_LUGARES 360

//Uma pessoa da fila de espera
typedef struct
{
    char NomePessoa[40];
    char Telefone[20];
} T_Esperando;

//A fila de espera de uma sala
typedef struct
{
    T_Esperando Item[MAXF];
    int Inicio, Fim, Total;
} T_FilaEspera;

//A arquibancada de uma sala
typedef int Arquibancada[19][21];

//A sala de uma sessao
typedef struct
{
    T_FilaEspera fila;
    int vendas;
    int reservas;
    int disp;
    Arquibancada A;
} T_Sala;

//Uma pessoa da lista de Reserva
typedef struct
{
    int cod;
    char Nome[40];
    char Tel[20];
    int linha, coluna;
    char lado;
} T_Reservante;

//Lista de Reserva
typedef struct
{
    T_Reservante Item[MAX];
    int Prim, Ult;
} T_ListaReserva;

//Sessao
typedef struct
{
    int cod;
    char NomeSessao[40];
    char TipoSessao[20];
    int Data;      	  //aaaaMMdd
    char HoraInicio[6];  //hh:mm
    char HoraFinal[6];   //hh:mm
    T_Sala S;
    T_ListaReserva R;
} T_Sessao;


//Lista de sessoes
typedef struct
{
    T_Sessao Item[MAX];
    int Prim, Ult;
} T_ListaSessoes;

typedef int Ponteiro;

////////////////////////////////////////////////
/////////////// LISTA DE SESSOES ///////////////
////////////////////////////////////////////////

void Criar_Lista_Vazia(T_ListaSessoes *L)
{
    L->Prim = 0;
    L->Ult = 0;

}

void Criar_Fila_Vazia(T_FilaEspera *F)
{
    F->Fim = 0;
    F->Inicio = 0;
    F->Total = 0;
}

int Verifica_Lista_Vazia(T_ListaSessoes L)
{
    return (L.Prim == L.Ult);
}

int Verifica_Lista_Cheia(T_ListaSessoes L)
{
    return (L.Ult == MAX);
}

int Verifica_Fila_Cheia(T_FilaEspera F)
{
    return((F.Fim+1)%MAXF == F.Fim);
}

int Verifica_Fila_Vazia(T_FilaEspera F)
{
    return(F.Inicio == F.Fim);
}

int Verifica_Lugar_Vago(Arquibancada A)
{
    int lugarVago = 0;
    int i, j;
    for (i = 0; i <18 ; i++)
    {
        for (j = 0; j < 20; j++)
        {
            if (A[i][j] == 0)
            {
                lugarVago++;
            }
        }
    }
    return (lugarVago != 0);
}

void InsereLista(T_ListaSessoes *L, T_Sessao X)
{
    int p;

    if (Verifica_Lista_Cheia( *L))
        printf("\n(!) Lista cheia, incapaz de adicionar.\n");
    else
    {
        p = L->Prim;
        while ((p < L->Ult) && (X.cod > L->Item[p].cod))
            p++;
        if (p == L->Ult)
        {
            L->Item[p] = X;
            L->Ult++;
        }
        else if (X.cod != L->Item[p].cod)
        {
            int i;
            for (i = L->Ult; i > p; i--)
                L->Item[i] = L->Item[i - 1];
            L->Item[p] = X;
            L->Ult++;
        }
        else printf("\n(!) Codigo ja existente, incapaz de cadastrar.\n");
    }
}

void InsereListaReserva(T_ListaReserva *L, T_Reservante X)
{
    int p;

    if (L->Ult == MAX)
        printf("\n(!) Lista cheia, incapaz de adicionar.\n");
    else
    {
        p = L->Prim;
        while ((p < L->Ult) && (X.cod > L->Item[p].cod))
            p++;
        if (p == L->Ult)
        {
            L->Item[p] = X;
            L->Ult++;
        }
        else if (X.cod != L->Item[p].cod)
        {
            int i;
            for (i = L->Ult; i > p; i--)
                L->Item[i] = L->Item[i - 1];
            L->Item[p] = X;
            L->Ult++;
        }
        else printf("\n(!) Codigo ja existente, incapaz de cadastrar.\n");
    }
}

void RemoverLista(T_ListaSessoes *L, T_Sessao *X)
{
    int p, i;
    p = L->Prim;
    while ((p < L->Ult) && (X->cod > L->Item[p].cod))
        p++;
    if ((p == L->Ult) || (X->cod != L->Item[p].cod))
        printf("\n(!) Codigo nao encontrado, incapaz de remover.\n");
    else
    {
        *X = L->Item[p];
        for (i = p; i < L->Ult; i++)
            L->Item[i] = L->Item[i + 1];
        L->Ult--;
        printf("\n\t(*) Removida com sucesso!\n");
    }
}

void ZerarArquibancada(Arquibancada a)
{
    int i, j;
    for (i = 0; i < 19; i++)
    {
        for (j = 0; j < 21; j++)
        {
            a[i][j] = 0;
        }
    }
}

///////////////////////////////////////////
/////////////// MANIPULACAO ///////////////
///////////////////////////////////////////

void ExibirPessoa(T_Esperando E, int lugar)
{
    printf("\t\t| NOME: %s\n", E.NomePessoa);
    printf("\t\t| TELEFONE: %s\n", E.Telefone);
    printf("\t\t| POSICAO NA FILA: %d \n", lugar);
    printf("\t\t+-----------------------------+\n");
}

void ExibirReservantes(T_Reservante R)
{
    printf("\t\t| NOME: %d\n", R.cod);
    printf("\t\t| NOME: %s\n", R.Nome);
    printf("\t\t| TELEFONE: %s\n", R.Tel);
    if (R.lado == 'e')
    {
        printf("\t\t| LADO: Esquerdo\n");
        printf("\t\t| PONTRONA: %dx%d\n", R.linha+1, 10-R.coluna);
    }
    else
    {
        printf("\t\t| LADO: Direito\n");
        printf("\t\t| PONTRONA: %dx%d\n", R.linha+1, R.coluna-9);
    }

    printf("\t\t+-----------------------------+\n");
}

void ExibirSessao(T_Sessao X)
{
    int dia, mes, ano;
    dia = X.Data%100;
    mes = (X.Data/100)%100;
    ano	= X.Data/10000;

    printf("\n");
    printf("\t+===============================+\n");
    printf("\t| SESSAO %8d               |\n", X.cod);
    printf("\t+===============================+\n");
    printf("\t| NOME: %s \n", X.NomeSessao);
    printf("\t| TIPO: %s \n", X.TipoSessao);
    printf("\t| DATA: %02d/%02d/%04d \n", dia, mes, ano);
    printf("\t| HORA: %s ate %s\n", X.HoraInicio, X.HoraFinal);
    printf("\t| LUGARES:\n");
    printf("\t|  - Vendidos:    %3d \n", X.S.vendas);
    printf("\t|  - Reservados:  %3d \n", X.S.reservas);
    printf("\t|  - Disponiveis: %3d \n", X.S.disp);
    printf("\t+===============================+\n");
}

void ExibirTeatro (Arquibancada m)
{
    char c[21];

    printf("\n");
    printf("\t+=================================== [PALCO] ===============================+\n");
    printf("\t|                                                                           |\n");
    printf("\t|     10  9  8  7  6  5  4  3  2  1 |       | 1  2  3  4  5  6  7  8  9  10 |\n");
    printf("\t|                                   |       |                               |\n");

    int i, j;
    for (i = 0; i < 18; i++)
    {
        for (j = 0; j < 20; j++)
        {
            if (m[i][j] == 1)
            {
                c[j] = 'X';
            }
            else if (m[i][j] == 2)
            {
                c[j] = '*';
            }
            else
            {
                c[j] = ' ';
            }
        }
        printf("\t| %2d  [%c][%c][%c][%c][%c][%c][%c][%c][%c][%c]|       |[%c][%c][%c][%c][%c][%c][%c][%c][%c][%c] |\n",
               i+1, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9],
               c[10], c[11], c[12], c[13], c[14], c[15], c[16], c[17], c[18], c[19]);
    }
    printf("\t|                                                                           |\n");
    printf("\t+===================+=======================================================+\n");
    printf("\t| LEGENDA           |\n");
    printf("\t|  [X] - Vendido    |\n");
    printf("\t|  [*] - Reservado  |\n");
    printf("\t|  [ ] - Disponivel |\n");
    printf("\t+===================+\n");

}

void ConsultarSessao(T_ListaSessoes L, int cod)
{
    int p;
    if (Verifica_Lista_Vazia(L))
        printf("\n(!) Lista de sessoes vazia, incapaz de exibir.\n");
    else
    {
        p = L.Prim;
        while ((p < L.Ult) && (cod > L.Item[p].cod))
            p++;
        if ((p == L.Ult) || (cod != L.Item[p].cod))
            printf("\n(!) Sessao nao encontrada, incapaz de exibir.\n ");
        else
        {
            ExibirSessao(L.Item[p]);
            ExibirTeatro(L.Item[p].S.A);
        }
    }
}

//Exibe todas as sesoes da lista
void Exibir_Lista(T_ListaSessoes L)
{
    int p;
    if (Verifica_Lista_Vazia(L))
        printf("\n(!) Lista de sessoes vazia, incapaz de exibir.\n");
    else
    {
        p = L.Prim;
        while (p < L.Ult)
        {
            ExibirSessao(L.Item[p]);
            p++;
        }
    }
}

//Exibe todas as pessoas que reservaram lugares
void Exibir_Lista_Reserva(T_ListaReserva L)
{
    int p;
    if (L.Prim == L.Ult)
        printf("\n(!) Lista de vazia, incapaz de exibir.\n");
    else
    {
        p = L.Prim;
        while (p < L.Ult)
        {
            ExibirReservantes(L.Item[p]);
            p++;
        }
    }
}

void LerDadosReservante(T_Reservante *P, int linha, int coluna, char lado)
{
    printf("\n\t\tInsira os dados da pessoa que realizou a reserva.\n");
    printf("\n\t\tCODIGO: ");
    scanf("%d", &P->cod);
    printf("\n\t\tNOME: ");
    scanf(" %[^\n]s", P->Nome);
    printf("\n\t\tTELEFONE: ");
    scanf(" %[^\n]s", P->Tel);
    P->linha = linha;
    P->coluna = coluna;
    P->lado = lado;
}

void LerDadosPessoa(T_Esperando *E)
{
    printf("\n\t\tInsira os dados da pessoa a esperar.\n");
    printf("\n\t\tNOME: ");
    scanf(" %[^\n]s", E->NomePessoa);
    printf("\t\tTELEFONE: ");
    scanf(" %[^\n]s", E->Telefone);
}

int DescobrePosicao(T_ListaSessoes L, int cod)
{
    int p;
    if (Verifica_Lista_Vazia(L))
    {
        //printf("\n(!) Lista de sessoes vazia!\n");
        return p = -1;
    }
    else
    {
        p = L.Prim;
        while ((p < L.Ult) && (cod > L.Item[p].cod))
        {
            p++;
        }
        if ((p == L.Ult) || (cod != L.Item[p].cod))
        {
            //printf("\nElemento nao encontrado na lista\n ");
            return p = -1;
        }
        else
        {
            return p;
        }
    }
}

void AtualizarSessao(T_Sessao *X, int vendas, int reservas)
{
    X->S.vendas += vendas;
    X->S.reservas += reservas;
    X->S.disp -= vendas+reservas;
}

void ReservarVenderEsquerdo(T_Sessao *X, int tipo, T_Reservante *P)
{
    int linha, coluna;
    int dados = 0;
    do
    {
        printf("\t\tInsira os dados do lugar a vender.\n");
        printf("\t\tLINHA (1~18): ");
        scanf("%d", &linha);
        if (linha < 1 || linha > 18)
        {
            printf("\t\t(!) Linha invalida, tente novamente.\n\n");
        }
        else
        {
            printf("\t\tCOLUNA (1~10): ");
            scanf("%d", &coluna);
            if (coluna < 1 || coluna > 10)
            {
                printf("\t\t(!) Coluna invalida, tente novamente.\n\n");
            }
            else
            {
                if (X->S.A[linha-1][10-coluna] == 0)
                {
                    if (tipo == 2)
                    {
                        printf("\n\tInsira os dados da pessoa que esta reservando esse lugar:\n");
                        int a = linha-1;
                        int b = 10-coluna;
                        LerDadosReservante(P, a, b, 'e');
                        InsereListaReserva(&X->R, *P);
                        X->S.A[linha-1][10-coluna] = tipo;
                        dados = 1;
                        printf("\n\tLugar Reservado com Sucesso!\n");
                    }
                    else
                    {
                        X->S.A[linha-1][10-coluna] = tipo;
                        dados = 1;
                        printf("\n\tLugar Vendido com sucesso!\n");
                    }
                }
                else
                {
                    printf("\t\t(!) Esse lugar ja esta ocupado, tente novamente.\n\n");
                }
            }
        }
    }
    while (dados != 1);
}

void ReservarVenderDireito(T_Sessao *X, int tipo, T_Reservante *P)
{
    int linha, coluna;
    int dados = 0;
    do
    {
        printf("\t\tInsira os dados do lugar a vender.\n");
        printf("\t\tLINHA (1~18): ");

        scanf("%d", &linha);
        if (linha < 1 || linha > 18)
        {
            printf("\t\t(!) Linha invalida, tente novamente.\n\n");
        }
        else
        {
            printf("\t\tCOLUNA (1~10): ");
            scanf("%d", &coluna);
            if (coluna < 1 || coluna > 10)
            {
                printf("\t\t(!) Coluna invalida, tente novamente.\n\n");
            }
            else
            {
                if (X->S.A[linha-1][coluna+9] == 0)
                {
                    if (tipo == 2)
                    {
                        printf("\n\tInsira os dados da pessoa que esta reservando esse lugar:\n");
                        int a = linha-1;
                        int b = coluna+9;
                        LerDadosReservante(P, a, b, 'd');
                        InsereListaReserva(&X->R, *P);

                        X->S.A[linha-1][coluna+9] = tipo;
                        dados = 1;
                    }
                    else
                    {
                        X->S.A[linha-1][coluna+9] = tipo;
                        dados = 1;
                    }
                }
                else
                {
                    printf("\t\t(!) Esse lugar ja esta ocupado, tente novamente.\n\n");
                }
            }
        }
    }
    while (dados != 1);
}

void RemoverAntigas(T_ListaSessoes *L)
{
    int p;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int diaAtual = tm.tm_mday;
    int mesAtual = tm.tm_mon + 1;
    int anoAtual = tm.tm_year + 1900;

    long dataAtual = (anoAtual * 10000) + (mesAtual * 100) + (diaAtual);

    if (Verifica_Lista_Vazia(*L))
    {
        printf("\n(!) Lista de sessoes vazia, incapaz de remover.\n");
    }
    else
    {
        p = L->Prim;
        while (p < L->Ult)
        {
            if (L->Item[p].Data < dataAtual)
            {
                printf("\n\tRemovendo sessao %d (com a data %d)...", L->Item[p].cod, L->Item[p].Data);
                RemoverLista(L, &L->Item[p]);
                p = L->Prim;
            }
            else
            {
                p++;
            }
        }
    }
}

void Enfileirar (Arquibancada A, T_FilaEspera *F, T_Esperando X)
{
    if (Verifica_Lugar_Vago(A))
    {
        printf("\n\t\t(!) A sessao possui lugares vagos, incapaz de criar fila de espera.\n");
    }
    else
    {
        if(Verifica_Fila_Cheia(*F))
            printf("\n(!) Fila de espera cheia, incapaz de adicionar.\n");
        else
        {
            F->Item[F->Fim] = X;
            F->Fim = (F->Fim + 1)% MAXF;
            F->Total++;
        }
    }
}

void Desenfileirar(T_FilaEspera *F, T_Esperando *X)
{
    if(Verifica_Fila_Vazia(*F))
        printf("\n(!) Incapaz de desenfileirar uma fila vazia.\n ");
    else
    {
        *X = F->Item[F->Inicio];
        F->Inicio = (F->Inicio + 1)%MAXF;
        F->Total--;
    }
}

void RemoveReserva(T_ListaReserva *L, int linha, int coluna)
{ int p, i;
		p = L->Prim;
		    while ((linha != L->Item[p].linha) && (coluna != L->Item[p].coluna)) {
		      p++;
			}
				printf("\n\tAntiga reserva cod: %d removida\n", L->Item[p].cod);
				for (i=p; i<L->Ult; i++)
				L->Item[i]=L->Item[i+1];
				L->Ult--;
}

void DesocuparDireito(T_Sessao *X)
{
    int linha, coluna;
    int dados = 0;

    if (Verifica_Lugar_Vago(X->S.A))
    {
        printf("\n\t(!) Sessao nao cheia, incapaz de desocupar.\n");
    }
    else
    {
        do
        {
            printf("\t\tInsira os dados do lugar a desocupar.\n");
            printf("\t\tLINHA (1~18): ");
            scanf("%d", &linha);
            if (linha < 1 || linha > 18)
            {
                printf("\t\t(!) Linha invalida, tente novamente.\n\n");
            }
            else
            {
                printf("\t\tCOLUNA (1~10): ");
                scanf("%d", &coluna);
                if (coluna < 1 || coluna > 10)
                {
                    printf("\t\t(!) Coluna invalida, tente novamente.\n\n");
                }
                else
                {
                    if (X->S.A[linha-1][coluna-9] == 1)
                    {
                        AtualizarSessao(X, -1, 0);
                    }
                    else
                    {
                        int a = linha-1, b = coluna-9;
                        RemoveReserva(&X->R, a, b);
                        AtualizarSessao(X, 0, -1);
                    }

                    X->S.A[linha-1][coluna-9] = 1;

                    printf("\n\t(*) Lugar vendido para o cliente (da fila de espera): %s\n", X->S.fila.Item[X->S.fila.Inicio].NomePessoa);
                    AtualizarSessao(X, 1, 0);

                    Desenfileirar(&X->S.fila, X->S.fila.Item);

                    dados = 1;
                }
            }
        }
        while (dados != 1);
    }
}

void DesocuparEsquerdo(T_Sessao *X)
{
    int linha, coluna;
    int dados = 0;

    if (Verifica_Lugar_Vago(X->S.A))
    {
        printf("\n\t(!) Sessao nao cheia, incapaz de desocupar.\n");
    }
    else
    {
        do
        {
            printf("\t\tInsira os dados do lugar a desocupar.\n");
            printf("\t\tLINHA (1~18): ");
            scanf("%d", &linha);
            if (linha < 1 || linha > 18)
            {
                printf("\t\t(!) Linha invalida, tente novamente.\n\n");
            }
            else
            {
                printf("\t\tCOLUNA (1~10): ");
                scanf("%d", &coluna);
                if (coluna < 1 || coluna > 10)
                {
                    printf("\t\t(!) Coluna invalida, tente novamente.\n\n");
                }
                else
                {
                    if (X->S.A[linha-1][10-coluna] == 1)
                    {
                        AtualizarSessao(X, -1, 0);
                    }
                    else
                    {
                        int a = linha-1, b = 10-coluna;
                        RemoveReserva(&X->R, a, b);
                        AtualizarSessao(X, 0, -1);
                    }

                    X->S.A[linha-1][10-coluna] = 1;


                    printf("\n\t(*) Lugar vendido para o cliente (da fila de espera): %s\n", X->S.fila.Item[X->S.fila.Inicio].NomePessoa);
                    AtualizarSessao(X, 1, 0);

                    Desenfileirar(&X->S.fila, X->S.fila.Item);

                    dados = 1;
                }
            }
        }
        while (dados != 1);
    }
}

int verificaDataPossivel(int data)
{

    //Separação da data que vier do main
    int dia = 0, mes = 0, ano = 0;
    ano = data/10000;
    mes = (data%10000)/100;
    dia = (data%10000)%100;

    //Pega data atual do computador----------------------------------------------------//
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int diaAtual = tm.tm_mday;
    int mesAtual = tm.tm_mon + 1;
    int anoAtual = tm.tm_year + 1900;

	//verifica entrada valida
	if (dia < 0 || dia > 31) {
		return 0;
	}
	if (mes < 0 || mes > 12) {
		return 0;
	}

    //Parte de verificação de data
    if(ano < anoAtual)
    {
        return 0;
    }
    else if(ano > anoAtual)
    {
        return 1;
    }
    else
    {
        if(mes < mesAtual)
        {
            return 0;
        }
        else if(mes > mesAtual)
        {
            return 1;
        }
        else
        {
            if(dia < diaAtual)
            {
                return 0;
            }
            else if(dia >= diaAtual)
            {
                return 1;
            }
        }
    }
    return 0;
}

int verificaHoraPossivel(int data, char horario[6])
{
    //Separação da data que vier do main
    int dia = 0, mes = 0, ano = 0;
    ano = data/10000;
    mes = (data%10000)/100;
    dia = (data%10000)%100;

    //Pega data atual do computador----------------------------------------------------//
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int diaAtual = tm.tm_mday;
    int mesAtual = tm.tm_mon + 1;
    int anoAtual = tm.tm_year + 1900;

    if((dia == diaAtual) && (mes == mesAtual) && (ano == anoAtual))
    {
        //Separação da hora que vier do main
        int hora = 0, minuto = 0;

        //Necessário diminuir os caracteres numericos por conta da formatação da tabela asci
        hora = ((horario[0] - 48) * 10) + (horario[1] - 48);
        minuto = ((horario[3] - 48) * 10) + (horario[4] - 48);
        //------------------------------------------------------------------------------------------//

        //Pega data e hora atuais do computador----------------------------------------------------//
        int horaAtual = tm.tm_hour;
        int minutoAtual = tm.tm_min;

        //Parte de verificação de hora possível
        if (hora< 0 || hora>24) {
			return 0;
		}
        if(hora < horaAtual)
        {
            return 0;
        }
        else if(hora > horaAtual)
        {
            return 1;
        }
        else
        {
            if(minuto <= minutoAtual)
            {
                return 0;
            }
            else if(minuto > minutoAtual)
            {
                return 1;
            }
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

int horarioAntesDe(int hora1, int minuto1, int hora2, int minuto2)
{
    if (hora1 < hora2)
    {
        return 1;
    }
    else if (hora1 > hora2)
    {
        return 0;
    }
    else
    {
        if (minuto1 < minuto2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int horarioDepoisDe(int hora1, int minuto1, int hora2, int minuto2)
{
    if (hora1 > hora2)
    {
        return 1;
    }
    else if (hora1 < hora2)
    {
        return 0;
    }
    else
    {
        if (minuto1 > minuto2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int VerificaHorarioDisponivel(int data, char horarioInicio[6], char horarioFinal[6], T_ListaSessoes L)
{
    int dia = 0, mes = 0, ano = 0;
    ano = data/10000;
    mes = (data%10000)/100;
    dia = (data%10000)%100;
    int horaInicio, minutoInicio;
    horaInicio = ((horarioInicio[0] - 48) * 10) + (horarioInicio[1] - 48);
    minutoInicio = ((horarioInicio[3] - 48) * 10) + (horarioInicio[4] - 48);


    int p;
    if (L.Prim == L.Ult)
        return 1;

    else
    {
        p = L.Prim;
        while (p < L.Ult)
        {
            int diaSessao = 0, mesSessao = 0, anoSessao = 0;
            anoSessao = L.Item[p].Data/10000;
            mesSessao = (L.Item[p].Data%10000)/100;
            diaSessao = (L.Item[p].Data%10000)%100;
            int horaInicioSessao = ((L.Item[p].HoraInicio[0] - 48) * 10) + (L.Item[p].HoraInicio[1] - 48);
            int minutoInicioSessao = ((L.Item[p].HoraInicio[3] - 48) * 10) + (L.Item[p].HoraInicio[4] - 48);
            int horaFimSessao = ((L.Item[p].HoraFinal[0] - 48) * 10) + (L.Item[p].HoraFinal[1] - 48);
            int minutoFimSessao = ((L.Item[p].HoraFinal[3] - 48) * 10) + (L.Item[p].HoraFinal[4] - 48);

            if (dia == diaSessao && ano == anoSessao && mes == mesSessao)
            {
                if (horarioAntesDe(horaInicio, minutoInicio, horaInicioSessao, minutoInicioSessao) && horarioDepoisDe(horaInicio, minutoInicio, horaFimSessao, minutoFimSessao))
                {
                    p++;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                p++;
            }
        }
    }

    return 1;
}

void LerDadosSessao(T_Sessao *X, T_ListaSessoes L)
{
    printf("\n");
    printf("\t+=====================================================+\n");
    printf("\t| CRIAR NOVA SESSAO                                   |\n");
    printf("\t+=====================================================+\n");
    printf("\t| Insira os dados da nova sessao.\n");
    printf("\t|\n");
    printf("\t| CODIGO: ");
    scanf("%d", &X->cod);
    getchar();

    printf("\t| NOME: ");
    scanf(" %[^\n]s",   X->NomeSessao);
    /*
      fgets(X->NomeSessao, sizeof(X->NomeSessao), stdin);
      char *pos;
      if ((pos=strchr(X->NomeSessao, '\n')) != NULL)
          *pos = '\0';
      //scanf("%[^\n]s", X->NomeSessao);
      //getchar();
    */
    printf("\t| TIPO: ");
    scanf(" %[^\n]s", X->TipoSessao);
    /*
    gets(X->TipoSessao, sizeof(X->TipoSessao), stdin);
    if ((pos=strchr(X->TipoSessao, '\n')) != NULL)
        *pos = '\0';
    //scanf("%[^\n]s", X->TipoSessao);
    //getchar();
    */
    //Inserção e verificação da data.
    do
    {
        printf("\t| DATA (no formato \"aaaaMMdd\"): ");
        scanf("%d", &X->Data);

        while(verificaDataPossivel(X->Data) == 0)
        {
            printf("\n\t(!) Data inserida invalida ... Tente Novamente.\n");
            printf("\t| DATA (no formato \"aaaaMMdd\"): ");
            scanf("%d", &X->Data);
        }

        //Inserção e verificação do horario inicial.
        printf("\t| HORARIO DE INICIO (no formato \"hh:mm\"): ");
        scanf("%s", X->HoraInicio);

        while(verificaHoraPossivel(X->Data, X->HoraInicio) == 0)
        {
            printf("\n\t(!) Horario fornecido Invalido e/ou menor que o atual... Tente Novamente.\n");
            printf("\t| HORARIO DE INICIO (no formato \"hh:mm\"): ");
            scanf("%s", X->HoraInicio);
        }

        //Inserção e verificação do horario final.
        printf("\t| HORARIO DE TERMINO (no formato \"hh:mm\"): ");
        scanf("%s", X->HoraFinal);

        while(verificaHoraPossivel(X->Data, X->HoraFinal) == 0)
        {
            printf("\n\t(!) Horario fornecido Invalido e/ou menor que o atual... Tente Novamente.\n");
            printf("\t| HORARIO DE TERMINO (no formato \"hh:mm\"): ");
            scanf("%s", X->HoraFinal);
        }

        if (VerificaHorarioDisponivel(X->Data, X->HoraInicio, X->HoraFinal, L) == 0)
        {
            printf("\n\t(!) Ja existe sessao neste horario/dia, incapaz de criar nova... Tente novamente.\n");
        }
    }
    while (VerificaHorarioDisponivel(X->Data, X->HoraInicio, X->HoraFinal, L) == 0);

    printf("\t+-----------------------------------------------------+\n");

    X->S.vendas = 0;
    X->S.reservas = 0;
    X->S.disp = TOTAL_LUGARES;
    Criar_Fila_Vazia(&X->S.fila);
    ZerarArquibancada(X->S.A);
}

void menuCliente(T_Sessao sessao)
{
    printf("\n");
    printf("\t+=========================================+\n");
    printf("\t| MENU SESSAO\n");
    printf("\t| SESSAO %d (%s)\n", sessao.cod, sessao.NomeSessao);
    printf("\t+=========================================+\n");
    printf("\t| 1 - Compra/reserva no lado esquerdo     |\n");
    printf("\t| 2 - Compra/reserva no lado direito      |\n");
    printf("\t| 3 - Adicionar uma desistencia           |\n");
    printf("\t| 4 - Adicionar na fila de espera         |\n");
    printf("\t| 5 - Exibir a fila de espera             |\n");
    printf("\t| 6 - Exibir a lista de reservas          |\n");
    printf("\t| 0 - Voltar                              |\n");
    printf("\t+-----------------------------------------+\n");
    printf("\t| OPCAO: ");
}



void menuTeatro()
{
    printf("\n");
    printf("+===========================================+\n");
    printf("|   MENU TEATRO                             |\n");
    printf("+===========================================+\n");
    printf("| 1 - Criar uma nova sessao                 |\n");
    printf("| 2 - Exibir uma sessao                     |\n");
    printf("| 3 - Exibir todas as sessoes               |\n");
    printf("| 4 - Remover uma sessao                    |\n");
    printf("| 5 - Remover todas as sessoes antigas      |\n");
    printf("| - - - - - - - - - - - - - - - - - - - - - |\n");
    printf("| 6 - Gravar as sessoes em arquivo          |\n");
    printf("| 7 - Carregar as sessoes de arquivo        |\n");
    printf("| - - - - - - - - - - - - - - - - - - - - - |\n");
    printf("| 8 - Acesso ao Menu Sessao                 |\n");
    printf("| - - - - - - - - - - - - - - - - - - - - - |\n");
    printf("| 0 - Sair                                  |\n");
    printf("+===========================================+\n");
    printf("| OPCAO: ");
}


///////////////////////////////////////////////////////////////////////
/////////////////////////// LOGICA PRINCIPAL //////////////////////////
///////////////////////////////////////////////////////////////////////

int main()
{
    int op, tipoLugar, lado, ope;
    int cod, found;

    Ponteiro P; //Para gravar em arquivo.
    FILE *arq;  //Arquivo que sera gravado.

    T_Esperando E;
    T_Reservante Y;
    T_Sessao X;
    T_ListaSessoes L;
    Criar_Lista_Vazia(&L);

    printf("Inicializando...\n");

    do
    {
        //Exibe o menu do teatro
        menuTeatro();
        scanf("%d", &op);
        getchar();
        printf("+-----------+\n");

        switch (op)
        {

        case 1:
            LerDadosSessao(&X, L);
            InsereLista(&L, X);
            break;

        case 2:
            printf("\n");
            printf("\t+====================================+\n");
            printf("\t| EXIBIR SESSAO                      |\n");
            printf("\t+====================================+\n");
            printf("\t| CODIGO da sessao a exibir: ");
            scanf("%d", &X.cod);
            printf("\t+------------------------------------+\n");
            ConsultarSessao(L, X.cod);
            break;

        case 3:
            Exibir_Lista(L);
            break;

        case 4:
            if (Verifica_Lista_Vazia(L))
            {
                printf("\n(!) Lista de sessoes vazia, incapaz de remover.\n");
                break;
            }
            else
            {
                printf("\n");
                printf("\t+====================================+\n");
                printf("\t| REMOVER SESSAO                     |\n");
                printf("\t+====================================+\n");
                printf("\t| CODIGO da sessao a remover: ");
                scanf("%d", &X.cod);
                printf("\t+------------------------------------+\n");
                RemoverLista(&L, &X);
            }
            break;

        case 5:
            if (Verifica_Lista_Vazia(L))
            {
                printf("\n(!) Lista de sessoes vazia, incapaz de remover.\n");
                break;
            }
            else
            {
                printf("\n");
                printf("\t+==================================================+\n");
                printf("\t| REMOVER SESSOES ANTIGAS                          |\n");
                printf("\t+==================================================+\n");
                printf("\t| Serao removidas sessoes anteriores a hoje.\n");
                printf("\t+--------------------------------------------------+\n");
                RemoverAntigas(&L);
            }
            break;

        case 6:
            arq = fopen("sessoesSalvas.dat", "wb");
            if (arq != NULL)
            {
                P = L.Prim;
                while (P < L.Ult)
                {
                    fwrite(&L.Item[P], sizeof(T_Sessao),1,arq);
                    P++;
                }
                fclose(arq);
                printf("\n(*) Arquivo 'sessoesSalvas.dat' salvo com sucesso!\n");
            }
            else
            {
                printf("\n(!) Nao foi possivel salvar o arquivo!\n");
            }
            break;

        case 7:
            arq = fopen("sessoesSalvas.dat", "rb");
            if (arq != NULL)
            {
                Criar_Lista_Vazia(&L);
                X.cod = 0;
                while (!feof(arq))
                {
                    if (X.cod != 0)
                        InsereLista(&L, X);
                    fread( &X, sizeof(T_Sessao), 1, arq);
                }
                fclose(arq);
                printf("\n(*) Arquivo 'sessoesSalvas.dat' carregado com sucesso!\n");
            }
            else
            {
                printf("\n(!) Nao foi possivel abrir o arquivo 'sessoesSalvas.dat'!\n");
            }
            break;

        case 8:
            printf("\n");
            printf("\tInsira o codigo da sessao a acessar.\n");
            printf("\tCODIGO: ");
            scanf("%d", &cod);
            found = DescobrePosicao(L, cod);
            if (found != -1)
            {
                do
                {
                    menuCliente(L.Item[found]);
                    scanf("%d", &ope);
                    printf("\t+-----------+\n");

                    switch (ope)
                    {
                    case 1:
                        if (!Verifica_Lugar_Vago(L.Item[found].S.A))
                        {
                            printf("\n\t(!) Sessao cheia, incapaz de comprar/reservar.\n");
                        }
                        else
                        {
                            printf("\n");
                            printf("\t\t+==========================+\n");
                            printf("\t\t| EFETUAR COMPRA/RESERVAR  |\n");
                            printf("\t\t+==========================+\n");
                            printf("\t\t| 1 - Comprar              |\n");
                            printf("\t\t| 2 - Reservar             |\n");
                            printf("\t\t+--------------------------+\n");
                            printf("\t\t| OPCAO: ");
                            scanf("%d", &tipoLugar);
                            printf("\t\t+-----------+\n\n");

                            switch (tipoLugar)
                            {
                            case 1:
                                ReservarVenderEsquerdo( &L.Item[found], 1, &Y);
                                AtualizarSessao( &L.Item[found], 1, 0);
                                break;

                            case 2:
                                ReservarVenderEsquerdo( & L.Item[found], 2, &Y);
                                AtualizarSessao( &L.Item[found], 0, 1);
                                break;

                            default:
                                printf("\n\t\t(!) Opcao inexistente, tente novamente.\n");
                            }
                        }
                        break;

                    case 2:
                        if (!Verifica_Lugar_Vago(L.Item[found].S.A))
                        {
                            printf("\n\t(!) Sessao cheia, incapaz de comprar/reservar.\n");
                        }
                        else
                        {
                            printf("\n");
                            printf("\t\t+==========================+\n");
                            printf("\t\t| EFETUAR COMPRA/RESERVAR  |\n");
                            printf("\t\t+==========================+\n");
                            printf("\t\t| 1 - Comprar              |\n");
                            printf("\t\t| 2 - Reservar             |\n");
                            printf("\t\t+--------------------------+\n");
                            printf("\t\t| OPCAO: ");
                            scanf("%d", &tipoLugar);
                            printf("\t\t+-----------+\n\n");

                            switch (tipoLugar)
                            {
                            case 1:
                                ReservarVenderDireito( &L.Item[found], 1, &Y);
                                AtualizarSessao( &L.Item[found], 1, 0);
                                break;

                            case 2:
                                ReservarVenderDireito( & L.Item[found], 2, &Y);
                                AtualizarSessao( &L.Item[found], 0, 1);
                                break;

                            default:
                                printf("\n\t\t(!) Opcao inexistente, tente novamente.\n");
                            }
                        }
                        break;

                    case 3:
                        if (Verifica_Fila_Vazia(L.Item[found].S.fila))
                        {
                            printf("\n\t(!) Nao ha ninguem na fila de espera, incapaz de desocupar.\n");
                        }
                        else
                        {
                            printf("\n");
                            printf("\t\t+==========================+\n");
                            printf("\t\t| DESOCUPAR LUGAR          |\n");
                            printf("\t\t+==========================+\n");
                            printf("\t\t| 1 - Do lado esquerdo     |\n");
                            printf("\t\t| 2 - Do lado direito      |\n");
                            printf("\t\t+--------------------------+\n");
                            printf("\t\t| OPCAO: ");
                            scanf("%d", &lado);
                            printf("\t\t+-----------+\n\n");

                            switch (lado)
                            {
                            case 1:
                                DesocuparEsquerdo(&L.Item[found]);
                                break;
                            case 2:
                                DesocuparDireito(&L.Item[found]);
                                break;
                            default:
                                printf("\n\t(!) Opcao inexistente, tente novamente.\n");
                            }
                        }
                        break;

                    case 4:
                        LerDadosPessoa(&E);
                        Enfileirar(L.Item[found].S.A, &L.Item[found].S.fila, E);
                        break;

                    case 5:
                        if (Verifica_Fila_Vazia(L.Item[found].S.fila))
                        {
                            printf("\n\t(!) Fila de espera vazia, incapaz de exibir.\n");
                        }
                        else
                        {
                            int k = L.Item[found].S.fila.Inicio;
                            int x = 1;

                            printf("\n\t\t+-----------------------------+\n");

                            while (k < L.Item[found].S.fila.Fim)
                            {
                                ExibirPessoa(L.Item[found].S.fila.Item[k], x);
                                k++;
                                x++;
                            }

                        }
                        break;
                    case 6:
                        Exibir_Lista_Reserva(L.Item[found].R);
                        break;
                    case 99:
                    	printf("\n\tEssa opcacao deixa:\n");
						printf("\n\t-Todos os lugares como VENDIDO\n");
						printf("\n\t-Lugar 1x10 do lado Esquerdo como RESERVADO\n");
                        L.Item[found].S.disp = 0;
                        L.Item[found].S.vendas = TOTAL_LUGARES;

                        int i, j;
                        for (i = 0; i <18 ; i++)
                        {
                            for (j = 0; j < 20; j++)
                            {
                                L.Item[found].S.A[i][j] = 1;
                            }
                        }
						L.Item[found].S.A[0][0] = 2;
                        break;

                    case 0:
                        break;

                    default:
                        printf("\n\t(!) Opcao invalida, tente novamente.\n");
                    }
                }
                while(ope != 0);

            }
            else
            {
                printf("\n(!) Sessao nao encontrada, incapaz de abrir cliente.\n");
            }
            break;

        case 0:
            printf("\nFinalizando...\n");
            printf("\nObrigado por utilizar nosso software!\n");
            break;
        default:
            printf("\n(!) Opcao invalida, escolha novamente.\n");
            break;
        }
    }
    while (op != 0);
    return 0;
}
