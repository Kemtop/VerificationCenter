namespace TestGranit
{
    partial class MainForm
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.CmbTable = new System.Windows.Forms.ComboBox();
            this.BtnStart = new System.Windows.Forms.Button();
            this.CbDisDebug = new System.Windows.Forms.CheckBox();
            this.TBLog = new System.Windows.Forms.RichTextBox();
            this.DGVUsers = new System.Windows.Forms.DataGridView();
            this.clId = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.clUser = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.clCol = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.программаToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SettToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ExitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.WaitPanel = new System.Windows.Forms.Panel();
            this.PgBWait = new System.Windows.Forms.ProgressBar();
            this.LblWait = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DGVUsers)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.WaitPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.CmbTable);
            this.panel1.Controls.Add(this.BtnStart);
            this.panel1.Controls.Add(this.CbDisDebug);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 542);
            this.panel1.Margin = new System.Windows.Forms.Padding(4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1067, 95);
            this.panel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(73, 34);
            this.label1.TabIndex = 3;
            this.label1.Text = "Таблица\r\nдля теста";
            // 
            // CmbTable
            // 
            this.CmbTable.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CmbTable.FormattingEnabled = true;
            this.CmbTable.Location = new System.Drawing.Point(100, 14);
            this.CmbTable.Name = "CmbTable";
            this.CmbTable.Size = new System.Drawing.Size(133, 24);
            this.CmbTable.TabIndex = 2;
            // 
            // BtnStart
            // 
            this.BtnStart.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.BtnStart.Location = new System.Drawing.Point(461, 38);
            this.BtnStart.Margin = new System.Windows.Forms.Padding(4);
            this.BtnStart.Name = "BtnStart";
            this.BtnStart.Size = new System.Drawing.Size(151, 42);
            this.BtnStart.TabIndex = 1;
            this.BtnStart.Text = "Начать тест!";
            this.BtnStart.UseVisualStyleBackColor = true;
            this.BtnStart.Click += new System.EventHandler(this.BtnStart_Click);
            // 
            // CbDisDebug
            // 
            this.CbDisDebug.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.CbDisDebug.AutoSize = true;
            this.CbDisDebug.Checked = true;
            this.CbDisDebug.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CbDisDebug.Location = new System.Drawing.Point(951, 7);
            this.CbDisDebug.Margin = new System.Windows.Forms.Padding(4);
            this.CbDisDebug.Name = "CbDisDebug";
            this.CbDisDebug.Size = new System.Drawing.Size(100, 38);
            this.CbDisDebug.TabIndex = 0;
            this.CbDisDebug.Text = "Отключить\r\nотладку";
            this.CbDisDebug.UseVisualStyleBackColor = true;
            // 
            // TBLog
            // 
            this.TBLog.Dock = System.Windows.Forms.DockStyle.Right;
            this.TBLog.Location = new System.Drawing.Point(527, 24);
            this.TBLog.Margin = new System.Windows.Forms.Padding(4);
            this.TBLog.Name = "TBLog";
            this.TBLog.ReadOnly = true;
            this.TBLog.Size = new System.Drawing.Size(540, 518);
            this.TBLog.TabIndex = 1;
            this.TBLog.Text = "";
            // 
            // DGVUsers
            // 
            this.DGVUsers.AllowUserToAddRows = false;
            this.DGVUsers.AllowUserToDeleteRows = false;
            this.DGVUsers.AllowUserToResizeColumns = false;
            this.DGVUsers.AllowUserToResizeRows = false;
            this.DGVUsers.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DGVUsers.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.clId,
            this.clUser,
            this.clCol});
            this.DGVUsers.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DGVUsers.Location = new System.Drawing.Point(0, 24);
            this.DGVUsers.Margin = new System.Windows.Forms.Padding(4);
            this.DGVUsers.Name = "DGVUsers";
            this.DGVUsers.ReadOnly = true;
            this.DGVUsers.RowHeadersWidth = 55;
            this.DGVUsers.Size = new System.Drawing.Size(527, 518);
            this.DGVUsers.TabIndex = 2;
            // 
            // clId
            // 
            this.clId.HeaderText = "Id";
            this.clId.Name = "clId";
            this.clId.ReadOnly = true;
            this.clId.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.clId.Width = 50;
            // 
            // clUser
            // 
            this.clUser.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.clUser.HeaderText = "Пользователь";
            this.clUser.Name = "clUser";
            this.clUser.ReadOnly = true;
            this.clUser.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // clCol
            // 
            this.clCol.HeaderText = "Кол-во записей";
            this.clCol.Name = "clCol";
            this.clCol.ReadOnly = true;
            this.clCol.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.программаToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(8, 2, 0, 2);
            this.menuStrip1.Size = new System.Drawing.Size(1067, 24);
            this.menuStrip1.TabIndex = 3;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // программаToolStripMenuItem
            // 
            this.программаToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.SettToolStripMenuItem,
            this.ExitToolStripMenuItem});
            this.программаToolStripMenuItem.Name = "программаToolStripMenuItem";
            this.программаToolStripMenuItem.Size = new System.Drawing.Size(84, 20);
            this.программаToolStripMenuItem.Text = "Программа";
            // 
            // SettToolStripMenuItem
            // 
            this.SettToolStripMenuItem.Name = "SettToolStripMenuItem";
            this.SettToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.SettToolStripMenuItem.Text = "Настройки";
            this.SettToolStripMenuItem.Click += new System.EventHandler(this.SettToolStripMenuItem_Click);
            // 
            // ExitToolStripMenuItem
            // 
            this.ExitToolStripMenuItem.Name = "ExitToolStripMenuItem";
            this.ExitToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.ExitToolStripMenuItem.Text = "Выход";
            this.ExitToolStripMenuItem.Click += new System.EventHandler(this.ExitToolStripMenuItem_Click);
            // 
            // WaitPanel
            // 
            this.WaitPanel.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.WaitPanel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.WaitPanel.Controls.Add(this.PgBWait);
            this.WaitPanel.Controls.Add(this.LblWait);
            this.WaitPanel.Controls.Add(this.pictureBox1);
            this.WaitPanel.Location = new System.Drawing.Point(276, 226);
            this.WaitPanel.Name = "WaitPanel";
            this.WaitPanel.Size = new System.Drawing.Size(501, 92);
            this.WaitPanel.TabIndex = 4;
            this.WaitPanel.Visible = false;
            // 
            // PgBWait
            // 
            this.PgBWait.Location = new System.Drawing.Point(88, 58);
            this.PgBWait.Name = "PgBWait";
            this.PgBWait.Size = new System.Drawing.Size(399, 20);
            this.PgBWait.TabIndex = 2;
            // 
            // LblWait
            // 
            this.LblWait.AutoSize = true;
            this.LblWait.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.LblWait.Location = new System.Drawing.Point(85, 29);
            this.LblWait.Name = "LblWait";
            this.LblWait.Size = new System.Drawing.Size(52, 17);
            this.LblWait.TabIndex = 1;
            this.LblWait.Text = "label1";
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::TestGranit.Properties.Resources.icon_wait;
            this.pictureBox1.Location = new System.Drawing.Point(13, 12);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(66, 66);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1067, 637);
            this.Controls.Add(this.WaitPanel);
            this.Controls.Add(this.DGVUsers);
            this.Controls.Add(this.TBLog);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.menuStrip1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Тест подписи";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.Shown += new System.EventHandler(this.MainForm_Shown);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DGVUsers)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.WaitPanel.ResumeLayout(false);
            this.WaitPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.RichTextBox TBLog;
        private System.Windows.Forms.DataGridView DGVUsers;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem программаToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem SettToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ExitToolStripMenuItem;
        private System.Windows.Forms.DataGridViewTextBoxColumn clId;
        private System.Windows.Forms.DataGridViewTextBoxColumn clUser;
        private System.Windows.Forms.DataGridViewTextBoxColumn clCol;
        private System.Windows.Forms.CheckBox CbDisDebug;
        private System.Windows.Forms.Button BtnStart;
        private System.Windows.Forms.ComboBox CmbTable;
        private System.Windows.Forms.Panel WaitPanel;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label LblWait;
        private System.Windows.Forms.ProgressBar PgBWait;
        private System.Windows.Forms.Label label1;
    }
}

