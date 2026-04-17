# Sistema Anti-Cheat Pangya (PoC)

Este projeto apresenta um conceito (*Proof of Concept*) de injecao e monitoramento ativo voltado para servidores do jogo Pangya. Ele interliga a DLL restrita do cliente (`ijl15.dll`) com a arquitetura C++ pura, validando integridade em tempo real atraves de um "Server-side Heartbeat".

## 🚀 Arquitetura do Sistema
O sistema e composto pelas sub-pecas:
1. **Pangya Client Hook (ijl15.dll)**: Escrito em C/WinAPI puro para injetar um Thread sem causar crash na inicializacao.
2. **Module Memory Scan**: Rotina assincrona para detectar mudancas em cheaters famosos em tempo real (Sempre Pangya, Alpha, Diametro, etc).
3. **Servidor Anti-Cheat (Heartbeat)**: Um servidor multi-thread para administrar validacoes baseadas em desafio-resposta entre as camadas Client e Server.
4. **Relatorio Banco de Dados**: A base para conectar no seu AuthServer/LoginServer futuramente e aplicar banimentos ou kicks em runtime.

## ⚙️ Fluxo do Desafio-Resposta (Challenge-Response)
1. Quando o jogo inicia o AntiCheat se esconde com a DLL de processamento nativa `ijl15`
2. O Client conecta via TCP no IP estipulado no banco do Heartbeat
3. A cada 15-300 segundos, o Servidor envia um **Timestamp Random** (O Desafio).
4. O Client une o status local (ex: `Limpo` ou `HACK_DETECTED`) com a String do Timeout e Encripta usando chaves mestras AES-256.
5. O Packet e retornado e descriptografado no Server, protegendo o sistema contra Replay Attacks!

---

> [!CAUTION] 
> ### INFORMACOES DE SEGURANCA E BOAS PRATICAS
> Este e um ambiente e arquitetura modelo. Se voce pretende rodar isso em linha de Producao para dezenas de jogadores com alta seguranca, tenha isso em pauta:
> - **Chaves Hardcoded**: A chave AES (`SECRET_KEY`) neste modelo de teste esta "Hardcoded" em texto puro para motivos de simulacao e aprendizado! Em ambiente de producao, NUNCA faca isso! As chaves nunca devem trafegar ou residir nativamente na sua DLL sem estar pelo menos ofuscada/polimorfizada em nivel Objeto, caso contrario hackers farao a extracao atravez de Assembly dumps (IDA/Ghidra).
> - **Exigencia de TLS Secure Connection**: Mesmo que o pacote esteja encriptado localmente, a ponte Windows Socket TCP requer conexao TLS (Transport Layer Security) para imunizar totalmente Sniffers e Handshakes falsos via Wireshark.
> - **Threads Isolados vs. Thread Hijacking**: Em um cenario global, injetores de memoria conseguem dropar "suspend thread" da arvore de processamento que nosso AntiCheat usa para espionar a memoria em runtime. Para uma escalabilidade de producao e ideal usar protecoes por Ring-0 (Kernel) ou validar com TLS Callback.

## 🛠 Como Compilar
Nao requer configuracao profunda por CMake. Utilizamos os compiladores puros da Microsoft atravez do **Visual Studio 2019/2022/2026 Developer Command Prompt**.

Para compilar, rode o `build_and_test.bat` presente neste repositorio, e ele automaticamente invoca o MSBuild ou o compilador nativo (VS/CL) e gera todos os artefatos `ijl15.dll`, `AntiCheatServer.exe` e executaveis de mock.

Sinta-se livre para ramificar e melhorar os monitoramentos!
