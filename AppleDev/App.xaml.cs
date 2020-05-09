using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
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
      private Thread mainWindowThread;

      protected override void OnStartup(StartupEventArgs e)
      {
         // call base class
         base.OnStartup(e);

         // launch the thread that runs AppleWin
         mainWindowThread = new Thread(AppleWin.Managed.AppleWinThread.Run);
         mainWindowThread.Name = "AppleWin";
         mainWindowThread.SetApartmentState(ApartmentState.STA);
         mainWindowThread.Start();
      }

      protected override void OnExit(ExitEventArgs e)
      {
         // call the base class
         base.OnExit(e);

         // tell AppleWin to shut down
         AppleWin.Managed.AppleWinThread.Shutdown();
         mainWindowThread.Join();
      }
   }
}
