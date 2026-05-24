# Laboratório Remoto de Cinemática

Um sistema de aquisição de dados em tempo real para ensaios de cinemática unidimensional. Este projeto integra uma arquitetura de hardware e software para monitoramento remoto, permitindo a leitura da posição espacial de um corpo e a visualização dinâmica de sua série temporal através de uma interface web.

## 📌 Resumo do Projeto

O sistema utiliza um transdutor ultrassônico (HC-SR04) controlado por um microcontrolador (Arduino) para medir a distância de objetos em movimento. Os dados brutos são transmitidos via barramento serial para um servidor local em Node.js, que atua como um roteador de telemetria. Através do protocolo WebSockets, a interface gráfica (*front-end*) recebe e plota os dados a uma taxa de amostragem controlada, permitindo a exportação dos vetores de posição para análises estatísticas e físicas (CSV).

## 🛠️ Arquitetura e Tecnologias

A infraestrutura foi dividida em três camadas principais:

* **Hardware (Aquisição):** Arduino UNO/Nano, Transdutor Ultrassônico HC-SR04.
* **Backend (Servidor e Roteamento):** Node.js, Express, Socket.io, SerialPort.
* **Frontend (Visualização):** HTML5, EJS, CSS (Inter Font), Chart.js, Socket.io-client.

## ⚙️ Esquema de Ligações Físicas (Hardware)

Para a correta execução do firmware (`Projeto_final_Cinematica.ino`), o transdutor ultrassônico deve ser conectado seguindo o diagrama de pinagem abaixo:

* **VCC:** 5V do Arduino
* **GND:** GND do Arduino
* **TRIG (Disparo):** Pino Digital **9**
* **ECHO (Recepção):** Pino Digital **10**

> **Nota Técnica:** Recomenda-se o uso de um capacitor de **10 µF** entre os pinos `RESET` e `GND` da placa Arduino após a gravação do firmware para evitar reinicializações indesejadas causadas pela abertura da porta serial pelo Node.js.

## 🚀 Instalação e Configuração do Ambiente

### 1. Preparação do Hardware

* Abra a IDE do Arduino e faça o upload do código contido em `Projeto_final_Cinematica.ino`.
* Verifique qual porta serial o Arduino está utilizando (ex: `COM3` no Windows ou `/dev/ttyUSB0` no Linux) e atualize a linha correspondente no arquivo `index.js`, se necessário:
```javascript
const port = new SerialPort({ path: 'COM3', baudRate: 9600 });

```


* Feche o Monitor Serial da IDE (a porta não pode estar em uso).

### 2. Preparação do Servidor (Node.js)

Certifique-se de ter o [Node.js](https://nodejs.org/) instalado. No terminal, navegue até a pasta raiz do projeto e execute:

```bash
# Instala as dependências listadas no package.json
npm install

# Inicia o servidor local
node index.js

```

### 3. Acesso à Interface

Com o servidor rodando, abra o navegador web de sua preferência e acesse:
`http://localhost:3389`

## 📊 Metodologia de Uso e Coleta de Dados

O ensaio é inteiramente controlado pela interface web, garantindo segurança na alocação da porta serial e evitando saturação de rede (*buffer overflow*).

1. **Inicialização:** Clique no botão **LIGAR**. O servidor enviará um sinal lógico ao microcontrolador, que iniciará o disparo de pulsos ultrassônicos.
2. **Taxa de Amostragem:** O sistema está configurado para uma base de tempo rigorosa de **$\Delta t$ = 500 ms** (**2 Hz**). A filtragem de ecos espúrios é realizada diretamente no firmware.
3. **Monitoramento:** A interface renderizará dinamicamente a posição do objeto no gráfico. Uma janela flutuante informará a grandeza escalar atual.
4. **Interrupção e Exportação:** O ensaio possui um teto estrutural de segurança de **10 minutos**. O operador pode abortar a coleta a qualquer instante clicando em **DESLIGAR**.
5. **Download:** O conjunto de dados validados pode ser exportado clicando em **BAIXAR DADOS**, gerando um arquivo `.csv` no formato `Tempo (s); Posição (cm)`.

## 📄 Estrutura de Arquivos

```text
/
├── index.js                     # Servidor Node.js e orquestrador de WebSockets
├── package.json                 # Manifesto de dependências do projeto
├── Projeto_final_Cinematica.ino # Firmware C++ de aquisição de dados (Arduino)
└── views/
    └── index.ejs                # Interface de controle e renderização do Chart.js

```

## 📝 Licença e Autoria

Projeto desenvolvido para fins de pesquisa e instrumentação didática.
Autor: **Leandro Galdino de Oliveira**
