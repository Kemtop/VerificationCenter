namespace ManagedLib.CipherStore
{
    /// <summary>
    /// Локальное хранилище кодируемых и декодируемых файлов.
    /// </summary>
    internal interface ILocalStore
    {
        /// <summary>
        /// Создает каталог для сохранения зашифрованных файлов.
        /// </summary>
        /// <param name="path"></param>
        void InitEncriptStore(string path);

        /// <summary>
        /// Создает каталог для сохранения расшифрованных файлов.
        /// </summary>
        /// <param name="path"></param>
        void InitDecriptStore(string path);
    }
}
