---
description: Query the Graphify RAG system to understand the PvZ codebase
---
# Graphify RAG Workflow

This workflow uses the local Graphify knowledge graph (`graphify-out/graph.json`) to analyze the Plants vs. Zombies codebase.

## Usage
When the user asks to query or explain a class, method, or subsystem in the PvZ codebase using "graphify", use this tool.

1. **Broad queries** (What is X connected to?)
```bash
// turbo
/home/nfg/.local/share/pipx/venvs/graphifyy/bin/python -m graphify query "{{query}}" "/home/nfg/İndirilenler/Updated Base/graphify-out/graph.json"
```

2. **Trace queries** (How does A reach B?)
```bash
// turbo
/home/nfg/.local/share/pipx/venvs/graphifyy/bin/python -m graphify path "{{start}}" "{{end}}" "/home/nfg/İndirilenler/Updated Base/graphify-out/graph.json"
```

3. **Deep-dive queries** (Explain what this node does)
```bash
// turbo
/home/nfg/.local/share/pipx/venvs/graphifyy/bin/python -m graphify explain "{{node}}" "/home/nfg/İndirilenler/Updated Base/graphify-out/graph.json"
```

## Rebuilding the Graph (Post-Modification)
After modifying code files, the graph should be updated to keep it current.
```bash
// turbo
/home/nfg/.local/share/pipx/venvs/graphifyy/bin/python -c "from graphify.watch import _rebuild_code; from pathlib import Path; _rebuild_code(Path('/home/nfg/İndirilenler/Updated Base'))"
```
