// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) 
{
    BitLen = len;
    if (len < 0) {
        throw length_error("Incorrect lenght");
    }
    if (BitLen % (8 * sizeof(TELEM)) == 0) {
        MemLen = BitLen / (8 * sizeof(TELEM));
    }
    else {
        MemLen = BitLen / (8 * sizeof(TELEM)) + 1;
    }
    pMem = new TELEM[MemLen]{ 0 };
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return (n / (8 * sizeof(TELEM)));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n > -1 && n < BitLen) {
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    }
    else {
        throw range_error("Out of range");
    }
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n > -1 && n < BitLen) {
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    }
    else {
        throw range_error("Out of range");
    }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n > -1 && n < BitLen) {
        return (pMem[GetMemIndex(n)] & GetMemMask(n));
    }
    else {
        throw range_error("Out of range");
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen) {
        for (int i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i])
                return 0;
        }
        return 1;
    }
    else return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField F(max(BitLen, bf.BitLen));
    for (int i = 0; i < min(MemLen, bf.MemLen); i++)
        F.pMem[i] = pMem[i] | bf.pMem[i];
    return F;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField F(max(BitLen, bf.BitLen));
    for (int i = 0; i < min(MemLen, bf.MemLen); i++) {
        F.pMem[i] = pMem[i] | bf.pMem[i];
    }
    for (int i = min(MemLen, bf.MemLen); i < F.MemLen; i++) {
        if (BitLen > bf.BitLen) {
            F.pMem[i] = pMem[i];
        }
        else {
            F.pMem[i] = bf.pMem[i];
        }
    }
    return F;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField F(BitLen);
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) == 0) {
            F.SetBit(i);
        }
        else {
            F.ClrBit(i);
        }
    }
    return F;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int tmp = 0;
    for (int i = 0; i < bf.BitLen; i++){
        istr >> tmp;
        bf.SetBit(tmp);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}

