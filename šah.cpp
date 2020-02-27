#pragma warning(disable : 4996)
#pragma warning(disable : 6031)
#define STRICT
#include <string>
#include <vector>
#include <dos.h>
#include <iostream>
#include <fstream>
#include <strsafe.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <shlwapi.h>
#include <fcntl.h>
#include <windows.h>
#include <cwchar>
#include <winreg.h>
#include <Wincon.h>
#include <cstdlib>
#define _O_U8TEXT	0x40000
#define SVETLO 0
#define TAMNO 1
#define SIVO 2
#define BELO 0
#define CRNO 1
#define ISKLJUCENO 0
#define UKLJUCENO 1
#define ZELENO 2
#define NEUTRALNO 0
#define IZABRANO 1
#define NAPADNUTO 2
// ●  ꞏ ⚫

using namespace std;

const wchar_t	TOP[] = L"♜ ",
KONJ[] = L"♞ ",
LOVAC[] = L"♝ ",
KRALJICA[] = L"♛ ",
KRALJ[] = L"♚ ",
PIJUN[] = L"♟ ",
PRAZNO[] = L"  ",
TACKA[] = L"";

int a, b;
bool EXITind = 0;
bool redo = 0;
short boja = SVETLO;
short rotacija = ISKLJUCENO;
short sacekaj;
short velicina = 29;
long brPoteza = 0;
long brPotezaMax = 0;
long winlog;
CONSOLE_CURSOR_INFO kursor;
HANDLE ConsoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE ConsoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);

typedef struct {
	short bojaPolja;
	short stanjePolja;
	wchar_t figura[4];
	short bojaFigure;
	short anpasan;
} Polje;

typedef struct {
	Polje polje[8][8];
	Polje levo[8][2];
	Polje desno[8][2];
	short brlevo;
	short brdesno;
	short malarbeli;
	short velikarbeli;
	short malarcrni;
	short velikarcrni;
	short krajigre;

} Tabla;

vector<Tabla> istorija(0);

void SakriKursor();
void ObrisiEkran();
void gotoxy(int x, int y);
void UcitajKlik(int& x, int& y);
void CentrirajKonzolu();
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
void SacuvajConsoleKey();
void OsveziTablu(Tabla tabla);
void PodesiKonzolu();
void NapraviTablu(Tabla& tabla);
void OdstampajTablu(Tabla tabla);
void OdstampajKonzolu();
void PozicijaNaTabli(int& x, int& y);
void NoviPotez(Tabla& tabla);
void PozicijaIzabranog(int& xizb, int& yizb, Tabla tabla);
short JelUnutarTable(int x, int y);
void OdstampajLevoDesno(Tabla tabla);
void ObrnutoOdstampajTablu(Tabla tabla);
void ObrnutaPozicijaNaTabli(int& x, int& y);
void ObrisiAnpasan(Tabla& tabla);
void PodesiVelicinu(int i);
void ObrisiTerminal();
Tabla StaviFiguruNaXY(Tabla tabla, int x, int y);
short JelSahBeli(Tabla tabla, int xpomeraj, int ypomeraj);
short JelSahCrni(Tabla tabla, int xpomeraj, int ypomeraj);
short PostaviKralja(Tabla& tabla, int x, int y);
short PostaviPijuna(Tabla& tabla, int x, int y);
short PostaviKonja(Tabla& tabla, int x, int y);
short PostaviTopa(Tabla& tabla, int x, int y);
short PostaviLovca(Tabla& tabla, int x, int y);
short PostaviKraljicu(Tabla& tabla, int x, int y);
short JelMozeIgratiBeli(Tabla tabla);
short JelMozeIgratiCrni(Tabla tabla);
short ZameniPijuna();
int FiguraUBroj(wchar_t figura[]);
const wchar_t* BrojUFiguru(int broj);
vector<short> NapraviSacuvaj();
void SacuvajIgru();
void OdaberiVelicinu();
//void AnimacijaUcitaj();
BOOL WINAPI EXIT(DWORD CEvent);

