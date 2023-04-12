using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KeyGen
{
    /// <summary>
    /// Класс для тестирования числовых рядов получаемых в результате выполнения функций.
    /// </summary>
    class TestSeries
    {
        public void TestF1()
        {
          
            Int64 y,x;
            UInt64 ret;

            FileStream fs = new FileStream("Test1.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            for (int i = 1; i < 100; i++)
            {
                x = i * (-15200);
                y = (715 * x) + 25300;
                ret = ((UInt64)y) & 0xffffffff;
                ret = ret << 16;               
                sw.WriteLine(x.ToString() + " " +ret.ToString());
            }

            sw.Close();
            fs.Close();
            MessageBox.Show("Завершен.");

        }
        

        /// <summary>
        /// График функции похож на экспоненту.
        /// </summary>
        public void TestF2()
        {
            UInt64 y,x;
            FileStream fs = new FileStream("Test2.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            for (UInt64 i = 1; i < 4294967296; i=i+1000)
            {
                x = i;
                y = (x * x) + 3578;
                sw.WriteLine(x.ToString() + " " + y.ToString());
            }

            sw.Close();
            fs.Close();
            MessageBox.Show("Завершен.");
            
        }


        /// <summary>
        /// График функции похож на экспоненту.
        /// </summary>
        public void TestF3()
        {
            Int64 y, x;
            UInt64 ret;
            FileStream fs = new FileStream("Test3.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            for (UInt64 i = 1; i < 4294967296; i = i + 1000)
            {
                x = (Int64)i;
                y = (x * x)+x*2-3578;
                ret = (UInt64)y;
                sw.WriteLine(x.ToString() + " " + y.ToString());
            }

            sw.Close();
            fs.Close();
            MessageBox.Show("Завершен.");

        }


        public void TestF4()
        {
            Int64 y, x;
            UInt64 ret;
            FileStream fs = new FileStream("Test4.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            for (UInt64 i = 1; i < 4294967296; i = i + 100000)
            {
                x = (Int64)i;
                y = 2793967291-((x * x) - x * 7 - 5718);
                ret = (UInt64)y;
                sw.WriteLine(x.ToString() + " " + ret.ToString());
            }

            sw.Close();
            fs.Close();
            MessageBox.Show("Завершен.");

        }


        public void TestF5()
        {
            Int64 y, x,t;
            UInt64 ret;
            FileStream fs = new FileStream("Test5.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            for (UInt64 i = 1; i < 4294967296; i = i + 1000000)
            {
                x = (Int64)i;
                y = ((x * x) - x * 7 - 5718);
                t = 2*(x * x) - x * 7 - 10287;
                y = (y + t)+78;
                ret = (UInt64)y;
                sw.WriteLine(x.ToString() + " " + ret.ToString());
            }

            sw.Close();
            fs.Close();
            MessageBox.Show("Завершен.");

        }


        /// <summary>
        /// Линейно возрастающая.
        /// </summary>
        public void TestF6()
        {
            Int64 y, x;
            UInt64 ret;
            FileStream fs = new FileStream("Test6.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            for (UInt64 i = 1; i < 4294967296; i = i + 1000000)
            {
                x = (Int64)i;
                y = 31979673410-((x * 7 + 57158)/80);
                ret = (UInt64)y;
                sw.WriteLine(x.ToString() + " " + ret.ToString());
            }

            sw.Close();
            fs.Close();
            MessageBox.Show("Завершен.");

        }

        /// <summary>
        /// Ломаная.
        /// </summary>
        public void TestF7()
        {
            Int64 y, x, t;
            UInt64 ret;
            FileStream fs = new FileStream("Test7.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            for (UInt64 i = 1; i < 4294967296; i = i + 1000000)
            {
                x = (Int64)i;
                y = (x * x*5) +x*3-15324;
                t = y >> 2;
                y = y | t;
                ret = (UInt64)y;
                sw.WriteLine(x.ToString() + " " + ret.ToString());
            }

            sw.Close();
            fs.Close();
            MessageBox.Show("Завершен.");

        }

        /// <summary>
        /// Ломаная.
        /// </summary>
        public void TestF8()
        {
            Int64 y, x, t;
            UInt64 ret;
            FileStream fs = new FileStream("Test8.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            for (UInt64 i = 1; i < 4294967296; i = i + 1000000)
            {
                x = (Int64)i;
                y = (x * x * 3) + x * 2 - 215324;
                t = (y >> 4)+(x*x);
                y = y & t;

                ret = (UInt64)y;                
                sw.WriteLine(x.ToString() + " " + ret.ToString());
            }

            sw.Close();
            fs.Close();
            MessageBox.Show("Завершен.");

        }

        /// <summary>
        /// Ломаная.
        /// </summary>
        public void TestF9()
        {
            Int64 y, x, t;
            UInt64 ret;
            FileStream fs = new FileStream("Test9.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            for (UInt64 i = 1; i < 4294967296; i = i + 1000000)
            {
                x = (Int64)i;
                y = x *712+482;
                t = (y >> 4) + (x * x)-107;
                y = y & t;

                ret = (UInt64)y;
                sw.WriteLine(x.ToString() + " " + ret.ToString());
            }

            sw.Close();
            fs.Close();
            MessageBox.Show("Завершен.");

        }

        /// <summary>
        /// Формирования графика для функции обработки crc
        /// </summary>
        public void TestSFCrc()
        {
            Int32 a,b,c,d,t;
            

            FileStream fs = new FileStream("TestSFCrc.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            //Формирую некоторый секретный ряд, которой не просто придумать. Ряд был протестирован, "отбалды" использовать нельзя!
            for (int i = 5; i <60 ; i = i + 5)
            {
                a = -1;
                b = a*a*i;

                c = 2 * i - 1;
                d = (c-b) * (3*i);
                t = d & 0xff;//Обрезаю байт.

                sw.WriteLine(i.ToString() + " " + t.ToString());
            }

            sw.Close();
            fs.Close();
            MessageBox.Show("Завершен.");

        }


        void GenerateAll()
        {
            TestF8();
            TestF8();
            TestF8();
            TestF8();
            TestF8();
            TestF8();
            TestF8();
            TestF8();
            TestF8();

        }

    }
}
