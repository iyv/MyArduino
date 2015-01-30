//S65-Shield: Demo 1 (Display + Rotary Encoder)

#include <S65Display.h>

S65Display lcd;

int8_t move, press;

void drawText(void)
{
  lcd.drawTextPGM(10, 50, PSTR("S65-Shield")          , 2, RGB(  0,255,  0), RGB(255,  0,  0));
  lcd.drawTextPGM(10, 80, PSTR("         by         "), 1, RGB(255,255,  0), RGB(255,  0,  0));
  lcd.drawTextPGM(10, 95, PSTR("Watterott electronic"), 1, RGB(255,255,255), RGB(255,  0,  0));
  lcd.drawTextPGM(10,110, PSTR(" www.watterott.com  "), 1, RGB(  0,  0,255), RGB(255,  0,  0));

  //Umlaut test text (remove FONT_END7F define)
  lcd.drawTextPGM(10, 10, PSTR("Test \x84 \x94 \x81 \x8E \x99 \x9A"), 1, RGB(  0,255,  0), RGB(255,  0,  0));
  lcd.drawText   (10, 25,      "Test \x84 \x94 \x81 \x8E \x99 \x9A",  1, RGB(  0,255,  0), RGB(255,  0,  0));
}

void drawBox(int x, int y, int col1=0,int col2=0,int col3=0)
{
  lcd.fillRect(x,y,S65_WIDTH-x,S65_HEIGHT-y,RGB(col1,col2,col3));
  lcd.drawRect(x,y,S65_WIDTH-x,S65_HEIGHT-y,RGB(0,0,0));
}

void drawBoxX(int x, int y, int col1=0,int col2=0,int col3=0)
{
  lcd.fillRect(x,y,52+x,30+y,RGB(col1,col2,col3));
  lcd.drawRect(x,y,52+x,30+y,RGB(0,0,0));
}

void setup()
{
  //init LCD
  lcd.init(4); //spi-clk = Fcpu/4

  //clear screen
  lcd.clear(RGB(125,125,255));
  
  drawText();
  delay(2000);
}


#define RECTSIZE 30
void loop()
{
  //const char &txt='Hello WORLD';
  lcd.clear(RGB(125,125,255));
  delay(1000);
  drawBox(10,10,0,200,0);
  delay(500);
  drawBox(25,25,0,100,0);
  delay(500);
  drawBox(40,40,200,0,0);
  delay(500);
  drawBox(55,55,0,0,200);
  delay(500);
  lcd.drawChar(80,55,'+',2,RGB(125,125,125),RGB(0,0,0));
  delay(2000);
  //lcd.drawText(40,20,txt,RGB(20,50,0),RGB(50,20,0));
  //delay(500);
  lcd.drawMLTextPGM(50,10, PSTR(" testing...  "), 1, RGB(  0,  0,255), RGB(255,  0,  0));
  delay(500);
  lcd.drawMLTextPGM(50,10, PSTR(" testing...  "), 1, RGB(  255,  0,0), RGB(0,  0,  255));
  delay(500);
  lcd.drawMLTextPGM(50,10, PSTR(" testing...  "), 1, RGB(  0,  0,255), RGB(255,  0,  0));
  delay(500);
  lcd.drawMLTextPGM(50,10, PSTR(" testing...  "), 1, RGB(  255,  0,0), RGB(0,  0,  255));
  delay(500);
  lcd.drawMLTextPGM(50,10, PSTR(" testing...  "), 1, RGB(  0,  0,255), RGB(255,  0,  0));
  delay(500);  
  //delay(500);
  lcd.drawTextPGM(77,120, PSTR("TEST"), 1, RGB(  255,  0,255), RGB(255,  0,  0));
  delay(2000);
  lcd.clear(RGB(125,125,255));
  delay(1000);
  drawBoxX(5, 10,0,200,0);
  lcd.drawTextPGM(15,20, PSTR("150"), 1, RGB(255,255,255), RGB(0,200,0));
  drawBoxX(62, 10,0,200,0);
  lcd.drawTextPGM(77,20, PSTR("xxx"), 1, RGB(255,255,255), RGB(0,200,0));
  drawBoxX(119,10,0,200,0);
  lcd.drawTextPGM(134,20, PSTR("84"), 1, RGB(255,255,255), RGB(0,200,0));
  drawBoxX(5, 70,0,200,0);
  drawBoxX(62, 70,0,200,0);
  drawBoxX(119,70,0,200,0);
  delay(2000);
}
