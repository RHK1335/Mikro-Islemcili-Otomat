#include "inc/tm4c123gh6pm.h"
#include <stdlib.h>
#include "driverlib/sysctl.h"
#include <stdbool.h>
#include <string.h>

#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"

#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#define BAUDRATE 600

#define LcdDataBits GPIO_PORTD_DATA_R

#define BIT_SET(PORT, PIN) do { \
        PORT |= 1<<PIN; \
} while (0)

#define BIT_RESET(PORT, PIN) do { \
        PORT &= ~(1<<PIN); \
} while (0)

int but1, but2, but3, but4, but5, but6;

int sayi1 = 0, sayi2 = 0, sayi3 = 0, sayi4 = 0, solbas = 0, sagbas = 0;
char bas1[50], bas2[50], bas3[50], bas4[50];
char stoksu[20], stokcay[20], stokbis[20], stokcik[20], stokkahve[20];
int as = 5;
int yb = 0;
int elli = 20;
int yuz = 10;
int ran, yedekpara;
int a = 0, b = 0, c = 0;

volatile unsigned long gecici;

void port_yukle() {
	SysCtlClockSet( //Hazir kutuphane kullanilarak zaman hesabi yapilir.
			SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
					| SYSCTL_OSC_MAIN); //16 M 1 saniye
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

	gecici = SYSCTL_RCGC2_R;

	GPIO_PORTD_DIR_R = 0xff;
	GPIO_PORTD_DEN_R = 0xff;

	GPIO_PORTE_DIR_R = 0xff;
	GPIO_PORTE_DEN_R = 0xff;

	GPIO_PORTA_DIR_R = 0xff;

	GPIO_PORTA_DEN_R = 0xff;

	GPIO_PORTF_DIR_R = 0xff;
	GPIO_PORTF_DEN_R = 0xff;

	GPIO_PORTB_DIR_R |= 0x00;
	GPIO_PORTB_PUR_R = 0xFF;
	GPIO_PORTB_AFSEL_R &= ~0x00;
	GPIO_PORTB_DEN_R |= 0xFF;

}

void lcd_ikili_yazdir(int x) {
	GPIO_PORTE_DATA_R = ((GPIO_PORTE_DATA_R & ~(1 << 1)) | (x << 1));
}

void EN(int x) {
	GPIO_PORTE_DATA_R = ((GPIO_PORTE_DATA_R & ~(1 << 2)) | (x << 2));
}

void resetEN() {
	EN(1);
	SysCtlDelay(500);
	EN(0);
}

void data(unsigned char c) {
	lcd_ikili_yazdir(1);
	SysCtlDelay(400);
	LcdDataBits = c >> 4;
	resetEN();
	LcdDataBits = c;
	resetEN();
}

void cmd(unsigned char c) {
	lcd_ikili_yazdir(0);
	SysCtlDelay(400);
	LcdDataBits = c >> 4;
	resetEN();
	LcdDataBits = c;
	resetEN();
}

void cmd_8bit(unsigned char c) {
	lcd_ikili_yazdir(0);
	SysCtlDelay(15000);
	LcdDataBits = (c);
	resetEN();
}

void temizle(void) {
	cmd(0x01);
	SysCtlDelay(30000);
}

void lcd_init() {
	cmd_8bit(0x30);
	cmd_8bit(0x30);
	cmd_8bit(0x30);
	cmd_8bit(0x20);
	cmd(0x28);
	cmd(0x0c);
	temizle();
	cmd(0x6);
}

void ekranyer(char x, char y) {
	if (x == 1)
		cmd(0x80 + ((y - 1) % 16));
	else
		cmd(0xC0 + ((y - 1) % 16));
}

