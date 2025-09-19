# README — Relatório Checagem MPI: "Filósofos"

**Versão:** 1.0  
**Data:** 2025-09-18

---

## Sumário

1. Resumo Executivo
2. Objetivos
3. Arquivos analisados
4. Como reproduzir
5. Erros observados
6. Diagnóstico e evidências
7. Causa raiz
8. Correções propostas e patches
9. Parser de logs (código)
10. Tratamento de CSV e leitura robusta
11. Análise de dados e suavização
12. Testes e validação
13. Recomendações e melhorias
14. Anexos / Apêndice

---

## 1. Resumo Executivo

Este README documenta toda a investigação sobre a execução do problema dos filósofos usando MPI, incluindo a reprodução do erro em `analise.py`, diagnóstico, causa raiz, correções, scripts para gerar CSV a partir dos logs e análise com visualização de dados.

---

## 2. Objetivos

- Reproduzir o erro ao rodar `analise.py`.
- Entender falhas na leitura de `filosofos.csv`.
- Criar parser robusto para gerar CSV a partir de logs.
- Aplicar correções e garantir leitura confiável.
- Gerar análise gráfica do percentual de filósofos alimentados e aplicar suavização.

---

## 3. Arquivos analisados

- `analise.py`  
- `filosofos.csv`  
- `log_filosofo.txt` (arquivo de logs)  
- `parser_logs.py` (proposto)

---

## 4. Como reproduzir

```bash
python3 analise.py
```
> Se ocorrer erro, verificar existência e caminho de `filosofos.csv`.

---

## 5. Erros observados

```
Traceback (most recent call last):
  File "analise.py", line 6, in <module>
    dados = pd.read_csv("filosofos.csv")
```
- Possíveis causas: arquivo ausente, encoding incorreto, delimitador inesperado.

---

## 6. Diagnóstico e evidências

1. Verificar diretório e existência: `ls -la filosofos.csv`  
2. Testar permissões: `stat filosofos.csv`  
3. Testar leitura manual: `pd.read_csv('filosofos.csv')`  
4. Testar diferentes encodings: 'utf-8', 'latin-1', 'cp1252'  
5. Gerar CSV do log caso não exista (ver seção 9)  
6. Validar formato e colunas: `df.head()` e `df.dtypes`

---

## 7. Causa raiz

- Arquivo não encontrado ou caminho incorreto.  
- Encoding problemático devido a acentos.  
- Estrutura CSV diferente do esperado.

---

## 8. Correções propostas e patches

```python
import pandas as pd
import argparse
from pathlib import Path

def load_csv_robusto(path):
    p = Path(path)
    if not p.exists():
        alt = Path(__file__).resolve().parent / path
        if alt.exists():
            p = alt
    if not p.exists():
        raise FileNotFoundError(f"Arquivo CSV não encontrado em: {path} ou {alt}")

    last_exc = None
    for enc in ('utf-8', 'latin-1', 'cp1252'):
        try:
            return pd.read_csv(p, encoding=enc)
        except Exception as e:
            last_exc = e
    raise last_exc

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--csv', default='filosofos.csv', help='Caminho para o CSV de entrada')
    args = parser.parse_args()
    dados = load_csv_robusto(args.csv)
```

---

## 9. Parser de logs (código)

```python
import re
import csv
from pathlib import Path

LOG = Path('log_filosofo.txt')
OUT = Path('filosofos.csv')

timestamp_re = re.compile(r"^===\s*(\d+)\s*===$")
action_re = re.compile(r"^Fil[oó]sofo\s+(\d+)\s+(.+?)\.?$", re.IGNORECASE)

rows = []
current_ts = None

with LOG.open('r', encoding='utf-8', errors='replace') as f:
    for line in f:
        line = line.strip()
        if not line:
            continue
        m_ts = timestamp_re.match(line)
        if m_ts:
            current_ts = int(m_ts.group(1))
            continue
        m_action = action_re.match(line)
        if m_action:
            pid = int(m_action.group(1))
            action = m_action.group(2).strip()
            rows.append({'timestamp': current_ts, 'philosopher': pid, 'action': action})

with OUT.open('w', encoding='utf-8', newline='') as csvf:
    fieldnames = ['timestamp', 'philosopher', 'action']
    writer = csv.DictWriter(csvf, fieldnames=fieldnames)
    writer.writeheader()
    writer.writerows(rows)

print(f"✅ CSV gerado: {OUT}")
```

---

## 10. Tratamento de CSV e leitura robusta

```python
import pandas as pd
p = Path('filosofos.csv')
df = pd.read_csv(p, encoding='utf-8')
print(df.head())
```

Exemplo de cálculo de percentual de alimentados:
```python
alimentado_mask = df['action'].str.contains('alimentou|comida|comendo|comeu', case=False, na=False)
group = df[alimentado_mask].groupby('timestamp')['philosopher'].nunique()
total_philosofers = 5
percent = (group / total_philosofers) * 100
percent = percent.reset_index().rename(columns={'philosopher': 'percent_fed'})
```

---

## 11. Análise de dados e suavização

### Spline (scipy)
```python
from scipy.interpolate import UnivariateSpline
import numpy as np
import matplotlib.pyplot as plt

x = percent['timestamp'].astype(float).values
y = percent['percent_fed'].values
idx = np.argsort(x)
x, y = x[idx], y[idx]
spline = UnivariateSpline(x, y, s=len(x))
xs = np.linspace(x.min(), x.max(), 500)
ys = spline(xs)
plt.plot(x, y, label='raw')
plt.plot(xs, ys, label='spline')
plt.legend()
plt.show()
```

### Média móvel (pandas)
```python
percent_sorted = percent.sort_values('timestamp')
percent_sorted['percent_fed_smoothed'] = percent_sorted['percent_fed'].rolling(window=5, center=True, min_periods=1).mean()
plt.plot(percent_sorted['timestamp'], percent_sorted['percent_fed'], label='raw')
plt.plot(percent_sorted['timestamp'], percent_sorted['percent_fed_smoothed'], label='smoothed')
plt.legend()
plt.show()
```

---

## 12. Testes e validação

- Unit test do parser com arquivo de teste.  
- Teste de integração: `python3 analise.py --csv filosofos.csv`  
- Edge cases: linhas vazias, timestamp ausente, acentos, pontuação extra.

---

## 13. Recomendações e melhorias

- Padronizar log (ISO-8601).  
- Usar formato JSONL.  
- Adicionar testes automatizados.  
- Adicionar CI/CD.  
- Instrumentar MPI com logs mais detalhados.

---

## 14. Anexos / Apêndice

- Comandos úteis:  
```bash
python3 parser_logs.py
python3 analise.py --csv filosofos.csv
```
- Checklist rápido: arquivo presente, CSV gerado, análise rodando, gráfico consistente.

