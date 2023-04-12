using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.IO;

namespace DLL_API_Example
{
    /// <summary>
    /// Класс для работы с библиотекой.
    /// </summary>
    class GranitAPI
    {
        public const string LIBRARY_NAME = "LibGranitK.dll"; //Название библиотеки.


        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern int Tst();

        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]

        static extern int LoadSecretKey(IntPtr LastError, IntPtr PathToKey);

        
        //Получает информацию о владельце секретного ключа.  //Получает информацию о владельце секретного ключа.
        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern int SkInfo(IntPtr Familia, IntPtr Imia, IntPtr Otchestvo, IntPtr DateBegin, IntPtr DateEnd, IntPtr DaysLeft);

           
        //Проверяет правильность пароля к секретному ключу пользователя =0 ошибка
        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern int ChkPassword(IntPtr Password, IntPtr Error);


        //Проверяет валидность сертификата для данного ключа.
        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern int CheckCertForKey(IntPtr Error);

        //Открытие базы данных сертификатов.
        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern int OpenCertDb(IntPtr PathDrivers, IntPtr PathDb, IntPtr Error);


        /*
	   Выполняет подпись данных содержащихся в массиве Message длиной Mlen.  
	   Помещает отпечаток ключа подписанта в KeyStamp.
	   Помещает цифровую подпись(вектора r и s ) в SignStr,длину подпись в Slen.
	   В случае ошибки возвращает сообщение. Если подпись сформирована успешно-возвращает пустую строку.
	   Теоретически можно подписать массив размером 2Гб, при желании можно увеличить размер до пентобайт)
	*/
        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern int SignData(IntPtr Message, int Mlen, IntPtr SignStr, IntPtr Slen, IntPtr Error);

        //Проверка подписи
        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern int LibCheckSign(IntPtr Message, int Mlen,IntPtr SignStr, IntPtr FIOp, IntPtr Error);


        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern void ClearLib();

        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern void closeDb();



        public GranitAPI()
        {

        }

        public void Test()
        {
            Tst();
        }

        /// <summary>
        /// Загружает секретный ключ пользователя. В случае ошибки-возвращает сообщение.
        /// </summary>
        /// <returns></returns>
        public string LoadSecretKey(string PathSk)
        {

            IntPtr Error = Marshal.AllocHGlobal(16384); //Выделяет память для сообщения об ошибке.
            IntPtr PathToKey = Marshal.StringToHGlobalAnsi(PathSk); //Копирую строку в не управляемую память.

            int ret = LoadSecretKey(Error, PathToKey); //Вызываю из dll функцию LoadSecretKey

            // Переводим результат в управляемый вид
            string strErr = Marshal.PtrToStringAnsi(Error);

            // Обязательно освобождаем выделенную память
            Marshal.FreeHGlobal(Error);
            Marshal.FreeHGlobal(PathToKey);

            if (ret != 1) //Возникла ошибка функция вернула 0.
            {
                return strErr;
            }

            return "";
        }

               
     

        //Получает информацию о владельце секретного ключа.
        public bool GetSkInfo(ref string Familia, ref string Imia, ref string Otchestvo, ref string DateBegin, ref string DateEnd, ref string DaysLeft)
        {
            IntPtr _Familia = Marshal.AllocHGlobal(255);
            IntPtr _Imia = Marshal.AllocHGlobal(255);
            IntPtr _Otchestvo = Marshal.AllocHGlobal(255);
            IntPtr _DateBegin = Marshal.AllocHGlobal(32);
            IntPtr _DateEnd = Marshal.AllocHGlobal(32);
            IntPtr _DaysLeft = Marshal.AllocHGlobal(32);

            int ret = SkInfo(_Familia, _Imia, _Otchestvo, _DateBegin, _DateEnd, _DaysLeft); //Вызываю из dll функцию SkInfo.
                                                                                            // Переводим результат в управляемый вид

            Familia = Marshal.PtrToStringAnsi(_Familia);
            Imia = Marshal.PtrToStringAnsi(_Imia);
            Otchestvo = Marshal.PtrToStringAnsi(_Otchestvo);
            DateBegin = Marshal.PtrToStringAnsi(_DateBegin);
            DateEnd = Marshal.PtrToStringAnsi(_DateEnd);
            DaysLeft = Marshal.PtrToStringAnsi(_DaysLeft);

            // Обязательно освобождаем выделенную память
            Marshal.FreeHGlobal(_Familia);
            Marshal.FreeHGlobal(_Imia);
            Marshal.FreeHGlobal(_Otchestvo);
            Marshal.FreeHGlobal(_DateBegin);
            Marshal.FreeHGlobal(_DateEnd);
            Marshal.FreeHGlobal(_DaysLeft);

            if (ret == 0) //Возникла ошибка.
            {
                return false;
            }

            return true;
        }


        public bool ChkPassword(string Password, ref string Error)
        {
            // IntPtr _Password = Marshal.AllocHGlobal(64);
            IntPtr _Error = Marshal.AllocHGlobal(255);

            IntPtr _Password = Marshal.StringToHGlobalAnsi(Password);

            int ret = ChkPassword(_Password, _Error);

            string Error_ = Marshal.PtrToStringAnsi(_Error);

            //Добавить затирание пароля
            Marshal.FreeHGlobal(_Password);
            Marshal.FreeHGlobal(_Error);

            if (ret == 0) //Возникла ошибка.
            {
                Error = Error_;
                return false;
            }

            return true;
        }



