using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;
using System.IO.Ports;
using ZedGraph;
using ShimmerAPI;
using ShimmerLibrary;

namespace ShimmerAPI
{
    public partial class Form1 : Form
    {
        public static double gsr;
        public int i = 0;
        public static int counter = 0;
        public static double timestamp;
        
        public static double [] timestampArray;
        public static double temp;
        
        public static string datafinal, califinal;
        public static List<String> gsrList = new List<String>();
        public static List<String> timeList = new List<String>();
        // public static double [] gsrValue= new double[1000000];
        //public static double [] gsrTime= new double[1000000];
        System.Windows.Forms.Timer MyTimer = new System.Windows.Forms.Timer();
        //TextWriter _writer = null;
        //public Boolean tempState;
        public Form1()
        {
            InitializeComponent();
            

        }
        //DataTable dt = new DataTable();
        //DataRow dr;
        Control obj1 = new Control();
        //private string filePath;
        //private readonly bool tempState;

        public Stream path { get; private set; }

        public void button1_Click(object sender, EventArgs e)
        {

            //this.Hide();
            //this.Show();
            obj1.Show();
            
            obj1.ControlForm_Load(sender, e);
            obj1.buttonReload_Click(sender, e);
            obj1.Connect();
            textBox2.Text = "Connecting";

            //if (Control.tempvalue == true)
            MyTimer.Interval = (7 * 1000); // 45 mins
            MyTimer.Tick += new EventHandler(MyTimer_Tick1);
            MyTimer.Start();
            //Control.Hide();
        }
        public void MyTimer_Tick1(object sender, EventArgs e)
        {
            if(Control.tempvalue == true)
            {
                textBox2.Text = "Connected";
                ShimmerPlugin.Global.statusVariable = 1;
            }
            else
            {
                textBox2.Text = "Not Connected";
            }

            //MessageBox.Show("The form will now be closed.", "Time Elapsed");

            // this.Close();
        }

        public void button2_Click(object sender, EventArgs e)
        {
            //Control obj2 = new Control();
            obj1.buttonStream_Click(sender, e);
            //  dataGridView1.Rows.Add();
            //dataGridView1.Rows[0].Cells[0].Value = Form1.temp;

            MyTimer.Interval = (1 * 1000); // 45 mins
            MyTimer.Tick += new EventHandler(MyTimer_Tick);
            MyTimer.Start();
       
        }
        public void MyTimer_Tick(object sender, EventArgs e)
        {
            ShimmerPlugin.Global.globalGSR = califinal;
            textBox6.Text = datafinal;
            textBox4.Text = califinal;
            //GsrValue = new string [califinal];
            //MessageBox.Show("The form will now be closed.", "Time Elapsed");
            MyTimer.Start();
           // this.Close();
        }
      
        public static void calibGsrData(double data)
        {
            
            
            //  gsrArray[counter] = data;
            
            //Console.WriteLine("CalibValue =" + data);
            //gsrValue[counter] = data;
            califinal = data.ToString();

            ShimmerPlugin.Global.globalGSR = data.ToString();
            //MessageBox.Show(ShimmerPlugin.Global.globalGSR);
            ShimmerPlugin.ShimmerPlugin.sendData(califinal);
            gsrList.Add(califinal);
         
            //counter++;
            //  _writer = new TextBoxStreamWriter(textBox6);
            // Redirect the out Console stream
            //   Console.SetOut(_writer);
           // Form1 formInst = new Form1();
           
            // Form1.temp = data;
           //this.textBox6.Text = data.ToString();
            //  formInst.display();
           // Thread.Sleep(10000);


        }
        //public void display()
        //{
        //   int n = dataGridView1.Rows.Add();
        // //textBox1.Text = Form1.temp.ToString();
        //// textBox1.Invoke(new Action(() => textBox1.Text = Form1.temp.ToString()));
        //   dataGridView1.Rows[n].Cells[i].Value = Form1.temp.ToString();
        //  //  i++;
        //}
        public static void calibTimeStamp(double data)
        {
            //  timestampArray[counter] = data;
            counter++;
            //double value = data;
            //System.Math.Round(data, 2);
           // gsrTime[counter] = data;
            //Console.WriteLine("CalibTimeStampValue =" + data);
            datafinal = data.ToString();
            timeList.Add(datafinal);

        }

        public void button3_Click(object sender, EventArgs e)
        {
            obj1.buttonStop_Click(sender,e);
            //Console.WriteLine(gsrValue[4]);
            for(int i=0; i< gsrList.Count; i++)
            {
                Console.WriteLine(gsrList[i]);
            }
            for (int i = 0; i < timeList.Count; i++)
            {
                Console.WriteLine(timeList[i]);
            }

        }

        public void button4_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog saveFile = new SaveFileDialog() { Filter = "CSV|*.csv" })
            { 
            if(saveFile.ShowDialog() == DialogResult.OK)
            {
                using(StreamWriter sw = new StreamWriter(new FileStream(saveFile.FileName,FileMode.Create), Encoding.UTF8))
                    {
                        StringBuilder sb = new StringBuilder();
                        //sb.AppendLine("Time,GSR");
                       for (int i = 0; i < gsrList.Count; i++)
                        {
                            sb.AppendLine(string.Join("{0}{1}", timeList[i],gsrList[i]));
                           sw.WriteLine(string.Join(",", timeList[i],gsrList[i]));

                        }
                        MessageBox.Show("File Created");
                    }
            }

            }
        }
       
        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
        
        }

        private void label2_Click_1(object sender, EventArgs e)
        {

        }

        private void label2_Click_2(object sender, EventArgs e)
        {

        }

        private void textBox6_TextChanged(object sender, EventArgs e)
        {

        }
    }
    
  
}
