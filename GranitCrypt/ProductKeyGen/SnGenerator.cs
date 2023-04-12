using KeyGen.DAO;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Windows.Forms;

namespace KeyGen
{
    class SnGenerator
    {

        /// <summary>
        /// Количество контрольных бит в серийном номере.
        /// </summary>
        const int ColBit = 9;

        // private string Bits = "";
        private string KeySum = "00000";

        /// <summary>
        /// Флаг работы класса в режиме само тестирования.
        /// </summary>
        private bool selfTestMode = false;
        /// <summary>
        /// Делегат для передачи базового числа.
        /// </summary>
        dg_setBaseNumValue pDelegate; 
        /// <summary>
        /// Генератор базового числа для тестов.
        /// </summary>
        BaseDigCounter Bdc = null;

        IDb iDb; //Интрефейс для работы с БД.

        //Объект для генерации случайного числа.
        private RNGCryptoServiceProvider rngCsp = new RNGCryptoServiceProvider();


        public SnGenerator(IDb i)
        {
            iDb = i;
            selfTestMode = false;
        }


        /// <summary>
        /// Взводит флаг режима тестирования.
        /// </summary>
        public void enableSelfTest()
        {
            selfTestMode = true;
        }

        /// <summary>
        /// Передает делегат для тестов.
        /// </summary>
        /// <param name="p"></param>
        public void setParamDelegate(dg_setBaseNumValue p)
        {
            pDelegate = p;
        }

        /// <summary>
        /// Передает объект для генерации базового числа.
        /// </summary>
        /// <param name="p"></param>
        public void setBaseDigCounter(BaseDigCounter p)
        {
            Bdc = p;
        }


        /// <summary>
        /// Генерирует число являющееся  основанием серийного номера. В случае ошибки возвращает false;
        /// </summary>
        /// <returns></returns>
        private bool GenBasicDigit(ref UInt32 Dig)
        {
            UInt32 baseNum = 0; //Базовое число.
            byte[] randomNumber = new byte[4];

            bool IsNoGen = true; //Базовое число еще не сгенерировано.
            bool HasErr = false; //Флаг возникновения ошибок в методах.
            bool findStts = false;//Флаг нахождения базового номера в БД.



            //Включен режим само тестирования.
            if (selfTestMode)
            {
                baseNum = Bdc.getBaseNum(); //Получаю очередное базовое число.

                Dig = baseNum;//Передаю базовое число.  
                return true;
            }



            //Генерирую случайное число, требуемой размерности,которого нет в БД ключей. 
            //Не превышающее 36 в степени 5 (60466175). Иначе получим более 5 символов.
            while (IsNoGen)
            {
                rngCsp.GetBytes(randomNumber); //Получаю очередной случайный блок.
                //Конвертация массива.
                if (BitConverter.IsLittleEndian)
                    Array.Reverse(randomNumber);
                baseNum = BitConverter.ToUInt32(randomNumber, 0);

                if (baseNum > 60466175) continue; //Случайное число превысило 36 в степени 5.

                //Проверяю нет ли такого  числа в БД.
                HasErr = iDb.findBasicDigit(baseNum, ref findStts);
                if (!HasErr) return false;

                if (findStts) continue; //Число уже существует в БД.

                IsNoGen = false; //Базовое число было сгенерированно.
            }

            Dig = baseNum;//Передаю базовое число.                     

            return true;
        }


        /// <summary>
        /// Конвертирует базовое число в 36 ричную систему счисления. Проверяет строку.
        /// </summary>
        /// <param name="Dig"></param>
        /// <returns></returns>
        bool BaseNumToStr(UInt32 Dig, ref string Str)
        {
            /*
            //Преобразовываю в 36 ричную систему счисления.
            //BitArray Rbits = new BitArray(randomNumber);
            //Array.Reverse(randomNumber); //Конвертер не принимает LittleEndian
            */
            Scale36 Conv = new Scale36();
            Conv.FromUInt32(Dig);

            string str = Conv.ToString();

            //Проверка-в строке должно быть только 5 символов базового числа,иначе получаем не корректное число.
            for (int i = 0; i < str.Length - 5; i++)
            {
                if (str[i] != '0')
                {
                    //Число содержит более 27бит.
                    ShowError("Базовое число содержит более 5-ти строковых символа! " + str);
                    return false;
                }
            }

            //Убираю лишние символы.
            Str = str.Substring(str.Length - 5);

            return true;
        }