int main() {
	system("MODE 16, 1");
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)EXIT, TRUE);
	SacuvajConsoleKey();

	Tabla tabla;
	NapraviTablu(tabla);
	istorija.push_back(tabla);

	//AnimacijaUcitaj();

	PodesiKonzolu();
	CentrirajKonzolu();
	OdstampajKonzolu();
	OdstampajTablu(tabla);

	int x, y, xizb, yizb, i = 35;

	while (1) {
		if (brPoteza % 2 == 1 && !JelMozeIgratiCrni(tabla) && JelSahCrni(tabla, 0, 0)) {
			if (boja == SVETLO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 82);
			else if (boja == TAMNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 101);
			else if (boja == SIVO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 152);

			tabla.krajigre = 1;
			gotoxy(10, 14);
			wprintf(L"Beli je pobednik");
		}
		else if (brPoteza % 2 == 0 && !JelMozeIgratiBeli(tabla) && JelSahBeli(tabla, 0, 0)) {
			if (boja == SVETLO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 82);
			else if (boja == TAMNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 101);
			else if (boja == SIVO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 152);

			tabla.krajigre = 1;
			gotoxy(10, 14);
			wprintf(L"Crni je pobednik");
		}
		else if (brPoteza % 2 == 1 && !JelMozeIgratiCrni(tabla)) {
			if (boja == SVETLO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 82);
			else if (boja == TAMNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 101);
			else if (boja == SIVO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 152);

			tabla.krajigre = 1;
			gotoxy(8, 14);
			wprintf(L"Beli je napravio pat");
		}
		else if (brPoteza % 2 == 0 && !JelMozeIgratiBeli(tabla)) {
			if (boja == SVETLO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 82);
			else if (boja == TAMNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 101);
			else if (boja == SIVO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 152);

			tabla.krajigre = 1;
			gotoxy(8, 14);
			wprintf(L"Crni je napravio pat");
		}

		sacekaj = ISKLJUCENO;

		UcitajKlik(x, y);
		ObrisiTerminal();

		if (y == 0 && (x == 4 || x == 5)) {
			NapraviTablu(tabla);
			istorija.clear();
			istorija.push_back(tabla);
			brPoteza = 0;
			NoviPotez(tabla);
			ObrisiTerminal();

			OsveziTablu(tabla);
		}
		else if (y == 0 && (x == 0 || x == 1)) {
			SacuvajIgru();
		}
		else if (y == 0 && (x == 2 || x == 3)) {
			wchar_t ime[18], zn;
			short pom;
			ObrisiTerminal();
			SetConsoleCursorInfo(ConsoleOutputHandle, &kursor);

			if (boja == SVETLO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 82);
			else if (boja == TAMNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 101);
			else if (boja == SIVO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 152);

			_setmode(_fileno(stdout), _O_TEXT);

			gotoxy(4, 14);
			printf("Ime igre:  ");

			bool jednom = 1;

			for (int i = 0; i < 18; i++) {
				fflush(stdin);
				zn = _getwch();
				fflush(stdin);

				if ((zn == 13 && i != 0) || i == 14) {
					Sleep(100);
					SakriKursor();
					ime[i] = '\0';
					break;
				}
				else if (zn == 13) {
					jednom = 0;
					break;
				}
				else if (zn == 8) {
					gotoxy(14 + i, 14);
					_putwch(' ');
					gotoxy(14 + i, 14);
					i -= 2;
				}
				else {
					gotoxy(15 + i, 14);
					_putwch(zn);
					ime[i] = zn;
				}
			}

			ObrisiTerminal();
			SakriKursor();

			if (jednom) {
				ifstream ulaz(wcscat(ime, L".igra"));

				if (ulaz.is_open()) {
					ulaz >> brPoteza;

					istorija.clear();

					for (int a = 0; a <= brPoteza; a++) {
						ulaz >> tabla.brlevo;
						ulaz >> tabla.brdesno;
						ulaz >> tabla.krajigre;
						ulaz >> tabla.malarbeli;
						ulaz >> tabla.malarcrni;
						ulaz >> tabla.velikarbeli;
						ulaz >> tabla.velikarcrni;

						for (int i = 0; i < 16; i++) {
							ulaz >> pom;
							wcscpy(tabla.levo[i / 2][i % 2].figura, BrojUFiguru(pom));
							ulaz >> pom;
							wcscpy(tabla.desno[i / 2][i % 2].figura, BrojUFiguru(pom));
						}

						for (int i = 0; i < 8; i++)
							for (int j = 0; j < 8; j++) {
								ulaz >> tabla.polje[i][j].anpasan;
								ulaz >> tabla.polje[i][j].bojaFigure;
								ulaz >> pom;
								wcscpy(tabla.polje[i][j].figura, BrojUFiguru(pom));
								ulaz >> tabla.polje[i][j].stanjePolja;
							}

						istorija.push_back(tabla);
					}

					ulaz.close();
				}
				else {
					gotoxy(4, 14);

					if (boja == SVETLO)
						SetConsoleTextAttribute(ConsoleOutputHandle, 82);
					else if (boja == TAMNO)
						SetConsoleTextAttribute(ConsoleOutputHandle, 101);
					else if (boja == SIVO)
						SetConsoleTextAttribute(ConsoleOutputHandle, 152);

					_setmode(_fileno(stdout), _O_U8TEXT);

					wprintf(L"Ne postoji igra sa tim imenom");
				}
				OsveziTablu(tabla);
			}
		}
		else if (y == 0 && (x == 6 || x == 7)) {
			boja++;

			if (boja == 3)
				boja = 0;

			OdstampajKonzolu();
			NoviPotez(tabla);

			OsveziTablu(tabla);
		}
		else if (y == 0 && (x == 8 || x == 9)) {
			short pom = velicina;
			OdaberiVelicinu();

			if (pom != velicina) {
				PodesiVelicinu(velicina);
				OdstampajKonzolu();
				OsveziTablu(tabla);
			}
		}
		else if (y == 0 && (x == 10 || x == 11) && brPoteza > 0) {
			tabla = istorija[brPoteza - (long long)1];
			if (brPotezaMax < brPoteza)
				brPotezaMax = brPoteza;
			brPoteza--;
			redo = 1;
			ObrisiTerminal();

			OsveziTablu(tabla);
		}
		else if (y == 0 && (x == 12 || x == 13) && brPotezaMax > 0 && redo && brPotezaMax > brPoteza) {
			tabla = istorija[brPoteza + (long long)1];
			brPoteza++;
			ObrisiTerminal();

			OsveziTablu(tabla);
		}
		else if (y == 0 && (x == 32 || x == 33)) {
			if (rotacija == UKLJUCENO) {
				rotacija = ISKLJUCENO;

				if (boja == SVETLO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 36);
				else if (boja == TAMNO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 87);
				else if (boja == SIVO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 138);

				gotoxy(32, 0);
				wprintf(L"");
			}
			else {
				rotacija = UKLJUCENO;

				if (boja == SVETLO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 36);
				else if (boja == TAMNO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 87);
				else if (boja == SIVO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 138);

				gotoxy(32, 0);
				wprintf(L"");
			}

			NoviPotez(tabla);

			OsveziTablu(tabla);
		}
		else if (y == 0 && (x == 34 || x == 35)) {
			int a, b;
			SetConsoleTextAttribute(ConsoleOutputHandle, 255);

			for (int i = 0; i < 18; i++)
				wprintf(L"\n");

			SetConsoleTextAttribute(ConsoleOutputHandle, 241);
			gotoxy(0, 1);
			wprintf(L"\t\tŠAH");
			wprintf(L"\n\n\n  Autor:");
			wprintf(L"\n\n  Momir Stanišić,");
			wprintf(L"\n  Terminal Game Developer");
			wprintf(L"\n\n\n\n  Verzija: 1.2.4");

			UcitajKlik(a, b);

			OdstampajKonzolu();
			OdstampajTablu(tabla);
		}
		else if (y > 2 && y < 11 && x > 9 && x < 26 && tabla.krajigre == 0 && JelMozeIgratiBeli(tabla) && JelMozeIgratiBeli(tabla)) {
			if (brPoteza % 2 && rotacija == UKLJUCENO && JelMozeIgratiCrni(tabla))
				ObrnutaPozicijaNaTabli(x, y);
			else
				PozicijaNaTabli(x, y);

			if (tabla.polje[y][x].stanjePolja == NAPADNUTO && wcscmp(tabla.polje[y][x].figura, KRALJ)) {
				PozicijaIzabranog(xizb, yizb, tabla);

				if (tabla.polje[yizb][xizb].bojaFigure == BELO && tabla.brlevo < 16) {
					if (tabla.polje[y + 1][x].anpasan == UKLJUCENO) {
						wcscpy(tabla.levo[tabla.brlevo / 2][tabla.brlevo % 2].figura, PIJUN);
						wcscpy(tabla.polje[y + 1][x].figura, PRAZNO);
					}
					else
						wcscpy(tabla.levo[tabla.brlevo / 2][tabla.brlevo % 2].figura, tabla.polje[y][x].figura);

					tabla.brlevo++;
				}
				else if (tabla.brdesno < 16) {
					if (tabla.polje[y - 1][x].anpasan == UKLJUCENO) {
						wcscpy(tabla.desno[tabla.brdesno / 2][tabla.brdesno % 2].figura, PIJUN);
						wcscpy(tabla.polje[y - 1][x].figura, PRAZNO);
					}
					else
						wcscpy(tabla.desno[tabla.brdesno / 2][tabla.brdesno % 2].figura, tabla.polje[y][x].figura);

					tabla.brdesno++;
				}

				if ((xizb == 7 && yizb == 7) || (x == 7 && y == 7))
					tabla.malarbeli = ISKLJUCENO;
				else if ((xizb == 0 && yizb == 7) || (x == 0 && y == 7))
					tabla.velikarbeli = ISKLJUCENO;
				else if ((xizb == 0 && yizb == 0) || (x == 0 && y == 0))
					tabla.velikarcrni = ISKLJUCENO;
				else if ((xizb == 7 && yizb == 0) || (x == 7 && y == 0))
					tabla.malarcrni = ISKLJUCENO;
				else if (xizb == 4 && yizb == 7) {
					tabla.malarbeli = ISKLJUCENO;
					tabla.velikarbeli = ISKLJUCENO;
				}
				else if (xizb == 4 && yizb == 0) {
					tabla.malarcrni = ISKLJUCENO;
					tabla.velikarcrni = ISKLJUCENO;
				}

				wcscpy(tabla.polje[y][x].figura, tabla.polje[yizb][xizb].figura);
				wcscpy(tabla.polje[yizb][xizb].figura, PRAZNO);
				tabla.polje[y][x].bojaFigure = tabla.polje[yizb][xizb].bojaFigure;
				NoviPotez(tabla);
				ObrisiAnpasan(tabla);

				if (!wcscmp(tabla.polje[y][x].figura, PIJUN) && ((tabla.polje[y][x].bojaFigure == BELO && y == 0) || (tabla.polje[y][x].bojaFigure == CRNO && y == 7))) {
					OdstampajTablu(tabla);
					short zamena = ZameniPijuna();

					switch (zamena) {
					case 0:
						wcscpy(tabla.polje[y][x].figura, KRALJICA);
						break;

					case 1:
						wcscpy(tabla.polje[y][x].figura, TOP);
						break;

					case 2:
						wcscpy(tabla.polje[y][x].figura, LOVAC);
						break;

					case 3:
						wcscpy(tabla.polje[y][x].figura, KONJ);
					}
				}

				if (JelSahBeli(tabla, 0, 0))
					for (int i = 0; i < 64; i++)
						if (!wcscmp(tabla.polje[i / 8][i % 8].figura, KRALJ) && tabla.polje[i / 8][i % 8].bojaFigure == BELO) {
							tabla.polje[i / 8][i % 8].stanjePolja = NAPADNUTO;
							break;
						}

				if (JelSahCrni(tabla, 0, 0))
					for (int i = 0; i < 64; i++)
						if (!wcscmp(tabla.polje[i / 8][i % 8].figura, KRALJ) && tabla.polje[i / 8][i % 8].bojaFigure == CRNO) {
							tabla.polje[i / 8][i % 8].stanjePolja = NAPADNUTO;
							break;
						}

				Sleep(260);

				OsveziTablu(tabla);

				NoviPotez(tabla);

				if (JelSahBeli(tabla, 0, 0))
					for (int i = 0; i < 64; i++)
						if (!wcscmp(tabla.polje[i / 8][i % 8].figura, KRALJ) && tabla.polje[i / 8][i % 8].bojaFigure == BELO) {
							tabla.polje[i / 8][i % 8].stanjePolja = NAPADNUTO;
							OsveziTablu(tabla);
							break;
						}

				if (JelSahCrni(tabla, 0, 0))
					for (int i = 0; i < 64; i++)
						if (!wcscmp(tabla.polje[i / 8][i % 8].figura, KRALJ) && tabla.polje[i / 8][i % 8].bojaFigure == CRNO) {
							tabla.polje[i / 8][i % 8].stanjePolja = NAPADNUTO;
							OsveziTablu(tabla);
							break;
						}

				for (long i = brPotezaMax; i > brPoteza&& redo; i--) {
					istorija.erase(istorija.begin() + i);
					brPotezaMax = brPoteza;
				}
				istorija.push_back(tabla);
				brPoteza++;
				redo = false;
				sacekaj = UKLJUCENO;

			}
			else if (!wcscmp(tabla.polje[y][x].figura, KRALJ)) {
				if ((tabla.polje[y][x].bojaFigure == BELO && brPoteza % 2 == 0) || (tabla.polje[y][x].bojaFigure == CRNO && brPoteza % 2 == 1)) {
					NoviPotez(tabla);

					PostaviKralja(tabla, x, y);
				}
				else
					NoviPotez(tabla);
			}
			else if (!wcscmp(tabla.polje[y][x].figura, PIJUN)) {
				if ((tabla.polje[y][x].bojaFigure == BELO && brPoteza % 2 == 0) || (tabla.polje[y][x].bojaFigure == CRNO && brPoteza % 2 == 1)) {
					NoviPotez(tabla);
					PostaviPijuna(tabla, x, y);
				}
				else
					NoviPotez(tabla);
			}
			else if (!wcscmp(tabla.polje[y][x].figura, KONJ)) {
				if ((tabla.polje[y][x].bojaFigure == BELO && brPoteza % 2 == 0) || (tabla.polje[y][x].bojaFigure == CRNO && brPoteza % 2 == 1)) {
					NoviPotez(tabla);
					PostaviKonja(tabla, x, y);
				}
				else
					NoviPotez(tabla);
			}
			else if (!wcscmp(tabla.polje[y][x].figura, TOP)) {
				if ((tabla.polje[y][x].bojaFigure == BELO && brPoteza % 2 == 0) || (tabla.polje[y][x].bojaFigure == CRNO && brPoteza % 2 == 1)) {
					NoviPotez(tabla);
					PostaviTopa(tabla, x, y);
				}
				else
					NoviPotez(tabla);
			}
			else if (!wcscmp(tabla.polje[y][x].figura, LOVAC)) {
				if ((tabla.polje[y][x].bojaFigure == BELO && brPoteza % 2 == 0) || (tabla.polje[y][x].bojaFigure == CRNO && brPoteza % 2 == 1)) {
					NoviPotez(tabla);
					PostaviLovca(tabla, x, y);
				}
				else
					NoviPotez(tabla);
			}
			else if (!wcscmp(tabla.polje[y][x].figura, KRALJICA)) {
				if ((tabla.polje[y][x].bojaFigure == BELO && brPoteza % 2 == 0) || (tabla.polje[y][x].bojaFigure == CRNO && brPoteza % 2 == 1)) {
					NoviPotez(tabla);
					PostaviKraljicu(tabla, x, y);
				}
				else
					NoviPotez(tabla);
			}
			else if (!wcscmp(tabla.polje[y][x].figura, PRAZNO))
				NoviPotez(tabla);
			else if (!wcscmp(tabla.polje[y][x].figura, TACKA)) {
				PozicijaIzabranog(xizb, yizb, tabla);
				wcscpy(tabla.polje[y][x].figura, tabla.polje[yizb][xizb].figura);
				wcscpy(tabla.polje[yizb][xizb].figura, PRAZNO);
				tabla.polje[y][x].bojaFigure = tabla.polje[yizb][xizb].bojaFigure;

				ObrisiAnpasan(tabla);

				if (tabla.malarbeli == UKLJUCENO && xizb == 4 && yizb == 7 && x == 6 && y == 7) {
					wcscpy(tabla.polje[7][7].figura, PRAZNO);
					wcscpy(tabla.polje[7][5].figura, TOP);
					tabla.polje[7][5].bojaFigure = BELO;
				}

				if (tabla.velikarbeli == UKLJUCENO && xizb == 4 && yizb == 7 && x == 1 && y == 7) {
					wcscpy(tabla.polje[7][0].figura, PRAZNO);
					wcscpy(tabla.polje[7][1].figura, PRAZNO);
					wcscpy(tabla.polje[7][2].figura, KRALJ);
					tabla.polje[7][2].bojaFigure = BELO;
					wcscpy(tabla.polje[7][3].figura, TOP);
					tabla.polje[7][3].bojaFigure = BELO;
				}

				if (tabla.malarcrni == UKLJUCENO && xizb == 4 && yizb == 0 && x == 6 && y == 0) {
					wcscpy(tabla.polje[0][7].figura, PRAZNO);
					wcscpy(tabla.polje[0][5].figura, TOP);
					tabla.polje[0][5].bojaFigure = CRNO;
				}

				if (tabla.velikarcrni == UKLJUCENO && xizb == 4 && yizb == 0 && x == 1 && y == 0) {
					wcscpy(tabla.polje[0][0].figura, PRAZNO);
					wcscpy(tabla.polje[0][1].figura, PRAZNO);
					wcscpy(tabla.polje[0][2].figura, KRALJ);
					tabla.polje[0][2].bojaFigure = CRNO;
					wcscpy(tabla.polje[0][3].figura, TOP);
					tabla.polje[0][3].bojaFigure = CRNO;
				}

				if (!wcscmp(tabla.polje[y][x].figura, PIJUN) && ((tabla.polje[y][x].bojaFigure == BELO && y == yizb - 2) || (tabla.polje[y][x].bojaFigure == CRNO && y == yizb + 2)))
					tabla.polje[y][x].anpasan = UKLJUCENO;

				if (xizb == 7 && yizb == 7)
					tabla.malarbeli = ISKLJUCENO;
				else if (xizb == 0 && yizb == 7)
					tabla.velikarbeli = ISKLJUCENO;
				else if (xizb == 0 && yizb == 0)
					tabla.velikarcrni = ISKLJUCENO;
				else if (xizb == 7 && yizb == 0)
					tabla.malarcrni = ISKLJUCENO;
				else if (xizb == 4 && yizb == 7) {
					tabla.malarbeli = ISKLJUCENO;
					tabla.velikarbeli = ISKLJUCENO;
				}
				else if (xizb == 4 && yizb == 0) {
					tabla.malarcrni = ISKLJUCENO;
					tabla.velikarcrni = ISKLJUCENO;
				}

				NoviPotez(tabla);

				if (!wcscmp(tabla.polje[y][x].figura, PIJUN) && ((tabla.polje[y][x].bojaFigure == BELO && y == 0) || (tabla.polje[y][x].bojaFigure == CRNO && y == 7))) {
					OdstampajTablu(tabla);
					short zamena = ZameniPijuna();

					switch (zamena) {
					case 0:
						wcscpy(tabla.polje[y][x].figura, KRALJICA);
						break;

					case 1:
						wcscpy(tabla.polje[y][x].figura, TOP);
						break;

					case 2:
						wcscpy(tabla.polje[y][x].figura, LOVAC);
						break;

					case 3:
						wcscpy(tabla.polje[y][x].figura, KONJ);
					}
				}

				if (JelSahBeli(tabla, 0, 0))
					for (int i = 0; i < 64; i++)
						if (!wcscmp(tabla.polje[i / 8][i % 8].figura, KRALJ) && tabla.polje[i / 8][i % 8].bojaFigure == BELO) {
							tabla.polje[i / 8][i % 8].stanjePolja = NAPADNUTO;
							OsveziTablu(tabla);
							break;
						}

				if (JelSahCrni(tabla, 0, 0))
					for (int i = 0; i < 64; i++)
						if (!wcscmp(tabla.polje[i / 8][i % 8].figura, KRALJ) && tabla.polje[i / 8][i % 8].bojaFigure == CRNO) {
							tabla.polje[i / 8][i % 8].stanjePolja = NAPADNUTO;
							OsveziTablu(tabla);
							break;
						}

				for (long i = brPotezaMax; i > brPoteza&& redo; i--) {
					istorija.erase(istorija.begin() + i);
					brPotezaMax = brPoteza;
				}
				istorija.push_back(tabla);
				brPoteza++;
				redo = false;
				sacekaj = UKLJUCENO;
			}

			OsveziTablu(tabla);
		}
		else {
			NoviPotez(tabla);
			OsveziTablu(tabla);
		}
	}
}

void NapraviTablu(Tabla& tabla) {
	tabla.brlevo = 0;
	tabla.brdesno = 0;
	tabla.krajigre = 0;
	tabla.malarbeli = UKLJUCENO;
	tabla.malarcrni = UKLJUCENO;
	tabla.velikarbeli = UKLJUCENO;
	tabla.velikarcrni = UKLJUCENO;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			tabla.polje[i][j].stanjePolja = NEUTRALNO;
			tabla.polje[i][j].anpasan = ISKLJUCENO;

			if ((i + j) % 2 == 0)
				tabla.polje[i][j].bojaPolja = BELO;
			else
				tabla.polje[i][j].bojaPolja = CRNO;
		}

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 8; j++) {
			tabla.polje[i][j].bojaFigure = CRNO;
			tabla.polje[i + 6][j].bojaFigure = BELO;
		}

	for (int j = 0; j < 8; j++) {
		wcscpy(tabla.polje[1][j].figura, PIJUN);
		wcscpy(tabla.polje[6][j].figura, PIJUN);
	}

	wcscpy(tabla.polje[0][0].figura, TOP);
	wcscpy(tabla.polje[0][7].figura, TOP);
	wcscpy(tabla.polje[7][0].figura, TOP);
	wcscpy(tabla.polje[7][7].figura, TOP);

	wcscpy(tabla.polje[0][1].figura, KONJ);
	wcscpy(tabla.polje[0][6].figura, KONJ);
	wcscpy(tabla.polje[7][1].figura, KONJ);
	wcscpy(tabla.polje[7][6].figura, KONJ);

	wcscpy(tabla.polje[0][2].figura, LOVAC);
	wcscpy(tabla.polje[0][5].figura, LOVAC);
	wcscpy(tabla.polje[7][2].figura, LOVAC);
	wcscpy(tabla.polje[7][5].figura, LOVAC);

	wcscpy(tabla.polje[0][3].figura, KRALJICA);
	wcscpy(tabla.polje[7][3].figura, KRALJICA);

	wcscpy(tabla.polje[0][4].figura, KRALJ);
	wcscpy(tabla.polje[7][4].figura, KRALJ);

	for (int i = 2; i < 6; i++)
		for (int j = 0; j < 8; j++) {
			tabla.polje[i][j].bojaFigure = ZELENO;
			wcscpy(tabla.polje[i][j].figura, PRAZNO);
		}

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 2; j++) {
			tabla.levo[i][j].anpasan = ISKLJUCENO;
			tabla.levo[i][j].bojaFigure = CRNO;
			tabla.levo[i][j].stanjePolja = NEUTRALNO;
			tabla.desno[i][j].anpasan = ISKLJUCENO;
			tabla.desno[i][j].bojaFigure = BELO;
			tabla.desno[i][j].stanjePolja = NEUTRALNO;
			wcscpy(tabla.levo[i][j].figura, PRAZNO);
			wcscpy(tabla.desno[i][j].figura, PRAZNO);
		}
}

void SakriKursor() {
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(ConsoleOutputHandle, &info);
}

void ObrisiEkran() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount;
	COORD homeCoords = { 0, 0 };

	if (ConsoleOutputHandle == INVALID_HANDLE_VALUE)
		return;

	GetConsoleScreenBufferInfo(ConsoleOutputHandle, &csbi);
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	FillConsoleOutputCharacter(ConsoleOutputHandle, (TCHAR)' ', cellCount, homeCoords, &count);
	FillConsoleOutputAttribute(ConsoleOutputHandle, csbi.wAttributes, cellCount, homeCoords, &count);
	SetConsoleCursorPosition(ConsoleOutputHandle, homeCoords);
}

