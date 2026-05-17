# TEMA3_POO

Se consideră o aplicație C++ care gestionează activitatea unei facultăți, permițând administrarea diferitelor tipuri de persoane implicate în cadrul acesteia. Aplicația are rolul de a organiza și evidenția studenții, profesorii și secretarii, precum și de a permite adăugarea dinamică de noi persoane și tratarea unor situații excepționale.

Fiecare persoană este identificată printr-un nume și este modelată printr-o clasă de bază abstractă Persoana, din care derivă următoarele tipuri:

Student, caracterizat prin domeniul de studiu; Profesor, asociat cu un curs (nume și număr de credite); Secretar, caracterizat prin departament și vechime

Datele inițiale ale facultății (nume, oraș, studenți, profesori, secretari) sunt citite dintr-un fișier text.

Aplicația trebuie să permită următoarele funcționalități:

citirea și stocarea datelor despre facultate din fișier; afișarea informațiilor despre facultate și toate categoriile de persoane; adăugarea de noi persoane (student, profesor sau secretar) prin intermediul unei interfețe bazate pe pointeri la clasa de bază; utilizarea funcțiilor virtuale pentru citire și afișare (polimorfism); tratarea erorilor printr-o ierarhie proprie de excepții (derivată din std::exception), pentru: erori de citire, erori de fișier, erori de duplicare; determinarea numărului de studenți adăugați ulterior rulării aplicației
