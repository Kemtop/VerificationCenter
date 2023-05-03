using CryptoRoomLib.Cipher3412.FastConst;
using System;

namespace CryptoRoomLib.Cipher3412
{
    /*
     * Алгоритмы шифрования ГОСТ 34.12.
     */
    public static class Logic3412
    {
        /// <summary>
        /// Выполняет L(линейное преобразование), S(нелинейное преобразование) преобразования.
        /// </summary>
        /// <param name="input"></param>
        /// <param name="output"></param>
        private static void LsTransformation(U128t input, ref U128t output)
        {
            output.Low = 0;
            output.Hi = 0;

            for (byte index = 0; index < 16; ++index)
            {
                output.Low ^= PrecomputedLSTableLeft.Matrix[index, input.GetByte(index)];
                output.Hi ^= PrecomputedLSTableRight.Matrix[index, input.GetByte(index)];
            }
        }

        /// <summary>
        /// Выполняет итерацию сети Фейстеля.
        /// </summary>
        /// <param name="constantIndex"></param>
        /// <param name="leftIndex"></param>
        /// <param name="rigthIndex"></param>
        /// <param name="roudkeys"></param>
        static unsafe void FTransformation(int constantIndex, int leftIndex, int rigthIndex, ulong[] roudkeys)
        {
            fixed (ulong* buff = roudkeys)
            {
                U128t temp;
                U128t temp1 = new U128t();

                temp.Low = *(buff + leftIndex) ^ CipherConst3412.RoundConstantsLeft[constantIndex];
                temp.Hi = *(buff + leftIndex + 1) ^ CipherConst3412.RoundConstantsRight[constantIndex];

                LsTransformation(temp, ref temp1);

                *(buff + rigthIndex) ^= temp1.Low;
                *(buff + rigthIndex+1) ^= temp1.Hi;

                SwapBlocks(roudkeys, leftIndex, rigthIndex);
            }
        }

        /// <summary>
        /// Перестановка блоков в памяти.
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="leftIndex"></param>
        /// <param name="rigthIndex"></param>
        private static unsafe void SwapBlocks(ulong[] buffer, int leftIndex, int rigthIndex)
        {
            fixed (ulong* buffPtr = buffer)
            {
                //left[0] = left[0] ^ right[0];
                //left[1] = left[1] ^ right[1];
                *(buffPtr+leftIndex) ^= *(buffPtr+rigthIndex);
                *(buffPtr + leftIndex + 1) ^= *(buffPtr + rigthIndex + 1);

                //right[0] = left[0] ^ right[0];
                //right[1] = left[1] ^ right[1];
                *(buffPtr + rigthIndex) ^= *(buffPtr + leftIndex);
                *(buffPtr + rigthIndex + 1) ^= *(buffPtr + leftIndex + 1);

                //left[0] = left[0] ^ right[0];
                //left[1] = left[1] ^ right[1];
                *(buffPtr + leftIndex) ^= *(buffPtr + rigthIndex);
                *(buffPtr + leftIndex + 1) ^= *(buffPtr + rigthIndex + 1);
            }
        }
        
        /// <summary>
        /// Формирую раундовые ключи шифрования(160 байт, 20(64-bit) чисел).
        /// </summary>
        /// <param name="key"></param>
        internal static void DeploymentEncryptionRoundKeys(byte[] key, ulong[] roundKeys)
        {
            //Копирую мастер ключ.
            Buffer.BlockCopy(key, 0, roundKeys, 0, 32);

            for (int nextKeyIndex = 2, constantIndex = 0; nextKeyIndex != 10; nextKeyIndex += 2)
            {
                //Переделать на Buffer.BlockCopy.
                Array.Copy(roundKeys, 2 * (nextKeyIndex - 2), roundKeys,
                2 * (nextKeyIndex), 4);

                for (int feistelRoundIndex = 0; feistelRoundIndex < 8; ++feistelRoundIndex)
                {
                    //Выполняет итерацию сети Фейстеля.
                    FTransformation(constantIndex++, 2 * (nextKeyIndex), 
                        2 * (nextKeyIndex + 1), roundKeys);
                }
            }
        }

        /// <summary>
        /// Шифрует блок.
        /// </summary>
        /// <param name="data"></param>
        /// <param name="roundKeys"></param>
        internal static unsafe void EncryptBlock(ref U128t data, ulong[] roundKeys)
        {
            int round = 0;
            U128t cache = new U128t();
            
            fixed (ulong* roundPtr = roundKeys)
            {
                for (; round < 10 - 1; ++round)
                {
                    cache.Low = data.Low ^ *(roundPtr + 2 * round);
                    cache.Hi = data.Hi ^ *(roundPtr + 2 * round + 1);

                    LsTransformation(cache, ref data);
                }

                data.Low ^= *(roundPtr + 2 * round);
                data.Hi ^= *(roundPtr + 2 * round + 1);
            }
        }
    }
}