void gotoxy(int x, int y) {
	COORD koordinate;
	koordinate.X = x;
	koordinate.Y = y;
	SetConsoleCursorPosition(ConsoleOutputHandle, koordinate);
}

void UcitajKlik(int& x, int& y) {
	DWORD info;
	INPUT_RECORD input;
	SetConsoleMode(ConsoleInputHandle, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	ReadConsoleInput(ConsoleInputHandle, &input, 1, &info);

	if (input.Event.MouseEvent.dwButtonState != 0x0001) {
	GOBACK:
		ReadConsoleInput(ConsoleInputHandle, &input, 1, &info);

		if (input.Event.MouseEvent.dwButtonState == 0x0001) {
		GOBACK2:
			ReadConsoleInput(ConsoleInputHandle, &input, 1, &info);

			if (input.Event.MouseEvent.dwButtonState != 0x0001) {
				x = input.Event.MouseEvent.dwMousePosition.X;
				y = input.Event.MouseEvent.dwMousePosition.Y;
			}
			else
				goto GOBACK2;
		}
		else
			goto GOBACK;
	}
	else
		UcitajKlik(x, y);
}

void CentrirajKonzolu() {
	RECT rectClient, rectWindow;
	HWND hWnd = GetConsoleWindow();
	GetClientRect(hWnd, &rectClient);
	GetWindowRect(hWnd, &rectWindow);
	int posx, posy;
	posx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2;
	posy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2;

	SetWindowPos(GetConsoleWindow(), HWND_TOP, posx, posy, 0, 0, SWP_NOSIZE);
}

void PodesiKonzolu() {
	GetConsoleCursorInfo(ConsoleOutputHandle, &kursor);
	SakriKursor();
	HWND consoleWindow = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(consoleWindow, FALSE);
	CONSOLE_FONT_INFOEX infof = { 0 };
	_CONSOLE_SCREEN_BUFFER_INFOEX info;

	infof.cbSize = sizeof(infof);
	infof.dwFontSize.Y = velicina;
	infof.FontWeight = FW_NORMAL;
	wcscpy(infof.FaceName, L"MS Gothic");
	SetCurrentConsoleFontEx(ConsoleOutputHandle, NULL, &infof);

	CentrirajKonzolu();

	winlog = GetWindowLong(consoleWindow, GWL_STYLE);
	SetWindowLong(consoleWindow, GWL_STYLE, winlog & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_VSCROLL);
	SetConsoleMode(ConsoleInputHandle, ENABLE_EXTENDED_FLAGS);
	SetConsoleTitleW(L"ŠAH");

	info.dwSize = { 36, 17 };
	info.cbSize = sizeof(info);
	GetConsoleScreenBufferInfoEx(ConsoleOutputHandle, &info);
	info.ColorTable[0] = RGB(242, 242, 242);
	info.ColorTable[1] = RGB(10, 10, 10);
	info.ColorTable[2] = RGB(198, 175, 141);
	info.ColorTable[3] = RGB(114, 71, 56);
	info.ColorTable[4] = RGB(50, 31, 26);
	info.ColorTable[5] = RGB(132, 94, 73);
	info.ColorTable[6] = RGB(73, 47, 37);
	info.ColorTable[7] = RGB(22, 11, 4);
	info.ColorTable[8] = RGB(158, 158, 158);
	info.ColorTable[9] = RGB(102, 102, 102);
	info.ColorTable[10] = RGB(43, 43, 43);
	info.ColorTable[11] = RGB(0, 119, 65);
	info.ColorTable[12] = RGB(126, 33, 35);
	info.ColorTable[13] = RGB(10, 10, 130);
	info.ColorTable[14] = RGB(240, 191, 40);
	info.ColorTable[15] = RGB(255, 255, 255);
	SetConsoleScreenBufferInfoEx(ConsoleOutputHandle, &info);
	system("MODE 36, 17");
	system("color 23");
	_setmode(_fileno(stdout), _O_U8TEXT);
}

void PodesiVelicinu(int i) {
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = i;
	info.FontWeight = FW_NORMAL;
	wcscpy(info.FaceName, L"MS Gothic");
	SetCurrentConsoleFontEx(ConsoleOutputHandle, NULL, &info);

	CentrirajKonzolu();
	CentrirajKonzolu();
	system("MODE 36, 17");
}

void OdstampajTablu(Tabla tabla) {
	_setmode(_fileno(stdout), _O_U8TEXT);

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			if (tabla.polje[i][j].stanjePolja == IZABRANO && tabla.polje[i][j].bojaFigure == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 177);
			else if (tabla.polje[i][j].stanjePolja == IZABRANO && tabla.polje[i][j].bojaFigure == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 176);
			else if (tabla.polje[i][j].stanjePolja == NAPADNUTO && (tabla.polje[i][j].bojaFigure == CRNO || tabla.polje[i][j].bojaFigure == ZELENO))
				SetConsoleTextAttribute(ConsoleOutputHandle, 193);
			else if (tabla.polje[i][j].stanjePolja == NAPADNUTO && tabla.polje[i][j].bojaFigure == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 192);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == SVETLO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 32);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == SVETLO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 33);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == TAMNO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 80);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == TAMNO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 81);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == SIVO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 128);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == SIVO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 129);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == SVETLO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 48);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == SVETLO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 49);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == TAMNO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 96);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == TAMNO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 97);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == SIVO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 144);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == SIVO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 145);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == SVETLO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 43);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == TAMNO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 91);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == SIVO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 139);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == SVETLO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 59);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == TAMNO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 107);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == SIVO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 155);

			gotoxy(10 + j * 2, 3 + i);
			wprintf(tabla.polje[i][j].figura);
		}

	OdstampajLevoDesno(tabla);
}

void ObrnutoOdstampajTablu(Tabla tabla) {
	_setmode(_fileno(stdout), _O_U8TEXT);

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			if (tabla.polje[i][j].stanjePolja == IZABRANO && tabla.polje[i][j].bojaFigure == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 177);
			else if (tabla.polje[i][j].stanjePolja == IZABRANO && tabla.polje[i][j].bojaFigure == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 176);
			else if (tabla.polje[i][j].stanjePolja == NAPADNUTO && tabla.polje[i][j].bojaFigure == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 193);
			else if (tabla.polje[i][j].stanjePolja == NAPADNUTO && tabla.polje[i][j].bojaFigure == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 192);
			else if (tabla.polje[i][j].stanjePolja == NAPADNUTO && !wcscmp(tabla.polje[i][j].figura, PRAZNO))
				SetConsoleTextAttribute(ConsoleOutputHandle, 192);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == SVETLO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 32);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == SVETLO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 33);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == TAMNO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 80);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == TAMNO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 81);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == SIVO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 128);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == SIVO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 129);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == SVETLO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 48);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == SVETLO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 49);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == TAMNO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 96);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == TAMNO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 97);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == BELO && boja == SIVO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 144);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == CRNO && boja == SIVO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 145);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == SVETLO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 43);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == TAMNO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 91);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == SIVO && tabla.polje[i][j].bojaPolja == BELO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 139);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == SVETLO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 59);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == TAMNO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 107);
			else if (tabla.polje[i][j].stanjePolja == NEUTRALNO && tabla.polje[i][j].bojaFigure == ZELENO && boja == SIVO && tabla.polje[i][j].bojaPolja == CRNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 155);

			gotoxy(24 - j * 2, 10 - i);
			wprintf(tabla.polje[i][j].figura);
		}

	OdstampajLevoDesno(tabla);
}

void OdstampajLevoDesno(Tabla tabla) {
	_setmode(_fileno(stdout), _O_U8TEXT);

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 2; j++) {
			if (boja == SVETLO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 81);
			else if (boja == TAMNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 97);
			else if (boja == SIVO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 145);

			gotoxy(2 + j * 2, 3 + i);
			wprintf(tabla.levo[i][j].figura);
		}

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 2; j++) {
			if (boja == SVETLO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 80);
			else if (boja == TAMNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 96);
			else if (boja == SIVO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 144);

			gotoxy(30 + j * 2, 3 + i);
			wprintf(tabla.desno[i][j].figura);
		}

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 68);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 119);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 170);


	gotoxy(26, 3);
	wprintf(L"██");
	gotoxy(26, 4);
	wprintf(L"██");
	gotoxy(26, 5);
	wprintf(L"██");
	gotoxy(26, 6);
	wprintf(L"██");
	gotoxy(26, 7);
	wprintf(L"██");
	gotoxy(26, 8);
	wprintf(L"██");
	gotoxy(26, 9);
	wprintf(L"██");
	gotoxy(26, 10);
	wprintf(L"██");
}

void PozicijaNaTabli(int& x, int& y) {
	switch (x) {
	case 10:
	case 11:
		x = 0;
		break;

	case 12:
	case 13:
		x = 1;
		break;

	case 14:
	case 15:
		x = 2;
		break;

	case 16:
	case 17:
		x = 3;
		break;

	case 18:
	case 19:
		x = 4;
		break;

	case 20:
	case 21:
		x = 5;
		break;

	case 22:
	case 23:
		x = 6;
		break;

	case 24:
	case 25:
		x = 7;
	}

	switch (y) {
	case 10:
		y = 7;
		break;

	case 9:
		y = 6;
		break;

	case 8:
		y = 5;
		break;

	case 7:
		y = 4;
		break;

	case 6:
		y = 3;
		break;

	case 5:
		y = 2;
		break;

	case 4:
		y = 1;
		break;

	case 3:
		y = 0;
	}
}

void ObrnutaPozicijaNaTabli(int& x, int& y) {
	switch (x) {
	case 10:
	case 11:
		x = 7;
		break;

	case 12:
	case 13:
		x = 6;
		break;

	case 14:
	case 15:
		x = 5;
		break;

	case 16:
	case 17:
		x = 4;
		break;

	case 18:
	case 19:
		x = 3;
		break;

	case 20:
	case 21:
		x = 2;
		break;

	case 22:
	case 23:
		x = 1;
		break;

	case 24:
	case 25:
		x = 0;
	}

	switch (y) {
	case 10:
		y = 0;
		break;

	case 9:
		y = 1;
		break;

	case 8:
		y = 2;
		break;

	case 7:
		y = 3;
		break;

	case 6:
		y = 4;
		break;

	case 5:
		y = 5;
		break;

	case 4:
		y = 6;
		break;

	case 3:
		y = 7;
	}
}

void NoviPotez(Tabla& tabla) {
	for (int a = 0; a < 8; a++)
		for (int b = 0; b < 8; b++) {
			if (tabla.polje[a][b].stanjePolja == IZABRANO)
				tabla.polje[a][b].stanjePolja = NEUTRALNO;

			if (tabla.polje[a][b].stanjePolja == NAPADNUTO)
				tabla.polje[a][b].stanjePolja = NEUTRALNO;

			if (!wcscmp(tabla.polje[a][b].figura, TACKA))
				wcscpy(tabla.polje[a][b].figura, PRAZNO);

			if (!wcscmp(tabla.polje[a][b].figura, PRAZNO))
				tabla.polje[a][b].bojaFigure = ZELENO;
		}
}

void ObrisiAnpasan(Tabla& tabla) {
	for (int a = 0; a < 8; a++)
		for (int b = 0; b < 8; b++)
			if (tabla.polje[a][b].anpasan == UKLJUCENO)
				tabla.polje[a][b].anpasan = ISKLJUCENO;

}

void PozicijaIzabranog(int& xizb, int& yizb, Tabla tabla) {
	for (int a = 0; a < 8; a++)
		for (int b = 0; b < 8; b++)
			if (tabla.polje[a][b].stanjePolja == IZABRANO) {
				yizb = a;
				xizb = b;
				return;
			}
}

short JelUnutarTable(int x, int y) {
	if (x >= 0 && x <= 7 && y >= 0 && y <= 7)
		return 1;
	else
		return 0;
}

