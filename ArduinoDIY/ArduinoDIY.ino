const int LAUTSPRECHER_PIN = 10;
const int TASTEN_PINS[] = {6, 7, 8, 9};
const int ANZAHL_TASTEN = sizeof(TASTEN_PINS) / sizeof(TASTEN_PINS[0]);

int LENGTH = 400;
int noten[4] = {100, 350, 600, 850};
int spielpattern[200];
int rundenanzahl = 1;

void setup() {
  for (int i = 2; i < 10; i++) {
    pinMode(i, OUTPUT);
  }

  for (int i = 6; i < 10; i++) {
    pinMode(i, INPUT);
  }

  Serial.begin(9600);
}

void loop() {
  hauptmenue();
}

void hauptmenue() {
  while (true) {
    for (int tastenIndex = 0; tastenIndex < ANZAHL_TASTEN; tastenIndex++) {
      if (digitalRead(TASTEN_PINS[tastenIndex]) == HIGH) {
        rundenanzahl = 20;
        spiel_generieren();
        spiel_starten();
      }
    }
  }
}

void spiel_starten() {
  int rundenZaehler = 0;
  int spielerZug = 1;
  bool tasteGedrueckt = false;
  int aktuelleNote;
  int benutzerEingabe = 0;
  bool verloren = false;
  for (int i = 1; i <= 4; i++) {
    note_abspielen(i, 100);
  }
  delay(1000);
  for (int aktuelleRunde = 1; (aktuelleRunde - 1) <= rundenanzahl; aktuelleRunde++) {
    rundenZaehler++;
    spielerZug = 1;
    tasteGedrueckt = false;
    benutzerEingabe = 0;
    for (int j = 1; j < aktuelleRunde; j++) {
      note_abspielen(spielpattern[j - 1], LENGTH);
    }
    while (spielerZug < aktuelleRunde) {
      aktuelleNote = spielpattern[spielerZug - 1];
      Serial.println(aktuelleNote);
      while (!tasteGedrueckt) {
        for (int tastenIndex = 0; tastenIndex < ANZAHL_TASTEN; tastenIndex++) {
          if (digitalRead(TASTEN_PINS[tastenIndex]) == HIGH) {
            tasteGedrueckt = true;
            benutzerEingabe = tastenIndex + 1;
            break;
          }
        }
        if (tasteGedrueckt) {
          note_abspielen(benutzerEingabe, LENGTH);
          if (aktuelleNote == benutzerEingabe) {
            spielerZug++;
          } else {
            spiel_ende(false);
          }
        }
      }
      tasteGedrueckt = false;
    }
  }
  if (!verloren) {
    Serial.println("Du hast gewonnen!");
    spiel_ende(true);
  }
}

void spiel_generieren() {
  randomSeed(analogRead(1));
  for (int i = 0; i < rundenanzahl; i++) {
    spielpattern[i] = random(1, 5);
  }
}

void note_abspielen(int index, int notenGeschwindigkeit) {
  digitalWrite(index + 1, HIGH);
  tone(LAUTSPRECHER_PIN, noten[index - 1], notenGeschwindigkeit);
  delay(notenGeschwindigkeit * 2);
  digitalWrite(index + 1, LOW);
}

void spiel_ende(bool gewonnen) {
  if (gewonnen) {
    Serial.println("Du hast gewonnen!");
    for (int i = 0; i < 10; i++) {
      for (int j = 1; j <= 4; j++) {
        note_abspielen(j, 50);
      }
    }
  } else {
    Serial.println("Du hast verloren!");
    for (int i = 0; i < 6; i++) {
      for (int j = 4; j >= 1; j--) {
        note_abspielen(j, 100);
      }
    }
  }
  Serial.println("Spiel vorbei");
  hauptmenue();
}
