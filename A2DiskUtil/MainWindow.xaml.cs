using Microsoft.Win32;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace A2DiskUtil
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

         // add event handlers
         menuitemOpenDisk.Click += MenuitemOpenDisk_Click;
      }

      /// <summary>
      /// Handles the File>Open Disk... item
      /// </summary>
      /// <param name="sender"></param>
      /// <param name="e"></param>
      private void MenuitemOpenDisk_Click(object sender, RoutedEventArgs e)
      {
         OpenFileDialog dialog = new OpenFileDialog();
         if (dialog.ShowDialog() == true)
         {
            try
            {
               DiskFile newFile = new DiskFile(dialog.FileName);
               DataContext = newFile;
            }
            catch (Exception ex) 
            {
               MessageBox.Show(ex.Message);
            }
         }
      }
   }
}