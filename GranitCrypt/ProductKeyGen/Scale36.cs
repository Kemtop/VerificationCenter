using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KeyGen
{
    class Global
    {
        internal static Scale36[] ConvertTable = new Scale36[77] {
        new Scale36() {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {28, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {20, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {4, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {8, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {16, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {32, 20, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {28, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {20, 11, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {4, 23, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {8, 10, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {16, 20, 14, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {32, 4, 29, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {28, 9, 22, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {20, 19, 8, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {4, 3, 17, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {8, 6, 34, 8, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {16, 12, 32, 17, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {32, 24, 28, 35, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {28, 13, 21, 35, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {20, 27, 6, 35, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {4, 19, 13, 34, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {8, 2, 27, 32, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {16, 4, 18, 29, 15, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {32, 8, 0, 23, 31, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {28, 17, 0, 10, 27, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {20, 35, 0, 20, 18, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {4, 35, 1, 4, 1, 35, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {8, 34, 3, 8, 2, 34, 3, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {16, 32, 7, 16, 4, 32, 7, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {32, 28, 15, 32, 8, 28, 15, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {28, 21, 31, 28, 17, 20, 31, 0, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {20, 7, 27, 21, 35, 4, 27, 1, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {4, 15, 18, 7, 35, 9, 18, 3, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {8, 30, 0, 15, 34, 19, 0, 7, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {16, 24, 1, 30, 32, 3, 1, 14, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {32, 12, 3, 24, 29, 7, 2, 28, 0, 0, 0, 0, 0, 0, 0 },
        new Scale36() {28, 25, 6, 12, 23, 15, 4, 20, 1, 0, 0, 0, 0, 0, 0 },
        new Scale36() {20, 15, 13, 24, 10, 31, 8, 4, 3, 0, 0, 0, 0, 0, 0 },
        new Scale36() {4, 31, 26, 12, 21, 26, 17, 8, 6, 0, 0, 0, 0, 0, 0 },
        new Scale36() {8, 26, 17, 25, 6, 17, 35, 16, 12, 0, 0, 0, 0, 0, 0 },
        new Scale36() {16, 16, 35, 14, 13, 34, 34, 33, 24, 0, 0, 0, 0, 0, 0 },
        new Scale36() {32, 32, 34, 29, 26, 32, 33, 31, 13, 1, 0, 0, 0, 0, 0 },
        new Scale36() {28, 29, 33, 23, 17, 29, 31, 27, 27, 2, 0, 0, 0, 0, 0 },
        new Scale36() {20, 23, 31, 11, 35, 22, 27, 19, 19, 5, 0, 0, 0, 0, 0 },
        new Scale36() {4, 11, 27, 23, 34, 9, 19, 3, 3, 11, 0, 0, 0, 0, 0 },
        new Scale36() {8, 22, 18, 11, 33, 19, 2, 7, 6, 22, 0, 0, 0, 0, 0 },
        new Scale36() {16, 8, 1, 23, 30, 3, 5, 14, 12, 8, 1, 0, 0, 0, 0 },
        new Scale36() {32, 16, 2, 10, 25, 7, 10, 28, 24, 16, 2, 0, 0, 0, 0 },
        new Scale36() {28, 33, 4, 20, 14, 15, 20, 20, 13, 33, 4, 0, 0, 0, 0 },
        new Scale36() {20, 31, 9, 4, 29, 30, 4, 5, 27, 30, 9, 0, 0, 0, 0 },
        new Scale36() {4, 27, 19, 8, 22, 25, 9, 10, 18, 25, 19, 0, 0, 0, 0 },
        new Scale36() {8, 18, 3, 17, 8, 15, 19, 20, 0, 15, 3, 1, 0, 0, 0 },
        new Scale36() {16, 0, 7, 34, 16, 30, 2, 5, 1, 30, 6, 2, 0, 0, 0 },
        new Scale36() {32, 0, 14, 32, 33, 24, 5, 10, 2, 24, 13, 4, 0, 0, 0 },
        new Scale36() {28, 1, 28, 28, 31, 13, 11, 20, 4, 12, 27, 8, 0, 0, 0 },
        new Scale36() {20, 3, 20, 21, 27, 27, 22, 4, 9, 24, 18, 17, 0, 0, 0 },
        new Scale36() {4, 7, 4, 7, 19, 19, 9, 9, 18, 12, 1, 35, 0, 0, 0 },
        new Scale36() {8, 14, 8, 14, 2, 3, 19, 18, 0, 25, 2, 34, 1, 0, 0 },
        new Scale36() {16, 28, 16, 28, 4, 6, 2, 1, 1, 14, 5, 32, 3, 0, 0 },
        new Scale36() {32, 20, 33, 20, 9, 12, 4, 2, 2, 28, 10, 28, 7, 0, 0 },
        new Scale36() {28, 5, 31, 5, 19, 24, 8, 4, 4, 20, 21, 20, 15, 0, 0 },
        new Scale36() {20, 11, 26, 11, 2, 13, 17, 8, 8, 4, 7, 5, 31, 0, 0 },
        new Scale36() {4, 23, 16, 23, 4, 26, 34, 16, 16, 8, 14, 10, 26, 1, 0 },
        new Scale36() {8, 10, 33, 10, 9, 16, 33, 33, 32, 16, 28, 20, 16, 3, 0 },
        new Scale36() {16, 20, 30, 21, 18, 32, 30, 31, 29, 33, 20, 5, 33, 6, 0 },
        new Scale36() {32, 4, 25, 7, 1, 29, 25, 27, 23, 31, 5, 11, 30, 13, 0 },
        new Scale36() {28, 9, 14, 15, 2, 22, 15, 19, 11, 27, 11, 22, 24, 27, 0 },
        new Scale36() {20, 19, 28, 30, 4, 8, 31, 2, 23, 18, 23, 8, 13, 19, 1 },
        new Scale36() {4, 3, 21, 25, 9, 16, 26, 5, 10, 1, 11, 17, 26, 2, 3 },
        new Scale36() {8, 6, 6, 15, 19, 32, 16, 11, 20, 2, 22, 34, 16, 5, 6 },
        new Scale36() {16, 12, 12, 30, 2, 29, 33, 22, 4, 5, 8, 33, 33, 10, 12 }
        };
    }

    class Scale36 : List<uint>
    {
        private void Resize(int count)
        {
            if (Count < count)
            {
                while (Count < count)
                {
                    Add(0);
                }
            }
        }

        public static Scale36 operator +(Scale36 val1, Scale36 val2)
        {
            Scale36 Summ = new Scale36();
            uint Reload = 0;
            val1.Resize(Math.Max(val1.Count(), val2.Count()));
            val2.Resize(Math.Max(val1.Count(), val2.Count()));

            for (int i = 0; i < val1.Count(); ++i)
            {
                uint bit = val1[i] + val2[i] + Reload;
                Reload = bit / 36;
                if (Summ.Count() < i + 1) Summ.Add(0);
                Summ[i] = bit - Reload * 36;
            }
            if (Reload > 0) Summ.Add(Reload);
            return Summ;
        }

        public static Scale36 operator -(Scale36 val1, Scale36 val2)
        {
            Scale36 Sub = new Scale36();
            uint Reload = 0;
            val1.Resize(Math.Max(val1.Count(), val2.Count()));
            val2.Resize(Math.Max(val1.Count(), val2.Count()));

            for (int i = 0; i < val1.Count(); ++i)
            {
                uint bit;
                if (val1[i] < (val2[i] + Reload))
                {
                    bit = 36 + val1[i] - val2[i] - Reload;
                    Reload = 1;
                }
                else
                {
                    bit = val1[i] - val2[i] - Reload;
                    Reload = 0;
                }
                Sub.Add(bit);
            }
            return Sub;
        }

        public static bool operator >(Scale36 val1, Scale36 val2)
        {
            return false;
        }

        public static bool operator <(Scale36 val1, Scale36 val2)
        {
            val1.Resize(Math.Max(val1.Count(), val2.Count()));
            val2.Resize(Math.Max(val1.Count(), val2.Count()));

            for (int i = 0; i < val1.Count(); ++i)
            {
                if (val1[val1.Count() - i - 1] != val2[val2.Count() - i - 1]) return val1[val1.Count() - i - 1] < val2[val2.Count() - i - 1];
            }
            return false;
        }

        public void FromString(string value)
        {
            this.Clear();
            value = value.ToUpper();
            for (int i = value.Count() - 1; i >= 0; --i)
            {
                uint ch = (uint)value[i];
                if (ch < 65)
                    this.Add(ch - 48);
                else
                    this.Add(ch - 55);
            }
        }

        public override string ToString()
        {
            string Res = "";
            for (int i = this.Count() - 1; i >= 0; --i)
            {
                if (this[i] < 10)
                    Res += (char)(this[i] + 48);
                else
                    Res += (char)(this[i] + 55);
            }
            return Res;
        }

        public void FromBitArray(BitArray Bits)
        {
            Scale36 Result = new Scale36() { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            for (int i = 0; i < 77; ++i)
            {
                if (Bits[i]) Result = Result + Global.ConvertTable[i];
            }
            this.Clear();
            for (int i = 0; i < Result.Count(); ++i)
            {
                this.Add(Result[i]);
            }
        }

        public void FromBitArray(BitArray Bits,int Len)
        {
            Scale36 Result = new Scale36() { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            for (int i = 0; i < Len; ++i)
            {
                if (Bits[i]) Result = Result + Global.ConvertTable[i];
            }
            this.Clear();
            for (int i = 0; i < Result.Count(); ++i)
            {
                this.Add(Result[i]);
            }
        }


        public void FromUInt32(UInt32 Dig)
        {
            Scale36 Result = new Scale36() { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

            UInt32 pow = 0;//Степень двойки.
            UInt32 t = 0; //Временное значение.

            for (int i=0;i<32;i++)
            {
                pow = 1;
                pow = pow << i;//Степень двойки.
                t = Dig & pow;
                //Если в бите стоит 1.
                if(t==pow) Result = Result + Global.ConvertTable[i];

            }

            this.Clear();

            for (int i = 0; i < Result.Count(); ++i)
            {
                this.Add(Result[i]);
            }
        }




        public BitArray ToBitArray()
        {
            Scale36 value = new Scale36();
            for (int i = 0; i < this.Count(); ++i) value.Add(this[i]);
            BitArray Result = new BitArray(77, false);
            for (int i = 76; i >= 0; --i)
            {
                if (!(value < Global.ConvertTable[i]))
                {
                    Result.Set(i, true);
                    value = value - Global.ConvertTable[i];
                }
            }
            return Result;
        }
    }
}
