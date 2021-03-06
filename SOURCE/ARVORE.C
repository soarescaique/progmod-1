/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo �rvore
*
*  Arquivo gerado:              ARVORE.C
*  Letras identificadoras:      ARV
*
*  Nome da base de software:    Exemplo de teste automatizado
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\SIMPLES.BSW
*
*  Projeto: Disciplinas INF 1628 / 1301
*  Gestor:  DI/PUC-Rio
*  Autores: avs - Arndt von Staa
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*       3.00   avs   28/02/2003 Uniformiza��o da interface das fun��es e
*                               de todas as condi��es de retorno.
*       2.00   avs   03/08/2002 Elimina��o de c�digo duplicado, reestrutura��o
*       1.00   avs   15/08/2001 In�cio do desenvolvimento
*
***************************************************************************/

#include   <malloc.h>
#include   <stdio.h>

#define ARVORE_OWN
#include "ARVORE.H"
#undef ARVORE_OWN

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor do n� da �rvore
*
*
*  $ED Descri��o do tipo
*     Descreve a organiza��o do n�
*
***********************************************************************/

   typedef struct tgNoArvore {

         struct tgNoArvore * pNoPai ;
               /* Ponteiro para pai
               *
               *$EED Assertivas estruturais
               *   � NULL sse o n� � raiz
               *   Se n�o for raiz, um de pNoEsq ou pNoDir de pNoPai do n�
               *   corrente apontam para o n� corrente */

         struct tgNoArvore * pNoEsq ;
               /* Ponteiro para filho � esquerda
               *
               *$EED Assertivas estruturais
               *   se pNoEsq do n� X != NULL ent�o pNoPai de pNoEsq aponta para o n� X */

         struct tgNoArvore * pNoDir ;
               /* Ponteiro para filho � direita
               *
               *$EED Assertivas estruturais
               *   se pNoDir do n� X != NULL ent�o pNoPai de pNoDir aponta para o n� X */

         char Valor ;
               /* Valor do n� */

		 ARV_tpModoVisita ModoVisita ;
		       /* Modo de visita do n� */

   } tpNoArvore ;

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor da cabe�a de uma �rvore
*
*
*  $ED Descri��o do tipo
*     A cabe�a da �rvore � o ponto de acesso para uma determinada �rvore.
*     Por interm�dio da refer�ncia para o n� corrente e do ponteiro
*     pai pode-se navegar a �rvore sem necessitar de uma pilha.
*     Pode-se, inclusive, operar com a �rvore em forma de co-rotina.
*
***********************************************************************/

   typedef struct tgArvore {

         tpNoArvore * pNoRaiz ;
               /* Ponteiro para a raiz da �rvore */

         tpNoArvore * pNoCorr ;
               /* Ponteiro para o n� corrente da �rvore */

   } tpArvore ;