void OdstampajKonzolu() {
	_setmode(_fileno(stdout), _O_U8TEXT);

	gotoxy(0, 0);

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 45);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 93);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 141);

	wprintf(L"");  // save

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 46);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 94);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 142);

	wprintf(L"");  // open

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 44);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 92);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 140);

	wprintf(L"");  // restart      

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 36);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 87);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 138);

	// gear 
	wprintf(L"");  // color
	wprintf(L"");  // resize
	wprintf(L"");  // undo
	wprintf(L"");  // redo
	wprintf(L"                  "); // space

	if (rotacija == UKLJUCENO)
		wprintf(L"");  // rotation 
	else
		wprintf(L"");  // rotation 

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 47);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 95);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 143);

	wprintf(L"\n");  // info

	//if (boja == SVETLO)
	//	SetConsoleTextAttribute(ConsoleOutputHandle, 44);
	//else if (boja == TAMNO)
	//	SetConsoleTextAttribute(ConsoleOutputHandle, 92);
	//else if (boja == SIVO)
	//	SetConsoleTextAttribute(ConsoleOutputHandle, 140);

	//wprintf(L"\n");  // quit

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 36);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 87);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 138);

	wprintf(L"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	wprintf(L"┏━━━━━┓                     ┏━━━━━┓ \n");
	wprintf(L"┃     ┃                     ┃     ┃ \n");
	wprintf(L"┃     ┃                     ┃     ┃ \n");
	wprintf(L"┃     ┃                     ┃     ┃ \n");
	wprintf(L"┃     ┃                     ┃     ┃ \n");
	wprintf(L"┃     ┃                     ┃     ┃ \n");
	wprintf(L"┃     ┃                     ┃     ┃ \n");
	wprintf(L"┃     ┃                     ┃     ┃ \n");
	wprintf(L"┃     ┃                     ┃     ┃ \n");
	wprintf(L"┗━━━━━┛                     ┗━━━━━┛ \n");
	wprintf(L"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓ \n");
	wprintf(L"┃                                 ┃ \n");
	wprintf(L"┃                                 ┃ \n");
	wprintf(L"┃                                 ┃ \n");
	wprintf(L"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ");

	gotoxy(0, 2);	wprintf(L"┏");
	gotoxy(0, 3);	wprintf(L"┃");
	gotoxy(0, 4);	wprintf(L"┃");
	gotoxy(0, 5);	wprintf(L"┃");
	gotoxy(0, 6);	wprintf(L"┃");
	gotoxy(0, 7);	wprintf(L"┃");
	gotoxy(0, 8);	wprintf(L"┃");
	gotoxy(0, 9);	wprintf(L"┃");
	gotoxy(0, 10);	wprintf(L"┃");
	gotoxy(0, 11);	wprintf(L"┗");

	gotoxy(6, 2);	wprintf(L"┓");
	gotoxy(6, 3);	wprintf(L"┃");
	gotoxy(6, 4);	wprintf(L"┃");
	gotoxy(6, 5);	wprintf(L"┃");
	gotoxy(6, 6);	wprintf(L"┃");
	gotoxy(6, 7);	wprintf(L"┃");
	gotoxy(6, 8);	wprintf(L"┃");
	gotoxy(6, 9);	wprintf(L"┃");
	gotoxy(6, 10);	wprintf(L"┃");
	gotoxy(6, 11);	wprintf(L"┛");

	gotoxy(28, 2);	wprintf(L"┏");
	gotoxy(28, 3);	wprintf(L"┃");
	gotoxy(28, 4);	wprintf(L"┃");
	gotoxy(28, 5);	wprintf(L"┃");
	gotoxy(28, 6);	wprintf(L"┃");
	gotoxy(28, 7);	wprintf(L"┃");
	gotoxy(28, 8);	wprintf(L"┃");
	gotoxy(28, 9);	wprintf(L"┃");
	gotoxy(28, 10);	wprintf(L"┃");
	gotoxy(28, 11);	wprintf(L"┗");

	gotoxy(34, 2);	wprintf(L"┓");
	gotoxy(34, 3);	wprintf(L"┃");
	gotoxy(34, 4);	wprintf(L"┃");
	gotoxy(34, 5);	wprintf(L"┃");
	gotoxy(34, 6);	wprintf(L"┃");
	gotoxy(34, 7);	wprintf(L"┃");
	gotoxy(34, 8);	wprintf(L"┃");
	gotoxy(34, 9);	wprintf(L"┃");
	gotoxy(34, 10);	wprintf(L"┃");
	gotoxy(34, 11);	wprintf(L"┛");

	gotoxy(0, 12);	wprintf(L"┏");
	gotoxy(0, 13);	wprintf(L"┃");
	gotoxy(0, 14);	wprintf(L"┃");
	gotoxy(0, 15);	wprintf(L"┃");
	gotoxy(0, 16);	wprintf(L"┗");

	gotoxy(34, 12);	wprintf(L"┓");
	gotoxy(34, 13);	wprintf(L"┃");
	gotoxy(34, 14);	wprintf(L"┃");
	gotoxy(34, 15);	wprintf(L"┃");
	gotoxy(34, 16);	wprintf(L"┛");

	ObrisiTerminal();

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 68);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 119);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 170);

	gotoxy(8, 2);	  	wprintf(L"████████████████████");
	gotoxy(8, 3);	  	wprintf(L"██");
	gotoxy(8, 4);	  	wprintf(L"██");
	gotoxy(8, 5);		wprintf(L"██");
	gotoxy(8, 6);		wprintf(L"██");
	gotoxy(8, 7);		wprintf(L"██");
	gotoxy(8, 8);   	wprintf(L"██");
	gotoxy(8, 9);	  	wprintf(L"██");
	gotoxy(8, 10);	  	wprintf(L"██");
	gotoxy(26, 3);	  	wprintf(L"██");
	gotoxy(26, 4);	  	wprintf(L"██");
	gotoxy(26, 5);	  	wprintf(L"██");
	gotoxy(26, 6);	  	wprintf(L"██");
	gotoxy(26, 7);	  	wprintf(L"██");
	gotoxy(26, 8);	  	wprintf(L"██");
	gotoxy(26, 9);	  	wprintf(L"██");
	gotoxy(26, 10);	  	wprintf(L"██");
	gotoxy(8, 11);	  	wprintf(L"████████████████████");
}

void OsveziTablu(Tabla tabla) {
	if ((brPoteza % 2 && rotacija == UKLJUCENO && JelMozeIgratiCrni(tabla)) || (!JelMozeIgratiBeli(tabla) && rotacija == UKLJUCENO && brPoteza % 2 == 0)) {
		if (sacekaj == UKLJUCENO && JelMozeIgratiBeli(tabla)) {
			OdstampajTablu(tabla);
			Sleep(260);
		}

		ObrnutoOdstampajTablu(tabla);
	}
	else {
		if (sacekaj == UKLJUCENO && rotacija == UKLJUCENO && JelMozeIgratiCrni(tabla)) {
			ObrnutoOdstampajTablu(tabla);
			Sleep(260);
		}

		OdstampajTablu(tabla);
	}
}

void ObrisiTerminal() {
	_setmode(_fileno(stdout), _O_U8TEXT);

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 85);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 102);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 153);

	gotoxy(2, 13);
	wprintf(L"████████████████████████████████");
	gotoxy(2, 14);
	wprintf(L"████████████████████████████████");
	gotoxy(2, 15);
	wprintf(L"████████████████████████████████");
}

Tabla StaviFiguruNaXY(Tabla tabla, int x, int y) {
	int xizb, yizb;
	PozicijaIzabranog(xizb, yizb, tabla);
	wcscpy(tabla.polje[y][x].figura, tabla.polje[yizb][xizb].figura);
	wcscpy(tabla.polje[yizb][xizb].figura, PRAZNO);
	tabla.polje[y][x].bojaFigure = tabla.polje[yizb][xizb].bojaFigure;
	NoviPotez(tabla);

	return tabla;
}

short JelSahBeli(Tabla tabla, int xpomeraj, int ypomeraj) {
	int x = 0, y = 0;

	for (int i = 0; i < 64; i++) {
		if (!wcscmp(tabla.polje[i / 8][i % 8].figura, KRALJ) && tabla.polje[i / 8][i % 8].bojaFigure == BELO) {
			if (JelUnutarTable(i % 8 + xpomeraj, i / 8 + ypomeraj)) {
				y = i / 8 + ypomeraj;
				x = i % 8 + xpomeraj;
			}
			else {
				x = i % 8;
				y = i / 8;
			}

			break;
		}
	}

	if (!wcscmp(tabla.polje[y - 1][x + 1].figura, PIJUN) && tabla.polje[y - 1][x + 1].bojaFigure == CRNO && JelUnutarTable(x + 1, y - 1))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x - 1].figura, PIJUN) && tabla.polje[y - 1][x - 1].bojaFigure == CRNO && JelUnutarTable(x - 1, y - 1))
		return 1;

	if (!wcscmp(tabla.polje[y - 2][x - 1].figura, KONJ) && tabla.polje[y - 2][x - 1].bojaFigure == CRNO && JelUnutarTable(x - 1, y - 2))
		return 1;

	if (!wcscmp(tabla.polje[y - 2][x + 1].figura, KONJ) && tabla.polje[y - 2][x + 1].bojaFigure == CRNO && JelUnutarTable(x + 1, y - 2))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x + 2].figura, KONJ) && tabla.polje[y - 1][x + 2].bojaFigure == CRNO && JelUnutarTable(x + 2, y - 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x + 2].figura, KONJ) && tabla.polje[y + 1][x + 2].bojaFigure == CRNO && JelUnutarTable(x + 2, y + 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 2][x + 1].figura, KONJ) && tabla.polje[y + 2][x + 1].bojaFigure == CRNO && JelUnutarTable(x + 1, y + 2))
		return 1;

	if (!wcscmp(tabla.polje[y + 2][x - 1].figura, KONJ) && tabla.polje[y + 2][x - 1].bojaFigure == CRNO && JelUnutarTable(x - 1, y + 2))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x - 2].figura, KONJ) && tabla.polje[y + 1][x - 2].bojaFigure == CRNO && JelUnutarTable(x - 2, y + 1))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x - 2].figura, KONJ) && tabla.polje[y - 1][x - 2].bojaFigure == CRNO && JelUnutarTable(x - 2, y - 1))
		return 1;

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y][x - i].figura, PRAZNO) || (!wcscmp(tabla.polje[y][x - i].figura, KRALJ) && tabla.polje[y][x - i].bojaFigure == BELO)) && JelUnutarTable(x - i, y));
		else if ((!wcscmp(tabla.polje[y][x - i].figura, TOP) || !wcscmp(tabla.polje[y][x - i].figura, KRALJICA)) && tabla.polje[y][x - i].bojaFigure == CRNO && JelUnutarTable(x - i, y))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y][x + i].figura, PRAZNO) || (!wcscmp(tabla.polje[y][x + i].figura, KRALJ) && tabla.polje[y][x + i].bojaFigure == BELO)) && JelUnutarTable(x + i, y));
		else if ((!wcscmp(tabla.polje[y][x + i].figura, TOP) || !wcscmp(tabla.polje[y][x + i].figura, KRALJICA)) && tabla.polje[y][x + i].bojaFigure == CRNO && JelUnutarTable(x + i, y))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y - i][x].figura, PRAZNO) || (!wcscmp(tabla.polje[y - i][x].figura, KRALJ) && tabla.polje[y - i][x].bojaFigure == BELO)) && JelUnutarTable(x, y - i));
		else if ((!wcscmp(tabla.polje[y - i][x].figura, TOP) || !wcscmp(tabla.polje[y - i][x].figura, KRALJICA)) && tabla.polje[y - i][x].bojaFigure == CRNO && JelUnutarTable(x, y - i))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y + i][x].figura, PRAZNO) || (!wcscmp(tabla.polje[y + i][x].figura, KRALJ) && tabla.polje[y + i][x].bojaFigure == BELO)) && JelUnutarTable(x, y + i));
		else if ((!wcscmp(tabla.polje[y + i][x].figura, TOP) || !wcscmp(tabla.polje[y + i][x].figura, KRALJICA)) && tabla.polje[y + i][x].bojaFigure == CRNO && JelUnutarTable(x, y + i))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y - i][x - i].figura, PRAZNO) || (!wcscmp(tabla.polje[y - i][x - i].figura, KRALJ) && tabla.polje[y - i][x - i].bojaFigure == BELO)) && JelUnutarTable(x - i, y - i));
		else if ((!wcscmp(tabla.polje[y - i][x - i].figura, LOVAC) || !wcscmp(tabla.polje[y - i][x - i].figura, KRALJICA)) && tabla.polje[y - i][x - i].bojaFigure == CRNO && JelUnutarTable(x - i, y - i))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y - i][x + i].figura, PRAZNO) || (!wcscmp(tabla.polje[y - i][x + i].figura, KRALJ) && tabla.polje[y - i][x + i].bojaFigure == BELO)) && JelUnutarTable(x + i, y - i));
		else if ((!wcscmp(tabla.polje[y - i][x + i].figura, LOVAC) || !wcscmp(tabla.polje[y - i][x + i].figura, KRALJICA)) && tabla.polje[y - i][x + i].bojaFigure == CRNO && JelUnutarTable(x + i, y - i))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y + i][x - i].figura, PRAZNO) || (!wcscmp(tabla.polje[y + i][x - i].figura, KRALJ) && tabla.polje[y + i][x - i].bojaFigure == BELO)) && JelUnutarTable(x - i, y + i));
		else if ((!wcscmp(tabla.polje[y + i][x - i].figura, LOVAC) || !wcscmp(tabla.polje[y + i][x - i].figura, KRALJICA)) && tabla.polje[y + i][x - i].bojaFigure == CRNO && JelUnutarTable(x - i, y + i))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y + i][x + i].figura, PRAZNO) || (!wcscmp(tabla.polje[y + i][x + i].figura, KRALJ) && tabla.polje[y + i][x + i].bojaFigure == BELO)) && JelUnutarTable(x + i, y + i));
		else if ((!wcscmp(tabla.polje[y + i][x + i].figura, LOVAC) || !wcscmp(tabla.polje[y + i][x + i].figura, KRALJICA)) && tabla.polje[y + i][x + i].bojaFigure == CRNO && JelUnutarTable(x + i, y + i))
			return 1;
		else
			break;
	}

	if (!wcscmp(tabla.polje[y][x - 1].figura, KRALJ) && tabla.polje[y][x - 1].bojaFigure == CRNO && JelUnutarTable(x - 1, y))
		return 1;

	if (!wcscmp(tabla.polje[y][x + 1].figura, KRALJ) && tabla.polje[y][x + 1].bojaFigure == CRNO && JelUnutarTable(x + 1, y))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x].figura, KRALJ) && tabla.polje[y - 1][x].bojaFigure == CRNO && JelUnutarTable(x, y - 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x].figura, KRALJ) && tabla.polje[y + 1][x].bojaFigure == CRNO && JelUnutarTable(x, y + 1))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x - 1].figura, KRALJ) && tabla.polje[y - 1][x - 1].bojaFigure == CRNO && JelUnutarTable(x - 1, y - 1))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x + 1].figura, KRALJ) && tabla.polje[y - 1][x + 1].bojaFigure == CRNO && JelUnutarTable(x + 1, y - 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x - 1].figura, KRALJ) && tabla.polje[y + 1][x - 1].bojaFigure == CRNO && JelUnutarTable(x - 1, y + 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x + 1].figura, KRALJ) && tabla.polje[y + 1][x + 1].bojaFigure == CRNO && JelUnutarTable(x + 1, y + 1))
		return 1;

	return 0;
}

