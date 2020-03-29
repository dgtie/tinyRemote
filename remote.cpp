namespace user
{

void wait(int);
void nec(char a, char c);
void nec_sc(short a, char c);
void samsung(char a1, char a2, char c);
void sharp(char a, char c);
void sony12(char c, char a);
void sony15(char c, char a);
void sony20(char c, char a, char e);
void rc6(char a, char c, char m);

}

namespace
{

using namespace user;

void k1() {
  sharp(1,1);
  nec(4,17);		// LG
  nec(64,1);		// TOSHIBA
  samsung(7,7,4);
  sony12(0,1);
}

void k2() {
  sharp(1,2);
  nec(4,18);		// LG
  nec(64,2);		// TOSHIBA
  samsung(7,7,5);
  sony12(1,1);
}

void k3() {
  sharp(1,3);
  nec(4,19);		// LG
  nec(64,3);		// TOSHIBA
  samsung(7,7,6);
  sony12(2,1);
}

void k7() {
  sharp(1,7);
  nec(4,23);		// LG
  nec(64,7);		// TOSHIBA
  samsung(7,7,12);
  sony12(6,1);
}

void k9() {
  sharp(1,9);
  nec(4,25);		// LG
  nec(64,9);		// TOSHIBA
  samsung(7,7,14);
  sony12(8,1);
}


} // anonymous

namespace user
{

void power() {
  sharp(1,22);
  nec(4,8);	// LG
  nec(64,18);	// TOSHIBA
  samsung(7,7,2);
  sony12(21,1);
}

void ch_up() {
  sharp(1,17);
  nec(4,0);	// LG
  nec(64,27);	// TOSHIBA
  samsung(7,7,18);
  sony12(16,1);
}

void ch_down() {
  sharp(1,18);
  nec(4,1);	// LG
  nec(64,31);	// TOSHIBA
  samsung(7,7,16);
  sony12(17,1);
}

void mute() {
  sharp(1,23);
  nec(4,9);	// LG
  nec(64,16);	// TOSHIBA
  samsung(7,7,15);
  sony12(20,1);
}

void vol_up() {
  sharp(1,20);
  nec(4,2);	// LG
  nec(64,26);	// TOSHIBA
  samsung(7,7,7);
  sony12(18,1);
}

void vol_down() {
  sharp(1,21);
  nec(4,3);	// LG
  nec(64,30);	// TOSHIBA
  samsung(7,7,11);
  sony12(19,1);
}

void input() {
  sharp(1,19);
  nec(4,11);	// LG
  nec(64,15);	// TOSHIBA
  samsung(7,7,1);
  sony12(37,1);
}

void k31(){
  k3(); k1();
}

void k32(){
  k3(); k2();
}

void k77(){
  k7(); wait(20); k7();
}

void k99(){
  k9(); wait(20); k9();
}

} // user
