using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PluginInterface;
using System.Collections;
using System.Security.Permissions;
using System.Runtime.Serialization;
using System.Windows.Forms;
using System.Timers;




namespace ShimmerPlugin
{
    [Serializable]
    public class ShimmerPlugin : IPlugin, ISerializable
    {
        private static System.Timers.Timer aTimer;
        string myName = "Shimmer GSR";
        string myDescription = "Reads GSR data from Shimmer sensor";
        string myAuthor = "Gryffindor";
        string myVersion = "1.0.0";
        IPluginHost myHost = null;
        int myID = -1;
        System.Windows.Forms.UserControl myMainInterface;
        ArrayList inPins = new ArrayList();
        ArrayList outPins = new ArrayList();
        IPin processInitiatorInputPin = null;
        IPin filenameInputPin = null;
        IPin gsrDataOutputPin = null;
        System.Windows.Forms.Timer MyTimer2 = new System.Windows.Forms.Timer();

        public static String var1 = "";
       //// public static string datafinal, califinal;

        public string Description
        {
            get { return myDescription; }
        }

        public string Author
        {
            get { return myAuthor; }
        }

        public IPluginHost Host
        {
            get { return myHost; }
            set { myHost = value; }
        }

        public int MyID
        {
            get { return myID; }
            set { myID = value; }
        }

        public string Name
        {
            get { return myName; }
        }

        public System.Windows.Forms.UserControl MainInterface
        {
            get { return myMainInterface; }
        }

        public string Version
        {
            get { return myVersion; }
        }

        public ArrayList InputPins
        {
            get { return inPins; }
        }

        public ArrayList OutputPins
        {
            get { return outPins; }
        }

        public void Initialize()
        {
            //This is the first Function called by the host...
            processInitiatorInputPin = Host.LoadOrCreatePin("Process Initiator", PinCategory.Optional, new Type[] { typeof(IIntegerData) });
            inPins.Add(processInitiatorInputPin);

            filenameInputPin = Host.LoadOrCreatePin("File Name", PinCategory.Optional, new Type[] { typeof(IStringData) });
            inPins.Add(filenameInputPin);

            gsrDataOutputPin = Host.LoadOrCreatePin("GSR Data", PinCategory.Optional, new Type[] { typeof(IFloatData) });
            outPins.Add(gsrDataOutputPin);

            Global.myForm.button2_Click(new object(), new EventArgs());
            myMainInterface = new ShimmerPluginUI(this);

        }


        public void Dispose()
        {
            //Put any cleanup code in here for when the program is stopped
        }

        public void Process(IPin pin, IPinData input)
        {
            //Form1 Obj2 = new Form1();
            //Put process code here
            //Console.WriteLine("Hello");
            //   this.Host.SignalCriticalProcessingIsFinished(inImage, this);

             // perform streaming

            if (pin == processInitiatorInputPin)
            {
                if (input != null)
                {

                    //code to process the input pin...
                    int trigger = ((IIntegerData)input).Value;

                    if (trigger == 1)
                    {
                        SetTimer();
                        Stream();
                        
                        //aTimer.Dispose();
                        //MyTimer2.Interval = (1 * 1000); // 45 mins
                        //MyTimer2.Tick += new EventHandler(MyTimer2_Tick);
                        //MyTimer2.Start();//myTimer Logic


                        //Suppose you are ready to send output on output pin

                    }
                    if (trigger == 0)
                    {
                        //myTimer Logic
                        aTimer.Stop();
                        aTimer.Dispose();
                        Global.myForm.button3_Click(new object(), new EventArgs());
                        
                        //Suppose you are ready to send output on output pin

                    }

                }
            }

            //   this.Host.SignalCriticalProcessingIsFinished(inImage, this);
            if (pin == filenameInputPin)
            {
                if (input != null)
                {
                    //code to process the filenameInputPin...
                    string fileName = ((IStringData)input).Data;

                }
            }

        }
        public void Stream() // form streaming
        {

            Global.myForm.button2_Click(new object(), new EventArgs());

           
            //ShimmerAPI.Form1.calibGsrData(double data1);
            //  MyTimer2.Interval = (1 * 1000); // 45 mins
            //MyTimer2.Tick += new EventHandler(MyTimer_Tick);
            //MyTimer2.Start();

        }
        public static void sendData(String s)
        {
            var1 = s;
            //MessageBox.Show(var1);
            //Console.WriteLine(var1);
            //Stream();
            //this.Host.SendData(gsrDataOutputPin, new FloatData(x), this);
        }
        public void MyTimer2_Tick(object sender, EventArgs e)
        {
            //Console.WriteLine(Global.globalGSR);
            ////float x = float.Parse("Global.globalGSR");
            //float x = float.Parse(var1);
            //MessageBox.Show(x.ToString());
            ////Console.WriteLine(x);

            //this.Host.SendData(gsrDataOutputPin, new FloatData(x), this);
        }
        private void SetTimer()
        {
            // Create a timer with a two second interval.
            aTimer = new System.Timers.Timer(2000);
            // Hook up the Elapsed event for the timer. 
            aTimer.Elapsed += OnTimedEvent;
            aTimer.AutoReset = true;
            aTimer.Enabled = true;
        }
        private void OnTimedEvent(Object source, ElapsedEventArgs e)
        {
           // double x = double.Parse(var1);
           double x = Convert.ToDouble(var1);
           double y = x/100000;
           float z = (float)y;
            this.Host.SendData(gsrDataOutputPin, new FloatData(z), this);
            Console.WriteLine("The Elapsed event was raised at {0:HH:mm:ss.fff}",
                              e.SignalTime);
        }
        ///////////////////////////////////////////////////////////////////////////////////////////
        #region ISerializable Members
        ///////////////////////////////////////////////////////////////////////////////////////////

        public ShimmerPlugin()
        {
            //
            // TODO: Add constructor logic here
            //
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        public ShimmerPlugin(SerializationInfo info, StreamingContext context)
        ///////////////////////////////////////////////////////////////////////////////////////////
        {


        }

        [SecurityPermissionAttribute(SecurityAction.Demand, SerializationFormatter = true)]
        ///////////////////////////////////////////////////////////////////////////////////////////
        void ISerializable.GetObjectData(SerializationInfo info, StreamingContext context)
        ///////////////////////////////////////////////////////////////////////////////////////////
        {

        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        #endregion
        ///////////////////////////////////////////////////////////////////////////////////////////

    }
}