short JelSahCrni(Tabla tabla, int xpomeraj, int ypomeraj) {
	int x = 0, y = 0;

	for (int i = 0; i < 64; i++) {
		if (!wcscmp(tabla.polje[i / 8][i % 8].figura, KRALJ) && tabla.polje[i / 8][i % 8].bojaFigure == CRNO) {
			if (JelUnutarTable(i % 8 + xpomeraj, i / 8 + ypomeraj)) {
				y = i / 8 + ypomeraj;
				x = i % 8 + xpomeraj;
			}
			else {
				x = i % 8;
				y = i / 8;
			}

			break;
		}
	}

	if (!wcscmp(tabla.polje[y + 1][x + 1].figura, PIJUN) && tabla.polje[y + 1][x + 1].bojaFigure == BELO && JelUnutarTable(x + 1, y + 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x - 1].figura, PIJUN) && tabla.polje[y + 1][x - 1].bojaFigure == BELO && JelUnutarTable(x - 1, y + 1))
		return 1;

	if (!wcscmp(tabla.polje[y - 2][x - 1].figura, KONJ) && tabla.polje[y - 2][x - 1].bojaFigure == BELO && JelUnutarTable(x - 1, y - 2))
		return 1;

	if (!wcscmp(tabla.polje[y - 2][x + 1].figura, KONJ) && tabla.polje[y - 2][x + 1].bojaFigure == BELO && JelUnutarTable(x + 1, y - 2))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x + 2].figura, KONJ) && tabla.polje[y - 1][x + 2].bojaFigure == BELO && JelUnutarTable(x + 2, y - 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x + 2].figura, KONJ) && tabla.polje[y + 1][x + 2].bojaFigure == BELO && JelUnutarTable(x + 2, y + 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 2][x + 1].figura, KONJ) && tabla.polje[y + 2][x + 1].bojaFigure == BELO && JelUnutarTable(x + 1, y + 2))
		return 1;

	if (!wcscmp(tabla.polje[y + 2][x - 1].figura, KONJ) && tabla.polje[y + 2][x - 1].bojaFigure == BELO && JelUnutarTable(x - 1, y + 2))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x - 2].figura, KONJ) && tabla.polje[y + 1][x - 2].bojaFigure == BELO && JelUnutarTable(x - 2, y + 1))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x - 2].figura, KONJ) && tabla.polje[y - 1][x - 2].bojaFigure == BELO && JelUnutarTable(x - 2, y - 1))
		return 1;

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y][x - i].figura, PRAZNO) || (!wcscmp(tabla.polje[y][x - i].figura, KRALJ) && tabla.polje[y][x - i].bojaFigure == CRNO)) && JelUnutarTable(x - i, y));
		else if ((!wcscmp(tabla.polje[y][x - i].figura, TOP) || !wcscmp(tabla.polje[y][x - i].figura, KRALJICA)) && tabla.polje[y][x - i].bojaFigure == BELO && JelUnutarTable(x - i, y))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y][x + i].figura, PRAZNO) || (!wcscmp(tabla.polje[y][x + i].figura, KRALJ) && tabla.polje[y][x + i].bojaFigure == CRNO)) && JelUnutarTable(x + i, y));
		else if ((!wcscmp(tabla.polje[y][x + i].figura, TOP) || !wcscmp(tabla.polje[y][x + i].figura, KRALJICA)) && tabla.polje[y][x + i].bojaFigure == BELO && JelUnutarTable(x + i, y))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y - i][x].figura, PRAZNO) || (!wcscmp(tabla.polje[y - i][x].figura, KRALJ) && tabla.polje[y - i][x].bojaFigure == CRNO)) && JelUnutarTable(x, y - i));
		else if ((!wcscmp(tabla.polje[y - i][x].figura, TOP) || !wcscmp(tabla.polje[y - i][x].figura, KRALJICA)) && tabla.polje[y - i][x].bojaFigure == BELO && JelUnutarTable(x, y - i))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y + i][x].figura, PRAZNO) || (!wcscmp(tabla.polje[y + i][x].figura, KRALJ) && tabla.polje[y + i][x].bojaFigure == CRNO)) && JelUnutarTable(x, y + i));
		else if ((!wcscmp(tabla.polje[y + i][x].figura, TOP) || !wcscmp(tabla.polje[y + i][x].figura, KRALJICA)) && tabla.polje[y + i][x].bojaFigure == BELO && JelUnutarTable(x, y + i))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y - i][x - i].figura, PRAZNO) || (!wcscmp(tabla.polje[y - i][x - i].figura, KRALJ) && tabla.polje[y - i][x - i].bojaFigure == CRNO)) && JelUnutarTable(x - i, y - i));
		else if ((!wcscmp(tabla.polje[y - i][x - i].figura, LOVAC) || !wcscmp(tabla.polje[y - i][x - i].figura, KRALJICA)) && tabla.polje[y - i][x - i].bojaFigure == BELO && JelUnutarTable(x - i, y - i))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y - i][x + i].figura, PRAZNO) || (!wcscmp(tabla.polje[y - i][x + i].figura, KRALJ) && tabla.polje[y - i][x + i].bojaFigure == CRNO)) && JelUnutarTable(x + i, y - i));
		else if ((!wcscmp(tabla.polje[y - i][x + i].figura, LOVAC) || !wcscmp(tabla.polje[y - i][x + i].figura, KRALJICA)) && tabla.polje[y - i][x + i].bojaFigure == BELO && JelUnutarTable(x + i, y - i))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y + i][x - i].figura, PRAZNO) || (!wcscmp(tabla.polje[y + i][x - i].figura, KRALJ) && tabla.polje[y + i][x - i].bojaFigure == CRNO)) && JelUnutarTable(x - i, y + i));
		else if ((!wcscmp(tabla.polje[y + i][x - i].figura, LOVAC) || !wcscmp(tabla.polje[y + i][x - i].figura, KRALJICA)) && tabla.polje[y + i][x - i].bojaFigure == BELO && JelUnutarTable(x - i, y + i))
			return 1;
		else
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y + i][x + i].figura, PRAZNO) || (!wcscmp(tabla.polje[y + i][x + i].figura, KRALJ) && tabla.polje[y + i][x + i].bojaFigure == CRNO)) && JelUnutarTable(x + i, y + i));
		else if ((!wcscmp(tabla.polje[y + i][x + i].figura, LOVAC) || !wcscmp(tabla.polje[y + i][x + i].figura, KRALJICA)) && tabla.polje[y + i][x + i].bojaFigure == BELO && JelUnutarTable(x + i, y + i))
			return 1;
		else
			break;
	}

	if (!wcscmp(tabla.polje[y][x - 1].figura, KRALJ) && tabla.polje[y][x - 1].bojaFigure == BELO && JelUnutarTable(x - 1, y))
		return 1;

	if (!wcscmp(tabla.polje[y][x + 1].figura, KRALJ) && tabla.polje[y][x + 1].bojaFigure == BELO && JelUnutarTable(x + 1, y))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x].figura, KRALJ) && tabla.polje[y - 1][x].bojaFigure == BELO && JelUnutarTable(x, y - 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x].figura, KRALJ) && tabla.polje[y + 1][x].bojaFigure == BELO && JelUnutarTable(x, y + 1))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x - 1].figura, KRALJ) && tabla.polje[y - 1][x - 1].bojaFigure == BELO && JelUnutarTable(x - 1, y - 1))
		return 1;

	if (!wcscmp(tabla.polje[y - 1][x + 1].figura, KRALJ) && tabla.polje[y - 1][x + 1].bojaFigure == BELO && JelUnutarTable(x + 1, y - 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x - 1].figura, KRALJ) && tabla.polje[y + 1][x - 1].bojaFigure == BELO && JelUnutarTable(x - 1, y + 1))
		return 1;

	if (!wcscmp(tabla.polje[y + 1][x + 1].figura, KRALJ) && tabla.polje[y + 1][x + 1].bojaFigure == BELO && JelUnutarTable(x + 1, y + 1))
		return 1;

	return 0;
}

short PostaviKralja(Tabla& tabla, int x, int y) {
	short ret = 0;

	tabla.polje[y][x].stanjePolja = IZABRANO;

	if (tabla.polje[y][x].bojaFigure == BELO) {
		if ((!wcscmp(tabla.polje[y - 1][x - 1].figura, PRAZNO) || !wcscmp(tabla.polje[y - 1][x - 1].figura, TACKA)) && JelUnutarTable(x - 1, y - 1) && !JelSahBeli(tabla, -1, -1)) {
			wcscpy(tabla.polje[y - 1][x - 1].figura, TACKA);
			tabla.polje[y - 1][x - 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y - 1][x - 1].figura, PRAZNO) && tabla.polje[y - 1][x - 1].bojaFigure == CRNO && JelUnutarTable(x - 1, y - 1) && !JelSahBeli(tabla, -1, -1)) {
			tabla.polje[y - 1][x - 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y - 1][x + 1].figura, PRAZNO) || !wcscmp(tabla.polje[y - 1][x + 1].figura, TACKA)) && JelUnutarTable(x + 1, y - 1) && !JelSahBeli(tabla, 1, -1)) {
			wcscpy(tabla.polje[y - 1][x + 1].figura, TACKA);
			tabla.polje[y - 1][x + 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y - 1][x + 1].figura, PRAZNO) && tabla.polje[y - 1][x + 1].bojaFigure == CRNO && JelUnutarTable(x + 1, y - 1) && !JelSahBeli(tabla, 1, -1)) {
			tabla.polje[y - 1][x + 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y + 1][x - 1].figura, PRAZNO) || !wcscmp(tabla.polje[y + 1][x - 1].figura, TACKA)) && JelUnutarTable(x - 1, y + 1) && !JelSahBeli(tabla, -1, 1)) {
			wcscpy(tabla.polje[y + 1][x - 1].figura, TACKA);
			tabla.polje[y + 1][x - 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y + 1][x - 1].figura, PRAZNO) && tabla.polje[y + 1][x - 1].bojaFigure == CRNO && JelUnutarTable(x - 1, y + 1) && !JelSahBeli(tabla, -1, 1)) {
			tabla.polje[y + 1][x - 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y + 1][x + 1].figura, PRAZNO) || !wcscmp(tabla.polje[y + 1][x + 1].figura, TACKA)) && JelUnutarTable(x + 1, y + 1) && !JelSahBeli(tabla, 1, 1)) {
			wcscpy(tabla.polje[y + 1][x + 1].figura, TACKA);
			tabla.polje[y + 1][x + 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y + 1][x + 1].figura, PRAZNO) && tabla.polje[y + 1][x + 1].bojaFigure == CRNO && JelUnutarTable(x + 1, y + 1) && !JelSahBeli(tabla, 1, 1)) {
			tabla.polje[y + 1][x + 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y][x + 1].figura, PRAZNO) || !wcscmp(tabla.polje[y][x + 1].figura, TACKA)) && JelUnutarTable(x + 1, y) && !JelSahBeli(tabla, 1, 0)) {
			wcscpy(tabla.polje[y][x + 1].figura, TACKA);
			tabla.polje[y][x + 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y][x + 1].figura, PRAZNO) && tabla.polje[y][x + 1].bojaFigure == CRNO && JelUnutarTable(x + 1, y) && !JelSahBeli(tabla, 1, 0)) {
			tabla.polje[y][x + 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y + 1][x].figura, PRAZNO) || !wcscmp(tabla.polje[y + 1][x].figura, TACKA)) && JelUnutarTable(x, y + 1) && !JelSahBeli(tabla, 0, 1)) {
			wcscpy(tabla.polje[y + 1][x].figura, TACKA);
			tabla.polje[y + 1][x].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y + 1][x].figura, PRAZNO) && tabla.polje[y + 1][x].bojaFigure == CRNO && JelUnutarTable(x, y + 1) && !JelSahBeli(tabla, 0, 1)) {
			tabla.polje[y + 1][x].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y][x - 1].figura, PRAZNO) || !wcscmp(tabla.polje[y][x - 1].figura, TACKA)) && JelUnutarTable(x - 1, y) && !JelSahBeli(tabla, -1, 0)) {
			wcscpy(tabla.polje[y][x - 1].figura, TACKA);
			tabla.polje[y][x - 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y][x - 1].figura, PRAZNO) && tabla.polje[y][x - 1].bojaFigure == CRNO && JelUnutarTable(x - 1, y) && !JelSahBeli(tabla, -1, 0)) {
			tabla.polje[y][x - 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y - 1][x].figura, PRAZNO) || !wcscmp(tabla.polje[y - 1][x].figura, TACKA)) && JelUnutarTable(x, y - 1) && !JelSahBeli(tabla, 0, -1)) {
			wcscpy(tabla.polje[y - 1][x].figura, TACKA);
			tabla.polje[y - 1][x].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y - 1][x].figura, PRAZNO) && tabla.polje[y - 1][x].bojaFigure == CRNO && JelUnutarTable(x, y - 1) && !JelSahBeli(tabla, 0, -1)) {
			tabla.polje[y - 1][x].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if (tabla.malarbeli == UKLJUCENO && !wcscmp(tabla.polje[7][5].figura, TACKA) && (!wcscmp(tabla.polje[7][6].figura, PRAZNO) || !wcscmp(tabla.polje[7][6].figura, TACKA)) && !JelSahBeli(tabla, 0, 0) && !JelSahBeli(tabla, 1, 0) && !JelSahBeli(tabla, 2, 0)) {
			wcscpy(tabla.polje[7][6].figura, TACKA);
			tabla.polje[7][6].bojaFigure = ZELENO;
			ret = 1;
		}

		if (tabla.velikarbeli == UKLJUCENO && !wcscmp(tabla.polje[7][3].figura, TACKA) && ((!wcscmp(tabla.polje[7][2].figura, PRAZNO) && !wcscmp(tabla.polje[7][1].figura, PRAZNO)) || (!wcscmp(tabla.polje[7][2].figura, TACKA) && !wcscmp(tabla.polje[7][1].figura, TACKA))) && !JelSahBeli(tabla, 0, 0) && !JelSahBeli(tabla, -1, 0) && !JelSahBeli(tabla, -2, 0) && !JelSahBeli(tabla, -3, 0)) {
			wcscpy(tabla.polje[7][1].figura, TACKA);
			tabla.polje[7][1].bojaFigure = ZELENO;
			ret = 1;
		}

	}
	else {
		if ((!wcscmp(tabla.polje[y - 1][x - 1].figura, PRAZNO) || !wcscmp(tabla.polje[y - 1][x - 1].figura, TACKA)) && JelUnutarTable(x - 1, y - 1) && !JelSahCrni(tabla, -1, -1)) {
			wcscpy(tabla.polje[y - 1][x - 1].figura, TACKA);
			tabla.polje[y - 1][x - 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y - 1][x - 1].figura, PRAZNO) && tabla.polje[y - 1][x - 1].bojaFigure == BELO && JelUnutarTable(x - 1, y - 1) && !JelSahCrni(tabla, -1, -1)) {
			tabla.polje[y - 1][x - 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y - 1][x + 1].figura, PRAZNO) || !wcscmp(tabla.polje[y - 1][x + 1].figura, TACKA)) && JelUnutarTable(x + 1, y - 1) && !JelSahCrni(tabla, 1, -1)) {
			wcscpy(tabla.polje[y - 1][x + 1].figura, TACKA);
			tabla.polje[y - 1][x + 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y - 1][x + 1].figura, PRAZNO) && tabla.polje[y - 1][x + 1].bojaFigure == BELO && JelUnutarTable(x + 1, y - 1) && !JelSahCrni(tabla, 1, -1)) {
			tabla.polje[y - 1][x + 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y + 1][x - 1].figura, PRAZNO) || !wcscmp(tabla.polje[y + 1][x - 1].figura, TACKA)) && JelUnutarTable(x - 1, y + 1) && !JelSahCrni(tabla, -1, 1)) {
			wcscpy(tabla.polje[y + 1][x - 1].figura, TACKA);
			tabla.polje[y + 1][x - 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y + 1][x - 1].figura, PRAZNO) && tabla.polje[y + 1][x - 1].bojaFigure == BELO && JelUnutarTable(x - 1, y + 1) && !JelSahCrni(tabla, -1, 1)) {
			tabla.polje[y + 1][x - 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y + 1][x + 1].figura, PRAZNO) || !wcscmp(tabla.polje[y + 1][x + 1].figura, TACKA)) && JelUnutarTable(x + 1, y + 1) && !JelSahCrni(tabla, 1, 1)) {
			wcscpy(tabla.polje[y + 1][x + 1].figura, TACKA);
			tabla.polje[y + 1][x + 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y + 1][x + 1].figura, PRAZNO) && tabla.polje[y + 1][x + 1].bojaFigure == BELO && JelUnutarTable(x + 1, y + 1) && !JelSahCrni(tabla, 1, 1)) {
			tabla.polje[y + 1][x + 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y][x + 1].figura, PRAZNO) || !wcscmp(tabla.polje[y][x + 1].figura, TACKA)) && JelUnutarTable(x + 1, y) && !JelSahCrni(tabla, 1, 0)) {
			wcscpy(tabla.polje[y][x + 1].figura, TACKA);
			tabla.polje[y][x + 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y][x + 1].figura, PRAZNO) && tabla.polje[y][x + 1].bojaFigure == BELO && JelUnutarTable(x + 1, y) && !JelSahCrni(tabla, 1, 0)) {
			tabla.polje[y][x + 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y + 1][x].figura, PRAZNO) || !wcscmp(tabla.polje[y + 1][x].figura, TACKA)) && JelUnutarTable(x, y + 1) && !JelSahCrni(tabla, 0, 1)) {
			wcscpy(tabla.polje[y + 1][x].figura, TACKA);
			tabla.polje[y + 1][x].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y + 1][x].figura, PRAZNO) && tabla.polje[y + 1][x].bojaFigure == BELO && JelUnutarTable(x, y + 1) && !JelSahCrni(tabla, 0, 1)) {
			tabla.polje[y + 1][x].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y][x - 1].figura, PRAZNO) || !wcscmp(tabla.polje[y][x - 1].figura, TACKA)) && JelUnutarTable(x - 1, y) && !JelSahCrni(tabla, -1, 0)) {
			wcscpy(tabla.polje[y][x - 1].figura, TACKA);
			tabla.polje[y][x - 1].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y][x - 1].figura, PRAZNO) && tabla.polje[y][x - 1].bojaFigure == BELO && JelUnutarTable(x - 1, y) && !JelSahCrni(tabla, -1, 0)) {
			tabla.polje[y][x - 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if ((!wcscmp(tabla.polje[y - 1][x].figura, PRAZNO) || !wcscmp(tabla.polje[y - 1][x].figura, TACKA)) && JelUnutarTable(x, y - 1) && !JelSahCrni(tabla, 0, -1)) {
			wcscpy(tabla.polje[y - 1][x].figura, TACKA);
			tabla.polje[y - 1][x].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y - 1][x].figura, PRAZNO) && tabla.polje[y - 1][x].bojaFigure == BELO && JelUnutarTable(x, y - 1) && !JelSahCrni(tabla, 0, -1)) {
			tabla.polje[y - 1][x].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if (tabla.malarcrni == UKLJUCENO && !wcscmp(tabla.polje[0][5].figura, TACKA) && (!wcscmp(tabla.polje[0][6].figura, PRAZNO) || !wcscmp(tabla.polje[0][6].figura, TACKA)) && !JelSahCrni(tabla, 0, 0) && !JelSahCrni(tabla, 1, 0) && !JelSahCrni(tabla, 2, 0)) {
			wcscpy(tabla.polje[0][6].figura, TACKA);
			tabla.polje[0][6].bojaFigure = ZELENO;
			ret = 1;
		}

		if (tabla.velikarcrni == UKLJUCENO && !wcscmp(tabla.polje[0][3].figura, TACKA) && ((!wcscmp(tabla.polje[0][2].figura, PRAZNO) && !wcscmp(tabla.polje[0][1].figura, PRAZNO)) || (!wcscmp(tabla.polje[0][2].figura, TACKA) && !wcscmp(tabla.polje[0][1].figura, TACKA))) && !JelSahCrni(tabla, 0, 0) && !JelSahCrni(tabla, -1, 0) && !JelSahCrni(tabla, -2, 0) && !JelSahCrni(tabla, -3, 0)) {
			wcscpy(tabla.polje[0][1].figura, TACKA);
			tabla.polje[0][1].bojaFigure = ZELENO;
			ret = 1;
		}
	}

	return ret;
}