/*****  Dados encapsulados no m�dulo  *****/

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static tpNoArvore * CriarNo( char ValorParm ) ;

   static ARV_tpCondRet CriarNoRaiz( tpArvore * pArvore , char ValorParm ) ;

   static void DestroiArvore( tpNoArvore * pNo ) ;

   static ARV_tpCondRet ObterModoNoCorr( tpArvore * pArvore , ARV_tpModoVisita * ModoParm ) ;

   static ARV_tpCondRet MarcarVisitado( tpArvore * pArvore , ARV_tpModoVisita Modo ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: ARV Criar �rvore
*  ****/

   ARV_tpCondRet ARV_CriarArvore( void ** ppArvoreParm )
   {
	  
	  tpArvore *pNovaArvore;
	  
      if ( *ppArvoreParm != NULL )
      {
         ARV_DestruirArvore( ppArvoreParm ) ;
      } /* if */

      pNovaArvore = ( tpArvore * ) malloc( sizeof( tpArvore )) ;
      if ( pNovaArvore == NULL )
      {
         return ARV_CondRetFaltouMemoria ;
      } /* if */

      pNovaArvore->pNoRaiz = NULL ;
      pNovaArvore->pNoCorr = NULL ;
	  *ppArvoreParm = pNovaArvore;

      return ARV_CondRetOK ;

   } /* Fim fun��o: ARV Criar �rvore */

/***************************************************************************
*
*  Fun��o: ARV Destruir �rvore
*  ****/

   void ARV_DestruirArvore( void ** ppArvoreParm )
   {

      tpArvore * pArvore ;

      pArvore = ( tpArvore * )( * ppArvoreParm ) ;
      if ( pArvore != NULL )
      {
         if ( pArvore->pNoRaiz != NULL )
         {
            DestroiArvore( pArvore->pNoRaiz ) ;
         } /* if */
         free( pArvore ) ;
         *ppArvoreParm = NULL ;
      } /* if */

   } /* Fim fun��o: ARV Destruir �rvore */

/***************************************************************************
*
*  Fun��o: ARV Exibe a �rvore
*  ****/

   void ARV_ExibirArvore( void * pArvoreParm )
   {
      
      /* vari�veis auxiliares */

      int NoEsq, NoDir, NoDirNulo;
      char valor;
      ARV_tpCondRet retorno;
      ARV_tpModoVisita modo = ARV_ModoNulo;
      tpArvore *pArvore;
      
      /* Tratar de �rvore inexistente */

      if(pArvoreParm == NULL)
      {
         printf("Arvore inexistente\n");
         return;
      } /* if */

      pArvore = ( tpArvore * ) ( pArvoreParm );

      /* Tratar de �rvore vazia */

      if(pArvore->pNoRaiz == NULL)
      {
         printf("Arvore vazia\n");
         return;
      } /* if */

      /* A partir daqui ignoramos as seguntes condi��es de retorno:
         ARV_CondRetArvoreNaoExiste
         ARV_CondRetArvoreVazia
      */

      /* Retornar ponteiro corrente para raiz */

      pArvore->pNoCorr = pArvore->pNoRaiz;

      /* Testa se o n� raiz � folha ou n�o */

      if(ARV_IrNoEsquerda(pArvore) == ARV_CondRetOK)
      {
         ARV_IrPai(pArvore);
      } /* if */
      else if(ARV_IrNoDireita(pArvore) == ARV_CondRetOK)
      {
         ARV_IrPai(pArvore);
      } /* else if */
      else
      {
         /* Se o n� raiz for folha, rodar caso elementar */

         ARV_ObterValorCorr(pArvore,&valor);
         printf("( %c )",valor);
         return;
      } /* else */

      printf("(");

      /* No algoritmo desta fun��o, � dado um modo de visita a um n�
         para rastrear o caminhamento feito na �rvore:
         
         ARV_ModoNulo - todos os n�s incialmente s�o nulos
         ARV_ModoDePai - quando o pai do n� � acessado
         ARV_ModoParaEsq - quando o n� esquerdo � acessado
         ARV_ModoParaDir - quando o n� direito � acessado

         O algoritmo cessar� quando o n� raiz tiver modo ARV_ModoDePai */

      while(pArvore->pNoRaiz->ModoVisita != ARV_ModoDePai)
      {

         /* flags: caso nenhum dos n�s filhos puderam ser acessados, portanto,
            inexistentes, o n� corrente � uma folha */

         NoDir = 0;
         NoEsq = 0;

         /* Apontar para o n� mais a esquerda */

         while(ARV_IrNoEsquerda(pArvore) == ARV_CondRetOK)
         {
            ARV_IrPai(pArvore);
            ARV_ObterValorCorr(pArvore,&valor);
            printf(" %c (",valor);
            MarcarVisitado(pArvore,ARV_ModoParaEsq);
            ARV_IrNoEsquerda(pArvore);
            NoEsq = 1;
         } /* while */

         /* Caso n�o haja n� a esquerda, ir para direita uma vez */

         if(ARV_IrNoDireita(pArvore) == ARV_CondRetOK)
         {
            ARV_IrPai(pArvore);
            ARV_ObterValorCorr(pArvore,&valor);
            printf(" %c",valor);
            MarcarVisitado(pArvore,ARV_ModoParaDir);
            ARV_IrNoDireita(pArvore);
            NoDir = 1;
         } /* if */

         /* Caso n�o houver nem n� � esquerda nem � direita, ou seja,
            for folha, retornar at� o n� mais pr�ximo ainda n�o visitado */

         if(!NoDir && !NoEsq)
         {

            /* O n� corrente passa a ser o n� pai enquanto a folha tiver modo de
               visita igual a ARV_ModoParaDir (vai para o pai uma vez ao menos) */
            
            do
            {
               if(ARV_IrPai(pArvore) == ARV_CondRetOK)
               {
                  ObterModoNoCorr(pArvore,&modo);
                  if(modo == ARV_ModoParaDir)
                  {
                     /* O n� corrente � o �ltimo dos irm�os */

                     ARV_IrNoDireita(pArvore);
                     ObterModoNoCorr(pArvore,&modo);
                     if(modo == ARV_ModoNulo)
                     {                        
                        ARV_ObterValorCorr(pArvore,&valor);
                        printf(" %c )",valor);
                        
                     }
                     MarcarVisitado(pArvore,ARV_ModoDePai);
                     ARV_IrPai(pArvore);
                     ObterModoNoCorr(pArvore,&modo);
                  } /* if */
                  else  /* s�o desconsiderados os modos de visita ARV_ModoDePai e ARV_ModoNulo
                           pois � il�gico este n� corrente ter sido acessado sem que
                           seu pai (que existe, como ARV_IrPai() = ARV_CondRetOK) tenha
                           sido percorrido (portanto n�o � ARV_ModoNulo) ou declarado como
                           inteiramente explorado (portanto n�o � ARV_ModoDePai). Portanto,
                           s� resta ser ARV_ModoParaEsq */
                  {
                     /* O n� corrente � o primeiro filho */

                     ARV_IrNoEsquerda(pArvore);
                     ObterModoNoCorr(pArvore,&modo);
                     if(modo == ARV_ModoNulo)
                     {                        
                        ARV_ObterValorCorr(pArvore,&valor);
                        printf(" %c )",valor);
                        
                     }
                     MarcarVisitado(pArvore,ARV_ModoDePai);
                     ARV_IrPai(pArvore);
                     ObterModoNoCorr(pArvore,&modo);
                  } /* else */
               } /* if */
               else
               {

                  /* Caso o n� corrente seja o n� raiz */

                  /* Neste bloco j� assumimos que ou todos os filhos �
                     esquerda do n� raiz foram explorados ou ele n�o
                     possui n�s filhos, e, portanto, � folha */

                  /* Portanto, testamos se h� um filho � direita
                     inexplorado. */

                  if(ARV_IrNoDireita(pArvore) == ARV_CondRetOK)
                  {
                     /* Se sim, alteramos o modo do n� raiz para ARV_ModoParaDir
                        e o filho � direita se torna o n� corrente */
                     ObterModoNoCorr(pArvore,&modo);
                     ARV_IrPai(pArvore);
                     if(modo == ARV_ModoNulo)
                     {
                        MarcarVisitado(pArvore,ARV_ModoParaDir);
                        ARV_IrNoDireita(pArvore);
                        ObterModoNoCorr(pArvore,&modo); /* modo = ARV_ModoNulo */
                     }
                     else
                     {
                        MarcarVisitado(pArvore,ARV_ModoDePai);
                        ObterModoNoCorr(pArvore,&modo); /* modo = ARV_ModoDePai */
                     }
                     break;
                  } /* if */
                  else
                  {
                     /* Se n�o houver filhos � direita, declaramos fim do caminhamento */

                     ARV_IrPai(pArvore);
                     MarcarVisitado(pArvore,ARV_ModoDePai);
                     printf(" )");
                     break;   
                  }  /* else */           
               } /* else */

            }while(modo == ARV_ModoParaDir); /* do-while */

            /* Caso a folha seja o n� a esquerda do seu pai */

            if(modo == ARV_ModoParaEsq)
            {

               NoDirNulo = 0;
               
               /* Enquanto n�o houver um n� a direita cujo modo de visita
                  seja ARV_tpModoNulo, ou seja, que n�o foi explorado ainda*/

               while(!NoDirNulo)
               {
                  if(ARV_IrNoDireita(pArvore) == ARV_CondRetOK)
                  {
                     ObterModoNoCorr(pArvore,&modo);
                     if(modo == ARV_ModoNulo)
                     {
                        NoDirNulo = 1; /* Achado o n� n�o explorado ainda, sairemos deste loop */

                        /* Caso o n� corrente seja a raiz e tiver um filho � direita, ARV_IrPai() vai falhar
                           mas o algoritmo continuar� funcionando, pois ele ir� retornar ao n� raiz quando
                           a �rvore � sua direita for completamente explorado */

                        ARV_IrPai(pArvore);
                        MarcarVisitado(pArvore,ARV_ModoParaDir);
                        ARV_IrNoDireita(pArvore);
                        break;

                     } /* if */
                     else if(modo == ARV_ModoDePai)
                     {
                        ARV_IrPai(pArvore);
                        MarcarVisitado(pArvore,ARV_ModoDePai);
                        if(ARV_IrPai(pArvore) == ARV_CondRetNohEhRaiz)
                        {
                           break;
                        }
                     }
                  } /* if */
                  else if(ARV_IrPai(pArvore) == ARV_CondRetOK)
                  {
                     retorno = ObterModoNoCorr(pArvore,&modo);
                     if(modo == ARV_ModoParaDir)
                     {
                        ARV_IrNoDireita(pArvore);
                        MarcarVisitado(pArvore,ARV_ModoDePai);
                        ARV_IrPai(pArvore);
                     } /* if */
                     else
                     {
                        ARV_IrNoEsquerda(pArvore);
                        MarcarVisitado(pArvore,ARV_ModoDePai);
                        ARV_IrPai(pArvore);
                     } /* else */
                  } /* else-if */
                  else
                  {
                     MarcarVisitado(pArvore,ARV_ModoDePai);
                     break;
                  } /* else */
               } /* while */
            } /* if */
         } /* if */

      } /* while */

   } /* Fim fun��o: ARV Exibe a �rvore */

/***************************************************************************
*
*  Fun��o: ARV Adicionar filho � esquerda
*  ****/

   ARV_tpCondRet ARV_InserirEsquerda( void * pArvoreParm , char ValorParm )
   {

      tpArvore *pArvore;

      ARV_tpCondRet CondRet ;

      tpNoArvore * pCorr ;
      tpNoArvore * pNo ;

      if(pArvoreParm == NULL)
      {
         return ARV_CondRetArvoreNaoExiste ;
      } /* if */

      pArvore = (tpArvore *) (pArvoreParm);

      /* Tratar vazio, esquerda */

         CondRet = CriarNoRaiz( pArvore, ValorParm ) ;
         if ( CondRet != ARV_CondRetNaoCriouRaiz )
         {
            return CondRet ;
         } /* if */

      /* Criar n� � esquerda de folha */

         pCorr = pArvore->pNoCorr ;
         if ( pCorr == NULL )
         {
            return ARV_CondRetErroEstrutura ;
         } /* if */
               
         if ( pCorr->pNoEsq == NULL )
         {
            pNo = CriarNo( ValorParm ) ;
            if ( pNo == NULL )
            {
               return ARV_CondRetFaltouMemoria ;
            } /* if */
            pNo->pNoPai      = pCorr ;
            pCorr->pNoEsq    = pNo ;
            pArvore->pNoCorr = pNo ;

            return ARV_CondRetOK ;
         } /* if */

      /* Tratar n�o folha � esquerda */

         return ARV_CondRetNaoEhFolha ;

   } /* Fim fun��o: ARV Adicionar filho � esquerda */

/***************************************************************************
*
*  Fun��o: ARV Adicionar filho � direita
*  ****/

   ARV_tpCondRet ARV_InserirDireita( void * pArvoreParm , char ValorParm )
   {

      tpArvore *pArvore;

      ARV_tpCondRet CondRet ;

      tpNoArvore * pCorr ;
      tpNoArvore * pNo ;

      if(pArvoreParm == NULL)
      {
         return ARV_CondRetArvoreNaoExiste ;
      } /* if */

      pArvore = (tpArvore *) (pArvoreParm);

      /* Tratar vazio, direita */

         CondRet = CriarNoRaiz( pArvore, ValorParm ) ;
         if ( CondRet != ARV_CondRetNaoCriouRaiz )
         {
            return CondRet ;
         } /* if */

      /* Criar n� � direita de folha */

         pCorr = pArvore->pNoCorr ;
         if ( pCorr == NULL )
         {
            return ARV_CondRetErroEstrutura ;
         } /* if */
               
         if ( pCorr->pNoDir == NULL )
         {
            pNo = CriarNo( ValorParm ) ;
            if ( pNo == NULL )
            {
               return ARV_CondRetFaltouMemoria ;
            } /* if */
            pNo->pNoPai      = pCorr ;
            pCorr->pNoDir    = pNo ;
            pArvore->pNoCorr = pNo ;

            return ARV_CondRetOK ;
         } /* if */

      /* Tratar n�o folha � direita */

         return ARV_CondRetNaoEhFolha ;

   } /* Fim fun��o: ARV Adicionar filho � direita */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� pai
*  ****/

   ARV_tpCondRet ARV_IrPai( void * pArvoreParm )
   {

      tpArvore *pArvore;

      if ( pArvoreParm == NULL )
      {
         return ARV_CondRetArvoreNaoExiste ;
      } /* if */

      pArvore = ( tpArvore * ) ( pArvoreParm );

      if ( pArvore->pNoCorr == NULL )
      {
         return ARV_CondRetArvoreVazia ;
      } /* if */

      if ( pArvore->pNoCorr->pNoPai != NULL )
      {
         pArvore->pNoCorr = pArvore->pNoCorr->pNoPai ;
         return ARV_CondRetOK ;
      } /* if */

      return ARV_CondRetNohEhRaiz ;
      

   } /* Fim fun��o: ARV Ir para n� pai */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� � esquerda
*  ****/

   ARV_tpCondRet ARV_IrNoEsquerda( void * pArvoreParm )
   {

      tpArvore *pArvore;

      if ( pArvoreParm == NULL )
      {
         return ARV_CondRetArvoreNaoExiste ;
      } /* if */

      pArvore = ( tpArvore * ) ( pArvoreParm );

      if ( pArvore->pNoCorr == NULL )
      {
         return ARV_CondRetArvoreVazia ;
      } /* if */

      if ( pArvore->pNoCorr->pNoEsq == NULL )
      {
         return ARV_CondRetNaoPossuiFilho ;
      } /* if */

      pArvore->pNoCorr = pArvore->pNoCorr->pNoEsq ;
      return ARV_CondRetOK ;

   } /* Fim fun��o: ARV Ir para n� � esquerda */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� � direita
*  ****/

   ARV_tpCondRet ARV_IrNoDireita( void * pArvoreParm )
   {

      tpArvore *pArvore;

      if ( pArvoreParm == NULL )
      {
         return ARV_CondRetArvoreNaoExiste ;
      } /* if */

      pArvore = ( tpArvore * ) ( pArvoreParm );

      if ( pArvore->pNoCorr == NULL )
      {
         return ARV_CondRetArvoreVazia ;
      } /* if */

      if ( pArvore->pNoCorr->pNoDir == NULL )
      {
         return ARV_CondRetNaoPossuiFilho ;
      } /* if */

      pArvore->pNoCorr = pArvore->pNoCorr->pNoDir ;
      return ARV_CondRetOK ;

   } /* Fim fun��o: ARV Ir para n� � direita */

/***************************************************************************
*
*  Fun��o: ARV Obter valor corrente
*  ****/

   ARV_tpCondRet ARV_ObterValorCorr( void * pArvoreParm , char * ValorParm )
   {

      tpArvore *pArvore;

      if ( pArvoreParm == NULL )
      {
         return ARV_CondRetArvoreNaoExiste ;
      } /* if */

      pArvore = ( tpArvore * ) ( pArvoreParm );

      if ( pArvore->pNoCorr == NULL )
      {
         return ARV_CondRetArvoreVazia ;
      } /* if */

      * ValorParm = pArvore->pNoCorr->Valor ;
      return ARV_CondRetOK ;

   } /* Fim fun��o: ARV Obter valor corrente */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/

/***********************************************************************
*
*  $FC Fun��o: ARV Altera o ModoVisita do n� corrente
*
*  $ED Descri��o da fun��o
*     Altera o modo de visita do n� corrente da �rvore corrente,
*     caso haja �rvore, e que n�o esteja vazia.
*
*  $EP Par�metros
*     $P ValorParm - novo modo do n� corrente.
*
*  $FV Valor retornado
*     ARV_CondRetOK
*     ARV_CondRetArvoreNaoExiste
*     ARV_CondRetArvoreVazia
*
***********************************************************************/

   ARV_tpCondRet MarcarVisitado( tpArvore * pArvore , ARV_tpModoVisita Modo )
   {
	   
      if(pArvore == NULL)
      {
         return ARV_CondRetArvoreNaoExiste;
	  } /* if */
	  if(pArvore->pNoCorr == NULL)
      {
         return ARV_CondRetArvoreVazia;
	  } /* if */

	  pArvore->pNoCorr->ModoVisita = Modo;
	  return ARV_CondRetOK;

   } /* Fim fun��o: ARV Altera o ModoVisita do n� corrente */

/***********************************************************************
*
*  $FC Fun��o: ARV Obter modo de visita do n� corrente
*
*  $EP Par�metros
*     $P ModoParm - � o valor que receber� o modo de visita do
*                   n� corrente
*
*  $FV Valor retornado
*     ARV_CondRetOK
*     ARV_CondRetArvoreNaoExiste
*     ARV_CondRetArvoreVazia
*
***********************************************************************/

   ARV_tpCondRet ObterModoNoCorr( tpArvore * pArvore , ARV_tpModoVisita * ModoParm )
   {

      if(pArvore == NULL)
      {
         return ARV_CondRetArvoreNaoExiste;
	  } /* if */
	  if(pArvore->pNoCorr == NULL)
      {
         return ARV_CondRetArvoreVazia;
	  } /* if */

      *ModoParm = pArvore->pNoCorr->ModoVisita;
      return ARV_CondRetOK;

   } /* Fim fun��o: ARV Recebe modo de visita do n� corrente */

/***********************************************************************
*
*  $FC Fun��o: ARV Criar n� da �rvore
*
*  $FV Valor retornado
*     Ponteiro para o n� criado.
*     Ser� NULL caso a mem�ria tenha se esgotado.
*     Os ponteiros do n� criado estar�o nulos e o valor � igual ao do
*     par�metro.
*
***********************************************************************/

   tpNoArvore * CriarNo( char ValorParm )
   {

      tpNoArvore * pNo ;

      pNo = ( tpNoArvore * ) malloc( sizeof( tpNoArvore )) ;
      if ( pNo == NULL )
      {
         return NULL ;
      } /* if */

      pNo->pNoPai = NULL ;
      pNo->pNoEsq = NULL ;
      pNo->pNoDir = NULL ;
      pNo->Valor  = ValorParm ;
	  pNo->ModoVisita = ARV_ModoNulo;
      return pNo ;

   } /* Fim fun��o: ARV Criar n� da �rvore */


/***********************************************************************
*
*  $FC Fun��o: ARV Criar n� raiz da �rvore
*
*  $FV Valor retornado
*     ARV_CondRetOK
*     ARV_CondRetFaltouMemoria
*     ARV_CondRetNaoCriouRaiz
*
***********************************************************************/

   ARV_tpCondRet CriarNoRaiz( tpArvore * pArvore , char ValorParm )
   {

      tpNoArvore * pNo ;

      if ( pArvore->pNoRaiz == NULL )
      {
         pNo = CriarNo( ValorParm ) ;
         if ( pNo == NULL )
         {
            return ARV_CondRetFaltouMemoria ;
         } /* if */

         pArvore->pNoRaiz = pNo ;
         pArvore->pNoCorr = pNo ;

         return ARV_CondRetOK ;
      } /* if */

      return ARV_CondRetNaoCriouRaiz ;

   } /* Fim fun��o: ARV Criar n� raiz da �rvore */


/***********************************************************************
*
*  $FC Fun��o: ARV Destruir a estrutura da �rvore
*
*  $EAE Assertivas de entradas esperadas
*     pNoArvore != NULL
*
***********************************************************************/

   void DestroiArvore( tpNoArvore * pNo )
   {

      if ( pNo->pNoEsq != NULL )
      {
         DestroiArvore( pNo->pNoEsq ) ;
      } /* if */

      if ( pNo->pNoDir != NULL )
      {
         DestroiArvore( pNo->pNoDir ) ;
      } /* if */

      free( pNo ) ;

   } /* Fim fun��o: ARV Destruir a estrutura da �rvore */

/********** Fim do m�dulo de implementa��o: M�dulo �rvore **********/

