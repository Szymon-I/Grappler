- Napisac sterownik do obslugi seriala, pomocna biblioteka: https://github.com/imabot2/serialib
- zaktualizowac zrodla swiatla dla klasy programHandler
prawdopodobnie trzeba zmienic przeliczanie odleglosci w shaderze,
aby swiatlo bylo w pozycji absolutnej, a nie wzglednej, bo wtedy tworzy sie 
osobno zrodlo swiatla dla kazdego obiektu
- napisac klase do obslugi seriala, tak aby nie importowac serial dependencies do programu
oraz obsluzyc zdarzenia z seriala w petli glownej programu (mozna uzyc GLFW), trzeba bedzie
ja dodatkowo zainstalowac, link do przykladowej petli GLFW: https://inf.ug.edu.pl/~mdziemia/opengl/02/glfw-template.cpp
- napisac klase/wrappera dla programHandler, ktora bedzie odpowiedzialna za przemieszanie
obiektow (np. bedzie do niej przekazywany obiekt programHandler i dodatkowo bindy na przyciski
klawiatury, ktore beda odpowiedzialne za ruch), mozna tutaj napisac osobna klase, ktora bedzie
przechowywala obiekt programHandler i wykonywala na nim operacje, lub dziedziczyc te klase i
dodac w niej dodatkowe funkcjonalnosci
- rozbic klasy napisane przez nas na pliki hpp/cpp dla ewentualnego zwiekszenia czytelnosci
- zrobic obiekty w blenderze do koncowego projektu + tekstury (projekty blendera przechowywac w
katalogu blender)
- nasluchiwanie dodatkowych zdarzen moza przekazac jako funkcje do glutIdleFunc(),
jest to funkcja wywolywana wraz z innymi funkcjami interrupt handler w glownej petli
freeglut (glutMainLoop()), akutalnie petla glowna nie ma ograniczenia fps, jest wywolywana
na zasadzie poolingu