#include "hwlib.hpp"
#include <array>

constexpr void fill_array(std::array< int, 16 > & array, const uint8_t player, const uint8_t data){
   array[0] = 1;
   for(unsigned int i = 0; i < (array.size() - 11); i++){ 
      array[5 - i] = (player >> i) & 1;
   }
   for(unsigned int i = 0; i < (array.size() - 11); i++){ 
      array[10 - i] = (data >> i) & 1;
   }
   for(unsigned int i = 0; i < (array.size() - 11); i++){ 
      array[15 - i] = array[5 - i] ^ array[10 - i];
   }
}

void print_array(std::array< int, 16 > & array){   // test
   for(unsigned int i = 0; i < array.size(); i++){
      hwlib::cout << array[i];
	}
   hwlib::cout << '\n';
}

void send(std::array< int, 16 > & array, hwlib::target::d2_36kHz & ir, hwlib::target::pin_out & red){   // done   //
   for(unsigned int i = 0; i < 16; i++){ 
      if(array[i] == 0){
         ir.write(1);
         red.write(1);  // test
         hwlib::wait_us(8000);
         ir.write(0);
         red.write(0);  // test
         hwlib::wait_us(16000);
         hwlib::cout << 0; // test
      }
      else{
         ir.write(1);
         red.write(1);  // test
         hwlib::wait_us(16000);
         ir.write(0);
         red.write(0);  // test
         hwlib::wait_us(8000);
         hwlib::cout << 1; // test
      }
   }
   hwlib::cout << '\n'; // test
}

int main( void ){	
   
   // IR output LED
   auto ir = hwlib::target::d2_36kHz();
   
   // red output LED
   auto red = hwlib::target::pin_out( hwlib::target::pins::d42 );
   
   // switch which enables the 36 kHz output
   auto sw = hwlib::target::pin_in( hwlib::target::pins::d43 );
   
   uint8_t player = 25;
   uint8_t data = 16;

   std::array<int, 16> signal_array;

   fill_array(signal_array, player, data);
   hwlib::cout << "array: ";  //test
   print_array(signal_array); // test

   // when the switch is pressed, 
   // sends signal twice with a 3ms delay between each other
   for(;;){
      sw.refresh();
      if(!sw.read()){
         hwlib::cout << "signal: "; //test
         send(signal_array, ir, red); //
         hwlib::wait_ms(3);
         send(signal_array, ir, red); //
         hwlib::wait_ms(5);
      }
   }
}
