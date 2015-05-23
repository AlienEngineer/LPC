#include <I2C.h>
#include <string.h>
#include <PIN.h>

#define I2C_SDA0 27
#define I2C_SCL0 28
#define I2C_SDA1 0
#define I2C_SCL1 1
#define I2C_SDA2 10
#define I2C_SCL2 11

#define I2CONSET_I2EN				((uint8_t)0x40)
#define I2CONSET_STA				((uint8_t)0x20)
#define I2CONSET_STO				((uint8_t)0x10)
#define I2CONSET_SI					((uint8_t)0x08)
#define I2CONSET_AA					((uint8_t)0x04)
#define I2CONCLR_I2ENC				((uint8_t)0x40)
#define I2CONCLR_STAC				((uint8_t)0x20)
#define I2CONCLR_STOC				((uint8_t)0x10)
#define I2CONCLR_SIC				((uint8_t)0x08)
#define I2CONCLR_AAC				((uint8_t)0x04)

#define PINSEL_I2C0		0x3
#define PINSEL_I2C1		0x3
#define PINSEL_I2C2		0x2

I2C::I2C(LPC_I2C_TypeDef * i2c, uint32_t frequency) {
	this->Init(i2c, frequency);
}

I2C::I2C() {
}

void I2C::InitSdaScl(uint32_t sdaPin, uint32_t sclPin, uint32_t func) {
	PIN sda(0, sdaPin);
	PIN scl(0, sclPin);
	sda.SetFunction(func);
	scl.SetFunction(func);
}

void I2C::Init(LPC_I2C_TypeDef * i2c, uint32_t frequency) {
	this->i2c = i2c;
	this->freq = frequency;

	if (i2c == LPC_I2C0) {
		this->InitSdaScl(I2C_SDA0, I2C_SCL0, PINSEL_I2C0);
	} else if (i2c == LPC_I2C1) {
		this->InitSdaScl(I2C_SDA1, I2C_SCL1, PINSEL_I2C1);
	} else if (i2c == LPC_I2C2) {
		this->InitSdaScl(I2C_SDA2, I2C_SCL2, PINSEL_I2C2);
	}
}

void I2C::Init() {
	/* Apaga todas as flags do registo I2CON */
	this->i2c->I2CONCLR = (I2CONCLR_I2ENC | I2CONCLR_STAC | I2CONCLR_STOC | I2CONCLR_SIC | I2CONCLR_AAC);

	/* Activação da função I2C */
	this->i2c->I2CONSET |= I2CONSET_I2EN;
}

/**
 * @brief  Realiza uma transferência I2C com um periférico "escravo".
 * @param  addr: Endereço do periférico escravo.
 * @param  read: Se 1 é para ler da linha SDA, se 0 é para escrever na linha SDA.
 * @param  data: Referência para a zona de memória para escrita/leitura de dados.
 * @param  size: Dimensão em bytes dos dados envolvidos em operações de escrita/leitura.
 * @param  freq: Frequência com que os dados são transferidos.
 * @retval : Valores do registo I2CSTAT do controlador I2C definidos no manual do fabricante.
 */
uint32_t I2C::Transfer(uint8_t addr, uint8_t read, void *data, uint32_t size) {
	uint32_t i = 0;
	char buffer[size];

	/* Preparação do registo I2CON, para iniciar comunicação. */
	this->Init();
	/* Configuração do ritmo de comunicação. */
	this->i2c->I2SCLH = (SystemCoreClock) / (2 * this->freq);
	this->i2c->I2SCLL = (SystemCoreClock) / (2 * this->freq);
	/* Inicio da comunicação. */
	this->i2c->I2CONSET |= I2CONSET_STA; /* Envio do Start bit. */
	while (this->i2c->I2STAT != 0x8) {
	} /* Espera confirmação de envio do Start bit. */
	this->i2c->I2DAT = addr | read; /* Envio do byte de controlo de leitura/escrita. */
	this->i2c->I2CONCLR = (I2CONCLR_STAC | I2CONCLR_SIC); /* Apaga bits STA e SI do registo de controlo do I2C. */
	if (read) /* Leitura de dados. */
	{
		while (this->i2c->I2STAT != 0x40) {
		} /* Espera confirmação de envio do byte de controlo de leitura. */
		this->i2c->I2CONCLR = I2CONCLR_SIC;
		for (; i < size; ++i) {
			while ((this->i2c->I2CONCLR & 0x00000008) == 0) {
			} /* Espera pela flag SI (Dados provindos do Slave). */
			this->i2c->I2CONCLR = I2CONCLR_SIC;
			buffer[i] = this->i2c->I2DAT; /* Lê dados enviados pelo Slave. */
			this->i2c->I2DAT = 0x50; /* Envia ACK. */
			this->i2c->I2CONCLR = I2CONCLR_SIC;
		}
		this->i2c->I2DAT = 0x58; /* Envia NAK. */
		this->i2c->I2CONSET |= I2CONSET_STO; /* Envio  de Stop bit. */
		memcpy(data, (void*) buffer, size);
	} else if (!read) /* Escrita de comandos. */
	{
		memcpy((void*) buffer, data, size);
		while (this->i2c->I2STAT != 0x18) {
		} /* Espera confirmação de envio do byte de controlo de escrita. */
		for (; i < size; ++i) {
			this->i2c->I2DAT = buffer[i];
			this->i2c->I2CONCLR = I2CONCLR_SIC;
			while (this->i2c->I2STAT != 0x28) {
			}
		}
	}
	return this->i2c->I2STAT;
}
