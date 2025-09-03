Análise da Latência entre Hosts e Possíveis Agrupamentos

A análise do heatmap de latência entre hosts (h1 a h6) evidencia a existência de padrões distintos de comunicação dentro da rede, os quais permitem identificar agrupamentos naturais de servidores com base em suas características de conectividade.
Em primeiro lugar, observa-se que os hosts h1 e h2 apresentam baixíssima latência entre si, configurando um subgrupo bastante coeso. Contudo, a comunicação desses hosts com os demais apresenta diferenças significativas: em relação a h3 e h4, a latência é intermediária, enquanto a troca de pacotes com h5 e h6 atinge valores muito 
elevados, indicando grande distância lógica ou gargalo de rede.
O segundo agrupamento é formado por h3 e h4, que também apresentam conexão direta com baixa latência. Em comparação com h1 e h2,
os valores de latência se mantêm em nível moderado, enquanto a comunicação com h5 e h6 apresenta desempenho
inferior, porém não tão elevado quanto no caso de h1 e h2. Dessa forma, h3 e h4 posicionam-se como um bloco 
intermediário, funcionando potencialmente como ponte de conectividade entre os demais clusters.
Por fim, os hosts h5 e h6 constituem o terceiro agrupamento. A latência entre ambos é extremamente baixa, demonstrando forte
proximidade de rede. Entretanto, sua comunicação com h1 e h2 apresenta valores elevados, superando 300 ms, o que reforça a
segregação desse grupo em relação aos demais. Já em relação a h3 e h4, a latência se mantém em nível intermediário, evidenciando 
maior proximidade lógica.
Dessa forma, a análise sugere a presença de três clusters principais dentro da topologia:

Cluster A: {h1, h2} – grupo coeso, mas distante dos demais;

Cluster B: {h3, h4} – grupo intermediário, com conexões moderadas a todos os demais clusters;

Cluster C: {h5, h6} – grupo coeso e relativamente isolado.


Essa estrutura de latências é um indicativo claro de segmentação dentro da rede, o que pode ter implicações diretas na alocação de
serviços distribuídos. Recomenda-se que tarefas de alta troca de mensagens sejam alocadas dentro de cada cluster, enquanto processos 
que exijam intercomunicação entre grupos considerem os hosts intermediários h3 e h4 como pontos estratégicos de integração.
