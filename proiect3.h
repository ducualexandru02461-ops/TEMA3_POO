#ifndef PROIECT3_H_INCLUDED
#define PROIECT3_H_INCLUDED
#include <iostream>
#include <vector>
#include <string>
#include <exception>
///EXCEPTII///

class EroareBD : public std::exception{
protected:
    std::string mesaj;

public:
    EroareBD(const std::string& msg) : mesaj(msg) {}

    const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class EroareFisier : public EroareBD{
public:
    EroareFisier() : EroareBD("Eroare la fisier\n") {}
};

class EroareCitire : public EroareBD{
public:
    EroareCitire() : EroareBD("Eroare la citire\n") {}
};

class EroareDuplicat : public EroareBD {
public:
    EroareDuplicat(const std::string& nume)
        : EroareBD("\n    ATENTIE! ELEMENT DUPLICAT: " + nume) {}
};

///PERSOANA///

class Persoana {
protected:
    std::string nume;
public:
    Persoana() {}
    Persoana(const std::string& n) : nume(n) {}
    std::string getNume() const { return nume; }
    virtual void citire (std::istream& in) = 0;
    void afisareNV() const {
        afisare();
    }
    virtual void afisare() const = 0;
    virtual ~Persoana() {}
};

///STUDENT///

class Student : public Persoana {
    std::string domeniu;
public:
    Student() {}
    Student(const std::string& n, const std::string& domeniu_) : Persoana(n), domeniu(domeniu_) {}

    void citire(std::istream& in) override {
        std::cout << "-NUME STUDENT: ";
        std::getline(in, nume);
        if (nume == ""){
            throw EroareCitire();
        }

        std::cout << "-DOMENIUL: ";
        std::getline(in, domeniu);
        if (domeniu == ""){
            throw EroareCitire();
        }
    }

    void afisare() const override {
        std::cout << "\nSTUDENT: " << nume << "\nDOMENIUL: " << domeniu << "\n";
    }

    std::string getDomeniu() const { return domeniu; }
};

///CURS///

template <typename T>

class Curs {
    std::string nume;
    int credite;
    T cod;
public:
    Curs() {}
    Curs(const std::string& n, const int c_, const T cod_) : nume(n), credite(c_), cod(cod_) {}

    std::string getNume() const { return nume; }
    int getCredite() const { return credite; }
    T getCod() const { return cod; }

    void citire(std::istream& in){
        std::cout << "-NUME CURS: ";
        std::getline(in, nume);
        if (nume == ""){
            throw EroareCitire();
        }

        std::cout << "-CREDITE: ";
        in >> credite;
        in.ignore();

        std::cout << "-COD CURS: ";
        in >> cod;
        in.ignore();
    }
};

///PROFESOR///

class Profesor : public Persoana {
    Curs<int> curs;

public:
    Profesor() {}
    Profesor(const std::string& n, const Curs<int> c)
        : Persoana(n), curs(c) {}

    std::string getNumeCurs() const { return curs.getNume(); }

    int getCrediteCurs() const { return curs.getCredite(); }

    template <typename T>
    T getCodCurs() const { return curs.getCod(); }

    void citire(std::istream& in) override {
        std::cout << "NUME PROFESOR: ";
        std::getline(in, nume);
        if (nume == ""){
            throw EroareCitire();
        }

        curs.citire(in);
    }

    void afisare () const override {
        std::cout << "\n-PROFESOR: " << nume
                  << "\n-CURS: " << curs.getNume()
                  << "\n-CREDITE: " << curs.getCredite()
                  << "\n-COD CURS: " << curs.getCod() << "\n";

    }
};

///SECRETAR///

class Secretar : public Persoana {
    std::string departament;
    int vechime;

public:
    Secretar() : Persoana(""), departament(""), vechime(0) {}

    Secretar(const std::string& nume_, const std::string& dep, int v)
        : Persoana(nume_), departament(dep), vechime(v){}

    std::string getDepartament() const{
        return departament;
    }

    int getVechime() const{
        return vechime;
    }

    void afisare() const {
        std::cout << "\n-SECRETAR: " << nume
                  << "\n-DEPARTAMENT: " << departament
                  << "\n-VECHIME: " << vechime << " ANI\n";
    }

    void citire(std::istream& in) override{
        std::cout << "-NUME SECRETAR: ";
        std::getline(in, nume);
        if (nume == ""){
            throw EroareCitire();
        }

        std::cout << "-DEPARTAMENT: ";
        std::getline(in, departament);
        if (departament == ""){
            throw EroareCitire();
        }

        std::cout << "-VECHIME(ANI): ";
        in >> vechime;
        in.ignore();
    }
};

///FACULTATE///

class Facultate {
    std::string nume;
    std::string oras;

public:
    std::vector<Student> studenti;
    std::vector<Profesor> profesori;
    std::vector<Secretar> secretari;
    std::vector<Persoana*> persoane;

    void setDate(const std::string& n, const std::string& o) {
        nume = n;
        oras = o;
    }

    void afisare() {
        std::cout << "  FACULTATE: " << nume << "\n";
        std::cout << "  ORAS: " << oras << "\n\n";

        std::cout << "    STUDENTI:\n";
        for (auto& s : studenti)
            std::cout << "- NUME: " << s.getNume() << " | DOMENIUL: " << s.getDomeniu() <<"\n";

        std::cout << "\n    PROFESORI:\n";
        for (auto& p : profesori) {
            std::cout << "- NUME: Profesor " << p.getNume()
                      << " | CURS: " << p.getNumeCurs()
                      << " | CREDITE: " << p.getCrediteCurs()
                      << " | COD CURS: " << p.getCodCurs<int>() << "\n";
        }

        std::cout << "\n    SECRETARI:\n";
        for (auto& s : secretari) {
            std::cout << "- NUME: " << s.getNume()
                      << " | DEPARTAMENT: " << s.getDepartament()
                      << " | ANI VECHIME: " << s.getVechime() << "\n";
        }

    }

    void afiseazaDate() {
        for (auto p : persoane)
            p->afisareNV();
    }


    int nrStudentiAdaugati() {
        int nr = 0;
        Persoana* p = new Student();
        for (auto p : persoane){
            if(Student* s = dynamic_cast<Student*>(p))
                nr += 1;
        }
        return nr;
    }

    void adaugaPersoana(Persoana *p) {
        for (auto pers : persoane){
            if (pers->getNume() == p->getNume())
                throw EroareDuplicat(p->getNume());
        }
    }

    ~Facultate() {
        for (auto p : persoane)
            delete p;
    }

};



#endif // PROIECT2_H_INCLUDED
