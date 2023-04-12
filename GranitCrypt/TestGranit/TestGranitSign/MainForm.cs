using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestGranitSign
{
    public partial class MainForm : Form
    {
        private Action LoadAction;
        private Action TestAction;
        private GranitAPI api = new GranitAPI();
        private List<User> Users = new List<User>();

        public MainForm()
        {
            InitializeComponent();

            DbTable[] Tables = new DbTable[]
            {
                new DbTable()
                {
                    Name = "TrBills",
                    Query ="select Id, dbo.TrBillsToSign(Id) as SignData, Tr1Key, Tr1Sign, Tr1UserId, Tr2Key, Tr2Sign, Tr2UserId, OprKey, OprSign, OprUserId from TrBills (nolock) where Del = 0",
                    Signs = new Sign[]
                    {
                        new Sign()
                        {
                            SignId = 0,
                            SignColName = "OprSign",
                            KeyColName = "OprKey",
                            UserColName = "OprUserId"
                        },
                        new Sign()
                        {
                            SignId = 2,
                            SignColName = "Tr2Sign",
                            KeyColName = "Tr2Key",
                            UserColName = "Tr2UserId"
                        },
                        new Sign()
                        {
                            SignId = 1,
                            SignColName = "Tr1Sign",
                            KeyColName = "Tr1Key",
                            UserColName = "Tr1UserId"
                        }
                    }.ToList()
                },
                new DbTable()
                {
                    Name = "Docs",
                    Query ="select Id, dbo.DocsToSign(Id) as SignData, FstKey, FstSign, FstUserId, SecKey, SecSign, SecUserId from Docs (nolock)",
                    Signs = new Sign[]
                    {
                        new Sign()
                        {
                            SignId = 2,
                            SignColName = "SecSign",
                            KeyColName = "SecKey",
                            UserColName = "SecUserId"
                        },
                        new Sign()
                        {
                            SignId = 1,
                            SignColName = "FstSign",
                            KeyColName = "FstKey",
                            UserColName = "FstUserId"
                        }
                    }.ToList()
                },
                new DbTable()
                {
                    Name = "Bills",
                    Query ="select Id, dbo.BillsToSign(Id) as SignData, FstKey, FstSign, FstUserId, SecKey, SecSign, SecUserId, OprKey, OprSign, OprUserId from Bills (nolock) where Date > '31.12.2018'",
                    Signs = new Sign[]
                    {
                        new Sign()
                        {
                            SignId = 0,
                            SignColName = "OprSign",
                            KeyColName = "OprKey",
                            UserColName = "OprUserId"
                        },
                        new Sign()
                        {
                            SignId = 2,
                            SignColName = "SecSign",
                            KeyColName = "SecKey",
                            UserColName = "SecUserId"
                        },
                        new Sign()
                        {
                            SignId = 1,
                            SignColName = "FstSign",
                            KeyColName = "FstKey",
                            UserColName = "FstUserId"
                        }
                    }.ToList()
                }
            };

            CmbTable.Items.AddRange(Tables);
            CmbTable.SelectedIndex = 0;

            LoadAction += Loading;
            TestAction += BeginTest;
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

        private void SettToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SettingForm sett = new SettingForm();
            if (sett.ShowDialog() == DialogResult.OK) LoadAction?.BeginInvoke(null, null);
        }

        private void ExitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
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

        private void MainForm_Shown(object sender, EventArgs e)
        {
            LoadAction?.BeginInvoke(null, null);
        }

        private void BtnStart_Click(object sender, EventArgs e)
        {
            TestAction?.BeginInvoke(null, null);
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

        private void ClearGrid()
        {
            InvokeExecute(() => DGVUsers.Rows.Clear());
        }

        private void ClearLog()
        {
            InvokeExecute(() => TBLog.Clear());
        }

        private void WriteLog(string Mess)
        {
            InvokeExecute(() => TBLog.AppendText(Mess + "\r\n"));
        }

        private void Loading()
        {
            string BasePath = Directory.GetFiles(Properties.Settings.Default.BasePath.Replace("\\", "/"), "*.grb").ToList().FirstOrDefault(), Err = "";
            if (!api.OpenCertDb(Application.StartupPath + "\\plugins", BasePath, ref Err)) return;
        }

        private void BeginTest()
        {
            BlockComponents(false);
            ClearLog();
            ClearGrid();
            WriteLog("Начат тест проверки подписи " + DateTime.Now);
            Users.Clear();
            DbTable tab = InvokeQuery<DbTable>(() => { return (DbTable)CmbTable.SelectedItem; });
            try
            {
                ShowWaitPanel(true);
                WaitMessage("Загрузка записей для проверки");
                using (SqlConnection conn = new SqlConnection())
                {
                    conn.ConnectionString = "Server=" + Properties.Settings.Default.DbServer + ";Database=" + Properties.Settings.Default.DbName + ";Trusted_Connection=True;";
                    conn.Open();
                    SqlCommand cmd = conn.CreateCommand();
                    cmd.CommandText = tab.Query;
                    SqlDataAdapter da = new SqlDataAdapter(cmd);
                    DataTable table = new DataTable();
                    da.Fill(table);
                    conn.Close();

                    string err = "", fio = "";
                    foreach (DataRow row in table.Rows)
                    {
                        WaitMessage("Проверка записи " + (table.Rows.IndexOf(row) + 1) + " из " + table.Rows.Count, table.Rows.Count, table.Rows.IndexOf(row) + 1);
                        foreach (Sign sign in tab.Signs)
                        {
                            string rowId = row["Id"].ToString();
                            string UserId = row[sign.UserColName].ToString();
                            if (CheckSign(UserId, row[sign.KeyColName].ToString(), row[sign.SignColName].ToString(), row["SignData"].ToString(), ref err, ref fio))
                            {
                                if (!CbDisDebug.Checked)
                                {
                                    WriteLog("Запись Id=" + rowId + " подпись " + (sign.SignId == 0 ? "операциониста" : (sign.SignId == 2 ? "второго лица" : "первого лица")) + " подписал " + fio);
                                }
                                if (Users.Where(n => n.Id == UserId).Count() == 0) Users.Add(new User() { Id = UserId, Name = fio, ColOpr = 0, ColTr1 = 0, ColTr2 = 0 });
                                User us = Users.Where(n => n.Id == UserId).First();
                                switch (sign.SignId)
                                {
                                    case 0:
                                        us.ColOpr++;
                                        break;
                                    case 1:
                                        us.ColTr1++;
                                        break;
                                    case 2:
                                        us.ColTr2++;
                                        break;
                                }

                                if (!CbDisDebug.Checked)
                                {
                                    UpdateUserGrid(us);
                                }
                            }
                            else
                            {
                                WriteLog("У записи с Id " + rowId + " неверная подпись " + (sign.SignId == 0 ? "операциониста" : (sign.SignId == 2 ? "второго лица" : "первого лица")));
                            }
                        }
                    }
                }
                foreach (User us in Users)
                {
                    UpdateUserGrid(us);
                }
                WriteLog("Окончен тест проверки подписи " + DateTime.Now);
            }
            catch (Exception ex)
            {
                WriteLog("Тест завершен с ошибкой");
                WriteLog(ex.Message);
            }

            ShowWaitPanel(false);
            BlockComponents(true);
        }

        private void UpdateUserGrid(User user)
        {
            InvokeExecute(() => 
            {
                if (DGVUsers.Rows.Cast<DataGridViewRow>().Where(n => n.Cells["clId"].Value.ToString() == user.Id).Count() == 0)
                {
                    DGVUsers.Rows.Add(user.Id, user.Name, 0, 0, 0);
                    DGVUsers.Sort(DGVUsers.Columns["clName"], ListSortDirection.Ascending);
                }
                DataGridViewRow row = DGVUsers.Rows.Cast<DataGridViewRow>().Where(n => n.Cells["clId"].Value.ToString() == user.Id).First();
                row.Cells["clOpr"].Value = user.ColOpr;
                row.Cells["clTr1"].Value = user.ColTr1;
                row.Cells["clTr2"].Value = user.ColTr2;
            });
        }

        private bool CheckSign(string UserId, string Key, string Sign, string Message, ref string Error, ref string FIO)
        {
            string DbUserPath = Properties.Settings.Default.BasePath.Replace("\\", "/") + "/" + UserId + "/";
            if (Directory.GetFiles(DbUserPath, "*.grk").Count() == 0)
            {
                Error = "Не найден файл ключа";
                return false;
            }
            Error = api.LoadSecretKey(Directory.GetFiles(DbUserPath, "*.grk").ToList().FirstOrDefault());
            if (!string.IsNullOrEmpty(Error)) return false;
            if(Directory.GetFiles(DbUserPath, "Certificate[*].cer").Count() == 0)
            {
                Error = "Не найден файл сертификата";
                return false;
            }
            if (!api.uCheckKeyCert(ref Error, Directory.GetFiles(DbUserPath, "Certificate[*].cer").ToList().FirstOrDefault())) return false;
            return api.CheckSign(ref Error, Encoding.UTF8.GetBytes(Message), Key, Sign, ref FIO);
        }
    }
}
