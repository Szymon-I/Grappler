# Useful links

- https://www.cyberciti.biz/hardware/5-linux-unix-commands-for-connecting-to-the-serial-console/
- https://github.com/imabot2/serialib
- https://itsfoss.com/putty-linux/
- https://linux.die.net/man/1/minicom
- https://linux.die.net/man/1/socat

# Dodatkowe instrukcje

Do obsługi uarta mogą posłużyc programy putty/minicom, 
prosta konfiguracje minicoma mozna wykonac przy pomocy sudo minicom -s i edycji ustawien.


Do testowania komunikacji uart mozna uzyc virtual COM utworzonych przy pomocy socat,
przykladowe użycie: 'socat -d -d pty,raw,echo=0 pty,raw,echo=0,b9600' tworzy 2 virtual COMY,
ktore sa ze soba sprzezone i ustawione na predkosc 9600 baud.

# pyser
odpalasz jako python pyser.py, potem w nieskonczonej petli przyjmuje inputa
i wysyla do programu opengl, wyslanie wiadomosci jest potwierdzone przez znak '\n'
odpalenie wymaga biblioteki pyserial, mozna ja zainstalowac przez "pip install pyserial", na razie device name/speed jest zhardcodowane, ale to tylko do testow
