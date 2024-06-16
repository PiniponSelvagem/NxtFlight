using System.IO.Ports;
using System.Text;
using FSUIPC;

namespace NxtFlightServer {
    public partial class FormMain : Form {

        internal static string title = "NxtFlight Server";

        public FormMain() {
            InitializeComponent();
            this.Text = title;
            SerialPort();
            this.textFreqHigh.Text = "118";
            this.textFreqLow.Text = "000";
        }

        private void FormMain_Load(object sender, EventArgs e) {
            setConnectionStatus();
        }

        private void button1_Click(object sender, EventArgs e) {

        }

        // Main Timer ticks every 50ms - Set in the timer properties
        private void timerMain_Tick(object sender, EventArgs e) {
            try {
                FSUIPCConnection.Process();
            }
            catch {
                this.timerConnection.Start();
                this.timerMain.Stop();
            }

            // --------------------
            // AIRSPEED
            // --------------------
            // FSUIPC Documentation says this offset is 4 bytes (uint) and holds the speed as knots * 128
            // So we need to convert back to knots by / 128
            // Offset is 'uint' so cast to double for conversion.
            double airspeedKnots = (double)this.airspeed.Value / 128d;
            this.txtAirspeed.Text = airspeedKnots.ToString("F2");


            //this.txtAirspeed.Text = this.com1_standby.Value.ToString();
            //this.textCOM1standby.Text = this.com1_standby.Value.ToString(); //textBox1.Text + textBox2.Text;
            this.textCOM1standby.Text = this.com1_standby.Value.ToString();

            if (this.swapFreqCOM1.Value > 0)
                this.swapFreqCOM1.Value = 0;
        }


        private void timerConnection_Tick(object sender, EventArgs e) {
            // try to open the connection
            try {
                FSUIPCConnection.Open();
            }
            catch {
                // connection failed. No need to do anything, we just keep trying
            }
            setConnectionStatus();
            if (FSUIPCConnection.IsOpen) {
                // connection opened
                // stop the timer that looks for a connection
                this.timerConnection.Stop();
                // start the main timer
                this.timerMain.Start();
            }
        }

        private void setConnectionStatus() {
            // set the text of the status label depending on the connection status
            if (FSUIPCConnection.IsOpen) {
                this.button1.Text = "Connected to " + FSUIPCConnection.FlightSimVersionConnected.ToString();
                this.button1.ForeColor = Color.DarkGreen;
            }
            else {
                this.button1.Text = "Disconnected. Looking for Flight Sim...";
                this.button1.ForeColor = Color.Red;
            }
        }


        private Offset<uint> airspeed = new Offset<uint>(0x02BC);             // 4-byte offset - Unsigned integer 
        private Offset<int> com1_standby = new Offset<int>(0x05CC);







        private Offset<ushort> setQNH = new Offset<ushort>(0x0330);

        private bool isChangingFreqH = true;
        private Offset<byte> swapFreqCOM1 = new Offset<byte>(0x3123);


        // Create the serial port with basic settings
        private SerialPort port = new SerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
        private StringBuilder dataBuffer = new StringBuilder();

        private void SerialPort() {
            // Begin communications
            port.Open();

            // Attach a method to be called when there
            // is data waiting in the port's buffer
            port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
        }

        private void port_DataReceived(object sender, SerialDataReceivedEventArgs e) {
            // Read the available data
            string data = port.ReadExisting();

            // Append the data to the buffer
            dataBuffer.Append(data);

            // Check for complete lines
            processBuffer();
        }

        private void processBuffer() {
            while (true) {
                // Find the position of the newline character
                string buffer = dataBuffer.ToString();
                int newlineIndex = buffer.IndexOf('\n');

                if (newlineIndex >= 0) {
                    // Extract the complete line
                    string line = buffer.Substring(0, newlineIndex + 1);

                    // Remove the processed line from the buffer
                    dataBuffer.Remove(0, newlineIndex + 1);

                    // Update the UI with the complete line
                    line = line.Trim();
                    if (line.StartsWith("rA")) {
                        line = line.Substring(2);
                        this.Invoke(new MethodInvoker(delegate {
                            updateTextQNH(int.Parse(line));
                        }));
                    }
                    else if (line.StartsWith("rB")) {
                        line = line.Substring(2);
                        this.Invoke(new MethodInvoker(delegate {
                            updateTextFreq(int.Parse(line));
                        }));
                    }
                    else if (line.StartsWith("bA")) {
                        this.Invoke(new MethodInvoker(updateToggleSwitchQNH));
                    }
                    else if (line.StartsWith("bB")) {
                        this.Invoke(new MethodInvoker(updateChangeFreqCOM1));
                    }
                    else if (line.StartsWith("bC")) {
                        isChangingFreqH = !isChangingFreqH;
                    }
                }
                else {
                    // No complete line in the buffer
                    break;
                }
            }
        }

        private void updateTextFreq(int value) {
            int high = int.Parse(this.com1_standby.Value.ToString().Substring(0,3));
            int low  = int.Parse(this.com1_standby.Value.ToString().Substring(3,3));
            if (isChangingFreqH) {
                high += value;
                if (high < 118) { high =118; }
                else if (high > 136) { high = 136; }
            }
            else {
                low += value * 5;
                if (low < 0) { low = 990; }
                else if (low > 990) { low = 0; }
            }

            this.textFreqHigh.Text = high.ToString().PadLeft(3, '0');
            this.textFreqLow.Text = low.ToString().PadLeft(3, '0');
            this.com1_standby.Value = int.Parse(textFreqHigh.Text + textFreqLow.Text + "000");
        }
        private void updateChangeFreqCOM1() {
            this.swapFreqCOM1.Value = 0x8;
        }
        private void updateTextQNH(int value) {
            short mode = (short)FSUIPCConnection.ReadLVar("XMLVAR_Baro_Selector_HPA_1");
            if (mode == 0) {
                double qnh = ((double)setQNH.Value / 541.75274908363878707097634121959) + ((float)value/100);
                this.textQNH.Text = qnh.ToString();
                setQNH.Value = (ushort)(qnh * 541.75274908363878707097634121959);
            }
            else {
                ushort qnh = (ushort)((setQNH.Value/16) + value);
                this.textQNH.Text = qnh.ToString();
                setQNH.Value = (ushort)(qnh * 16);
            }
        }
        private void updateToggleSwitchQNH() {
            int mode = (int)FSUIPCConnection.ReadLVar("XMLVAR_Baro1_Mode");
            if (mode == 1) mode = 3;
            else           mode = 1;
            FSUIPCConnection.WriteLVar("XMLVAR_Baro1_Mode", mode);
        }
    }
}