        /// <summary>
        /// Проверяет соответствие закрытого ключа и сертификата по указанному пути.
        /// </summary>
        /// <param name="Error"></param>
        /// <param name="PathToCert"></param>
        /// <returns></returns>
        public bool uCheckKeyCert(ref string Error)
        {
            IntPtr _Error = Marshal.AllocHGlobal(1024);

            int ret = CheckCertForKey(_Error);
            string Error_ = Marshal.PtrToStringAnsi(_Error);

            Marshal.FreeHGlobal(_Error);

            if (ret == 0) //Возникла ошибка.
            {
                Error = Error_;
                return false;
            }

            return true;
        }


        /// <summary>
        ///Открытие базы данных сертификатов.
        /// </summary>
        /// <param name="PathDb"></param>
        /// <param name="Error"></param>
        /// <returns></returns>
        public bool OpenCertDb(string PathDrivers,string PathDb, ref string Error)
        {
            IntPtr _Error = Marshal.AllocHGlobal(255);
            IntPtr _PathDb = Marshal.StringToHGlobalAnsi(PathDb);
            IntPtr _PathDrivers = Marshal.StringToHGlobalAnsi(PathDrivers);

            int ret = OpenCertDb(_PathDrivers,_PathDb, _Error);
            string Error_ = Marshal.PtrToStringAnsi(_Error);

            Marshal.FreeHGlobal(_PathDb);
            Marshal.FreeHGlobal(_Error);
            Marshal.FreeHGlobal(_PathDrivers);
            if (ret == 0) //Возникла ошибка.
            {
                Error = Error_;
                return false;
            }

            return true;
        }


        /*
          Выполняет подпись данных содержащихся в массиве Message длиной Mlen.
           Помещает отпечаток ключа подписанта в KeyStamp.
           Помещает цифровую подпись(вектора r и s ) в SignStr,длину подпись в Slen.
           В случае ошибки возвращает сообщение. Если подпись сформирована успешно-возвращает пустую строку.
            Теоретически можно подписать массив размером 2Гб, при желании можно увеличить размер до пентобайт)
         */
        public bool SignData(byte[] Message, ref string SignStr, ref string Error)
        {
            bool HasError = false; //Ошибок нет
            string Error_ = "";//Сообщение об ошибке.

            int Msize = Message.Length; //Размер сообщения.
            IntPtr Mpnt = Marshal.AllocHGlobal(Msize);
                
            IntPtr _SignStr = Marshal.AllocHGlobal(8048); //Подпись.
            IntPtr _Slen = Marshal.AllocHGlobal(4);//Длина подписи.
            IntPtr _Error = Marshal.AllocHGlobal(1024); //Сообщение об ошибке.

            try
            {
                //Копирует массив в неуправляемую память. 
                Marshal.Copy(Message, 0, Mpnt, Message.Length);

                int ret = SignData(Mpnt, Msize, _SignStr, _Slen, _Error);

                //Сохранение результатов.
                Error_ = Marshal.PtrToStringAnsi(_Error);
                string SignStr_ = Marshal.PtrToStringAnsi(_SignStr);
                int Slen_ = Marshal.ReadInt32(_Slen);

                if (ret == 0) //Возникла ошибка.
                {
                    HasError = true;
                }
                else
                {
                    SignStr = SignStr_;
                }

            }
            catch (Exception er)
            {
                Error_ = "При подписи сообщения возникло исключение: " + er.Message;
                HasError = true;
            }
            finally
            {

                //Очистка не управляемой памяти.             
                Marshal.FreeHGlobal(Mpnt);
                Marshal.FreeHGlobal(_SignStr);
                Marshal.FreeHGlobal(_Slen);
                Marshal.FreeHGlobal(_Error);

            }

            //Возникла ошибка
            if (HasError)
            {
                Error = Error_;
                return false; //Возникло исключение.
            }

            return true;
        }

        public bool CheckSign(ref string Error, byte[] Message,string Sign, ref string FIOp)
        {
            string Error_ = "";
            bool HasError = false;

            IntPtr _SignStr = Marshal.StringToHGlobalAnsi(Sign); //ЭЦП.
            IntPtr _Error = Marshal.AllocHGlobal(1024); //Сообщение об ошибке.
            IntPtr _FIOp = Marshal.AllocHGlobal(1024); //ФИО подписанта.

            int Msize = Message.Length; //Размер сообщения.
            IntPtr Mpnt = Marshal.AllocHGlobal(Msize); //Выделяю память для сообщения.


            try
            {
                //Копирует массив в не управляемую память. 
                Marshal.Copy(Message, 0, Mpnt, Msize);

                int ret = LibCheckSign(Mpnt, Msize, _SignStr, _FIOp, _Error);
                //Сохранение результатов.
                Error_ = Marshal.PtrToStringAnsi(_Error);
                FIOp = Marshal.PtrToStringAnsi(_FIOp);


                if (ret == 0) //Возникла ошибка.
                    HasError = true;

            }
            catch (Exception er)
            {
                HasError = true;
                Error_ = er.Message;
            }
            finally
            {
                Marshal.FreeHGlobal(_SignStr);
                Marshal.FreeHGlobal(_Error);
                Marshal.FreeHGlobal(_FIOp);
                Marshal.FreeHGlobal(Mpnt);
            }

            //Возникла ошибка
            if (HasError)
            {
                Error = Error_;
                return false;
            }

            return true;

        }

        /// <summary>
        ///Чистит конфиденциальные данные.
        /// </summary>
        public void ClearLibr()
        {
            ClearLib();
        }

        /// <summary>
        /// Закрываю соединение с БД.
        /// </summary>
        public void CloseDB()
        {
           closeDb();
        }

    }
}
