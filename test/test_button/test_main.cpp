#include <Arduino.h>
#include <unity.h>

const int BUTTON_PIN = 0; // GPIO0

// Funktion, die blockierend auf den Zustand des Buttons wartet
void wait_for_button_state(int target_state, const char* prompt) {
    Serial.println(prompt);
    
    // WARTE SCHLEIFE: Blockiert hier, bis der Pin den Zielzustand erreicht
    while (digitalRead(BUTTON_PIN) != target_state) {
        // Hier wird einfach nur gewartet.
        delay(10); 
    }
}


// ** Der Haupt-Button-Test-Fall **
void test_io0_full_cycle(void) {
    // 1. Initialzustand pruefen: MUSS HIGH sein (Ruhezustand)
    // Wenn der Pin beim Start LOW ist, ist der Button beim Teststart bereits gedrueckt.
    TEST_ASSERT_EQUAL_MESSAGE(HIGH, digitalRead(BUTTON_PIN), 
                              "FEHLER: Startzustand ist nicht HIGH (Taster ist bereits gedrueckt).");

    // --- BLOCKIERENDE WARTE-LOGIK FUER DRUECKEN ---
    wait_for_button_state(LOW, "\n>>> TEST: BITTE DRUECKE JETZT den Button (IO0) und halte ihn! <<<");
    
    Serial.println(">>> Button-DRUCK erkannt!");

    // --- BLOCKIERENDE WARTE-LOGIK FUER LOSLASSEN ---
    wait_for_button_state(HIGH, ">>> Button erkannt! BITTE LASSE DEN BUTTON JETZT LOS. <<<");
        
    Serial.println(">>> Button-LOSLASSEN erkannt!");
    
    Serial.println(">>> Button-Test erfolgreich abgeschlossen. <<<");
}


void setup() {
    // 1. Längere Wartezeit nach dem Reset, um den Pin-Zustand zu stabilisieren.
    delay(4000); 
    
    // 2. PIN-SETUP: MUSS VOR UNITY_BEGIN() ODER RUN_TEST() ERFOLGEN
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    UNITY_BEGIN(); // WICHTIG: Startet den Unity Test-Runner
    
    // Fuehrt den gesamten blockierenden Test aus
    RUN_TEST(test_io0_full_cycle);
    
    UNITY_END(); // Stoppt den Unit Test
}

void loop() {
    // Bleibt leer
}
