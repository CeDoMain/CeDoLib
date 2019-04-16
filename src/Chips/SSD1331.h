#ifndef SSD1331_H
#define SSD1331_H

#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include "BaseIO.h"

typedef short decimal;

class SSD1331
{
    // Datentypen
public:
    struct Color
    {
        decimal R, G, B;
        static const Color Black;
        static const Color DarkGrey;
        static const Color LightGrey;
        static const Color White;
        static const Color Red;
        static const Color Orange;
        static const Color Yellow;
        static const Color Green;
        static const Color Blue;
        static const Color Violett;
    };

    // Felder
private:
    static const byte Cmd_ContrastA       = 0x81;
    static const byte Cmd_ContrastB       = 0x82;
    static const byte Cmd_ContrastC       = 0x83;
    static const byte Cmd_MasterCurrent   = 0x87;
    static const byte Cmd_SecPrechargeA   = 0x8A;
    static const byte Cmd_SecPrechargeB   = 0x8B;
    static const byte Cmd_SecPrechargeC   = 0x8C;
    static const byte Cmd_Remap           = 0xA0;
    static const byte Cmd_MasterConf      = 0xAD;
    static const byte Cmd_DisplayOff      = 0xAE;
    static const byte Cmd_DisplayOn       = 0xAF;
    static const byte Cmd_Precharge       = 0xB1;
    static const byte Cmd_ClockDiv        = 0xB3;
    static const byte Cmd_PrechargeLevel  = 0xBB;

    static const byte Cmd_SetColumnAddr   = 0x15;
    static const byte Cmd_SetRowAddr      = 0x75;
    static const byte Cmd_DrawLine        = 0x21;
    static const byte Cmd_DrawRect        = 0x22;
    static const byte Cmd_Copy            = 0x23;
    static const byte Cmd_Clear           = 0x25;
    static const byte Cmd_Fill            = 0x26;
    static const byte FontData[];

    DigitalOut* Pin_RST;    // Reset Pin (0 = Reset)
    DigitalOut* Pin_CS;     // ChipSelect Pin
    DigitalOut* Pin_DC;     // Data/Command Pin (1 = Data, 0 = Command)
    bool IsFillActive;      // Gibt an, ob Rechtecke gefüllt werden
    bool IsInCommandMode;   // Gibt an, ob sich das Display im Kommandomodus befindet
    bool IsDrawing;         // Gibt an, ob der ChipSelect des Displays aktiviert ist

    // Konstruktor
public:
    SSD1331();

    // Methoden
public:
    // Initialisiert den SSD1331 Chip
    void Begin(DigitalOut* pin_RST, DigitalOut* pin_CS, DigitalOut* pin_DC, bool flipHorizontal = false, bool flipVertical = false);

    // Startet einen Zeichenvorgang auf dem Display
    void StartDrawing();

    // Beendet einen Zeichenvorgang auf dem Display
    void EndDrawing();

    // Zeichnet ein Testbild mit den vorgegebenen Farben
    void DrawTestPreset();

    // Zeichnet zwei Farbverläufe
    void DrawTestGradient();

    // Zeichnet eine Linie
    void DrawLine(byte x1, byte y1, byte x2, byte y2, Color c);

    // Zeichnet ein Rechteck
    void DrawRect(byte x1, byte y1, byte x2, byte y2, Color line);

    // Zeichnet ein gefülltes Rechteck
    void DrawRectFilled(byte x1, byte y1, byte x2, byte y2, Color line, Color fill);

    // Schreibt Text
    void DrawString(byte x, byte y, const char* text, Color foreground, Color background);

    // Kopiert einen Bereich
    void Copy(byte x1, byte y1, byte x2, byte y2, byte xNew, byte yNew);

    // Löscht einen Bereich
    void Clear(byte x1 = 0, byte y1 = 0, byte x2 = 95, byte y2 = 63);

    // Wandelt HSV Farbmischung in RGB um
    static Color HSVToRGB(float h, float s, float v);

private:
    // sendet Befehle über SPI
    void WriteCommand(byte* command, int count);

    // sendet Daten über SPI
    void WriteData(byte* data, int count);

    // Berechnet den Modulo einer Dezimalzahl
    static float Mod(float a, float b)
    {
        while (a >= b) a -= b;
        return a;
    }

    // Gibt für einen Buchstaben die Position im FontData-Array an (muss noch mit 12 multipliziert werden)
    static int CharToFontPos(char c)
    {
        if ('A' <= c && c <= 'Z')
            return c - 'A';
        if ('a' <= c && c <= 'z')
            return c - 'a' + 26;
        if ('0' <= c && c <= '9')
            return c - '0' + 26 + 26;
        switch (c)
        {
            case '!': return 26 + 26 + 10 + 0;
            case ',': return 26 + 26 + 10 + 1;
            case '-': return 26 + 26 + 10 + 2;
            case '.': return 26 + 26 + 10 + 3;
            case ':': return 26 + 26 + 10 + 4;
            case ';': return 26 + 26 + 10 + 5;
            case ' ': return 26 + 26 + 10 + 6;
            default: return CharToFontPos(' ');
        }
    }
};

#endif
