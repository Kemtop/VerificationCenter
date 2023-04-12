using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KeyGen
{
    /// <summary>
    /// Класс содержите биективные методы.
    /// </summary>
    class Biektion
    {
        /// <summary>
        /// Сравниваю два массива размерностью len. Если хотя бы один бит отличается в массивах-возвращаю false.
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <param name="len"></param>
        /// <returns></returns>
        bool CompireArray(bool[] a, bool[] b, int len)
        {
            //Сравниваю биты.
            bool Res = false;
            for (int i = 0; i < len; i++)
            {
                Res = a[i] ^ b[i]; //сложение по модулю 2 возвращает 1 если биты отличаются.
                if (Res) return false; //1-Биты не совпали, ошибка.
            }

            return true;
        }


        public void Biektiv0(ref bool[] Bits, bool[] block1)
        {
            Bits[76] = block1[9]; //1 3 4 6 8 9 10
            Bits[24] = block1[3];
            Bits[75] = block1[10];
            Bits[0] = block1[6];
            Bits[28] = block1[1];
            Bits[58] = block1[8];
            Bits[38] = block1[4];
        }

        /// <summary>
        /// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
        /// </summary>
        public bool ChekBiektiv0(bool[] Y, ref bool[] ControlBits)
        {
            bool[] hashBits = new bool[7];

            hashBits[0] = Y[9];
            hashBits[1] = Y[3];
            hashBits[2] = Y[10];
            hashBits[3] = Y[6];
            hashBits[4] = Y[1];
            hashBits[5] = Y[8];
            hashBits[6] = Y[4];

            bool[] CBits = new bool[7];
            CBits[0] = ControlBits[76];
            CBits[1] = ControlBits[24];
            CBits[2] = ControlBits[75];
            CBits[3] = ControlBits[0];
            CBits[4] = ControlBits[28];
            CBits[5] = ControlBits[58];
            CBits[6] = ControlBits[38];


            //Сравниваю массивы.
            if (!CompireArray(hashBits, CBits, 7))
                return false;

            return true;
        }



        public void Biektiv1(ref bool[] Bits, bool[] block1)
        {
            Bits[44] = block1[5]; //0 1 5 6 7 10 11
            Bits[66] = block1[11];
            Bits[11] = block1[1];
            Bits[6] = block1[6];
            Bits[33] = block1[10];
            Bits[60] = block1[7];
            Bits[34] = block1[0];
        }

        /// <summary>
        /// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
        /// </summary>
        public bool ChekBiektiv1(bool[] Y, ref bool[] ControlBits)
        {
            bool[] hashBits = new bool[7];

            hashBits[0] = Y[5];
            hashBits[1] = Y[11];
            hashBits[2] = Y[1];
            hashBits[3] = Y[6];
            hashBits[4] = Y[10];
            hashBits[5] = Y[7];
            hashBits[6] = Y[0];


            bool[] CBits = new bool[7];
            CBits[0] = ControlBits[44];
            CBits[1] = ControlBits[66];
            CBits[2] = ControlBits[11];
            CBits[3] = ControlBits[6];
            CBits[4] = ControlBits[33];
            CBits[5] = ControlBits[60];
            CBits[6] = ControlBits[34];


            //Сравниваю массивы.
            if (!CompireArray(hashBits, CBits, 7))
                return false;

            return true;
        }



        public void Biektiv2(ref bool[] Bits, bool[] block1)
        {
            Bits[52] = block1[6];
            Bits[10] = block1[2];
            Bits[67] = block1[8];
            Bits[1] = block1[11];
            Bits[73] = block1[3];
            Bits[29] = block1[0];
            Bits[2] = block1[1];

        }

        /// <summary>
        /// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
        /// </summary>
        public bool ChekBiektiv2(bool[] Y, ref bool[] ControlBits)
        {
            //Функция перестановки.
            bool[] hashBits = new bool[7];
            hashBits[0] = Y[6];
            hashBits[1] = Y[2];
            hashBits[2] = Y[8];
            hashBits[3] = Y[11];
            hashBits[4] = Y[3];
            hashBits[5] = Y[0];
            hashBits[6] = Y[1];


            bool[] CBits = new bool[7];
            CBits[0] = ControlBits[52];
            CBits[1] = ControlBits[10];
            CBits[2] = ControlBits[67];
            CBits[3] = ControlBits[1];
            CBits[4] = ControlBits[73];
            CBits[5] = ControlBits[29];
            CBits[6] = ControlBits[2];


            //Сравниваю массивы.
            if (!CompireArray(hashBits, CBits, 7))
                return false;

            return true;
        }


        public void Biektiv3(ref bool[] Bits, bool[] block1)
        {
            Bits[70] = block1[8];
            Bits[54] = block1[3];
            Bits[50] = block1[4];
            Bits[17] = block1[5];
            Bits[41] = block1[9];
            Bits[62] = block1[10];
            Bits[39] = block1[11];

        }

        /// <summary>
        /// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
        /// </summary>
        public bool ChekBiektiv3(bool[] Y, ref bool[] ControlBits)
        {
            bool[] hashBits = new bool[7];
            hashBits[0] = Y[8];
            hashBits[1] = Y[3];
            hashBits[2] = Y[4];
            hashBits[3] = Y[5];
            hashBits[4] = Y[9];
            hashBits[5] = Y[10];
            hashBits[6] = Y[11];


            bool[] CBits = new bool[7];
            CBits[0] = ControlBits[70];
            CBits[1] = ControlBits[54];
            CBits[2] = ControlBits[50];
            CBits[3] = ControlBits[17];
            CBits[4] = ControlBits[41];
            CBits[5] = ControlBits[62];
            CBits[6] = ControlBits[39];
            //Сравниваю массивы.
            if (!CompireArray(hashBits, CBits, 7))
                return false;

            return true;
        }

        public void Biektiv4(ref bool[] Bits, bool[] block1)
        {
            Bits[22] = block1[7];
            Bits[4] = block1[5];
            Bits[53] = block1[11];
            Bits[26] = block1[1];
            Bits[37] = block1[8];
            Bits[31] = block1[3];
            Bits[71] = block1[9];

        }

        /// <summary>
        /// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
        /// </summary>
        public bool ChekBiektiv4(bool[] Y, ref bool[] ControlBits)
        {
            bool[] hashBits = new bool[7];
            bool[] CBits = new bool[7];

            hashBits[0] = Y[7];
            hashBits[1] = Y[5];
            hashBits[2] = Y[11];
            hashBits[3] = Y[1];
            hashBits[4] = Y[8];
            hashBits[5] = Y[3];
            hashBits[6] = Y[9];

            CBits[0] = ControlBits[22];
            CBits[1] = ControlBits[4];
            CBits[2] = ControlBits[53];
            CBits[3] = ControlBits[26];
            CBits[4] = ControlBits[37];
            CBits[5] = ControlBits[31];
            CBits[6] = ControlBits[71];


            //Сравниваю массивы.
            if (!CompireArray(hashBits, CBits, 7))
                return false;

            return true;
        }



        public void Biektiv5(ref bool[] Bits, bool[] block1)
        {
            Bits[16] = block1[1];
            Bits[40] = block1[6];
            Bits[56] = block1[4];
            Bits[5] = block1[5];
            Bits[65] = block1[0];
            Bits[19] = block1[7];
            Bits[49] = block1[11];

        }


        /// <summary>
        /// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
        /// </summary>
        public bool ChekBiektiv5(bool[] Y, ref bool[] ControlBits)
        {
            bool[] hashBits = new bool[7];
            bool[] CBits = new bool[7];


            hashBits[0] = Y[1];
            hashBits[1] = Y[6];
            hashBits[2] = Y[4];
            hashBits[3] = Y[5];
            hashBits[4] = Y[0];
            hashBits[5] = Y[7];
            hashBits[6] = Y[11];

            CBits[0] = ControlBits[16];
            CBits[1] = ControlBits[40];
            CBits[2] = ControlBits[56];
            CBits[3] = ControlBits[5];
            CBits[4] = ControlBits[65];
            CBits[5] = ControlBits[19];
            CBits[6] = ControlBits[49];

            //Сравниваю массивы.
            if (!CompireArray(hashBits, CBits, 7))
                return false;

            return true;
        }




        public void Biektiv6(ref bool[] Bits, bool[] block1)
        {
            Bits[68] = block1[10];
            Bits[25] = block1[6];
            Bits[57] = block1[2];
            Bits[48] = block1[3];
            Bits[7] = block1[0];
            Bits[51] = block1[1];
            Bits[42] = block1[4];

        }


        /// <summary>
        /// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
        /// </summary>
        public bool ChekBiektiv6(bool[] Y, ref bool[] ControlBits)
        {

            bool[] hashBits = new bool[7];
            bool[] CBits = new bool[7];

            hashBits[0] = Y[10];
            hashBits[1] = Y[6];
            hashBits[2] = Y[2];
            hashBits[3] = Y[3];
            hashBits[4] = Y[0];
            hashBits[5] = Y[1];
            hashBits[6] = Y[4];

            //
            CBits[0] = ControlBits[68];
            CBits[1] = ControlBits[25];
            CBits[2] = ControlBits[57];
            CBits[3] = ControlBits[48];
            CBits[4] = ControlBits[7];
            CBits[5] = ControlBits[51];
            CBits[6] = ControlBits[42];


            //Сравниваю массивы.
            if (!CompireArray(hashBits, CBits, 7))
                return false;

            return true;
        }


        public void Biektiv7(ref bool[] Bits, bool[] block1)
        {
            Bits[32] = block1[11];
            Bits[55] = block1[5];
            Bits[13] = block1[1];
            Bits[14] = block1[7];
            Bits[72] = block1[0];
            Bits[8] = block1[3];
            Bits[74] = block1[8];

        }



        /// <summary>
        /// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
        /// </summary>
        public bool ChekBiektiv7(bool[] Y, ref bool[] ControlBits)
        {

            bool[] hashBits = new bool[7];
            bool[] CBits = new bool[7];

            hashBits[0] = Y[11];
            hashBits[1] = Y[5];
            hashBits[2] = Y[1];
            hashBits[3] = Y[7];
            hashBits[4] = Y[0];
            hashBits[5] = Y[3];
            hashBits[6] = Y[8];


            CBits[0] = ControlBits[32];
            CBits[1] = ControlBits[55];
            CBits[2] = ControlBits[13];
            CBits[3] = ControlBits[14];
            CBits[4] = ControlBits[72];
            CBits[5] = ControlBits[8];
            CBits[6] = ControlBits[74];


            //Сравниваю массивы.
            if (!CompireArray(hashBits, CBits, 7))
                return false;

            return true;
        }


        public void Biektiv8(ref bool[] Bits, bool[] block1)
        {
            Bits[3] = block1[5];
            Bits[18] = block1[10];
            Bits[46] = block1[1];
            Bits[23] = block1[0];
            Bits[15] = block1[6];
            Bits[64] = block1[7];
            Bits[12] = block1[9];
        }

        /// <summary>
        /// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
        /// </summary>
        public bool ChekBiektiv8(bool[] Y, ref bool[] ControlBits)
        {

            bool[] hashBits = new bool[7];
            bool[] CBits = new bool[7];

            hashBits[0] = Y[5];
            hashBits[1] = Y[10];
            hashBits[2] = Y[1];
            hashBits[3] = Y[0];
            hashBits[4] = Y[6];
            hashBits[5] = Y[7];
            hashBits[6] = Y[9];

            CBits[0] = ControlBits[3];
            CBits[1] = ControlBits[18];
            CBits[2] = ControlBits[46];
            CBits[3] = ControlBits[23];
            CBits[4] = ControlBits[15];
            CBits[5] = ControlBits[64];
            CBits[6] = ControlBits[12];

            //Сравниваю массивы.
            if (!CompireArray(hashBits, CBits, 7))
                return false;

            return true;
        }

        /// <summary>
        /// Отображение множество в котором закодирована дата окончания действия на другое множество.
        /// </summary>
        /// <param name="Bits"></param>
        /// <param name="block1"></param>
        public void BiektivDateBlock(ref bool[] Bits, bool[] bitDays)
        {
            Bits[27] = bitDays[0];
            Bits[30] = bitDays[1];
            Bits[36] = bitDays[2];
            Bits[47] = bitDays[3];
            Bits[61] = bitDays[4];
            Bits[45] = bitDays[5];
            Bits[9] = bitDays[6];
            Bits[20] = bitDays[7];
            Bits[43] = bitDays[8];
            Bits[59] = bitDays[9];
            Bits[21] = bitDays[10];
            Bits[35] = bitDays[11];
            Bits[63] = bitDays[12];
            Bits[69] = bitDays[13];

        }

        /// <summary>
        /// Обратное отображение множество в котором закодирована дата окончания действия на другое множество.
        /// </summary>
        /// <param name="Bits"></param>
        /// <param name="block1"></param>
        public void UnBiektivDateBlock(ref bool[] dt, bool[] bits)
        {
            dt[0] = bits[27];
            dt[1] = bits[30];
            dt[2] = bits[36];
            dt[3] = bits[47];
            dt[4] = bits[61];
            dt[5] = bits[45];
            dt[6] = bits[9];
            dt[7] = bits[20];
            dt[8] = bits[43];
            dt[9] = bits[59];
            dt[10] = bits[21];
            dt[11] = bits[35];
            dt[12] = bits[63];
            dt[13] = bits[69];
        }


        }
}
