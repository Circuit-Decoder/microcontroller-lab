// Chapter 3, Practice 2 - Pocket Money
// How many snacks can you buy, and how much money is left over?

int pocketMoney = 100;     // taka per week (an int: whole taka)
float snackPrice = 15.5;   // taka per snack (a float: it has decimals)

void setup() {
  Serial.begin(115200);
  delay(1000);

  int snacks = pocketMoney / snackPrice;          // decimals are cut off
  float left = pocketMoney - snacks * snackPrice;

  Serial.print("Pocket money: ");
  Serial.println(pocketMoney);
  Serial.print("Snack price:  ");
  Serial.println(snackPrice);
  Serial.print("Snacks you can buy: ");
  Serial.println(snacks);
  Serial.print("Money left over:    ");
  Serial.println(left);
}

void loop() {
}
