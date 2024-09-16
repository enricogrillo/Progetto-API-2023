# PoliMi - Progetto di API - A.A. 2022-2023
Progetto di Algoritmi e Strutture Dati realizzato per il conseguimento della "PROVA FINALE (PROGETTO DI ALGORITMI E STRUTTURE DATI)" al Politecnico di Milano.
Anno accademico 2022-2023; Votazione 30 e Lode

## Abstract del Progetto
Il progetto consiste nel realizzare un programma in C che consenta di gestire un'autostrada, aggiungendo e rimuovendo stazioni, e di calcolare il percorso ottimo tra due stazioni qualsiasi, tenendo conto dell'autonomia delle automobili parcheggiate in ognuna di esse.

## Prestazioni
Oltre a produrre un output corretto (condizione essenziale), il programma deve anche rispettare dei requisiti di tempo e spazio, come indicato in tabella:

| Votazione | Memoria    | Tempo    | Esito |
| :---:   | :---: | :---: | :---: |
|18  | 128 MiB | 19s | ✅ |
|21  | 118 MiB | 15s | ✅ |
|24  | 108 MiB | 10s | ✅ |
|27  | 98 MiB | 6s | ✅ |
|30  | 88 MiB | 4s | ✅ |
|30L  | 78 MiB | 1s | ✅ |

Il qui presente progetto ha superato ogni batteria di test, riuscendo nell'ultima sottoposizione a completare il calcolo in 0,519s, occupando 55,3 MiB di memoria.

![image](https://github.com/user-attachments/assets/77ef174f-abb4-45fd-a8a9-3a134fd61a94)


## Documentazione
La specifica sintetica del progetto è disponibile <a href=""> qui </a> <br>
I test (NON ufficiali) sono disponibili <a href=""> qui </a>

## Scelte progettuali
La documentazione dettagliata relativa alle scelte progettuali è disponibile <a href=""> qui </a>.

### Una rapida occhiata

La struttura dati per la memorizzazione dei nodi autostradali è così definita:

//

Per memorizzare l'autostrada ho scelto di utilizzare un BST (Binary Search Tree), un albero binario di ricerca. Questa scelta si è rivelata vincente data la grande efficienza portata nelle fasi di ricerca e inserimento delle stazioni. È inoltre relativamente poco complesso "linearizzare" questa struttura nel momento in cui si operano gli algoritmi di ricerca dei percorsi ottimi e occorre visitare i nodi in ordine.

## Strumenti utilizzati

| Strumento | Descrizione   |
| :---:   | :---: |
| Notepad++ | IDE   |
| gcc | Compilatore   |
| Windows 10 | Sistema Operativo   |
