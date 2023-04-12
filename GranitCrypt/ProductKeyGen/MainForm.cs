using KeyGen.DAO;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SQLite;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
//using System.Threading.Tasks;
using System.Windows.Forms;

namespace KeyGen
{
    /// <summary>
    /// Делегат для передачи базового числа из класса который выполняет тест алгоритма.
    /// </summary>
    /// <param name="value"></param>
    delegate void dg_setBaseNumValue(int value);

    public partial class MainForm : Form
    {

        IDb iDb = new ImplDb(); //Интрефейс для работы с БД.
        int valBaseNum = 0; //Значение базового числа, используется в тестах.

        public MainForm()
        {
            InitializeComponent();
            valBaseNum=-10000; //Значение начального флага.
        }

        /// <summary>
        /// Выводит сообщение об ошибке.
        /// </summary>
        void ShowError(string mess)
        {
            MessageBox.Show(mess, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }


        /// <summary>
        /// Сохраняю серийный номер в файл.
        /// </summary>
        /// <returns></returns>
        bool SaveSnToFile(string Sn, DateTime now)
        {
            //Получаю путь к exe приложению он же. 
            string exePath = Application.StartupPath.ToString();
            string pathSn = exePath + "\\Sn";

            //Существует ли папка в которую сохраняю файл с серийным номером?
            bool folderExists = Directory.Exists(pathSn);
            if (!folderExists)
                Directory.CreateDirectory(pathSn);
            else
            {
                //Удалить все содержимое из папки.
                System.IO.DirectoryInfo di = new DirectoryInfo(pathSn);
                foreach (FileInfo file in di.GetFiles())
                {
                    file.Delete();
                }

            }

            //Формирую имя файла.
            string strNow = now.ToString("dd.MM.yyyy HH:mm:ss");
            strNow = strNow.Replace('.', '_');
            strNow = strNow.Replace(':', '_');
            strNow = strNow.Replace(' ', '_');

            string FileName = "Sn[" + strNow + "].txt";
            pathSn = pathSn + "\\" + FileName;

            //Сохраняю в файл.
            using (FileStream fs = File.Create(pathSn))
            {
                byte[] info = new UTF8Encoding(true).GetBytes(Sn);
                fs.Write(info, 0, info.Length);
                fs.Close();
            }

            string sInfo = "Серийный номер сохранен в файл по пути: " + pathSn;
            richTextBoxInfo.Clear();
            richTextBoxInfo.AppendText(sInfo);
            richTextBoxInfo.Visible = true;

            return true;
        }


        private void BtnGen_Click(object sender, EventArgs e)
        {
            string Note = richTextBoxNote.Text; //Примечание.
            if (String.IsNullOrEmpty(Note))
            {
                ShowError("Примечание не может быть пустым.");
                return;
            }

            UInt32 baseDig = 0; //Базовое число.
            string StrBaseDig = "";//Базовое число в 36 ричной системе.
            string Serial = ""; //Серийный номер.

            DateTime DEnd = DtpDate.Value.Date; //Дата окончания действия серийного номера.

            SnGenerator SnG = new SnGenerator(iDb);//Объект для генерации серийных номеров.

            //Генерирую серийный номер.
            if (!SnG.GenSerial(ref Serial, ref StrBaseDig, ref baseDig, DEnd)) return;

            //Такого быть не может. Но проверю на всякий случай. Поиск серийного номера в БД.
            bool findSerialStts = false;
            if (!iDb.findSerial(Serial, ref findSerialStts)) return;
            if (findSerialStts)
            {
                ShowError("Обнаружен повторяющийся серийный номер. " + Serial);
                return;
            }


            //Текущая дата.
            DateTime now = DateTime.Now;
            DateTime dtEnd = new DateTime();        

            //Выполняю проверку серийного номера.
            SnCheck Sch = new SnCheck();
            int a = Sch.Check(Serial, now, ref dtEnd);
            if (a > 0)
            {
                ShowError("Ошибка генерации серийного номера. Код " + a.ToString() + ".");
                return;
            }

            if (DEnd != dtEnd)
            {
                ShowError("Ошибка генерации серийного номера.Не совпадение даты окончания. dtEndKey=" +
                      DEnd.ToString() + ", dtEndProgram " + dtEnd.ToString()
                    );
                return;
            }


            //Проверка серийного номера с использованием статической c++ библиотеки, используемой в защищаемом приложении.
            NativeDllStApi ApiDll = new NativeDllStApi();
            string nativePkDateEnd = ""; //Дата окончания действия ключа продукта.
            int res = ApiDll.ApiCheckKey(Serial, ref nativePkDateEnd);

            if (res != 1)
            {
                ShowError("Ошибка генерации серийного номера. Нативная библиотека не подтвердила правильность номера.");
                return;
            }

            //Проверка дат.
            DateTime PkDateEnd = DtpDate.Value.Date; //Текущая дата в контроле.
            string str_PkDateEnd = PkDateEnd.ToString("dd.MM.yyyy");

            if (nativePkDateEnd != str_PkDateEnd)
            {
                ShowError("Ошибка генерации серийного номера.Не совпадение даты окончания. UserContorol=" +
                    str_PkDateEnd + ",nativeCheck=" + nativePkDateEnd);
                return;
            }


            long id = 0; //Ид вставленной строки.

            //Cохраняю данные в БД.
            if (!iDb.SaveSerial(StrBaseDig, baseDig, Serial, Note, now, DEnd, ref id)) return;
            SaveSnToFile(Serial, now); //Сохранение серийного номера в файл.

            TbSerial.Text = Serial;

            //Обновление данных в гриде.
            iDb.UpdateGrid(ref dataGridView1, id);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {

            //Получаю путь к exe приложению он же. 
            string exePath = Application.StartupPath.ToString();
            string pathDb = exePath + @"\\Db";

            //Существует ли папка с БД?
            bool folderExists = Directory.Exists(pathDb);
            if (!folderExists)
                Directory.CreateDirectory(pathDb);

            iDb.setDbPath(pathDb); //Устанавливаю путь к БД.

            //Установка даты окончания
            DtpDate.Value = DateTime.Now.AddYears(1);//Прибавляю год.  
                                                     //Чтение данных.
            iDb.SelectData(ref dataGridView1);
        }

        /// <summary>
        /// Запуск теста.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            dg_setBaseNumValue p;
            p = setBaseNumValue; //Делегат для получения базового числа.

            SnGenerator SnG = new SnGenerator(iDb); //Объект генерации ключа.
            SnG.setParamDelegate(p); //Передаю делегат для получения данных о базовом числе.
            
            backgroundWorker1.RunWorkerAsync(); //Запускаю поток для отображения прогресса.

            //В отдельном потоке запускаю сам тест.
            ThreadStart ths = new ThreadStart(SnG.BeginTest);
            Thread t = new Thread(ths);
            t.Start();

        }


        /// <summary>
        /// Передает базовое число.
        /// </summary>
        /// <param name="value"></param>
        void setBaseNumValue(int value)
        {
            valBaseNum = value;
        }


        /// <summary>
        /// То что будет делать фоновый процесс.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            //Проверяю не было ли команды завершить поток.
            if (backgroundWorker1.CancellationPending)
                return;      

            int max = SnGenerator.getMaxBaseNum(); 

            bool Work = true; //Окончание работы потока.

            while (Work)
            {
                Thread.Sleep(1000);
                if (valBaseNum == -10000) continue; //Начальный флаг, программа только запустилась.

                //Возникла ошибка или тест завершился.
                if ((valBaseNum == 0) || (valBaseNum < 0)) Work = false;

                int curValue = valBaseNum;
                         
                double oneP = max / 100;  //1%=oneP
                double pers = curValue / oneP; //Количество процентов.
                int persent = (int)pers;

                backgroundWorker1.ReportProgress(persent);
            }


        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if (valBaseNum == -10000) return; //Начальный флаг, программа только запустилась.

            richTextBoxInfo.Visible = true;
            richTextBoxInfo.Clear();
            string mess = "";

            //Тест еще работает.
            if (valBaseNum > 0)
            {
                mess = "Тест запущен.\n " +
               "Выполнено " + e.ProgressPercentage.ToString() + " % \n" +
               "Текущее значение: " + valBaseNum.ToString();
            }
            else
            {
                if (valBaseNum==0) //Тест закончен.
                {
                    mess = "Тест завершен успешно.";
                }

                if (valBaseNum<0) //Ошибка выполнения теста.
                {
                    mess = "Ошибка выполнения теста. Код "+ valBaseNum.ToString();
                }

            }
            
            richTextBoxInfo.AppendText(mess);
        }
    }

}

