# Grappler
Jest to początkowa wersja naszego projektu na zaliczenie przedmiotu
"Programowanie grafiki 3D w OpenGL". 
Program ten pozwala na obsługę Grapplera - chwytaka, który bedzie mógł sie poruszać
po scenie i podnosić przdmioty. W aktualnej wersji została wykorzystana scena z 
poprzednich laboratoriów wraz z modyfikacjami. Docelowo scena będzie się prawdopodobnie składać z hali z przedmiotami.

# Features

## Context menu
Menu dostępne jest po naciśnieciu prawego przycisku myszki w programie.
- exit - wyjscie z programu (również jako ESC key)
- shaders - zmiana wykorzystywanych shaderow w programie
- serial - zmiana nasluchiwania portu szeregowego (w postaci printf)
- camera - zmiana trybu kamery

## Camera
Rozglądanie się kamerą jest możliwe poprzez użycie myszki.
Obsługa kamery odbywa się w 2 trybach:

### Free camera
Jest to tak zwana wolna kamera, można się nią poruszać niezależnie od sceny.
Poruszanie kamery jest możliwe poprzez użycie klawiszy:

- w/scroll up - poruszanie sie do przodu
- w/scroll down - poruszanie sie do tyłu
- a - poruszanie sie w lewo (strafing)
- d - poruszanie sie w prawo (strafing)

### Third person
Kamera trzecioosobowa jest bezpośrednio związana z obiektem Grappler (akutalnie wilk).
W tym przypadku możliwe jest użycie klawiszy 'w,a,s,d' do poruszania się kamery wraz
z grapplerem.

## Shaders
Aktualnie znajdują się 3 różne 'shadery fragmentu', których zmiana modyfikuje akutalne
światło na scenie, dostępne światła:
- fragment - domyślne światło
- fragment_directional - światło skierowanie
- fragment_spotlight - światło latarki

Do wszystkich shaderów dodane zostały materiały.

## Animacje
Dodane zostały animacje do obiektów, które umożliwiają poruszanie się/obracanie każdego z nich

## Tekstury
Wczytywanie tekstur odbywa się przy pomocy biblioteki SOIL, co pozwala na używanie tekstur w innym formacie niż bmp, co zaoszczędza miejsce na dysku.

## Odświeżanie obrazu
W głownej pętli programu znajduje sie timer, który powoduje odświeżanie sceny 60 
klatek na sekunde.

## Serial
Została napisana klasa która pozwala na szeregowa komunikację w pętli głównej programu
z urządzeniami podłączonymi poprzez usb lub wirtualnymi (socat)

# Kontroler
W ramach projektu został stworzony również dedykowany kontroler oparty o architekturę
AVR ATmega328 wraz z portem szeregowym CH340. Użyte elementy:
- mikrokontroler arduino nano
- wyświetlacz oled
- akcelerometr/żyroskop MPU6050
- potencjometr
- joystick

Kontroler działa w trybie joysticka lub akcelerometru.
Przesunięcie graplera w osi x/z jest możliwe za pomocą osi x/y joysticka lub akcelerometru. Oś y jest kontrolowana za pomocą potencjometru i jest ona bezwzględna, 
co oznacza, że ustawienie konkretnej wartości dla osi y powoduje zatrzymanie się tam obiektu, a nie ustala jego wektoru prędkości.

Kontroler komunikuje się z programem za pomocą portu szeregowego i wysyłanie kontrolnej
ramki komunikacyjnej w formie "float/float/float" (oś x/ oś z/ oś y) z ustalonym zakresem maksymalnej wartości (akutalna rozdzielność danych to od -1000 do 1000)

Uruchomienie programu z kontrolerem wymaga podania jego nazwy nadanej przez os oraz prędkości transmisji (baudrate).

Zlokalizowanie portu urządzenia w systemie linux jest możliwe przy pomocy komendy:

```bash
dmesg | grep 'tty'
```

przykładowa informacja zwrotna:

```bash
[    0.097615] printk: console [tty0] enabled
[    1.043506] 00:04: ttyS0 at I/O 0x3f8 (irq = 4, base_baud = 115200) is a 16550A
[    1.136305] tty ttyS13: hash matches
[ 8565.271845] usb 1-6: ch341-uart converter now attached to ttyUSB0
```

Zatem nasz kontroler posiada nazwę

```bash
/dev/ttyUSB0
```
Prędkość urządzenia jest wyświetlana na ekranie oled i musi być podana wraz z nazwą
urządzenia podczas staru programu.

