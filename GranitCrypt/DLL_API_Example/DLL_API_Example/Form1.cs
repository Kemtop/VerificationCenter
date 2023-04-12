using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace DLL_API_Example
{
    public partial class Form1 : Form
    {
        /// <summary>
        /// Путь к секретному ключу пользователя.
        /// </summary>
        string PathToSk = "";

        /// <summary>
        /// Путь к базе данных системы криптографической защитыинформации.
        /// </summary>
        string PathToGranitDb = "";

        /// <summary>
        /// Пароль к секретному ключу
        /// </summary>
        string SkPswd = "";

        //Путь к ini файлу
        string PathToIni = "";

        //Поток вывода медленно работает.
        List<string> infoMess = new List<string>(); //Сообщения.
        int lastReadMess = 0; //Последнее сообщение.


        void GlMessage(string val)
        {
            infoMess.Add(val);
        }


        public Form1()
        {
            InitializeComponent();
        }


        private void button1_Click(object sender, EventArgs e)
        {
            infoMess.Clear();
            lastReadMess = 0;
            richTextBox1.Clear();
            backgroundWorker1.RunWorkerAsync();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
         /*
             //Для проверки алгоритмов.
            //Читаем файл в шестнацетиричной форме.
            string hexStr = System.IO.File.ReadAllText("sign1.txt");
            hexStr = hexStr.Replace("\r","");
            hexStr = hexStr.Replace("\n", "");
            byte[] array;

            array= Enumerable.Range(0, hexStr.Length).Where(x => x % 2 == 0)
                             .Select(x => Convert.ToByte(hexStr.Substring(x, 2), 16))
                             .ToArray();

            File.WriteAllBytes("sign1.bin", array);

            int x1 = 0;
           */ 
           


            //Путь к exe файлу приложения.
            string StartupPath = Application.StartupPath;
            //Путь к ini файлу
            PathToIni = StartupPath + "\\api_config.ini";

            //Создание объекта, для работы с ini файлом
            INIManager manager = new INIManager(PathToIni);

            //Чтение секций ini файла.
            //Получить значение по ключу PathToSecretKey из секции PathConfig
            //Путь к секретному ключу пользователя.
            PathToSk = manager.GetPrivateString("PathConfig", "PathToSecretKey");
            //Путь к базе данных системы криптографической защиты информации.
            PathToGranitDb = manager.GetPrivateString("PathConfig", "PathToGranitDb");
            //Читаю пароль к секретному ключу
            SkPswd = manager.GetPrivateString("PathConfig", "SkPwd");


            string mess1 = "";
            mess1 = "Путь к секретному ключу пользователя: \r\n" + PathToSk + "\r\n";
            richTextBox1.AppendText(mess1);

            mess1 = "Путь к БД системы: \r\n" + PathToGranitDb;
            richTextBox1.AppendText(mess1);

        }

        private void buttonPathSk_Click(object sender, EventArgs e)
        {
            openFileDialog1.FileName = "КлючПользователя";

            openFileDialog1.Filter = "Key files(*.grk)|*.grk|All files(*.*)|*.*";
            if (openFileDialog1.ShowDialog() == DialogResult.Cancel)
                return;

            // получаем выбранный файл
            string filename = openFileDialog1.FileName;


            //Создание объекта, для работы с ini файлом
            INIManager manager = new INIManager(PathToIni);

            //Путь к секретному ключу.
            manager.WritePrivateString("PathConfig", "PathToSecretKey", filename);

            PathToSk = filename;

            richTextBox1.Clear();
            string mess1 = "";
            mess1 = "Изменен путь к секретному ключу пользователя: \r\n" + PathToSk + "\r\n";
            richTextBox1.AppendText(mess1);
        }

        private void buttonPathDb_Click(object sender, EventArgs e)
        {
            openFileDialog1.FileName = "GranitDb";

            openFileDialog1.Filter = "Sys Db files(*.grb)|*.grb|All files(*.*)|*.*";
            if (openFileDialog1.ShowDialog() == DialogResult.Cancel)
                return;

            // получаем выбранный файл
            string filename = openFileDialog1.FileName;

            //Создание объекта, для работы с ini файлом
            INIManager manager = new INIManager(PathToIni);

            // Путь к базе данных системы криптографической защиты информации.
            manager.WritePrivateString("PathConfig", "PathToGranitDb", filename);

            PathToGranitDb = filename;

            richTextBox1.Clear();
            string mess1 = "Изменен путь к БД системы: \r\n" + PathToGranitDb;
            richTextBox1.AppendText(mess1);
        }

        private void buttonPswd_Click(object sender, EventArgs e)
        {
            FormPsw form = new FormPsw();
            if (form.ShowDialog() != DialogResult.OK) return;

            //Создание объекта, для работы с ini файлом
            INIManager manager = new INIManager(PathToIni);

            string psw = form.getPsw();

            /* Сохраняю пароль в ini файл. Ни в коем случае так не делать в рабочих приложениях! 
               Данная программа всего лишь демонстрирует работу с библиотекой и не в коем случае не может быть использована
                в иных целях!
            */
            manager.WritePrivateString("PathConfig", "SkPwd", psw);
            SkPswd = psw;
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            //Проверяю не было ли команды завершить поток.
            if (backgroundWorker1.CancellationPending)
                return;

            string ErrFlag = "";

            try
            {
                //Создание объекта для работы с библиотекой.
                GranitAPI grApi = new GranitAPI();

                //Получаю путь к секретному ключу.
                string Path = PathToSk;
                Path = Path.Replace("\\", "/"); //Преобразование пути понятное dll-ке.

                //Чтение контейнера ключа по указанному пути.
                GlMessage("Загрузка ключа.\r\n");
                backgroundWorker1.ReportProgress(0);

                ErrFlag = grApi.LoadSecretKey(Path);
                if (!String.IsNullOrEmpty(ErrFlag))
                {
                    GlMessage("Ошибка:" + ErrFlag);
                    backgroundWorker1.ReportProgress(0); //Ошибка.
                    return;
                }

                //Проверка пароля
                GlMessage("\r\nПроверка пароля к секретному ключу...");
                backgroundWorker1.ReportProgress(0);
                string Password = SkPswd;//"23Avx3ewt"; //"62Dfsakl11FFssd3";//"12345678";

                bool doRet = grApi.ChkPassword(Password, ref ErrFlag);
                if (!doRet)
                {
                    GlMessage("\r\n" + "Ошибка:" + ErrFlag);
                    backgroundWorker1.ReportProgress(0); //Ошибка.
                    return;
                }

                GlMessage("\r\nВерный пароль.");
                backgroundWorker1.ReportProgress(0);


                //Информация о ключе.
                string Familia = "";
                string Imia = "";
                string Otchestvo = "";
                string DateBegin = "";
                string DateEnd = "";
                string DaysLeft = "";
                //Получаю информацию секретного ключа пользователя.
                doRet = grApi.GetSkInfo(ref Familia, ref Imia, ref Otchestvo, ref DateBegin, ref DateEnd, ref DaysLeft);
                if (!doRet)
                {
                    GlMessage("Ошибка:" + "Не загружен секретный ключ.");
                    backgroundWorker1.ReportProgress(0); //Ошибка.
                    return;
                }

                //Вывожу информацию о секретном ключе пользователя.
                GlMessage("\r\nВладелец секретного ключа: " + Familia + " " + Imia + " " + Otchestvo + "\r\n" +
                            "Дата начала действия ключа: " + DateBegin + "\r\n" +
                            "Дата окончания действия ключа: " + DateEnd + "\r\n" +
                            "До окончания действия ключа осталось: " + DaysLeft + " дней." + "\r\n");
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.


                
                //Открытие базы  данных системы криптографической защиты информации ГранитК.
                string PathDb = PathToGranitDb;
                GlMessage("\r\nОткрытие базы  данных системы криптографической защиты информации ГранитК.");
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.

                //Путь к exe файлу приложения.
                string StartupPath = Application.StartupPath;
                //Путь к плагинам
                string PathToPlugin = StartupPath + "\\plugins";
                doRet = grApi.OpenCertDb(PathToPlugin, PathDb, ref ErrFlag);
                if (!doRet)
                {
                    GlMessage("\r\n" + "Ошибка:" + ErrFlag);
                    backgroundWorker1.ReportProgress(0); //Ошибка.
                    return;
                }

                GlMessage("\r\nБД открыта.");
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.


                //Проверяет валидность сертификата для данного ключа.
                GlMessage("\r\nПроверка валидности сертификата ключа.");
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.

                doRet = grApi.uCheckKeyCert(ref ErrFlag);
                if (!doRet)
                {
                    grApi.CloseDB(); //Закрытие БД системы. Нужно иначе сложно будет обновить БД.
                    GlMessage("\r\n" + "Ошибка:" + ErrFlag);
                    backgroundWorker1.ReportProgress(0); //Ошибка.
                    return;
                }

                GlMessage("\r\nСертификат закрытого ключа актуален.");
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.


                //Подпись сообщения.

                //------Формирую сообщение.
                byte[] aMessage = new byte[2057];
                int ax_point = 0;
                for (int i = 0; i < 2057; i++)
                {

                    if (ax_point < 254)
                    {
                        aMessage[i] = (byte)ax_point;
                        ax_point++;
                    }
                    else
                    {
                        ax_point = 0;
                        aMessage[i] = (byte)ax_point;
                    }
                }
                //----------Подписываю сообщение.
                GlMessage("\r\nПодпись сообщения..");
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.

                string SignStr = "";

                doRet = grApi.SignData(aMessage,ref SignStr, ref ErrFlag);
                if (!doRet)
                {
                    grApi.CloseDB(); //Закрытие БД системы. Нужно иначе сложно будет обновить БД.
                    GlMessage("\r\n" + "Ошибка:" + ErrFlag);
                    backgroundWorker1.ReportProgress(0); //Ошибка.
                    return;
                }

                GlMessage("Подписано байт:" + aMessage.Length.ToString() + "\r\n" +
                "Подпись: " + SignStr + "\r\n" +
                "\r\n" +
                "Длина подписи:" + SignStr.Length.ToString() +" символов" +
                "\r\n");
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.

                //Проверка подписи
                string FIOp = "";
                doRet = grApi.CheckSign(ref ErrFlag, aMessage,SignStr, ref FIOp);
                if (!doRet)
                {
                    grApi.CloseDB(); //Закрытие БД системы. Нужно иначе сложно будет обновить БД.
                    GlMessage("\r\n" + "Ошибка:" + "При проверке подписи возникла ошибка: " + ErrFlag);
                    backgroundWorker1.ReportProgress(0); //Ошибка.
                    return;
                }

                GlMessage("\r\n" +
                "Подпись верна. Подписант:" + FIOp);
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.

                //Сломаем сообщение и проверим подпись.
                GlMessage("\r\n" +
                "Порчу сообщение и проверяю подпись.");
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.

                aMessage[754] = 98;

                doRet = grApi.CheckSign(ref ErrFlag, aMessage,SignStr, ref FIOp);
                if (!doRet)
                {
                    GlMessage("\r\n" + "Ошибка:" + "При проверке подписи возникла ошибка: " + ErrFlag);
                    backgroundWorker1.ReportProgress(1); //Ошибка.

                }

                //Очищаю конфиденциальные данные.
                GlMessage("\r\n" +
                "Очистка памяти... ");
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.
                grApi.ClearLibr();
                GlMessage("\r\n" +
                "Успешно!");
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.

            }
            catch (Exception e1)
            {
                GlMessage("Возникло исключение в API " + e1.Message);
                backgroundWorker1.ReportProgress(0); //Вывожу информацию.
                return;
            }

        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {

            for (int i = lastReadMess; i < infoMess.Count(); i++)
            {
                richTextBox1.AppendText(infoMess[i]);
                lastReadMess++;
            }

        }
    }
}
