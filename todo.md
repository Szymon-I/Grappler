- rozbic klasy napisane przez nas na pliki hpp/cpp dla ewentualnego zwiekszenia czytelnosci
- zrobic obiekty w blenderze do koncowego projektu + tekstury (projekty blendera przechowywac w
katalogu blender)
- poprawic przerwanie w firmwarze, bo aktualnie skacze nawet na ustawieniu FALLING i badaniu czasu
od poprzedniego przerwania
- ustalic koncowa scene - warehouse z obiektami dekoracyjnymi i boxami do podnoszenia + swiatla i cienie
- dodac podnoszenie obiektow, bedzie to lista przechowywujaca programy (boxy), podnoszony obiekt bedzie
powiazany z grapplerem, a jego tekstura bedzie zmieniana w momencie podnoszenia
- dodac spadanie obiektow (boxow), bedzie to funkcja ktora zmienia polozenie w osi y (boxow),
funkcja ta bedzie wywolywana w timerze freeglut i bedzie iterowala wszystkie boxy, mozna przeliczac nastepne
polozenie w osi y zgodnie z przyspieszeniem grawitacyjnym, lub w uproszczonej wersji wykonywac skok o ustalona wartosc,
gdzie wartosc ostatniego skoku bedzie mniejsza lub rowna ustalonej wartosci, tak aby obiekt zatrzymal sie rowno na ziemi,
uproszeczeniem bedzie tutaj wyrenderowanie boxow w taki sposob, aby ich punkt 0,0 byl na srodku na dole
- dodac fixa, aby zmiana na kamere 3-os nie powodowala zmiany aktualnej pozycji grapplera, a tym samym kamery
- dodac fixa aby kamera nie skakala podczas uzywania wasd przy kamerze 3 os (raczej wynika to z freegluta, bo na kontrolerze dziala dobrze)
- dodac sterowanie grapplerem za pomoca klawiatury (dla kammery free), oraz dodac sterowanie w pionie dla kamery 3 os + akcja podnoszenia
- zmienic punkt 0.0 dla boxow (render w blenderze), bo teraz nie dotykaja ziemi
- zrefaktorowac caly kod :)