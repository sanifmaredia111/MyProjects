using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ShimmerAPI;
using PluginInterface;

namespace ShimmerPlugin
{
    public partial class ShimmerPluginUI : UserControl
    {
        ShimmerPlugin plugin;
        System.Windows.Forms.Timer MyTimer1 = new System.Windows.Forms.Timer();
        ShimmerAPI.Control obj = new ShimmerAPI.Control();
        public ShimmerPluginUI()
        {
            InitializeComponent();
           
            ShimmerPlugin SP = new ShimmerPlugin();


           
        }

        public ShimmerPluginUI(ShimmerPlugin setPlugin)
        {
            InitializeComponent();
            plugin = setPlugin;
        }
        //Form1 obj2 = new Form1();


        private void button1_Click(object sender, EventArgs e)
        {
           // plugin.MyTimer_Tick(sender,e);
            //Connect Logic
            //Global.myForm.button2_Click(sender,e);
           Global.myForm.button3_Click(new object(), new EventArgs());
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {

        }

        private void Connect_Click(object sender, EventArgs e)
        {
          
            status.Text = "Connecting";
            Global.myForm.button1_Click(new object(), new EventArgs());
            MyTimer1.Interval = (7 * 1000); // 45 mins
            MyTimer1.Tick += new EventHandler(status_Click);
            MyTimer1.Start();

            Connect.CheckOnClick = true;
            Connect.Enabled = false;
            toolStrip1.Visible = false;
            stopDiscon.Visible = true;
            Disconnect.Visible = true;
           
            
           
             
            //Connect.CheckedChanged += new_EventHandler(connnect_CheckedChanged);
        }
        

        private void ShimmerPluginUI_Load(object sender, EventArgs e)
        {

        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void status_Click(object sender, EventArgs e)
        {
            if (Global.statusVariable == 1)
            {
                status.Text = "Connected";
                stopDiscon.Enabled = true;
                stopDiscon.Visible = true;


            }
            else
            {
                status.Text = "Connecting...";
               
            }
        }
        

        private void button3_Click(object sender, EventArgs e)
        {
            Global.myForm.button4_Click(new object(), new EventArgs());

        }

        private void Disconnect_Click(object sender, EventArgs e)
        {

            ShimmerAPI.Control obj = new ShimmerAPI.Control();
            obj.buttonDisconnect_Click(sender, e);
            MyTimer1.Stop();
            status.Text = "Disconnect";
            //Connect.Enabled = true;
            //Connect.Visible = true;
            stopDiscon.CheckOnClick = true;
            Connect.Enabled = true;
            //Connect.Visible = true;
            stopDiscon.Visible = false;
            Disconnect.Visible = false;
            toolStrip1.Visible = true;
        }

        private void Discon_Click(object sender, EventArgs e)
        {
            //ShimmerAPI.Control obj = new ShimmerAPI.Control();
            //obj.buttonDisconnect_Click(sender, e);
            //MyTimer1.Stop();
            //status.Text = "Disconnect";
            ////Connect.Enabled = true;
            ////Connect.Visible = true;
           
            //Connect.Enabled = true;
            //Connect.Visible = true;
            //Discon.Visible = false;
        }
    }
}