short PostaviPijuna(Tabla& tabla, int x, int y) {
	short ret = 0;

	tabla.polje[y][x].stanjePolja = IZABRANO;

	if (tabla.polje[y][x].bojaFigure == BELO) {
		if ((!wcscmp(tabla.polje[y - 1][x].figura, PRAZNO) || !wcscmp(tabla.polje[y - 1][x].figura, TACKA)) && JelUnutarTable(x, y - 1) && !JelSahBeli(StaviFiguruNaXY(tabla, x, y - 1), 0, 0)) {
			wcscpy(tabla.polje[y - 1][x].figura, TACKA);
			tabla.polje[y - 1][x].bojaFigure = ZELENO;
			ret = 1;
		}

		if (y == 6 && (!wcscmp(tabla.polje[y - 1][x].figura, PRAZNO) || !wcscmp(tabla.polje[y - 1][x].figura, TACKA)) && (!wcscmp(tabla.polje[y - 2][x].figura, PRAZNO) || !wcscmp(tabla.polje[y - 2][x].figura, TACKA)) && JelUnutarTable(x, y - 2) && !JelSahBeli(StaviFiguruNaXY(tabla, x, y - 2), 0, 0)) {
			wcscpy(tabla.polje[y - 2][x].figura, TACKA);
			tabla.polje[y - 2][x].bojaFigure = ZELENO;
			ret = 1;
		}

		if (wcscmp(tabla.polje[y - 1][x + 1].figura, PRAZNO) && tabla.polje[y - 1][x + 1].bojaFigure == CRNO && JelUnutarTable(x + 1, y - 1) && !JelSahBeli(StaviFiguruNaXY(tabla, x + 1, y - 1), 0, 0)) {
			tabla.polje[y - 1][x + 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if (wcscmp(tabla.polje[y - 1][x - 1].figura, PRAZNO) && tabla.polje[y - 1][x - 1].bojaFigure == CRNO && JelUnutarTable(x - 1, y - 1) && !JelSahBeli(StaviFiguruNaXY(tabla, x - 1, y - 1), 0, 0)) {
			tabla.polje[y - 1][x - 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if (!wcscmp(tabla.polje[y][x + 1].figura, PIJUN) && tabla.polje[y][x + 1].bojaFigure == CRNO && tabla.polje[y][x + 1].anpasan == UKLJUCENO && JelUnutarTable(x + 1, y - 1) && !JelSahBeli(StaviFiguruNaXY(tabla, x + 1, y - 1), 0, 0)) {
			tabla.polje[y - 1][x + 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if (!wcscmp(tabla.polje[y][x - 1].figura, PIJUN) && tabla.polje[y][x - 1].bojaFigure == CRNO && tabla.polje[y][x - 1].anpasan == UKLJUCENO && JelUnutarTable(x - 1, y - 1) && !JelSahBeli(StaviFiguruNaXY(tabla, x - 1, y - 1), 0, 0)) {
			tabla.polje[y - 1][x - 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}
	}
	else {
		if ((!wcscmp(tabla.polje[y + 1][x].figura, PRAZNO) || !wcscmp(tabla.polje[y + 1][x].figura, TACKA)) && JelUnutarTable(x, y + 1) && !JelSahCrni(StaviFiguruNaXY(tabla, x, y + 1), 0, 0)) {
			wcscpy(tabla.polje[y + 1][x].figura, TACKA);
			tabla.polje[y + 1][x].bojaFigure = ZELENO;
			ret = 1;
		}

		if (y == 1 && (!wcscmp(tabla.polje[y + 1][x].figura, PRAZNO) || !wcscmp(tabla.polje[y + 1][x].figura, TACKA)) && (!wcscmp(tabla.polje[y + 2][x].figura, PRAZNO) || !wcscmp(tabla.polje[y + 2][x].figura, TACKA)) && JelUnutarTable(x, y + 2) && !JelSahCrni(StaviFiguruNaXY(tabla, x, y + 2), 0, 0)) {
			wcscpy(tabla.polje[y + 2][x].figura, TACKA);
			tabla.polje[y + 2][x].bojaFigure = ZELENO;
			ret = 1;
		}

		if (wcscmp(tabla.polje[y + 1][x + 1].figura, PRAZNO) && tabla.polje[y + 1][x + 1].bojaFigure == BELO && JelUnutarTable(x + 1, y + 1) && !JelSahCrni(StaviFiguruNaXY(tabla, x + 1, y + 1), 0, 0)) {
			tabla.polje[y + 1][x + 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if (wcscmp(tabla.polje[y + 1][x - 1].figura, PRAZNO) && tabla.polje[y + 1][x - 1].bojaFigure == BELO && JelUnutarTable(x - 1, y + 1) && !JelSahCrni(StaviFiguruNaXY(tabla, x - 1, y + 1), 0, 0)) {
			tabla.polje[y + 1][x - 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if (!wcscmp(tabla.polje[y][x - 1].figura, PIJUN) && tabla.polje[y][x - 1].bojaFigure == BELO && tabla.polje[y][x - 1].anpasan == UKLJUCENO && JelUnutarTable(x - 1, y + 1) && !JelSahCrni(StaviFiguruNaXY(tabla, x - 1, y + 1), 0, 0)) {
			tabla.polje[y + 1][x - 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}

		if (!wcscmp(tabla.polje[y][x + 1].figura, PIJUN) && tabla.polje[y][x + 1].bojaFigure == BELO && tabla.polje[y][x + 1].anpasan == UKLJUCENO && JelUnutarTable(x + 1, y + 1) && !JelSahCrni(StaviFiguruNaXY(tabla, x + 1, y + 1), 0, 0)) {
			tabla.polje[y + 1][x + 1].stanjePolja = NAPADNUTO;
			ret = 1;
		}
	}

	return ret;
}

short PostaviKonja(Tabla& tabla, int x, int y) {
	short ret = 0;
	tabla.polje[y][x].stanjePolja = IZABRANO;

	if ((!wcscmp(tabla.polje[y - 2][x - 1].figura, PRAZNO) || !wcscmp(tabla.polje[y - 2][x - 1].figura, TACKA)) && JelUnutarTable(x - 1, y - 2) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - 1, y - 2), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - 1, y - 2), 0, 0)))) {
		wcscpy(tabla.polje[y - 2][x - 1].figura, TACKA);
		tabla.polje[y - 2][x - 1].bojaFigure = ZELENO;
		ret = 1;
	}
	else if (wcscmp(tabla.polje[y - 2][x - 1].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y - 2][x - 1].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y - 2][x - 1].bojaFigure == BELO)) && JelUnutarTable(x - 1, y - 2) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - 1, y - 2), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - 1, y - 2), 0, 0)))) {
		tabla.polje[y - 2][x - 1].stanjePolja = NAPADNUTO;
		ret = 1;
	}

	if ((!wcscmp(tabla.polje[y - 2][x + 1].figura, PRAZNO) || !wcscmp(tabla.polje[y - 2][x + 1].figura, TACKA)) && JelUnutarTable(x + 1, y - 2) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + 1, y - 2), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + 1, y - 2), 0, 0)))) {
		wcscpy(tabla.polje[y - 2][x + 1].figura, TACKA);
		tabla.polje[y - 2][x + 1].bojaFigure = ZELENO;
		ret = 1;
	}
	else if (wcscmp(tabla.polje[y - 2][x + 1].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y - 2][x + 1].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y - 2][x + 1].bojaFigure == BELO)) && JelUnutarTable(x + 1, y - 2) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + 1, y - 2), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + 1, y - 2), 0, 0)))) {
		tabla.polje[y - 2][x + 1].stanjePolja = NAPADNUTO;
		ret = 1;
	}

	if ((!wcscmp(tabla.polje[y - 1][x + 2].figura, PRAZNO) || !wcscmp(tabla.polje[y - 1][x + 2].figura, TACKA)) && JelUnutarTable(x + 2, y - 1) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + 2, y - 1), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + 2, y - 1), 0, 0)))) {
		wcscpy(tabla.polje[y - 1][x + 2].figura, TACKA);
		tabla.polje[y - 1][x + 2].bojaFigure = ZELENO;
		ret = 1;
	}
	else if (wcscmp(tabla.polje[y - 1][x + 2].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y - 1][x + 2].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y - 1][x + 2].bojaFigure == BELO)) && JelUnutarTable(x + 2, y - 1) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + 2, y - 1), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + 2, y - 1), 0, 0)))) {
		tabla.polje[y - 1][x + 2].stanjePolja = NAPADNUTO;
		ret = 1;
	}

	if ((!wcscmp(tabla.polje[y + 1][x + 2].figura, PRAZNO) || !wcscmp(tabla.polje[y + 1][x + 2].figura, TACKA)) && JelUnutarTable(x + 2, y + 1) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + 2, y + 1), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + 2, y + 1), 0, 0)))) {
		wcscpy(tabla.polje[y + 1][x + 2].figura, TACKA);
		tabla.polje[y + 1][x + 2].bojaFigure = ZELENO;
		ret = 1;
	}
	else if (wcscmp(tabla.polje[y + 1][x + 2].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y + 1][x + 2].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y + 1][x + 2].bojaFigure == BELO)) && JelUnutarTable(x + 2, y + 1) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + 2, y + 1), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + 2, y + 1), 0, 0)))) {
		tabla.polje[y + 1][x + 2].stanjePolja = NAPADNUTO;
		ret = 1;
	}

	if ((!wcscmp(tabla.polje[y + 2][x + 1].figura, PRAZNO) || !wcscmp(tabla.polje[y + 2][x + 1].figura, TACKA)) && JelUnutarTable(x + 1, y + 2) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + 1, y + 2), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + 1, y + 2), 0, 0)))) {
		wcscpy(tabla.polje[y + 2][x + 1].figura, TACKA);
		tabla.polje[y + 2][x + 1].bojaFigure = ZELENO;
		ret = 1;
	}
	else if (wcscmp(tabla.polje[y + 2][x + 1].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y + 2][x + 1].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y + 2][x + 1].bojaFigure == BELO)) && JelUnutarTable(x + 1, y + 2) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + 1, y + 2), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + 1, y + 2), 0, 0)))) {
		tabla.polje[y + 2][x + 1].stanjePolja = NAPADNUTO;
		ret = 1;
	}

	if ((!wcscmp(tabla.polje[y + 2][x - 1].figura, PRAZNO) || !wcscmp(tabla.polje[y + 2][x - 1].figura, TACKA)) && JelUnutarTable(x - 1, y + 2) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - 1, y + 2), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - 1, y + 2), 0, 0)))) {
		wcscpy(tabla.polje[y + 2][x - 1].figura, TACKA);
		tabla.polje[y + 2][x - 1].bojaFigure = ZELENO;
		ret = 1;
	}
	else if (wcscmp(tabla.polje[y + 2][x - 1].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y + 2][x - 1].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y + 2][x - 1].bojaFigure == BELO)) && JelUnutarTable(x - 1, y + 2) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - 1, y + 2), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - 1, y + 2), 0, 0)))) {
		tabla.polje[y + 2][x - 1].stanjePolja = NAPADNUTO;
		ret = 1;
	}

	if ((!wcscmp(tabla.polje[y + 1][x - 2].figura, PRAZNO) || !wcscmp(tabla.polje[y + 1][x - 2].figura, TACKA)) && JelUnutarTable(x - 2, y + 1) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - 2, y + 1), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - 2, y + 1), 0, 0)))) {
		wcscpy(tabla.polje[y + 1][x - 2].figura, TACKA);
		tabla.polje[y + 1][x - 2].bojaFigure = ZELENO;
		ret = 1;
	}
	else if (wcscmp(tabla.polje[y + 1][x - 2].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y + 1][x - 2].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y + 1][x - 2].bojaFigure == BELO)) && JelUnutarTable(x - 2, y + 1) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - 2, y + 1), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - 2, y + 1), 0, 0)))) {
		tabla.polje[y + 1][x - 2].stanjePolja = NAPADNUTO;
		ret = 1;
	}

	if ((!wcscmp(tabla.polje[y - 1][x - 2].figura, PRAZNO) || !wcscmp(tabla.polje[y - 1][x - 2].figura, TACKA)) && JelUnutarTable(x - 2, y - 1) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - 2, y - 1), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - 2, y - 1), 0, 0)))) {
		wcscpy(tabla.polje[y - 1][x - 2].figura, TACKA);
		tabla.polje[y - 1][x - 2].bojaFigure = ZELENO;
		ret = 1;
	}
	else if (wcscmp(tabla.polje[y - 1][x - 2].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y - 1][x - 2].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y - 1][x - 2].bojaFigure == BELO)) && JelUnutarTable(x - 2, y - 1) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - 2, y - 1), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - 2, y - 1), 0, 0)))) {
		tabla.polje[y - 1][x - 2].stanjePolja = NAPADNUTO;
		ret = 1;
	}

	return ret;
}

