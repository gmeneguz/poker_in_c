// Programa: Go Poker, No Limit Texas Hold'em poker (jogo de cartas)
// Autor: Gianfranco Meneguz
// Data: Dezembro 2010 - Trabalho Linguagem C

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#define CMAX 52 // Numero maximo do rand
#define CMIN 1  // Numero minimo do rand
#define CMESA 9 // Cartas totais em jogo

#define sblind 10 // small blind
#define bblind 20 // big blind

int cartas_exib[CMESA][3]; // 9 cartas 3 caracteres (1� num 2� num 3� simb ASCII naipe)
char naipe_exib[CMESA][8];
// Inicio Funcoes RANDOM!
long seed = 0;

int getRandom(int min, int max) // FUNCAO que gera numero aleatorio
{
  int r;
  if (!seed)
  {
    time(&seed);
    srand((unsigned)seed);
  }
  r = min + rand() % (max - min + 1);
  return r;
}
// Fim

// FUNCOES para analise de jogo
// Verif_flush:
int verif_flush(int *maoo, int *vhighs)
{

  int cont_f, contt, contaa, temp_f; // Auxiliares do for
  int quant_f;                       // Quantidade de cartas no naipe
  int matriz_f[4][8];

  int ttt, ccont, tempt; // Usados na avaliacao (posterior)
  tempt = 0;
  // matriz com 8 num para cada naipe
  // o [0] de cada linha � o contador
  //  do numero de cartas deste naipe
  //  os outros recebem eventuais valores
  for (contt = 0; contt < 4; contt++)
  {
    for (cont_f = 0; cont_f < 8; cont_f++)
      matriz_f[contt][cont_f] = 0; // Zera a Matriz inteira
  }
  for (cont_f = 0; cont_f < 7; cont_f++)
  {
    temp_f = *(maoo + cont_f) / 100;                        // O ponteiro vai adiante conforme o cont_f
    matriz_f[temp_f - 1][0]++;                              // a posic 0 da linha do naipe � incrementada em 1
    quant_f = matriz_f[temp_f - 1][0];                      // quant_f recebe a quantidade acima ja incrementada
    matriz_f[temp_f - 1][quant_f] = *(maoo + cont_f) % 100; // a matriz recebe o valor
                                                            // da carta na pos do q o valor do contador tem(quant_f]
  }
  // Fim da matriz
  // Inicio da avaliacao
  for (ttt = 0; ttt < 4; ttt++)
  {
    if (matriz_f[ttt][0] >= 5) // IMPORTANTE: se existir flush ele
                               // vai entrar nesse if e vai retornar um valor
                               // para o int main aqui dentro mesmo
    {
      int nf; // nf:numerop de cartas no flush
      int aux_f;
      int flush_ordenado[matriz_f[ttt][0]]; // flush ord recebe o tamanho
                                            // exato do num cartas do flush(nf)(5,6 ou 7)
      nf = matriz_f[ttt][0];

      // Esse for aqui orderna TUDO!! hehehe
      for (contaa = nf; contaa > 0; contaa--)
      {
        temp_f = 0;
        for (ccont = nf; ccont > 0; ccont--)
        {
          if (temp_f < matriz_f[ttt][ccont])
          {
            temp_f = matriz_f[ttt][ccont];
            aux_f = ccont;
          }
        }
        flush_ordenado[contaa - 1] = temp_f;
        matriz_f[ttt][aux_f] = 0; // o valor que era o mais alto ali em cima se torna ZERO
        //  printf("\n teste val flush ord n. %d",flush_ordenado[contaa-1]);
        //  getch();                                      // fazendo assim q os valores entrem de forma gradativa no vetor flush_ordenado
      }
      // Fim do Ordenador

      // Inicio Verificador ROYAL STRAIGHT FLUSH (somente de 10 ate As Naipado)
      if ((flush_ordenado[nf - 1] == 13) && (flush_ordenado[nf - 4] == 10) && (flush_ordenado[0] == 1))
      // Complexo: para haver Royal � necessario a pessoa haver a carta 13(K) a carta  10 e As(1)...
      //  e se a pessoa o tiver, eles devem estar exatamente na ordem do vetor como esta no if!!!
      {
        //   printf("Voce fez Royal straight FlushH!!!!Uau!");
        //   getch();
        return 9014;
      }
      else
      {
        ccont = 0;                              // auxiliar
        for (contaa = 1; contaa < nf; contaa++) //-1 devido a comparacao com o prox valor
        {
          if (flush_ordenado[contaa] - 1 == flush_ordenado[contaa - 1])
          {
            ccont++;
            aux_f = flush_ordenado[contaa];
            if (contaa < nf - 1)
              continue;
            else
              ;
          }
          if (ccont < 4)
          {
            if (contaa == nf - 1) // A ultima comparacao � nf-2
            {
              int aux_extra = 0; // Auxiliar extra para vhighs
              int aux_dois = 1;
              if (flush_ordenado[0] == 1) // resolve o problema do As
              {
                temp_f = 14;
                *(vhighs) = 14;
                aux_extra++;
              }
              else
                temp_f = flush_ordenado[nf - 1];

              while (aux_extra < 5)
              {
                *(vhighs + aux_extra) = flush_ordenado[nf - aux_dois];
                aux_extra++;
                aux_dois++;
              }
              return temp_f + 6000; // aqui retornamos o valor do flush
            }
            else         // para poupar trabalho, visto q
              ccont = 0; // havendo flush sem sequencia
          }              // Flush se torna a melhor mao possivel
          else
          {
            return aux_f + 9000;
          }
        } // Fim do for
      }   // Fim do else
    }     // Fim do If
  }       // Fim do FOR

  return 0; // Se a mao nao tiver straight flush e NEM FLUSH o retorno � 0
  // aqui provavelmente vou chamar outra funcao (quadra)
} // FIm da funcao verif_flush

