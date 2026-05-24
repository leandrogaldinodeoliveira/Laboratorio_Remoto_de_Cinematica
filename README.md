# Laboratório Remoto de Cinemática

Um sistema de aquisição de dados em tempo real para ensaios de cinemática unidimensional. Este projeto integra uma arquitetura de hardware e software para monitoramento remoto, permitindo a aferição da posição espacial de um corpo e a visualização dinâmica de sua série temporal através de uma interface web.

## 📌 Resumo do Projeto

O sistema utiliza um transdutor ultrassônico (HC-SR04) controlado por um microcontrolador (Arduino) para medir a distância de objetos em movimento. Os dados brutos são transmitidos via barramento serial para um servidor local em Node.js, que atua como um roteador de telemetria. Através do protocolo WebSockets, a interface gráfica (*front-end*) recebe e plota os dados a uma taxa de amostragem controlada, exibindo o valor em tempo real em um painel numérico dedicado e permitindo a exportação dos vetores de posição para análises estatísticas e físicas através de arquivos CSV.

## 🛠️ Arquitetura e Tecnologias

A infraestrutura foi dividida em três camadas principais:

* **Hardware (Aquisição):** Arduino (UNO/Nano), Transdutor Ultrassônico HC-SR04.
* **Backend (Servidor e Roteamento):** Node.js, Express, Socket.io, SerialPort.
* **Frontend (Visualização):** HTML5, EJS, CSS (Inter Font), Chart.js, Socket.io-client.

## ⚙️ Esquema de Ligações Físicas (Hardware)

Para a correta execução do firmware (`Projeto_final_Cinematica.ino`), o transdutor ultrassônico deve ser conectado seguindo o diagrama de pinagem abaixo:

* **VCC:** 5V do Arduino
* **GND:** GND do Arduino
* **TRIG (Disparo):** Pino Digital **9**
* **ECHO (Recepção):** Pino Digital **10**

> **Nota Técnica:** Recomenda-se o uso de um capacitor de **10 µF** entre os pinos `RESET` e `GND` da placa Arduino após a gravação do firmware para atenuar o sinal de DTR e evitar reinicializações indesejadas causadas pela abertura automática da porta serial pelo Node.js.

## 🚀 Instalação e Configuração do Ambiente

### 1. Preparação do Hardware

* Abra a IDE do Arduino e faça o upload do código contido em `Projeto_final_Cinematica.ino`.
* Verifique qual porta serial o Arduino está utilizando (ex: `COM3` no Windows ou `/dev/ttyUSB0` no Linux) e atualize a linha correspondente no arquivo `index.js`, se necessário:
```javascript
const port = new SerialPort({ path: 'COM3', baudRate: 9600 });

```


* Certifique-se de fechar o Monitor Serial da IDE do Arduino, pois a porta não pode ser compartilhada simultaneamente.

### 2. Configuração do Servidor (Node.js)

Certifique-se de ter o [Node.js](https://nodejs.org/) instalado em sua máquina. No terminal de comando, navegue até o diretório raiz do projeto e execute os seguintes passos:

1. Inicialize o manifesto do repositório (opcional, caso não possua o `package.json`):
```bash
npm init -y

```


2. Instale as bibliotecas e dependências estruturais do sistema:
```bash
npm install express ejs serialport socket.io

```


3. Inicie o servidor local de telemetria:
```bash
node index.js

```



### 3. Acesso à Interface

Com o servidor rodando em background, abra o navegador web de sua preferência e acesse o endereço local:
`http://localhost:3389`

## 📊 Metodologia de Uso e Coleta de Dados

O ensaio é inteiramente controlado pela interface web de forma síncrona, otimizando o consumo de banda de rede e evitando o transbordo do buffer serial (*buffer overflow*).

1. **Inicialização:** Clique no botão **LIGAR**. O servidor enviará instantaneamente o caractere de controle `T` via barramento, ativando a varredura do transdutor.
2. **Taxa de Amostragem:** O sistema opera sob uma base de tempo rigorosa de **$\Delta t$ = 500 ms** (**2 Hz**). A filtragem de ecos espúrios e ruídos de reflexão lateral é processada em nível de firmware.
3. **Monitoramento:** A interface renderiza graficamente a curva cinemática. Adicionalmente, um painel numérico de alta visibilidade fixado acima do gráfico exibe o valor numérico imediato da distância, eliminando a necessidade de consulta manual aos pontos da curva.
4. **Interrupção:** O ensaio possui um teto cronometrado de segurança de **10 minutos** para proteção do barramento. O operador pode interromper a amostragem a qualquer instante clicando no botão **DESLIGAR**.
5. **Exportação de Dados:** O conjunto de dados validados da sessão pode ser baixado localmente clicando em **BAIXAR DADOS**, gerando um arquivo `.csv` estruturado no formato padronizado `Tempo (s);Posicao (cm)`.

## 📄 Estrutura de Arquivos

```text
/
├── index.js                     # Servidor Node.js e orquestrador de WebSockets
├── package.json                 # Manifesto de dependências do ecossistema Node
├── Projeto_final_Cinematica.ino # Firmware C++ de aquisição de dados (Arduino)
└── views/
    └── index.ejs                # Interface gráfica, display digital e motor do Chart.js

```

## Autoria

Projeto desenvolvido para fins de pesquisa e instrumentação didática.
Autor: **Leandro Galdino de Oliveira**