        /// <summary>
        /// Вычисляет контрольный бит.
        /// </summary>
        /// <param name="Serial"></param>
        /// <param name="Bits"></param>
        /// <param name="Bit"></param>
        /// <returns></returns>
        private BitArray HasihBit(string Serial, BitArray Bits, int Bit)
        {
            //Класс который из 36 ричной системе преобразовывает в двоичную.
            Scale36 ser = new Scale36();
            ser.FromString(Serial); //Преобразовываем серийный номер в двоичный вид.
            BitArray serBits = ser.ToBitArray();

            serBits.Length = 26; //Расширяем значение.
            bool[] arr = new bool[26];
            bool[] arrBlock = new bool[13];
            serBits.CopyTo(arr, 0); //Копирую в буфер.

            //Разделяю на два блока.
            Array.Copy(arr, 0, arrBlock, 0, 13);
            BitArray block1 = new BitArray(arrBlock);

            Array.Copy(arr, 13, arrBlock, 0, 13);
            BitArray block2 = new BitArray(arrBlock);

            //Каждая функция генерирует блок длиной в 7 бит на основании серийного номера.




            switch (Bit)
            {
                case 0:
                    block1 = block1.And(block2);
                    Bits[76] = block1[9]; //1 3 4 6 8 9 10
                    Bits[24] = block1[3];
                    Bits[75] = block1[10];
                    Bits[0] = block1[6];
                    Bits[28] = block1[1];
                    Bits[58] = block1[8];
                    Bits[38] = block1[4];
                    break;
                case 1:
                    block1 = block1.Or(block2);
                    Bits[44] = block1[5]; //0 1 5 6 7 10 11
                    Bits[66] = block1[11];
                    Bits[11] = block1[1];
                    Bits[6] = block1[6];
                    Bits[33] = block1[10];
                    Bits[60] = block1[7];
                    Bits[34] = block1[0];
                    break;
                case 2:
                    block1 = block1.And(block2);
                    Bits[52] = block1[6];
                    Bits[10] = block1[2];
                    Bits[67] = block1[8];
                    Bits[1] = block1[11];
                    Bits[73] = block1[3];
                    Bits[29] = block1[0];
                    Bits[2] = block1[1];
                    break;
                case 3:
                    block1 = block1.And(block2);
                    Bits[70] = block1[8];
                    Bits[54] = block1[3];
                    Bits[50] = block1[4];
                    Bits[17] = block1[5];
                    Bits[41] = block1[9];
                    Bits[62] = block1[10];
                    Bits[39] = block1[11];
                    break;
                case 4:
                    block1 = block1.And(block2);
                    Bits[22] = block1[7];
                    Bits[4] = block1[5];
                    Bits[53] = block1[11];
                    Bits[26] = block1[1];
                    Bits[37] = block1[8];
                    Bits[31] = block1[3];
                    Bits[71] = block1[9];
                    break;
                case 5:
                    block1 = block1.And(block2);
                    Bits[16] = block1[1];
                    Bits[40] = block1[6];
                    Bits[56] = block1[4];
                    Bits[5] = block1[5];
                    Bits[65] = block1[0];
                    Bits[19] = block1[7];
                    Bits[49] = block1[11];
                    break;
                case 6:
                    block1 = block1.And(block2);
                    Bits[68] = block1[10];
                    Bits[25] = block1[6];
                    Bits[57] = block1[2];
                    Bits[48] = block1[3];
                    Bits[7] = block1[0];
                    Bits[51] = block1[1];
                    Bits[42] = block1[4];
                    break;
                case 7:
                    block1 = block1.And(block2);
                    Bits[32] = block1[11];
                    Bits[55] = block1[5];
                    Bits[13] = block1[1];
                    Bits[14] = block1[7];
                    Bits[72] = block1[0];
                    Bits[8] = block1[3];
                    Bits[74] = block1[8];
                    break;
                case 8:
                    block1 = block1.And(block2);
                    Bits[3] = block1[5];
                    Bits[18] = block1[10];
                    Bits[46] = block1[1];
                    Bits[23] = block1[0];
                    Bits[15] = block1[6];
                    Bits[64] = block1[7];
                    Bits[12] = block1[9];
                    break;
            }
            return Bits;
        }


