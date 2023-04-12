using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KeyGen
{
    /// <summary>
    /// Функции. В качестве аргумента принимают 32битный массив.
    /// </summary>
    class SecureFunc
    {
        /// <summary>
        /// Массив для работы метода SF0;
        /// </summary>
        UInt64[] ArSF0 = new UInt64[100];


        /// <summary>
        /// Преобразовывает базовое число в массив бит, проверяет его.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Arr32"></param>
        /// <returns></returns>
       public bool ConvertBaseNum(UInt32 BaseNum,ref bool[] Arr32 )
        {
            UInt32 pow = 0;//Степень двойки.
            UInt32 t = 0; //Временное значение.
            int cnt = 0;

            for (int i = 0; i < 32; i++)
            {
                pow = 1;
                pow = pow << i;//Степень двойки.
                t = BaseNum & pow;
                //Если в бите стоит 1.
                if (t == pow)
                {
                    Arr32[i] = true;
                    cnt++;
                }
                else Arr32[i] = false;
            }

            //Базовое число не может содержать более 26 бит.
            if (cnt > 26) return false;

            return true;
        }






        /// <summary>
        /// Вращения 32 битного кольца. Сдвиг с обратной связью-"Вращение кольца".
        /// </summary>
        public void TwistTheRing(int rotate, ref bool[] R)
        {
            bool tmp1 = false;
            bool tmp2 = false;

            //Вращение кольца n-раз.
            for (int i = 0; rotate < 5; i++)
            {
                //Замкнутый сдвиг блока размером 32 бита.
                tmp1 = R[31]; //Копирую последний бит который станет первым.              
                for (int j = 0; j < 32; j++)
                {
                    tmp2 = R[j]; //Сохраняю текущее значение бита.
                    R[j] = tmp1;//Сдвигаю
                    tmp1 = tmp2;//Подготавливаю для следующего сдвига.
                }

            }
        }

        /// <summary>
        /// Подсчитывает количество единиц в массиве длиной len
        /// </summary>
        /// <param name="Val"></param>
        /// <returns></returns>
        int CountUnits(bool[] Val, int Len)
        {
            int cnt = 0;

            for (int i = 0; i < Len; i++)
            {
                if (Val[i] == true) cnt++;
            }

            return cnt;
        }


        /// <summary>
        /// Заполняет массив  ArSF0 данными некоторой функции.  
        /// </summary>
        void FillArSF0()
        {
            Int64 y, x;
            UInt64 ret;

            for (int i = 1; i < 100; i++)
            {
                x = i * (-15200);
                y = (715 * x) + 25300;
                ret = ((UInt64)y) & 0xffffffff;
                ret = ret << 16;
                ArSF0[i - 1] = ret;
            }

        }

        /// <summary>
        /// Конвертирует битовый массив в UInt64.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="result"></param>
        void ConvertBaseNum(bool[] BaseNum, ref UInt64 result)
        {
            bool[] In = new bool[32]; //Временный массив.

            //Копирую базовое число во временный массив.
            for (int i = 0; i < 26; i++)
            {
                In[i] = BaseNum[i];
            }

            UInt32[] bVAL = new UInt32[32]; //Значения 2ки в соответствующей степени.

            bVAL[0] = 0x00000001;//1
            bVAL[1] = 0x00000002;//2
            bVAL[2] = 0x00000004;//4
            bVAL[3] = 0x00000008;//8
            bVAL[4] = 0x00000010;//16
            bVAL[5] = 0x00000020;//32
            bVAL[6] = 0x00000040;//64
            bVAL[7] = 0x00000080;//128
            bVAL[8] = 0x00000100;//256
            bVAL[9] = 0x00000200;//512
            bVAL[10] = 0x00000400;//1024
            bVAL[11] = 0x00000800;//2048
            bVAL[12] = 0x00001000;//4096
            bVAL[13] = 0x00002000;//8192
            bVAL[14] = 0x00004000;//16384
            bVAL[15] = 0x00008000;//32768
            bVAL[16] = 0x00010000;//65536
            bVAL[17] = 0x00020000;//131072
            bVAL[18] = 0x00040000;//262144
            bVAL[19] = 0x00080000;//524288
            bVAL[20] = 0x00100000;//1048576
            bVAL[21] = 0x00200000;
            bVAL[22] = 0x00400000;
            bVAL[23] = 0x00800000;
            bVAL[24] = 0x01000000;
            bVAL[25] = 0x02000000;
            bVAL[26] = 0x04000000;
            bVAL[27] = 0x08000000;
            bVAL[28] = 0x10000000;
            bVAL[29] = 0x20000000;
            bVAL[30] = 0x40000000;
            bVAL[31] = 0x80000000;//2147483648


            UInt64 Res = 0;//Результат преобразования.

            for (int i = 0; i < 32; i++)
            {
                if (In[i] == true) //1 в разряде
                {
                    Res = Res + bVAL[i]; //Преобразовываем.
                }
            }

            result = Res;
        }

        /// <summary>
        /// Конвертирует битовый массив в Int64.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="result"></param>
        void ConvertBaseNum(bool[] BaseNum, ref Int64 result)
        {
            bool[] In = new bool[32]; //Временный массив.

            //Копирую базовое число во временный массив.
            for (int i = 0; i < 26; i++)
            {
                In[i] = BaseNum[i];
            }

            Int64 tmp = 0;
            result = 0;

            for (int i = 0; i < 32; i++)
            {
                if (In[i] == true)
                {
                    tmp = 1;
                    tmp = tmp << i; //2 в соответсвующей степени.
                    result = result + tmp;
                }

            }

        }


        /// <summary>
        /// Преобразовывает 64битное без знаковое число в байт массив. 
        /// </summary>
        /// <param name="Arr"></param>
        /// <param name="Result"></param>
        void ConvertU64ToBool(UInt64 d, ref bool[] Result)
        {
            bool[] res = new bool[32]; //Временный массив.

            UInt64 tmp = 0;
            UInt64 PowOF2 = 0;//Степень двойки.

            for (int i = 0; i < 32; i++)
            {
                PowOF2 = 1;
                PowOF2 = PowOF2 << i; //Возведение в степень 2ки.

                tmp = d & PowOF2; //Вырезание не нужных бит.
                //Сравнение
                if (tmp == PowOF2) //Присутствует 1 текущем разряде.
                    res[i] = true;

            }

            Result = res;
        }



        /// <summary>
        /// Преобразовывает 64битное без знаковое число в байт 32 битный байт массив. 
        /// </summary>
        /// <param name="Arr"></param>
        /// <param name="Result"></param>
        void ConvertI64ToBool(Int64 d, ref bool[] Result)
        {
            bool[] res = new bool[32]; //Временный массив.

            Int64 tmp = 0;
            Int64 PowOF2 = 0;//Степень двойки.

            for (int i = 0; i < 32; i++)
            {
                PowOF2 = 1;
                PowOF2 = PowOF2 << i; //Возведение в степень 2ки.

                tmp = d & PowOF2; //Вырезание не нужных бит.
                //Сравнение
                if (tmp == PowOF2) //Присутствует 1 текущем разряде.
                    res[i] = true;
                else
                    res[i] = false;

            }

            Result = res;
        }


        /// <summary>
        /// Функция для получения первого блока секретных битов.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Dst"></param>
        public void SF0(bool[] BaseNum, ref bool[] Dst)
        {    
            //Вращаю  базовое число.
            TwistTheRing(48, ref BaseNum);

            //Количество единиц в числе.
            int UnitsCnt = CountUnits(BaseNum, 32);

            FillArSF0();//Формирую значения некоторой функции.
            UInt64 a = ArSF0[UnitsCnt]; //Получаю значение соответствующее текущему количеству.
            UInt64 tmp1 = 0; //хранит промежуточное значение
            UInt64 mask = 1;//Маска.
            bool tmp = false;//хранит промежуточное значение

            //Выполняю  побитовое сложение по модулю 2.
            for (int i = 0; i < 26; i++)
            {
                mask = 1; //Готовлю бит для сдвига.
                mask = mask << i;//Устанавливает 1 в нужном бите.
                tmp1 = a & mask; //Вырезаем нужный бит.

                //Если в бите единица тогда результат равен маске, иначе в бите не единица.
                if (tmp1 == mask) tmp = true;
                else tmp = false;

                Dst[i] = BaseNum[i] ^ tmp;
            }
        }

        

        /// <summary>
        /// Функция для получения второго блока секретных битов.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Dst"></param>
        public void SF1(bool[] BaseNum, ref bool[] Dst)
        {
            UInt64 x = 0;
            ConvertBaseNum(BaseNum, ref x); //Конвертируем базовое число в виде байт массива в нормальное число.

            UInt64 y;
            y = (x * x) + 3578;

            //Обратное преобразование
            ConvertU64ToBool(y, ref Dst);
        }


        /// <summary>
        /// Функция для получения третьего блока секретных битов.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Dst"></param>
        public void SF2(bool[] BaseNum, ref bool[] Dst)
        {
            Int64 x = 0;
            //Конвертируем базовое число в виде байт массива в нормальное число.
            ConvertBaseNum(BaseNum, ref x);

            Int64 y;
            y = (x * x) + (x * 2) - 2354;

            //Обратное преобразование
            ConvertI64ToBool(y, ref Dst);
        }
        

        /// <summary>
        /// Функция для получения четвертого блока секретных битов.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Dst"></param>
        public void SF3(bool[] BaseNum, ref bool[] Dst)
        {
            Int64 x = 0;
            //Конвертируем базовое число в виде байт массива в нормальное число.
            ConvertBaseNum(BaseNum, ref x);

            Int64 y;

            y = (715 * x) + 25300;

            //Обратное преобразование
            ConvertI64ToBool(y, ref Dst);
        }




        /// <summary>
        /// Функция для получения пятого блока секретных битов.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Dst"></param>
        public void SF4(bool[] BaseNum, ref bool[] Dst)
        {
            Int64 x = 0;
            //Конвертируем базовое число в виде байт массива в нормальное число.
            ConvertBaseNum(BaseNum, ref x);

            Int64 y;
            y = 2793967291 - ((x * x) - (x * 7) - 5718);

            //Обратное преобразование
            ConvertI64ToBool(y, ref Dst);
        }


        

        /// <summary>
        /// Функция для получения шестого блока секретных битов.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Dst"></param>
        public void SF5(bool[] BaseNum, ref bool[] Dst)
        {
            Int64 x = 0;
            //Конвертируем базовое число в виде байт массива в нормальное число.
            ConvertBaseNum(BaseNum, ref x);

            Int64 y, t;
       
            y = ((x * x) - x * 7 - 5718);
            t = 2 * (x * x) - x * 7 - 10287;
            y = (y + t) + 78;

            //Обратное преобразование
            ConvertI64ToBool(y, ref Dst);
        }

      
        /// <summary>
        /// Функция для получения седьмого блока секретных битов.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Dst"></param>
        public void SF6(bool[] BaseNum, ref bool[] Dst)
        {
            Int64 x = 0;
            //Конвертируем базовое число в виде байт массива в нормальное число.
            ConvertBaseNum(BaseNum, ref x);

            Int64 y;
            y = 31979673410 - ((x * 7 + 57158) / 80);

            //Обратное преобразование
            ConvertI64ToBool(y, ref Dst);
        }


       



        /// <summary>
        /// Функция для получения восьмого блока секретных битов.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Dst"></param>
        public void SF7(bool[] BaseNum, ref bool[] Dst)
        {
            Int64 x = 0;
            //Конвертируем базовое число в виде байт массива в нормальное число.
            ConvertBaseNum(BaseNum, ref x);

            Int64 y, t;

            y = (x * x * 5) + (x * 3) - 15324;
            t = y >> 2;
            y = y | t;

            //Обратное преобразование
            ConvertI64ToBool(y, ref Dst);
        }

       


        /// <summary>
        /// Функция для получения девятого блока секретных битов.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Dst"></param>
        public void SF8(bool[] BaseNum, ref bool[] Dst)
        {
            Int64 x = 0;
            //Конвертируем базовое число в виде байт массива в нормальное число.
            ConvertBaseNum(BaseNum, ref x);

            Int64 y, t;

            y = (x * x * 3) + x * 2 - 215324;
            t = (y >> 4) + (x * x);
            y = y & t;

            //Обратное преобразование
            ConvertI64ToBool(y, ref Dst);
        }


        /// <summary>
        /// Формирует кодовые блоки для даты.
        /// </summary>
        public void SCdate(DateTime date, ref bool[] bitDays_)
        {
            //Количество дней от 1 января 2019года.
            int days = (date - new DateTime(2019, 1, 1)).Days;

            //Переводим дату в двоичную систему что бы запутать отладчик.
            //BitArray bitDays = new BitArray(14, false);
            bool[] bitDays = new bool[14];

            for (int i = 13; i >= 0; --i)
            {
                int val = (int)Math.Pow(2, i);
                bitDays[i] = false;

                if (days >= val)
                {
                    bitDays[i] = true;
                    days -= val;
                }
            }

            bitDays_ = bitDays;
        }

        /// <summary>
        /// Функция для усложнения контрольной суммы.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        public void SfCRC(ref byte[] Xi)
        {
            byte[] k = new byte[11];

            Int32 a, b, c, d, t;
            int pos = 0;
            //Формирую некоторый секретный ряд, которой не просто придумать. Ряд был протестирован, "отбалды" использовать нельзя!
            for (int i = 5; i < 60; i = i + 5)
            {
                a = -1;
                b = a * a * i;

                c = 2 * i - 1;
                d = (c - b) * (3 * i);
                t = d & 0xff;//Обрезаю байт.
                k[pos] = (byte)t;
                pos++;
            }

            uint x1 = 0;
            uint x2 = 0;
            uint y = 0;
            //Замешиваю байты.
            for (int i=0;i<11;i++)
            {
                x1 = (uint)Xi[i];
                x2= (uint)k[i];
                y = x1 ^ x2;
                Xi[i]=(byte)y;
            }

        }

    }
}
