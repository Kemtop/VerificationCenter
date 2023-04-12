namespace TestGranitSign
{
    partial class SettingForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.TbServer = new System.Windows.Forms.TextBox();
            this.TbBase = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.TbPath = new System.Windows.Forms.TextBox();
            this.BtPath = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.BtOk = new System.Windows.Forms.Button();
            this.BtCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "Сервер БД:";
            // 
            // TbServer
            // 
            this.TbServer.Location = new System.Drawing.Point(121, 6);
            this.TbServer.Name = "TbServer";
            this.TbServer.Size = new System.Drawing.Size(289, 23);
            this.TbServer.TabIndex = 1;
            // 
            // TbBase
            // 
            this.TbBase.Location = new System.Drawing.Point(121, 36);
            this.TbBase.Name = "TbBase";
            this.TbBase.Size = new System.Drawing.Size(289, 23);
            this.TbBase.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 39);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(63, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = "Имя БД:";
            // 
            // TbPath
            // 
            this.TbPath.Location = new System.Drawing.Point(121, 65);
            this.TbPath.Name = "TbPath";
            this.TbPath.Size = new System.Drawing.Size(252, 23);
            this.TbPath.TabIndex = 4;
            // 
            // BtPath
            // 
            this.BtPath.Location = new System.Drawing.Point(379, 65);
            this.BtPath.Name = "BtPath";
            this.BtPath.Size = new System.Drawing.Size(31, 23);
            this.BtPath.TabIndex = 5;
            this.BtPath.Text = "...";
            this.BtPath.UseVisualStyleBackColor = true;
            this.BtPath.Click += new System.EventHandler(this.BtPath_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 68);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(108, 17);
            this.label3.TabIndex = 6;
            this.label3.Text = "Путь к ключам:";
            // 
            // BtOk
            // 
            this.BtOk.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.BtOk.Location = new System.Drawing.Point(121, 102);
            this.BtOk.Name = "BtOk";
            this.BtOk.Size = new System.Drawing.Size(75, 23);
            this.BtOk.TabIndex = 7;
            this.BtOk.Text = "Ок";
            this.BtOk.UseVisualStyleBackColor = true;
            // 
            // BtCancel
            // 
            this.BtCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.BtCancel.Location = new System.Drawing.Point(240, 102);
            this.BtCancel.Name = "BtCancel";
            this.BtCancel.Size = new System.Drawing.Size(75, 23);
            this.BtCancel.TabIndex = 8;
            this.BtCancel.Text = "Отмена";
            this.BtCancel.UseVisualStyleBackColor = true;
            // 
            // SettingForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(422, 137);
            this.Controls.Add(this.BtCancel);
            this.Controls.Add(this.BtOk);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.BtPath);
            this.Controls.Add(this.TbPath);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.TbBase);
            this.Controls.Add(this.TbServer);
            this.Controls.Add(this.label1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SettingForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Настройки";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SettingForm_FormClosing);
            this.Load += new System.EventHandler(this.SettingForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox TbServer;
        private System.Windows.Forms.TextBox TbBase;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox TbPath;
        private System.Windows.Forms.Button BtPath;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button BtOk;
        private System.Windows.Forms.Button BtCancel;
    }
}