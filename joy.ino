int T_PIN = 0;
int ENA_PIN = 0;
int H1_PIN = 1;
int H2_PIN = 2;

int OVERSAMPLING = 6;
float R0 = 100000.0;
float T0 = 25.0 + 273.15;
float BETA = 4400.0;

long MAX_SAMPLE = ((long)1 << (10+6)) - 1;

long sample(int pin, int addBits)
{
  word numSamples = (word)1 << (2*addBits);
  long accum = 0;
  
  for (word i=0; i<numSamples; i++)
    accum += analogRead(pin);

  return accum / ((word)1 << addBits);
}

float readTemp(int pin)
{
  word S = sample(pin, OVERSAMPLING);
  float V = S * 5.0 / MAX_SAMPLE;
  float R = R0 / ((float)S / (float)(MAX_SAMPLE-S));
  float T = 1.0 / (1.0/T0 + (1.0/BETA)*log(R/R0));
  return T - 273.15;
}

void heat() {
  digitalWrite(H1_PIN, HIGH);
  digitalWrite(H2_PIN, LOW);  
}

void cool() {
  digitalWrite(H1_PIN, LOW);
  digitalWrite(H2_PIN, HIGH);  
}

void neutral() {
  digitalWrite(H1_PIN, LOW);
  digitalWrite(H2_PIN, LOW);  
}


void setup()
{
  pinMode(ENA_PIN, OUTPUT);
  pinMode(H1_PIN, OUTPUT);
  pinMode(H2_PIN, OUTPUT);

  for (int i=3; i<6; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  digitalWrite(ENA_PIN, HIGH);
  heat();

  Serial.begin(9600);
  Serial.println("Begin temperature readings");
}

void loop()
{
  static int mode = 1;
  float temp = readTemp(T_PIN);
  
  if (mode == 1) {
    if (temp >= 40.0) {
        cool();
        mode = 2;
    }
  }
  else if (mode == 2) {
    if (temp <= 30.0) {
      heat();
      mode = 1;
    }
  }
       
  Serial.println(temp);
}
