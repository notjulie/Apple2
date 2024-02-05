using A2DiskUtil.ViewModel;
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

         // set our view model
         DataContext = new MainViewModel();
      }

      /// <summary>
      /// Gets the main view model
      /// </summary>
      private MainViewModel? MainViewModel
      {
         get
         {
            return DataContext as MainViewModel;
         }
      }

      /// <summary>
      /// Handles the File>Open Disk... item
      /// </summary>
      /// <param name="sender"></param>
      /// <param name="e"></param>
      private void MenuitemOpenDisk_Click(object sender, RoutedEventArgs e)
      {
         MainViewModel?.OpenFile();
      }
   }
}