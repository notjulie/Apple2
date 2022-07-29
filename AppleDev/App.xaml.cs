using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace AppleDev
{
   /// <summary>
   /// Interaction logic for App.xaml
   /// </summary>
   [System.Diagnostics.CodeAnalysis.SuppressMessage("Design", "CA1001:Types that own disposable fields should be disposable", Justification = "<Pending>")]
   public partial class App : Application
   {
      private const string PipeName = "AppleDevNamedPipe";
      private const string ProgramFileTooShortMessage = "Program file too short";
      private Thread mainWindowThread;
      private DispatcherTimer timer = new DispatcherTimer();
      private NamedPipeServerStream pipeServer;

      /// <summary>
      /// Performs actions on application startup
      /// </summary>
      /// <param name="e"></param>
      protected override void OnStartup(StartupEventArgs e)
      {
         // call base class
         base.OnStartup(e);

         // try registering ourselves as the pipe server
         BeginPipeListener();

         // nab the command line arguments
         string[] commandLineArgs = Environment.GetCommandLineArgs();

         // if we have a command line argument and we are not the server, try to send the command
         // line to the actual server to handle it instead of doing it ourself
         if (commandLineArgs.Length > 1 && pipeServer==null)
         {
            try
            {
               using (var clientStream = new NamedPipeClientStream(".", PipeName, PipeDirection.Out, PipeOptions.None))
               {
                  clientStream.Connect(100);
                  using (var pipeClient = new StreamWriter(clientStream))
                  {
                     pipeClient.Write(commandLineArgs[1]);
                  }
               }
               this.Shutdown();
               return;
            }
            catch (IOException x)
            {
               MessageBox.Show(x.Message);
            }
            catch (Exception x)
            {
               MessageBox.Show(x.GetType().Name);
               MessageBox.Show(x.Message);
               throw;
            }
         }

         // launch the thread that runs AppleWin
         mainWindowThread = new Thread(AppleWin.Managed.AppleWinThread.Run);
         mainWindowThread.Name = "AppleWin";
         mainWindowThread.SetApartmentState(ApartmentState.STA);
         mainWindowThread.Start();

         // show it
         AppleWin.Managed.AppleWinThread.Show(true);

         // if we have a single command line parameter it's a program to run
         if (commandLineArgs.Length == 2)
         {
            LoadAndRunFile(commandLineArgs[1]);
         }

         // start a timer to monitor if it closes
         timer.Interval = TimeSpan.FromMilliseconds(100);
         timer.Tick += Timer_Tick;
         timer.IsEnabled = true;
      }

      private void PipeConnectionReceived(IAsyncResult asyncResult)
      {
         if (pipeServer != null)
         {
            string message;
            pipeServer.EndWaitForConnection(asyncResult);
            using (var reader = new StreamReader(pipeServer))
               message = reader.ReadToEnd();

            BeginPipeListener();

            if (message != null)
               LoadAndRunFile(message);
         }
      }

      [System.Diagnostics.CodeAnalysis.SuppressMessage("Globalization", "CA1303:Do not pass literals as localized parameters", Justification = "<Pending>")]
      private static void LoadAndRunFile(string filename)
      {
         try
         {
            ushort address;
            List<byte> program = new List<byte>();
            using (FileStream file = new FileStream(filename, FileMode.Open, FileAccess.Read))
            {
               int lowByte = file.ReadByte();
               int highByte = file.ReadByte();
               if (lowByte < 0 || highByte < 0)
                  throw new IOException(ProgramFileTooShortMessage);
               address = (ushort)(lowByte + 256 * highByte);

               for (; ; )
               {
                  int b = file.ReadByte();
                  if (b < 0)
                     break;
                  program.Add((byte)b);
               }
            }

            AppleWin.Managed.Machine.ExecuteProgram(program.ToArray(), address);
         }
         catch (IOException x)
         {
            MessageBox.Show(x.Message);
         }
      }

      private void BeginPipeListener()
      {
         try
         {
            pipeServer = new NamedPipeServerStream(PipeName, PipeDirection.In, 1, PipeTransmissionMode.Byte, PipeOptions.Asynchronous);
            pipeServer.BeginWaitForConnection(
               PipeConnectionReceived,
               null
               );
         }
         catch (IOException)
         {
         }
      }

      private void Timer_Tick(object sender, EventArgs e)
      {
         if (!AppleWin.Managed.AppleWinThread.IsShowing())
            this.Shutdown();
      }

      /// <summary>
      /// Performs actions on application exit
      /// </summary>
      /// <param name="e"></param>
      protected override void OnExit(ExitEventArgs e)
      {
         // call the base class
         base.OnExit(e);

         // tell AppleWin to shut down
         AppleWin.Managed.AppleWinThread.Shutdown();
         mainWindowThread.Join();

         // save the default settings
         AppleDev.Properties.Settings.Default.Save();

         // shut down our pipe server
         if (pipeServer != null)
         {
            pipeServer.Dispose();
            pipeServer = null;
         }
      }
   }
}