short PostaviTopa(Tabla& tabla, int x, int y) {
	short ret = 0;

	tabla.polje[y][x].stanjePolja = IZABRANO;

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y][x - i].figura, PRAZNO) || !wcscmp(tabla.polje[y][x - i].figura, TACKA)) && JelUnutarTable(x - i, y) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - i, y), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - i, y), 0, 0)))) {
			wcscpy(tabla.polje[y][x - i].figura, TACKA);
			tabla.polje[y][x - i].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y][x - i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y][x - i].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y][x - i].bojaFigure == BELO)) && JelUnutarTable(x - i, y) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - i, y), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - i, y), 0, 0)))) {
			tabla.polje[y][x - i].stanjePolja = NAPADNUTO;
			ret = 1;
			break;
		}
		else if (wcscmp(tabla.polje[y][x - i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y][x - i].bojaFigure == BELO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y][x - i].bojaFigure == CRNO)) && JelUnutarTable(x - i, y))
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y][x + i].figura, PRAZNO) || !wcscmp(tabla.polje[y][x + i].figura, TACKA)) && JelUnutarTable(x + i, y) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + i, y), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + i, y), 0, 0)))) {
			wcscpy(tabla.polje[y][x + i].figura, TACKA);
			tabla.polje[y][x + i].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y][x + i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y][x + i].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y][x + i].bojaFigure == BELO)) && JelUnutarTable(x + i, y) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + i, y), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + i, y), 0, 0)))) {
			tabla.polje[y][x + i].stanjePolja = NAPADNUTO;
			ret = 1;
			break;
		}
		else if (wcscmp(tabla.polje[y][x + i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y][x + i].bojaFigure == BELO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y][x + i].bojaFigure == CRNO)) && JelUnutarTable(x + i, y))
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y - i][x].figura, PRAZNO) || !wcscmp(tabla.polje[y - i][x].figura, TACKA)) && JelUnutarTable(x, y - i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x, y - i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x, y - i), 0, 0)))) {
			wcscpy(tabla.polje[y - i][x].figura, TACKA);
			tabla.polje[y - i][x].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y - i][x].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y - i][x].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y - i][x].bojaFigure == BELO)) && JelUnutarTable(x, y - i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x, y - i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x, y - i), 0, 0)))) {
			tabla.polje[y - i][x].stanjePolja = NAPADNUTO;
			ret = 1;
			break;
		}
		else if (wcscmp(tabla.polje[y - i][x].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y - i][x].bojaFigure == BELO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y - i][x].bojaFigure == CRNO)) && JelUnutarTable(x, y - i))
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y + i][x].figura, PRAZNO) || !wcscmp(tabla.polje[y + i][x].figura, TACKA)) && JelUnutarTable(x, y + i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x, y + i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x, y + i), 0, 0)))) {
			wcscpy(tabla.polje[y + i][x].figura, TACKA);
			tabla.polje[y + i][x].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y + i][x].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y + i][x].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y + i][x].bojaFigure == BELO)) && JelUnutarTable(x, y + i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x, y + i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x, y + i), 0, 0)))) {
			tabla.polje[y + i][x].stanjePolja = NAPADNUTO;
			ret = 1;
			break;
		}
		else if (wcscmp(tabla.polje[y + i][x].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y + i][x].bojaFigure == BELO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y + i][x].bojaFigure == CRNO)) && JelUnutarTable(x, y + i))
			break;
	}

	return ret;
}

short PostaviLovca(Tabla& tabla, int x, int y) {
	short ret = 0;

	tabla.polje[y][x].stanjePolja = IZABRANO;

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y - i][x - i].figura, PRAZNO) || !wcscmp(tabla.polje[y - i][x - i].figura, TACKA)) && JelUnutarTable(x - i, y - i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - i, y - i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - i, y - i), 0, 0)))) {
			wcscpy(tabla.polje[y - i][x - i].figura, TACKA);
			tabla.polje[y - i][x - i].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y - i][x - i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y - i][x - i].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y - i][x - i].bojaFigure == BELO)) && JelUnutarTable(x - i, y - i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - i, y - i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - i, y - i), 0, 0)))) {
			tabla.polje[y - i][x - i].stanjePolja = NAPADNUTO;
			ret = 1;
			break;
		}
		else if (wcscmp(tabla.polje[y - i][x - i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y - i][x - i].bojaFigure == BELO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y - i][x - i].bojaFigure == CRNO)) && JelUnutarTable(x - i, y - i))
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y - i][x + i].figura, PRAZNO) || !wcscmp(tabla.polje[y - i][x + i].figura, TACKA)) && JelUnutarTable(x + i, y - i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + i, y - i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + i, y - i), 0, 0)))) {
			wcscpy(tabla.polje[y - i][x + i].figura, TACKA);
			tabla.polje[y - i][x + i].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y - i][x + i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y - i][x + i].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y - i][x + i].bojaFigure == BELO)) && JelUnutarTable(x + i, y - i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + i, y - i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + i, y - i), 0, 0)))) {
			tabla.polje[y - i][x + i].stanjePolja = NAPADNUTO;
			ret = 1;
			break;
		}
		else if (wcscmp(tabla.polje[y - i][x + i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y - i][x + i].bojaFigure == BELO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y - i][x + i].bojaFigure == CRNO)) && JelUnutarTable(x + i, y - i))
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y + i][x - i].figura, PRAZNO) || !wcscmp(tabla.polje[y + i][x - i].figura, TACKA)) && JelUnutarTable(x - i, y + i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - i, y + i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - i, y + i), 0, 0)))) {
			wcscpy(tabla.polje[y + i][x - i].figura, TACKA);
			tabla.polje[y + i][x - i].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y + i][x - i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y + i][x - i].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y + i][x - i].bojaFigure == BELO)) && JelUnutarTable(x - i, y + i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x - i, y + i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x - i, y + i), 0, 0)))) {
			tabla.polje[y + i][x - i].stanjePolja = NAPADNUTO;
			ret = 1;
			break;
		}
		else if (wcscmp(tabla.polje[y + i][x - i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y + i][x - i].bojaFigure == BELO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y + i][x - i].bojaFigure == CRNO)) && JelUnutarTable(x - i, y + i))
			break;
	}

	for (int i = 1; i < 8; i++) {
		if ((!wcscmp(tabla.polje[y + i][x + i].figura, PRAZNO) || !wcscmp(tabla.polje[y + i][x + i].figura, TACKA)) && JelUnutarTable(x + i, y + i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + i, y + i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + i, y + i), 0, 0)))) {
			wcscpy(tabla.polje[y + i][x + i].figura, TACKA);
			tabla.polje[y + i][x + i].bojaFigure = ZELENO;
			ret = 1;
		}
		else if (wcscmp(tabla.polje[y + i][x + i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y + i][x + i].bojaFigure == CRNO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y + i][x + i].bojaFigure == BELO)) && JelUnutarTable(x + i, y + i) && ((tabla.polje[y][x].bojaFigure == BELO && !JelSahBeli(StaviFiguruNaXY(tabla, x + i, y + i), 0, 0)) || (tabla.polje[y][x].bojaFigure == CRNO && !JelSahCrni(StaviFiguruNaXY(tabla, x + i, y + i), 0, 0)))) {
			tabla.polje[y + i][x + i].stanjePolja = NAPADNUTO;
			ret = 1;
			break;
		}
		else if (wcscmp(tabla.polje[y + i][x + i].figura, PRAZNO) && ((tabla.polje[y][x].bojaFigure == BELO && tabla.polje[y + i][x + i].bojaFigure == BELO) || (tabla.polje[y][x].bojaFigure == CRNO && tabla.polje[y + i][x + i].bojaFigure == CRNO)) && JelUnutarTable(x + i, y + i))
			break;
	}

	return ret;
}

short PostaviKraljicu(Tabla& tabla, int x, int y) {
	int ret = 0;
	ret += PostaviLovca(tabla, x, y);
	ret += PostaviTopa(tabla, x, y);

	if (ret != 0)
		ret = 1;

	return ret;
}

short JelMozeIgratiBeli(Tabla tabla) {
	short ret = 0;

	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			if (tabla.polje[y][x].bojaFigure == BELO) {
				if (!wcscmp(tabla.polje[y][x].figura, KRALJ)) {
					ret += PostaviKralja(tabla, x, y);

					NoviPotez(tabla);
				}
				else if (!wcscmp(tabla.polje[y][x].figura, PIJUN)) {
					ret += PostaviPijuna(tabla, x, y);

					NoviPotez(tabla);
				}
				else if (!wcscmp(tabla.polje[y][x].figura, KONJ)) {
					ret += PostaviKonja(tabla, x, y);

					NoviPotez(tabla);
				}
				else if (!wcscmp(tabla.polje[y][x].figura, TOP)) {
					ret += PostaviTopa(tabla, x, y);

					NoviPotez(tabla);
				}
				else if (!wcscmp(tabla.polje[y][x].figura, LOVAC)) {
					ret += PostaviLovca(tabla, x, y);

					NoviPotez(tabla);
				}
				else if (!wcscmp(tabla.polje[y][x].figura, KRALJICA)) {
					ret += PostaviKraljicu(tabla, x, y);

					NoviPotez(tabla);
				}
			}

	if (ret != 0)
		ret = 1;

	return ret;
}

short JelMozeIgratiCrni(Tabla tabla) {
	short ret = 0;

	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			if (tabla.polje[y][x].bojaFigure == CRNO) {
				if (!wcscmp(tabla.polje[y][x].figura, KRALJ)) {
					ret += PostaviKralja(tabla, x, y);

					NoviPotez(tabla);
				}
				else if (!wcscmp(tabla.polje[y][x].figura, PIJUN)) {
					ret += PostaviPijuna(tabla, x, y);

					NoviPotez(tabla);
				}
				else if (!wcscmp(tabla.polje[y][x].figura, KONJ)) {
					ret += PostaviKonja(tabla, x, y);

					NoviPotez(tabla);
				}
				else if (!wcscmp(tabla.polje[y][x].figura, TOP)) {
					ret += PostaviTopa(tabla, x, y);

					NoviPotez(tabla);
				}
				else if (!wcscmp(tabla.polje[y][x].figura, LOVAC)) {
					ret += PostaviLovca(tabla, x, y);

					NoviPotez(tabla);
				}
				else if (!wcscmp(tabla.polje[y][x].figura, KRALJICA)) {
					ret += PostaviKraljicu(tabla, x, y);

					NoviPotez(tabla);
				}
			}

	if (ret != 0)
		ret = 1;

	return ret;
}

