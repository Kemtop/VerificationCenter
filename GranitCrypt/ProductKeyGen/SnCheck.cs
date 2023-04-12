using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

namespace KeyGen
{
    /// <summary>
    /// Класс для проверки серийного номера.
    /// </summary>
    class SnCheck
    {
        /// <summary>
        ///Дата окончания действия серийного номера.
        /// </summary>
         DateTime dateEnd;

        /// <summary>
        /// Возвращает дату окончания действия серийного номера.
        /// </summary>
        /// <returns></returns>
        public DateTime GetDateEnd(string Serial)
        {


            return dateEnd;
        }


        /// <summary>
        /// Выполняет проверку серийного номера. Если серийный номер валидный возвращает 0, иначе >0.
        /// </summary>
        /// <returns></returns>
       public int Check(string Serial,DateTime CurDt,ref DateTime dtEnd)
        {
            //Проверка контрольной суммы серийного номера.
            if (!CheckKeySum(Serial))
                return 1;

            //Непосредственная проверка самого серийного номера
            if(!CheckSerial(Serial))
                return 2;

            //Получаю дату окончания действия серийного номера.          
            dtEnd=GetDateEnd(Serial); //Дата окончания действия серийного номера.

            //Текущая дата больше даты действия серийного номера.
            if(CurDt>dtEnd) return 3;

            return 0;
        }




        /// <summary>
        /// Выполняет парсинг серийного номера.
        /// </summary>
        /// <param name="Serial"></param>
        /// <returns></returns>
        bool GetSnData(string Serial, ref string BaseNum, ref string ControlBits)
        {
            string[] strList = Serial.Split('-');
            if (strList.Length < 5)
            {
                return false;
                //return "Ошибка А49: Не верный серийный номер.";
            }
            BaseNum = strList[0]; //Передаю базовый номер.
            ControlBits = strList[1] + strList[2] + strList[3];    //Передаю контрольный биты.

            return true;
        }


        /// <summary>
        /// Проверяет серийный номер.
        /// </summary>
        /// <param name="Serial"></param>
        /// <returns></returns>
        public bool CheckSerial(string Serial)
        {
            string BaseNum = ""; //36 ричное представление базового числа.
            string ControlBits = "";  //36 ричное представление контрольных битов.

            //Выполняю парсинг серийного номера. Разделяю на базовое число и контрольные биты.
            if (!GetSnData(Serial, ref BaseNum, ref ControlBits))
                return false;

            //Конвертирование 36 ричного базового числа в байт массив.
            Scale36 Ser = new Scale36();
            Ser.FromString(BaseNum); 
            BitArray baBaseNum = Ser.ToBitArray(); //Базовый номер в бинарном представлении.

            Ser.FromString(ControlBits); //Преобразовываю контрольные биты из 36 ричной системы в байт массив.
            BitArray baControlBits = Ser.ToBitArray(); //Базовый номер в бинарном представлении.
            
            bool[] biteArr = new bool[80]; //Преобразовываю в простой массив.
            baControlBits.CopyTo(biteArr, 0); //Данные для анализа.

            bool[] aBaseNum = new bool[32]; //Преобразовываю BitArray  в массив.
            if (!ConvertBaseNum(baBaseNum, ref aBaseNum)) return false;
            
            //Блок нужен если будет внедрятся в приложение!
            DateTime now = DateTime.Now;
            int day = now.Day; //Текущий день недели.
            int t = day % 9;

            bool[] Res = new bool[9];

            //На основании базового числа,вычисляю значения функций, и сравниваю результаты.
            for(int i=0;i<9;i++)
            {
                Res[i] =ChkBit(aBaseNum, biteArr, i);
            }

            //Получаю дату окончания действия серийного номера.
            dateEnd= GetDateEnd(biteArr);

            /*
            Res[0] = ChkBit0(aBaseNum, biteArr);//CheckBit0(baBaseNum, biteArr);
            Res[1] = ChkBit1(aBaseNum, biteArr);//CheckBit1(baBaseNum, biteArr);
            Res[2] = ChkBit2(aBaseNum, biteArr);//CheckBit2(baBaseNum, biteArr);
            Res[3] = ChkBit3(aBaseNum, biteArr);//CheckBit3(baBaseNum, biteArr);
            Res[4] = ChkBit4(aBaseNum, biteArr);//CheckBit4(baBaseNum, biteArr);
            Res[5] = ChkBit5(aBaseNum, biteArr);//CheckBit5(baBaseNum, biteArr);
            Res[6] = ChkBit6(aBaseNum, biteArr);//CheckBit6(baBaseNum, biteArr);
            Res[7] = ChkBit7(aBaseNum, biteArr);//CheckBit7(baBaseNum, biteArr);
            Res[8] = ChkBit8(aBaseNum, biteArr);//CheckBit8(baBaseNum, biteArr);
            */

            //Проверка правильности вычисления блоков.
            for (int i=0;i<9;i++)
            {
                if (!Res[i]) return false;
            }
                        

            return true;
        }


