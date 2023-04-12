using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KeyGen.DAO
{
    internal interface IDb
    {
        /// <summary>
        /// Передает путь к базе данных.
        /// </summary>
        /// <param name="Path"></param>
        void setDbPath(string Path);

        /// <summary>
        /// В базе данных ищет базовое число.  В случае ошибки выводит сообщение, возвращает false;
        /// </summary>
        /// <param name="strDig"></param>
        /// <param name="dig"></param>
        /// <returns></returns>
        bool findBasicDigit(UInt32 dig, ref bool findStts);

        /// <summary>
        /// Ищет в БД серийный номер.
        /// </summary>
        /// <param name="dig"></param>
        /// <param name="findStts"></param>
        /// <returns></returns>
        bool findSerial(string serial, ref bool findStts);

        /// <summary>
        /// Сохраняет серийный номер и сопутствующие данные в БД.
        /// </summary>
        /// <returns></returns>
        bool SaveSerial(string strBaseNum, UInt32 BaseNum, string Serial, string Notes, DateTime Stamp, DateTime dEnd, ref long Id);

        /// <summary>
        /// Выбирает и отображает данные таблицы серийных номеров. В случае ошибки возвращает false.
        /// </summary>
        /// <param name="dgv"></param>
        /// <returns></returns>
        bool SelectData(ref DataGridView dgv);

        /// <summary>
        /// Обновляет данные в гриде и устанавливает на добавленную запись.
        /// </summary>
        /// <param name="dgv"></param>
        /// <param name="id"></param>
        /// <returns></returns>
        bool UpdateGrid(ref DataGridView dgv, long id);
    }
}
