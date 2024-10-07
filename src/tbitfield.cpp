// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>
#include <algorithm>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) : BitLen(len)
{
    if (len > 0) {
        if (BitLen % (8 * sizeof(TELEM)) == 0) {
            MemLen = BitLen / (8 * sizeof(TELEM));
        }
        else {
            MemLen = BitLen / (8 * sizeof(TELEM)) + 1;
        }
        pMem = new TELEM[MemLen]{ 0 };
        if (pMem == nullptr) {
            throw bad_alloc();
        }
    }
    else throw length_error("Incorrect lenght!");
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    if (pMem == nullptr)
        throw bad_alloc();
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
    return ((TELEM)1 << n % (8 * sizeof(TELEM)));
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
        if ((pMem[GetMemIndex(n)] & GetMemMask(n)) == 0) {
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        throw range_error("Out of range");
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        if (bf.MemLen != MemLen) {
            MemLen = bf.MemLen;
            delete[] pMem;
            pMem = new TELEM[MemLen];
            if (pMem == nullptr) {
                throw bad_alloc();
            }
        }
        BitLen = bf.BitLen;
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
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

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField F(max(BitLen, bf.BitLen));
    for (int i = 0; i < min(MemLen, bf.MemLen); i++) {
        F.pMem[i] = pMem[i] | bf.pMem[i];
    }
    for (int i = min(MemLen, bf.MemLen); i < max(MemLen, bf.MemLen); i++) {
        if (BitLen > bf.BitLen) {
            F.pMem[i] = pMem[i];
        }
        else {
            F.pMem[i] = bf.pMem[i];
        }
    }
    return F;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField F(max(BitLen, bf.BitLen));
    for (int i = 0; i < min(MemLen, bf.MemLen); i++)
        F.pMem[i] = pMem[i] & bf.pMem[i];
    return F;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField F(BitLen);
    for (int i = 0; i < MemLen; i++) {
        F.pMem[i] = ~pMem[i];
    }
    int tail = BitLen % (sizeof(TELEM) * 8);
    if (tail != 0) {
        F.pMem[MemLen - 1] = F.pMem[MemLen - 1] & (GetMemMask(tail) - 1);
    }
    return F;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    string cin;
    istr >> cin;
    if (cin.size() <= bf.BitLen)
        for (int i = 0; i < cin.size(); i++) {
            if (cin[i] == '1') bf.SetBit(i);
            else if (cin[i] == '0') bf.ClrBit(i);
            else throw invalid_argument("A symbol must be 0 or 1!");
        }
    else throw length_error("Incorrect length!");
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}
