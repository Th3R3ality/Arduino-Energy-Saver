//////////////////////////
//Arduino implementation//
//////////////////////////
#define PIN 13
#define INPUT_BUFFER_SIZE 64

//forward declarations
void set_time();
bool check_time();
unsigned long parse_timestamp(char* input, int index_offset = 0, int index_end = 0);
unsigned long milliseconds(int clock_time);
bool is_within(unsigned long a, unsigned long b, unsigned long t);

//globals
char input_buffer[INPUT_BUFFER_SIZE];
unsigned long offset_milliseconds = 0;
bool override_power = false;
unsigned long power_off_from = 0, power_off_to = 0;


void setup()
{
  Serial.begin(9600);
  pinMode(PIN, OUTPUT);
}

void loop()
{
	memset(input_buffer, 0x0, INPUT_BUFFER_SIZE);
  	while (Serial.available() > 0){
      Serial.readString().toCharArray(input_buffer, INPUT_BUFFER_SIZE);
      if (input_buffer[0] == '!')
        override_power = !override_power;
      else if (input_buffer[0] == '/')
        set_time();
      else
        offset_milliseconds = parse_timestamp(input_buffer);
    }
  
  if (override_power || check_time())
    digitalWrite(PIN, LOW);
  else
    digitalWrite(PIN, HIGH);
}

unsigned long parse_timestamp(char* input, int index_offset, int index_end){
  	int clock_time = 0;
  	if (index_end == 0)
      index_end = strlen(input);
    for (int idx = index_offset; idx < index_end; idx++){
        if (input[idx] >= '0' && input[idx] <= '9'){
          	clock_time *= 10;
            clock_time += input[idx] - '0';
        }
    }
    return milliseconds(clock_time);
}

void set_time(){
  int offset = 0;
  if (input_buffer[3] > '9' || input_buffer[3] < '0'){
    offset = 1;
  }
  power_off_from = parse_timestamp(input_buffer, 0, 5 + offset);
  power_off_to = parse_timestamp(input_buffer, 5 + offset);
}

bool check_time(){
  unsigned long current_time = (millis() + offset_milliseconds) % parse_timestamp("24:00");
  if (power_off_from < power_off_to)
    return is_within(power_off_from, power_off_to, current_time);
  else
    return is_within(power_off_to, power_off_from, current_time);
}

bool is_within(unsigned long a, unsigned long b, unsigned long t){
  return (a < t && t < b);
}

unsigned long milliseconds(int clock_time) {
	unsigned long hours = (clock_time - (clock_time - static_cast<int>(clock_time / 100) * 100)) / 100;
	unsigned long minutes = clock_time - hours * 100;
  	return hours * 60 * 60 * 1000 + minutes * 60 * 1000;
}
