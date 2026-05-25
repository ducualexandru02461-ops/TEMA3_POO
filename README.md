În dezvoltarea sistemelor software moderne, gestionarea eficientă și scalabilă a datelor reprezintă o provocare fundamentală. Acest proiect abordează proiectarea și implementarea unui sistem de gestiune pentru o instituție de învățământ superior, utilizând limbajul C++. Scopul principal este modelarea fluxurilor de date specifice unei facultăți, unde interacționează diverse categorii de personal și beneficiari: studenți, profesori și secretari.

La nivel universitar, un astfel de sistem trebuie să fie extrem de flexibil. Schimbările legislative, modificările în structura planurilor de învățământ sau apariția unor noi roluri administrative cer ca arhitectura software să fie decuplată și ușor de extins. Pentru a răspunde acestor cerințe, proiectul evoluează de la o structură monolitică și rigidă către o arhitectură orientată pe obiecte complexă, care integrează polimorfismul dinamic, programarea generică (șabloane/templates), un sistem robust de tratare a excepțiilor și șabloane de proiectare (Design Patterns) recunoscute la nivel industrial, precum Factory Method.

Pilonul central al modelării orientate pe obiecte în acest proiect este ierarhia construită în jurul clasei abstracte Persoana. În lumea reală, un student, un profesor și un secretar sunt, înainte de toate, persoane. Acest raport de tip "is-a" (este o) este transpus în C++ prin intermediul moștenirii publice.

Clasa de bază Persoana definește interfața comună pentru toate entitățile din sistem. Ea conține un membru protejat nume (accesibil claselor derivate) și două metode virtuale pure fundamentale: citire() și afisare(). Prezența descriptorului = 0 transformă Persoana într-o clasă abstractă, ceea ce înseamnă că nu se pot instanția obiecte de acest tip în mod direct. Rolul ei este pur conceptual, obligând clasele derivate să ofere implementări concrete pentru aceste metode. Un aspect critic în această clasă este definirea unui destructor virtual (virtual ~Persoana() {}). În absența unui destructor virtual, eliberarea memoriei prin intermediul unui pointer de clasă de bază către un obiect derivat ar genera comportament nedefinit (undefined behavior), determinând scurgeri de memorie (memory leaks) deoarece destructorul clasei derivate nu ar fi apelat.

Clasa Student extinde Persoana prin adăugarea membrului specific domeniu (linia de studiu). Implementează metodele de citire și afișare adaptate fluxului său de date. Clasa Secretar introduce atributele departament și vechime. Aceasta personalizează comportamentul de afișare, adăugând formatarea specifică anilor de vechime în muncă. Clasa Profesor modelează personalul didactic și introduce o relație de compoziție ("has-a") cu un obiect de tip Curs. Profesorul nu doar că are un nume, dar gestionează în mod direct un curs universitar.

Unul dintre cele mai puternice mecanisme din C++ este programarea generică, implementată prin șabloane (templates). În versiunile inițiale ale proiectului, clasa Curs folosea un identificator rigid pentru credite și nume. Totuși, în sistemele academice globale, codurile cursurilor nu sunt uniforme: unele universități folosesc coduri pur numerice (int), în timp ce altele folosesc coduri alfanumerice (ex: "CS101", de tip std::string).

Pentru a flexibiliza această structură, clasa Curs a fost transformată într-o clasă șablon: template <typename T> class Curs. Atributul cod primește tipul generic T. Acest lucru permite reutilizarea aceleiași structuri logice de clasă, indiferent dacă stocăm codul sub formă de număr sau text.

În cadrul clasei Profesor, s-a optat pentru specializarea acestui șablon sub forma Curs<std::string>. Această decizie asigură compatibilitatea maximă cu inputul din fișiere text și permite stocarea unor coduri de curs complexe. Tot în clasa Curs s-a integrat și o funcție prietenă (friend), responsabilă cu supraîncărcarea operatorului de inserție în flux (operator<<). Deoarece clasa este de tip template, funcția friend a fost definită direct în interiorul clasei (inline). Această abordare permite scrierea unui cod extrem de curat în clasele client; de exemplu, în Profesor::afisare(), afișarea detaliilor cursului se rezumă la std::cout << curs;, lăsând operatorul prieten să acceseze direct membrii privați ai cursului fără a mai apela succesiv metode de tip getter.

Clasa Facultate acționează ca un container centralizator (bază de date în memorie). Aceasta utilizează containere din Biblioteca Standard (STL), precum std::vector, pentru a stoca separat colecții de studenți, profesori și secretari. Totuși, pentru înregistrările noi realizate la runtime, clasa expune un vector de pointeri la clasa de bază: std::vector<Persoana*> persoane.

