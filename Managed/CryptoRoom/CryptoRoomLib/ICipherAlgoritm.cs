namespace CryptoRoomLib
{
    /// <summary>
    /// Алгоритм шифрования.
    /// </summary>
    internal interface ICipherAlgoritm
    {
        /// <summary>
        /// Развертывание раундовых ключей.
        /// </summary>
        void DeployDecryptRoundKeys(byte[] key);

        /// <summary>
        /// Расшифровывает блок данных.
        /// </summary>
        /// <param name="block"></param>
        void DecryptBlock(ref Block128t block);
    }
}
