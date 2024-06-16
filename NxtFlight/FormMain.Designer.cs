namespace NxtFlightServer {
    partial class FormMain {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            components = new System.ComponentModel.Container();
            button1 = new Button();
            timerMain = new System.Windows.Forms.Timer(components);
            timerConnection = new System.Windows.Forms.Timer(components);
            txtAirspeed = new TextBox();
            textFreqHigh = new TextBox();
            textFreqLow = new TextBox();
            textCOM1standby = new TextBox();
            textQNH = new TextBox();
            SuspendLayout();
            // 
            // button1
            // 
            button1.Location = new Point(12, 12);
            button1.Name = "button1";
            button1.Size = new Size(75, 23);
            button1.TabIndex = 0;
            button1.Text = "button1";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // timerMain
            // 
            timerMain.Interval = 50;
            timerMain.Tick += timerMain_Tick;
            // 
            // timerConnection
            // 
            timerConnection.Enabled = true;
            timerConnection.Interval = 1000;
            timerConnection.Tick += timerConnection_Tick;
            // 
            // txtAirspeed
            // 
            txtAirspeed.Location = new Point(12, 41);
            txtAirspeed.Name = "txtAirspeed";
            txtAirspeed.Size = new Size(100, 23);
            txtAirspeed.TabIndex = 1;
            // 
            // textFreqHigh
            // 
            textFreqHigh.Location = new Point(12, 107);
            textFreqHigh.Name = "textFreqHigh";
            textFreqHigh.Size = new Size(100, 23);
            textFreqHigh.TabIndex = 2;
            // 
            // textFreqLow
            // 
            textFreqLow.Location = new Point(118, 107);
            textFreqLow.Name = "textFreqLow";
            textFreqLow.Size = new Size(100, 23);
            textFreqLow.TabIndex = 3;
            // 
            // textCOM1standby
            // 
            textCOM1standby.Location = new Point(12, 136);
            textCOM1standby.Name = "textCOM1standby";
            textCOM1standby.Size = new Size(206, 23);
            textCOM1standby.TabIndex = 4;
            // 
            // textQNH
            // 
            textQNH.Location = new Point(12, 206);
            textQNH.Name = "textQNH";
            textQNH.Size = new Size(100, 23);
            textQNH.TabIndex = 6;
            // 
            // FormMain
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(textQNH);
            Controls.Add(textCOM1standby);
            Controls.Add(textFreqLow);
            Controls.Add(textFreqHigh);
            Controls.Add(txtAirspeed);
            Controls.Add(button1);
            Name = "FormMain";
            Text = "Form1";
            Load += FormMain_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button button1;
        private System.Windows.Forms.Timer timerMain;
        private System.Windows.Forms.Timer timerConnection;
        private TextBox txtAirspeed;
        private TextBox textFreqHigh;
        private TextBox textFreqLow;
        private TextBox textCOM1standby;
        private TextBox textQNH;
    }
}
