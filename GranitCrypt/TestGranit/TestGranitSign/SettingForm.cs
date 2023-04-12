using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestGranitSign
{
    public partial class SettingForm : Form
    {
        public SettingForm()
        {
            InitializeComponent();
        }

        private void SettingForm_Load(object sender, EventArgs e)
        {
            TbServer.Text = Properties.Settings.Default.DbServer;
            TbBase.Text = Properties.Settings.Default.DbName;
            TbPath.Text = Properties.Settings.Default.BasePath;
        }

        private void SettingForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK)
            {
                Properties.Settings.Default.DbServer = TbServer.Text;
                Properties.Settings.Default.DbName = TbBase.Text;
                Properties.Settings.Default.BasePath = TbPath.Text;
                Properties.Settings.Default.Save();
            }
        }

        private void BtPath_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dial = new FolderBrowserDialog();
            if (!string.IsNullOrEmpty(TbPath.Text) && Directory.Exists(TbPath.Text))
            {
                dial.SelectedPath = TbPath.Text;
            }

            if (dial.ShowDialog() == DialogResult.OK)
            {
                TbPath.Text = dial.SelectedPath;
            }
        }
    }
}
