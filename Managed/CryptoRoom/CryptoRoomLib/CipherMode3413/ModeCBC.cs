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
using Microsoft.VisualBasic;
using Microsoft.Win32;

namespace CryptoRoomLib.CipherMode3413
{
    /// <summary>
    /// Реализация режима работы блочного шифра - режим простой замены с зацеплением,
    /// согласно ГОСТ 34.13-2015
    /// (Cipher Block Chaining, СВС)
    /// </summary>
    internal class ModeCBC
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
            XorBlocks(ref cBlock, ref msb, ref part); //Складываю  блоки.
        }

        /// <summary>
        /// Складывает блоки длиной 16 байт по модулю 2.
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
        void IterationCBC(ref Register256t register, ref Block128t tmpBlock, ref Block128t lsb, ref Block128t сBlock, ref Block128t pBlock)
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
        /// 
        /// </summary>
        /// <param name="iv">Начальный вектор считанный из файла, 32 байта.</param>
        public void DecryptData(ulong[] iv, string cryptfile, string outfile, int beginPos)
        {
            Register256t register = new Register256t();// Регистр размером m = kn =  2*16
            Block128t lsb = new Block128t();//значением n разрядов регистра сдвига с большими номерами

            Block128t сBlock = new Block128t(); //Входящий шифротекст.
            Block128t pBlock = new Block128t(); //Исходящий декодированный текст.
            Block128t tmpBlock =  new Block128t(); //хранит временные данные
            
            сBlock.Low = 0xda3ecc31a05c9124; //Удалить после переноса алгоритма!
            сBlock.Hi = 0x04139dc14ab5b347;

            register.FromArray(iv); //Заполнение регистра данными IV
            IterationCBC(ref register, ref tmpBlock, ref lsb, ref сBlock, ref pBlock);
            
            //Первый блок является длиной данных-вынести в логику формирования файла.
            ulong dataLen = pBlock.Low;//Длина блока данных в байтах. 
            ulong blockCount = dataLen / (ulong)_algoritm.BlockSize; //Количество блоков в сообщении

            //Прогресс обработки файлов.
            //Вызвать функцию принимающую максимальное количество блоков и передать ей block_count.

            /*
             *Продумать алгоритм работы с большими файлами
             *  Необходимо для ускорения работы алгоритма-иначе огромное количество  времени тратиться на
             * движения прогресс бара
			   //При 200мб это сильно ощущается
		        if (block_count > 10000) progress_step = 1000;
             */

            byte[] buffer = new byte[_algoritm.BlockSize];
            byte[] writeBuffer = new byte[_algoritm.BlockSize];

            Span<byte> buffSpan = new Span<byte>(buffer);
            Span<byte> writeSpan = new Span<byte>(writeBuffer);
            
            //Добавить проверку на out of range.

            using (FileStream outFile = new FileStream(outfile, FileMode.Create, FileAccess.Write))
            using (FileStream inFile = new FileStream(cryptfile, FileMode.Open, FileAccess.Read))
            {
                inFile.Position = beginPos; //Читаем с места где содержатся данные.

                //Цикл расшифровывания блоков										   
                for (ulong i = 0; i < blockCount; i++)
                {
                    inFile.Read(buffSpan);
                    сBlock.FromArray(buffer);

                    IterationCBC(ref register, ref tmpBlock, ref lsb, ref сBlock, ref pBlock);

                    pBlock.ToArray(writeBuffer);
                    outFile.Write(writeSpan);
                }

                //Кратна ли длина сообщения размеру блока шифра?
                int rLen = (int)(dataLen % (ulong)_algoritm.BlockSize);

                //Длина сообщения не кратна размеру блока шифра
                if (rLen != 0)
                {
                    //Считываю очередной блок
                    inFile.Read(buffSpan);
                    сBlock.FromArray(buffer);

                    IterationCBC(ref register, ref tmpBlock, ref lsb, ref сBlock, ref pBlock);

                    outFile.Write(writeBuffer,  0, rLen);
                }

                inFile.Close();
                outFile.Close();
            }
        }
    }
}
