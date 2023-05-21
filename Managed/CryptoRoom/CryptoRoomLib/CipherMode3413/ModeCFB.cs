namespace CryptoRoomLib.CipherMode3413
{
    /// <summary>
    /// Шифрование в режиме CFB, режим обратной связи по шифротексту.
    /// </summary>
    internal class ModeCFB
    {
        private readonly ICipherAlgoritm _algoritm;
        public ModeCFB(ICipherAlgoritm algoritm)
        {
            _algoritm = algoritm;
        }

        /// <summary>
        /// Выполняет одну итерацию.
        /// </summary>
        /// <param name="сBlock"></param>
        /// <param name="tmpBlock"></param>
        private void IterationCFB( ref Block128t сBlock, ref Block128t tmpBlock)
        {
            //Полученный в предыдущем цикле текст шифруется.
            _algoritm.EncryptBlock(ref сBlock);

            //Полученный шифротекст складывается с текущим блоком.
            сBlock.Low ^= tmpBlock.Low;
            сBlock.Hi ^= tmpBlock.Hi;
        }

        /// <summary>
        /// Шифрование в режиме CFB(Режим обратной связи по шифротексту).
        /// </summary>
        /// <param name="src"></param>
        /// <param name="dst"></param>
        /// <param name="initVector"></param>
        public void CfbEncrypt(byte[] src, byte[] initVector)
        {
            byte[] tmp = new byte[_algoritm.BlockSize];

            //В качестве входящего текста берем начальный вектор.
            Block128t сBlock = new Block128t();
            сBlock.FromArray(initVector);

            //Блок данных подлежащих кодированию.
            Block128t tmpBlock = new Block128t();
            Buffer.BlockCopy(src, 0, tmp, 0, _algoritm.BlockSize);
            tmpBlock.FromArray(tmp);

            IterationCFB(ref сBlock, ref tmpBlock);

            //Копируем результат.
            сBlock.ToArray(tmp);
            Buffer.BlockCopy(tmp,0, src, 0, _algoritm.BlockSize);

            int blockCount = src.Length / _algoritm.BlockSize; //Количество блоков подлежащих шифрованию.

            for (int i = 1; i < blockCount; i++)
            {
                //Копируем блок текста из предыдущей итерации. 
                Buffer.BlockCopy(src, (i - 1) * _algoritm.BlockSize, tmp, 0, _algoritm.BlockSize);
                сBlock.FromArray(tmp);

                //Копируем блок текста подлежащий кодированию.
                Buffer.BlockCopy(src, i * _algoritm.BlockSize, tmp, 0, _algoritm.BlockSize);
                tmpBlock.FromArray(tmp);

                IterationCFB(ref сBlock, ref tmpBlock);

                //Копирую результат.
                сBlock.ToArray(tmp);
                Buffer.BlockCopy(tmp, 0, src, i * _algoritm.BlockSize, _algoritm.BlockSize);
            }
        }
    }
}
