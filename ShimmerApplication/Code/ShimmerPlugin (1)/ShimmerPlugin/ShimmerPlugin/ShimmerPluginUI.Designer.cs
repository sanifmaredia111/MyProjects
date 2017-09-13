namespace ShimmerPlugin
{
    partial class ShimmerPluginUI
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ShimmerPluginUI));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.Connect = new System.Windows.Forms.ToolStripButton();
            this.status = new System.Windows.Forms.Label();
            this.save = new System.Windows.Forms.Button();
            this.Disconnect = new System.Windows.Forms.ToolStrip();
            this.stopDiscon = new System.Windows.Forms.ToolStripButton();
            this.toolStrip1.SuspendLayout();
            this.Disconnect.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Connect});
            this.toolStrip1.Location = new System.Drawing.Point(78, 142);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(99, 27);
            this.toolStrip1.Stretch = true;
            this.toolStrip1.TabIndex = 5;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // Connect
            // 
            this.Connect.BackColor = System.Drawing.SystemColors.Info;
            this.Connect.Image = ((System.Drawing.Image)(resources.GetObject("Connect.Image")));
            this.Connect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.Connect.Name = "Connect";
            this.Connect.Size = new System.Drawing.Size(87, 24);
            this.Connect.Text = "Connect";
            this.Connect.Click += new System.EventHandler(this.Connect_Click);
            // 
            // status
            // 
            this.status.AutoSize = true;
            this.status.BackColor = System.Drawing.SystemColors.Info;
            this.status.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.status.Location = new System.Drawing.Point(221, 147);
            this.status.Margin = new System.Windows.Forms.Padding(4);
            this.status.Name = "status";
            this.status.Size = new System.Drawing.Size(101, 22);
            this.status.TabIndex = 6;
            this.status.Text = "   Status   ";
            this.status.Click += new System.EventHandler(this.status_Click);
            // 
            // save
            // 
            this.save.BackColor = System.Drawing.SystemColors.Info;
            this.save.Location = new System.Drawing.Point(221, 203);
            this.save.Margin = new System.Windows.Forms.Padding(4);
            this.save.Name = "save";
            this.save.Size = new System.Drawing.Size(111, 36);
            this.save.TabIndex = 8;
            this.save.Text = "Save";
            this.save.UseVisualStyleBackColor = false;
            this.save.Click += new System.EventHandler(this.button3_Click);
            // 
            // Disconnect
            // 
            this.Disconnect.Dock = System.Windows.Forms.DockStyle.None;
            this.Disconnect.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.Disconnect.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.stopDiscon});
            this.Disconnect.Location = new System.Drawing.Point(78, 176);
            this.Disconnect.Name = "Disconnect";
            this.Disconnect.Size = new System.Drawing.Size(118, 27);
            this.Disconnect.TabIndex = 9;
            this.Disconnect.Text = "Disocnnect";
            this.Disconnect.Visible = false;
            // 
            // stopDiscon
            // 
            this.stopDiscon.BackColor = System.Drawing.SystemColors.Info;
            this.stopDiscon.Image = ((System.Drawing.Image)(resources.GetObject("stopDiscon.Image")));
            this.stopDiscon.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.stopDiscon.Name = "stopDiscon";
            this.stopDiscon.Size = new System.Drawing.Size(106, 24);
            this.stopDiscon.Text = "Disconnect";
            this.stopDiscon.Click += new System.EventHandler(this.Disconnect_Click);
            // 
            // ShimmerPluginUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(11F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.Disconnect);
            this.Controls.Add(this.save);
            this.Controls.Add(this.status);
            this.Controls.Add(this.toolStrip1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.Name = "ShimmerPluginUI";
            this.Size = new System.Drawing.Size(609, 354);
            this.Load += new System.EventHandler(this.ShimmerPluginUI_Load);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.Disconnect.ResumeLayout(false);
            this.Disconnect.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton Connect;
        private System.Windows.Forms.Label status;
        public System.Windows.Forms.Button save;
        private System.Windows.Forms.ToolStrip Disconnect;
        private System.Windows.Forms.ToolStripButton stopDiscon;
    }
}
