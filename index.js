const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const io = require('socket.io')(server);
const { SerialPort } = require('serialport');

const port = new SerialPort({ path: 'COM3', baudRate: 9600 });

app.engine('ejs', require('ejs').__express);
app.set('view engine', 'ejs');

app.get('/', (req, res) => res.render('index'));

io.on('connection', (socket) => {
    console.log('Cliente conectado.');

    socket.on('Ligar', () => {
        port.write('T');
        console.log('Comando T enviado.');
    });

    socket.on('Desligar', () => {
        port.write('F');
        console.log('Comando F enviado.');
    });

    // LEITURA BRUTA: Lê cada byte que chega
    port.on('data', (data) => {
        let msg = data.toString().trim();
        if (msg.length > 0) {
            console.log("Recebido via Serial:", msg);
            socket.emit('data', { valor: [msg] });
        }
    });
});

server.listen(3389, () => console.log("Servidor rodando na porta 3389"));