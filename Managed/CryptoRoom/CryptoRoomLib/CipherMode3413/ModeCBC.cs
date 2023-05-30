using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlTypes;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using CryptoRoomLib.AsymmetricCipher;
using Microsoft.VisualBasic;
using Microsoft.Win32;

namespace CryptoRoomLib.CipherMode3413
{
    /// <summary>
    /// Реализация режима работы блочного шифра - режим простой замены с зацеплением,
    /// согласно ГОСТ 34.13-2015
    /// (Cipher Block Chaining, СВС)
    /// </summary>
    public class ModeCBC : IBlockCipherMode
    {
        private readonly ICipherAlgoritm _algoritm;
        public ModeCBC(ICipherAlgoritm algoritm)
        {
            _algoritm = algoritm;
        }

        /// <summary>
        /// Итерация в режиме простой замены(ГОСТп5.4стр16)
        /// P-открытый текст,MSB-значением n разрядов регистра сдвига с большими номерами,
        /// C-результирующий шифртекст
        ///
        /// part 16 байт - Блок сообщения подлежащего шифрованию
        /// MSB 16 байт - значением n разрядов регистра сдвига с большими номерами
        /// C_block[16] - результирующий шифротекст
        /// </summary>
        internal void DeсryptIterationCBC(ref Block128t part, Block128t msb, ref Block128t cBlock)
        {
            _algoritm.DecryptBlock(ref cBlock); //Расшифровываю блок.

            //Складываю  блоки.
            //Использование двух строк вместо вызова метода(XorBlocks) ускоряет работу.
            part.Low = cBlock.Low ^ msb.Low;
            part.Hi = cBlock.Hi ^ msb.Hi;
        }

        /// <summary>
        /// Складывает блоки длиной 16 байт по модулю 2. Метод используется только для тестов.
        /// </summary>
        /// <param name="block1"></param>
        /// <param name="block2"></param>
        /// <param name="result"></param>
        internal void XorBlocks(ref Block128t block1, ref Block128t block2, ref Block128t result)
        {
            result.Low = block1.Low ^ block2.Low;
            result.Hi = block1.Hi ^ block2.Hi;
        }

        /// <summary>
        /// Выполняет одну итерацию сцепки блоков.
        /// </summary>
        internal void IterationCBC(ref Register256t register, ref Block128t tmpBlock, ref Block128t lsb, ref Block128t сBlock, ref Block128t pBlock)
        {
            tmpBlock.Copy(ref сBlock); //Копирую С, так как оно измениться.
            lsb = register.LSB; //Получаю младшие байты LSB из регистра
            DeсryptIterationCBC(ref pBlock, lsb, ref сBlock); //Расшифровываю

            //Быстрый сдвиг.
            register.RightShift(); //Сдвигаю регистр R вправо на 16 байт, старшие ячейки заполняю значением шифротекста 

            //Заполнение старших бит значением блока шифротекста
            register.MSB = tmpBlock;
        }

