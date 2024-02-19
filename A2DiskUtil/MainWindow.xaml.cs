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
         MainViewModel?.OpenDiskImageFile();
      }

      /// <summary>
      /// Previews the data that is being dragged in order to tell the
      /// system whether or not we would accept it
      /// </summary>
      /// <param name="e"></param>
      protected override void OnPreviewDragOver(DragEventArgs e)
      {
         base.OnPreviewDragOver(e);

         e.Effects = DragDropEffects.None;
         if (true == MainViewModel?.CanDropData(e.Data))
            e.Effects = DragDropEffects.Copy;
         e.Handled = true;
      }

      /// <summary>
      /// Accepts dropped data
      /// </summary>
      /// <param name="e"></param>
      protected override void OnDrop(DragEventArgs e)
      {
         // call the viewmodel
         MainViewModel?.DropData(e.Data);

         // call the base
         base.OnDrop(e);
      }

      #endregion
   }
}