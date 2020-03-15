namespace user
{

void nec(char a, char c);
void nec_sc(short a, char c);
void samsung(char a, char c);
void sharp(char a, char c);
void sony12(char a, char c);
void sony15(char a, char c);
void sony20(char a, char c, char e);
void rc6(char a, char c, char m);


void power() {
  sharp(1,22);
  nec(4,8);	// LG
  sony12(1,21);
  nec(64,18);	// TOSHIBA
  samsung(7,2);
}

void ch_up() {
  sharp(1,17);
  nec(4,0);	// LG
  sony12(1,16);
  nec(64,27);	// TOSHIBA
  samsung(7,18);
}

void ch_down() {
  sharp(1,18);
  nec(4,1);	// LG
  sony12(1,17);
  nec(64,31);	// TOSHIBA
  samsung(7,16);
}

void mute() {
  sharp(1,23);
  nec(4,9);	// LG
  sony12(1,20);
  nec(64,16);	// TOSHIBA
  samsung(7,15);
}

void vol_down() {
  sharp(1,21);
  nec(4,3);	// LG
  sony12(1,19);
  nec(64,30);	// TOSHIBA
  samsung(7,11);
}

void vol_up() {
  sharp(1,20);
  nec(4,3);	// LG
  sony12(1,18);
  nec(64,26);	// TOSHIBA
  samsung(7,7);
}

} // user
