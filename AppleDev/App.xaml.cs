using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.IO;
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
   public partial class App : Application
   {
      private const string ProgramFileTooShortMessage = "Program file too short";
      private Thread mainWindowThread;
      private DispatcherTimer timer = new DispatcherTimer();


      /// <summary>
      /// Performs actions on application startup
      /// </summary>
      /// <param name="e"></param>
      [System.Diagnostics.CodeAnalysis.SuppressMessage("Globalization", "CA1303:Do not pass literals as localized parameters", Justification = "<Pending>")]
      protected override void OnStartup(StartupEventArgs e)
      {
         // call base class
         base.OnStartup(e);

         // launch the thread that runs AppleWin
         mainWindowThread = new Thread(AppleWin.Managed.AppleWinThread.Run);
         mainWindowThread.Name = "AppleWin";
         mainWindowThread.SetApartmentState(ApartmentState.STA);
         mainWindowThread.Start();

         // show it
         AppleWin.Managed.AppleWinThread.Show(true);

         // if we have a single command line parameter it's a program to run
         string[] commandLineArgs = Environment.GetCommandLineArgs();
         if (commandLineArgs.Length == 2)
         {
            try
            {
               ushort address;
               List<byte> program = new List<byte>();
               using (FileStream file = new FileStream(commandLineArgs[1], FileMode.Open, FileAccess.Read))
               {
                  int lowByte = file.ReadByte();
                  int highByte = file.ReadByte();
                  if (lowByte < 0 || highByte < 0)
                     throw new IOException(ProgramFileTooShortMessage);
                  address = (ushort)(lowByte + 256 * highByte);

                  for(; ; )
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

         // start a timer to monitor if it closes
         timer.Interval = TimeSpan.FromMilliseconds(100);
         timer.Tick += Timer_Tick;
         timer.IsEnabled = true;
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
      }
   }
}