void yazdir(char *p) {
	while (*p)
		data(*p++);
}
void para(int para) {
	sayi1 = 0;
	sayi2 = 0;
	sayi3 = 0;
	sayi4 = 0;
	if (para >= 1000) {
		sayi4 = para / 1000;
		para = para - sayi4 * 1000;
		sayi3 = (para) / 100;
		para = para - sayi3 * 100;
		sayi2 = (para) / 10;
		para = para - sayi2 * 10;
		sayi1 = (para) / 1;

		itoa(sayi4, bas4, 10);
		itoa(sayi3, bas3, 10);
		itoa(sayi2, bas2, 10);
		itoa(sayi1, bas1, 10);
		SysCtlDelay(800000);

		yazdir(bas4);
		yazdir(bas3);
		yazdir(".");
		yazdir(bas2);
		yazdir(bas1);
	} else if (para < 1000 && para >= 100) {

		sayi3 = (para) / 100;
		para = para - sayi3 * 100;
		sayi2 = (para) / 10;
		para = para - sayi2 * 10;
		sayi1 = (para) / 1;

		itoa(sayi3, bas3, 10);
		itoa(sayi2, bas2, 10);
		itoa(sayi1, bas1, 10);
		SysCtlDelay(800000);

		yazdir(bas3);
		yazdir(".");
		yazdir(bas2);
		yazdir(bas1);

	} else if (para < 100 && para >= 10) {

		sayi2 = (para) / 10;
		para = para - sayi2 * 10;
		sayi1 = (para) / 1;

		itoa(sayi2, bas2, 10);
		itoa(sayi1, bas1, 10);
		SysCtlDelay(800000);

		yazdir("0.");
		yazdir(bas2);
		yazdir(bas1);
	} else if (para < 10 && para >= 1) {

		sayi1 = (para) / 1;

		itoa(sayi1, bas1, 10);
		SysCtlDelay(800000);

		yazdir("0.0");
		yazdir(bas1);
	} else {
		ekranyer(1, 1);
		yazdir("hic para atmadiniz");
		ekranyer(2, 1);
		yazdir("00.00");

	}

}
void paraust(int para) {
	int x;
	x = para;
	a = 0;
	b = 0;
	c = 0;
	int gecicipara = x;

	char a1[20], b1[20], c1[20];
	if ((x / 100) >= 1) {
		a = x / 100;
		if (a <= yuz) {
			//a kadar 100 kr para üstü
			gecicipara = gecicipara - (a * 100);
			yuz = yuz - a;
			//  printf("%d\n",gecicipara);
		} else if (a > yuz) {
			//yuz kadar 100 kr para üstü
			gecicipara = gecicipara - (yuz * 100);
			a = yuz;
			yuz = 0;
		}
		if ((gecicipara / 50) >= 1) {
			b = gecicipara / 50;
			if (b <= elli) {
				//b kadar 50 kr para üstü
				gecicipara = gecicipara - (b * 50);
				elli = elli - b;
			} else if (b > elli) {
				//elli kadar para üstü
				gecicipara = gecicipara - (elli * 50);
				b = elli;
				elli = 0;
			}

			if ((gecicipara / 25) >= 1) {
				c = gecicipara / 25;
				if (c <= yb) {
					//c kadar 25 kr para üstü
					gecicipara = gecicipara - (c * 25);
					yb = yb - c;
				} else if (c > yb) {
					// yb kadar para üstü
					gecicipara = gecicipara - (yb * 25);
					c = yb;
					yb = 0;
				}

			}

		} else if ((gecicipara / 50) < 1) {

			if ((gecicipara / 25) >= 1) {
				c = gecicipara / 25;
				if (c <= yb) {
					//c kadar 25 kr para üstü
					gecicipara = gecicipara - (c * 25);
					yb = yb - c;
				} else if (c > yb) {
					// yb kadar para üstü
					gecicipara = gecicipara - (yb * 25);
					c = yb;
					yb = 0;
				}

			}

		}
	} else if ((x / 100) < 1) {
		if ((gecicipara / 50) >= 1) {
			b = gecicipara / 50;
			if (b <= elli) {
				//b kadar 50 kr para üstü
				gecicipara = gecicipara - (b * 50);
				elli = elli - b;
			} else if (b > elli) {
				//elli kadar para üstü
				gecicipara = gecicipara - (elli * 50);
				b = elli;
				elli = 0;
			}

			if ((gecicipara / 25) >= 1) {

				c = gecicipara / 25;
				if (c <= yb) {
					//c kadar 25 kr para üstü
					gecicipara = gecicipara - (c * 25);
					yb = yb - c;
				} else if (c > yb) {
					// yb kadar para üstü
					gecicipara = gecicipara - (yb * 25);
					c = yb;
					yb = 0;

				}

			}

		} else if ((gecicipara / 50) < 1) {

			if ((gecicipara / 25) >= 1) {
				c = gecicipara / 25;
				if (c <= yb) {
					//c kadar 25 kr para üstü
					gecicipara = gecicipara - (c * 25);
					yb = yb - c;
				} else if (c > yb) {
					// yb kadar para üstü
					gecicipara = gecicipara - (yb * 25);
					c = yb;
					yb = 0;
				}

			}
		}
	}
	temizle();
	itoa(a, a1, 10);
	itoa(b, b1, 10);
	itoa(c, c1, 10);

	yazdir("1 tl :");
	yazdir(a1);
	yazdir(" 50 kr :");
	yazdir(b1);
	ekranyer(2, 1);
	yazdir("25 kr :");
	yazdir(c1);
	SysCtlDelay(3800000);

}

