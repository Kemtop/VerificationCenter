using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Common;
using System.Data.SQLite;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KeyGen.DAO
{
    class ImplDb : IDb
    {
        /// <summary>
        /// Полный путь к месту расположению БД SQLite.
        /// </summary>
        string DbPath = "";
        /// <summary>
        /// Флаг существования базы данных на диске.
        /// </summary>
        bool DbExist = true;

        /// <summary>
        /// Флаг БД открыта или нет. 
        /// </summary>
        bool DbIsOpen = false;

        //Соединение с БД.
        SQLiteConnection connection;

        /// <summary>
        /// Название базы данных.
        /// </summary>
        string baseName = "Data.db3"; //Название базы данных.


        /// <summary>
        /// Передает путь к базе данных.
        /// </summary>
        /// <param name="Path"></param>
        public void setDbPath(string Path)
        {
            DbPath = Path + "\\" + baseName;
            //Проверяю существует ли файл БД .
            if (!File.Exists(DbPath))
                DbExist = false;
        }

        /// <summary>
        /// Открывает соединение с БД если она уже не была открыта.
        /// </summary>
        bool OpenDb()
        {
            if (DbIsOpen) return true; //Соединение уже открыто.

            //Если БД не существует.
            if (!DbExist)
            {
                CreateDb(); //Создаю БД.
                DbIsOpen = true;
                return true;
            }


            //Бд существует.
            try
            {
                connection = new SQLiteConnection("Data Source=" + DbPath);
                connection.Open();
                DbIsOpen = true;
            }
            catch (Exception e)
            {
                ShowError(e.Message);
                return false;
            }


            return true;
        }



        /// <summary>
        /// В базе данных  ищет базовый номер.
        /// </summary>
        /// <param name="strDig"></param>
        /// <param name="dig"></param>
        /// <returns></returns>
        public bool findBasicDigit(UInt32 dig, ref bool findStts)
        {
            if (!OpenDb()) return false;

            findStts = false; //Запись не найдена.

            string sql = "select Id from Keys where BaseNum=" + dig.ToString();

            try
            {
                using (SQLiteCommand command = new SQLiteCommand(connection))
                {
                    command.CommandText = sql;
                    command.CommandType = CommandType.Text;
                    SQLiteDataReader rdr = command.ExecuteReader();

                    if (rdr.HasRows)
                    {
                        findStts = true;
                        return true;
                    }

                }

            }
            catch (Exception e)
            {
                ShowError(e.Message);
                return false;
            }

            return true;
        }


        /// <summary>
        /// Ищет в БД серийный номер.
        /// </summary>
        /// <param name="dig"></param>
        /// <param name="findStts"></param>
        /// <returns></returns>
        public bool findSerial(string serial, ref bool findStts)
        {
            if (!OpenDb()) return false;

            findStts = false; //Запись не найдена.

            string sql = "select Id from Keys where SoftKey='" + serial + "'";

            try
            {
                using (SQLiteCommand command = new SQLiteCommand(connection))
                {
                    command.CommandText = sql;
                    command.CommandType = CommandType.Text;
                    SQLiteDataReader rdr = command.ExecuteReader();

                    if (rdr.HasRows)
                    {
                        findStts = true;
                        return true;
                    }

                }

            }
            catch (Exception e)
            {
                ShowError(e.Message);
                return false;
            }

            return true;
        }






        /// <summary>
        /// Сохраняет серийный номер и сопутствующие данные в БД.
        /// </summary>
        /// <returns></returns>
        public bool SaveSerial(string strBaseNum, UInt32 BaseNum, string Serial, string Notes, DateTime Stamp, DateTime dEnd, ref long Id)
        {
            if (!OpenDb()) return false;

            /*
             [id] integer PRIMARY KEY AUTOINCREMENT NOT NULL,
                    [BaseNum] integer NOT NULL,
                    [BaseNumStr] VARCHAR(5) NOT NULL,
                    [SoftKey] VARCHAR(50) NOT NULL, 
                    [Note] VARCHAR(255),
                    [DateEnd] DATETIME NOT NULL,
                    [Stamp] DATETIME NOT NULL,
                    [UserName] VARCHAR(255) NOT NULL
             */
            string sql = "INSERT INTO Keys (BaseNum,BaseNumStr,SoftKey,Note,DateEnd,Stamp,UserName) " +
                            "  values (@BaseNum,@BaseNumStr,@SoftKey,@Note,@DateEnd,@Stamp,@UserName)";


            string now_ = Stamp.ToString("dd.MM.yyyy HH:mm:ss");
            string userName = System.Security.Principal.WindowsIdentity.GetCurrent().Name;
            string strDEnd = dEnd.ToString("dd.MM.yyyy HH:mm:ss");

            List<SQLiteParameter> parameters = new List<SQLiteParameter>()
          {
            new SQLiteParameter("@BaseNum", BaseNum ),
            new SQLiteParameter("@BaseNumStr",strBaseNum),
             new SQLiteParameter("@SoftKey",Serial),
              new SQLiteParameter("@Note",Notes),
               new SQLiteParameter("@Stamp",now_),
                new SQLiteParameter("@UserName",userName),
                new SQLiteParameter("@DateEnd",strDEnd),
            };

            long LastId = 0;
            if (!ExecuteNonQueryParams(sql, parameters, ref LastId)) return false;

            //Получаю последний вставленный идентификатор.
            Id = LastId;

            return true;
        }





        /// <summary>
        /// Создает структуру БД.
        /// </summary>
        bool CreateDb()
        {
            try
            {
                SQLiteConnection.CreateFile(DbPath); //Создает пустой файл БД.

                connection = new SQLiteConnection("Data Source=" + DbPath);
                connection.Open();//Открываю соединение.
            }
            catch (Exception e)
            {
                ShowError(e.Message);
                return false;
            }


            /*
                    [BaseNum] базовое число
                    [BaseNumStr] базовое число в 36 ричной системе счисления.
                    [SoftKey] ключ выданный пользователю.
                    [Stamp] временный отпечаток-когда была вставлена запись.
                    [UserName] имя пользователя вставившего запись.
                    [Note] примечание,
                    [DateEnd]-дата окончания действия серийного номера.

             */
            string sql = @"CREATE TABLE [Keys] (
                    [Id] integer PRIMARY KEY AUTOINCREMENT NOT NULL,
                    [BaseNum] integer NOT NULL,
                    [BaseNumStr] VARCHAR(7) NOT NULL,
                    [SoftKey] VARCHAR(30) NOT NULL, 
                    [Note] VARCHAR(255),
                    [DateEnd] VARCHAR(20) NOT NULL,
                    [Stamp] VARCHAR(20) NOT NULL,
                    [UserName] VARCHAR(255) NOT NULL
                    );";

            if (!ExecuteNonQuery(sql)) return false;


            //Создаю уникальные индексы.
            sql = "CREATE UNIQUE INDEX  IX_Keys1 ON Keys (BaseNum)";
            if (!ExecuteNonQuery(sql)) return false;

            //Создаю уникальные индексы.
            sql = "CREATE UNIQUE INDEX  IX_Keys2 ON Keys (SoftKey)";
            if (!ExecuteNonQuery(sql)) return false;

            return true;
        }

        /// <summary>
        /// Выполняет запрос без возвращения данных.
        /// </summary>
        /// <param name="sql"></param>
        /// <returns></returns>
        bool ExecuteNonQuery(string sql)
        {
            try
            {
                using (SQLiteCommand command = new SQLiteCommand(connection))
                {
                    command.CommandText = sql;
                    command.CommandType = CommandType.Text;
                    command.ExecuteNonQuery();
                }
            }
            catch (Exception e)
            {
                ShowError(e.Message);
                return false;
            }

            return true;
        }


        /// <summary>
        /// Выполняет запрос без возвращения данных.
        /// </summary>
        /// <param name="sql"></param>
        /// <returns></returns>
        bool ExecuteNonQueryParams(string sql, List<SQLiteParameter> Params, ref long LastId)
        {

            try
            {
                using (SQLiteCommand command = new SQLiteCommand(connection))
                {
                    command.CommandText = sql;
                    command.CommandType = CommandType.Text;
                    for (int i = 0; i < Params.Count; i++)
                    {
                        command.Parameters.Add(Params[i]);
                    }

                    command.ExecuteNonQuery();
                    LastId = connection.LastInsertRowId;
                }


            }
            catch (Exception e)
            {
                ShowError(e.Message);
                return false;
            }

            return true;
        }


        /// <summary>
        /// Выводит сообщение об ошибке.
        /// </summary>
        void ShowError(string mess)
        {
            MessageBox.Show(mess, "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        /// <summary>
        /// Выбирает и отображает данные таблицы серийных номеров. В случае ошибки возвращает false.
        /// </summary>
        /// <param name="dgv"></param>
        /// <returns></returns>
        public bool SelectData(ref DataGridView dgv)
        {
            if (!OpenDb()) return false;

            string sql = "Select Id,SoftKey,Note,Stamp,DateEnd from [Keys]";
            try
            {
                SQLiteDataAdapter adapter = new SQLiteDataAdapter(sql, connection);
                DataTable dt = new DataTable();
                DataSet ds = new DataSet();
                adapter.Fill(dt);
                ds.Tables.Add(dt);
                dgv.DataSource = ds.Tables[0];
            }
            catch (Exception e)
            {
                ShowError(e.Message);
                return false;
            }

            return true;
        }


        /// <summary>
        /// Обновляет данные в гриде и устанавливает на добавленную запись.
        /// </summary>
        /// <param name="dgv"></param>
        /// <param name="id"></param>
        /// <returns></returns>
        public bool UpdateGrid(ref DataGridView dgv, long id)
        {
            if (!OpenDb()) return false;

            string sql = "Select Id,SoftKey,Note,Stamp,DateEnd from [Keys]";
            
            try
            {
                SQLiteDataAdapter adapter = new SQLiteDataAdapter(sql, connection);
                DataTable dt = new DataTable();
                DataSet ds = new DataSet();
                adapter.Fill(dt);
                ds.Tables.Add(dt);
                dgv.DataSource = ds.Tables[0];


                //Устанавливаю на запись с требуемым ид.
                string s_id = id.ToString();
                foreach (DataGridViewRow row in dgv.Rows)
                {
                    if (row.Cells["Id"].Value.ToString() == s_id)
                    {
                        dgv.CurrentCell = dgv[1, row.Index];
                    }
                }
            }
            catch (Exception e)
            {
                ShowError(e.Message);
                return false;
            }

            return true;
        }

    }
}
