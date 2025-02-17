#include <fstream>
#include <typeinfo>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

struct taisykles {
	string eBusena;
	char eSimbolis;
	char nSimbolis;
	char kryptis;
	string nBusena;
};
struct taisykles2 {
	string eBusena;
	char eSimbolisT1;
	char eSimbolisT2;
	char nSimbolisT1;
	char nSimbolisT2;
	char kryptisT1;
	char kryptisT2;
	string nBusena;
};

atomic<bool> stopRequested(false);

void dokumentoSkaitymas2(const string& filename, vector <taisykles2>& taisykles2Vek, string& juosta1, string& juosta2, int& juostuKiek, int& pozicija1, int& pozicija2, string& dabartineBusena);
void isvedimas2(vector <taisykles2>& taisykles2Vek, string& juosta1, string& juosta2, int& juostuKiek, int& pozicija1, int& pozicija2, string& dabartineBusena);

void dokumentoSkaitymas(const string& filename, vector<taisykles>& taisyklesVek, string& juosta, int& juostuKiek, int& pozicija, string& dabartineBusena);
void isvedimas( vector<taisykles>& taisyklesVek, string& juosta, int& juostuKiek, int& pozicija, string& dabartineBusena);

void stopButton() {
	//cout << "Paspauskit enter " << endl;
	cin.get();
	stopRequested = true;
	if (stopRequested)
		cout << "Sustabdyta"<<endl;
}

