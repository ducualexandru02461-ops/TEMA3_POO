#include <fstream>
#include <iostream>
#include "proiect3.h"

int main() {


    try{
    std::ifstream fin("date.txt");

    if(!fin){
        throw EroareFisier();
    }

    Facultate f;

    std::string linie;

    std::getline(fin, linie);
    std::string numeFac = linie;

    if(numeFac == ""){
        throw EroareFisier();
    }

    std::getline(fin, linie);
    std::string oras = linie;

    if(oras == ""){
        throw EroareFisier();
    }


    f.setDate(numeFac, oras);

    std::getline(fin, linie);
    std::getline(fin, linie);

    std::string domeniu;

    while (std::getline(fin, linie) && linie != "") {
        std::getline(fin, domeniu);
        if (domeniu == ""){
            throw EroareFisier();
        }
        f.studenti.push_back(Student(linie, domeniu));
    }

    std::getline(fin, linie);

    std::string numeProf, numeCurs;
    int credite;
    int cod;

    while (std::getline(fin, numeProf)) {
        if (numeProf == "")break;

        std::getline(fin, numeCurs);
        fin >> credite >> cod;
        fin.ignore();

        Curs<int> c(numeCurs, credite, cod);
        f.profesori.push_back(Profesor(numeProf, c));
    }


    std::string nume, prenume, departament;
    int vechime;

    std::getline(fin, linie);

    while (fin >> nume >> prenume >> departament >> vechime) {
        if (nume == "" || prenume == "" || departament == ""){
            throw EroareFisier();
        }
        std::string numeComplet = nume + " " + prenume;
        f.secretari.push_back(Secretar(numeComplet, departament, vechime));
    }

    f.afisare();


    while(1){
        std::cout << "\n   INSERATI CIFRA:\n"
                  << "0 -> EXIT\n"
                  << "1 -> INREGISTRARE PERSOANE NOI\n"
                  << "2 -> NUMARUL STUDENTILOR NOI INREGISTRATI\n";
        int btn;
        std::cout << "\n-> ";
        std::cin >> btn;
        if (btn == 0){
            std::cout << "\n   CLOSED";
            break;
        }
        switch(btn){
            case 1:
                {
                //std::vector<Persoana*> persoane;
                while(1){

                    int tip, ok =1;
                    std::cout << "\n0 ->EXIT \n1 -> INREGISTRARE STUDENT\n2 -> INREGISTRARE PROFESOR\n3 -> INREGISTRARE SECRETAR\n ";
                    std::cin >> tip;
                    std::cin.ignore();

                    Persoana* p = nullptr;

                    if (tip == 0)
                        break;
                    else if (tip == 1)
                        p = new Student();
                        else if (tip == 2)
                            p = new Profesor();
                            else if (tip == 3)
                                p = new Secretar();

                    if (p) {
                        try {
                            p->citire(std::cin);
                        }
                        catch (const std::exception& e) {
                            ok = 0;
                            std::cout << e.what();
                        }
                        try {
                            f.adaugaPersoana(p);
                        }
                        catch (const std::exception& e) {
                            ok = 0;
                            std::cout << e.what();
                        }
                        if (ok == 1)
                            f.persoane.push_back(p);
                    }
                }

                if (f.persoane.size() == 0)std::cout << "\n   NU AU FOST INTRODUSE PERSOANE NOI.";
                else {
                    std::cout << "\n   PERSOANE NOI:";
                    f.afiseazaDate();
                }
                break;
                }
            case 2:
                {
                    if (f.nrStudentiAdaugati() != 1){
                        std::cout << "\n   ->AU FOST INREGISTRATI " << f.nrStudentiAdaugati() << " STUDENTI NOI.\n";
                    }
                    else std::cout << "\n   ->A FOST INREGISTRAT UN SINGUR STUDENT NOU.\n";
                    break;
                }
            default:
                std::cout << "\n   OPTIUNE INVALIDA\n";
                break;
        }
    }
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    return 0;
}
