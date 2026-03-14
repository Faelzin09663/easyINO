# Publicação (PyPI + Arduino)

Este projeto tem **dois artefatos**:

- **Python (PyPI)**: pacote `easyino`
- **Arduino**: biblioteca `EasyIno` em `arduino/EasyIno/`

## 1) Publicar no PyPI

### Checklist

- Atualize a versão em `pyproject.toml` (`[project].version`)
- Garanta que o `README.md` descreve o uso

### Build

```bash
python -m pip install --upgrade build twine
python -m build
python -m twine check dist/*
```

### Upload

```bash
python -m twine upload dist/*
```

> Dica: use o TestPyPI antes (opcional):
> `python -m twine upload --repository testpypi dist/*`

## 2) Publicar a lib Arduino

Para o Arduino Library Manager, normalmente o **repositório** é o pacote da lib.
Como aqui você também tem o pacote Python, a forma mais simples é publicar a pasta
`arduino/EasyIno` em um repositório separado (ex: `EasyIno-Arduino`) e manter:

- `library.properties`
- `src/`
- `examples/`
- `README.md`

Depois, você cria uma release/tag (ex: `0.1.0`) e solicita inclusão no índice do Arduino.

## Estrutura final (referência)

```
easy_ino/
  easyino/                 # Python package (PyPI)
  examples/python/         # Python examples
  arduino/EasyIno/         # Arduino library
    src/
    examples/
    library.properties
    README.md
```