        /// <summary>
        /// На основание базового номера создает 9 блоков проверочных бит.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Bits"></param>
        bool HashBit(UInt32 baseNum, ref bool[] Bits)
        {
            bool[] BaseNum = new bool[32];
            //Базовое число не может превышать значения 36 в степени 5 т.е. 60 466 176 или 03 9A A3 FF
            //т.е. 4х байт или 26 бит.

            Biektion B = new Biektion();
            SecureFunc Sf = new SecureFunc();

            if (!Sf.ConvertBaseNum(baseNum, ref BaseNum))
            {
                ShowError("Базовое число содержит более 26 бит.");
                return false;
            }

            bool[] ResBlock = new bool[32];//Значение аргумента функции.

            Sf.SF0(BaseNum, ref ResBlock); //Используя базовый номер в качестве аргумента вычисляем значение функции. y=f0(x);
            B.Biektiv0(ref Bits, ResBlock);//Выполняю отображение во множестве контрольных бит.

            Sf.SF1(BaseNum, ref ResBlock);//Используя базовый номер в качестве аргумента вычисляем значение функции. y=f1(x);
            B.Biektiv1(ref Bits, ResBlock);//Выполняю отображение во множестве контрольных бит.

            Sf.SF2(BaseNum, ref ResBlock);
            B.Biektiv2(ref Bits, ResBlock);

            Sf.SF3(BaseNum, ref ResBlock);
            B.Biektiv3(ref Bits, ResBlock);

            Sf.SF4(BaseNum, ref ResBlock);
            B.Biektiv4(ref Bits, ResBlock);

            Sf.SF5(BaseNum, ref ResBlock);
            B.Biektiv5(ref Bits, ResBlock);

            Sf.SF6(BaseNum, ref ResBlock);
            B.Biektiv6(ref Bits, ResBlock);

            Sf.SF7(BaseNum, ref ResBlock);
            B.Biektiv7(ref Bits, ResBlock);

            Sf.SF8(BaseNum, ref ResBlock);
            B.Biektiv8(ref Bits, ResBlock);

            return true;
        }




        /// <summary>
        /// На основании базового номера формирует массив контрольных бит и переводит их в 36ричную систему счисления.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="date"></param>
        /// <param name="Str"></param>
        /// <returns></returns>
        private bool GenBits(UInt32 BaseNum, DateTime date, ref string Str)
        {
            bool[] CrBits = new bool[77]; //Для преобразования данных.

            //Формирую контрольные блоки серийного номера.
            if (!HashBit(BaseNum, ref CrBits)) return false;

            //Добавляем дату окончания срока действия ключа.
            SecureFunc Sf = new SecureFunc();
            Biektion Bie = new Biektion();

            bool[] bitDays = new bool[14];
            Sf.SCdate(date, ref bitDays);
            Bie.BiektivDateBlock(ref CrBits, bitDays);

            BitArray Bits = new BitArray(77, false); //Массив из 77бит,по умолчанию заполняется 0.
            for (int i = 0; i < 77; i++) Bits[i] = CrBits[i]; //Копирую.

            //Преобразовывает массив из 77 в 36(15 символов длина блока ключа)ричную систему счисления.
            Scale36 chBits = new Scale36();
            chBits.FromBitArray(Bits);

            Str = chBits.ToString();

            return true;
        }



        /// <summary>
        /// Генерирует контрольную сумму на основании базового числа и контрольных бит.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Bits"></param>
        /// <returns></returns>
        private string GenKeySum(string BaseNum, string Bits)
        {
            SHA1Managed sha1 = new SHA1Managed();

            string str = "a51" + BaseNum + "k" + Bits + "p";
            byte[] x = Encoding.ASCII.GetBytes(str);

            SecureFunc sf = new SecureFunc();
            sf.SfCRC(ref x); //Модифицирую строку байт.
            string Res = string.Concat(sha1.ComputeHash(x).Select(n => n.ToString("x2"))).ToUpper();
            return Res.Substring(Res.Length - 5, 5);
        }