        /// <summary>
        /// Конвертирует базовый номер из BitArray в массив bool, считает количество единиц и проверяет.
        /// </summary>
        /// <param name="BaseNum"></param>
        /// <param name="Arr"></param>
        bool ConvertBaseNum(BitArray BaseNum,ref bool[] Arr)
        {
            int Cnt = 0;
            //Копирую 
            for (int i = 0; i < 26; i++)
            {
                Arr[i] = BaseNum[i];
                if (Arr[i] == true) Cnt++;
            }

            //Если базовое число содержит более 26 единиц.
            if (Cnt > 26) return false;

            return true;
        }


        /// <summary>
        /// Проверяет правильность n блока контрольных бит.
        /// </summary>
        /// <param name="X">базовое число в виде бит массива</param>
        /// <param name="ControlBits">контрольные биты</param>
        /// <returns></returns>
        bool ChkBit(bool[] X, bool[] ControlBits,int n)
        {
            SecureFunc Sf = new SecureFunc();
            Biektion Be = new Biektion(); //Перестановки в рядах.

            bool[] Y = new bool[32]; //Результат выполнения функции.

            bool res=false; //Результат проверки.

            switch (n)
            {
                case 0:
                    Sf.SF0(X, ref Y); //На основании базового числа получаю первую функцию.
                    res = Be.ChekBiektiv0(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.
                    break;

                case 1:
                    Sf.SF1(X, ref Y); //На основании базового числа получаю первую функцию.
                    res = Be.ChekBiektiv1(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.
                    break;

                case 2:
                    Sf.SF2(X, ref Y); //На основании базового числа получаю первую функцию.
                    res = Be.ChekBiektiv2(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.
                    break;

                case 3:
                    Sf.SF3(X, ref Y); //На основании базового числа получаю первую функцию.
                    res = Be.ChekBiektiv3(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.
                    break;

                case 4:
                    Sf.SF4(X, ref Y); //На основании базового числа получаю первую функцию.
                    res = Be.ChekBiektiv4(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.
                    break;

                case 5:
                    Sf.SF5(X, ref Y); //На основании базового числа получаю первую функцию.
                    res = Be.ChekBiektiv5(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.
                    break;

                case 6:
                    Sf.SF6(X, ref Y); //На основании базового числа получаю первую функцию.
                    res = Be.ChekBiektiv6(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.
                    break;

                case 7:
                    Sf.SF7(X, ref Y); //На основании базового числа получаю первую функцию.
                    res = Be.ChekBiektiv7(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.
                    break;

                case 8:
                    Sf.SF8(X, ref Y); //На основании базового числа получаю первую функцию.
                    res = Be.ChekBiektiv8(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.
                    break;
            }
            
            return res;

        }

        /// <summary>
        /// Возвращает дату окончания действия серийного номера.
        /// </summary>
        /// <param name="bits"></param>
        /// <returns></returns>
        DateTime GetDateEnd(bool [] bits)
        {
            Biektion Bie = new Biektion();
            bool[] dt = new bool[14];
            Bie.UnBiektivDateBlock(ref dt, bits);

            int countDay = 0;
            int tmp = 0;
            int tmp1 = 0;

            for (int i = 0; i < 14; ++i)
            {
                tmp = 1;
                tmp1 = tmp << i;
               if (dt[i]) countDay += tmp1;
            }

            DateTime date = new DateTime(2019, 1, 1);
            date = date.AddDays(countDay);

            return date;
        }

       

        /// <summary>
        /// Проверяет правильность нулевого блока контрольных бит.
        /// </summary>
        /// <param name="X">базовое число в виде бит массива</param>
        /// <param name="ControlBits">контрольные биты</param>
        /// <returns></returns>
        bool ChkBit0(bool[] X, bool[] ControlBits)
        {
            SecureFunc Sf = new SecureFunc();
            bool[] Y = new bool[32]; //Результат выполнения функции.

            Sf.SF0(X,ref Y); //На основании базового числа получаю первую функцию.

            Biektion Be = new Biektion(); //Перестановки в рядах.
            bool res = Be.ChekBiektiv0(Y,ref ControlBits); //Делает обратные перестановки и сравнивает ряды.

            return res;
        }

        /// <summary>
        /// Проверяет правильность первого блока контрольных бит.
        /// </summary>
        /// <param name="X">базовое число в виде бит массива</param>
        /// <param name="ControlBits">контрольные биты</param>
        /// <returns></returns>
        bool ChkBit1(bool[] X, bool[] ControlBits)
        {
            SecureFunc Sf = new SecureFunc();
            bool[] Y = new bool[32]; //Результат выполнения функции.

            Sf.SF1(X, ref Y); //На основании базового числа получаю первую функцию.

            Biektion Be = new Biektion(); //Перестановки в рядах.
            bool res = Be.ChekBiektiv1(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.

            return res;
        }

        /// <summary>
        /// Проверяет правильность второго блока контрольных бит.
        /// </summary>
        /// <param name="X">базовое число в виде бит массива</param>
        /// <param name="ControlBits">контрольные биты</param>
        /// <returns></returns>
        bool ChkBit2(bool[] X, bool[] ControlBits)
        {
            SecureFunc Sf = new SecureFunc();
            bool[] Y = new bool[32]; //Результат выполнения функции.

            Sf.SF2(X, ref Y); //На основании базового числа получаю первую функцию.

            Biektion Be = new Biektion(); //Перестановки в рядах.
            bool res = Be.ChekBiektiv2(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.

            return res;
        }

        /// <summary>
        /// Проверяет правильность третьего блока контрольных бит.
        /// </summary>
        /// <param name="X">базовое число в виде бит массива</param>
        /// <param name="ControlBits">контрольные биты</param>
        /// <returns></returns>
        bool ChkBit3(bool[] X, bool[] ControlBits)
        {
            SecureFunc Sf = new SecureFunc();
            bool[] Y = new bool[32]; //Результат выполнения функции.

            Sf.SF3(X, ref Y); //На основании базового числа получаю значение функции.

            Biektion Be = new Biektion(); //Перестановки в рядах.
            bool res = Be.ChekBiektiv3(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.

            return res;
        }

        /// <summary>
        /// Проверяет правильность четвертого блока контрольных бит.
        /// </summary>
        /// <param name="X">базовое число в виде бит массива</param>
        /// <param name="ControlBits">контрольные биты</param>
        /// <returns></returns>
        bool ChkBit4(bool[] X, bool[] ControlBits)
        {
            SecureFunc Sf = new SecureFunc();
            bool[] Y = new bool[32]; //Результат выполнения функции.

            Sf.SF4(X, ref Y); //На основании базового числа получаю значение функции.

            Biektion Be = new Biektion(); //Перестановки в рядах.
            bool res = Be.ChekBiektiv4(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.

            return res;
        }

        /// <summary>
        /// Проверяет правильность пятого блока контрольных бит.
        /// </summary>
        /// <param name="X">базовое число в виде бит массива</param>
        /// <param name="ControlBits">контрольные биты</param>
        /// <returns></returns>
        bool ChkBit5(bool[] X, bool[] ControlBits)
        {
            SecureFunc Sf = new SecureFunc();
            bool[] Y = new bool[32]; //Результат выполнения функции.

            Sf.SF5(X, ref Y); //На основании базового числа получаю значение функции.

            Biektion Be = new Biektion(); //Перестановки в рядах.
            bool res = Be.ChekBiektiv5(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.

            return res;
        }

        /// <summary>
        /// Проверяет правильность шестого блока контрольных бит.
        /// </summary>
        /// <param name="X">базовое число в виде бит массива</param>
        /// <param name="ControlBits">контрольные биты</param>
        /// <returns></returns>
        bool ChkBit6(bool[] X, bool[] ControlBits)
        {
            SecureFunc Sf = new SecureFunc();
            bool[] Y = new bool[32]; //Результат выполнения функции.

            Sf.SF6(X, ref Y); //На основании базового числа получаю значение функции.

            Biektion Be = new Biektion(); //Перестановки в рядах.
            bool res = Be.ChekBiektiv6(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.

            return res;
        }


        /// <summary>
        /// Проверяет правильность седьмого блока контрольных бит.
        /// </summary>
        /// <param name="X">базовое число в виде бит массива</param>
        /// <param name="ControlBits">контрольные биты</param>
        /// <returns></returns>
        bool ChkBit7(bool[] X, bool[] ControlBits)
        {
            SecureFunc Sf = new SecureFunc();
            bool[] Y = new bool[32]; //Результат выполнения функции.

            Sf.SF7(X, ref Y); //На основании базового числа получаю значение функции.

            Biektion Be = new Biektion(); //Перестановки в рядах.
            bool res = Be.ChekBiektiv7(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.

            return res;
        }

        /// <summary>
        /// Проверяет правильность восьмого блока контрольных бит.
        /// </summary>
        /// <param name="X">базовое число в виде бит массива</param>
        /// <param name="ControlBits">контрольные биты</param>
        /// <returns></returns>
        bool ChkBit8(bool[] X, bool[] ControlBits)
        {
            SecureFunc Sf = new SecureFunc();
            bool[] Y = new bool[32]; //Результат выполнения функции.

            Sf.SF8(X, ref Y); //На основании базового числа получаю значение функции.

            Biektion Be = new Biektion(); //Перестановки в рядах.
            bool res = Be.ChekBiektiv8(Y, ref ControlBits); //Делает обратные перестановки и сравнивает ряды.

            return res;
        }


     
    

        /// <summary>
        /// Получаю данные для вычисления контрольной суммы серийного номера, а так же саму контрольную сумму.
        /// </summary>
        /// <param name="Serial"></param>
        /// <returns></returns>
        string GetSnBlock(string Serial, ref string ControlSum)
        {
            string[] strList = Serial.Split('-');
            if (strList.Length < 5) return "";

            string str = "a51" + strList[0] + "k" + strList[1] + strList[2] + strList[3] + "p";
            ControlSum = strList[4];

            return str;
        }



        /// <summary>
        /// Проверяет контрольную сумму серийного номера.
        /// </summary>
        /// <param name="Serial"></param>
        /// <returns></returns>
        public bool CheckKeySum(string Serial)
        {
            //Получаю строку для вычисления контрольной суммы и саму контрольную сумму.
            string SnCrc = "";
            string str = GetSnBlock(Serial, ref SnCrc);
            
            //Преобразовываю строку в массив.
            byte[] bytes = Encoding.ASCII.GetBytes(str);

            SecureFunc sf = new SecureFunc();
            sf.SfCRC(ref bytes);

            //Вычисляю хэш  функцию.
            byte[] hashBytes;
            using (var sha1 = SHA1.Create())
            {
                hashBytes = sha1.ComputeHash(bytes);
            }

            //Преобразовываю массив в 16 ричную систему счисления.
            string Res = string.Concat(hashBytes.Select(n => n.ToString("x2"))).ToUpper();
            //Отрезаем последние 5 символов. 
            Res = Res.Substring(Res.Length - 5, 5);

            //Сравниваю контрольную сумму.
            if (Res != SnCrc) return false;

            return true;
        }

        /// <summary>
        /// Cудя по всему возвращает базовое число.
        /// </summary>
        /// <param name="Serial"></param>
        /// <returns></returns>
        string GetSerCode(string Serial)
        {
            string[] strList = Serial.Split('-');
            if (strList.Length < 5) return "";

            return strList[0];
        }


        string GetKeySum(string Serial)
        {
            string[] strList = Serial.Split('-');
            if (strList.Length < 5) return "";

            return strList[4];

        }

        string GetBits(string Serial)
        {
            string[] strList = Serial.Split('-');
            if (strList.Length < 5) return "";

            string str = strList[1] + strList[2] + strList[3];

            return str;
        }




    }
}
