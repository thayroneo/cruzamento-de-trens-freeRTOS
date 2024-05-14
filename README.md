# Simulação de cruzamento de trilhos de trem

## Objetivo
O projeto consiste na criação de um sistema que simule a interação entre uma linha ferroviária e uma estrada para carros em um cruzamento. Tanto os trens quanto os carros podem se aproximar de qualquer direção do cruzamento.  

Para evitar colisões, sempre que um trem se aproximar, uma barreira deve ser acionada e todos os veículos devem parar. A coordenação do acesso ao cruzamento é vital e será gerenciada utilizando semáforos. Serão aplicados conceitos de sincronização e exclusão mútua para garantir o funcionamento seguro e ordenado do sistema.

![Simulação](src/img/simu.png)

## Tecnologia utilizada

A implementação foi realizada com o uso do `FreeRTOS` e programação em C, utilizando `Semáforos` para auxiliar no controle do sistema.

## Requisitos atendidos

1. Modelagem dos trens e carros: cada trem e cada carro deve ser representado por uma thread (ou processo). Eles podem vir de qualquer uma das quatro direções.
2. Cruzamento: Recurso compartilhado, onde o acesso é controlado por semáforos. Podemos ter dois trens cruzando se eles estiverem em sentidos opostos. Os carros só podem cruzar se a cancela não estiver abaixada e a passagem do trem sempre tem prioridade.
3. Semáforos: Deve-se garantir que os semáforos sejam usados para evitar condições de corrida e deadlocks, permitindo uma passagem segura e eficiente.
4. Interface com o usuário: Mostra o estado atual dos trens e dos carros (aproximando-se, passando, e passou) e a situação do cruzamento.

## Construção do Projeto

### Importação das Bibliotecas

Declaração dos semáforos e criação das funções que serão as threads dentro do programa

```

```

### Variáveis e Estruturas

Criação de algumas variáveis que serão usadas nos delays das threads e das estruturas que serão as filas de trens e carros.

```

```

### Filas de trens e carros

Criação das filas de trens e carros, e declaração de funções auxiliares

```

```

### Main

Na função main, os semáforos são criados como Mutex para garantir a exclusão mútua de recursos compartilhados. Em seguida, as tasks são criadas, especificando a função que cada task executará, seu nome, tamanho da pilha, parâmetros (se houver), prioridade e identificador da task (opcional). Por fim, o Scheduler é chamado para gerenciar a execução das tarefas.

```

```

### Função - Criar CARRO

Após a função main, são definidas as funções do programa. A primeira é a função criar_carro, que recebe um ID e uma entrada (0 ou 1) para determinar por qual fila o carro chegará. Com base na entrada, o carro é adicionado à fila correspondente usando as funções add_car ou add_car2. Em seguida, é utilizado vTaskDelay para definir um intervalo de espera até a chegada do próximo carro.

```

```

### Função - Criar TREM

A próxima função é criar_trem, que possui variáveis semelhantes e lógica à função criar_carro. A diferença é que, para criar o trem, é necessário acessar o semáforo semaphore_chegando_trem. Esse semáforo impede que um carro atravesse o cruzamento enquanto um trem estiver se aproximando. Notavelmente, o semáforo não é liberado dentro desta função, pois apenas cria o trem. A liberação do semáforo ocorre na função de remover trem, após o trem terminar de cruzar o cruzamento.

```

```

### Adicionar os carros e trens as filas

```

```

### Percorrer as filas

Outras funções do programa incluem percorrer as filas de carros e imprimir o estado de cada fila. Essas funções são responsáveis por percorrer as filas e imprimir os carros e os trens nelas até que as filas estejam vazias.

```

```

### Remover carro da fila

A função remover_carro verifica se as filas de carros estão vazias. Se estiverem, aguarda um tempo determinado. Se não estiverem vazias, as funções de impressão são chamadas. Em seguida, a função tenta obter acesso aos semáforos, o que ocorre apenas quando não há trens se aproximando ou cruzando o cruzamento. Após obter acesso aos semáforos, um carro é removido de cada fila e os semáforos são liberados.

```

```

### Remover trem da fila

A última função é remover_trem. Ela é semelhante à função de remover_carro, mas precisa apenas acessar o xSemaphore para liberar o trem. Isso ocorre porque a função de criar_trem já acessa o semaphore_chegando_trem, impedindo que a função de remover_carro ocorra antes da remoção do trem. Nessa função, o semaphore_chegando_trem é liberado.

```

```

### Remover trem da fila

A última função é remover_trem. Ela é semelhante à função de remover_carro, mas precisa apenas acessar o xSemaphore para liberar o trem. Isso ocorre porque a função de criar_trem já acessa o semaphore_chegando_trem, impedindo que a função de remover_carro ocorra antes da remoção do trem. Nessa função, o semaphore_chegando_trem é liberado.

```

```

## 📺Vídeo de Demonstração

[![Vídeo de Demonstração](https://img.youtube.com/vi/ZyIT2PiXCF4/hqdefault.jpg)](https://youtu.be/ZyIT2PiXCF4)


## ✒️ Colaboradores
* **Bruno Nascimento de Oliveira** - [BRUNONASCIOLI](https://github.com/BRUNONASCIOLI)
* **José Tayrone Santos de Oliveira** - [thayroneo](https://github.com/thayroneo)
* **Yuri Siqueira Dantas** - [YuriDants](https://github.com/YuriDants)

Você também pode ver a lista de todos os [colaboradores](https://github.com/BRUNONASCIOLI/Projeto_STR/colaboradores) que participaram deste projeto.
