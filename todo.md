- zaktualizowac zrodla swiatla dla klasy programHandler
prawdopodobnie trzeba zmienic przeliczanie odleglosci w shaderze,
aby swiatlo bylo w pozycji absolutnej, a nie wzglednej, bo wtedy tworzy sie 
osobno zrodlo swiatla dla kazdego obiektu
- napisac klase/wrappera dla programHandler, ktora bedzie odpowiedzialna za przemieszanie
obiektow (np. bedzie do niej przekazywany obiekt programHandler i dodatkowo bindy na przyciski
klawiatury, ktore beda odpowiedzialne za ruch), mozna tutaj napisac osobna klase, ktora bedzie
przechowywala obiekt programHandler i wykonywala na nim operacje, lub dziedziczyc te klase i
dodac w niej dodatkowe funkcjonalnosci
- rozbic klasy napisane przez nas na pliki hpp/cpp dla ewentualnego zwiekszenia czytelnosci
- zrobic obiekty w blenderze do koncowego projektu + tekstury (projekty blendera przechowywac w
katalogu blender)
- poprawic przerwanie w firmwarze, bo aktualnie skacze nawet na ustawieniu FALLING i badaniu czasu
od poprzedniego przerwania
- dodac kamere poruszajaca sie razem z grapplerem, spacja powoduje zmiane kamery miedzy:
free camera, camera 3-os grapplera, poruszanie sie kamery 3 os mozna wywolac w klasie Grappler, 
za kazdym obsluzeniem nowej pozycji
- dodac flage w klasie Grappler, ktora bedzie informowala o tym, ze obiekt sie przemiescil wzgledem
poprzedniej pozycji, bo w innym przypadku za kazdym razem, gdy odbierany jest pakiet (w firware
okreslony jest czas LOOP_INTERVAL) generowana jest nowa scena poprzez wywolanie glutPostRedisplay