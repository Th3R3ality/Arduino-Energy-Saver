#define PIN 13

char input_buffer[64];
unsigned long offset_milliseconds = 0;

void parse_timestamp();
unsigned long milliseconds(int clock_time);

void setup()
{
  Serial.begin(9600);
  pinMode(PIN, OUTPUT);
}

void loop()
{

   	for (int i = 0; i < strlen(input_buffer); i++){
   		input_buffer[i] = ' ';
 	 }
  
  	while (Serial.available() > 0){
      Serial.readString().toCharArray(input_buffer, 64);
      parse_timestamp();
      Serial.println("New time offset set.");
  	}
  
  
}

void parse_timestamp(){
  	int clock_time = 0;
    for (int idx = 0; idx < strlen(input_buffer); idx++){
        if (input_buffer[idx] >= '0' && input_buffer[idx] <= '9'){
          	clock_time *= 10;
            clock_time += input_buffer[idx] - '0';
        }
    }
    offset_milliseconds = milliseconds(clock_time);

}

unsigned long milliseconds(int clock_time) {
	unsigned long hours = (clock_time - (clock_time - static_cast<int>(clock_time / 100) * 100)) / 100;
	unsigned long minutes = clock_time - hours * 100;
  	return hours * 60 * 60 * 1000 + minutes * 60 * 1000;
}