int main(int argc, char* argv[])
{
	vector<taisykles> taisyklesVek;
	vector<taisykles2> taisykles2Vek;

	int juostuKiek = 0, pozicija = 0, pozicija1 = 0, pozicija2 = 0;
	string juosta, juosta1, juosta2;
	string pradineBusena;

	int n = 0;
	if (argc < 2) {
		cout << "Nepateiktas argumentas";
		return 1;
	}
	ifstream dokumentas(argv[1]);
	if (!dokumentas) {
		cout << "Nepavyko atidaryti failo " << argv[1] << endl;
		return 1;
	}
	dokumentas >> juostuKiek;
	dokumentas.close();

	thread stopThread(stopButton);

	if (juostuKiek == 1) {
		dokumentoSkaitymas(argv[1], taisyklesVek, juosta, juostuKiek, pozicija, pradineBusena);
		isvedimas(taisyklesVek, juosta, juostuKiek, pozicija, pradineBusena);
	}
	else if (juostuKiek == 2) {
		dokumentoSkaitymas2(argv[1], taisykles2Vek, juosta1, juosta2, juostuKiek, pozicija1, pozicija2, pradineBusena);
		isvedimas2(taisykles2Vek, juosta1, juosta2, juostuKiek, pozicija1, pozicija2, pradineBusena); 
	}

	stopThread.join();

	return 0;
}
void dokumentoSkaitymas(const string& filename, vector<taisykles>& taisyklesVek, string& juosta, int& juostuKiek, int& pozicija, string& pradineBusena) {

	ifstream dokumentas(filename);

	if (!dokumentas) {
		cout << "Nepavyo atidaryti failo " << filename << endl;
		return;
	}

	dokumentas >> juostuKiek;
	dokumentas >> juosta;
	dokumentas >> pozicija;
	pozicija -= 1;

	taisykles temp;

	if (dokumentas >> temp.eBusena >> temp.eSimbolis >> temp.nSimbolis >> temp.kryptis >> temp.nBusena) {
		pradineBusena = temp.eBusena;
		taisyklesVek.push_back(temp);
	}
	while (dokumentas >> temp.eBusena >> temp.eSimbolis >> temp.nSimbolis >> temp.kryptis >> temp.nBusena) {
		taisyklesVek.push_back(temp);
	}

}
void dokumentoSkaitymas2(const string& filename, vector <taisykles2>& taisykles2Vek, string& juosta1, string& juosta2, int& juostuKiek, int& pozicija1, int& pozicija2, string& pradineBusena) {
	ifstream dokumentas(filename);
	if (!dokumentas) {
		cout << "Nepavyo atidaryti failo " << filename << endl;
		return;
	}
	dokumentas >> juostuKiek;
	dokumentas >> juosta1;
	dokumentas >> juosta2;
	dokumentas >> pozicija1;
	dokumentas >> pozicija2;
	pozicija1 -= 1;
	pozicija2 -= 1;

	taisykles2 temp;
	pradineBusena = "0";

	while (dokumentas >> temp.eBusena >> temp.eSimbolisT1 >> temp.eSimbolisT2 >> temp.nSimbolisT1 >> temp.nSimbolisT2 >> temp.kryptisT1 >> temp.kryptisT2 >> temp.nBusena) {
		taisykles2Vek.push_back(temp);
		//cout << temp.eBusena << "#" << temp.eSimbolisT1 << "#" << temp.eSimbolisT2 << "#" << temp.nSimbolisT1 << "#" << temp.nSimbolisT2 << "#" << temp.kryptisT1 << "#" << temp.kryptisT2 << "#" << temp.nBusena << endl;
	}

}
void isvedimas(vector<taisykles>& taisyklesVek, string& juosta, int& juostuKiek, int& pozicija, string& pradineBusena) {
	
		bool taisykleRasta = true;
		int count = 0;
		while (taisykleRasta && !stopRequested) {

			taisykleRasta = false;
			string senaJuosta = juosta;
		
			for (const auto& taisykle : taisyklesVek) {
				if (taisykle.eBusena == pradineBusena && taisykle.eSimbolis == juosta[pozicija]) {

					juosta[pozicija] = taisykle.nSimbolis; //simbolio keitimas

					//pajudejimas
					if (taisykle.kryptis == 'L') {
						if (pozicija == 0) {
							//cout << "Yeet"<<endl;
							//pridedamas '0' jog programa nepakibtu atsizvelgiant i 3.txt busenas
							if (count < 2) {
								juosta.insert(juosta.begin(), '0');
								count++;
							}
							else {
								juosta.insert(juosta.begin(), 'N');
							}
						}
						else {
							pozicija--;
						}
						//pozicija--;
					}
					else if (taisykle.kryptis == 'R') {
						pozicija++;
						if (pozicija >= juosta.size()) {
							juosta.push_back(' ');
						}
					}

					pradineBusena = taisykle.nBusena; //busenos pakeitimas

					//informacijos spausdinimas
					int galvosPozicija = max(0, pozicija);
					if (juosta != senaJuosta) {
						//this_thread::sleep_for(chrono::milliseconds(300));
						cout << juosta << " | " << "busena : " << taisykle.eBusena << " | " << "Simbolis : " << taisykle.eSimbolis << endl;
						cout << string(galvosPozicija, ' ') << "^" << endl;
					}
					taisykleRasta = true;
					break;
				}
			}
			if (!taisykleRasta) {
				cout << "Nera taisykles busenai: " << pradineBusena << " ir simboliui: " <<
					(pozicija >= 0 && pozicija < juosta.size() ? (juosta[pozicija] == ' ' ? '_' : juosta[pozicija]) : '?')
					<< endl;
			}
		}
		cout << "Galutine juosta: " << juosta << endl;
}
void isvedimas2(vector <taisykles2>& taisykles2Vek, string& juosta1, string& juosta2, int& juostuKiek, int& pozicija1, int& pozicija2, string& pradineBusena) {
	bool taisykleRasta = true;
	while (taisykleRasta && !stopRequested) {
		taisykleRasta = false;
		string senaJuosta1 = juosta1;
		string senaJuosta2 = juosta2;

		for (const auto& taisykle : taisykles2Vek) {
			//cout << taisykle.eBusena << " " << pradineBusena << " " << taisykle.eSimbolisT1 << " " << juosta1[pozicija1] << " " << taisykle.eSimbolisT2<< " " << juosta2[pozicija2]<<"poz1: "<<pozicija1<< "poz2: "<<pozicija2 <<endl;
			if (taisykle.eBusena == pradineBusena && taisykle.eSimbolisT1 == juosta1[pozicija1] && taisykle.eSimbolisT2 == juosta2[pozicija2]) {
				//cout << "yeet" << endl;
				juosta1[pozicija1] = taisykle.nSimbolisT1;
				juosta2[pozicija2] = taisykle.nSimbolisT2;

				if (taisykle.kryptisT1 == 'L') {
					if (pozicija1 == 0) {
						juosta1.insert(juosta1.begin(), ' ');
					}
					else {
						pozicija1--;
					}
				}
				else if (taisykle.kryptisT1 == 'R') {
					pozicija1++;
					if (pozicija1 >= juosta1.size()) {
						juosta1.push_back(' ');
					}
				}
				if (taisykle.kryptisT2 == 'L') {
					if (pozicija2 == 0) {
						juosta2.insert(juosta2.begin(), ' ');
					}
					else {
						pozicija2--;
					}
				}
				else if (taisykle.kryptisT2 == 'R') {
					pozicija2++;
					if (pozicija2 >= juosta2.size()) {
						juosta2.push_back(' ');
					}
				}
				//cout << pradineBusena<<"V" << endl;
				pradineBusena = taisykle.nBusena;
				//cout << pradineBusena << "A" << endl;

				if (juosta1 != senaJuosta1 || juosta2 != senaJuosta2) {
					this_thread::sleep_for(chrono::milliseconds(300));
					cout << juosta1 << " | " << "busena : " << taisykle.eBusena << " | " << "Simbolis 1 : " << taisykle.eSimbolisT1 << endl;
					cout << string(pozicija1, ' ') << "^" << endl;

					cout << juosta2 << " | " << "busena : " << taisykle.eBusena << " | " << "Simbolis 2 : " << taisykle.eSimbolisT2 << endl;
					cout << string(pozicija2, ' ') << "^" << endl;
				}

				taisykleRasta = true;
				break;
			}
		}
		if (!taisykleRasta) {
			cout << "Halted. Nera taisykles '" << pradineBusena << "' ir simboliui: '"
				<< (pozicija1 >= 0 && pozicija1 < juosta1.size() ? juosta1[pozicija1] : '?') << "', '"
				<< (pozicija2 >= 0 && pozicija2 < juosta2.size() ? juosta2[pozicija2] : '?') << "'." << endl;
		}
	}

}

