using System;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;

namespace YuB_X_Interface
{
    public partial class YuB_X_Interface : Form
    {
        public YuB_X_Interface()
        {
            InitializeComponent();
        }

        private void YuB_X_Interface_Load(object sender, EventArgs e)
        {

        }

        private void Execute_Click(object sender, EventArgs e)
        {
            try
            {
                byte[] ScriptBytes = Encoding.UTF8.GetBytes(ScriptEditor.Text);
                int ScriptLength = ScriptBytes.Length;

                byte[] LengthBytes = BitConverter.GetBytes(ScriptLength);
                if (BitConverter.IsLittleEndian)
                    Array.Reverse(LengthBytes);

                using (TcpClient Client = new TcpClient())
                {
                    Client.Connect("127.0.0.1", 6969);
                    using (NetworkStream Stream = Client.GetStream())
                    {
                        Stream.Write(LengthBytes, 0, 4);
                        Stream.Write(ScriptBytes, 0, ScriptBytes.Length);
                        Stream.Flush();
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to Execute script: " + ex.Message, "YuB-X-Interface", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private async void Inject_Click(object sender, EventArgs e)
        {
            string InjectorPath = System.IO.Path.Combine(Application.StartupPath, "YuB-X-Injector.exe");

            if (!System.IO.File.Exists(InjectorPath))
            {
                MessageBox.Show("Press OK to download the injector, this might take a few seconds", "YuB-X-Interface", MessageBoxButtons.OK, MessageBoxIcon.Information);
                try
                {
                    using (var HttpClient = new System.Net.Http.HttpClient())
                    {
                        byte[] Data = await HttpClient.GetByteArrayAsync("https://github.com/vuxqzofx/YuB-X-Injector/raw/refs/heads/main/YuB-X-Injector-V5.exe");
                        System.IO.File.WriteAllBytes(InjectorPath, Data);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Failed to download injector: " + ex.Message, "YuB-X-Interface", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }

            System.Diagnostics.Process.Start(new System.Diagnostics.ProcessStartInfo(InjectorPath) { UseShellExecute = true });
        }
    }
}
