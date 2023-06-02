using CryptoRoomLib.Cipher3412;
using CryptoRoomLib.CipherMode3413;
using CryptoRoomLib;
using CryptoRoomLib.KeyGenerator;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Avalonia.Controls.Shapes;
using Path = System.IO.Path;
using System.Xml.Linq;
using System.Diagnostics;
using System.Threading;

namespace VanishBox
{
    public class CipherService : ICipherService
    {
        /// <summary>
        /// Сообщение об ошибке.
        /// </summary>
        public string LastError { get; set; }

        private readonly KeyService _keyService;

        public CipherService()
        {
            _keyService = new KeyService();
        }

        /// <summary>
        /// Проверяет пароль для ключа.
        /// </summary>
        /// <returns></returns>
        public bool CheckPassword(string password, string pathToKey)
        {
            
            var res = _keyService.LoadKey(pathToKey);
            if (!res)
            {
                LastError = _keyService.LastError;
                return false;
            }

            res = _keyService.CheckPassword(password);
            if (!res)
            {
                LastError = _keyService.LastError;
                return false;
            }

            return true;
        }

        /// <summary>
        /// Шифрует/расшифровывает файлы.
        /// </summary>
        /// <param name="paths"></param>
        /// <param name="direction"></param>
        /// <param name="sendInfo"></param>
        /// <param name="progressIteration"></param>
        /// <param name="textIteration"></param>
        /// <returns></returns>
        public bool RunOperation(string[] paths, bool direction, Action<string> sendInfo, 
            Action<int> progressIteration, Action<string> textIteration)
        {
            var dstDirName = "Decrypted_files";
            var processText = "Расшифровывание файла";
            var resultText = "расшифрован";
            if (direction)
            {
                dstDirName = "Encrypted_files";
                processText = "Шифрование файла";
                resultText = "зашифрован";
            }

            var dstDir = CreateDstDir(paths[0], dstDirName);


            ulong blockCount = 0;
            ulong blockNum = 0;
            ulong decryptDataSize = 0;
            
            ICipherAlgoritm algoritm = new CipherAlgoritm3412();
            IBlockCipherMode cipherMode = new ModeCBC(algoritm);

            CipherWorker worker = new CipherWorker(cipherMode);
            
            foreach (var file in paths)
            {
                var resultFileName = RemoveCryptExtension(Path.GetFileName(file));
                resultFileName = Path.Combine(dstDir, resultFileName);
                var fileName = Path.GetFileName(resultFileName);

                /*
                Проверка наличия файлa в папке. 
                    Касперский антивирус при попытки перезаписать точно такой  файл думает что он программа вирус,
                пытается откатить изменения программ. Т.е. если пользователь будет "играться" то антивирус примет такое поведения
                за вирусную программу.
                */
                if (File.Exists(resultFileName))
                {
                    sendInfo($"Файл {fileName} существует. Действий не требуется.");
                    continue;
                }

                textIteration($"{processText} {file}");

                Stopwatch stopwatch = new Stopwatch(); //Измерение времени операции.
                int steps = 1;
                
                bool result = worker.DecryptingFile(file, resultFileName, _keyService.GetPrivateAsymmetricKey(),
                    _keyService.KeyContainer.EcOid, _keyService.EcPublicKey,
                    (size) => { decryptDataSize = size; },
                    (max) => { blockCount = max; },
                    (number) =>
                    {
                        double progress = ((double)number / blockCount) * 100;
                        progressIteration((int)progress);
                    }, 
                    (text) =>
                    {
                        if (stopwatch.IsRunning)
                        {
                            stopwatch.Stop();
                            TimeSpan time = TimeSpan.FromMilliseconds(stopwatch.ElapsedMilliseconds);
                            sendInfo($"Обработка {fileName} шаг {steps} завершена. Затрачено {String.Format("{0:hh\\:mm\\:ss\\:fff}", time)} .");
                            steps++;
                        }

                        if (!stopwatch.IsRunning) stopwatch.Start();

                        textIteration($"{text} {file}");
                    });

                sendInfo($"Файл {fileName} {resultText}. Сохранен как {Path.GetFileName(resultFileName)}.");
            }

            return true;
        }

        /// <summary>
        /// Используя путь к файлу, создает директорию для сохранения шифрованых/расшифрованных файлов,
        /// возвращает путь к директории
        /// </summary>
        /// <param name="filePath"></param>
        /// <returns></returns>
        private string CreateDstDir(string filePath, string dirName)
        {
            try
            {
                var fileDir = Path.GetDirectoryName(filePath);
                var saveDir = Path.Combine(fileDir, dirName);

                if (!Directory.Exists(saveDir))
                {
                    Directory.CreateDirectory(saveDir);
                }

                return saveDir;
            }
            catch (Exception e)
            {
                LastError = e.Message;
                return null;
            }
        }

        /// <summary>
        /// Удаляет расширение ".crypt" из имени файла.
        /// </summary>
        /// <returns></returns>
        private string RemoveCryptExtension(string name)
        {
            return name.Replace(".crypt", "");
        }

        /// <summary>
        /// Проверяет наличие файла по указанному пути.
        /// </summary>
        /// <param name="filePath"></param>
        /// <returns></returns>
        private bool CheckFileIsExist(string filePath)
        {
            if (File.Exists(filePath))
            {
                return false;
            }

            return true;
        }
    }
}
