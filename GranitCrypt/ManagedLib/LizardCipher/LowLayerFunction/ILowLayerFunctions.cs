namespace LizardEncryptLib.LowLayerFunction
{
    /// <summary>
    /// Доступ к низкоуровневым функциям.
    /// </summary>
    public interface ILowLayerFunctions
    {
        /// <summary>
        /// Возвращает последнее сообщение об ошибке.
        /// </summary>
        /// <returns></returns>
        string GetLastError();

        /// <summary>
        /// Задает тип ключа, с которым работает система-работа без удостоверяющего центра.
        /// </summary>
        /// <returns></returns>
        bool SetSingleModeKeyType();

        /// <summary>
        /// Загружает секретный ключ из файла контейнера.
        /// </summary>
        /// <param name="pathToKey"></param>
        /// <returns></returns>
        bool LoadSecretKey(string pathToKey);

        /// <summary>
        /// Проверяет пароль для доступа к контейнеру.
        /// </summary>
        /// <returns></returns>
        bool CheckPasswordForContainer(string password);
    }
}