        /// <summary>
        /// Декодирует данные.
        /// </summary>
        /// <param name="cryptfile"></param>
        /// <param name="outfile"></param>
        /// <param name="setMaxBlockCount">Возвращает количество обрабатываемых блоков в файле.</param>
        /// <param name="endIteration">Возвращает номер обработанного блока. Необходим для движения ProgressBar на форме UI.</param>
        /// <param name="setDataSize">Возвращает размер декодируемых данных.</param>
        public bool DecryptData(string cryptfile, string outfile, Action<ulong> setDataSize ,Action<ulong> setMaxBlockCount,
            Action<ulong> endIteration, Func<ulong, FileStream, byte[]> asReader)
        {
            Register256t register = new Register256t();// Регистр размером m = kn =  2*16
            Block128t lsb = new Block128t();//значением n разрядов регистра сдвига с большими номерами

            Block128t сBlock = new Block128t(); //Входящий шифротекст.
            Block128t pBlock = new Block128t(); //Исходящий декодированный текст.
            Block128t tmpBlock =  new Block128t(); //Хранит временные данные
            
            byte[] readBuffer = new byte[_algoritm.BlockSize];
            byte[] writeBuffer = new byte[_algoritm.BlockSize];

            Span<byte> buffSpan = new Span<byte>(readBuffer);
            Span<byte> writeSpan = new Span<byte>(writeBuffer);
            
            //Добавить проверку на out of range.

            using (FileStream outFile = new FileStream(outfile, FileMode.Create, FileAccess.Write))
            using (FileStream inFile = new FileStream(cryptfile, FileMode.Open, FileAccess.Read))
            {
                ulong usefulDataSize = FileFormat.ReadDataSize(inFile); //Считывает из файла размер блока шифрованных данных.

                byte[] sessionKey = asReader(usefulDataSize, inFile); //Чтение данных ассиметричной системы. Получение сессионного ключа.
                if (sessionKey == null) return false;
                
                _algoritm.DeployDecryptRoundKeys(sessionKey);

                var iv = FileFormat.ReadIV(inFile, usefulDataSize); //Считывает значение вектора iv.

                //Читаем с места где содержатся данные.
                inFile.Position = FileFormat.BeginDataBlock + FileFormat.DataSizeInfo;
                inFile.Read(buffSpan);
                сBlock.FromArray(readBuffer);

                register.FromArray(iv); //Заполнение регистра данными IV
                IterationCBC(ref register, ref tmpBlock, ref lsb, ref сBlock, ref pBlock);

                //Первый блок является длиной данных-вынести в логику формирования файла.
                ulong dataLen = pBlock.Low;//Длина блока данных в байтах. 
                ulong blockCount = dataLen / (ulong)_algoritm.BlockSize; //Количество блоков в сообщении

                setDataSize(dataLen);
                setMaxBlockCount(blockCount);

                //Цикл расшифровывания блоков	
                ulong blockNum = 0;
                for (; blockNum < blockCount; blockNum++)
                {
                    inFile.Read(buffSpan);
                    сBlock.FromArray(readBuffer);

                    IterationCBC(ref register, ref tmpBlock, ref lsb, ref сBlock, ref pBlock);

                    pBlock.ToArray(writeBuffer);
                    outFile.Write(writeSpan);

                    endIteration(blockNum);
                }

                //Кратна ли длина сообщения размеру блока шифра?
                int rLen = (int)(dataLen % (ulong)_algoritm.BlockSize);

                //Длина сообщения не кратна размеру блока шифра
                if (rLen != 0)
                {
                    //Считываю очередной блок
                    inFile.Read(buffSpan);
                    сBlock.FromArray(readBuffer);

                    IterationCBC(ref register, ref tmpBlock, ref lsb, ref сBlock, ref pBlock);

                    outFile.Write(writeBuffer,  0, rLen);

                    endIteration(blockNum);
                }

                inFile.Close();
                outFile.Close();
            }

            return true;
        }

        /// <summary>
        /// Кодирует данные.
        /// </summary>
        /// <param name="cryptfile"></param>
        /// <param name="outfile"></param>
        /// <param name="setMaxBlockCount">Возвращает количество обрабатываемых блоков в файле.</param>
        /// <param name="endIteration">Возвращает номер обработанного блока. Необходим для движения ProgressBar на форме UI.</param>
        /// <param name="setDataSize">Возвращает размер декодируемых данных.</param>
        public bool CryptData(string cryptfile, string outfile, Action<ulong> setDataSize,
            Action<ulong> setMaxBlockCount,
            Action<ulong> endIteration, Func<ulong, FileStream, byte[]> asReader)
        {
            byte[] sessionKey = CipherTools.GenerateRand(32); //Формирую случайное число размером 32байта, которое является сеансовым ключом.
            byte[] iv = CipherTools.GenerateRand(32); //Формирую случайный начальный вектор(32байта).



            //_algoritm. DeployDecryptRoundKeys(sessionKey);

            return true;
        }
    }
}