//////////////////////////////////////////////////////////////////////////////////////////////

int verif_quadra(int *maooq, int *vhighs)
{
  int val_organizado[7];       // recebe valores da maooq mod 100
  int val_ordenado[7];         // Recebe os valores da maooq em ordem progressiva (menor p maior. ex:  3,5,5,7,9,10,13)
  int contq1, contq2, contq3;  // auxiliares for
  int temp_q, aux_q;           // auxiliares ordenador
  int iguais_q, dupla, trinca; // auxiliares avaliador
  int dupla_maior, dupla_menor, high_2d, trinca_maior, trinca_menor;
  int cont_s1, aux_s, cont_s2;
  int high_quadra; // Carta high do jogo QUADRA
  int high_full;   // a dupla do Full House

  for (contq1 = 0; contq1 < 7; contq1++) // val org � uma copia da maooq s� q MOD 100.
  {
    if (*(maooq + contq1) % 100 == 1)
      val_organizado[contq1] = 14; // problema As!!!
    else
      val_organizado[contq1] = *(maooq + contq1) % 100;
  }

  for (contq2 = 6; contq2 >= 0; contq2--) // Esse for organiza TUDO!! (ordenador)
  {
    temp_q = 0;

    for (contq3 = 0; contq3 < 7; contq3++)
    {
      if (temp_q < val_organizado[contq3])
      {
        temp_q = val_organizado[contq3];
        aux_q = contq3;
      }
    }
    val_ordenado[contq2] = temp_q;
    val_organizado[aux_q] = 0;
  }
  // Fim do for ordenador
  // avaliador de quadra e etc...

  temp_q = 0;
  iguais_q = 0;
  dupla = 0;
  trinca = 0;
  dupla_menor = 0;
  dupla_maior = 0;
  trinca_maior = 0;
  trinca_menor = 0;

  for (contq1 = 1; contq1 < 7; contq1++)
  {
    if (val_ordenado[contq1] == val_ordenado[contq1 - 1]) // testa se a carta anterior � do mesmo valor
    {                                                     // para quadra, fullh,trinca, 2 dupla e 1 dupla.
      iguais_q++;
      temp_q = val_ordenado[contq1];
      if (contq1 < 6) // Importante pois senao ele nao faz a ultima verificacao
        continue;
    }
    if (iguais_q == 0)
      continue;

    switch (iguais_q)
    {
    case 1:
      dupla++;
      if (dupla_maior)
        dupla_menor = dupla_maior;
      dupla_maior = temp_q; // auxilia no valor do retorno da funcao
      iguais_q = 0;
      break;

    case 2:
      trinca++;
      if (trinca_maior)
        trinca_menor = trinca_maior;
      trinca_maior = temp_q; // auxilia no valor do retorno da funcao
      iguais_q = 0;
      break;

    case 3: // printf("quadra Uau!");      //se houver 3 igualdades de cartas � quadra
      if (val_ordenado[6] == temp_q)
        high_quadra = val_ordenado[2];
      else
        high_quadra = val_ordenado[6];

      return high_quadra + (temp_q * 20) + 8000; // havendo Quadra o retorno � imediato
      break;                                     // � necessario este break??

    default:
      printf("deu pau no switch do iguais_q");
    }
  }
  // Se o prgrama chegou at� aqui temos certeza de que a mao nao tem:
  //  - Royal SF(Straight flus ou seq naipada), SF, Flush, Quadra.

  // Agora devemos verificar primeiramente Full House com as variaveis dupla e trinca acima
  //  incrementadas
  // printf("val trinca: %d    val dupla: %d",trinca,dupla);
  // Inicio verificador Full House (barbada)
  if ((trinca > 0 && dupla > 0) || (trinca > 1)) // Evnetualmente a pessoa pode ter 2 trincas(q nao e um jogo)
  {
    if (trinca > 1)
      high_full = trinca_menor;
    else
      high_full = dupla_maior;

    return high_full + (trinca_maior * 20) + 7000; // mas o porograma nao interpretaria como Full entao
  }
  // fim verificador Full House                 //adicionei a possibilidade || trinca>1

  //////////Verificador Sequencia Straight

  cont_s1 = 0;
  for (cont_s2 = 1; cont_s2 < 7; cont_s2++)
  {
    if ((val_ordenado[cont_s2] == val_ordenado[cont_s2 - 1]) && (cont_s2 < 6))
      continue; // Isto resolve o problema de carta de mesmo num e seq
    else
      ;

    if (val_ordenado[cont_s2] - 1 == val_ordenado[cont_s2 - 1])
    {
      cont_s1++;
      aux_s = val_ordenado[cont_s2];
      if (cont_s2 < 6) // Para analisar a ultima iteracao.
        continue;
      else
        ; // Else so para organizar
    }
    else
      ;

    if (cont_s1 < 4)
    {
      if (cont_s2 == 6) // ultima iteracao do ciclo (sai fora q nao tem sequencia)
        break;
      else
        cont_s1 = 0;
    }
    else
      return aux_s + 5000;
  }
  // verifica excecao!! (AS(14),2,3,4,5)
  if ((val_ordenado[0] == 2) && val_ordenado[6] == 14) // Para economizar tempo se a prim nao for 2 e a ultima 14 (As) nem perde tempo
  {
    cont_s1 = 0;
    for (cont_s2 = 1; cont_s2 < 7; cont_s2++)
    {
      if ((val_ordenado[cont_s2] == val_ordenado[cont_s2 - 1]) && (cont_s2 < 6))
        continue;
      else if (val_ordenado[cont_s2] == val_ordenado[cont_s2 - 1] + 1)
      {
        cont_s1++;
        if (cont_s1 == 3)
          return 5005;
        else
          ;
      }
      else
        break;
    }
  }
  // Fim verificacao excecao!! UFaa!
  if (trinca == 1)
  { // Bloco para mandar as 2 high cards acompanhantes da trinca
    int auxx_trinca = 0;
    int ff_trinca;
    for (ff_trinca = 6; auxx_trinca < 2; ff_trinca--) // Pq 2?? porque 2 sao as cartas Highs necessarias para completar a mao trinca
    {
      if (val_ordenado[ff_trinca] == trinca_maior)
        continue;
      else
      {
        *(vhighs + auxx_trinca) = val_ordenado[ff_trinca];
        auxx_trinca++;
      }
    }                           // Fim do bloco acompanhantes
    return trinca_maior + 4000; // retrono de trinca
  }

  if (dupla >= 2)
  {
    high_2d = 0; // high card 2 duplas
    contq1 = 6;  // auxiliar no high card das 2 duplas
    while ((high_2d == 0) || (high_2d == dupla_maior) || (high_2d == dupla_menor))
    {
      high_2d = val_ordenado[contq1];
      contq1--;
    }
    *(vhighs) = high_2d;
    return dupla_maior * 20 + dupla_menor + 3000; // retorno de 2 duplas
  }

  if (dupla == 1)
  {
    int auxi_dupla = 0;
    int ff_dupla;

    for (ff_dupla = 6; auxi_dupla < 3; ff_dupla--) // Pq <3?? pela mesma razao que a trinca
    {
      if (val_ordenado[ff_dupla] == dupla_maior)
        continue;
      else
      {
        *(vhighs + auxi_dupla) = val_ordenado[ff_dupla];
        auxi_dupla++;
      }
    }

    return dupla_maior + 2000; // retorno de 1 dupla
  }

  //       INICIO HIGH CARDS LIMPO (sem jogo NENHUMM!!!)
  else
  {
    int aux_ind;
    int aux_ii = 0;
    for (aux_ind = 6; aux_ind > 1; aux_ind--)
    {
      *(vhighs + aux_ii) = val_ordenado[aux_ind];
      aux_ii++;
    }
  }
  return 1000 + val_ordenado[6];
}
////////////////////////////////////////////////////////////////////////////////////

