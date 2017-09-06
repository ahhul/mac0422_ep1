# mac0422_ep1


Ideia do escalonador
--------------------

A ideia que tive foi a seguinte: se tivermos '''n''' núcleos na máquina

1 - o programa cria '''n+1''' threads (1 para receber novos processos, 1 para cada núcleo)

2 - as threads de cada núcleo vão ser responsaveis por escalonar os processos da tabela de processos nesses nucleos

3 - a thread restante vai ser responsável por adicionar novos processos na tabela de processos