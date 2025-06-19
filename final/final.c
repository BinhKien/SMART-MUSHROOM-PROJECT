#include <16F877A.h>

#fuses HS, NOWDT, NOLVP, NOPUT
#use delay(clock=20000000)             
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, bits=8)

#define IN1 PIN_B0
#define IN2 PIN_B1
#define DHT11_PIN PIN_D0  // Chon ket noi DHT11
#define LIGHT_SENSOR PIN_A2  // Ch√¢n k?t n?i c?m bi?n
#define SERVO_PIN  PIN_D1   // Servo trÍn RD1
#define LED_PIN    PIN_C0   // LED trÍn RC0

unsigned int i;

// Quay servo v? 0 d?
void servoRotate0()
{
   for(i = 0; i < 50; i++)
   {
      output_high(SERVO_PIN);
      delay_us(1000);           // 1ms ? 0 d?
      output_low(SERVO_PIN);
      delay_us(19000);          // cÚn l?i d? d? 20ms
   }
}

// Quay servo d?n 180 d?
void servoRotate180()
{
   for(i = 0; i < 50; i++)
   {
      output_high(SERVO_PIN);
      delay_us(2000);           // 2ms ? 180 d?
      output_low(SERVO_PIN);
      delay_us(18000);
   }
}
// Ham doc du lieu tu DHT11
int8 Read_DHT11(int8 *humidity, int8 *temperature) {
   int8 buffer[5] = {0, 0, 0, 0, 0};
   int8 i, j;
   
   // Gui tin hieu bat dau
   output_low(DHT11_PIN);
   delay_ms(18);
   output_high(DHT11_PIN);
   delay_us(30);
   input(DHT11_PIN);

   // Cho phan hoi DHT11
   if (input(DHT11_PIN)) return 0;
   while (!input(DHT11_PIN));
   while (input(DHT11_PIN));

   // Doc 5 byte du lieu
   for (i = 0; i < 5; i++) {
      for (j = 0; j < 8; j++) {
         while (!input(DHT11_PIN));  // Cho bit bat dau
         delay_us(30);
         if (input(DHT11_PIN))
            buffer[i] |= (1 << (7 - j));
         while (input(DHT11_PIN));  // Cho bit ket thuc
      }
   }

   // Kiem tra tong kiem tra (checksum)
   if ((buffer[0] + buffer[1] + buffer[2] + buffer[3]) == buffer[4]) {
      *humidity = buffer[0];       // Do am
      *temperature = buffer[2];    // Nhiet do
      return 1;  // Thanh cong
   } else {
      return 0;  // Loi checksum
   }
}

void main() {
   set_tris_b(0xFF);   // PORTB l‡ input
   set_tris_c(0x00);   // PORTC l‡ output
   set_tris_d(0x00);   // PORTD l‡ output
   int8 temperature, humidity;
   int1 light_value;
   printf("Khoi dong cam bien anh sang...\r\n");
   while(TRUE) {
      printf("Hello ESP32 FORM PIC\r\n");
      delay_ms(500);
     
      // Doc cam bien DHT11
      if (Read_DHT11(&humidity, &temperature)) {
         printf("Nhiet do: %u?C - Do am: %u%% \r\n", temperature, humidity);
      } else {
         printf("Loi doc DHT11!\r\n");
      }
      // Dieu khien dua vao nhiet do va do am
      if(temperature > 30 && humidity > 80) {
         output_high(IN1);
         output_low(IN2);
      } else {
         output_low(IN1);
         output_low(IN2);
      }
      //doc cam bien anh sang
      light_value = input(LIGHT_SENSOR);

      if(light_value) {
         servoRotate0();        // N?u tÌn hi?u l‡ 0
         output_high(LED_PIN);  // B?t LED
      } else {
         printf("Sang: 0\r\n");
         servoRotate180();      // N?u tÌn hi?u l‡ 1
         output_low(LED_PIN);   // T?t LED
      }

       delay_ms(1000); // ƒê?c m?i gi√¢y    
   }
}
