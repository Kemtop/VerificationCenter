using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace TestGranit
{
    public partial class MainForm : Form
    {
        private Action LoadAction;
        private Action TestAction;

        private GranitAPI gApi = new GranitAPI();
        private List<User> Users = new List<User>();

        public MainForm()
        {
            InitializeComponent();

            DbTable[] Tables = new DbTable[] 
            {
                new DbTable()
                {
                    Name = "TrBills",
                    Query ="select Id, dbo.TrBillsToSign(Id) as SignData from TrBills (nolock) where Del = 0",
                    Signs = new Sign[]
                    {
                        new Sign()
                        {
                            SignColName = "OprSign",
                            KeyColName = "OprKey",
                            UserColName = "OprUserId"
                        },
                        new Sign()
                        {
                            SignColName = "Tr2Sign",
                            KeyColName = "Tr2Key",
                            UserColName = "Tr2UserId"
                        },
                        new Sign()
                        {
                            SignColName = "Tr1Sign",
                            KeyColName = "Tr1Key",
                            UserColName = "Tr1UserId"
                        }
                    }.ToList()
                },
                new DbTable()
                {
                    Name = "Docs",
                    Query ="select Id, dbo.DocsToSign(Id) as SignData from Docs (nolock)",
                    Signs = new Sign[]
                    {
                        new Sign()
                        {
                            SignColName = "SecSign",
                            KeyColName = "SecKey",
                            UserColName = "SecUserId"
                        },
                        new Sign()
                        {
                            SignColName = "FstSign",
                            KeyColName = "FstKey",
                            UserColName = "FstUserId"
                        }
                    }.ToList()
                },
                new DbTable()
                {
                    Name = "Bills",
                    Query ="select Id, dbo.BillsToSign(Id) as SignData from Bills (nolock) where Date > '31.12.2018'",
                    Signs = new Sign[]
                    {
                        new Sign()
                        {
                            SignColName = "OprSign",
                            KeyColName = "OprKey",
                            UserColName = "OprUserId"
                        },
                        new Sign()
                        {
                            SignColName = "SecSign",
                            KeyColName = "SecKey",
                            UserColName = "SecUserId"
                        },
                        new Sign()
                        {
                            SignColName = "FstSign",
                            KeyColName = "FstKey",
                            UserColName = "FstUserId"
                        }
                    }.ToList()
                }
            };

            CmbTable.Items.AddRange(Tables);
            CmbTable.SelectedIndex = 0;

            LoadAction += LoadKeys;
            TestAction += TestSign;
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(Properties.Settings.Default.DbServer) || string.IsNullOrEmpty(Properties.Settings.Default.DbName) || string.IsNullOrEmpty(Properties.Settings.Default.BasePath))
            {
                MessageBox.Show("Обнаружены не все настройки программы", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                SettingForm sett = new SettingForm();
                if (sett.ShowDialog() != DialogResult.OK) Application.Exit();

                if (string.IsNullOrEmpty(Properties.Settings.Default.DbServer) || string.IsNullOrEmpty(Properties.Settings.Default.DbName) || string.IsNullOrEmpty(Properties.Settings.Default.BasePath))
                {
                    MessageBox.Show("Обнаружены не все настройки программы", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    Application.Exit();
                }
            }
        }

        private void ExitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void SettToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SettingForm sett = new SettingForm();
            if (sett.ShowDialog() == DialogResult.OK) LoadAction?.BeginInvoke(null, null);
        }

        private void MainForm_Shown(object sender, EventArgs e)
        {
            LoadAction?.BeginInvoke(null, null);
        }

        //При открытии программы или изменении настроек - перезагружаем пользователей
        //Делаем выгрузку из БД всех пользователей, имеющих галочку "Имеет подпись"
        //Ищем в папке с ключами папку пользователя
        //Ищем в папке пользователя закрытый ключ, сертификат, файл с паролем и базу открытых ключей
        //Если все найдено - делаем тест загрузки ключа, проверку пароля и соответствие закрытого ключа сертификату
        //Если все тесты пройдены - добавляем пользователя в список
        private void LoadKeys()
        {
            BlockComponents(false);
            ShowWaitPanel(true);
            WaitMessage("Загрузка пользователей");
            ClearLog();
            ClearUsers();
            Users.Clear();
            WriteLog("Загрузка пользователей из БД");
            using (SqlConnection conn = new SqlConnection())
            {
                try
                {
                    conn.ConnectionString = "Server=" + Properties.Settings.Default.DbServer + ";Database=" + Properties.Settings.Default.DbName + ";Trusted_Connection=True;";
                    conn.Open();
                    SqlCommand cmd = conn.CreateCommand();
                    cmd.CommandText = "select Id, Name from Users (nolock) where HasSign = 1 order by Name";
                    SqlDataAdapter da = new SqlDataAdapter(cmd);
                    DataTable table = new DataTable();
                    da.Fill(table);
                    foreach (DataRow row in table.Rows)
                    {
                        WaitMessage("Загрузка пользователей", table.Rows.Count, table.Rows.IndexOf(row));
                        string Id = row["Id"].ToString();
                        string Name = row["Name"].ToString();
                        //Проверяем пользователя в папке с сертификатами
                        if (!Directory.Exists(Properties.Settings.Default.BasePath + "\\" + Id))
                        {
                            if (!CbDisDebug.Checked)
                            {
                                WriteLog("Пользователь " + Name + " не найден в папке с ключами");
                            }
                            continue;
                        }

                        List<string> FIO = Name.Split(' ').ToList();
                        string ShortName = FIO[0] + FIO[1][0] + FIO[2][0];

                        User us = new User
                        {
                            Id = Id,
                            UserName = Name,
                            KeyFile = Properties.Settings.Default.BasePath + "\\" + Id + "\\Key"+ ShortName + ".grk",
                            CertFile = Properties.Settings.Default.BasePath + "\\" + Id + "\\Certificate[" + ShortName + "].cer"
                        };

                        string PassFile = Properties.Settings.Default.BasePath + "\\" + Id + "\\Pass.txt";

                        if (!File.Exists(PassFile))
                        {
                            if (!CbDisDebug.Checked)
                            {
                                WriteLog("У пользователя " + us.UserName +  " не найден файл пароля");
                            }
                            MessageBox.Show("У пользователя " + us.UserName + " не найден файл пароля", "Внимание", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            continue;
                        }

                        try
                        {
                            StreamReader fs = File.OpenText(PassFile);
                            us.Password = fs.ReadToEnd();
                            fs.Close();
                        }
                        catch (Exception ex)
                        {
                            if (!CbDisDebug.Checked)
                            {
                                WriteLog(ex.Message);
                            }
                            MessageBox.Show(ex.Message, "Внимание", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            continue;
                        }

                        List<string> files = Directory.GetFiles(Properties.Settings.Default.BasePath.Replace("\\", "/") + "/" + Id, "*.grb").ToList();
                        if (files.Count() == 0)
                        {
                            if (!CbDisDebug.Checked)
                            {
                                WriteLog("В папке пользователя " + us.UserName + " не найдена база ключей");
                            }
                            MessageBox.Show("В папке пользователя " + us.UserName + " не найдена база ключей", "Внимание", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            continue;
                        }

                        us.BasePath = files[0];


                        string err = us.CheckKeys();
                        if (!string.IsNullOrEmpty(err))
                        {
                            if (!CbDisDebug.Checked)
                            {
                                WriteLog(err + "\r\n");
                            }
                            MessageBox.Show(err, "Внимание", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            continue;
                        }

                        err = us.TestKeys(gApi);
                        if (!string.IsNullOrEmpty(err))
                        {
                            if (!CbDisDebug.Checked)
                            {
                                WriteLog("Для пользователя "+ us.UserName + ": " + err);
                            }
                            MessageBox.Show("Для пользователя " + us.UserName + ": " + err, "Внимание", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            continue;
                        }

                        Users.Add(us);

                        AddRow(Id, Name);
                        if (!CbDisDebug.Checked)
                        {
                            WriteLog("Пользователь " + Name + " добавлен\r\n");
                        }
                    }
                    conn.Close();
                    WriteLog("Пользователи загружены");
                }
                catch (Exception ex)
                {
                    if (CbDisDebug.Checked)
                    {
                        MessageBox.Show(ex.Message, "Внимание", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        WriteLog("Ошибка загрузки пользователей");
                    }
                    else
                        WriteLog(ex.Message);
                    ShowWaitPanel(false);
                    BlockComponents(true);
                    return;
                }
            }

            //string KeyStamp = "", SignStr = "", Err = "";

            //foreach (User us in Users)
            //{
            //    us.SignData(gApi, "Test message", ref KeyStamp, ref SignStr, ref Err);
            //    WriteLog(KeyStamp + "\r\n");
            //}
            ShowWaitPanel(false);
            BlockComponents(true);
        }

        private void ClearUsers()
        {
            InvokeExecute(() => DGVUsers.Rows.Clear());
        }

        private void AddRow(string Id, string Name)
        {
            InvokeExecute(() =>
            {
                DGVUsers.Rows.Add(Id, Name, 0);
                foreach(DataGridViewRow row in DGVUsers.Rows)
                {
                    row.HeaderCell.Value = String.Format("{0}", row.Index + 1);
                }
            });
        }

        private void ClearLog()
        {
            InvokeExecute(() => TBLog.Clear());
        }

        private void WriteLog(string Mess)
        {
            InvokeExecute(() => TBLog.AppendText(Mess + "\r\n"));
        }

        private void WaitMessage(string Message)
        {
            InvokeExecute(() => 
            {
                PgBWait.Visible = false;
                LblWait.Text = Message;
                LblWait.Visible = true;
            });
        }

        private void WaitMessage(string Message, int PrgMax, int PrgPos)
        {
            InvokeExecute(() => 
            {
                LblWait.Text = Message;
                PgBWait.Maximum = PrgMax;
                PgBWait.Value = PrgPos;
                LblWait.Visible = PgBWait.Visible = true;
            });
        }

        private void ShowWaitPanel(bool flag)
        {
            InvokeExecute(() =>
            {
                WaitPanel.Visible = flag;
                if (flag)
                {
                    LblWait.Visible = PgBWait.Visible = false;
                }
            });
        }

        private void BlockComponents(bool flag)
        {
            InvokeExecute(() =>
            {
                ControlBox = CmbTable.Enabled = BtnStart.Enabled = menuStrip1.Enabled = CbDisDebug.Enabled = flag;
            });
        }

        private void InvokeExecute(Action act)
        {
            if (InvokeRequired)
            {
                Invoke((MethodInvoker)(() => { act.Invoke(); }));
            }
            else
            {
                act.Invoke();
            }
        }

        public T InvokeQuery<T>(Func<T> func)
        {
            T t = default(T);
            if (InvokeRequired)
            {
                Invoke((MethodInvoker)(() => { t = func.Invoke(); }));
            }
            else
            {
                t = func.Invoke();
            }
            return t;
        }

        private void BtnStart_Click(object sender, EventArgs e)
        {
            TestAction?.BeginInvoke(null, null);
        }

        private void TestSign()
        {
            foreach (User us in Users)
            {
                us.CountSigns = 0;
                SetCount(us.Id, 0);
            }
            ClearLog();
            WriteLog("Начат тест подписи " + DateTime.Now);
            Random rnd = new Random();
            BlockComponents(false);
            ShowWaitPanel(true);
            WaitMessage("Загрузка полей для подписи");
            DbTable tab = (DbTable)InvokeQuery<object>(() => {return CmbTable.SelectedItem; });
            using (SqlConnection conn = new SqlConnection())
            {
                try
                {
                    conn.ConnectionString = "Server=" + Properties.Settings.Default.DbServer + ";Database=" + Properties.Settings.Default.DbName + ";Trusted_Connection=True;";
                    conn.Open();
                    SqlCommand cmd = conn.CreateCommand();
                    cmd.CommandText = tab.Query;
                    SqlDataAdapter da = new SqlDataAdapter(cmd);
                    DataTable table = new DataTable();
                    da.Fill(table);

                    string KeyStamp = "", SignStr = "", Err = "";

                    foreach (DataRow row in table.Rows)
                    {
                        WaitMessage("Тест подписи: " + (table.Rows.IndexOf(row) + 1) + " из " + table.Rows.Count, table.Rows.Count, table.Rows.IndexOf(row) + 1);
                        foreach (Sign sign in tab.Signs)
                        {
                            //Thread th = new Thread(() => 
                            //{
                            //    User us = Users[rnd.Next(0, Users.Count)];
                            //    if (!CbDisDebug.Checked)
                            //    {
                            //        WriteLog("Подписываем запись Id=" + row["Id"].ToString() + " ключом " + us.UserName);
                            //    }
                            //    bool sig = us.SignData(gApi, row["SignData"].ToString(), ref KeyStamp, ref SignStr, ref Err);
                            //    if (!sig)
                            //    {
                            //        WriteLog("Ошибка :" + Err);
                            //    }
                            //    try
                            //    {
                            //        cmd.Parameters.Clear();
                            //        cmd.CommandText = "update " + tab.Name + " set " + sign.UserColName + "=@UserId, " + sign.KeyColName + "=@Key, " + sign.SignColName + "=@Sign where Id = @RecId";
                            //        cmd.Parameters.Add(new SqlParameter("UserId", us.Id));
                            //        cmd.Parameters.Add(new SqlParameter("Key", KeyStamp));
                            //        cmd.Parameters.Add(new SqlParameter("Sign", SignStr));
                            //        cmd.Parameters.Add(new SqlParameter("RecId", row["Id"].ToString()));
                            //        cmd.ExecuteNonQuery();
                            //    }
                            //    catch (Exception ex)
                            //    {
                            //        WriteLog(ex.Message);
                            //    }
                            //    us.CountSigns++;
                            //    if (!CbDisDebug.Checked)
                            //    {
                            //        SetCount(us.Id, us.CountSigns);
                            //    }
                            //});

                            //th.Start();
                            User us = Users[rnd.Next(0, Users.Count)];
                            if (!CbDisDebug.Checked)
                            {
                                WriteLog("Подписываем запись Id=" + row["Id"].ToString() + " ключом " + us.UserName);
                            }
                            bool sig = us.SignData(gApi, row["SignData"].ToString(), ref KeyStamp, ref SignStr, ref Err);
                            if (!sig)
                            {
                                WriteLog("Ошибка :" + Err);
                                continue;
                            }
                            try
                            {
                                cmd.Parameters.Clear();
                                cmd.CommandText = "update " + tab.Name + " set " + sign.UserColName + "=@UserId, " + sign.KeyColName + "=@Key, " + sign.SignColName + "=@Sign where Id = @RecId";
                                cmd.Parameters.Add(new SqlParameter("UserId", us.Id));
                                cmd.Parameters.Add(new SqlParameter("Key", KeyStamp));
                                cmd.Parameters.Add(new SqlParameter("Sign", SignStr));
                                cmd.Parameters.Add(new SqlParameter("RecId", row["Id"].ToString()));
                                cmd.ExecuteNonQuery();
                            }
                            catch (Exception ex)
                            {
                                WriteLog(ex.Message);
                                continue;
                            }
                            us.CountSigns++;
                            if (!CbDisDebug.Checked)
                            {
                                SetCount(us.Id, us.CountSigns);
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    if (CbDisDebug.Checked)
                    {
                        MessageBox.Show(ex.Message, "Внимание", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        WriteLog("Ошибка запуска теста");
                    }
                    else
                        WriteLog(ex.Message);
                    return;
                }
            }

            foreach (User us in Users)
            {
                SetCount(us.Id, us.CountSigns);
            }

            WriteLog("Окончен тест подписи " + DateTime.Now);
            ShowWaitPanel(false);
            BlockComponents(true);
        }

        private void SetCount(string Id, int count)
        {
            InvokeExecute(() =>
            {
                IEnumerable<DataGridViewRow> rows = DGVUsers.Rows.Cast<DataGridViewRow>().Where(n => n.Cells["clId"].Value.ToString() == Id);
                if (rows.Count() > 0)
                {
                    rows.First().Cells["clCol"].Value = count;
                }
            });
        }
    }
}

