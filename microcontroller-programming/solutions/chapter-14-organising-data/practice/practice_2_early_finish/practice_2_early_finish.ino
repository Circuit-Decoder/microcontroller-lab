// Chapter 14, Practice 2 - Early Finish
// Bubble sort that stops as soon as a pass makes no swaps.

const int COUNT = 6;
int values[COUNT] = {12, 25, 45, 64, 90, 78};   // nearly sorted

void setup() {
  Serial.begin(115200);
  delay(1000);

  for (int pass = 0; pass < COUNT - 1; pass++) {
    bool swapped = false;
    for (int i = 0; i < COUNT - 1 - pass; i++) {
      if (values[i] > values[i + 1]) {
        int temp = values[i];
        values[i] = values[i + 1];
        values[i + 1] = temp;
        swapped = true;
      }
    }
    Serial.print("Pass ");
    Serial.print(pass + 1);
    Serial.print(": ");
    printList();
    if (!swapped) {
      Serial.println("No swaps: already sorted, stopping.");
      break;
    }
  }
}

void loop() {
}

void printList() {
  for (int i = 0; i < COUNT; i++) {
    Serial.print(values[i]);
    Serial.print(" ");
  }
  Serial.println();
}
