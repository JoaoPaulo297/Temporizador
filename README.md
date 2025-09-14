Projeto em Arduino que implementa um temporizador digital com alarme, usando **LCD I2C**, **buzzer** e **botões de controle**.

---

## Descrição

O usuário define o tempo inicial no formato `hh:mm:ss` através do **Monitor Serial**.  
O sistema converte esse tempo em segundos e inicia a **contagem regressiva** exibida no LCD 16x2.

Durante a execução:

- **Botão PAUSE**: pausa ou retoma a contagem.
- **Botão STOP**: desliga o alarme quando o tempo termina.
- **Buzzer**: dispara quando o tempo chega a zero.
- Mensagens informativas são exibidas no LCD em cada estado.
