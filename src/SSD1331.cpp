#include "SSD1331.h"

#define RED(x) ((byte)(x.R * 0x1F) << 1 & 0x3E)
#define GRN(x) ((byte)(x.G * 0x3F) & 0x3F)
#define BLU(x) ((byte)(x.B * 0x1F) << 1 & 0x3E)

SSD1331::Color const SSD1331::Color::Black     = { 0.0, 0.0, 0.0 };
SSD1331::Color const SSD1331::Color::DarkGrey  = { 0.3, 0.3, 0.3 };
SSD1331::Color const SSD1331::Color::LightGrey = { 0.6, 0.6, 0.6 };
SSD1331::Color const SSD1331::Color::White     = { 1.0, 1.0, 1.0 };
SSD1331::Color const SSD1331::Color::Red       = { 1.0, 0.0, 0.0 };
SSD1331::Color const SSD1331::Color::Orange    = { 1.0, 0.5, 0.0 };
SSD1331::Color const SSD1331::Color::Yellow    = { 1.0, 1.0, 0.0 };
SSD1331::Color const SSD1331::Color::Green     = { 0.0, 1.0, 0.0 };
SSD1331::Color const SSD1331::Color::Blue      = { 0.0, 0.0, 1.0 };
SSD1331::Color const SSD1331::Color::Violett   = { 1.0, 0.0, 1.0 };

