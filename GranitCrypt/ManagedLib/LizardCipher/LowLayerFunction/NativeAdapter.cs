using System.Runtime.InteropServices;

namespace LizardEncryptLib.LowLayerFunction
{
    /// <summary>
    /// Реализует доступ к низкоуровневой библиотеке. Реализует основные функции необходимые для работы алгоритмов кодирования.
    /// ToDo
    /// Привести входные параметры функций в dll к одному формату.
    /// Передалать вызовы на Func<IntPtr,IntPtr,int>.
    /// </summary>
    public class NativeAdapter : ILowLayerFunctions
    {
        const string NativeLib = "NativeCrypt.dll";

        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int LoadSecretKey(IntPtr lastError, IntPtr pathToKey);
        
        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int SetSingleModeChipper();

        //Проверяет правильность пароля к секретному ключу пользователя.
        [DllImport(NativeLib, CallingConvention = CallingConvention.Cdecl)]
        static extern int CheckPasswordForSecretKey(IntPtr password, IntPtr error);

        /// <summary>
        /// Текст ошибки возникшей после вызова метода.
        /// </summary>
        private string _lastError;

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
        /// Возвращает последнее сообщение об ошибке.
        /// </summary>
        /// <returns></returns>

        public string GetLastError()
        {
            return _lastError;
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
