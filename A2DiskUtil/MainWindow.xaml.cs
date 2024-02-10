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
      #region Constructor

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

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the main view model
      /// </summary>
      public MainViewModel? MainViewModel
      {
         get
         {
            return DataContext as MainViewModel;
         }
      }

      #endregion

      #region Event Handlers

      /// <summary>
      /// Handles the File>Open Disk... item
      /// </summary>
      /// <param name="sender"></param>
      /// <param name="e"></param>
      private void MenuitemOpenDisk_Click(object sender, RoutedEventArgs e)
      {
         MainViewModel?.OpenFile();
      }

      protected override void OnPreviewDragOver(DragEventArgs e)
      {
         base.OnPreviewDragOver(e);

         e.Effects = DragDropEffects.None;
         e.Handled = true;

         bool isFile = e.Data.GetFormats().Contains("FileName");
         if (isFile)
         {
            string[]? data = e.Data.GetData("FileName") as string[];
            if (data != null && data.Length > 0)
            {
               if (MainViewModel?.CanDropFile(data[0]) == true)
                  e.Effects = DragDropEffects.Copy;
            }
         }
      }

      #endregion
   }
}