SSD1331::SSD1331()
  : IsFillActive(false)
{

}
void SSD1331::Begin(DigitalOut* pin_RST, DigitalOut* pin_CS, DigitalOut* pin_DC, bool flipHorizontal)
{
  Pin_RST = pin_RST;
  Pin_CS = pin_CS;
  Pin_DC = pin_DC;
  SPI.begin();

  // ChipSelect aktivieren
  (*Pin_CS)(false);

  // Chip resetten
  (*Pin_RST)(true);
  delay(10);
  (*Pin_RST)(false);
  delay(10);
  (*Pin_RST)(true);
  delay(10);

  // Chip initialisieren
  byte CmdInit[] =
  {
    Cmd_DisplayOff,                 // Display ausschalten
    Cmd_Remap, flipHorizontal ? 0x62 : 0x72,        // Nötig für das verwendete Display
    Cmd_MasterConf, 0x8E,           // Nötig für das verwendete Display
    Cmd_Precharge, 0xFF,            // Nötig für das verwendete Display (auf 0xFF geändert)
    Cmd_ClockDiv, 0xF0,             // größte Bildwiederholrate
    Cmd_SecPrechargeA, 0x64,        // Nötig für das verwendete Display
    Cmd_SecPrechargeB, 0x78,        // Nötig für das verwendete Display
    Cmd_SecPrechargeC, 0x64,        // Nötig für das verwendete Display
    Cmd_PrechargeLevel, 0x3E,       // Nötig für das verwendete Display
    Cmd_MasterCurrent, 0x06,        // Nötig für das verwendete Display
    Cmd_ContrastA, 0x91,            // Nötig für das verwendete Display
    Cmd_ContrastB, 0x50,            // Nötig für das verwendete Display
    Cmd_ContrastC, 0x7D,            // Nötig für das verwendete Display
    Cmd_DisplayOn                   // Display einschalten
  };
  WriteSPI(CmdInit, sizeof(CmdInit));
  Clear(0, 0, 95, 63);
}
void SSD1331::DrawTestPreset()
{
  Clear(0, 0, 95, 63);
  DrawRect(0,  0,  15, 31, Color::Black, true, Color::Black);
  DrawRect(16, 0,  31, 31, Color::Black, true, Color::DarkGrey);
  DrawRect(32, 0,  47, 31, Color::Black, true, Color::LightGrey);
  DrawRect(48, 0,  63, 31, Color::Black, true, Color::White);

  DrawRect(0,  32, 15, 63, Color::Black, true, Color::Red);
  DrawRect(16, 32, 31, 63, Color::Black, true, Color::Orange);
  DrawRect(32, 32, 47, 63, Color::Black, true, Color::Yellow);
  DrawRect(48, 32, 63, 63, Color::Black, true, Color::Green);
  DrawRect(64, 32, 79, 63, Color::Black, true, Color::Blue);
  DrawRect(80, 32, 95, 63, Color::Black, true, Color::Violett);
}
void SSD1331::DrawTestGradient(float phaseColor, float phaseBlack)
{
  for (int i = 0; i < 96; i++)
  {
    DrawLine(i, 0, i, 31, HSVToRGB(Mod(i / 95.0 + phaseColor, 1) * 360, 1, 1));
    DrawLine(i, 32, i, 63, HSVToRGB(0, 0, Mod(i + phaseBlack, 1) / 95.0));
  }
}
void SSD1331::WriteRawData(byte x1, byte y1, byte x2, byte y2, short* data, int count)
{
  byte CmdAddr[] =
  {
    Cmd_SetColumnAddr,
    x1 & 0x7F,
    x2 & 0x7F,
    Cmd_SetRowAddr,
    y1 & 0x3F,
    y2 & 0x3F
  };

  WriteSPI(CmdAddr, sizeof(CmdAddr));
  WriteSPI(reinterpret_cast<byte*>(data), count * 2, true);
}
void SSD1331::DrawLine(byte x1, byte y1, byte x2, byte y2, Color c)
{
  byte CmdLine[] =
  {
    Cmd_DrawLine,
    x1 & 0x7F,
    y1 & 0x3F,
    x2 & 0x7F,
    y2 & 0x3F,
    RED(c),
    GRN(c),
    BLU(c)
  };

  WriteSPI(CmdLine, sizeof(CmdLine));
}
void SSD1331::DrawRect(byte x1, byte y1, byte x2, byte y2, Color line, bool isFilled, Color fill)
{
  // Bei Bedarf Füllung aktivieren
  if(isFilled != IsFillActive)
  {
    byte CmdFill[] =
    {
      Cmd_Fill,
      isFilled ? 0x01 : 0x00
    };
    WriteSPI(CmdFill, sizeof(CmdFill));
    IsFillActive = isFilled;
  }

  byte CmdRect[] =
  {
    Cmd_DrawRect,
    x1 & 0x7F,
    y1 & 0x3F,
    x2 & 0x7F,
    y2 & 0x3F,
    RED(line),
    GRN(line),
    BLU(line),
    RED(fill),
    GRN(fill),
    BLU(fill)
  };
  WriteSPI(CmdRect, sizeof(CmdRect));
}
void SSD1331::DrawString(byte x, byte y, const char* text, FontSize size, Color foreground, bool isBold, FontFamily family, Color background)
{
  char filename[14];
  sprintf(filename, "%i/%s%s",
    size == FontSize::pt9 ? 9 :
    size == FontSize::pt12 ? 12 :
    size == FontSize::pt18 ? 18 :
    size == FontSize::pt24 ? 24 :
    0,
    family == FontFamily::Mono ? "Mono" :
    family == FontFamily::Sans ? "Sans" :
    family == FontFamily::Serif ? "Serif" :
    "",
    isBold ? "B.hex" : ".hex");
  pinMode(4, OUTPUT);
  SD.begin(4);
  File f = SD.open(filename, FILE_READ);
  byte newlineX = x;

  // Text zeichenweise zeichnen
  for (int i = 0; text[i] != '\0'; i++)
  {
    switch (size)
    {
      case FontSize::pt9:
      case FontSize::pt12:
        DrawChar<64>(x, y, newlineX, text[i], &f, foreground, background);
        break;
      case FontSize::pt18:
        DrawChar<128>(x, y, newlineX, text[i], &f, foreground, background);
        break;
      case FontSize::pt24:
        DrawChar<256>(x, y, newlineX, text[i], &f, foreground, background);
        break;
    }
  }
  f.close();
}
template<int buffersize>
  void SSD1331::DrawChar(byte& x, byte& y, byte newlineX, const char c, File* f, Color foreground, Color background)
{
  // Schriftinformationen auslesen
  f->seek(0);
  byte asciiStart = f->read();
  byte asciiEnd = f->read();
  byte newlineHeight = f->read();

  // Buchtstaben prüfen
  if (c == '\n')
  {
    y += newlineHeight;
    x = newlineX;
    return;
  }
  if (c < asciiStart || c > asciiEnd)
    return;

  // Buchstabeninformationen auslesen
  f->seek(3 + 7 * (c - asciiStart));
  short AddrLo = f->read();
  short AddrHi = f->read();
  short bitmapAddress = 3
    + 7 * (asciiEnd - asciiStart + 1)
    + AddrLo + (AddrHi << 8);
  byte bitmapWidth = f->read();
  byte bitmapHeight = f->read();
  byte stepX = f->read();
  byte OffsetX = f->read();
  byte OffsetY = f->read();

  // Wenn der Buchstabe nach rechts hinausragt neue Zeile beginnen
  if (x + bitmapWidth + OffsetX > 96)
  {
    y += newlineHeight;
    x = newlineX;
  }

  // Bitmap laden
  byte bitmap[buffersize];
  float bitmapBits = bitmapWidth * bitmapHeight;
  short bitmapBytes = (short)(bitmapBits / 8 + 0.999);
  f->seek(bitmapAddress);
  f->read(bitmap, bitmapBytes);

  // Schreibbereich im GDDRAM setzen
  byte CmdGDDRAMArea[] =
  {
    Cmd_SetColumnAddr,
    (x + OffsetX) & 0x7F,
    (x + OffsetX + bitmapWidth - 1) & 0x7F,
    Cmd_SetRowAddr,
    (y + OffsetY) & 0x7F,
    (y + OffsetY + bitmapHeight) & 0x7F
  };
  WriteSPI(CmdGDDRAMArea, sizeof(CmdGDDRAMArea));

  byte foreHi = (int)(foreground.R * 0x1F) << 3 & 0xF8;
  foreHi += (int)(foreground.G * 0x3F) >> 3 & 0x07;
  byte foreLo = (int)(foreground.G * 0x3F) << 5 & 0xE0;
  foreLo += (int)(foreground.B * 0x1F) & 0x1F;
  byte backHi = (int)(background.R * 0x1F) << 3 & 0xF8;
  backHi += (int)(background.G * 0x3F) >> 3 & 0x07;
  byte backLo = (int)(background.G * 0x3F) << 5 & 0xE0;
  backLo += (int)(background.B * 0x1F) & 0x1F;

  // Byteweise in Farben umwandeln und zeichnen
  for (int i = 0; i < bitmapBytes; i++)
  {
    byte data = bitmap[i];
    byte colormap[16] =
    {
      (data & B10000000) == B10000000 ? foreHi : backHi,
      (data & B10000000) == B10000000 ? foreLo : backLo,
      (data & B01000000) == B01000000 ? foreHi : backHi,
      (data & B01000000) == B01000000 ? foreLo : backLo,
      (data & B00100000) == B00100000 ? foreHi : backHi,
      (data & B00100000) == B00100000 ? foreLo : backLo,
      (data & B00010000) == B00010000 ? foreHi : backHi,
      (data & B00010000) == B00010000 ? foreLo : backLo,
      (data & B00001000) == B00001000 ? foreHi : backHi,
      (data & B00001000) == B00001000 ? foreLo : backLo,
      (data & B00000100) == B00000100 ? foreHi : backHi,
      (data & B00000100) == B00000100 ? foreLo : backLo,
      (data & B00000010) == B00000010 ? foreHi : backHi,
      (data & B00000010) == B00000010 ? foreLo : backLo,
      (data & B00000001) == B00000001 ? foreHi : backHi,
      (data & B00000001) == B00000001 ? foreLo : backLo
    };
    WriteSPI(colormap, sizeof(colormap), true);
  }
  x += stepX;
}

