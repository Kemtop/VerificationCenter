using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DLL_API_Example
{
    public partial class FormPsw : Form
    {
        string pswd = "";

      public string  getPsw()
        {
            return pswd;
        }

        public FormPsw()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

            string psw = textBoxPsw.Text;
            if(String.IsNullOrEmpty(psw))
            {
                MessageBox.Show("Пароль не может быть пустой.", "Внимание.",
                      MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (psw.Length<8)
            {
                MessageBox.Show("Пароль не может содержать менее 8 ми символов.", "Внимание.",
                      MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            pswd = psw;

            DialogResult = DialogResult.OK;
        }
    }
}
