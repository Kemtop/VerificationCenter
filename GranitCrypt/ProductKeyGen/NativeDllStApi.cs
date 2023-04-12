using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace KeyGen
{
    class NativeDllStApi
    {
        public const string LIBRARY_NAME = "DllProductKeySelfTest.dll"; //Название библиотеки.


        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern int Tst();

        [DllImport(LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        static extern int CheckKey(IntPtr ProductKey, IntPtr dateEnd);
        

        /// <summary>
        /// Проверяет ключ продукта. В случает критической ошибки возвращает -1. Если ключ не верный =0, если верный =1;
        /// </summary>
        /// <param name="ProductKey"></param>
        /// <param name="dateEnd"></param>
        /// <returns></returns>
      public int ApiCheckKey(string ProductKey, ref string dateEnd)
      {
            IntPtr ptrProductKey = Marshal.StringToHGlobalAnsi(ProductKey); //Копирую строку в не управляемую память.
            IntPtr ptrDateEnd = Marshal.AllocHGlobal(32); //Выделяет память для строки об дате окончания действия ключа.

            int ret = 0;

            try
            {
                //Вызываю из dll функцию.
                ret = CheckKey(ptrProductKey, ptrDateEnd);
                
            }
            catch(Exception ex)
            {
                // Обязательно освобождаем выделенную память
                Marshal.FreeHGlobal(ptrProductKey);
                Marshal.FreeHGlobal(ptrDateEnd);
                MessageBox.Show(ex.Message);
                return -1;
            }



            // Переводим результат в управляемый вид, если ключ продукта верен.
           if(ret==1) dateEnd = Marshal.PtrToStringAnsi(ptrDateEnd);

            // Обязательно освобождаем выделенную память
            Marshal.FreeHGlobal(ptrProductKey);
            Marshal.FreeHGlobal(ptrDateEnd);

            return ret;
       }


        /*
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
        */

    }
}