// FUNCAO INTERFACE JOGADOR 1x //
int interf_jog(int jog, int turno, int *stack, int pot)
{
  int cccc;
  int bc = 178;
  int contm;

  printf("\n");
  //    printf("\a");
  if ((jog == 1) || (jog == 0)) // jogador 0 quer dizer virar ambos jogadores para cima
  {
    printf("\n\t\t\t\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c %c%c    %c %c  %c %c%c    %c %c", bc, cartas_exib[0][0], cartas_exib[0][1], cartas_exib[0][2], bc, bc, cartas_exib[1][0], cartas_exib[1][1], cartas_exib[1][2], bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c    %c%c   %c  %c    %c%c   %c", bc, cartas_exib[0][0], cartas_exib[0][1], bc, bc, cartas_exib[1][0], cartas_exib[1][1], bc);
    printf("\n\t\t\t\t\t  %c %s %c  %c %s %c", bc, naipe_exib[0], bc, bc, naipe_exib[1], bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c %c    %c%c %c  %c %c    %c%c %c", bc, cartas_exib[0][2], cartas_exib[0][0], cartas_exib[0][1], bc, bc, cartas_exib[1][2], cartas_exib[1][0], cartas_exib[1][1], bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
  }
  else
  {
    for (cccc = 0; cccc < 12; cccc++)
      printf("\n\t\t\t\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
  }
  printf("\n\n\n");

  printf("\t\t\t\t\t Jogador 1 - Fichas:  %d", stack[0]);

  printf("\n\n\n");

  //  CARTAS DO MEIAO!! (mesa)
  printf("\t\t\t\t  Flop  \t\t\tTurn \t       River\n");

  switch (turno)
  {
  case 1:
    for (cccc = 0; cccc < 12; cccc++)
      printf("\n\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);

    break;

  case 2:

    printf("\n\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c %c%c    %c %c  %c %c%c    %c %c  %c %c%c    %c %c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, cartas_exib[4][0], cartas_exib[4][1], cartas_exib[4][2], bc, bc, cartas_exib[5][0], cartas_exib[5][1], cartas_exib[5][2], bc, bc, cartas_exib[6][0], cartas_exib[6][1], cartas_exib[6][2], bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c    %c%c   %c  %c    %c%c   %c  %c    %c%c   %c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, cartas_exib[4][0], cartas_exib[4][1], bc, bc, cartas_exib[5][0], cartas_exib[5][1], bc, bc, cartas_exib[6][0], cartas_exib[6][1], bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c %s %c  %c %s %c  %c %s %c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, naipe_exib[4], bc, bc, naipe_exib[5], bc, bc, naipe_exib[6], bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c %c    %c%c %c  %c %c    %c%c %c  %c %c    %c%c %c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, cartas_exib[4][2], cartas_exib[4][0], cartas_exib[4][1], bc, bc, cartas_exib[5][2], cartas_exib[5][0], cartas_exib[5][1], bc, bc, cartas_exib[6][2], cartas_exib[6][0], cartas_exib[6][1], bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);

    break;

  case 3:
    printf("\n\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c %c%c    %c %c  %c %c%c    %c %c  %c %c%c    %c %c     %c %c%c    %c %c     %c%c%c%c%c%c%c%c%c%c%c", bc, cartas_exib[4][0], cartas_exib[4][1], cartas_exib[4][2], bc, bc, cartas_exib[5][0], cartas_exib[5][1], cartas_exib[5][2], bc, bc, cartas_exib[6][0], cartas_exib[6][1], cartas_exib[6][2], bc, bc, cartas_exib[7][0], cartas_exib[7][1], cartas_exib[7][2], bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c    %c%c   %c  %c    %c%c   %c  %c    %c%c   %c     %c    %c%c   %c     %c%c%c%c%c%c%c%c%c%c%c", bc, cartas_exib[4][0], cartas_exib[4][1], bc, bc, cartas_exib[5][0], cartas_exib[5][1], bc, bc, cartas_exib[6][0], cartas_exib[6][1], bc, bc, cartas_exib[7][0], cartas_exib[7][1], bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c %s %c  %c %s %c  %c %s %c     %c %s %c     %c%c%c%c%c%c%c%c%c%c%c", bc, naipe_exib[4], bc, bc, naipe_exib[5], bc, bc, naipe_exib[6], bc, bc, naipe_exib[7], bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c %c    %c%c %c  %c %c    %c%c %c  %c %c    %c%c %c     %c %c    %c%c %c     %c%c%c%c%c%c%c%c%c%c%c", bc, cartas_exib[4][2], cartas_exib[4][0], cartas_exib[4][1], bc, bc, cartas_exib[5][2], cartas_exib[5][0], cartas_exib[5][1], bc, bc, cartas_exib[6][2], cartas_exib[6][0], cartas_exib[6][1], bc, bc, cartas_exib[7][2], cartas_exib[7][0], cartas_exib[7][1], bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);

    break;
  case 4:
    printf("\n\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c         %c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c %c%c    %c %c  %c %c%c    %c %c  %c %c%c    %c %c     %c %c%c    %c %c     %c %c%c    %c %c", bc, cartas_exib[4][0], cartas_exib[4][1], cartas_exib[4][2], bc, bc, cartas_exib[5][0], cartas_exib[5][1], cartas_exib[5][2], bc, bc, cartas_exib[6][0], cartas_exib[6][1], cartas_exib[6][2], bc, bc, cartas_exib[7][0], cartas_exib[7][1], cartas_exib[7][2], bc, bc, cartas_exib[8][0], cartas_exib[8][1], cartas_exib[8][2], bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c         %c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c         %c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c    %c%c   %c  %c    %c%c   %c  %c    %c%c   %c     %c    %c%c   %c     %c    %c%c   %c", bc, cartas_exib[4][0], cartas_exib[4][1], bc, bc, cartas_exib[5][0], cartas_exib[5][1], bc, bc, cartas_exib[6][0], cartas_exib[6][1], bc, bc, cartas_exib[7][0], cartas_exib[7][1], bc, bc, cartas_exib[8][0], cartas_exib[8][1], bc);
    printf("\n\t\t  %c %s %c  %c %s %c  %c %s %c     %c %s %c     %c %s %c", bc, naipe_exib[4], bc, bc, naipe_exib[5], bc, bc, naipe_exib[6], bc, bc, naipe_exib[7], bc, bc, naipe_exib[8], bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c         %c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c         %c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c %c    %c%c %c  %c %c    %c%c %c  %c %c    %c%c %c     %c %c    %c%c %c     %c %c    %c%c %c", bc, cartas_exib[4][2], cartas_exib[4][0], cartas_exib[4][1], bc, bc, cartas_exib[5][2], cartas_exib[5][0], cartas_exib[5][1], bc, bc, cartas_exib[6][2], cartas_exib[6][0], cartas_exib[6][1], bc, bc, cartas_exib[7][2], cartas_exib[7][0], cartas_exib[7][1], bc, bc, cartas_exib[8][2], cartas_exib[8][0], cartas_exib[8][1], bc);
    printf("\n\t\t  %c         %c  %c         %c  %c         %c     %c         %c     %c         %c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c     %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);

    break;
  }

  // Fim das cartas mesa
  printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t POTE: %d", pot);
  printf("\n\n\n\n");

  // JOOGADOR 2
  if ((jog == 2) || (jog == 0)) // jogador 0 quer dizer virar ambos jogadores para cima
  {
    printf("\n\t\t\t\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c %c%c    %c %c  %c %c%c    %c %c", bc, cartas_exib[2][0], cartas_exib[2][1], cartas_exib[2][2], bc, bc, cartas_exib[3][0], cartas_exib[3][1], cartas_exib[3][2], bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c    %c%c   %c  %c    %c%c   %c", bc, cartas_exib[2][0], cartas_exib[2][1], bc, bc, cartas_exib[3][0], cartas_exib[3][1], bc);
    printf("\n\t\t\t\t\t  %c %s %c  %c %s %c", bc, naipe_exib[2], bc, bc, naipe_exib[3], bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c %c    %c%c %c  %c %c    %c%c %c", bc, cartas_exib[2][2], cartas_exib[2][0], cartas_exib[2][1], bc, bc, cartas_exib[3][2], cartas_exib[3][0], cartas_exib[3][1], bc);
    printf("\n\t\t\t\t\t  %c         %c  %c         %c", bc, bc, bc, bc);
    printf("\n\t\t\t\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
  }
  else
  {
    for (cccc = 0; cccc < 12; cccc++)
      printf("\n\t\t\t\t\t  %c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c", bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc, bc);
  }
  printf("\n\n\n");
  printf("\t\t\t\t\t Jogador 2 - Fichas:  %d", stack[1]);
}
// FIm Interface jogador 1 //

int main(void)
{
  printf("\n\n\n\n\n\t\t\t\t\t\t Prepare-se para jogar GO POKER!! \n\n \t\t\t\t\t\t No Limit Texas Hold'en Poker  \n\n \t\t\t\t\t\t Desenvolvido por gmeneguz \n\n\n\n\n\n\n\n\n\n \t\t\t\t Defina o Jogador 1 e o Jogador 2 e pressione qualquer tecla");
  getch();
  system("color F0");
  int aux_j, vjogador; // Vez do jogador
  int naojogador;      // o numero do jogador contrario a vez
  int cont_vez;        // Quem joga? se for impar jogador 1 se par jogador 2.
  int cont_rand, cc;
  int cartas_t[9];
  //      int cartas_exib[CMESA][3];     //9 cartas 3 caracteres (1� num 2� num 3� simb ASCII naipe)
  //      char naipe_exib[CMESA][8];     //Nome de cada naipe efetivo ate 7 letras(ESPADAS,PAUS...)
  // para cada uma das 9 cartas

  int cont_exib;   // usado no for
  int temp_naipex; // temp_naipex recebe de 1 a 4  (dep do naipe)
  int temp_numex;  // temp_numex recebe de 1 a 13 (dep do val)

  int turno_cont; // Auxiliar de turno
  int turno;      // sao 5 turnos: Inicio BLinds(1), Floppy(2), Turn(3), River (4), Vencedor(5).
  int fichas[2];  // fichas   pos 0 fichas jogador 1 pos 1 fichas jogador 2
  int j_anterior; // jogador da ultima rodada
  int aposta;     // Valor da aposta... quando diferente de 0 aparece "pagar" para o outro jogador
  int pote;
  int falar;
  int jsmall; // Jogador small blind
  int jbig;   // jogador Big blind
  int vencedormao;
  int vencedorp; // Vencedor de toda a partida ate o fim das fichas
  int apmin;     // Aposta minima
  int apmax;     // Aposta maxima
  int aumento;

  for (;;) // For geral Volta pra ca quando um jogador perde de vez e decidem jogar outra partida
  {

    // Valores alteraveis
    fichas[0] = 10000;
    fichas[1] = 10000;
    j_anterior = 1; //  O jogador que comeca como Bblind

    for (;;) // For para uma mao inteira....quando reinicia, uma nova mao comeca e quando acaba a partida toda acaba... E volta ao menu acima
    {
      int cont_verif;         // cont_verif � o valor do [] do vetor que progride
      int temp_verif;         // temp_verif recebe o valor do [] temporariamente para teste
      int bb_verif, ss_verif; // auxiliares for

      for (cont_rand = 0; cont_rand < 9; cont_rand++)
        cartas_t[cont_rand] = getRandom(CMIN, CMAX); // a posicao do vetor recebe um valor aleatorio

      // Importante: Das cartas_t[9] ...as 2 primeiras[0]e[1]   sao da mao do jogador 1
      //...a  3�,4�  [2] e [3]     sao da mao do jogador 2
      //...da 5� a 9� [4][5][6][7][8] sao da mesa em si.

      // FUNCAO que testa se existe cartas repetidas

      ss_verif = 1;
      while (ss_verif >= 0)
      {
        ss_verif = -1;
        for (cont_verif = 0; cont_verif < 8; cont_verif++)
        {
          temp_verif = cartas_t[cont_verif];
          for (bb_verif = 8; bb_verif > cont_verif; bb_verif--)
          {
            if (temp_verif == cartas_t[bb_verif])
            {
              cartas_t[cont_verif] = getRandom(CMIN, CMAX);
              ss_verif = 0;
            }
            else
              ;
          }
        }
      }
      // FIM DA FUNCAO que testa se existe cartas rep

      // Inicio funcao que transforma nuemros de 1 a 52 em 101 a 413
      int cont_trans, temp_trans, aux_trans;

      for (cont_trans = 0; cont_trans < CMESA; cont_trans++)
      {
        temp_trans = cartas_t[cont_trans];
        aux_trans = (temp_trans - 1) / 13;
        switch (aux_trans)
        {
        case 0:
          cartas_t[cont_trans] = 101 + (temp_trans - 1) % 13;
          break;
        case 1:
          cartas_t[cont_trans] = 201 + (temp_trans - 1) % 13;
          break;
        case 2:
          cartas_t[cont_trans] = 301 + (temp_trans - 1) % 13;
          break;
        case 3:
          cartas_t[cont_trans] = 401 + (temp_trans - 1) % 13;
          break;
        default:
          printf("\n Deu pau no switch.. Valor da div ta errado hehe");
        }
      }

      for (cont_rand = 0; cont_rand < 9; cont_rand++)

        for (cont_exib = 0; cont_exib < CMESA; cont_exib++)
        {
          temp_naipex = cartas_t[cont_exib] / 100;
          temp_numex = cartas_t[cont_exib] % 100;

          if (temp_numex > 1 && temp_numex < 10)         // Numeros de 2 a 9 sao
          {                                              // Inseridos como eles mesmos
            cartas_exib[cont_exib][0] = 48 + temp_numex; // '48' � 0 em ASCII + o temp_numex
            cartas_exib[cont_exib][1] = 32;              //'32' � Espaco  em ASCII
          }
          else
          {
            switch (temp_numex)
            {
            case 1:
              cartas_exib[cont_exib][0] = 65; //'65' � A em ASCII
              cartas_exib[cont_exib][1] = 32; //'32' � Espaco ASCII
              break;
            case 10:
              cartas_exib[cont_exib][0] = 49; //'49' � 1 em ASCII
              cartas_exib[cont_exib][1] = 48; //'48' � 0 em ASCII
              break;
            case 11:
              cartas_exib[cont_exib][0] = 74; //'74' � J em ASCII
              cartas_exib[cont_exib][1] = 32; //'32' � Espaco ASCII
              break;
            case 12:
              cartas_exib[cont_exib][0] = 81; //'81' � Q em ASCII
              cartas_exib[cont_exib][1] = 32; //'32' � Espaco  ASCII
              break;
            case 13:
              cartas_exib[cont_exib][0] = 75; //'75' � K em ASCII
              cartas_exib[cont_exib][1] = 32; //'32' � Espaco ASCII
              break;

            default:
              printf("Valor invalido na hora da exibicao cont_exib");
            } // Fim do Switch
          }   // Fim do Else

          // Inicio distribuicao de Naipes

          switch (temp_naipex)
          {
          case 1:
            cartas_exib[cont_exib][2] = 3;            // 3 � copas(coracao) em ASCII
            strcpy(naipe_exib[cont_exib], " COPAS "); // copia a palavra copas
            break;

          case 2:
            cartas_exib[cont_exib][2] = 6;            // 6 � espadas em ASCII
            strcpy(naipe_exib[cont_exib], "ESPADAS"); // copia a palavra ESPADAS
            break;

          case 3:
            cartas_exib[cont_exib][2] = 4;            // 4 � ouros em ASCII
            strcpy(naipe_exib[cont_exib], " OUROS "); // copia a palavra OUROS
            break;

          case 4:
            cartas_exib[cont_exib][2] = 5;            // 5 � Paus em ASCII
            strcpy(naipe_exib[cont_exib], "  PAUS "); // copia a palavra Paus
            break;

          default:
            printf("erro no valor da divsao inteira de naipe");
          } // FIm do Switch.

        } // Fim do FOR!!!

      // ATENCAO: para imprimir os valores das cartas � necessario
      // usar o modelo abaixo...
      //  *   %c%c para o num da carta     | cartas_exib[x][0]....[x][1]
      //  *   %c   para o naipe em ASCII   | cartas_exib[x][2]
      //  *   %s   para o naipe escrito    | naipe_exib[x]
      //  Onde x varia de 0 a 8  referente a qual das 9 cartas.
      //  Feitoriaaa!

      // Inicio distribuicao de maos pelos vetores
      int cont_out, cont_in;
      int mao_j1[7]; // 5 da mesa mais 2 jogador 1
      int mao_j2[7]; // 5 da mesa mais 2 jogador 2

      int vhigh1[5]; // Sao utilizadas para analisar as High cards jog 1 e 2
      int vhigh2[5];

      cont_in = 0; // Jogador 1
      for (cont_out = 0; cont_out < 9; cont_out++)
      {
        if ((cont_out == 2) || (cont_out == 3))
          continue;
        mao_j1[cont_in] = cartas_t[cont_out];
        cont_in++;
      }
      for (cont_out = 2; cont_out < 9; cont_out++) // Jogador 2
        mao_j2[cont_out - 2] = cartas_t[cont_out];

      jbig = j_anterior;                               // O big � o que nao � o small // Serio???
      j_anterior == 2 ? j_anterior = 1 : j_anterior++; // Para alternar o jogador que comeca B blind
      vjogador = j_anterior;
      jsmall = vjogador; // O small � o primeiro a falar portanto o jogador da  vez 1.
      turno_cont = 1;
      turno = 1;
      vencedormao = 0;
      falar = 0;
      apmin = bblind;

      system("cls");
      printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t");
      // Caso o jogador Big tenha menos fichas que a BBlind A partida acaba.
      if (fichas[jbig - 1] < bblind)
      {
        printf("O jogador %d nao tem fichas suficientes para o Big Blind.", jbig);
        getch();
        vencedorp = jsmall;
        break;
      }
      else
      {
        printf("Jogador %d  -  Fichas %d  -  Coloca o BIG Blind (%d)", jbig, fichas[jbig - 1], bblind);
        pote = bblind;
        fichas[jbig - 1] -= bblind;
      }
      printf("\n\n\n\t\t\t\t\t");

      // Caso o jogador Small tenha menos fichas que a Sblind
      if (fichas[jsmall - 1] < bblind)
      {
        printf("O jogador %d nao tem fichas suficientes para o Big Blind.", jsmall);
        getch();
        vencedorp = jbig;
        break;
      }
      else
      {
        printf("Jogador %d  -  Fichas %d  -  Coloca o SMALL Blind (%d)", jsmall, fichas[jsmall - 1], sblind);
        pote += sblind;
        fichas[jsmall - 1] -= sblind;
      }
      getch();
      aposta = sblind;

      for (;;)
      {              // Etapas jogador x
        int aux_ccc; // Auxiliar no desempate por HIGH CARDS (dentro do bloco de comparacao)
        int opc;
        int ret1; // retorno da funcao jogador 1
        int ret2; // retorno funcao jogador 2
        if (turno != 5)
        {
          system("cls");
          printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t");
          if (vjogador) // Se vjogador for zero a interface ira mostrar todas as cartas (1 e 2)
            printf("Jogador %d aperte uma tecla\n", vjogador);
          else
            printf("Aperte uma tecla para mostrar as cartas de ambos jogadores 1 e 2.");
          getch();
          system("cls");
          interf_jog(vjogador, turno, fichas, pote); // variavies

          naojogador = (!(vjogador - 1) + 1);                                      // O adversario da vez  Se vjogador for 1, naojogador recebe 2, vice-v
          apmax = (aposta ? fichas[vjogador - 1] - aposta : fichas[vjogador - 1]); // Se o stack do jogador da vez for menor que a aposta maxima entao aposta maxima recebe o stack
          apmax > fichas[naojogador - 1] ? apmax = fichas[naojogador - 1] : 0;     // Porem se a aposta maxima for maior que o stack do aversario a aposta maxima recebe o stack do adversario

          printf("\n O que deseja fazer?\n");
          if ((aposta == fichas[vjogador - 1]) || (fichas[naojogador - 1] == 0)) // Situacao de All In pra cima dELEE!! Uhul@! vai correr hein??? vamo ve se tu eh homem hein!
          {
            printf("[1] Pagar (All IN  %d)\n", aposta);
            printf("[2] .........\n");
            printf("[3] Desistir \n");
            do
            {
              scanf("%d", &opc);
              fflush(stdin);
            } while (opc != 1 && opc != 3);
            if (opc == 1)
            {
              pote += aposta;
              fichas[vjogador - 1] -= aposta;
              aposta = 0;
              turno = 5;
              continue;
            }
          }
          else
          {
            if (aposta)
              printf("[1] Pagar (%d)\n", aposta);
            else
              printf("[1] Mesa (Check)\n");
            if (apmin >= apmax)
              printf("[2] Aumentar (ALL IN: + %d)\n", apmax);
            else
              printf("[2] Aumentar (minimo %d (+ %d da aposta oponente))\n", apmin, aposta);
            printf("[3] Desistir \n");

            do
            {
              scanf("%d", &opc);
              fflush(stdin);
            } while (opc < 1 || opc > 3);
          }
          printf("\n");
          switch (opc)
          {
          case 1:
            if ((aposta) && (fichas[vjogador - 1] >= aposta))
            {
              pote += aposta;
              fichas[vjogador - 1] -= aposta;
              aposta == sblind ? falar = 0 : falar++; // Sei la nao to afim de escrever
              aposta = 0;
            }
            else if (aposta)
              printf("puts deu pau na aposta");
            else // Else do CHECK ..so chega aqui se houver check e 2 check o falar � incrementado fazendo aumentar um turno (adiante)
            {
              if (turno == turno_cont)
                falar++;
              else
                turno_cont = turno;
            }
            break;

          case 2:
            if (apmin >= apmax)
            {
              printf("\nALL IN, aperte qualquer tecla");
              getch();
              aumento = apmax;
            }
            else
            {
              printf("Aumentar. Informe o valor do aumento (Minimo : %d  -  Maximo : %d)\n", apmin, apmax);
              do
              {
                scanf("%d", &aumento);
                fflush(stdin);
              } while ((aumento < apmin) || (aumento > apmax));
              fflush(stdin);
            }

            if (aposta)
            {
              pote += (aumento + aposta);
              fichas[vjogador - 1] -= (aumento + aposta);
              apmin = aumento + aposta;
            }
            else
            {
              pote += aumento;
              fichas[vjogador - 1] -= aumento;
              apmin = aumento;
            }
            aposta = aumento;
            aumento = 0;
            break;
          case 3:
            vencedormao = !(vjogador - 1) + 1;
            break;

          } // Fim do switch

        } // Fim do if (!5)
        else
        {
          for (cont_out = 0; cont_out < 5; cont_out++) // Zera valores de ambos vetores abaixo
          {
            vhigh1[cont_out] = 0;
            vhigh2[cont_out] = 0;
          }

          // inicio Valores de retorno Jog 1
          ret1 = verif_flush(mao_j1, vhigh1);
          if (!ret1)
            ret1 = verif_quadra(mao_j1, vhigh1);

          // inicio Valores de retorno Jog 2
          ret2 = verif_flush(mao_j2, vhigh2);
          if (!ret2)
            ret2 = verif_quadra(mao_j2, vhigh2);

          // Inicio COMPARACAO!
          if (ret1 == ret2)
          {
            if ((ret1 < 5000) || (ret1 / 1000 == 6))
            {
              int aux_comp;

              switch (ret1 / 1000) // Aqui um aspecto bem interessante
              {                    // O programador tem que apenas digitar quantas cartas quer comparar em cada case:
              case 6:
                aux_ccc = 5; // Flush: devo compara as highs se a primeira for igual
                break;

              case 4:
                aux_ccc = 2; // Trinca: Eu tenho um jogo de 3 cartas e devo comparar Mais 2
                break;
              case 3:
                aux_ccc = 1; // 2 Duplas eu tenho 4 cartas num jog  e devo comparar 1 excedente
                break;
              case 2:
                aux_ccc = 3; // 1 dupla tenho 2 cartas no jogo e devo comparar 3 Higs
                break;
              case 1:
                aux_ccc = 5; // Sem nenhum jogo eu devo comparar uma por uma
                break;

              default:
                printf("Erro na comparacao");
              } // Fim do Switch

              // Avaliacao do aux_ccc
              for (aux_comp = 0; aux_comp < aux_ccc; aux_comp++)
              {
                if (vhigh1[aux_comp] == vhigh2[aux_comp])
                {
                  if (aux_comp == (aux_ccc - 1))
                  {
                    vencedormao = 3;
                    break;
                  }
                  else
                  {
                    continue;
                  }
                }
                else
                  vhigh1[aux_comp] > vhigh2[aux_comp] ? vencedormao = 1 : vencedormao = 2;
                break;
              }

            }                  // Fim do if <5000 ||  ...
            else               // Caso o jogo seja de 5000(sequencia) ou mais (exceto flush) a resposta ja vem completa da funcao
              vencedormao = 3; // Portanto se houver empate � empate mesmo.
          }                    // Fim do if ret == ret
          else
          {
            if (ret1 > ret2)
              vencedormao = 1;
            else if (ret1 < ret2)
              vencedormao = 2;
            else
              printf("Erro na escolha do vencedor");
          }

        } // Fim do else que incia a avaliacao dos jogos

        if (vencedormao)
        {
          if (vencedormao == 3)
          {
            system("cls");
            printf("\n Empatou \n\n Dividem o pote");
            getch();
            interf_jog(0, 4, fichas, pote); // variavies
            //       printf("\n\n Valores de retorno para teste:\n Jogador 1: %d \n Jogador 2: %d",ret1,ret2);
            fichas[0] += pote / 2;
            fichas[1] += pote / 2;
            pote = 0;
            break;
          }
          else
          {
            system("cls");
            printf("\n O jogador %d Perdeu \n\n O jogador %d recebe o valor do pote: %d", !(vencedormao - 1) + 1, vencedormao, pote);
            if (turno == 5)
            {
              interf_jog(0, 4, fichas, pote); // variavies
              printf("\n\n Valores de retorno para teste:\n Jogador 1: %d \n Jogador 2: %d", ret1, ret2);
            }
            if ((ret1 == ret2) && (ret1 < 5000 || ret1 / 1000 == 6))
            {
              printf("\n");
              for (cont_out = 0; cont_out < aux_ccc; cont_out++)
                printf("\n Valores de comparacao High cards Jog 1:  %2.0d     Jog 2:  %2.0d", vhigh1[cont_out], vhigh2[cont_out]);
            }
            getch();
            fichas[vencedormao - 1] += pote;
            pote = 0;
            system("cls");
            break;
          }
        }
        else
        {
          if (falar)
          {
            turno++;
            vjogador = jbig;
            falar = 0;
          }
          else
            vjogador = !(vjogador - 1) + 1;
        }
      } // Fim do for dos turnos
      printf("\n fim turno");
      getch();
    } // Fim do for das partidas
    int sair;
    printf("\n\n O Jogador %d Venceu a partida!!! PARABENS!\n\n", vencedorp);
    printf("\nDeseja jogar outra partida?\n\n[1]Sim\n[2]Nao (sair)\n");
    scanf("%d", &sair);
    if (sair == 2)
      exit(0);
  } // FIm do for geral  quando um jogador perde de vez e decidem jogar outra partida
} // FIM DO PROGRAMA
