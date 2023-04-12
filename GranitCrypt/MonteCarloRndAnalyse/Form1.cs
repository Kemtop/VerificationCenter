using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestGenerator
{
    public partial class Form1 : Form
    {
        private BitArray bits;

        public Form1()
        {
            InitializeComponent();
            label1.Text = label2.Text = label3.Text = label4.Text = "";
            label6.Visible = false;
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == (Keys.Control | Keys.C))
            {
                if (bits == null)
                {
                    MessageBox.Show("Нечего копировать");
                    return true;
                }
                if (string.IsNullOrEmpty(label4.Text))
                {
                    MessageBox.Show("Расчет еще не завершен");
                    return true;
                }
                Clipboard.SetText(label2.Text.Substring(4) + "|" + label3.Text.Substring(4) + "|" + label4.Text.Substring(4) + "|" + BitsUP.Text);
                return true;
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void Form1_DoubleClick(object sender, EventArgs e)
        {
            FileDialog dial = new OpenFileDialog();
            dial.Filter = "Данные выборки (*.bin)|*.bin";
            if (dial.ShowDialog() != DialogResult.OK) return;
            byte[] data;
            using (Stream fs = File.OpenRead(dial.FileName))
            {
                data = new byte[fs.Length];
                fs.Read(data, 0, data.Length);
            }
            bits = new BitArray(data);

            Clean();

            GenChart1();
            GenChart2();
            if (BitsUP.Text != "8")
                BitsUP.SelectedIndex = 0;
            else
                GenChart3();
        }

        private void Clean()
        {
            label1.Text = label2.Text = label3.Text = label4.Text = "";
            label6.Visible = false;
            chart1.Series[0].Points.Clear();
            pictureBox1.Image = new Bitmap(1,1);
            pictureBox2.Image = new Bitmap(1, 1);
            Application.DoEvents();
        }

        private void GenChart1()
        {
            chart1.Series[0].Points.Clear();
            if (bits == null) return;
            int N = (int)Math.Pow(10, (double)PowUD.Value);
            Dictionary<int, float> blocks = new Dictionary<int, float>();
            for (int i = 0; i < ((int)(bits.Length / N)) + 1; ++i)
            {
                int countBlock = (i + 1) * N > bits.Length ? bits.Length - i * N : N;
                int numberOnes = 0;
                for (int j = i * N; j < (i * N + countBlock); ++j)
                {
                    if (bits[j]) numberOnes++;
                }
                blocks.Add(i, ((numberOnes - (float)countBlock / 2) / (float)countBlock / 2) * 100);
            }
            chart1.Series[0].Points.DataBindY(blocks.Values);
            Application.DoEvents();
        }

        private void GenChart2()
        {
            pictureBox1.Image = new Bitmap(1, 1);
            if (bits == null) return;
            int sizeBitMap = (int)Math.Sqrt(bits.Length);
            Bitmap bitmap = new Bitmap(sizeBitMap, sizeBitMap);
            for (int i = 0; i < sizeBitMap; ++i)
            {
                for (int j=0; j < sizeBitMap; ++j)
                {
                    Color bit = bits[i * sizeBitMap + j] ? Color.Black : Color.White;
                    bitmap.SetPixel(i, j, bit);
                }
            }
            pictureBox1.Image = bitmap;
            Application.DoEvents();
        }

        private void GenChart3()
        {
            pictureBox2.Image = new Bitmap(1, 1);
            label1.Text = label2.Text = label3.Text = label4.Text = "";
            label6.Visible = false;
            if (bits == null) return;
            int Resolution = Math.Min(pictureBox2.Size.Width, pictureBox2.Size.Height);
            int colBits = int.Parse(BitsUP.Text);
            double R = Math.Pow(2, colBits);
            int dotsCount = (int)(bits.Length / (2 * colBits));
            label1.Text = "Выборка: " + dotsCount * 2 * colBits + " бит";
            Bitmap bitmap = new Bitmap(Resolution+1, Resolution+1);
            //for (int i = 0; i < Resolution; ++i)
            //{
            //    int y = (int)Math.Sqrt(Resolution * Resolution - i * i);
            //    bitmap.SetPixel(i, Resolution - y, Color.Black);
            //}

            int dotsIn = 0;
            for (int i = 0; i < dotsCount; ++i)
            {
                Color color;
                double x = 0, y = 0;
                for (int j = 0; j < colBits; ++j) x += bits[2 * colBits * i + j] ? Math.Pow(2, j) : 0;
                for (int j = 0; j < colBits; ++j) y += bits[2 * colBits * i + colBits + j] ? Math.Pow(2, j) : 0;

                if ((x * x + y * y) <= R*R)
                {
                    dotsIn++;
                    color = Color.Red;
                }
                else
                {
                    color = Color.Blue;
                }

                bitmap.SetPixel((int)(x * Resolution / R), Resolution - (int)(y * Resolution / R), color);
            }

            double q = (double)dotsIn / (double)dotsCount;
            label2.Text = "q = " + q;

            double Pi = 4 * q;
            label3.Text = "π = " + Pi;

            double e = Math.Abs(Pi - Math.PI) / Math.PI * 100;
            label4.Text = "e = " + e + "%";

            label6.Visible = true;

            pictureBox2.Image = bitmap;
            Application.DoEvents();
        }

        private void PowUD_ValueChanged(object sender, EventArgs e)
        {
            GenChart1();
        }

        private void BitsUP_ValueChanged(object sender, EventArgs e)
        {
            GenChart3();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            pictureBox1.SizeMode = pictureBox1.SizeMode == PictureBoxSizeMode.StretchImage ? PictureBoxSizeMode.Normal : PictureBoxSizeMode.StretchImage;
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            pictureBox2.SizeMode = pictureBox2.SizeMode == PictureBoxSizeMode.StretchImage ? PictureBoxSizeMode.Normal : PictureBoxSizeMode.StretchImage;
        }
    }
}