Această structură permite utilizarea polimorfismului dinamic. La parcurgerea vectorului, apelul p->afisare() va rezolva la runtime (prin intermediul tabelei de funcții virtuale - vtable) tipul exact al obiectului reținut (fie el Student, Profesor sau Secretar), executând codul specific.

Gestionarea memoriei pe Heap implică responsabilități majore. Destructorul clasei Facultate parcurge în mod explicit acest vector de pointeri și aplică operatorul delete pentru fiecare element, prevenind acumularea de memorie orfană pe parcursul execuției programului.

Un alt mecanism avansat utilizat în clasa Facultate este dynamic_cast, regăsit în metoda nrStudentiAdaugati(). Deoarece vectorul stochează pointeri generici de tip Persoana*, programul nu știe direct ce tip de obiect se ascunde în spatele fiecărui pointer. dynamic_cast<Student*>(p) încearcă să convertească pointerul la un tip specific (Student*). Dacă obiectul indicat este într-adevăr un Student, conversia reușește și returnează un pointer valid; în caz contrar (dacă este Profesor sau Secretar), returnează nullptr. Acest mecanism de Run-Time Type Information (RTTI) permite filtrarea și numărarea sigură a entităților dintr-o colecție eterogenă.

Pentru a elimina cuplajul strâns (tight coupling) din funcția main și pentru a centraliza logica de instanțiere, proiectul integrează șablonul de proiectare Factory Method. Au fost dezvoltate două fabrici distincte, fiecare adresând o problemă specifică de design.

În faza inițială, funcția main conținea o structură decizională rigidă pentru crearea obiectelor pe baza opțiunii utilizatorului. Prin introducerea clasei PersoanaFactory, această responsabilitate a fost complet delegată. Fabrica expune o metodă statică creeazaPersoana(int tip) care încapsulează instrucțiunea switch.

Dacă pe viitor se dorește adăugarea unui nou tip de persoană (ex: Asistent), codul din main.cpp va rămâne complet intact. Singurul loc care va suferi modificări este această fabrică, respectându-se astfel principiul Open/Closed din SOLID (deschis pentru extensie, închis pentru modificare).

Cea de-a doua fabrică se ocupă de instanțierea obiectelor de tip Curs<std::string>. Spre deosebire de prima fabrică, aceasta nu se bazează pe o ierarhie polimorfică, ci are rolul de a încapsula logica de asamblare a unui obiect complex dinamic. Metoda statică creeazaCurs primește parametrii citiți din fișier și returnează un obiect gata configurat, izolând procesul de construcție de fluxul principal din bucla de citire a profesorilor.

Un sistem software academic nu poate fi considerat fiabil fără un mecanism riguros de toleranță la erori. În cadrul acestui proiect, siguranța execuției este garantată printr-o ierarhie proprie de excepții, ce moștenește clasa standard std::exception.

Clasa de bază pentru erori este EroareBD. Aceasta stochează un mesaj personalizat și rescrie metoda what() const noexcept pentru a returna un pointer de caractere compatibil cu standardul C. Din EroareBD sunt derivate excepții specializate:

EroareFisier: Aruncată atunci când fișierul date.txt nu poate fi deschis sau are un format corupt.

EroareCitire: Declanșată în metodele citire() dacă utilizatorul introduce câmpuri vide.

EroareDuplicat: Aruncată de clasa Facultate în momentul în care se încearcă adăugarea unei persoane cu un nume care există deja în sistem.

În main.cpp, toate operațiile critice (citirea din fișier, interacțiunea cu utilizatorul) sunt izolate în blocuri try-catch. Acest mod de abordare garantează că, indiferent de natura erorii (un fișier lipsă sau o tastare greșită), programul nu se va prăbuși catastrofal, ci va afișa un mesaj elegant de avertizare și va continua execuția sau se va închide în siguranță, eliberând resursele.

Proiectul de față reprezintă o evoluție evidentă de la programarea procedurală elementară către o arhitectură orientată pe obiecte matură și curată. Prin combinarea polimorfismului dinamic cu flexibilitatea oferită de șabloanele de programare, s-a obținut un sistem capabil să gestioneze date eterogene într-o manieră unificată.

Implementarea șabloanelor de proiectare de tip Factory Method demonstrează o preocupare directă pentru decuplarea codului și scalabilitate, transformând aplicația dintr-un script secvențial într-un model arhitectural extensibil, aliniat cu bunele practici din industria dezvoltării software. Gestiunea atentă a memoriei și sistemul ierarhic de excepții completează profilul unui proiect robust, gata să fie extins pentru cerințe academice mult mai complexe.