Przykładowe uruchomienie programu:

```bash
./run.sh src /dev/ttyUSB0 115200
```

Skrypt `run.sh` jest odpowiedzialny za kompilację i uruchomienie programu. 
Pierwszy argument to ścieżka w której znajduje się program (src), drugi argument
to nazwa urządzenia, trzeci to prędkość transmiji.

Oczywiście program może być uruchomiony bez podłączonego kontrolera, a w tym przypadku
zostaną zachowane wszystkie funkcję z wyłaczeniem sterowania grapplerem za pomocą kontrolera.

# Symulacja kontrolera
Możlie jest też zasymulowanie pracy kontrolera za pomocą software.

Należy w tym przypadku utworzyć wirtualne porty szeregowe za pomocą programu socat.
Pogram ten tworzy 2 porty szeregowe, które są ze sobą sprzeżone i można użyc jednego z nich jako input do programu opengl, a 2 jako output ze skrytpu symulującego pracę kontrolera napisanego w pythonie `GrapplerMove.py`.

## Uruchomienie wirtualnych portów
Uruchomienie portów można osiągnąć poprzez użycie komendy:

```bash
socat -d -d pty,raw,echo=0 pty,raw,echo=0,b115200
```

gdzie argument b115200 to prędkość transmisji i musi być ona identyczna dla każdego
medium transmisyjnego.

Przykładowy output programu:

```bash
2020/05/02 15:59:32 socat[6773] N PTY is /dev/pts/2
2020/05/02 15:59:32 socat[6773] N PTY is /dev/pts/3
2020/05/02 15:59:32 socat[6773] N starting data transfer loop with FDs [5,5] and [7,7]
```
oznacza, że zostały uruchomione porty szeregowe o nazwach "/dev/pts/2", "/dev/pts/3"
i prędkości 115200 bodów (bitów na sekundę).

## Przykładowe uruchomienie
Odnosząć się do powyższych informacji, aby uruchomić program z symulacja kontrolera
należy uruchomić komendy:
```bash
./run.sh src /dev/pts/2 115200
python GrapplerMove.py /dev/pts/3 115200
```
Następnie poprzez input do skryptu pythona możemy wykonywać wcześniej zdefiniowane ruchy grapplera.

# Uruchomienie programu
Zakłada się, że program będzie uruchamiany na systemie linux, zatem należy najpierw
zainstalować wymagane biblioteki poprzez uruchomienie skryptu `install_libs.sh`, 
następnie można uruchomić program za pomocą komendy:
```bash
./run.sh src <port kontrolera> <prędkość transmisji>
```
jeżeli program ma być uruchomiony bez mikrokontrolera można w miejsce portu i prędkości
podać dowolne argumenty, w ten sposob program zostanie poinformowany, że dane urzadzenie nie znajduje się pod aktualnym ustawieniem, a transmisja szeregowa nie zostanie nawiązana, przykładowo:
```bash
./run.sh src arg1 ar2
```
Uruchomienie programu w trybie symulacji zostało opisane w rozdziale `Symulacja kontrolera`.

# Zawartość plików źródłowych
- folder blender -> przechowywanie projektów programu blender
- folder firmware -> firmware napisany na mikrokontroler
- img -> obrazy użyte do readme
- src -> głowny folder z kodem programu
    - libs -> folder z dodatkowymi bibliotekami
    - objects -> folder z plikami .obj obiektów
    - shaders -> folder z shaderami
    - textures -> folder z teksturami
    - Camera.hpp -> wyświetlanie/ruch kamery
    - Grappler.hpp -> wyświetlanie/ruch grapplera
    - IOHandler.hpp -> bind przycisków klawiatury/myszki do obsługi w pętli głownej
    - Light.hpp -> klasa zawierająca informacje o świetle
    - main.cpp -> głowny program
    - main.hpp -> nagłowek głownego programu ze zmiennymi globalnymi (extern)
    - Material.hpp -> klasa ze zmiennymi statycznymi używanych materiałow
    - Menu.hpp -> menu kontekstowe
    - ProgramHandler.hpp -> klasa zawierająca wszystkie informacje o obiekcie, takie jak:
        - tekstura
        - macierz obj (wierzchołki, wektory normalne, wektory tekstur)
        - akutalne przesunięcie/obrót/sklaowanie
        - używane shadery
        - światła
    - Serial.hpp -> obsługa portu szeregowego