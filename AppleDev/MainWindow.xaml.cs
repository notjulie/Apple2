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

using Suna;

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
         compileItem.Click += CompileItem_Click;

         // grab the program text from the settings
         programText.Text = AppleDev.Properties.Settings.Default.ProgramText;
      }

      private void CompileItem_Click(object sender, RoutedEventArgs e)
      {
         try
         {
            // create a source module from the text
            SourceModule sourceModule = new SourceModule(programText.Text);

            SunaCompiler compiler = new SunaCompiler();
            compiler.Compile(sourceModule);
         }
         catch (NotImplementedException niX)
         {
            MessageBox.Show("Feature not implemented: " + niX.Message);
         }
      }

      private void ProgramText_TextChanged(object sender, TextChangedEventArgs e)
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