short ZameniPijuna() {
	int x, y, pom = 1, izb;

	_setmode(_fileno(stdout), _O_U8TEXT);

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 82);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 101);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 152);

	gotoxy(8, 14);
	wprintf(KRALJICA);
	gotoxy(14, 14);
	wprintf(TOP);
	gotoxy(20, 14);
	wprintf(LOVAC);
	gotoxy(26, 14);
	wprintf(KONJ);

	while (pom) {
		pom = 0;
		UcitajKlik(x, y);

		if ((x == 8 || x == 9) && y == 14)
			izb = 0;
		else if ((x == 14 || x == 15) && y == 14)
			izb = 1;
		else if ((x == 20 || x == 21) && y == 14)
			izb = 2;
		else if ((x == 26 || x == 27) && y == 14)
			izb = 3;
		else
			pom = 1;
	}

	ObrisiTerminal();

	return izb;
}

int FiguraUBroj(wchar_t figura[]) {
	_setmode(_fileno(stdout), _O_U8TEXT);

	if (!wcscmp(figura, KRALJ))
		return 0;
	else if (!wcscmp(figura, KRALJICA))
		return 1;
	else if (!wcscmp(figura, TOP))
		return 2;
	else if (!wcscmp(figura, LOVAC))
		return 3;
	else if (!wcscmp(figura, KONJ))
		return 4;
	else if (!wcscmp(figura, PIJUN))
		return 5;
	else if (!wcscmp(figura, PRAZNO))
		return 6;
	else if (!wcscmp(figura, TACKA))
		return 7;

	return 6;
}

const wchar_t* BrojUFiguru(int broj) {
	_setmode(_fileno(stdout), _O_U8TEXT);

	switch (broj) {
	case 0:
		return KRALJ;

	case 1:
		return KRALJICA;

	case 2:
		return TOP;

	case 3:
		return LOVAC;

	case 4:
		return KONJ;

	case 5:
		return PIJUN;

	case 6:
		return PRAZNO;

	case 7:
		return TACKA;
	}

	return PRAZNO;
}

vector<short> NapraviSacuvaj() {
	vector<short> memo;
	memo.clear();

	for (int a = 0; a <= brPoteza; a++) {
		memo.push_back(istorija[a].brdesno);
		memo.push_back(istorija[a].brlevo);
		memo.push_back(istorija[a].krajigre);
		memo.push_back(istorija[a].malarbeli);
		memo.push_back(istorija[a].malarcrni);
		memo.push_back(istorija[a].velikarbeli);
		memo.push_back(istorija[a].velikarcrni);

		for (int i = 0; i < 16; i++) {
			memo.push_back(FiguraUBroj(istorija[a].levo[i / 2][i % 2].figura));
			memo.push_back(FiguraUBroj(istorija[a].desno[i / 2][i % 2].figura));
		}

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				memo.push_back(istorija[a].polje[i][j].anpasan);
				memo.push_back(istorija[a].polje[i][j].bojaFigure);
				memo.push_back(FiguraUBroj(istorija[a].polje[i][j].figura));
				memo.push_back(istorija[a].polje[i][j].stanjePolja);
			}
	}

	return memo;
}

void SacuvajIgru() {
	wchar_t ime[18], zn;
	vector<short> memo;
	ObrisiTerminal();
	SetConsoleCursorInfo(ConsoleOutputHandle, &kursor);

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 82);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 101);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 152);

	_setmode(_fileno(stdout), _O_TEXT);

	gotoxy(4, 14);
	printf("Ime igre:  ");

	for (int i = 0; i < 18; i++) {
		fflush(stdin);
		zn = _getwch();
		fflush(stdin);

		if ((zn == 13 && i != 0) || i == 14) {
			Sleep(100);
			ime[i] = '\0';
			break;
		}
		else if (zn == 13) {
			ObrisiTerminal();
			SakriKursor();
			return;
		}
		else if (zn == 8) {
			gotoxy(14 + i, 14);
			_putwch(L' ');
			gotoxy(14 + i, 14);
			i -= 2;
		}
		else {
			gotoxy(15 + i, 14);
			_putwch(zn);
			ime[i] = zn;
		}
	}

	ObrisiTerminal();
	SakriKursor();

	memo = NapraviSacuvaj();

	ofstream izlaz(wcscat(ime, L".igra"));

	if (izlaz.is_open()) {
		izlaz << brPoteza;

		for (int i = 0; i < (brPoteza + 1) * 295; i++) {
			if (!(i % 295))
				izlaz << endl;

			izlaz << memo[i] << " ";
		}

		izlaz.close();
	}

	memo.clear();
}

void OdaberiVelicinu() {
	velicina = (velicina - 15) / 2;
	ObrisiTerminal();

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 82);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 101);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 152);

	_setmode(_fileno(stdout), _O_U8TEXT);

	if (velicina > 9)
		gotoxy(30, 14);
	else
		gotoxy(31, 14);

	wprintf(L"%d", velicina);

	gotoxy(4, 14);
	wprintf(L"Veličina: ");

	for (int i = 1; i < velicina; i++) {
		if (boja == SVETLO)
			SetConsoleTextAttribute(ConsoleOutputHandle, 51);
		else if (boja == TAMNO)
			SetConsoleTextAttribute(ConsoleOutputHandle, 68);
		else if (boja == SIVO)
			SetConsoleTextAttribute(ConsoleOutputHandle, 170);

		wprintf(L"█");
	}

	if (boja == SVETLO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 102);
	else if (boja == TAMNO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 119);
	else if (boja == SIVO)
		SetConsoleTextAttribute(ConsoleOutputHandle, 17);

	wprintf(L"█");

	for (int i = 15; i > velicina; i--) {
		if (boja == SVETLO)
			SetConsoleTextAttribute(ConsoleOutputHandle, 34);
		else if (boja == TAMNO)
			SetConsoleTextAttribute(ConsoleOutputHandle, 85);
		else if (boja == SIVO)
			SetConsoleTextAttribute(ConsoleOutputHandle, 136);

		wprintf(L"█");
	}

	DWORD info;
	INPUT_RECORD input;
	SetConsoleMode(ConsoleInputHandle, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

GOBACK1:
	ReadConsoleInput(ConsoleInputHandle, &input, 1, &info);

	if (input.Event.MouseEvent.dwButtonState != 0x0001) {
	GOBACK2:
		ReadConsoleInput(ConsoleInputHandle, &input, 1, &info);

		if (input.Event.MouseEvent.dwButtonState == 0x0001 && input.Event.MouseEvent.dwMousePosition.Y == 14 && input.Event.MouseEvent.dwMousePosition.X > 13 && input.Event.MouseEvent.dwMousePosition.X < 29) {
		GOBACK3:
			ReadConsoleInput(ConsoleInputHandle, &input, 1, &info);

			velicina = input.Event.MouseEvent.dwMousePosition.X - 13;

			if (velicina < 1)
				velicina = 1;

			if (velicina > 15)
				velicina = 15;

			if (boja == SVETLO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 82);
			else if (boja == TAMNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 101);
			else if (boja == SIVO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 152);

			gotoxy(30, 14);

			if (velicina < 10)
				wprintf(L" ");

			wprintf(L"%d", velicina);

			gotoxy(14, 14);

			for (int i = 1; i < velicina; i++) {
				if (boja == SVETLO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 51);
				else if (boja == TAMNO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 68);
				else if (boja == SIVO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 170);

				wprintf(L"█");
			}

			if (boja == SVETLO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 102);
			else if (boja == TAMNO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 119);
			else if (boja == SIVO)
				SetConsoleTextAttribute(ConsoleOutputHandle, 17);

			wprintf(L"█");

			for (int i = 15; i > velicina; i--) {
				if (boja == SVETLO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 34);
				else if (boja == TAMNO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 85);
				else if (boja == SIVO)
					SetConsoleTextAttribute(ConsoleOutputHandle, 136);

				wprintf(L"█");
			}

			if (input.Event.MouseEvent.dwButtonState == 0x0001)
				goto GOBACK3;
		}
		else if (input.Event.MouseEvent.dwButtonState == 0x0001) {

		GOBACK4:
			ReadConsoleInput(ConsoleInputHandle, &input, 1, &info);

			if (input.Event.MouseEvent.dwButtonState == 0x0001 && input.Event.MouseEvent.dwMousePosition.Y == 14 && input.Event.MouseEvent.dwMousePosition.X > 13 && input.Event.MouseEvent.dwMousePosition.X < 29)
				goto GOBACK3;
			else if (input.Event.MouseEvent.dwButtonState == 0x0001)
				goto GOBACK4;

			ObrisiTerminal();
			velicina = velicina * 2 + 15;
			return;
		}
		else
			goto GOBACK2;
	}
	else
		goto GOBACK1;

	Sleep(300);
	ObrisiTerminal();
	velicina = velicina * 2 + 15;
}

/*void AnimacijaUcitaj()
{
	system("MODE 36, 18");
	PodesiVelicinu(velicina);

	HWND consoleWindow = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(consoleWindow, FALSE);

	winlog = GetWindowLong(consoleWindow, GWL_STYLE);
	SetWindowLong(consoleWindow, GWL_STYLE, winlog & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_VSCROLL);
	SetConsoleMode(ConsoleInputHandle, ENABLE_EXTENDED_FLAGS);
	_setmode(_fileno(stdout), _O_U8TEXT);
	SetConsoleTitleW(L"ŠAH");
	_CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(info);
	GetConsoleScreenBufferInfoEx(ConsoleOutputHandle, &info);
	info.ColorTable[2] = RGB(198, 175, 141);
	info.ColorTable[3] = RGB(114, 71, 56);
	SetConsoleScreenBufferInfoEx(ConsoleOutputHandle, &info);
	SakriKursor();

	SetConsoleTextAttribute(ConsoleOutputHandle, 35);
	wprintf(L"\n");

	char str[20];

	for (int i = 1; i < 18; i++)
	{
		sprintf(str, "MODE 36, %d", i);
		system(str);
		if (_kbhit() || EXITind)
			return;
	}

	Sleep(200);
	if (_kbhit() || EXITind)
		return;

	gotoxy(6, 2);
	wprintf(L"Najbolja igrica na svetu");
	gotoxy(12, 4);
	wprintf(L"se učitava...");

	for (int i = 0; i < 2; i++)
	{
		if (_kbhit() || EXITind)
			return;

		gotoxy(17, 8);
		wprintf(L"● ");
		Sleep(250);

		if (_kbhit() || EXITind)
			return;

		gotoxy(21, 9);
		wprintf(L"● ");
		Sleep(220);

		if (_kbhit() || EXITind)
			return;

		gotoxy(23, 11);
		wprintf(L"● ");
		Sleep(150);

		if (_kbhit() || EXITind)
			return;

		gotoxy(21, 13);
		wprintf(L"● ");
		Sleep(70);

		if (_kbhit() || EXITind)
			return;

		gotoxy(17, 14);
		wprintf(L"● ");
		Sleep(90);

		if (_kbhit() || EXITind)
			return;

		gotoxy(13, 13);
		wprintf(L"● ");
		Sleep(105);

		if (_kbhit() || EXITind)
			return;

		gotoxy(11, 11);
		wprintf(L"● ");
		Sleep(120);

		if (_kbhit() || EXITind)
			return;

		gotoxy(13, 9);
		wprintf(L"● ");
		Sleep(260);

		if (_kbhit() || EXITind)
			return;

		gotoxy(21, 9);
		wprintf(L"  ");
		Sleep(250);

		if (_kbhit() || EXITind)
			return;

		gotoxy(23, 11);
		wprintf(L"  ");
		Sleep(220);

		if (_kbhit() || EXITind)
			return;

		gotoxy(21, 13);
		wprintf(L"  ");
		Sleep(150);

		if (_kbhit() || EXITind)
			return;

		gotoxy(17, 14);
		wprintf(L"  ");
		Sleep(70);

		if (_kbhit() || EXITind)
			return;

		gotoxy(13, 13);
		wprintf(L"  ");
		Sleep(90);

		if (_kbhit() || EXITind)
			return;

		gotoxy(11, 11);
		wprintf(L"  ");
		Sleep(105);

		if (_kbhit() || EXITind)
			return;

		gotoxy(13, 9);
		wprintf(L"  ");
		Sleep(260);

		if (_kbhit() || EXITind)
			return;

		if (i)
		{
			if (_kbhit() || EXITind)
				return;

			gotoxy(17, 8);
			wprintf(L"● ");
			Sleep(250);

			if (_kbhit() || EXITind)
				return;

			gotoxy(21, 9);
			wprintf(L"● ");
			Sleep(220);

			if (_kbhit() || EXITind)
				return;

			gotoxy(23, 11);
			wprintf(L"● ");
			Sleep(150);

			if (_kbhit() || EXITind)
				return;

			gotoxy(21, 13);
			wprintf(L"● ");

			Sleep(550);
		}
	}
	return;
} */

BOOL WINAPI EXIT(DWORD CEvent) {
	if (CEvent == CTRL_CLOSE_EVENT) {
		EXITind = 1;
		_setmode(_fileno(stdout), _O_U8TEXT);

		istorija.clear();

		if (boja == SVETLO)
			SetConsoleTextAttribute(ConsoleOutputHandle, 35);
		else if (boja == TAMNO)
			SetConsoleTextAttribute(ConsoleOutputHandle, 84);
		else if (boja == SIVO)
			SetConsoleTextAttribute(ConsoleOutputHandle, 138);

		for (int i = 0; i < 30; i++)
			wprintf(L"\n");

		gotoxy(13, 6);
		wprintf(L"DOVIĐENJA!");

		HKEY key;
		RegOpenKeyA(HKEY_CURRENT_USER, "Console", &key);
		RegDeleteTreeA(key, NULL);
		system("reg import C:\\temp\\RegistryBackup.reg >nul 2>&1");

		Sleep(999);
	}
	return TRUE;
}

void SacuvajConsoleKey() {
	system("reg export HKCU\\Console C:\\temp\\RegistryBackup.reg /y >nul 2>&1");
}