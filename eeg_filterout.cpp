// Define constants
#define SAMPLING_RATE 256 
#define BAUD_RATE 115200
#define INPUT_PIN 35

// Function prototypes
void setupEEG();
float applyEEGFilter(float input);

void setup() {
  // Begin serial communication
  setupEEG();
}

void loop() { 
  // Calculate time intervals
  static unsigned long prevTime = 0;
  unsigned long currTime = micros();
  unsigned long elapsedTime = currTime - prevTime;
  prevTime = currTime;

  // Run timer
  static long timerCounter = 0;
  timerCounter -= elapsedTime;

  // Sample and filter
  if (timerCounter < 0) {
    timerCounter += 1000000 / SAMPLING_RATE;
    float sensorReading = analogRead(INPUT_PIN);
    float filteredSignal = applyEEGFilter(sensorReading);
    Serial.println(filteredSignal);
  }
}

// Function to setup EEG
void setupEEG() {
  Serial.begin(BAUD_RATE); // Initialize serial communication
}

// Function to apply EEG filter
float applyEEGFilter(float input) {
  static float prevOutput = 0;
  static float filterState1 = 0, filterState2 = 0;

  // First section
  float x = input - -0.95391350 * filterState1 - 0.25311356 * filterState2;
  float output = 0.00735282 * x + 0.01470564 * filterState1 + 0.00735282 * filterState2;
  filterState2 = filterState1;
  filterState1 = x;

  // Second section
  x = output - -1.20596630 * filterState1 - 0.60558332 * filterState2;
  output = 1.00000000 * x + 2.00000000 * filterState1 + 1.00000000 * filterState2;
  filterState2 = filterState1;
  filterState1 = x;

  // Third section
  x = output - -1.97690645 * filterState1 - 0.97706395 * filterState2;
  output = 1.00000000 * x + -2.00000000 * filterState1 + 1.00000000 * filterState2;
  filterState2 = filterState1;
  filterState1 = x;

  // Fourth section
  x = output - -1.99071687 * filterState1 - 0.99086813 * filterState2;
  output = 1.00000000 * x + -2.00000000 * filterState1 + 1.00000000 * filterState2;
  filterState2 = filterState1;
  filterState1 = x;

  // Return filtered output
  return output;
}