void SSD1331::Copy(byte x1, byte y1, byte x2, byte y2, byte xNew, byte yNew)
{
  byte CmdCopy[] =
  {
    Cmd_Copy,
    x1 & 0x7F,
    y1 & 0x3F,
    x2 & 0x7F,
    y2 & 0x3F,
    xNew & 0x7F,
    yNew & 0x3F
  };

  WriteSPI(CmdCopy, sizeof(CmdCopy));
}
void SSD1331::Clear(byte x1, byte y1, byte x2, byte y2)
{
  byte CmdClear[] =
  {
    Cmd_Clear,
    x1 & 0x7F,
    y1 & 0x3F,
    x2 & 0x7F,
    y2 & 0x3F
  };

  WriteSPI(CmdClear, sizeof(CmdClear));
}
SSD1331::Color SSD1331::HSVToRGB(float h, float s, float v)
{
  float c = v * s;
  float x = c * (1 - abs(Mod(h / 60, 2) - 1));
  float m = v - c;
  if (0 <= h && h < 60)
    return { c + m, x + m, 0 + m };
  if (60 <= h && h < 120)
    return { x + m, c + m, 0 + m };
  if (120 <= h && h < 180)
    return { 0 + m, c + m, x + m };
  if (180 <= h && h < 240)
    return { 0 + m, x + m, c + m };
  if (240 <= h && h < 300)
    return { x + m, 0 + m, c + m };
  if (300 <= h && h < 360)
    return { c + m, 0 + m, x + m };

  return { 0, 0, 0 };
}
void SSD1331::WriteSPI(byte* data, int count, bool isData)
{
  SPI.beginTransaction(SPISettings(6000000, MSBFIRST, SPI_MODE0));
  (*Pin_CS)(false);

  (*Pin_DC)(isData ? HIGH : LOW); // Befehl oder Grafikdaten
  SPI.transfer(data, count);

  (*Pin_CS)(true);
  SPI.endTransaction();
}
