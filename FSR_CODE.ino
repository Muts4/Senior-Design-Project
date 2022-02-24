float cf = 9.75;  // calibration factor
 
int ffs1 = 25;  // FlexiForce sensor connected to A0

int ffsdata = 0;
float vout;

void setup() // put your setup code here, to run once:
{
  Serial.begin(115200);
  pinMode(ffs1, INPUT);
  

}

void loop() // put your main code here, to run repeatedly:
{

  ffsdata = analogRead(ffs1);
  vout = (ffsdata * 5.0)/ 1023.0;
  vout = (vout*cf)/4.448; 
  Serial.print("Flexiforce sensor: ");
  Serial.print(vout,3);
  Serial.print("");
  delay(100);
  

}