struct urunler {
	int UrunID;
	char UrunAd[20];
	int UrunStokSayisi;
	int UrunFiyat;
} urunler1[5];

void init_UARTstdio() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(0x00000001);
	GPIOPinConfigure(0x00000401);
	GPIOPinTypeUART(0x40004000, 0x00000001 | 0x00000002);
	UARTConfigSetExpClk(0x40004000, SysCtlClockGet(), BAUDRATE,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
			UART_CONFIG_PAR_NONE));
	UARTStdioConfig(0, BAUDRATE, SysCtlClockGet());
}

int main() {
	int kasa = yb * 25 + elli * 50 + yuz * 100;
	port_yukle();

	lcd_init();
	temizle();
	SysCtlDelay(8000000);
	int mani = 0, bir, yarim, ceyrek, sayac = 0;
	int kahve = 0, su = 0, cay = 0, bis = 0, cik = 0;
	char oku[20];

	init_UARTstdio();
	UARTprintf("%d", sayac);

	urunler1[0].UrunID = 1;
	strcpy(urunler1[0].UrunAd, "su");
	urunler1[0].UrunStokSayisi = 30;
	urunler1[0].UrunFiyat = 50;

	urunler1[1].UrunID = 2;
	strcpy(urunler1[1].UrunAd, "cay");
	urunler1[1].UrunStokSayisi = 2;
	urunler1[1].UrunFiyat = 100;

	urunler1[2].UrunID = 3;
	strcpy(urunler1[2].UrunAd, "kahve");
	urunler1[2].UrunStokSayisi = 15;
	urunler1[2].UrunFiyat = 150;

	urunler1[3].UrunID = 4;
	strcpy(urunler1[3].UrunAd, "cikolata");
	urunler1[3].UrunStokSayisi = 50;
	urunler1[3].UrunFiyat = 175;

	urunler1[4].UrunID = 5;
	strcpy(urunler1[4].UrunAd, "biskuvi");
	urunler1[4].UrunStokSayisi = 100;
	urunler1[4].UrunFiyat = 200;
	res: temizle();
	mani = 0;
	sayac = 0;
	su = 0;
	cay = 0;
	kahve = 0;
	cik = 0;
	bis = 0;
	ceyrek = 0;
	yarim = 0;
	bir = 0;



	ekranyer(1, 1);
	yazdir("para giriniz");
	while (1) {

		but1 = GPIO_PORTB_DATA_R & 0x01;
		but2 = GPIO_PORTB_DATA_R & 0x02;
		but3 = GPIO_PORTB_DATA_R & 0x04;
		but4 = GPIO_PORTB_DATA_R & 0x08;
		but5 = GPIO_PORTB_DATA_R & 0x10;
		but6 = GPIO_PORTB_DATA_R & 0x20;


		itoa(urunler1[0].UrunStokSayisi, stoksu, 10);
		itoa(urunler1[1].UrunStokSayisi, stokcay, 10);
		itoa(urunler1[2].UrunStokSayisi, stokkahve, 10);
		itoa(urunler1[3].UrunStokSayisi, stokcik, 10);
		itoa(urunler1[4].UrunStokSayisi, stokbis, 10);

		if (sayac == 0) {

			if (but1 == 0) {
				temizle();
				mani += 25;
				ceyrek++;
				yazdir("25 kurus atildi");
				SysCtlDelay(800000);
				ekranyer(2, 1);
				para(mani);
				GPIO_PORTF_DATA_R = 0x08;
				SysCtlDelay(3800000);
				yb++;
				GPIO_PORTF_DATA_R &= 0x00;

			}
			if (but2 == 0) {
				temizle();
				mani += 50;
				yazdir("50 kurus atildi");
				SysCtlDelay(800000);
				ekranyer(2, 1);
				para(mani);
				yarim++;
				elli++;
				GPIO_PORTF_DATA_R = 0x08;
				SysCtlDelay(3800000);

				GPIO_PORTF_DATA_R &= 0x00;
			}
			if (but3 == 0) {
				temizle();
				mani += 100;
				yazdir("1 tl atildi");
				SysCtlDelay(800000);
				ekranyer(2, 1);
				para(mani);
				GPIO_PORTF_DATA_R = 0x08;
				SysCtlDelay(3800000);
				yuz++;
				GPIO_PORTF_DATA_R &= 0x00;

				bir++;
			}
			if (but4 == 0) {
				temizle();

				para(mani);

				SysCtlDelay(32000000);
				sayac++;
				temizle();
				yazdir(urunler1[0].UrunAd);

				yazdir(stoksu);
				yazdir(" ");
				yazdir(urunler1[1].UrunAd);

				yazdir(stokcay);
				yazdir(" ");
				yazdir("kah");

				yazdir(stokkahve);
				ekranyer(2, 1);
				yazdir("cik");

				yazdir(stokcik);
				yazdir(" ");
				yazdir("bis");

				yazdir(stokbis);
				GPIO_PORTF_DATA_R = 0x08;
				SysCtlDelay(35000000);

				GPIO_PORTF_DATA_R &= 0x00;

			}

		} else if (sayac == 1) {
			temizle();

			yazdir("secim yapiniz");
			SysCtlDelay(800000);

			if (but1 == 0) {
				temizle();
				su++;

				yazdir("su secildi");
				SysCtlDelay(3200000);

				if (urunler1[0].UrunStokSayisi < su) {
					temizle();
					yazdir("su stokta yok !!");
					GPIO_PORTE_DATA_R = 0x02;
					SysCtlDelay(6400000);
					GPIO_PORTF_DATA_R &= 0x00;
					su--;

				} else {
					GPIO_PORTF_DATA_R = 0x08;
					SysCtlDelay(3800000);

					GPIO_PORTF_DATA_R &= 0x00;
				}

			}
			if (but2 == 0) {
				temizle();
				yazdir("cay secildi");
				cay++;
				SysCtlDelay(3200000);

				if (urunler1[1].UrunStokSayisi < cay) {
					temizle();
					yazdir("cay stokta yok !!");
					GPIO_PORTF_DATA_R = 0x02;
					SysCtlDelay(6400000);
					GPIO_PORTF_DATA_R &= 0x00;
					cay--;

				} else {
					GPIO_PORTF_DATA_R = 0x08;
					SysCtlDelay(3800000);

					GPIO_PORTF_DATA_R &= 0x00;
				}

			}
			if (but3 == 0) {
				temizle();
				yazdir("kahve secildi");
				kahve++;
				SysCtlDelay(3200000);

				if (urunler1[2].UrunStokSayisi < kahve) {
					temizle();
					yazdir("kahve stokta yok !!");
					GPIO_PORTF_DATA_R = 0x02;
					SysCtlDelay(6400000);
					GPIO_PORTF_DATA_R &= 0x00;
					kahve--;

				} else {
					GPIO_PORTF_DATA_R = 0x08;
					SysCtlDelay(3800000);

					GPIO_PORTF_DATA_R &= 0x00;
				}

			}
			if (but4 == 0) {
				temizle();
				yazdir("cikolata secildi");
				cik++;
				SysCtlDelay(3200000);

				if (urunler1[3].UrunStokSayisi < cik) {
					temizle();
					yazdir("cikolata stokta yok !!");
					GPIO_PORTF_DATA_R = 0x02;
					SysCtlDelay(6400000);
					GPIO_PORTF_DATA_R &= 0x00;
					cik--;

				} else {
					GPIO_PORTF_DATA_R = 0x08;
					SysCtlDelay(3800000);

					GPIO_PORTF_DATA_R &= 0x00;
				}

			}
			if (but5 == 0) {
				temizle();
				yazdir("biskuvi secildi");
				bis++;
				SysCtlDelay(3200000);

				if (urunler1[4].UrunStokSayisi < bis) {
					temizle();
					yazdir("biskuvi stokta yok !!");
					GPIO_PORTF_DATA_R = 0x02;
					SysCtlDelay(6400000);
					GPIO_PORTF_DATA_R &= 0x00;
					bis--;

				} else {
					GPIO_PORTF_DATA_R = 0x08;
					SysCtlDelay(3800000);

					GPIO_PORTF_DATA_R &= 0x00;
				}
			}
			if (but6 == 0) {
				temizle();
				yazdir("secimler bitti");
				sayac++;

				GPIO_PORTF_DATA_R = 0x08;
				SysCtlDelay(3800000);
				SysCtlDelay(6400000);

				GPIO_PORTF_DATA_R &= 0x00;

			}





		} else if (sayac == 2) {
			break;
		}
	}
	kasa = yb * 25 + elli * 50 + yuz * 100;

	yedekpara = mani;
	mani = mani
			- ((su * 50) + (cay * 100) + (kahve * 150) + (cik * 175)
					+ (bis * 200));
	ran = 1;

	if (ran == 2) {

		yb -= ceyrek;
		elli -= yarim;
		yuz -= bir;

		GPIO_PORTF_DATA_R = 0x02;
		temizle();
		yazdir("para sikismasi");
		ekranyer(2, 1);
		yazdir("para iade ediliyor");
		SysCtlDelay(40000000);
		temizle();
		yazdir("iade tutari :");

		ekranyer(2, 1);
		para(yedekpara);
		SysCtlDelay(40000000);

		GPIO_PORTF_DATA_R &= 0x00;

	} else if (mani < 0) {
		yb -= ceyrek;
		elli -= yarim;
		yuz -= bir;
		GPIO_PORTF_DATA_R = 0x02;

		temizle();

		yazdir("Yetersiz Bakiye");
		ekranyer(2, 1);
		yazdir("para iade ediliyor");
		SysCtlDelay(4000000);
		SysCtlDelay(40000000);
		temizle();
		yazdir("iade tutari :");
		SysCtlDelay(800000);
		ekranyer(2, 1);
		para(yedekpara);

		SysCtlDelay(40000000);
		GPIO_PORTF_DATA_R &= 0x00;
	} else {
		paraust(mani);

		urunler1[0].UrunStokSayisi -= su;
		urunler1[1].UrunStokSayisi -= cay;
		urunler1[2].UrunStokSayisi -= kahve;
		urunler1[3].UrunStokSayisi -= cik;
		urunler1[4].UrunStokSayisi -= bis;
		GPIO_PORTF_DATA_R |= 0x08;

		SysCtlDelay(40000000);
		SysCtlDelay(3200000);
		GPIO_PORTF_DATA_R &= 0x00;

	}

	SysCtlDelay(4000000);
	temizle();
	kasa = yb * 25 + elli * 50 + yuz * 100;
	yazdir("kasa : ");
	para(kasa);

	SysCtlDelay(40000000);
	goto res;
}

