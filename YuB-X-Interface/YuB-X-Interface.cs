using System;
using System.Diagnostics;
using System.IO;
using System.Net.Http;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace YuB_X_Interface
{
    public partial class YuB_X_Interface : Form
    {
        private const string LoopbackHost = "127.0.0.1";
        private const int CommunicationPort = 6969;
        private const int TcpTimeoutMilliseconds = 5000;
        private const string InjectorFileName = "YuB-X-Injector.exe";
        private const string InjectorDownloadUrl = "https://github.com/vuxqzofx/YuB-X-Injector/raw/refs/heads/main/YuB-X-Injector-V5.exe";
        private static readonly HttpClient SharedHttpClient = CreateHttpClient();

        public YuB_X_Interface()
        {
            InitializeComponent();
        }

        private void YuB_X_Interface_Load(object sender, EventArgs e)
        {
        }

        private async void Execute_Click(object sender, EventArgs e)
        {
            string Script = ScriptEditor.Text;
            if (string.IsNullOrWhiteSpace(Script))
            {
                MessageBox.Show("Please enter a script before executing.", "YuB-X-Interface", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            SetUiBusy(true);

            try
            {
                await SendScriptAsync(Script);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to execute script: " + ex.Message, "YuB-X-Interface", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                SetUiBusy(false);
            }
        }

        private async void Inject_Click(object sender, EventArgs e)
        {
            string InjectorPath = Path.Combine(Application.StartupPath, InjectorFileName);
            SetUiBusy(true);

            try
            {
                if (!File.Exists(InjectorPath))
                {
                    MessageBox.Show("Press OK to download the injector, this might take a few seconds.", "YuB-X-Interface", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    await DownloadInjectorAsync(InjectorPath);
                }

                Process.Start(new ProcessStartInfo(InjectorPath) { UseShellExecute = true });
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to inject: " + ex.Message, "YuB-X-Interface", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                SetUiBusy(false);
            }
        }

        private static HttpClient CreateHttpClient()
        {
            var Client = new HttpClient();
            Client.Timeout = TimeSpan.FromSeconds(30);
            return Client;
        }

        private async Task SendScriptAsync(string Script)
        {
            byte[] ScriptBytes = Encoding.UTF8.GetBytes(Script);
            byte[] LengthBytes = BitConverter.GetBytes(ScriptBytes.Length);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(LengthBytes);
            }

            using (TcpClient Client = new TcpClient())
            {
                Task ConnectTask = Client.ConnectAsync(LoopbackHost, CommunicationPort);
                Task TimeoutTask = Task.Delay(TcpTimeoutMilliseconds);
                Task CompletedTask = await Task.WhenAny(ConnectTask, TimeoutTask);
                if (CompletedTask != ConnectTask)
                {
                    throw new TimeoutException("Connection to injector timed out.");
                }

                await ConnectTask;
                Client.SendTimeout = TcpTimeoutMilliseconds;
                Client.ReceiveTimeout = TcpTimeoutMilliseconds;

                using (NetworkStream Stream = Client.GetStream())
                {
                    await Stream.WriteAsync(LengthBytes, 0, LengthBytes.Length);
                    await Stream.WriteAsync(ScriptBytes, 0, ScriptBytes.Length);
                    await Stream.FlushAsync();
                }
            }
        }

        private static async Task DownloadInjectorAsync(string InjectorPath)
        {
            byte[] Data = await SharedHttpClient.GetByteArrayAsync(InjectorDownloadUrl);
            if (Data.Length == 0)
            {
                throw new InvalidDataException("Downloaded injector file is empty.");
            }

            File.WriteAllBytes(InjectorPath, Data);
        }

        private void SetUiBusy(bool IsBusy)
        {
            Execute.Enabled = !IsBusy;
            Inject.Enabled = !IsBusy;
            Cursor = IsBusy ? Cursors.WaitCursor : Cursors.Default;
        }
    }
}
