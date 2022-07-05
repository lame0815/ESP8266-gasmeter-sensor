# ESP8266-gasmeter-sensor
Smarter Gaszähler-Sensor auf Basis des ESP8266

## Funktionsweise und Voraussetzungen
Der Sensor hat die Funktion, daß es den Erdgaszähler ausliest. Diese Funktion ist sehr nützlich, wenn man
einen Überblick über den Gasverbrauch erlangen möchte. Der Sensor funktioniert so, daß er die Umdrehungen des
Gaszählers misst und mit jeder Umdrehung einen Zählerwert hochzählt.
Der Sensor benötigt einige Voraussetzungen, damit man ihn auch nutzen kann:
- Einen kompatiblen Gaszähler
- Ein passendes ESP8266-Board
- Ein passenden Reed-Sensor
- Ein USB- Netzteil mit Mikro-USB Stecker
- Einen Server der die vom Sensor empfangenen Daten speichert und auswertet.
Im folgenden gehe ich in Kurzform auf die einzelnen Komponenten ein

## Benötige Komponenten (ca. 10 Euro)

### Kompatibler Gaszähler
Zuallererst muss man herausfinden, ob der heimische Gaszähler für die Auslesung durch einen Sensor geeignet ist.
Geeignete Zähler besitzen in einer der hinteren Zählscheiben einen Magneten, der sich mit der Zählung dreht 
und mit jeder Umdrehung einen Sensor auslösen kann.
Um dieses herauszufinden ist es hilfreich, beim Gaszählerhersteller das Datenblatt vom Zähler zu suchen. Ein Indiz ist auch,
dass viele Gaszähler unter den letzten Stellen eine Einbuchtung für genau einen solchen Sensor haben.

### ESP8266 Board
Benötigt wird: Ein ESP8266-Entwicklungsboard. Diese Boards werden auch als Development-Boards angeboten
und können direkt per USB programmiert und versorgt werden. Ich habe die Software mit einem "Wemos D1 Mini" kompatiblen
Board mit 4Mb Flash ausprobiert und es läuft. Wenn Ihr Erfahrungen mit anderen Boards macht, informiert mich gerne und
ich nehme diese mit auf.
![ESP8266 Development Board](https://github.com/lame0815/ESP8266-gasmeter-sensor/blob/main/readme-assets/esp8266.jpg "ESP8266 Development Board")


### Ein Reed-Sensor
Ein Reed-Sensor ist ein elektrischer Schalter, der bei einem Magnetfeld schaltet (dem Magneten im Zähler).
Hier ist die Schwierigkeit, einen passenden Sensor zu finden. Teilweise bieten die Zählerhersteller auf Anfrage
Sensoren zum Kauf an, man kann diese aber auch selber bauen. Das Finden des richtigen eigenen Sensors ist mitunter 
aber recht schwierig.


#### Schwierigkeiten bei der Sensor-Suche
Der Sensor muss 3,3v geeignet sein und möglichst empfindlich. Hier bestellt man am Besten mehrere Versionen und probiert den passenden
aus. Hier gilt es auszuprobieren. Hinweis: Die Empfindlichkeit von Reed-Sensoren ist durch AT oder AW angegeben. Je niedriger die Zahl ist,
desto empfindlicher ist der Sensor. 
Ich hatte zuerst einen Sensor-Board mit dem Namen KY-021 bestellt. Leider war der daraufgelöstete Reed-Sensor nicht empfindlich genug für meinen
Zähler. Im zweiten Schritt habe ich daher den original Reed-Sensor ausgelötet und gegen den KSK1A66-1020 Reed-Kontakt 1xEIN 200V= 0,5A 10W AW10-20 von Meder
ausgetauscht.
![KY021 Sensor Modul](https://github.com/lame0815/ESP8266-gasmeter-sensor/blob/main/readme-assets/ky-021.jpg "KY021 Sensor Modul")


#### Sensor-Positionierung
Der Sensor muss wirklich ganz exakt unter der richtigen Drehscheibe des Zählers positioniert werden. Hier gilt es:
Bei laufender Heizung ausprobieren.
![Sensor am Gaszähler montiert](https://github.com/lame0815/ESP8266-gasmeter-sensor/blob/main/readme-assets/sensor_montiert.jpg "Sensor am Gaszähler montiert")

### Ein USB-Netzteil 
Die ESP8266-Entwicklungsboards werden per USB programmiert und können auch darüber betrieben werden. Hierzu genügt ein
USB-Netzteil mit passendem Kabel

### Ein Server
Hier gibt es viel auf dem Markt, nutzt Euer Smart Home System. Ich setze einen IOBroker ein mit einer angebundenen InfluxDB. Zur Auswertung
habe ich ein Grafana aufgesetzt.

## Anschluss
Der Anschluss des Sensors erfolgt über den Port D2. Hierbei ist unbedingt darauf zu achten, daß der Strom vom 3,3v-Pin zu holen ist. Die Eingänge es ESP8266 vertragen nur 3,3v, ein Anschluss von 5v kann das Modul zerstören.
Im folgenden zwei Möglichkeiten des Anschlusses, einmal ein direkter Reed-sensor-Anschluss und einmal über das KY021
![Anschluss des REED-Sensors](https://github.com/lame0815/ESP8266-gasmeter-sensor/blob/main/readme-assets/anschluss.jpg "Anschluss des REED-Sensors")

## Flashen
Das Flashen kann z.B. über das Windows-Tool FlashESP8266.exe geschehen. Dazu muss man
- Den ESP8266 per USB an den PC anschließen
- Das Tool starten
- den richtigen COM-Port auswählen
- Die Firmware-Datei auswählen
- Auf "Flash" klicken
![Flashen in ESPEasy](https://github.com/lame0815/ESP8266-gasmeter-sensor/blob/main/readme-assets/espeasy.jpg "Flashen in ESPEasy")

War der Flashvorgang erfolgreich, kann man auf den ESP zugreifen.

