using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using AppleWin.Managed;
using Suna;
using Suna.Link;

namespace AppleDev
{
   /// <summary>
   /// Interaction logic for MainWindow.xaml
   /// </summary>
   public partial class MainWindow : Window
   {
      /// <summary>
      /// Initializes a new instance of class MainWindow
      /// </summary>
      public MainWindow()
      {
         // normal component initialization
         InitializeComponent();

         // event handlers
         windowMenu.SubmenuOpened += WindowMenu_SubmenuOpened;
         emulatorItem.Click += EmulatorItem_Click;
         programText.TextChanged += ProgramText_TextChanged;
         compileAndRunItem.Click += CompileAndRunItem_Click;

         // grab the program text from the settings
         programText.Text = AppleDev.Properties.Settings.Default.ProgramText;
      }

      /// <summary>
      /// Compiles and runs
      /// </summary>
      /// <param name="sender"></param>
      /// <param name="e"></param>
      private void CompileAndRunItem_Click(object sender, RoutedEventArgs e)
      {
         try
         {
            // create a source module from the text
            SourceModule sourceModule = new SourceModule(programText.Text);

            // compile it
            SunaCompiler compiler = new SunaCompiler();
            LinkedModule linkedModule = compiler.Compile(sourceModule);

            // Show the simulator if it's not
            AppleWinThread.Show(true);

            // enter monitor so that the Apple ][ is not using memory that we want
            // not used
            Machine.ExecuteProgram(linkedModule.GetImage(), linkedModule.BaseAddress);
         }
         catch (NotImplementedException niX)
         {
            MessageBox.Show("Feature not implemented: " + niX.Message);
         }
         catch (CompileException cX)
         {
            MessageBox.Show("Compile exception: " + cX.Message);
         }
      }

      private void ProgramText_TextChanged(object sender, EventArgs e)
      {
         AppleDev.Properties.Settings.Default.ProgramText = programText.Text;
      }

      private void EmulatorItem_Click(object sender, RoutedEventArgs e)
      {
         AppleWin.Managed.AppleWinThread.Show(!emulatorItem.IsChecked);
      }

      private void WindowMenu_SubmenuOpened(object sender, RoutedEventArgs e)
      {
         emulatorItem.IsChecked = AppleWin.Managed.AppleWinThread.IsShowing();
      }
   }
}
