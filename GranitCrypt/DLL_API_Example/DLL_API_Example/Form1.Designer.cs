namespace DLL_API_Example
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.button1 = new System.Windows.Forms.Button();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.buttonPathSk = new System.Windows.Forms.Button();
            this.buttonPathDb = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.buttonPswd = new System.Windows.Forms.Button();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(243, 410);
            this.button1.Margin = new System.Windows.Forms.Padding(4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(307, 32);
            this.button1.TabIndex = 0;
            this.button1.Text = "Демонстрация.";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(18, 17);
            this.richTextBox1.Margin = new System.Windows.Forms.Padding(4);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(753, 311);
            this.richTextBox1.TabIndex = 1;
            this.richTextBox1.Text = "";
            // 
            // buttonPathSk
            // 
            this.buttonPathSk.Location = new System.Drawing.Point(18, 342);
            this.buttonPathSk.Name = "buttonPathSk";
            this.buttonPathSk.Size = new System.Drawing.Size(224, 32);
            this.buttonPathSk.TabIndex = 2;
            this.buttonPathSk.Text = "Изменить путь к ключу";
            this.buttonPathSk.UseVisualStyleBackColor = true;
            this.buttonPathSk.Click += new System.EventHandler(this.buttonPathSk_Click);
            // 
            // buttonPathDb
            // 
            this.buttonPathDb.Location = new System.Drawing.Point(547, 342);
            this.buttonPathDb.Name = "buttonPathDb";
            this.buttonPathDb.Size = new System.Drawing.Size(224, 32);
            this.buttonPathDb.TabIndex = 3;
            this.buttonPathDb.Text = "Изменить путь к БД \"Гранит\"";
            this.buttonPathDb.UseVisualStyleBackColor = true;
            this.buttonPathDb.Click += new System.EventHandler(this.buttonPathDb_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // buttonPswd
            // 
            this.buttonPswd.Location = new System.Drawing.Point(292, 342);
            this.buttonPswd.Name = "buttonPswd";
            this.buttonPswd.Size = new System.Drawing.Size(224, 32);
            this.buttonPswd.TabIndex = 4;
            this.buttonPswd.Text = "Ввести пароль СК";
            this.buttonPswd.UseVisualStyleBackColor = true;
            this.buttonPswd.Click += new System.EventHandler(this.buttonPswd_Click);
            // 
            // backgroundWorker1
            // 
            this.backgroundWorker1.WorkerReportsProgress = true;
            this.backgroundWorker1.WorkerSupportsCancellation = true;
            this.backgroundWorker1.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker1_DoWork);
            this.backgroundWorker1.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker1_ProgressChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 455);
            this.Controls.Add(this.buttonPswd);
            this.Controls.Add(this.buttonPathDb);
            this.Controls.Add(this.buttonPathSk);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.button1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Демонстрация работы LibGranitK.";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button buttonPathSk;
        private System.Windows.Forms.Button buttonPathDb;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button buttonPswd;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
    }
}

