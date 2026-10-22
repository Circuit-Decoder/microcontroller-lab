// ===================================================================
//  MultiTap.h  —  an old-phone keyboard you can reuse in any project
//  Keys 0-9 type letters (press quickly to cycle), * deletes.
//  In loop(), call:  if (mtUpdate(key)) { ...redraw the screen... }
// ===================================================================

// The letters on each key, in tap order. The digit itself comes last.
String mtLetters[10] = {
  " 0",       // key 0: space, 0
  ".,?!1",    // key 1: punctuation, 1
  "ABC2",     // key 2
  "DEF3",     // key 3
  "GHI4",     // key 4
  "JKL5",     // key 5
  "MNO6",     // key 6
  "PQRS7",    // key 7
  "TUV8",     // key 8
  "WXYZ9"     // key 9
};

String mtText = "";                    // the text typed so far
char mtActiveKey = 0;                  // key being tapped now (0 = none)
int mtTapCount = 0;                    // how many taps on that key
unsigned long mtLastKeyTime = 0;       // when it was last tapped
const unsigned long MT_TIMEOUT = 800;  // pause (ms) that locks a letter
unsigned int mtMaxLength = 90;         // the longest text allowed
bool mtLowercase = false;              // CHALLENGE 1: small letters?

// The letter shown right now for the active key
char mtCurrentLetter() {
  if (mtActiveKey == 0) return 0;
  char c = mtLetters[mtActiveKey - '0'][mtTapCount];
  if (mtLowercase && c >= 'A' && c <= 'Z') {
    c = c + ('a' - 'A');               // CHALLENGE 1: 'A' (65) + 32 = 'a' (97)
  }
  return c;
}

// Add the current letter to the text and get ready for the next one
void mtLockIn() {
  if (mtActiveKey == 0) return;
  if (mtText.length() < mtMaxLength) {
    mtText += mtCurrentLetter();
  }
  mtActiveKey = 0;
  mtTapCount = 0;
}

// Start again with empty text
void mtClear() {
  mtText = "";
  mtActiveKey = 0;
  mtTapCount = 0;
}

// The text plus the letter that is still being chosen
String mtPreview() {
  String s = mtText;
  if (mtActiveKey != 0) s += mtCurrentLetter();
  return s;
}

// Call this in EVERY loop, even when no key is pressed (key = 0).
// Returns true when something changed and the screen should be redrawn.
bool mtUpdate(char key) {
  bool changed = false;

  // 1. A pause long enough? Lock in the letter.
  if (mtActiveKey != 0 && millis() - mtLastKeyTime > MT_TIMEOUT) {
    mtLockIn();
    changed = true;
  }

  // 2. A digit key: cycle (same key) or start a new letter (new key)
  if (key >= '0' && key <= '9') {
    if (key == mtActiveKey) {
      mtTapCount++;
      if (mtTapCount >= mtLetters[key - '0'].length()) {
        mtTapCount = 0;                // after the last letter, start again
      }
    } else {
      mtLockIn();                      // a new key locks the old letter
      mtActiveKey = key;
      mtTapCount = 0;
    }
    mtLastKeyTime = millis();
    changed = true;
  }

  // 3. * = delete: cancel the letter being chosen, or the last letter
  else if (key == '*') {
    if (mtActiveKey != 0) {
      mtActiveKey = 0;
      mtTapCount = 0;
    } else if (mtText.length() > 0) {
      mtText.remove(mtText.length() - 1);
    }
    changed = true;
  }

  return changed;
}