        /// <summary>
        /// Генерирую серийный номер.
        /// </summary>
        /// <param name="serial"></param>
        /// <returns></returns>
        public bool GenSerial(ref string serial, ref string StrbaseDig, ref UInt32 baseDig, DateTime DEnd)
        {
            // Базовое число  в 36системе счисления.
            string BaseNumStr = "00000";

            //Генерирую базовое число серийного номера.
            UInt32 BASEdig = 0;
            if (!GenBasicDigit(ref BASEdig)) return false;

            //Преобразовываю базовое число в 36систему счисления.
            if (!BaseNumToStr(BASEdig, ref BaseNumStr)) return false;
            baseDig = BASEdig;

            StrbaseDig = BaseNumStr;//Передаю базовое число.

            string Bits = "";
            //Получаем массив контрольных бит из базового числа и даты.
            if (!GenBits(BASEdig, DEnd, ref Bits)) return false;
            KeySum = GenKeySum(BaseNumStr, Bits);  //Считаем контрольную сумму.

            //Собираем строку.
            serial = BaseNumStr + "-" + Bits.Substring(0, 5) + "-" + Bits.Substring(5, 5) + "-" + Bits.Substring(10, 5) + "-" + KeySum;

            return true;
        }

        /// <summary>
        /// Выводит сообщение об ошибке.
        /// </summary>
        void ShowError(string mess)
        {
            MessageBox.Show(mess, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }



        /// <summary>
        /// Запуск теста проверки генератора ключей продукта.
        /// </summary>
        public void BeginTest()
        {
            SelfTest(iDb, pDelegate);
        }


        /// <summary>
        /// Возвращает максимально возможное базовое число. 36 в степени 5.
        /// </summary>
        /// <returns></returns>
        public static int getMaxBaseNum()
        {
            return 60466176;
        }

        /// <summary>
        /// Запуск теста проверки генератора ключей продукта.
        /// </summary>
        void SelfTest(IDb iDb,dg_setBaseNumValue dg)
        {
             //Дата окончания действия серийного номера.
            DateTime DEnd = DateTime.Now.AddYears(1);//Прибавляю год.  
            DateTime tmpDt = new DateTime();
            DateTime NowDt = DateTime.Now.Date;


            UInt32 baseDig = 0; //Базовое число.
            string StrBaseDig = "";//Базовое число в 36 ричной системе.
            string Serial = ""; //Серийный номер. 
            int res = 0; //Результат теста.       
            long id = 0; //Ид вставленной строки.   

            BaseDigCounter bdc = new BaseDigCounter();//Объект для генерации базового числа при тестировании.

            //Перебираю всю вероятность. 36 в степени 5. 5 36 ричных чисел в базовом числе
            //60466176
            for (long i = 0; i < getMaxBaseNum()+1; i++)
            {
               
                SnGenerator SnG = new SnGenerator(iDb);//Объект для генерации серийных номеров.
                SnG.enableSelfTest(); //Включаю режим тестирования.
                SnG.setBaseDigCounter(bdc); //Передаю объект для генерации тестового базового числа.
                                      //Генерирую серийный номер.
                if (!SnG.GenSerial(ref Serial, ref StrBaseDig, ref baseDig, DEnd))
                {
                    dg(-10);
                    return;
                }
                 

                //Такого быть не может. Но проверю на всякий случай. Поиск серийного номера в БД.
                bool findSerialStts = false;
                if (!iDb.findSerial(Serial, ref findSerialStts))
                {
                    dg(-1);
                    return;
                }
                    
                if (findSerialStts)
                {
                    ShowError("Обнаружен повторяющийся серийный номер. " + Serial);
                    dg(-2);
                    return;
                }

                //Проверка серийного номера.
                SnCheck Sch = new SnCheck();
                res = Sch.Check(Serial, NowDt, ref tmpDt);

                if (res > 0)
                {
                    ShowError("Ошибка генерации серийного номера. Код " + res.ToString() + ", итерация " + i.ToString() + ".");
                    dg(-3);
                    return;
                }


                //Cохраняю данные в БД.
                DateTime now = DateTime.Now;
                string Note = "Test " + i.ToString();

                if (!iDb.SaveSerial(StrBaseDig, baseDig, Serial, Note, now, DEnd, ref id))
                {
                    dg(-4);
                    return;
                }
                
                dg((int)i); //Отображаю процесс.
                                
            }
            
            dg(0); //Говорю что тест завершен.
            return;
        }


    }
}
