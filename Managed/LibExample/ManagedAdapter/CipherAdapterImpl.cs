using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ManagedAdapter
{
    /// <summary>
    /// Оболочка над не управляемой dll данной криптосистемы.
    /// </summary>
    public class CipherAdapterImpl : IManagedAdapter
    {
        const string NativeLib = "NativeDll.dll";

        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int LoadSecretKey(IntPtr lastError, IntPtr pathToKey);

        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int SetSingleModeChipper();

        //Проверяет правильность пароля к секретному ключу пользователя.
        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int CheckPasswordForSecretKey(IntPtr password, IntPtr error);

        //Загружает ассиметричный ключ шифрования.
        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int LoadAsymmetricKey(IntPtr password, IntPtr error);

        //Загружает ключ подписи с использованием эллиптической кривой.
        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int LoadSignedSecretKey(IntPtr password, IntPtr error);

        //Инициализирует механизм шифрования.
        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int InitCipher();

        //Удаляет объект механизма шифрования.
        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int ClearCipher();

        //Шифрует файл.
        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int CryptLocalFile(IntPtr srcPath, IntPtr dstDir, IntPtr error);

        //Дешифрует файл.
        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int DecryptLocalFile(IntPtr srcPath, IntPtr dstDir, IntPtr keyPassword, IntPtr error);

        //Генерирует 32 байтное число(int256) использую псевдослучайный генератор чисел библиотеки.
        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int GenerateRand256(IntPtr array, IntPtr getZero);

        /// <summary>
        /// Текст ошибки возникшей после вызова метода.
        /// </summary>
        private string _lastError;

        public string LastError
        {
            get => _lastError;
        }

        /// <summary>
        /// Задает тип ключа, с которым работает система-работа без удостоверяющего центра.
        /// </summary>
        /// <returns></returns>
        public bool SetSingleModeKeyType()
        {
            ClearError();

            int ret = SetSingleModeChipper();
            if (ret == 0)
            {
                _lastError = "Не удалось задать тип ключа.";
                return false;
            }

            return true;
        }

        /// <summary>
        /// Загружает секретный ключ из файла контейнера.
        /// </summary>
        /// <param name="pathToKey"></param>
        /// <returns></returns>
        public bool LoadSecretKey(string pathToKey)
        {
            ClearError();

            var param = AllocateWithParam(pathToKey);

            int ret = LoadSecretKey(param.error, param.str); //Вызываю из dll функцию LoadSecretKey

            // Переводим результат в управляемый вид
            string strErr = Marshal.PtrToStringAnsi(param.error);

            //Обязательно освобождаем выделенную память
            FreeResources(param.str, param.error);

            return CheckError(strErr, ret);
        }

        /// <summary>
        /// Проверяет правильность пароля к секретному ключу пользователя.
        /// </summary>
        /// <param name="password"></param>
        /// <returns></returns>
        public bool CheckPasswordForContainer(string password)
        {
            ClearError();

            var param = AllocateWithParam(password);
            int ret = CheckPasswordForSecretKey(param.str, param.error);
            // Переводим результат в управляемый вид
            string strErr = Marshal.PtrToStringAnsi(param.error);

            //Обязательно освобождаем выделенную память
            FreeResources(param.str, param.error);

            return CheckError(strErr, ret);
        }

        /// <summary>
        /// Загружает ассиметричный ключ шифрования.
        /// </summary>
        /// <param name="password"></param>
        /// <returns></returns>
        public bool LoadAsymmetricKey(string password)
        {
            ClearError();

            var param = AllocateWithParam(password);
            int ret = LoadAsymmetricKey(param.str, param.error);
            // Переводим результат в управляемый вид
            string strErr = Marshal.PtrToStringAnsi(param.error);

            //Обязательно освобождаем выделенную память
            FreeResources(param.str, param.error);

            return CheckError(strErr, ret);
        }

        /// <summary>
        /// Инициализирует механизм шифрования.
        /// </summary>
        /// <returns></returns>
        public bool InitCipherWorker()
        {
            ClearError(); ;
            int ret = InitCipher();
            if (ret == 0)
            {
                _lastError = "Не удалось инициализировать механизм шифрования.";
                return false;
            }

            return true;
        }

        /// <summary>
        /// Инициализирует механизм шифрования.
        /// </summary>
        /// <returns></returns>
        public bool ClearCipherWorker()
        {
            ClearError(); ;
            int ret = ClearCipher();
            if (ret == 0)
            {
                _lastError = "Не удалось очистить механизм шифрования.";
                return false;
            }

            return true;
        }

        /// <summary>
        /// Загружает ключ подписи с использованием эллиптической кривой
        /// </summary>
        /// <param name="password"></param>
        /// <returns></returns>
        public bool LoadSignedSecretKey(string password)
        {
            ClearError();

            var param = AllocateWithParam(password);
            int ret = LoadSignedSecretKey(param.str, param.error);
            // Переводим результат в управляемый вид
            string strErr = Marshal.PtrToStringAnsi(param.error);

            //Обязательно освобождаем выделенную память
            FreeResources(param.str, param.error);

            return CheckError(strErr, ret);
        }

        /// <summary>
        /// Шифрует файл.
        /// </summary>
        /// <param name="srcPath"></param>
        /// <param name="dstDir"></param>
        /// <returns></returns>
        public bool CryptlFile(string srcPath, string dstDir)
        {
            ClearError();

            IntPtr srcFilePath = Marshal.StringToHGlobalAnsi(srcPath);
            IntPtr dstDirPtr = Marshal.StringToHGlobalAnsi(dstDir);
            IntPtr error = Marshal.AllocHGlobal(16384);

            int ret = CryptLocalFile(srcFilePath, dstDirPtr, error);
            // Переводим результат в управляемый вид
            string strErr = Marshal.PtrToStringAnsi(error);

            //Обязательно освобождаем выделенную память
            FreeResources(srcFilePath, dstDirPtr, error);

            return CheckError(strErr, ret);
        }

        /// <summary>
        /// Расшифровывает файл.
        /// </summary>
        /// <param name="srcPath"></param>
        /// <param name="dstDir"></param>
        /// <param name="passwordToKey"></param>
        /// <returns></returns>
        public bool DecryptlFile(string srcPath, string dstDir, string passwordToKey)
        {
            ClearError();

            IntPtr srcFilePath = Marshal.StringToHGlobalAnsi(srcPath);
            IntPtr dstDirPtr = Marshal.StringToHGlobalAnsi(dstDir);
            IntPtr keyPasswordPtr = Marshal.StringToHGlobalAnsi(passwordToKey);
            IntPtr error = Marshal.AllocHGlobal(16384);

            int ret = DecryptLocalFile(srcFilePath, dstDirPtr, keyPasswordPtr, error);
            // Переводим результат в управляемый вид
            string strErr = Marshal.PtrToStringAnsi(error);

            //Обязательно освобождаем выделенную память
            FreeResources(srcFilePath, dstDirPtr, error, keyPasswordPtr);

            return CheckError(strErr, ret);
        }

        /// <summary>
        /// //Генерирует 32 байтное число(int256) использую псевдослучайный генератор чисел библиотеки.
        /// mode =1, возвращает пустой массив. 
        /// </summary>
        /// <returns></returns>
        public (bool result, byte[] data) Rand256(byte mode)
        {
            const int size = 32;
            IntPtr data = Marshal.AllocHGlobal(size);
            IntPtr modePtr = Marshal.AllocHGlobal(1);
            Marshal.WriteByte(modePtr, mode);

            int ret = GenerateRand256(data, modePtr);
            if (ret == 0)
            {
                _lastError = "Не удалось использовать встроенный генератор случайных чисел.";
                FreeResources(data, modePtr);
                return (false, new byte[] { });
            }

            byte[] result = new byte[size];
            Marshal.Copy(data, result, 0, size);
            FreeResources(data, modePtr);

            return (true, result);
        }


        /// <summary>
        /// Очищает значение внутренней переменной хрянящей сообщение об ошибке.
        /// </summary>
        private void ClearError()
        {
            _lastError = string.Empty;
        }

        /// <summary>
        /// Выделяет неуправляемую память и копирует входящую строку.
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        private (IntPtr error, IntPtr str) AllocateWithParam(string param)
        {
            IntPtr error = Marshal.AllocHGlobal(16384); //Выделяет память для сообщения об ошибке.
            IntPtr str = Marshal.StringToHGlobalAnsi(param); //Копирую строку в неуправляемую память.

            return (error, str);
        }

        /// <summary>
        /// Освобождает выделенную неуправляемую память.
        /// </summary>
        /// <param name="ptr"></param>
        private void FreeResources(params IntPtr[] ptr)
        {
            foreach (var item in ptr)
            {
                //Добавить затирание объекта нулями в памяти.
                Marshal.FreeHGlobal(item);
            }
        }

        /// <summary>
        /// Если функция dll вернула 0, копирует сообщение об ошибке, возвращает false.
        /// </summary>
        /// <param name="error"></param>
        /// <param name="result"></param>
        /// <returns></returns>
        private bool CheckError(string error, int result)
        {
            if (result != 1) //Возникла ошибка функция вернула 0.
            {
                _lastError = error;
                return false;
            }

            return true;
        }
    }
}
